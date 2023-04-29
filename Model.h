#pragma once
#include <fstream>
#include <sstream>
#include <vector>
#include "Angel.h"
using namespace std;

class Model {
public:
    GLuint character[3][3], fireball[3], star[3], mushroom[3], cube[3];
    Model();
    void load(const string&, GLuint*);
};


