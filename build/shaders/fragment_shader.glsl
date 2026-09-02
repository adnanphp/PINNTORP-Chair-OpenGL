#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform sampler2D texture_diffuse;
uniform sampler2D texture_normal;
uniform int renderMode; // 0=wireframe, 1=textured, 2=lit

out vec4 FragColor;

void main() {
    // ========== WIREFRAME MODE ==========
    if (renderMode == 0) {
        FragColor = vec4(objectColor, 1.0);
        return;
    }
    
    // ========== TEXTURED MODE ==========
    vec4 texColor = texture(texture_diffuse, TexCoord);
    if (renderMode == 1) {
        FragColor = texColor;
        return;
    }
    
    // ========== LIT MODE ==========
    // Ambient
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor;
    
    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;
    
    // Combine
    vec3 result = (ambient + diffuse + specular) * vec3(texColor);
    FragColor = vec4(result, 1.0);
}
