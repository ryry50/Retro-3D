#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <array>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <cmath> 
#include "Render.h"

class Transform {
private:
	template <typename T>
	int signum(T val) {
		if (val > 0) return 1;
		if (val < 0) return -1;
		return 0;
	}
	double lastTime = 0.0;
	double currentTime = glfwGetTime();
	float deltaTime = currentTime - lastTime;
	double toleranceX = 0.1;
	double toleranceY = 0.1;
	double toleranceZ = 0.1;
	bool AtTargetX;
	bool AtTargetY;
	bool AtTargetZ;

	glm::vec3 current;
	glm::vec3 prev;
public:
	glm::vec3 init(glm::vec3 init) {
		current.x = init.x;
		current.y = init.y;
		current.z = init.z;

		return current;
	}

	double translateX(double targetX, double speed) {
		AtTargetX = -toleranceX < (targetX - current.x) < toleranceX;
		if (targetX - current.x > 0 && !AtTargetX) {
				currentTime = glfwGetTime();
				deltaTime = currentTime - lastTime;
				lastTime = currentTime;
				current.x += speed * deltaTime;
				return current.x;
		}
		if (targetX - current.x < 0 && AtTargetX) {
				currentTime = glfwGetTime();
				deltaTime = currentTime - lastTime;
				lastTime = currentTime;
				current.x -= speed * deltaTime;
				return current.x;
		}
		return targetX;
	}

	double translateY(double targetY, double speed) {
		AtTargetY = -toleranceY < (targetY - current.y) < toleranceY;
		if (targetY - current.y > 0 && !AtTargetY) {
			currentTime = glfwGetTime();
			deltaTime = currentTime - lastTime;
			lastTime = currentTime;
			current.y += speed * deltaTime;
			return current.y;
			std::cout << current.y << std::endl;
		}
		if (targetY - current.y < 0 && AtTargetY) {
				currentTime = glfwGetTime();
				deltaTime = currentTime - lastTime;
				lastTime = currentTime;
				current.y -= speed * deltaTime;
				return current.y;
		}
		return targetY;
	}

	double translateZ(double targetZ, double speed) {
		AtTargetZ = -toleranceZ < (targetZ - current.z) < toleranceZ;
		if (targetZ - current.z > 0 && !AtTargetZ) {
				currentTime = glfwGetTime();
				deltaTime = currentTime - lastTime;
				lastTime = currentTime;
				current.z += speed * deltaTime;
				return current.z;
		}
		if (targetZ - current.z < 0 && AtTargetZ) {
			currentTime = glfwGetTime();
			deltaTime = currentTime - lastTime;
			lastTime = currentTime;
			current.z -= speed * deltaTime;
			return current.z;
		}
		return targetZ;
	}

	glm::vec3 translateTo(glm::vec3 target, float speed) {
		prev = current;
		AtTargetZ = -toleranceZ < (target.z - current.z) < toleranceZ;
		AtTargetY = -toleranceY < (target.y - current.y) < toleranceY;
		AtTargetX = -toleranceX < (target.x - current.x) < toleranceX;
		currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		current += speed * (target - current) * deltaTime;
		
		return current;
	}


	void exponent(double x);
	void linear(double x);
};
