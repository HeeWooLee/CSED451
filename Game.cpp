#include "Game.h"
#include <glm/gtc/type_ptr.hpp>

Game::Game() {
	onGame = true;
	scale = vec3(0.001f, 0.001f, 0.001f);
	speed = -0.0002f;
	baseline = initBaseline = -0.5f;
	score = 0;
	Attenuation att(0.1f, 0.1f, 0.1f);
	//pointLight = PointLight(
	//	vec3(1.0f, 0.0f, 0.0f),
	//	vec3(-1.0f, -1.0f, 1.0f),
	//	0.5f,
	//	att
	//);
	ambientLight = vec3(0.1f, 0.1f, 0.1f);
	specularPower = 1;
	pointLight.colour = vec3(1.0f, 1.0f, 1.0f);
	pointLight.position = vec3(0.0f, 0.0f, 0.0f);
	pointLight.intensity = 0.2f;
	pointLight.att = att;
	directionalLight = DirectionalLight(
		vec3(1.0f, 1.0f, 1.0f),
		vec3(-1.0f, -1.0f, 1.0f),
		0.1f
	);
}

void Game::shaderInit() {
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	/* og */
	//program = InitShader("vshader.glsl", "fshader.glsl");
	//glUseProgram(program);

	//ver_loc = glGetAttribLocation(program, "vPosition");
	//glEnableVertexAttribArray(ver_loc);

	//texture_loc = glGetAttribLocation(program, "vTex");
	//glEnableVertexAttribArray(texture_loc);

	//normal_loc = glGetAttribLocation(program, "aNormal");
	//glEnableVertexAttribArray(normal_loc);
	//tangent_loc = glGetAttribLocation(program, "aTangent");
	//glEnableVertexAttribArray(tangent_loc);
	//bitangent_loc = glGetAttribLocation(program, "aBitangent");
	//glEnableVertexAttribArray(bitangent_loc);


	//color_loc = glGetAttribLocation(program, "aColor");
	//glEnableVertexAttribArray(color_loc);

	//model_loc = glGetUniformLocation(program, "model_view");
	//projection_loc = glGetUniformLocation(program, "projection");
	//alpha_loc = glGetUniformLocation(program, "alpha");

	//light_loc = glGetUniformLocation(program, "LightPosition");
	//shadingMode_loc = glGetUniformLocation(program, "ShadingMode");
	//textureMode_loc = glGetUniformLocation(program, "NormalMappingMode");
	//normalMap_loc = glGetUniformLocation(program, "normalMap");
	//glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambientProduct);
	//glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuseProduct);
	//glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specularProduct);
	//glUniform1f(glGetUniformLocation(program, "Shininess"), shininess);
	//glUniform1i(shadingMode_loc, 2);
	//glUniform4fv(light_loc, 1, vec4(2.0f, 2.0f, 2.0f, 0.0f));

	/* tex */
	program = InitShader("text.v", "text.f");
	glUseProgram(program);
	 
	/* vs in */
	ver_loc = glGetAttribLocation(program, "position");
	glEnableVertexAttribArray(ver_loc);
	texture_loc = glGetAttribLocation(program, "texCoord");
	glEnableVertexAttribArray(texture_loc);
	normal_loc = glGetAttribLocation(program, "vertexNormal");
	glEnableVertexAttribArray(normal_loc);
	tangent_loc = glGetAttribLocation(program, "vertexTangent");
	glEnableVertexAttribArray(tangent_loc);
	bitangent_loc = glGetAttribLocation(program, "vertexBitangent");
	glEnableVertexAttribArray(bitangent_loc);

	/* vs uniform */
	model_loc = glGetUniformLocation(program, "modelViewMatrix");
	projection_loc = glGetUniformLocation(program, "projectionMatrix");

	/* fs uniform */
	textureSamplerLoc = glGetUniformLocation(program, "texture_sampler");
	ambientLightLoc = glGetUniformLocation(program, "ambientLight");
	specularPowerLoc = glGetUniformLocation(program, "specularPower");
	materialLoc = glGetUniformLocation(program, "material");
	pointLightLoc = glGetUniformLocation(program, "pointLight");
	directionalLightLoc = glGetUniformLocation(program, "directionalLight");
	/* uniform init */
	glUniform3fv(ambientLightLoc, 1, ambientLight);
	glUniform1f(specularPowerLoc, specularPower);
	/* texture map */
	glUniform1i(glGetUniformLocation(program, "texture_sampler"), 0);
	/* normal map */
	glUniform1i(glGetUniformLocation(program, "norm_sampler"), 1);
}


void Game::init() {
	model = new Model();
	 // cube = new Cube();
	// character = new Character();
	star = new Star();
	fireball = new Fireball();
}



void Game::drawAll() {
	// cube->draw();
	//character->draw();
	star->draw();
	fireball->draw();
}

void Game::setOnGame(bool _onGame) {
	onGame = _onGame;
}
void Game::addScore(int _score) {
	score += _score;
}