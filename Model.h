#pragma once
#include <fstream>
#include <sstream>
#include <vector>
#include "Angel.h"
#include "Texture.h"
#include "NormalMap.h"
using namespace std;

class Model {
public:
    GLuint character[3][4], fireball[4], star[4], mushroom[4], cube[4];
    vec3 characterInfo[3][2], fireballInfo[2], starInfo[2], mushroomInfo[2], cubeInfo[2];
    Texture texture_cube, texture_fireball[2];

    NormalMap normalmap_fireball;

    Model();
    void load(const string&, GLuint*, vec3* , vec3);
};


