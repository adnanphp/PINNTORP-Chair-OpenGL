




#include "obj_loader.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <iostream>

bool loadOBJ(const char* path, MeshData& outMesh) {
    // Open the OBJ file
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "ERROR: Failed to open OBJ file: " << path << std::endl;
        return false;
    }

    // Temporary storage for raw data
    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec2> temp_uvs;
    std::vector<glm::vec3> temp_normals;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        // Parse vertex position: v x y z
        if (prefix == "v") {
            glm::vec3 vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            temp_vertices.push_back(vertex);
        }
        // Parse texture coordinate: vt u v
        else if (prefix == "vt") {
            glm::vec2 uv;
            iss >> uv.x >> uv.y;
            temp_uvs.push_back(uv);
        }
        // Parse vertex normal: vn nx ny nz
        else if (prefix == "vn") {
            glm::vec3 normal;
            iss >> normal.x >> normal.y >> normal.z;
            temp_normals.push_back(normal);
        }
        // Parse face: f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3
        else if (prefix == "f") {
            std::string v1, v2, v3;
            iss >> v1 >> v2 >> v3;

            // Helper lambda to process a single face token
            auto processFace = [&](const std::string& token) {
                std::string t = token;
                std::replace(t.begin(), t.end(), '/', ' ');
                std::istringstream fss(t);
                int v, t_idx, n;
                fss >> v >> t_idx >> n;

                // Handle negative indices (relative to end)
                if (v < 0) v = temp_vertices.size() + v + 1;
                if (t_idx < 0) t_idx = temp_uvs.size() + t_idx + 1;
                if (n < 0) n = temp_normals.size() + n + 1;

                // Add vertex data (convert to 0-based indexing)
                outMesh.vertices.push_back(temp_vertices[v - 1]);
                if (!temp_uvs.empty()) {
                    outMesh.uvs.push_back(temp_uvs[t_idx - 1]);
                }
                if (!temp_normals.empty()) {
                    outMesh.normals.push_back(temp_normals[n - 1]);
                }
                // Add index for this vertex
                outMesh.indices.push_back(outMesh.indices.size());
            };

            // Process all three vertices of the triangle
            processFace(v1);
            processFace(v2);
            processFace(v3);
        }
    }

    file.close();
    std::cout << "SUCCESS: Loaded " << outMesh.vertices.size() << " vertices from " << path << std::endl;
    return true;
}
