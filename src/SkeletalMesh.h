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

class SkeletalMesh {
public:
	SkeletalMesh();
	~SkeletalMesh();
	void loadMesh(const std::string& path);
	void drawMesh();
private:
	int GetDisplayList(char* path);
	int getAnimationData(char* path);
	int meshID;
	std::vector<int> boneIDs;
};