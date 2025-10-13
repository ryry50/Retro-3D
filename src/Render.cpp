#include "Render.h"

void Render::init(float vertex[], float color[]) {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    //glEnableClientState(GL_TEXTURE_2D_ARRAY);
    //glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, vertex);
    glColorPointer(4, GL_FLOAT, 0, color);
    //glTexCoordPointer(2, GL_FLOAT, 0, texPos);
}

void Render::clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Render::flush(GLFWwindow* window){
	glFlush();
	/* Swap front and back buffers */
	glfwSwapBuffers(window);
	/* Poll for and process events */
	glfwPollEvents();
}

void Render::draw(unsigned int indices[], int count, float material[], 
    double frustum[6], float translation[3],
    glm::vec3 camTran, glm::vec3 cameraFront, glm::vec3 cameraUp,
    float rotate[3], float rSpeed) {
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(frustum[0], frustum[1], frustum[2], frustum[3], frustum[4], frustum[5]);
    glTranslatef(translation[0], translation[1], translation[2]);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camTran.x, camTran.y, camTran.z,
        camTran.x + cameraFront.x, (camTran.y + cameraFront.y), camTran.z + cameraFront.z,
        cameraUp.x, cameraUp.y, cameraUp.z);
    glRotatef(rSpeed, rotate[0], rotate[1], rotate[2]);
    glNormal3f(0, 1, 0);

	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, indices);
}

void Render::drawStart(
    double frustum[6], float translation[3],
    glm::vec3 camTran, glm::vec3 cameraFront, glm::vec3 cameraUp,
    float rotate[3], float rSpeed) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(frustum[0], frustum[1], frustum[2], frustum[3], frustum[4], frustum[5]);
    glTranslatef(translation[0], translation[1], translation[2]);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camTran.x, camTran.y, camTran.z,
        camTran.x + cameraFront.x, (camTran.y + cameraFront.y), camTran.z + cameraFront.z,
        cameraUp.x, cameraUp.y, cameraUp.z);
    glRotatef(rSpeed, rotate[0], rotate[1], rotate[2]);
    glNormal3f(0, 1, 0);
}