#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Texture.h"
#include "Render.h"
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <list>
#include <algorithm>
#include <utility>
#include <fstream>
#include <string>



class Model {
	public:
		Model();
		~Model();
		void loadModel(const std::string& path);
		void drawModel();
	private:
		int GetDisplayList(char* path);
		int listID;
};
