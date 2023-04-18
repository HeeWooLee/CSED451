#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/vec3.hpp>
#include <vector>
#include <assimp/Importer.hpp>
#include <filesystem>

#include "shader_m.h"
#include "camera.h"
#include "model.h"


// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

std::vector<glm::vec3> position = {
glm::vec3(0.0f, 0.0f, 0.0f),
glm::vec3(1.0f, 0.0f, 0.0f),
glm::vec3(0.0f, 1.0f, 0.0f),
};
void renderScene(void) {
	// render
	// ------
	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
	stbi_set_flip_vertically_on_load(true);
	glEnable(GL_DEPTH_TEST);
	//glColor3f(1.0f, 0.0f, 0.0f);
	//glBegin(GL_POLYGON);
	//for (size_t i = 0; i < position.size(); i++) {
	//	glVertex3f(position[i][0], position[i][1], position[i][2]);
	//}
	//glEnd();
	//glFlush();


	// configure global opengl state
	// -----------------------------

	// build and compile shaders
	// -------------------------
	Shader ourShader("1.model_loading.vs", "1.model_loading.fs");

	// load models
	// -----------
	Model ourModel(("meshes/star.obj"));

	// don't forget to enable shader before setting uniforms
	ourShader.use();

	////// view/projection transformations
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();
	ourShader.setMat4("projection", projection);
	ourShader.setMat4("view", view);


	// render the loaded model
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
	ourShader.setMat4("model", model);
	ourModel.Draw(ourShader);

}
void main(int argc, char** argv) {
	// inspect array elements and print to console

	Assimp::Importer importer;

	const float* probe = &position[0].x;
	for (size_t i = 0; i < position.size() * 3; i++) {
		std::cout << probe[i] << std::endl;
	}
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Hello OpenGL");
	glewInit();

	/// <summary>
	/// LearnOpenGL
	/// </summary>



	glutDisplayFunc(renderScene);
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glutMainLoop();
}