#include "object2D.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <glm/glm.hpp>
#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

using namespace std;

namespace object2D {
    Mesh* CreateTerrain2(const std::string& name, unsigned int m, unsigned int n) {
        std::vector<VertexFormat> vertices;
        std::vector<unsigned int> indices;

        float width = 50.0f;
        float height = 50.0f;
        float halfWidth = width / 2.0f;
        float halfHeight = height / 2.0f;

        // Generate vertices
        for (unsigned int i = 0; i <= m; ++i) {
            for (unsigned int j = 0; j <= n; ++j) {
                float x = width * (float)j / n - halfWidth; // Center x
                float z = height * (float)i / m - halfHeight; // Center z
                glm::vec3 texCoord = glm::vec3((float)j / n, (float)i / m, 0.0f);
                vertices.emplace_back(glm::vec3(x, 0.0f, z), glm::vec3(0.0f, 1.0f, 0.0f), texCoord);
            }
        }

        // Generate indices
        for (unsigned int i = 0; i < m; ++i) {
            for (unsigned int j = 0; j < n; ++j) {
                unsigned int topLeft = i * (n + 1) + j;
                unsigned int topRight = topLeft + 1;
                unsigned int bottomLeft = (i + 1) * (n + 1) + j;
                unsigned int bottomRight = bottomLeft + 1;

                // First triangle
                indices.push_back(topLeft);
                indices.push_back(bottomLeft);
                indices.push_back(topRight);

                // Second triangle
                indices.push_back(topRight);
                indices.push_back(bottomLeft);
                indices.push_back(bottomRight);
            }
        }

        Mesh* terrain = new Mesh(name);
        terrain->SetDrawMode(GL_TRIANGLES);
        terrain->InitFromData(vertices, indices);
        return terrain;
    }

    Mesh* CreateBalloonBase(const std::string& name, glm::vec3 color) {
        std::vector<VertexFormat> vertices;
        std::vector<unsigned int> indices;

        // Box vertices
        glm::vec3 boxVertices[] = {
            glm::vec3(-1, 0.0f, -0.5f), // 0
            glm::vec3(1, 0.0f, -0.5f), // 1
            glm::vec3(1, 0.0f, 0.5f), // 2
            glm::vec3(-1, 0.0f, 0.5f), // 3
            glm::vec3(-1, 0.5f, -0.5f), // 4
            glm::vec3(1, 0.5f, -0.5f), // 5
            glm::vec3(1, 0.5f, 0.5f), // 6
            glm::vec3(-1, 0.5f, 0.5f), // 7
        };

        // Box indices
        unsigned int boxIndices[] = {
            0, 1, 2,
            0, 2, 3,
            0, 4, 5,
            0, 5, 1,
            1, 5, 6,
            1, 6, 2,
            2, 6, 7,
            2, 7, 3,
            3, 7, 4,
            3, 4, 0,
        };

        // Sticks vertices - sticks will be 1.5 units tall and 0.1 units wide
        glm::vec3 stickVertices[] = {
            // Stick 1
            glm::vec3(-1.0f, 0.5f, -0.5f), // 8
            glm::vec3(-1.0f, 2.0f, -0.5f), // 9
            glm::vec3(-0.9f, 0.5f, -0.5f), // 10
            glm::vec3(-0.9f, 2.0f, -0.5f), // 11
            glm::vec3(-1.0f, 0.5f, -0.4f), // 12
            glm::vec3(-1.0f, 2.0f, -0.4f), // 13
            glm::vec3(-0.9f, 0.5f, -0.4f), // 14
            glm::vec3(-0.9f, 2.0f, -0.4f), // 15

            // Stick 2
            glm::vec3(1.0f, 0.5f, -0.5f), // 16
            glm::vec3(1.0f, 2.0f, -0.5f), // 17
            glm::vec3(0.9f, 0.5f, -0.5f), // 18
            glm::vec3(0.9f, 2.0f, -0.5f), // 19
            glm::vec3(1.0f, 0.5f, -0.4f), // 20
            glm::vec3(1.0f, 2.0f, -0.4f), // 21
            glm::vec3(0.9f, 0.5f, -0.4f), // 22
            glm::vec3(0.9f, 2.0f, -0.4f), // 23

            // Stick 3
            glm::vec3(1.0f, 0.5f, 0.5f), // 24
            glm::vec3(1.0f, 2.0f, 0.5f), // 25
            glm::vec3(0.9f, 0.5f, 0.5f), // 26
            glm::vec3(0.9f, 2.0f, 0.5f), // 27
            glm::vec3(1.0f, 0.5f, 0.4f), // 28
            glm::vec3(1.0f, 2.0f, 0.4f), // 29
            glm::vec3(0.9f, 0.5f, 0.4f), // 30
            glm::vec3(0.9f, 2.0f, 0.4f), // 31

            // Stick 4
            glm::vec3(-1.0f, 0.5f, 0.5f), // 32
            glm::vec3(-1.0f, 2.0f, 0.5f), // 33
            glm::vec3(-0.9f, 0.5f, 0.5f), // 34
            glm::vec3(-0.9f, 2.0f, 0.5f), // 35
            glm::vec3(-1.0f, 0.5f, 0.4f), // 36
            glm::vec3(-1.0f, 2.0f, 0.4f), // 37
            glm::vec3(-0.9f, 0.5f, 0.4f), // 38
            glm::vec3(-0.9f, 2.0f, 0.4f), // 39
        };

        // Sticks indices - only the 4 long faces of the sticks
        unsigned int stickIndices[] = {
            // Stick 1
            8, 9, 10,
            9, 10, 11,
            12, 13, 14,
            13, 14, 15,
            8, 9, 12,
            9, 12, 13,
            10, 11, 14,
            11, 14, 15,

            // Stick 2
            16, 17, 18,
            17, 18, 19,
            20, 21, 22,
            21, 22, 23,
            16, 17, 20,
            17, 20, 21,
            18, 19, 22,
            19, 22, 23,

            // Stick 3
            24, 25, 26,
            25, 26, 27,
            28, 29, 30,
            29, 30, 31,
            24, 25, 28,
            25, 28, 29,
            26, 27, 30,
            27, 30, 31,

            // Stick 4
            32, 33, 34,
            33, 34, 35,
            36, 37, 38,
            37, 38, 39,
            32, 33, 36,
            33, 36, 37,
            34, 35, 38,
            35, 38, 39,
        };

        // Add vertices and indices to the mesh
        vertices.insert(vertices.end(), std::begin(boxVertices), std::end(boxVertices));
        vertices.insert(vertices.end(), std::begin(stickVertices), std::end(stickVertices));
        indices.insert(indices.end(), std::begin(boxIndices), std::end(boxIndices));
        indices.insert(indices.end(), std::begin(stickIndices), std::end(stickIndices));

        Mesh* balloonBase = new Mesh(name);
        balloonBase->SetDrawMode(GL_TRIANGLES);
        balloonBase->InitFromData(vertices, indices);
        return balloonBase;
    }
}  // namespace object2D