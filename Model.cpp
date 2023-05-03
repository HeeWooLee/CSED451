#include "Model.h"
#include <iostream>
#include <fstream>
#include <string>

#include "Game.h"
extern Game* game;

/* model size and center checker */
float minX = FLT_MAX, minY = FLT_MAX, minZ = FLT_MAX;
float maxX = -FLT_MAX, maxY = -FLT_MAX, maxZ = -FLT_MAX;

struct Vertex {
    vec3 position;
};

Model::Model() {
     load("meshes/character_pose1.obj", character[0], characterInfo[0]);
     // load("meshes/character_pose2.obj", character[1], characterInfo[1]);
     // load("meshes/character_pose3.obj", character[2], characterInfo[2]);
     load("meshes/fireball.obj", fireball, fireballInfo);
     //  load("meshes/star.obj", star, starInfo);
     load("meshes/Mushroom.obj", mushroom, mushroomInfo);
     load("meshes/cube.obj", cube, cubeInfo);
}

void Model::load (const string& filename, GLuint* buf, vec3* info)
{
    vector<Vertex> vertex_positions;
    vector<vec2> texture_coordinates;
    vector<vec3> vertex_normals;
    vector<GLuint> face_position_indices;
    vector<vec3> face_texture_indices;
    vector<GLuint> face_normal_indices;

    ifstream ifs(filename);
    if (!ifs.is_open()) {
        throw runtime_error("Failed to open file " + filename);
    }

    string line;
    while (getline(ifs, line)) {
        stringstream ss(line);
        string keyword;
        ss >> keyword;

        if (keyword == "v") {
            vec3 pos;
            ss >> pos.x >> pos.y >> pos.z;
            Vertex vertex;
            /* match character mesh into one place */
            if (filename == "meshes/character_pose2.obj") {
                pos.x -= 103.712;
            }
            else if (filename == "meshes/character_pose3.obj") {
                pos.x -= 230.289;
            }
            vertex.position = pos;
            vertex_positions.emplace_back(vertex);
            /*for (int i = 0; i < 3; i++) {
                vertex_positions.emplace_back(pos[i]);
            }*/
        }
        else if (keyword == "vt") {
            vec2 tex_coord;
            ss >> tex_coord.x >> tex_coord.y;
            texture_coordinates.emplace_back(tex_coord);
        }
        else if (keyword == "vn") {
            vec3 normal;
            ss >> normal.x >> normal.y >> normal.z;
            vertex_normals.emplace_back(normal);
        }
        else if (keyword == "f") {
            string face_data;
            vector<string> vertex_data;

            // split the face data by spaces
            while (ss >> face_data) {
                vertex_data.emplace_back(face_data);
            }

            vec3 face_pos_idx, face_norm_idx, face_tex_idx;

            for (int i = 0; i < vertex_data.size(); i++) {
                stringstream data_ss(vertex_data[i]);
                string token;
                vector<string> index_data;

                // split the vertex data by '/'
                while (getline(data_ss, token, '/')) {
                    index_data.emplace_back(token);
                }

                // vertex position index
                face_pos_idx[i] = stoi(index_data[0]) - 1;

                // vertex texture index
                face_tex_idx[i] = stoi(index_data[1]) - 1;

                // vertex normal index
                face_norm_idx[i] = stoi(index_data[2]) - 1;
            }

            for (int i = 0; i < 3; i++) {
                face_position_indices.emplace_back(face_pos_idx[i]);
                face_texture_indices.emplace_back(face_tex_idx[i]);
                face_normal_indices.emplace_back(face_norm_idx[i]);
            }
        }
    }

    ifs.close();

    /* check model center & size */
    for (int i = 0; i < vertex_positions.size(); i++) {
        vec3 pos = vertex_positions[i].position;
        if (pos.x < minX) minX = pos.x;
        if (pos.y < minY) minY = pos.y;
        if (pos.z < minZ) minZ = pos.z;
        if (pos.x > maxX) maxX = pos.x;
        if (pos.y > maxY) maxY = pos.y;
        if (pos.z > maxZ) maxZ = pos.z;
    }

    vec3 modelCenter = vec3((minX + maxX) / 2.0f, (minY + maxY) / 2.0f, (minZ + maxZ) / 2.0f);
    vec3 modelSize = vec3(maxX - minX, maxY - minY, maxZ - minZ);
    
    info[0] = game->scale * modelCenter;
    info[1] = game->scale * modelSize;

    cout << "FILENAME: " << filename << endl;
    cout << "vertex_positions size: " << vertex_positions.size() << endl;
    cout << "texture_coordinates size: " << texture_coordinates.size() << endl;
    cout << "vertex_normals size: " << vertex_normals.size() << endl;
    cout << "face_position_indices size: " << face_position_indices.size() << endl;
    cout << "face_texture_indices size: " << face_texture_indices.size() << endl;
    cout << "face_normal_indices size: " << face_normal_indices.size() << endl;
    cout << endl;


    glGenBuffers(1, &buf[0]);
    glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
    glBufferData(GL_ARRAY_BUFFER, vertex_positions.size() * sizeof(Vertex), &vertex_positions[0], GL_STATIC_DRAW);

    glGenBuffers(1, &buf[1]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, face_position_indices.size() * sizeof(GLuint), &face_position_indices[0], GL_STATIC_DRAW);

    if (buf[0] == 0) {
        cout << filename << " VBO genertaion FAIL" << endl;
    }
    if (buf[1] == 0) {
        cout << filename << " EBO genertaion FAIL" << endl;
    }

    buf[2] = face_position_indices.size();

    vertex_positions.clear();
    texture_coordinates.clear();
    vertex_normals.clear();
    face_position_indices.clear();
    face_texture_indices.clear();
    face_normal_indices.clear();

    /* reinitialize size checker */
    minX = FLT_MAX, minY = FLT_MAX, minZ = FLT_MAX;
    maxX = -FLT_MAX, maxY = -FLT_MAX, maxZ = -FLT_MAX;
}