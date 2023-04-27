#include "GameObject.h"

/* Game Object class */
GameObject::GameObject(const std::string& filename, GLfloat width, GLfloat height, GLfloat depth, GLfloat moveSpeed)
	: width(width), height(height), depth(depth), moveSpeed(moveSpeed) {
	model = loadObjFile(filename);
	sendDataToOpenGL(&model, &VAO, &VBO, &EBO);

	halfWidth = width / 2;
	halfHeight = height / 2;
	halfDepth = depth / 2;

}

GLfloat GameObject::getWidth() {
	return width;
}

GLfloat GameObject::getHeight() {
	return height;
}

GLfloat GameObject::getDepth() {
	return depth;
}

/* Star class */
Star::Star(const std::string& filename, GLfloat width, GLfloat height, GLfloat depth, GLfloat moveSpeed)
	:GameObject(filename, width, height, depth, moveSpeed) {
	transparency.assign(100, 1.0f);
}

void Star::init() {

}

void Star::draw() {
	drawModel(&VAO, &model);
}

void Star::detectCollision(glm::fvec3) {

}