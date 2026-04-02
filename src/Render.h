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
		double frustum[6], glm::vec3 tran,
		glm::vec3 camTran, glm::vec3 cameraFront, glm::vec3 cameraUp, 
		glm::vec4 rotation);
	void drawTex(unsigned int indices[], int count, float material[],
		double frustum[6], glm::vec3 tran,
		glm::vec3 camTran, glm::vec3 cameraFront, glm::vec3 cameraUp,
		glm::vec4 rotation, Texture tex);
	void drawStart(
		double frustum[6], glm::vec3 tran,
		glm::vec3 camTran, glm::vec3 cameraFront, glm::vec3 cameraUp,
		glm::vec4 rotation);
	void drawStartNoCam(
		double frustum[6], glm::vec3 tran,
		glm::vec4 rotation);
	void drawStartTex(
		double frustum[6], glm::vec3 tran,
		glm::vec3 camTran, glm::vec3 cameraFront, glm::vec3 cameraUp,
		glm::vec4 rotation, void shape(), Texture tex);
};