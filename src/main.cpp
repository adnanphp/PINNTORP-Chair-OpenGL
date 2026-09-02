



/**
 * @file main.cpp
 * @brief Complete OpenGL application for PINNTORP chair
 * 
 * Controls:
 *   ESC - Exit
 *   R   - Reset all transformations
 *   
 *   TRANSLATION:
 *   W/A/S/D - Move on X/Y axes
 *   Q/E     - Move on Z axis
 *   
 *   ROTATION:
 *   Arrow Left/Right - Rotate chair
 *   Arrow Up/Down   - Tilt chair
 *   
 *   SCALE:
 *   +/- - Scale up/down
 *   
 *   CAMERA:
 *   I/J/K/L - Orbit camera
 *   O/P     - Zoom in/out
 *   U       - Reset camera
 *   
 *   DISPLAY MODES:
 *   1 - Wireframe
 *   2 - Textured
 *   3 - Lit with textures
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "obj_loader.hpp"
#include "stb_image.h"

// ==================== WINDOW SETTINGS ====================
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// ==================== TRANSFORMATION VARIABLES ====================
glm::vec3 translation(0.0f, 0.0f, 0.0f);
glm::vec3 rotation(0.0f, 0.0f, 0.0f);
glm::vec3 scaleFactor(1.0f, 1.0f, 1.0f);

// ==================== CAMERA VARIABLES ====================
glm::vec3 cameraPos = glm::vec3(0.0f, 1.5f, 5.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.5f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float cameraZoom = 5.0f;
float cameraAngleX = 0.0f;
float cameraAngleY = 0.0f;

// ==================== LIGHTING ====================
glm::vec3 lightPos(2.0f, 3.0f, 2.0f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
glm::vec3 objectColor(0.78f, 0.6f, 0.4f); // Pine wood color

// ==================== RENDER MODE ====================
int renderMode = 2; // 0=wireframe, 1=textured, 2=lit

// ==================== TEXTURE IDs ====================
unsigned int textureDiffuse = 0;
unsigned int textureNormal = 0;

// ==================== SHADER PROGRAM ====================
unsigned int shaderProgram = 0;

// ==================== FUNCTION PROTOTYPES ====================
std::string readShaderFile(const char* filepath);
unsigned int compileShader(const std::string& source, GLenum type);
unsigned int createShaderProgram(const char* vertexPath, const char* fragmentPath);
unsigned int loadTexture(const char* path, bool isNormalMap = false);
void printControls();

// ==================== SHADER HELPERS ====================
std::string readShaderFile(const char* filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "ERROR: Could not open shader file: " << filepath << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

unsigned int compileShader(const std::string& source, GLenum type) {
    unsigned int shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "ERROR: Shader compilation failed\n" << infoLog << std::endl;
        return 0;
    }
    return shader;
}

unsigned int createShaderProgram(const char* vertexPath, const char* fragmentPath) {
    std::string vertexSource = readShaderFile(vertexPath);
    std::string fragmentSource = readShaderFile(fragmentPath);
    
    if (vertexSource.empty() || fragmentSource.empty()) return 0;

    unsigned int vertexShader = compileShader(vertexSource, GL_VERTEX_SHADER);
    unsigned int fragmentShader = compileShader(fragmentSource, GL_FRAGMENT_SHADER);
    
    if (vertexShader == 0 || fragmentShader == 0) return 0;

    unsigned int program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "ERROR: Shader program linking failed\n" << infoLog << std::endl;
        return 0;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return program;
}

// ==================== TEXTURE LOADING ====================
unsigned int loadTexture(const char* path, bool isNormalMap) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    
    int width, height, nrChannels;
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    
    if (data) {
        GLenum format;
        if (nrChannels == 1) format = GL_RED;
        else if (nrChannels == 3) format = GL_RGB;
        else if (nrChannels == 4) format = GL_RGBA;
        
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        std::cout << "SUCCESS: Loaded texture " << path << " (" << width << "x" << height << ")" << std::endl;
        stbi_image_free(data);
    } else {
        std::cerr << "ERROR: Failed to load texture: " << path << std::endl;
        stbi_image_free(data);
        return 0;
    }
    
    return textureID;
}

// ==================== CONTROLS DISPLAY ====================
void printControls() {
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║          PINNTORP CHAIR - COMPLETE CONTROLS                ║\n";
    std::cout << "╠══════════════════════════════════════════════════════════════╣\n";
    std::cout << "║                                                              ║\n";
    std::cout << "║  TRANSFORMATIONS:                                            ║\n";
    std::cout << "║    W/A/S/D     - Move chair (Up/Left/Down/Right)            ║\n";
    std::cout << "║    Q/E         - Move chair (Backward/Forward)              ║\n";
    std::cout << "║    Arrow Keys  - Rotate chair                               ║\n";
    std::cout << "║    +/-         - Scale chair (Up/Down)                     ║\n";
    std::cout << "║    R           - Reset all transformations                  ║\n";
    std::cout << "║                                                              ║\n";
    std::cout << "║  CAMERA CONTROLS:                                            ║\n";
    std::cout << "║    I/J/K/L     - Orbit camera                               ║\n";
    std::cout << "║    O/P         - Zoom (Out/In)                             ║\n";
    std::cout << "║    U           - Reset camera                              ║\n";
    std::cout << "║                                                              ║\n";
    std::cout << "║  DISPLAY MODES:                                              ║\n";
    std::cout << "║    1           - Wireframe mode                            ║\n";
    std::cout << "║    2           - Textured mode                             ║\n";
    std::cout << "║    3           - Lit mode (with textures)                  ║\n";
    std::cout << "║                                                              ║\n";
    std::cout << "║  OTHER:                                                     ║\n";
    std::cout << "║    ESC         - Exit application                          ║\n";
    std::cout << "║                                                              ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════╝\n";
    std::cout << "\n";
}

// ==================== KEYBOARD CALLBACK ====================
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        // ========== EXIT ==========
        if (key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window, true);
        }
        
        // ========== RESET ==========
        if (key == GLFW_KEY_R && action == GLFW_PRESS) {
            translation = glm::vec3(0.0f, 0.0f, 0.0f);
            rotation = glm::vec3(0.0f, 0.0f, 0.0f);
            scaleFactor = glm::vec3(1.0f, 1.0f, 1.0f);
            std::cout << "TRANSFORMATIONS RESET\n";
        }
        
        // ========== TRANSLATION ==========
        const float TRANS_SPEED = 0.05f;
        if (key == GLFW_KEY_W) translation.y += TRANS_SPEED;
        if (key == GLFW_KEY_S) translation.y -= TRANS_SPEED;
        if (key == GLFW_KEY_A) translation.x -= TRANS_SPEED;
        if (key == GLFW_KEY_D) translation.x += TRANS_SPEED;
        if (key == GLFW_KEY_Q) translation.z -= TRANS_SPEED;
        if (key == GLFW_KEY_E) translation.z += TRANS_SPEED;
        
        // ========== ROTATION ==========
        const float ROT_SPEED = 0.03f;
        if (key == GLFW_KEY_LEFT) rotation.y -= ROT_SPEED;
        if (key == GLFW_KEY_RIGHT) rotation.y += ROT_SPEED;
        if (key == GLFW_KEY_UP) rotation.x -= ROT_SPEED;
        if (key == GLFW_KEY_DOWN) rotation.x += ROT_SPEED;
        
        // ========== SCALE ==========
        const float SCALE_STEP = 0.05f;
        if (key == GLFW_KEY_EQUAL || key == GLFW_KEY_KP_ADD) {
            scaleFactor *= (1.0f + SCALE_STEP);
        }
        if (key == GLFW_KEY_MINUS || key == GLFW_KEY_KP_SUBTRACT) {
            scaleFactor /= (1.0f + SCALE_STEP);
        }
        
        // ========== CAMERA ORBIT ==========
        const float CAM_SPEED = 0.05f;
        if (key == GLFW_KEY_I) cameraAngleX += CAM_SPEED;
        if (key == GLFW_KEY_K) cameraAngleX -= CAM_SPEED;
        if (key == GLFW_KEY_J) cameraAngleY -= CAM_SPEED;
        if (key == GLFW_KEY_L) cameraAngleY += CAM_SPEED;
        
        // ========== CAMERA ZOOM ==========
        const float ZOOM_SPEED = 0.2f;
        if (key == GLFW_KEY_O) cameraZoom += ZOOM_SPEED;
        if (key == GLFW_KEY_P) cameraZoom -= ZOOM_SPEED;
        if (cameraZoom < 1.0f) cameraZoom = 1.0f;
        if (cameraZoom > 20.0f) cameraZoom = 20.0f;
        
        // ========== CAMERA RESET ==========
        if (key == GLFW_KEY_U && action == GLFW_PRESS) {
            cameraAngleX = 0.0f;
            cameraAngleY = 0.0f;
            cameraZoom = 5.0f;
            std::cout << "CAMERA RESET\n";
        }
        
        // ========== RENDER MODES ==========
        if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
            renderMode = 0;
            std::cout << "MODE: Wireframe\n";
        }
        if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
            renderMode = 1;
            std::cout << "MODE: Textured\n";
        }
        if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
            renderMode = 2;
            std::cout << "MODE: Lit with textures\n";
        }
    }
}

// ==================== UPDATE CAMERA ====================
void updateCamera() {
    // Calculate camera position from angle and zoom
    float radius = cameraZoom;
    float theta = cameraAngleX;
    float phi = cameraAngleY;
    
    cameraPos.x = radius * sin(theta) * cos(phi);
    cameraPos.y = radius * sin(phi) + 1.0f;
    cameraPos.z = radius * cos(theta) * cos(phi);
    
    cameraTarget = glm::vec3(translation.x, 0.5f + translation.y, translation.z);
}

// ==================== MAIN ====================
int main() {
    // ========== 1. Initialize GLFW ==========
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // ========== 2. Create Window ==========
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "PINNTORP Chair - Complete Controls", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "ERROR: Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    // ========== 3. Initialize GLAD ==========
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "ERROR: Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // ========== 4. Print Controls ==========
    printControls();

    // ========== 5. Create Shader Program ==========
    shaderProgram = createShaderProgram("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");
    if (shaderProgram == 0) return -1;

    // ========== 6. Load Textures ==========
    textureDiffuse = loadTexture("assets/models/plank_flooring_02_diff_1k.jpg", false);
    textureNormal = loadTexture("assets/models/plank_flooring_02_nor_gl_1k.png", true);
    if (textureDiffuse == 0) {
        std::cout << "WARNING: Diffuse texture not loaded, using default color\n";
    }

    // ========== 7. Load Model ==========
    MeshData chair;
    if (!loadOBJ("assets/models/pinntorp_chair.obj", chair)) {
        std::cerr << "ERROR: Failed to load chair model" << std::endl;
        return -1;
    }

    // ========== 8. Set Up OpenGL Buffers ==========
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 
                 (chair.vertices.size() + chair.normals.size() + chair.uvs.size()) * sizeof(glm::vec3),
                 nullptr, GL_STATIC_DRAW);

    // Upload vertices
    glBufferSubData(GL_ARRAY_BUFFER, 0, chair.vertices.size() * sizeof(glm::vec3), chair.vertices.data());
    size_t offset = chair.vertices.size() * sizeof(glm::vec3);
    
    // Upload normals
    glBufferSubData(GL_ARRAY_BUFFER, offset, chair.normals.size() * sizeof(glm::vec3), chair.normals.data());
    size_t normalOffset = offset;
    offset += chair.normals.size() * sizeof(glm::vec3);
    
    // Upload UVs
    glBufferSubData(GL_ARRAY_BUFFER, offset, chair.uvs.size() * sizeof(glm::vec2), chair.uvs.data());
    size_t uvOffset = offset;

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)normalOffset);
    glEnableVertexAttribArray(1);
    
    // UV attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)uvOffset);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, chair.indices.size() * sizeof(unsigned int), 
                 chair.indices.data(), GL_STATIC_DRAW);

    // ========== 9. Enable Features ==========
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // ========== 10. Main Render Loop ==========
    while (!glfwWindowShouldClose(window)) {
        // Update camera
        updateCamera();
        
        // Clear screen
        glClearColor(0.15f, 0.15f, 0.18f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // ========== SET MATRICES ==========
        glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 
                                                (float)SCR_WIDTH / (float)SCR_HEIGHT, 
                                                0.1f, 100.0f);

        // ========== MODEL MATRIX (CORRECT ORDER!) ==========
        // For object transformations, the correct order is:
        // 1. Scale (at origin)
        // 2. Rotate (around origin)
        // 3. Translate (move to final position)
        //
        // This ensures that scaling and rotation happen around the object's center
        // and translation moves the entire object without affecting rotation/scale.
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, scaleFactor);                    // Scale FIRST
        model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate Y
        model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate X
        model = glm::translate(model, translation);               // Translate LAST

        // ========== SET UNIFORMS ==========
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        
        // Lighting
        glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 1, glm::value_ptr(lightPos));
        glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, glm::value_ptr(cameraPos));
        glUniform3fv(glGetUniformLocation(shaderProgram, "lightColor"), 1, glm::value_ptr(lightColor));
        glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(objectColor));
        
        // Textures
        glUniform1i(glGetUniformLocation(shaderProgram, "texture_diffuse"), 0);
        glUniform1i(glGetUniformLocation(shaderProgram, "texture_normal"), 1);
        
        // Render mode
        glUniform1i(glGetUniformLocation(shaderProgram, "renderMode"), renderMode);

        // ========== BIND TEXTURES ==========
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureDiffuse);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureNormal);

        // ========== DRAW ==========
        if (renderMode == 0) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glLineWidth(1.5f);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, chair.indices.size(), GL_UNSIGNED_INT, 0);

        // Reset polygon mode
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // ========== 11. Cleanup ==========
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
    glDeleteTextures(1, &textureDiffuse);
    glDeleteTextures(1, &textureNormal);

    glfwTerminate();
    std::cout << "Application closed.\n";
    return 0;
}
