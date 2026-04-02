#include "SkeletalMesh.h"

SkeletalMesh::SkeletalMesh() {}

SkeletalMesh::~SkeletalMesh() {}

int SkeletalMesh::getAnimationData(char* path) {
	struct Bones { int ID; float x, y, z; float xr, yr, zr; };

	std::vector< std::vector<Bones> > boneProperties;

	std::cout << "Loading animation data: " << path << std::endl;

	int id;
	double bPos[3];
	double bRot[3];

	FILE* fd = NULL;
	errno_t err = fopen_s(&fd, path, "r");

	if (err != 0 || fd == NULL) {
		perror("fopen_s failed");
		return 0;
	}

	do {
		fscanf_s(fd, "%d", &id);
		fscanf_s(fd, "%lf %lf %lf", &bPos[0], &bPos[1], &bPos[2]);



	} while (!feof(fd));

	return 0;
}

int SkeletalMesh::GetDisplayList(char* path) {
	struct Coord { float x, y, z; };
	struct Bones { int ID; float x, y, z; float xr, yr, zr; };

	std::vector<Coord> vertexs;
	std::vector< std::pair<float, float> > texcoords;
	std::vector<Coord> normals; //per vertex
	std::vector< std::vector< std::vector<int> > > faces;
	std::vector<Bones> bones;

	std::cout << "Loading model: " << path << std::endl;

	char c[3];
	double bPos[3];
	double bRot[3];
	
	FILE* fd = NULL;
	errno_t err = fopen_s(&fd, path, "r");

	if (err != 0 || fd == NULL) {
		perror("fopen_s failed");
		return 0;
	}

	
}