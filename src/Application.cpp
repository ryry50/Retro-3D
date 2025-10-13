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

//Initialized variables
int width = 640, height = 480;
GLdouble nearPlane = 1.0, farPlane = 50.0;
GLdouble left = -2, right = 2, bottom = -1.5, top = 1.5;
float rotation = 0.0;
float cRot = 0.0;
float axisSpeed = 1.0;
float down = 10;
float speed = 0.05;
glm::vec3 camTran = glm::vec3(0.0f, 0.5f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
bool firstMouse = true;
double yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
double pitch = 0.0f;
double lastX = width / 2.0;
double lastY = height / 2.0;
double fov = 45.0f;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


void window_size_callback(GLFWwindow* window, int x, int y)
{
    width = x;
    height = y;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camTran += speed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camTran -= speed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camTran -= speed * glm::normalize(glm::cross(cameraFront, cameraUp));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camTran += speed * glm::normalize(glm::cross(cameraFront, cameraUp));
    
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camTran.y += speed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camTran.y -= speed;
        
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}
void mouseInput(GLFWwindow* window, double xposIn, double yposIn);

void drawCubeTex() {
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f,-0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f, -0.5f,-0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f, -0.5f, 0.5f);
    glEnd();

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, 0.5f,-0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f, 0.5f,-0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f, 0.5f, 0.5f);
    glEnd();

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f,-0.5f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f,-0.5f,-0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f,-0.5f,-0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f, 0.5f,-0.5f);
    glEnd();

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f,-0.5f, 0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f,-0.5f, 0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f, 0.5f, 0.5f);
    glEnd();

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f,-0.5f, 0.5f );
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f,-0.5f,-0.5f );
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, 0.5f,-0.5f );
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
    glEnd();

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, -0.5f, 0.5f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, 0.5f, -0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.5f);
    glEnd();
}

void checkerBox(int x, int z, float *first, float *second) {
    for (x = -16; x < 16; x++) {
        for (z = -16; z < 16; z++) {

            if ((x + z) % 2) {
                glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, first);
            }
            else {
                glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, second);
            }
            glBegin(GL_QUADS);
            //glNormal3f(0, 1, 0);
            glVertex3f(x + 0, 0, z + 0);
            glVertex3f(x + 1, 0, z + 0);
            glVertex3f(x + 1, 0, z + 1);
            glVertex3f(x + 0, 0, z + 1);
            glEnd();

            glBegin(GL_QUADS);
            //glNormal3f(0, 1, 0);
            glVertex3f(x + 0, 16, z + 0);
            glVertex3f(x + 1, 16, z + 0);
            glVertex3f(x + 1, 16, z + 1);
            glVertex3f(x + 0, 16, z + 1);
            glEnd();

            glBegin(GL_QUADS);
            //glNormal3f(0, 1, 0);
            glVertex3f(16, x + 0, z + 0);
            glVertex3f(16, x + 1, z + 0);
            glVertex3f(16, x + 1, z + 1);
            glVertex3f(16, x + 0, z + 1);
            glEnd();

            glBegin(GL_QUADS);
            //glNormal3f(0, 1, 0);
            glVertex3f(-16, x + 0, z + 0);
            glVertex3f(-16, x + 1, z + 0);
            glVertex3f(-16, x + 1, z + 1);
            glVertex3f(-16, x + 0, z + 1);
            glEnd();


            glBegin(GL_QUADS);
            glNormal3f(0, 1, 0);
            glVertex3f(x + 0, z + 0, -16);
            glVertex3f(x + 1, z + 0, -16);
            glVertex3f(x + 1, z + 1, -16);
            glVertex3f(x + 0, z + 1, -16);
            glEnd();
        }
    }
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Retro 3D", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSetWindowAspectRatio(window, 4, 3);
    if (glewInit() != GLEW_OK)
        std::cout << "Error" << std::endl;

    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;

    //Vertices  
    float positions[]{
        0.0f,-0.5f, 0.0f,

       -0.25f,0.25f,-0.5f, //1
       -0.5f, 0.25f, 0.0f, //2
       -0.25f,0.25f, 0.5f, //3
        0.25f,0.25f, 0.5f, //4
        0.5f, 0.25f, 0.0f, //5
        0.25f,0.25f,-0.5f, //6

       -0.15f,0.5f,-0.25f, //7
       -0.25f, 0.5f, 0.0f, //8
       -0.15f,0.5f, 0.25f, //9
        0.15f,0.5f, 0.25f, //10
        0.25f, 0.5f, 0.0f, //11
        0.15f,0.5f,-0.25f, //12

        0.0f, 0.5f, 0.0f, //13
    };

    //index buffers indicies
    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        0, 4, 5,
        0, 5, 6,
        0, 6, 1,

        1, 2, 7,
        2, 8, 7,
        2, 3, 8,
        3, 9, 8,
        3, 4, 9,
        4, 10, 9,
        4, 5, 10,
        5, 11, 10,
        5, 6, 11,
        6, 12, 11,
        6, 1, 12,
        1, 7, 12,

        13, 7, 8,
        13, 8, 9,
        13, 9, 10,
        13, 10, 11,
        13, 11, 12,
        13, 12, 7,
    };

    //Texture Coordinates (unused)
    float texPos[] = {
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,

        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,

        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,

        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,

        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,

        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
    };

    //Colors
    float colors[] = {
        0.0f, 0.8f, 0.0f, 0.5f,
        0.0f, 0.8f, 0.0f, 0.5f,
        0.0f, 0.8f, 0.0f, 0.5f,
        0.0f, 0.8f, 0.0f, 0.5f,
        0.0f, 0.8f, 0.0f, 0.5f,
        0.0f, 0.8f, 0.0f, 0.5f,
        0.0f, 0.8f, 0.0f, 0.5f,
        0.0f, 0.8f, 0.0f, 0.5f,
        0.0f, 0.8f, 0.0f, 0.5f,
        0.0f, 0.8f, 0.0f, 0.5f,
        0.0f, 0.8f, 0.0f, 0.5f,
        0.0f, 0.8f, 0.0f, 0.5f,
        0.0f, 0.8f, 0.0f, 0.5f,
        0.8f, 0.0f, 0.0f, 0.5f,
    };

    //Colors and lightPos

    float white[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float red[] = { 1.0f, 0.0f, 0.0f, 1.0f };
    float green[] = { 0.0f, 1.0f, 0.0f, 1.0f };
    float blue[] = { 0.0f, 0.0f, 1.0f, 1.0f };
    float black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float redTran[] = { 1.0f, 0.0f, 0.0f, 0.6f };
    float greenTran[] = { 0.0f, 1.0f, 0.0f, 0.7f };

    float lightPos0[] = { 1.0f, 0.4f, 0.6f, 0.0f };
    float lightPos1[] = { 0.0f, 0.2f, 0.7f, 0.0f };
    float lightPos2[] = { 3.0f, 3.0f, 3.0f, 1.0f };
    float lightPos2Dir[] = { -1.0f, -1.0f, -1.0f };
    float light2Cutoff = 5;
    //Matrix(unused)
    {
        glm::mat4 proj = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 model = glm::mat4(1.0f);

        proj = glm::perspective(glm::radians(45.0f), (float)(width / height), 0.1f, 1500.0f);
        view = glm::translate(glm::mat4(1.0f), camTran);
        model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

        glm::mat4 mvp = proj * view * model;
    }

    Font font;
    font.init();
    Render render;

    glEnable(GL_TEXTURE_2D);
    Texture texture("res/Textures/Met.jpg");
    
    render.init(positions, colors);
    double view[] = { left, right, bottom, top, nearPlane, farPlane };

    //Lights

    {
        glEnable(GL_LIGHTING);
        glShadeModel(GL_SMOOTH);
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
        glEnable(GL_LIGHT2);

        glLightfv(GL_LIGHT0, GL_AMBIENT, blue);
        glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, green);
        glLightfv(GL_LIGHT0, GL_SPECULAR, white);

        glLightfv(GL_LIGHT1, GL_AMBIENT, black);
        glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, blue);
        glLightfv(GL_LIGHT1, GL_SPECULAR, white);

        glLightfv(GL_LIGHT2, GL_AMBIENT, black);
        glLightfv(GL_LIGHT2, GL_POSITION, lightPos2);
        glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, lightPos2Dir);
        glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, light2Cutoff);
        glLightfv(GL_LIGHT2, GL_DIFFUSE, white);
    }


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        render.clear();
        glfwSetWindowSizeCallback(window, window_size_callback);
        glfwSetWindowAspectRatio(window, 4, 3);
        glfwSetCursorPosCallback(window, mouseInput);
        processInput(window);
        glViewport(0, 0, width, height);

        int x = 0, z = 0;
        if (down > 0.5) {
            down -= 0.05;
        }
        axisSpeed = down <= 0.6 ? 10.0 : 1.0;
        rotation += axisSpeed;
        cRot++;
        //floor
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glShadeModel(GL_SMOOTH);
        
        {
			float floorTran[3] = { 0, -1, 1 };
			float floorRot[3] = { 0, 0, 0 };
            render.drawStart(view, floorTran, camTran, cameraFront, cameraUp, floorRot, NULL);
            checkerBox(x, z, black, white);

        }
        
        //Lights
        {
            glEnable(GL_LIGHTING);
            glShadeModel(GL_SMOOTH);
            glLightfv(GL_LIGHT0, GL_AMBIENT, blue);
            glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
            glLightfv(GL_LIGHT0, GL_DIFFUSE, green);
            glLightfv(GL_LIGHT0, GL_SPECULAR, white);

            glLightfv(GL_LIGHT1, GL_AMBIENT, black);

            glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
            glLightfv(GL_LIGHT1, GL_DIFFUSE, blue);
            glLightfv(GL_LIGHT1, GL_SPECULAR, white);

            glLightfv(GL_LIGHT2, GL_AMBIENT, black);
            glLightfv(GL_LIGHT2, GL_POSITION, lightPos2);
            glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, lightPos2Dir);
            glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, light2Cutoff);
            glLightfv(GL_LIGHT2, GL_DIFFUSE, white);
        }
        //Gem
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            float gemTran[3] = {0, down, 1 };
			float gemRot[3] = { 0, 1, 0 };
            render.draw(indices, sizeof(indices) / sizeof(indices[0]), greenTran, 
                view, gemTran, camTran, cameraFront, cameraUp,
                gemRot, rotation);

            glDisable(GL_BLEND);
        }

        //cube 
        texture.bind();
        {
			float cubeTran[3] = { 1.7, 0.5, 1 };
			float cubeRot[3] = { 0, 1, 0 };
            render.drawStart(view, cubeTran, camTran, cameraFront, cameraUp, cubeRot, cRot);
            drawCubeTex();
        }
        texture.unBind();

        //Text
        {
            font.init();
            font.reshape(width, height);
            GLfloat white[3] = { 1.0, 1.0, 1.0, };
            font.printO("RYERS EMERALD OR SMN LIKE THAT", 300, 50, white);
        }

        //glRasterPos2i(200, 200);
        //glDrawPixels(imageData.width, imageData.height, GL_RGBA, GL_UNSIGNED_BYTE, imageData.pixel_data);
       
        render.flush(window);

    }
    //glDeleteTextures(1, &textureID);
    glfwTerminate();
    return 0;
}

void mouseInput(GLFWwindow* window, double xposIn, double yposIn) {
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {

        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
        lastX = xpos;
        lastY = ypos;

        float sensitivity = 0.1f; // change this value to your liking
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(front);

        //std::cout << "pitch" << pitch << std::endl;
        //std::cout << "yaw" << yaw << std::endl;
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
        // Unhides cursor since camera is not looking around anymore
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        // Makes sure the next time the camera looks around it doesn't jump
        firstMouse = true;
    }
}