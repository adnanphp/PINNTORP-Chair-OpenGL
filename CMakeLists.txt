

cmake_minimum_required(VERSION 3.10)
project(OpenGLChair)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# ==================== SOURCE FILES ====================
set(SOURCES
    src/main.cpp
    src/obj_loader.cpp
    src/glad.c
    src/stb_image.cpp          # ← ADD THIS
)

add_executable(chair_app ${SOURCES})

# ==================== INCLUDE DIRECTORIES ====================
target_include_directories(chair_app PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
    /usr/include/glm
    /usr/local/include/glm
)

# ==================== LINK LIBRARIES ====================
target_link_libraries(chair_app
    OpenGL::GL
    glfw
    dl
)

# ==================== COPY ASSETS ====================
add_custom_command(TARGET chair_app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:chair_app>/assets
    COMMENT "Copying assets to build directory"
)

add_custom_command(TARGET chair_app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders
    $<TARGET_FILE_DIR:chair_app>/shaders
    COMMENT "Copying shaders to build directory"
)
