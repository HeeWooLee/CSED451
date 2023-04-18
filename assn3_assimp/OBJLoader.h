#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

void loadObjFile(const std::string& filename,
    std::vector<glm::vec3>& vertex_positions,
    std::vector<glm::vec2>& texture_coordinates,
    std::vector<glm::vec3>& vertex_normals,
    std::vector<GLuint>& face_position_indices,
    std::vector<glm::vec3>& face_texture_indices,
    std::vector<GLuint>& face_normal_indices)
{
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
            vertex_positions.emplace_back(pos);
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
}