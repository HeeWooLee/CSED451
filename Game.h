#pragma once
#include "Angel.h"
#include "Model.h"
#include "Object.h"
using namespace std;


struct Attenuation
{
	float constant;
	float linear;
	float exponent;

	Attenuation() {}
	Attenuation(float constantVal, float linearVal, float exponentVal)
		: constant(constantVal), linear(linearVal), exponent(exponentVal) {
	}
};

struct PointLight
{
	vec3 colour;
	// Light position is assumed to be in view coordinates
	vec3 position;
	float intensity;
	Attenuation att;
	PointLight() { }
	PointLight(const vec3& colourVal, const vec3& positionVal, float intensityVal, const Attenuation& attenuationVal)
		: colour(colourVal), position(positionVal), intensity(intensityVal), att(attenuationVal) {
	}
};

struct DirectionalLight
{
	vec3 colour;
	vec3 direction;
	float intensity;
	DirectionalLight() {}
	DirectionalLight(const vec3& colourVal, const vec3& directionVal, float intensityVal)
		: colour(colourVal), direction(directionVal), intensity(intensityVal) {
	}
};

class Game {
private:
	Model* model;
	Character* character;
	Cube* cube;
	Star* star;
	Fireball* fireball;
	Mushroom* mushroom;
	bool onGame;

	vec4 light_position;
	vec4 diffuseProduct = vec4(0.7, 0.7, 0.7, 0.0);
	vec4 ambientProduct = vec4(0.1, 0.1, 0.1, 0.0);
	vec4 specularProduct = vec4(0.1, 0.1, 0.1, 0.0);
	float shininess = 0.1;
	vec3 ambientLight;
	float specularPower;
	int score;
public:
	GLuint program;
	vec3 scale;
	float speed;
	float baseline, initBaseline, nextBaseline;
	bool isMushroomAhead;

	GLuint ver_loc, color_loc, texture_loc, normal_loc, tangent_loc, bitangent_loc; /* attribute variable */
	GLuint model_loc, alpha_loc, light_loc, shadingMode_loc, textureMode_loc, normalMap_loc; /* uniform variable */
	GLuint projection_loc; /* uniform variable */

	/* text vs */
	GLint textureSamplerLoc;
	GLint ambientLightLoc;
	GLint specularPowerLoc;
	GLint materialLoc;
	GLint pointLightLoc;
	GLint directionalLightLoc;

	PointLight pointLight;
	Attenuation attenuation;
	DirectionalLight directionalLight;


	Game();
	void shaderInit();
	void init();
	void drawAll();
	void setOnGame(bool);
	void addScore(int);

	bool getOnGame() { return onGame; }
	int getScore() { return score; }
	GLuint getProgram() { return program; }
	Model* getModel() { return model; }
	Character* getCharacter() { return character; }
	Fireball* getFireball() { return fireball; }
	Star* getStar() { return star; }
};