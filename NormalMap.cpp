#include "NormalMap.h"
#include <vector>
#include <iostream>
#include <cstdio>
#include "Bitmap.h"
using namespace std;

bool NormalMap::load(const char* fn) {
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
	cout << "w: " << w << "h: " << h << endl;
	cout << "size of pixels: " << pixels.size() << endl;
		 
	glGenTextures(1, &buf);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, buf);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, &pixels[0]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, w, h, GL_RGB, GL_UNSIGNED_BYTE, &pixels[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	cout << "Normalmap: " << fn << " is successfully loaded" << endl;
	cout << endl;
	return true;
}

void NormalMap::bind() {
	glBindTexture(GL_TEXTURE_2D, buf);
}