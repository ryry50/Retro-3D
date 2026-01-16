#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <array>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "stb_image/stb_image.h"
#include "Texture.h"
#include "Font.h"
#include "Render.h"
#include "Transform.h"
class Collision {
	public:
		// Axis-Aligned Bounding Box (AABB) structure
		struct AABB {
			glm::vec3 min; // Minimum corner
			glm::vec3 max; // Maximum corner
		};
		// Check for collision between two AABBs
		static bool checkAABBCollision(const AABB& box1, const AABB& box2) {
			return (box1.min.x <= box2.max.x && box1.max.x >= box2.min.x) &&
				   (box1.min.y <= box2.max.y && box1.max.y >= box2.min.y) &&
				   (box1.min.z <= box2.max.z && box1.max.z >= box2.min.z);
		}

		static bool hitBox(glm::vec3 bound, glm::vec3 obj, const AABB& box1) {
			return glm::all(glm::lessThanEqual(box1.min, obj + bound)) &&
				glm::all(glm::greaterThanEqual(box1.max, obj - bound));
		}

		//Check for collision in directions

	private:

};