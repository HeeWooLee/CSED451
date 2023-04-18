#pragma once
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

struct Vertex {
    glm::vec3 position;
};
struct Model {
    std::vector<Vertex> vertex_positions;
    std::vector<GLuint> face_position_indices;
};

Model loadObjFile(const std::string& filename);
void sendDataToOpenGL(Model* model,
    GLuint* VAO,
    GLuint* VBO,
    GLuint* EBO);

void draw(GLuint* VAO, Model* obj);
