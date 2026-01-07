#include "Model.h"

Model::Model() {}

Model::~Model() {}

int Model::GetDisplayList(char* path) {
	struct Coord { float x, y, z; };

	std::vector<Coord> vertexs;
	std::vector< std::pair<float, float> > texcoords;
	std::vector<Coord> normals; //per vertex
	std::vector< std::vector< std::vector<int> > > faces;

	std::cout << "Loading model: " << path << std::endl;

	char c;
	FILE* fd = NULL;
	errno_t err = fopen_s(&fd, path, "r");

	if (err != 0 || fd == NULL) {
		perror("fopen_s failed");
		return 0;
	}

	do {

		fscanf_s(fd, "%c", &c, 1);

		if (c == 'v')
		{
			Coord aux;
			fscanf_s(fd, "%c", &c, 1); //espacio
			fscanf_s(fd, "%f", &aux.x);
			fscanf_s(fd, "%c", &c, 1); //espacio
			fscanf_s(fd, "%f", &aux.y);
			fscanf_s(fd, "%c", &c, 1); //espacio
			fscanf_s(fd, "%f", &aux.z);

			vertexs.push_back(aux);
			fscanf_s(fd, "%c", &c, 1); //salto de linea
		}
		if (c == 't')
		{
			std::pair<float, float> st;
			fscanf_s(fd, "%c", &c, 1); //espacio
			fscanf_s(fd, "%f", &st.first);
			fscanf_s(fd, "%c", &c, 1); //espacio
			fscanf_s(fd, "%f", &st.second);

			texcoords.push_back(st);
			fscanf_s(fd, "%c", &c, 1); //salto de linea
		}
		if (c == 'n')
		{
			Coord aux;
			fscanf_s(fd, "%c", &c, 1); //espacio
			fscanf_s(fd, "%f", &aux.x);
			fscanf_s(fd, "%c", &c, 1); //espacio
			fscanf_s(fd, "%f", &aux.y);
			fscanf_s(fd, "%c", &c, 1); //espacio
			fscanf_s(fd, "%f", &aux.z);

			normals.push_back(aux);
			fscanf_s(fd, "%c", &c, 1); //salto de linea
		}
		if (c == 'f')
		{
			std::vector< std::vector<int> > points;
			std::vector<int> point(3);

			fscanf_s(fd, "%c", &c, 1); //espacio

			do {
				fscanf_s(fd, "%d", &point[0]);
				fscanf_s(fd, "%c", &c, 1); //separador
				fscanf_s(fd, "%d", &point[1]);
				fscanf_s(fd, "%c", &c, 1); //separador
				fscanf_s(fd, "%d", &point[2]);

				points.push_back(point);
				fscanf_s(fd, "%c", &c, 1); //espacio o salto de linea
			} while (c == ' ');

			faces.push_back(points);
		}

	} while (!feof(fd));

	int dl = glGenLists(1);
	glNewList(dl, GL_COMPILE);

	std::cout << "Model loaded: " << vertexs.size() << " vertexs, " << texcoords.size() << " texcoords, " << normals.size() << " normals, " << faces.size() << " faces." << std::endl;
/*
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		if (faces[i].size() == 3)
		{
			glBegin(GL_TRIANGLES);
			glTexCoord2f(texcoords[faces[i][0][1] - 1].first, texcoords[faces[i][0][1] - 1].second);
			glNormal3f(normals[faces[i][0][2] - 1].x, normals[faces[i][0][2] - 1].y, normals[faces[i][0][2] - 1].z); //TO FIX: normals per face?
			glVertex3f(vertexs[faces[i][0][0] - 1].x, vertexs[faces[i][0][0] - 1].y, vertexs[faces[i][0][0] - 1].z);

			glTexCoord2f(texcoords[faces[i][1][1] - 1].first, texcoords[faces[i][1][1] - 1].second);
			glNormal3f(normals[faces[i][1][2] - 1].x, normals[faces[i][1][2] - 1].y, normals[faces[i][1][2] - 1].z);
			glVertex3f(vertexs[faces[i][1][0] - 1].x, vertexs[faces[i][1][0] - 1].y, vertexs[faces[i][1][0] - 1].z);

			glTexCoord2f(texcoords[faces[i][2][1] - 1].first, texcoords[faces[i][2][1] - 1].second);
			glNormal3f(normals[faces[i][2][2] - 1].x, normals[faces[i][2][2] - 1].y, normals[faces[i][2][2] - 1].z);
			glVertex3f(vertexs[faces[i][2][0] - 1].x, vertexs[faces[i][2][0] - 1].y, vertexs[faces[i][2][0] - 1].z);
			glEnd();
		}
		else
		{
			glBegin(GL_QUADS);
			glTexCoord2f(texcoords[faces[i][0][1] - 1].first, texcoords[faces[i][0][1] - 1].second);
			glNormal3f(normals[faces[i][0][2] - 1].x, normals[faces[i][0][2] - 1].y, normals[faces[i][0][2] - 1].z);
			glVertex3f(vertexs[faces[i][0][0] - 1].x, vertexs[faces[i][0][0] - 1].y, vertexs[faces[i][0][0] - 1].z);

			glTexCoord2f(texcoords[faces[i][1][1] - 1].first, texcoords[faces[i][1][1] - 1].second);
			glNormal3f(normals[faces[i][1][2] - 1].x, normals[faces[i][1][2] - 1].y, normals[faces[i][1][2] - 1].z);
			glVertex3f(vertexs[faces[i][1][0] - 1].x, vertexs[faces[i][1][0] - 1].y, vertexs[faces[i][1][0] - 1].z);

			glTexCoord2f(texcoords[faces[i][2][1] - 1].first, texcoords[faces[i][2][1] - 1].second);
			glNormal3f(normals[faces[i][2][2] - 1].x, normals[faces[i][2][2] - 1].y, normals[faces[i][2][2] - 1].z);
			glVertex3f(vertexs[faces[i][2][0] - 1].x, vertexs[faces[i][2][0] - 1].y, vertexs[faces[i][2][0] - 1].z);

			glTexCoord2f(texcoords[faces[i][3][1] - 1].first, texcoords[faces[i][3][1] - 1].second);
			glNormal3f(normals[faces[i][3][2] - 1].x, normals[faces[i][3][2] - 1].y, normals[faces[i][3][2] - 1].z);
			glVertex3f(vertexs[faces[i][3][0] - 1].x, vertexs[faces[i][3][0] - 1].y, vertexs[faces[i][3][0] - 1].z);
			glEnd();
		}
	}
	*/

	glEndList();
	fclose(fd);

	return dl;
}

void Model::loadModel(const std::string& path, int listID) {
	// Load model from file and create display list
	listID = GetDisplayList(const_cast<char*>(path.c_str()));
}

void Model::drawModel(int model_id) {
	// Draw the model using the display list
	glCallList(model_id);
}

