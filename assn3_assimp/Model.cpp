#include "Model.h"

Model loadObjFile(const std::string& filename)
{
    std::vector<Vertex> vertex_positions;
    std::vector<glm::vec2> texture_coordinates;
    std::vector<glm::vec3> vertex_normals;
    std::vector<GLuint> face_position_indices;
    std::vector<glm::vec3> face_texture_indices;
    std::vector<GLuint> face_normal_indices;

    Model model;

    std::ifstream ifs(filename);
    if (!ifs.is_open()) {
        throw std::runtime_error("Failed to open file " + filename);
    }

    std::string line;
    while (std::getline(ifs, line)) {
        std::stringstream ss(line);
        std::string keyword;
        ss >> keyword;

        if (keyword == "v") {
            glm::vec3 pos;
            ss >> pos.x >> pos.y >> pos.z;
            Vertex vertex;
            vertex.position = pos;
            vertex_positions.emplace_back(vertex);
            /*for (int i = 0; i < 3; i++) {
                vertex_positions.emplace_back(pos[i]);
            }*/
        }
        else if (keyword == "vt") {
            glm::vec2 tex_coord;
            ss >> tex_coord.x >> tex_coord.y;
            texture_coordinates.emplace_back(tex_coord);
        }
        else if (keyword == "vn") {
            glm::vec3 normal;
            ss >> normal.x >> normal.y >> normal.z;
            vertex_normals.emplace_back(normal);
        }
        else if (keyword == "f") {
            std::string face_data;
            std::vector<std::string> vertex_data;

            // split the face data by spaces
            while (ss >> face_data) {
                vertex_data.emplace_back(face_data);
            }

            glm::vec3 face_pos_idx, face_norm_idx, face_tex_idx;

            for (int i = 0; i < vertex_data.size(); i++) {
                std::stringstream data_ss(vertex_data[i]);
                std::string token;
                std::vector<std::string> index_data;

                // split the vertex data by '/'
                while (std::getline(data_ss, token, '/')) {
                    index_data.emplace_back(token);
                }

                // vertex position index
                face_pos_idx[i] = std::stoi(index_data[0]) - 1;

                // vertex texture index
                face_tex_idx[i] = std::stoi(index_data[1]) - 1;

                // vertex normal index
                face_norm_idx[i] = std::stoi(index_data[2]) - 1;
            }

            for (int i = 0; i < 3; i++) {
                face_position_indices.emplace_back(face_pos_idx[i]);
                face_texture_indices.emplace_back(face_tex_idx[i]);
                face_normal_indices.emplace_back(face_norm_idx[i]);
            }
        }
    }

    ifs.close();

    model.face_position_indices = face_position_indices;
    model.vertex_positions = vertex_positions;

    return model;
}


void sendDataToOpenGL(Model* model,
    GLuint* VAO,
    GLuint* VBO,
    GLuint* EBO)
{
    //TODO:
    //Load objects and bind to VAO & VBO
    //Load texture
    Model obj = *model;
    glGenVertexArrays(1, VAO);
    glBindVertexArray(*VAO);
    //Create Vertex Buffer Objects
    glGenBuffers(1, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, obj.vertex_positions.size() * sizeof(Vertex), &obj.vertex_positions[0], GL_STATIC_DRAW);
    //Create Element array Buffer Objects
    glGenBuffers(1, EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj.face_position_indices.size() * sizeof(GLuint), &obj.face_position_indices[0], GL_STATIC_DRAW);
    // 1st attribute buffer : position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0, // attribute
        3, // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        sizeof(Vertex), // stride
        (void*)offsetof(Vertex, position) // array buffer offset
    );
}

void draw(GLuint* VAO, Model* obj) {
    glBindVertexArray(*VAO);

    glDrawElements(GL_TRIANGLES, (*obj).face_position_indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

