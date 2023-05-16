#include "Object.h"

#include <time.h>
#include "Game.h"
using namespace std;
extern Game* game;
extern deque<float> starVis;

Object::Object() {
	x = 0; y = 0; width = 0; height = 0;
	ver_loc = game->ver_loc;
	tex_loc = game->texture_loc;
	color_loc = game->color_loc;
	model_loc = game->model_loc;
	alpha_loc = game->alpha_loc;
	program = game->getProgram();
	transform = Scale(game->scale) * mat4(1.0f);
	alpha = 1.0f;
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
	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf[1]);
	glVertexAttribPointer(ver_loc, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glVertexAttribPointer(color_loc, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(buf[3]));
	glVertexAttribPointer(tex_loc, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(buf[3]));
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, transform);
	glUniform1f(glGetUniformLocation(program, "alpha"), alpha);

	glDrawElements(GL_TRIANGLES, buf[2], GL_UNSIGNED_INT, 0);
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


}

void Character::draw() {
	float halfHeight = getHeight() / 2.0;
	float PosY = getY();
	float baseline = game->baseline;
	float initPosY = game->initBaseline - game->getModel()->cubeInfo[1].y / 2.0 + halfHeight;
	float baselineChange = baseline - initPosY;
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

	for (size_t i = 0; i < 20; i++) {
		GLuint level = levels[i];
		transform = Translate(width * i, 0.0, 0.0) * matStack.top();
		matStack.push(transform);

		mushroom->setTransform(Translate(width * i, 0.0, 0.0)* mushMatStack->top());
		mushMatStack->push(mushroom->getTransform());

		for (size_t j = 0; j < level; j++) {
			transform = Translate(0.0, height * j, 0.0) * matStack.top();
			draw_code(buf);
		}
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

}

void Star::draw() {
	float width = getWidth();
	float height = getHeight();
	speed = game->getOnGame() ? speed : 0.0;


	matStack.push(transform);

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
}

void Fireball::draw() {
	float width = getWidth();
	float height = getHeight();
	speed = game->getOnGame() ? speed : 0.0;

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

	transform = matStack.top();
	matStack.pop();
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