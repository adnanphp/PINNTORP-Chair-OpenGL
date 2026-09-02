


#ifndef OBJ_LOADER_HPP
#define OBJ_LOADER_HPP

#include <vector>
#include <glm/glm.hpp>

/**
 * @brief Structure to hold mesh data loaded from an OBJ file
 */
struct MeshData {
    std::vector<glm::vec3> vertices;  // Vertex positions (x, y, z)
    std::vector<glm::vec3> normals;   // Vertex normals (nx, ny, nz)
    std::vector<glm::vec2> uvs;       // Texture coordinates (u, v)
    std::vector<unsigned int> indices; // Indices for indexed drawing
};

/**
 * @brief Load an OBJ file from disk
 * @param path Path to the .obj file
 * @param outMesh Reference to MeshData structure to fill
 * @return true if loaded successfully, false otherwise
 */
bool loadOBJ(const char* path, MeshData& outMesh);

#endif // OBJ_LOADER_HPP
