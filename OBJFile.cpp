//
// Created by Rohan Gajadhar on 22/07/2022.
//

#include "OBJFile.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>

void OBJFile::loadObj(char filepath[521]) {
    std::vector<glm::vec3> tempPositions;
    std::vector<glm::vec3> tempNormals;
    std::ifstream file(filepath);
    for (int i =0; i < 4; i++) { // Skip first 4 lines
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    char lineData[100];
    while (file.peek() != EOF) {
        file.getline(lineData, 100);
        std::stringstream stream;
        stream << lineData;
        char type[32];
        stream.getline(type, 32, ' ');

        if (std::string(type) == "v"){
            glm::vec3 position;
            stream >> position.x >> position.y >> position.z;
            tempPositions.push_back(position);
        }

        else if (std::string(type) == "vn"){
            glm::vec3 normal;
            stream >> normal.x >> normal.y >> normal.z;
            tempNormals.push_back(normal);
        }

        else if (std::string(type) == "f") {
            char vertices[100];
            for (int i = 0; i < 3; i++) {
                char vertexData[100];
                int x1, x2, x3;
                stream >> vertices;
                std::stringstream vertexStream;
                vertexStream << vertices;

                vertexStream.getline(vertexData, 30, '/');
                x1 = std::stoi(vertexData);
                vertexStream >> x2;
                vertexStream >> vertexData;
                x3 = std::stoi(std::string(vertexData).substr(1));
                orderedPositions.push_back(tempPositions.at(x1-1));
                orderedNormals.push_back(tempNormals.at(x3-1));
            }
        }
    }
    for (int i = 0; i < orderedPositions.size(); i++) {
        vertexIndices.push_back(i);
    }
}

void OBJFile::buildObj(){
    if (m_vao == 0) {
        glGenVertexArrays(1, &m_vao);
        glGenBuffers(1, &m_vbo_pos);
        glGenBuffers(1, &m_vbo_norm);
        glGenBuffers(1, &m_ibo);

        glBindVertexArray(m_vao);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo_pos);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * orderedPositions.size(), orderedPositions.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo_norm);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * orderedNormals.size(), orderedNormals.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * vertexIndices.size(), vertexIndices.data(), GL_STATIC_DRAW);

        glBindVertexArray(0);
    }
}

void OBJFile::drawObj() {
    if (m_vao == 0) return;
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, vertexIndices.size(), GL_UNSIGNED_INT, 0);
    
}

void OBJFile::destroyObj() {
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo_pos);
    glDeleteBuffers(1, &m_vbo_norm);
    glDeleteBuffers(1, &m_ibo);
    m_vao = 0;
}

void OBJFile::printMeshDataObj(){
    for(int i = 0; i < orderedPositions.size(); i++){
        std::cout << "Position: " << orderedPositions.at(i).x << " " << orderedPositions.at(i).y << " " << orderedPositions.at(i).z << " " <<
        "Normal: " << orderedNormals.at(i).x << " " << orderedNormals.at(i).y << " " << orderedNormals.at(i).z << " " <<
        "Vertex: " << vertexIndices.at(i) << std::endl;
    }
}