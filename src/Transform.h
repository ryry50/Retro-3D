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
	glm::vec3 tolerance;
	bool AtTarget;
	double pitch;
	double yaw;

	glm::vec3 current;
	glm::vec3 prev;
public:
	glm::vec3 init(glm::vec3 init) {
		current.x = init.x;
		current.y = init.y;
		current.z = init.z;
		std::cout << "X: " << current.x << " Y: " << current.y << " Z: " << current.z << std::endl;

		return current;
	}

	glm::vec3 translateTo(glm::vec3 target, float speed) {
		std::cout << "X: " << current.x << " Y: " << current.y << " Z: " << current.z << std::endl;
		tolerance = speed * glm::abs(target - current);
		//AtTarget = -tolerance < (target - current) < tolerance;
		currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		//if (!AtTarget)
		//{
			current += speed * glm::normalize(target - current) * deltaTime;
			return current;
		//}
		//return target;
	}


	void exponent(double x);
	void linear(double x);
};
