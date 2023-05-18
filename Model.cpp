#include "Model.h"
#include <iostream>
#include <fstream>
#include <string>

#include "Game.h"
extern Game* game;

/* model size and center checker */
float minX = FLT_MAX, minY = FLT_MAX, minZ = FLT_MAX;
float maxX = -FLT_MAX, maxY = -FLT_MAX, maxZ = -FLT_MAX;

vec3 purplep = vec3(0.5, 0.0, 0.5);
vec3 yellowy = vec3(1.0, 1.0, 0.0);
vec3 redr = vec3(1.0, 0.0, 0.0);
vec3 greeng = vec3(0.0, 1.0, 0.0);
vec3 brownb = vec3(0.647, 0.165, 0.165);


Model::Model() {
    // load("meshes/character_pose1.obj", character[0], characterInfo[0], purplep);
    //load("meshes/character_pose2.obj", character[1], characterInfo[1], purplep);
    //load("meshes/character_pose3.obj", character[2], characterInfo[2], purplep);

    normalmap_fireball.load("textured_mesh/fire_ball/M_ContentWindow_N.bmp");
    // load("meshes/fireball.obj", fireball, fireballInfo, redr);
     texture_fireball[0].load("textured_mesh/fire_ball/M_ContentWindow_D.bmp");
    // texture_fireball[1].load("textured_mesh/fire_ball/M_ContentWindow_S.bmp");

    load("meshes/star.obj", star, starInfo, yellowy);

    //   load("meshes/Mushroom.obj", mushroom, mushroomInfo, brownb);

       //   load("meshes/cube.obj", cube, cubeInfo, greeng);


    texture_cube.load("textured_mesh/cube/BasicShapeMaterial_D.bmp");
}

vector<string> Model::tokenize(string str, string delim) {
    vector<string> v;
    size_t seek = 0;

    while (str.length() > seek) {
        size_t pos;
        pos = str.find_first_of(delim, seek);

        if (pos != string::npos) {
            v.push_back(str.substr(seek, pos - seek));
            seek = pos + 1;
        }
        else {
            v.push_back(str.substr(seek));
            seek = str.length();
        }
    }

    return v;
}

void Model::load(const string& filename, GLuint* buf, vec3* info, vec3 _color)
{

    vector<vec3> v;
    vector<vec3> color;
    vector<vec2> vt;
    vector<vec3> vn;
    vector<vec3> vec;
    vector<vec2> tex;
    vector<vec3> normal;
    vector<vec3> normal_flat;
    vector<vec3> tangent;
    vector<vec3> bitangent;

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
            vec3 norm;
            ss >> norm.x >> norm.y >> norm.z;
            vn.emplace_back(norm);
        }
        else if (keyword == "f") {
            vec3 flat_norm;
            string face_data;
            vector<string> t;

            // split the face data by spaces
            while (ss >> face_data) {
                t.emplace_back(face_data);
            }

            vector<string> p1 = tokenize(t[0], "/");
            vector<string> p2 = tokenize(t[1], "/");
            vector<string> p3 = tokenize(t[2], "/");
            vector<string> p4;
            if (t.size() == 4)
                p4 = tokenize(t[3], "/");

            vec.push_back(v[stoi(p1[0]) - 1]); color.push_back(_color);
            vec.push_back(v[stoi(p2[0]) - 1]); color.push_back(_color);
            vec.push_back(v[stoi(p3[0]) - 1]); color.push_back(_color);

            tex.push_back(vt[stoi(p1[1]) - 1]);
            tex.push_back(vt[stoi(p2[1]) - 1]);
            tex.push_back(vt[stoi(p3[1]) - 1]);

            normal.push_back(vn[stoi(p1[2]) - 1]);
            normal.push_back(vn[stoi(p2[2]) - 1]);
            normal.push_back(vn[stoi(p3[2]) - 1]);

            /* flat shading */
            normal_flat.push_back(vn[stoi(p1[2]) - 1]);
            normal_flat.push_back(vn[stoi(p1[2]) - 1]);
            normal_flat.push_back(vn[stoi(p1[2]) - 1]);


            /* normal mapping */
            vec3 pos1 = v[stoi(p2[0]) - 1] - v[stoi(p1[0]) - 1];
            vec3 pos2 = v[stoi(p3[0]) - 1] - v[stoi(p1[0]) - 1];
            vec2 uv1 = vt[stoi(p2[1]) - 1] - vt[stoi(p1[1]) - 1];
            vec2 uv2 = vt[stoi(p3[1]) - 1] - vt[stoi(p1[1]) - 1];
            float r = 1.0f / (uv1.x * uv2.y - uv1.y * uv2.x);

            vec3 tan = (pos1 * uv2.y - pos2 * uv1.y) * r;
            vec3 bi = (pos2 * uv1.x - pos1 * uv2.x) * r;
            for (int i = 0; i < 3; i++) {
                tangent.push_back(tan);
                bitangent.push_back(bi);
            }
            if (t.size() == 4) {
                vec.push_back(v[stoi(p3[0]) - 1]);
                vec.push_back(v[stoi(p4[0]) - 1]);
                vec.push_back(v[stoi(p1[0]) - 1]);
                color.push_back(_color);
                color.push_back(_color);
                color.push_back(_color);


                tex.push_back(vt[stoi(p3[1]) - 1]);
                tex.push_back(vt[stoi(p4[1]) - 1]);
                tex.push_back(vt[stoi(p1[1]) - 1]);

                normal.push_back(vn[stoi(p3[2]) - 1]);
                normal.push_back(vn[stoi(p4[2]) - 1]);
                normal.push_back(vn[stoi(p1[2]) - 1]);


                /* flat shading */
                normal_flat.push_back(vn[stoi(p1[2]) - 1]);
                normal_flat.push_back(vn[stoi(p1[2]) - 1]);
                normal_flat.push_back(vn[stoi(p1[2]) - 1]);

                /* normal mapping */
                pos1 = v[stoi(p4[0]) - 1] - v[stoi(p3[0]) - 1];
                pos2 = v[stoi(p1[0]) - 1] - v[stoi(p3[0]) - 1];
                uv1 = vt[stoi(p4[1]) - 1] - vt[stoi(p3[1]) - 1];
                uv2 = vt[stoi(p1[1]) - 1] - vt[stoi(p3[1]) - 1];
                r = 1.0f / (uv1.x * uv2.y - uv1.y * uv2.x);

                tan = (pos1 * uv2.y - pos2 * uv1.y) * r;
                bi = (pos2 * uv1.x - pos1 * uv2.x) * r;

                for (int i = 0; i < 3; i++) {
                    tangent.push_back(tan);
                    bitangent.push_back(bi);
                }
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
    glBufferData(GL_ARRAY_BUFFER, vertexSize + colorSize + textureSize + normalSize * 4, NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertexSize, vec.front());
    glBufferSubData(GL_ARRAY_BUFFER, vertexSize, colorSize, color.front());
    glBufferSubData(GL_ARRAY_BUFFER, vertexSize + colorSize, textureSize, tex.front());
    glBufferSubData(GL_ARRAY_BUFFER, vertexSize + colorSize + textureSize, normalSize, normal.front());
    glBufferSubData(GL_ARRAY_BUFFER, vertexSize + colorSize + textureSize + normalSize, normalSize, normal_flat.front());
    glBufferSubData(GL_ARRAY_BUFFER, vertexSize + colorSize + textureSize + normalSize *2, normalSize, tangent.front());
    glBufferSubData(GL_ARRAY_BUFFER, vertexSize + colorSize + textureSize + normalSize *3, normalSize, bitangent.front());


    if (buf[0] == 0) {
        cout << filename << " VBO genertaion FAIL" << endl;
    }

    v.clear();
    vt.clear();
    vn.clear();
    vec.clear();
    tex.clear();
    normal.clear();
    color.clear();
    bitangent.clear();
    tangent.clear();

    /* reinitialize size checker */
    minX = FLT_MAX, minY = FLT_MAX, minZ = FLT_MAX;
    maxX = -FLT_MAX, maxY = -FLT_MAX, maxZ = -FLT_MAX;
}