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
	double tolerance = 0.1;
	double toleranceC = 0.03;
	bool AtTarget;
	double pitch;
	double yaw;

	glm::vec3 current;
	glm::vec3 delta;
public:
	glm::vec3 init(glm::vec3 init) {
		current.x = init.x;
		current.y = init.y;
		current.z = init.z;
		std::cout << "X: " << current.x << " Y: " << current.y << " Z: " << current.z << std::endl;

		return current;
	}

	//Linear translation function
	glm::vec3 transLinear(glm::vec3 target, float speed) {
		//Display the current position
		//std::cout << "X: " << current.x << " Y: " << current.y << " Z: " << current.z << std::endl;

		//Calculate delta time (frame time)
		currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		//Check for large delta times
		if (deltaTime > 0.5) deltaTime = 0.0;

		//Calculate the difference between target and current position
		delta = target - current;
		//Check if we are at the target position
		AtTarget = glm::length(delta) <= tolerance;
		//Magic line
		if (!AtTarget)
		{
			current += speed * glm::normalize(delta) * deltaTime;
			return current;
		}
		//if we are at the target position, return the target position
		return target;
	}

	//ease out function translation
	glm::vec3 transOut(glm::vec3 target, float speed) {
		//Display the current position
		//std::cout << "X: " << current.x << " Y: " << current.y << " Z: " << current.z << std::endl;

		//Calculate delta time (frame time)
		currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		//Check for large delta times
		if (deltaTime > 0.5) deltaTime = 0.0;

		//Calculate the difference between target and current position
		delta = target - current;
		//Check if we are at the target position
		AtTarget = glm::length(delta) <= toleranceC;
		//Magic line
		if (!AtTarget)
		{
			current += speed * delta * deltaTime;
			return current;
		}
		//if we are at the target position, return the target position
		return target;
	}



	void exponent(double x);
	void linear(double x);
};
