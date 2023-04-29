#include "Object.h"

#include "Game.h"
using namespace std;
extern Game* game;


Object::Object() {
	x = 0; y = 0; width = 0; height = 0;
	ver_loc = game->ver_loc;
	model_loc = game->model_loc;
	program = game->getProgram();
}

void Object::set(float _x, float _y, float _width, float _height) {
	x = _x; y = _y; width = _width; height = _height;
}

void Object::setSize(float _width, float _height) {
	width = _width; height = _height;
}

void Object::setName(int _name) {
	name = _name;
}

void Object::setY(float _y) {
	y = _y;
}

void Object::setX(float _x) {
	x = _x;
}

void Object::draw_code(GLuint* buf) {
	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf[1]);
	glVertexAttribPointer(ver_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, transform);
	glDrawElements(GL_TRIANGLES, buf[2], GL_UNSIGNED_INT, 0);
}

void Object::move(float _x, float _y) {
	x += _x; y += _y;
}

/*
 * Character class
 */
Character::Character() {
	state = 0;
	move(48, 270);
	frame = 20;
	count = 0; upDown = 0; direction = 0;
	setSize(70, 70);
}
//
//GLuint* loadData() {
//
//	GLuint VBO;
//	GLuint EBO;
//	GLuint buf[3];
//	vector<vec3> vertex_positions = {
//		vec3(-1.0f, -1.0f, -1.0f),
//		vec3(-1.0f, -1.0f,  1.0f),
//		vec3(-1.0f,  1.0f, -1.0f),
//		vec3(-1.0f,  1.0f,  1.0f),
//		vec3(1.0f, -1.0f, -1.0f),
//		vec3(1.0f, -1.0f,  1.0f),
//		vec3(1.0f,  1.0f, -1.0f),
//		vec3(1.0f,  1.0f,  1.0f),
//	};
//
//	vector<GLuint> indices = {
//		0, 1, 2, 2, 1, 3, // left face
//		4, 5, 6, 6, 5, 7, // right face
//		0, 4, 2, 2, 4, 6, // bottom face
//		1, 5, 3, 3, 5, 7, // top face
//		0, 1, 4, 4, 1, 5, // front face
//		2, 3, 6, 6, 3, 7  // back face
//	};
//
//	glGenBuffers(1, &VBO);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, vertex_positions.size() * sizeof(vec3), &vertex_positions[0], GL_STATIC_DRAW);
//
//	glGenBuffers(1, &EBO);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
//
//	buf[0] = VBO;
//	buf[1] = EBO;
//	buf[2] = indices.size();
//
//	return buf;
//}


void Character::draw() {
	GLuint* buf = game->getModel()->star;
	transform = Scale(0.001f, 0.001f, 0.001f);
	// GLuint* buf = loadData();
	draw_code(buf);

	// cout << "character drawing success" << endl;
}
