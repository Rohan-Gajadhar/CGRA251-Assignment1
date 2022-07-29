//
// Created by Rohan Gajadhar on 22/07/2022.
//
#include <vector>
#include <glm/glm.hpp>
#include "opengl.hpp"

class OBJFile{
    private:
        GLuint m_vao = 0;
        GLuint m_vbo_pos = 0;
        GLuint m_vbo_norm = 0;
        GLuint m_ibo = 0;

        std::vector <glm::vec3> orderedPositions;
        std::vector <glm::vec3> orderedNormals;
        std::vector<unsigned int> vertexIndices;

    public:
        void loadObj(char[]);

        void buildObj();

        void drawObj();

        void destroyObj();

        void printMeshDataObj();
};
