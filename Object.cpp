#include "Object.h"

#include <time.h>
#include <cstddef>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Game.h"


using namespace std;
extern Game* game;
extern deque<float> starVis;

vec3 purple = vec3(0.5, 0.0, 0.5);
vec3 yellow = vec3(1.0, 1.0, 0.0);
vec3 red = vec3(1.0, 0.0, 0.0);
vec3 green = vec3(0.0, 1.0, 0.0);
vec3 brown = vec3(0.647, 0.165, 0.165);

glm::vec3 ConvertToVec3(vec3 vec)
{
	return glm::vec3(vec.x, vec.y, vec.z);
}



Object::Object() {
	x = 0; y = 0; width = 0; height = 0;
	/* og */
	tangent_loc = game->tangent_loc;
	bitangent_loc = game->bitangent_loc;
	//color_loc = game->color_loc;
	//alpha_loc = game->alpha_loc;
	//textureMode_loc = game->textureMode_loc;
	//normalMap_loc = game->normalMap_loc;

	/* text attrib*/
	ver_loc = game->ver_loc;
	tex_loc = game->texture_loc;
	normal_loc = game->normal_loc;
	model_loc = game->model_loc;

	/* text vs */
	textureSamplerLoc = game->textureSamplerLoc;
	ambientLightLoc = game->ambientLightLoc;
	specularPowerLoc = game->specularPowerLoc;
	materialLoc = game->materialLoc;
	pointLightLoc = game->pointLightLoc;
	directionalLightLoc = game->directionalLightLoc;

	program = game->getProgram();
	transform = Scale(game->scale) * mat4(1.0f);
	alpha = 1.0f;

	material = Material(
		vec3(1.0f, 1.0f, 1.0f),
		0,
		0.5f
	);
}

void Object::set(float _x, float _y, float _width, float _height) {
	x = _x; y = _y; width = _width; height = _height;
}

void Object::setSize(vec3 size) {
	width = size.x; height = size.y; depth = size.z;
}

void Object::setY(float _y) {
	y = _y;
}

void Object::setX(float _x) {
	x = _x;
}

void Object::setSpeed(float _speed) {
	speed = _speed;
}

void Object::initPosition(float _x, float _y, vec3 modelCenter) {
	setX(_x);
	setY(_y);
	transform = Translate(getX(),getY(), 0.0) * Translate(-modelCenter) * transform;
}

void Object::draw_code(GLuint* buf) {
	int sizeVer = buf[1] * sizeof(vec3);
	int sizeCol = buf[1] * sizeof(vec3);
	int sizeTex = buf[1] * sizeof(vec2);
	int sizeNorm = buf[1] * sizeof(vec3);
	
	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
	// glVertexAttribPointer(color_loc, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeVer));
	 glVertexAttribPointer(tangent_loc, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeVer + sizeCol + sizeTex + sizeNorm * 2));
	 glVertexAttribPointer(bitangent_loc, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeVer + sizeCol + sizeTex + sizeNorm * 3));
	// flat shading 
	// glVertexAttribPointer(normal_loc, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeVer + sizeCol + sizeTex + sizeNorm)); 
	// smooth shading

	glVertexAttribPointer(ver_loc, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glVertexAttribPointer(tex_loc, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeVer + sizeCol));
	glVertexAttribPointer(normal_loc, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeVer + sizeCol + sizeTex));

	glUniformMatrix4fv(model_loc, 1, GL_TRUE, transform);
	// glUniform1f(glGetUniformLocation(program, "alpha"), alpha);

	/* */


	glUniform3fv(glGetUniformLocation(game->program, "material.colour"), 1, material.colour);
	glUniform1i(glGetUniformLocation(game->program, "material.useColour"), material.useColour);
	glUniform1f(glGetUniformLocation(game->program, "material.reflectance"),  material.reflectance);

	glUniform3fv(glGetUniformLocation(game->program, "pointLight.colour"), 1, glm::value_ptr(ConvertToVec3(game->pointLight.colour)));
	glUniform3fv(glGetUniformLocation(game->program, "pointLight.position"), 1, glm::value_ptr(ConvertToVec3(game->pointLight.position)));
	glUniform1f(glGetUniformLocation(game->program, "pointLight.intensity"), game->pointLight.intensity);
	glUniform1f(glGetUniformLocation(game->program, "pointLight.att.constant"), game->pointLight.att.constant);
	glUniform1f(glGetUniformLocation(game->program, "pointLight.att.linear"), game->pointLight.att.linear);
	glUniform1f(glGetUniformLocation(game->program, "pointLight.att.exponent"), game->pointLight.att.exponent);

	glUniform3fv(glGetUniformLocation(game->program, "directionalLight.colour"), 1, glm::value_ptr(ConvertToVec3(game->directionalLight.colour)));
	glUniform3fv(glGetUniformLocation(game->program, "directionalLight.direction"), 1, glm::value_ptr(ConvertToVec3(game->directionalLight.direction)));
	glUniform1f(glGetUniformLocation(game->program, "directionalLight.intensity"), game->directionalLight.intensity);

	glUniform3fv(glGetUniformLocation(game->program, "col"), 1, vec3(1.0, 0.0, 1.0));
	glUniform1i(glGetUniformLocation(game->program, "normalMapMode"), normalMapMode);

	glDrawArrays(GL_TRIANGLES, 0, buf[1]);
}

void Object::move(float _x, float _y) {
	x += _x; y += _y;
}

/*
 * Character class
 */
Character::Character() {
	frame = 0;
	count = 0;
	state = 0;
	init = true;
	jumpLimit = actualLimit = 1.0;
	speed = 0.0007;

	buf = game->getModel()->character[frame];
	setSize(game->getModel()->characterInfo[0][1]);
	initPosition(-0.7f, game->initBaseline - game->getModel()->cubeInfo[1].y / 2.0 + getHeight() / 2.0, game->getModel()->characterInfo[0][0]);

	material = Material(
		purple,
		1,
		0.5f
	);

}

void Character::draw() {
	float halfHeight = getHeight() / 2.0;
	float PosY = getY();
	float baseline = game->baseline;
	float initPosY = game->initBaseline - game->getModel()->cubeInfo[1].y / 2.0 + halfHeight;
	float baselineChange = baseline - initPosY;

	glUniform1i(textureMode_loc, 0);

	/* frame selection */
	frameSelection();

	speed = game->getOnGame() ? speed : 0;
	matStack.push(transform);

	if (init && PosY == initPosY) {
		setY(baseline + halfHeight);
		PosY = getY();
		init = false;
	}
	/* jumping */
	if (state == 1) {
		if (PosY < actualLimit) {
			move(0.0, speed);
		}
		else {
			state = 2; /* set as falling */
		}
	}
	/* falling from jump */
	else if (state == 2 || state == 0)  {
		if (PosY > (baseline + halfHeight + 0.01f) ) {
			move(0.0, -speed);
		}
		else if ((baseline + halfHeight - 0.01f) < PosY &&
			PosY <= (baseline + halfHeight + 0.01f)) {
			setY(baseline + halfHeight);
			state = 0;
		}
	}
	/* falling to hole */
	else if (state == 3) {
		move(0.0, -3 * speed);
	}
	/* if hole falling */
	if (state != 3 && baseline < initPosY && 
		PosY < (game->nextBaseline + halfHeight)
		) {
		cout << "hole falling" << endl;
		state = 3;
	}
	/* transform */
	transform = Translate(0.0, getY() - initPosY, 0.0) * transform;
	/* fix frame when jumping */
	if (state == 1 || state == 2) {
		frame = 0;
	}
	buf = game->getModel()->character[frame];
	draw_code(buf);
	transform = matStack.top();
	matStack.pop();
}

void Character::frameSelection() {
	count++;
	int interval = 60;
	if (0 <= count && count < interval) {
		frame = 0;
	}
	else if (1 * interval <= count && count < 2 * interval) {
		frame = 1;
	}
	else if (2 * interval <= count && count < 3 * interval) {
		frame = 2;
	}
	else {
		count = 0;
	}

	/* frame for */
	if (state != 0) {
		frame = 0;
	}

	frame = 0;
}

void Character::setJumping() {
	if (state != 1 && state != 3 && ((getY() - getHeight()/2.0) < (game->baseline + 0.05))) {
		state = 1;
		/* lower jump: mushroom ahead*/
		if (game->isMushroomAhead) { 
			actualLimit = game->baseline + jumpLimit * 0.5;
			cout << "jumping in front of mushroom  " << endl;
		}
		else {
			actualLimit = game->baseline + jumpLimit;
		}
	}
}

/*
 * Cube class
 */
Cube::Cube() {
	buf = game->getModel()->cube;
	setSize(game->getModel()->cubeInfo[1]);
	setSpeed(game->speed);
	currStart = resetStart = getWidth() * -6.0;

	mushroom = new Mushroom(resetStart);
	initPosition( resetStart, game->baseline, game->getModel()->cubeInfo[0]);
	game->baseline -= getHeight() / 2.0;


	levels = { 1,1,1,2,1, 1, 0,  1, 2,3 };
	

	material = Material(
		vec3(1.0f, 1.0f, 1.0f),
		0,
		0.5f
	);

	randomLevel();
}

void Cube::randomLevel() {
	srand(time(NULL));
	int num = rand() % 4;
	int prev = num;
	for (int i = 0; i < 200; i++) {
		num = rand() % 4;
		if (num == 3) {
			if (prev == 1) {
				num = rand() % 2 + 1;
			}
		}
		if (num == 0) {
			if (rand() % 4 == 0 || prev == 0) {
				num = rand() % 3 + 1;
			}
		}

		levels.push_back(num);
		if (num == 0) {
			levels.push_back(prev);
		}
		else {
			prev = num;
		}
	}
}

void Cube::draw() {
	float width = getWidth();
	float height = getHeight();
	glUniform1i(textureMode_loc, 0);

	stack<mat4>* mushMatStack = mushroom->getMatStack();
	deque<bool>* mushIsDrawn = mushroom->getIsDrawn();

	/* renew start point of drawing */
	if (currStart == resetStart) {
		matStack.push(transform);

		mushMatStack->push(mushroom->getTransform());
	}
	currStart += speed;
	if (currStart < (resetStart - getWidth())) {
		currStart = resetStart;

		levels.push_back(levels.front());
		levels.pop_front();

		mushIsDrawn->push_back(mushIsDrawn->front());
		mushIsDrawn->pop_front();

		transform = matStack.top();
		matStack.pop();

		mushroom->setTransform(mushMatStack->top());
		mushMatStack->pop();
	}

	/* draw terrain */
	speed = game->getOnGame() ? speed : 0.0;
	transform = Translate(speed, 0.0f, 0.0f) * transform;
	matStack.push(transform);

	mushroom->setTransform(Translate(speed, 0.0f, 0.0f) * mushroom->getTransform());
	mushMatStack->push(mushroom->getTransform());


	glActiveTexture(GL_TEXTURE0);
	game->getModel()->texture_cube.bind();

	for (size_t i = 0; i < 20; i++) {
		GLuint level = levels[i];
		transform = Translate(width * i, 0.0, 0.0) * matStack.top();
		matStack.push(transform);

		mushroom->setTransform(Translate(width * i, 0.0, 0.0)* mushMatStack->top());
		mushMatStack->push(mushroom->getTransform());

		/* texture mapping*/
		glActiveTexture(GL_TEXTURE0);
		game->getModel()->texture_cube.bind();

		for (size_t j = 0; j < level; j++) {
			transform = Translate(0.0, height * j, 0.0) * matStack.top();


			draw_code(buf); // draw cube
		}
		/* unbind texture */
		glBindTexture(GL_TEXTURE_2D, 0);
		if (level != 0 && mushIsDrawn->at(i)) {
			mushroom->setTransform(Translate(0.0, height * (level - 0.5) + mushroom->getHeight() / 2.0, 0.0) * mushMatStack->top());
			mushroom->draw();
		}
		matStack.pop();
		mushMatStack->pop();
	}

	transform = matStack.top();
	matStack.pop();

	mushroom->setTransform(mushMatStack->top());
	mushMatStack->pop();
	/* baseline update */
	GLuint xIndex = ceil((game->getCharacter()->getX() - (float)currStart - width/2.0) / width) ;
	game->baseline = (levels[xIndex] == 3)? 0: game->initBaseline - height / 2.0 + levels[xIndex]  * height;
	/* additional update under mushroom presence */
	if (levels[xIndex] != 0 && mushIsDrawn->at(xIndex)
		&&
		(resetStart - width / 2.0 - mushroom->getWidth() / 2.0) < currStart &&
		(resetStart - width / 2.0 + mushroom->getWidth() / 2.0) > currStart
		)  {
		game->baseline += mushroom->getHeight();
	}
	game->nextBaseline  = game->initBaseline - height / 2.0 + (levels[xIndex+1]) *height ;
	/* check if mushroom is ahead */
	game->isMushroomAhead = (mushIsDrawn->at(xIndex + 1)) ? true : false;
}

/*
 * Star class
 */
Star::Star() {
	buf = game->getModel()->star;
	startPos = -1.5f;
	setSpeed(game->speed);
	setSize(game->getModel()->starInfo[1]);
	initPosition(startPos, 0.0, game->getModel()->starInfo[0]);

	for (size_t i = 0; i < 5; i++) {
		randomGen();
	}

	material = Material(
		yellow,
		1,
		0.5f
	);

	/* MOVE 2 FIREBALL */
	// normalMapMode = 1;
}

void Star::draw() {
	float width = getWidth();
	float height = getHeight();
	speed = game->getOnGame() ? speed : 0.0;

	matStack.push(transform);


	///* MOVE 2 FIREBALL */
	 //  glActiveTexture(GL_TEXTURE1);
	  // game->getModel()->normalmap_fireball.bind();
	
	/* draw fireballs */
	for (size_t i = 0; i < 4; i++) {
		transform = Translate(starPos[i].x, 0.0, 0.0) * matStack.top();
		matStack.push(transform);
		transform = Translate(0.0, starPos[i].y, starPos[i].z) * transform;
		alpha = starVis[i] ;
		draw_code(buf);

		/* update if fireball is out of viewport */
		starPos[i].x += speed;
		if (starPos[i].x < 0.0) {
			starPos.pop_front();
			starVis.pop_front();
			randomGen();
		}

		matStack.pop();
	}

	/* MOVE 2 FIREBALL */
	/* unbind texture */
	//  glBindTexture(GL_TEXTURE_2D, 0);

	transform = matStack.top();
	matStack.pop();
}


void Star::randomGen() {
	float _x = (rand() % 5 + 4) / 10.0;
	float x = starPos.empty() ? _x : starPos.back().x + _x;
	float y = ((rand() % 5) * 2) / 10.0;
	starPos.push_back(vec3(x, y, 0.0));
	starVis.push_back(1.0f);
}

/*
 * Fireball class
 */
Fireball::Fireball() {
	buf = game->getModel()->fireball;
	startPos = -1.2f;
	setSpeed(game->speed * 1.4);
	setSize(game->getModel()->fireballInfo[1]);
	initPosition(startPos, 0.0, game->getModel()->fireballInfo[0]);

	for (size_t i = 0; i < 5; i++) {
		randomGen();
	}
	material = Material(
		red,
		0,
		0.5f
	);
	normalMapMode = 1;
}

void Fireball::draw() {
	float width = getWidth();
	float height = getHeight();
	speed = game->getOnGame() ? speed : 0.0;

	glUniform1i(textureMode_loc, 1); 
	glUniform1i(normalMap_loc, 0); // Map TEture0 to normalMap

	glActiveTexture(GL_TEXTURE0);
	game->getModel()->texture_fireball[0].bind();

	glActiveTexture(GL_TEXTURE1);
	game->getModel()->normalmap_fireball.bind();

	matStack.push(transform);

	/* draw fireballs */
	for (size_t i = 0; i < 4; i++) {
		transform = Translate(firePos[i].x, 0.0, 0.0) * matStack.top();
		matStack.push(transform);
		transform = Translate(0.0, firePos[i].y, firePos[i].z) * transform ;
		draw_code(buf);

		/* update if fireball is out of viewport */
		firePos[i].x += speed;
		if (firePos[i].x < 0.0) {
			firePos.pop_front();
			randomGen();
		}

		matStack.pop();
	}

	/* unbind texture */
	glBindTexture(GL_TEXTURE_2D, 0);

	transform = matStack.top();
	matStack.pop();

	// Texture unbind
}

void Fireball::randomGen() {
	float _x = (rand() % 5 + 8) / 10.0;
	float x = firePos.empty()? _x: firePos.back().x + _x;
	float y = ((rand() % 5) * 2) / 10.0;
	
	firePos.push_back(vec3(x,y,0.0));
}

/*
 * Mushroom class
 */
Mushroom::Mushroom(float resetStart) {
	buf = game->getModel()->mushroom;
	setSize(game->getModel()->mushroomInfo[1]);
	initPosition(resetStart, game->baseline, game->getModel()->mushroomInfo[0]);

	randomDrawn();

	material = Material(
		brown,
		1,
		0.5f
	);
}

void Mushroom::draw() {
	draw_code(buf);
}

void Mushroom::randomDrawn() {
	for (int i = 0; i < 20; i++) {
		bool value = (rand() % 8 == 0);
		isDrawn.push_back(value);
	}
}

void Mushroom::setTransform(mat4 _transform) {
	transform = _transform;
}