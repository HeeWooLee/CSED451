#pragma once
#include "Game.h"
Game::Game() {
	gameSpeed = 0.005f;
}

void Game::init() {
	character.init();
	terrain.init();

	GLfloat tmp = 1.5f;
	Position tmpPos;
	for (size_t i = 0; i < 100; ++i) {
		tmp += (rand() % 13 + 6.0) / 10.0;
		stars.push_back(tmp);
		starsPatternChoice.push_back(rand() % 2);
		std::vector<GLfloat> consumed(starPatterns[starsPatternChoice[i]].size(), 1.0f);
		starsConsumed.push_back(consumed);
	}


	tmp = 1.5f;
	for (size_t i = 0; i < 150; ++i) {
		tmp += (rand() % 20 + 20.0) / 10.0;
		tmpPos.x = tmp * scale;
		tmpPos.y = rand() % 8 / 10.0 * scale;
		fireballs.push_back(tmpPos );
	}
}
void Game::update() {
	anime++;
	character.updatePosY();

	starContactDetected();

	starPosX -= gameSpeed;
	fireballPosX -= fireballSpeed;
}

void Game::draw() {
	if (onGame) {
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(-0.5f, 0.0f, 0.0f);
		glScalef(0.003f, 0.003f, 0.003f);
		glPushMatrix();
		glTranslatef(0.0f, 1000 * character.getPosY(), 0.0f);
		glTranslatef(0.0f, 1000*character.getPosY(), 0.0f);
		character.drawCharacter(&anime);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-gameSpeed, 0.0f, 0.0f);
		terrain.drawT();

		glTranslatef(gameSpeed, 0.0f, 0.0f);
		glPopMatrix();

		for (int i = 0; i < stars.size(); ++i) {
			GLuint choice = starsPatternChoice[i];
			GLfloat PosX = stars[i] + starPosX;
			if (PosX >= 4) {
				break;
			}
			if (PosX > -4 * scale  && PosX < 4 *scale) {

				glPushMatrix();
				glTranslatef(0.0f, -0.5 * scale , 0.0f);
				glTranslatef(PosX * scale,  0.0f, 0.0f);
				std::vector<Position> chosen = starPatterns[choice];
				for (int j = 0; j < chosen.size(); ++j) {
					glPushMatrix();
					glTranslatef(0.0f, chosen[j].y * scale, 0.0f);
					glTranslatef(chosen[j].x * scale , 0.0f, 0.0f);
					glTranslatef(gameSpeed, 0.0f, 0.0f);
					glScalef(0.5f, 0.5f, 0.5f);
					glColor4f(0.98f, 0.98f, 0.0f, starsConsumed[i][j]);
					terrain.drawStar();
					glPopMatrix();
				}
				glPopMatrix();
			}
		}

		for (int i = 0; i < fireballs.size(); ++i) {
			
			GLuint choice = starsPatternChoice[i];
			choice = 0 ? 1: 0;
			GLfloat PosX = stars[i] + starPosX ;
			if (PosX >= 4) {
				break;
			}
			if (PosX > -4 * scale && PosX < 4 * scale) {

				glPushMatrix();
				glTranslatef(0.0f, -0.5 * scale, 0.0f);
				glTranslatef(PosX * scale, 0.0f, 0.0f);
				std::vector<Position> chosen = fireballPatterns[choice];
				for (int j = 0; j < chosen.size(); ++j) {
					glTranslatef(j * gameSpeed, 0.0f, 0.0f);
					glPushMatrix();
					glTranslatef(0.0f, chosen[j].y * scale, 0.0f);
					glTranslatef(chosen[j].x * scale, 0.0f, 0.0f);
					glScalef(0.5f, 0.5f, 0.5f);
					glColor4f(0.98f, 0.0f, 0.0f, 0.0f);
					terrain.drawBall();
					glPopMatrix();
				}
				glPopMatrix();
			}
		}


		drawScore();

		if (onGame == false ) {
			glPushMatrix();
			glColor3f(0.529412f, 0.807843f, 0.980392f);
			glTranslatef(-1.6f, 0.8f, 0.0f);
			drawText("Score: " + std::to_string(score), GLUT_BITMAP_TIMES_ROMAN_24);
			glPopMatrix();

			glPushMatrix();
			glColor3f(0.62f, 0.12f, 0.94f);
			glTranslatef(-0.15f, 0.0f, 0.0f);
			drawText("Game Over", GLUT_BITMAP_TIMES_ROMAN_24);
			glPopMatrix();

			glPushMatrix();
			glColor3f(0.62f, 0.12f, 0.94f);
			glTranslatef(-0.16f, -0.1f, 0.0f);
			drawText("Final Score: " + std::to_string(score), GLUT_BITMAP_TIMES_ROMAN_24);
			glPopMatrix();
		}
	}
}

void Game::setJump() {
	character.setJumping(0);
}

void Game::endGame() {
	onGame = false;
}

void Game::downScale() {

	glScalef(0.003f, 0.003f, 0.003f);
}

void Game::upScale() {

	glScalef(1.0f/0.003f, 1.0f / 0.003f, 1.0f / 0.003f);
}


void Game::starContactDetected() {
}


void Game::drawText(std::string text, void* font) {
	glRasterPos2i(0, 0);
	for (int i = 0; i < text.length(); i++) {
		glutBitmapCharacter(font, text[i]);
	}
}

void Game::drawScore() {
	glPushMatrix();
	glColor3f(0.62f, 0.12f, 0.94f);
	glTranslatef(-1.6f, 0.8f, 0.0f);
	drawText("Score: " + std::to_string(score), GLUT_BITMAP_TIMES_ROMAN_24);
	glPopMatrix();
}