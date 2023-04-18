#pragma once
#include "Game.h"
Game::Game() {

}

void Game::init() {
	character.init();
}
void Game::update() {
	anime++;
}

void Game::draw() {
	if (onGame) {
		character.drawCharacter(&anime);
	}
}

void Game::setJump() {
	character.setJumping(0);
}

void Game::endGame() {
	onGame = false;
}
