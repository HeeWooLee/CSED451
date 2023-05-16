#include "Texture.h"
#include <vector>
#include <iostream>
#include <cstdio>
#include "Bitmap.h"
using namespace std;

bool Texture::load(const char* fn) {
	vector<unsigned char> pixels;
	//vector<unsigned char> png;
	//vector<unsigned char> image;
	//unsigned error;

	//error = lodepng::load_file(png, fn);
	//if (error) {
	//	cout << "Error " << error << " loading image " << fn << endl;
	//	cout << endl;
	//	return false;
	//}
	Bitmap bitmap;
	bitmap.LoadBMP(fn, &pixels, &w, &h);
	//if (error) {
	//	cout << "Error " << error << " loading image " << fn << endl;
	//	cout << endl;
	//	return false;
	//}

	glGenTextures(1, &buf);
	glBindTexture(GL_TEXTURE_2D, buf);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, &pixels[0]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, w, h, GL_RGB, GL_UNSIGNED_BYTE, &pixels[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	cout << "Texture: " << fn << " is successfully loaded" << endl;

	cout << endl;
	return true;
}

void Texture::bind() {
	glBindTexture(GL_TEXTURE_2D, buf);
}