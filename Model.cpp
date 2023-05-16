#include "Model.h"
#include <iostream>
#include <fstream>
#include <string>

#include "Game.h"
extern Game* game;

/* model size and center checker */
float minX = FLT_MAX, minY = FLT_MAX, minZ = FLT_MAX;
float maxX = -FLT_MAX, maxY = -FLT_MAX, maxZ = -FLT_MAX;

vec3 purple = (0.5, 0.0, 0.5);
vec3 yellow = (1.0, 1.0, 0.0);
vec3 red = (1.0, 0.0, 0.0);
vec3 green = (0.0, 1.0, 0.0);
vec3 brown = (0.647, 0.165, 0.165);


Model::Model() {
     load("meshes/character_pose1.obj", character[0], characterInfo[0], purple);
     load("meshes/character_pose2.obj", character[1], characterInfo[1], purple);
     load("meshes/character_pose3.obj", character[2], characterInfo[2], purple);

     normalmap_fireball.load("textured_mesh/fire_ball/M_ContentWindow_N.bmp");
     load("meshes/fireball.obj", fireball, fireballInfo, red);
     texture_fireball[0].load("textured_mesh/fire_ball/M_ContentWindow_D.bmp");
     texture_fireball[1].load("textured_mesh/fire_ball/M_ContentWindow_S.bmp");

     load("meshes/star.obj", star, starInfo, yellow);

     load("meshes/Mushroom.obj", mushroom, mushroomInfo, brown);

     load("meshes/cube.obj", cube, cubeInfo, green);
     texture_cube.load("textured_mesh/cube/BasicShapeMaterial_D.bmp");


}

void Model::load (const string& filename, GLuint* buf, vec3* info, vec3 _color)
{

    vector<vec3> v;
    vector<vec3> color ;
    vector<vec2> vt;
    vector<vec3> vn;
    vector<vec3> vec;
    vector<vec2> tex;
    vector<vec3> normal;

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
            /* match character mesh into one place */
            if (filename == "meshes/character_pose2.obj") {
                pos.x -= 103.712;
            }
            else if (filename == "meshes/character_pose3.obj") {
                pos.x -= 230.289;
            }
            v.emplace_back(pos);
            /*for (int i = 0; i < 3; i++) {
                vertex_positions.emplace_back(pos[i]);
            }*/
        }
        else if (keyword == "vt") {
            vec2 tex_coord;
            ss >> tex_coord.x >> tex_coord.y;
            vt.emplace_back(tex_coord);
        }
        else if (keyword == "vn") {
            vec3 normal;
            ss >> normal.x >> normal.y >> normal.z;
            vn.emplace_back(normal);
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
                vec.emplace_back(v[stoi(index_data[0]) - 1]);
                // vertex texture index
                tex.emplace_back(vt[stoi(index_data[1]) - 1]);
                // vertex normal index
                normal.emplace_back(vn[stoi(index_data[2]) - 1]);
                // color
                color.emplace_back(_color);
            }
        }
    }

    ifs.close();

    /* check model center & size */
    for (int i = 0; i < v.size(); i++) {
        vec3 pos = v[i];
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


    buf[1] = vec.size();
    // colors.assign(buf[3], _color);

    buf[3] = vec.size();
    cout << "FILENAME: " << filename << endl;
    cout << "vertex_positions size: " << v.size() << endl;
    cout << "colors size: " << color.size() << endl;
    cout << "texture_coordinates size: " << vt.size() << endl;
    cout << "vertex_normals size: " << vn.size() << endl;
    cout << "face_position_indices size: " << vec.size() << endl;
    cout << "face_texture_indices size: " << tex.size() << endl;
    cout << "face_normal_indices size: " << normal.size() << endl;
    cout << endl;

    int vertexSize = buf[1] * sizeof(vec3);
    int colorSize = buf[1] * sizeof(vec3);
    int textureSize = buf[1] * sizeof(vec2);
    int normalSize = buf[1] * sizeof(vec3);

    glGenBuffers(1, &buf[0]);
    glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
    glBufferData(GL_ARRAY_BUFFER, vertexSize + colorSize + textureSize + normalSize , NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertexSize , vec.front());
    glBufferSubData(GL_ARRAY_BUFFER, vertexSize , colorSize , color.front());
    glBufferSubData(GL_ARRAY_BUFFER, vertexSize + colorSize, textureSize, tex.front());
    glBufferSubData(GL_ARRAY_BUFFER, vertexSize + colorSize + textureSize, normalSize, normal.front());


    if (buf[0] == 0) {
        cout << filename << " VBO genertaion FAIL" << endl;
    }

    buf[2] = vec.size();

    v.clear();
    vt.clear();
    vn.clear();
    vec.clear();
    tex.clear();
    normal.clear();
    color.clear();

    /* reinitialize size checker */
    minX = FLT_MAX, minY = FLT_MAX, minZ = FLT_MAX;
    maxX = -FLT_MAX, maxY = -FLT_MAX, maxZ = -FLT_MAX;
}