#pragma once
#include <string>

using namespace std;
class Bitmap
{
public:
    void LoadBMP(const char* Filename, vector<unsigned char>* image, unsigned int* w, unsigned int* h); //loads bitmap from Filename in *.BMP format
};