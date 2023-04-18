#pragma once
#include "Game.h"
Game::Game() {
	gameSpeed = 100.0f;
}

void Game::init() {
	character.init();
	terrain.init();
}
void Game::update() {
	anime++;
	character.updatePosY();
}

void Game::draw() {
	if (onGame) {
		glColor3f(1.0f, 1.0f, 1.0f);
		glScalef(0.3f, 0.3f, 0.3f);
		glTranslatef(-0.5f, 0.0f, 0.0f);
		glScalef(0.01f, 0.01f, 0.01f);
		glPushMatrix();
		glTranslatef(0.0f, 1000*character.getPosY(), 0.0f);
		character.drawCharacter(&anime);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-gameSpeed, 0.0f, 0.0f);
		terrain.drawT();
		glPopMatrix();
		 
	}
}

void Game::setJump() {
	character.setJumping(0);
}

void Game::endGame() {
	onGame = false;
}
