#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <array>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "stb_image/stb_image.h"
#include "Texture.h"

static class Render {
public:
	void init(float vertex[], float color[]);
	void clear();
	void flush(GLFWwindow* window);
	void draw(unsigned int indices[], int count, float material[],
		double frustum[6], float translation[3],
		glm::vec3 camTran, glm::vec3 cameraFront, glm::vec3 cameraUp, 
		float rotate[3], float rSpeed);
	void drawStart(
		double frustum[6], float translation[3],
		glm::vec3 camTran, glm::vec3 cameraFront, glm::vec3 cameraUp,
		float rotate[3], float rSpeed);
};