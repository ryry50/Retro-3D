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

//SCREEN SETTINGS
int width = 640, height = 480;
GLdouble nearPlane = 1.0, farPlane = 50.0;
GLdouble left = -1, right = 1, bottom = -0.75, top = 0.75;

//OBJECT VARIABLES
float speed = 0.07;
float h = 2.0f;

//CAMERA VARIABLES
glm::vec3 camTran = glm::vec3(0.0f, 2.0f, 8.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraForward = glm::vec3(0.0f, 0.0f, -1.0f);
bool firstMouse = true;
double yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
double pitch = 0.0f;
double lastX = width / 2.0;
double lastY = height / 2.0;
double fov = 45.0f;

//TIMING
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
        camTran += speed * cameraForward;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camTran -= speed * cameraForward;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camTran -= speed * glm::normalize(glm::cross(cameraForward, cameraUp));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camTran += speed * glm::normalize(glm::cross(cameraForward, cameraUp));
    
    //camTran.y = h; //lock y position
    
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

static void drawCubeTex(double size) {
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, -size, size);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, -size,-size);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( size, -size,-size);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( size, -size, size);
    glEnd();

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size, size);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, size,-size);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( size, size,-size);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( size, size, size);
    glEnd();

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size,-size);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-size,-size,-size);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( size,-size,-size);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( size, size,-size);
    glEnd();

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size, size);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-size,-size, size);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( size,-size, size);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( size, size, size);
    glEnd();

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-size,-size, size );
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-size,-size,-size );
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, size,-size );
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, size, size);
    glEnd();

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(size, -size, size);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(size, -size, -size);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(size, size, -size);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size, size);
    glEnd();
}

static void drawRecTex(double length, double width, double height) {
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-length, -height, width);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-length, -height, -width);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(length, -height, -width);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(length, -height, width);
    glEnd();

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-length, height, width);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-length, height, -width);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(length, height, -width);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(length, height, width);
    glEnd();

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-length, height, -width);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-length, -height, -width);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(length, -height, -width);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(length, height, -width);
    glEnd();

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-length, height, width);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-length, -height, width);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(length, -height, width);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(length, height, width);
    glEnd();

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-length, -height, width);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-length, -height, -width);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-length, height, -width);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-length, height, width);
    glEnd();

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(length, -height, width);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(length, -height, -width);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(length, height, -width);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(length, height, width);
    glEnd();
}


static void checkerBox(int x, int z, float *first, float *second) {
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

static void light( float color1[], float pos1[], float color2[], float pos2[], float color3[], float pos3[], float cuttof, float dir[], float ambient[], float spec[]) {
    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);

    glLightfv(GL_LIGHT0, GL_AMBIENT, color2);
    glLightfv(GL_LIGHT0, GL_POSITION, pos1);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, color1);
    glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT1, GL_POSITION, pos2);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, color2);
    glLightfv(GL_LIGHT1, GL_SPECULAR, spec);

    glLightfv(GL_LIGHT2, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT2, GL_POSITION, pos3);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, dir);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, cuttof);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, color3);
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

    //initialize font, texture and renderer
    Font font;
    font.init();

    Render render;
    render.init(positions, colors);

    Transform move;
    Transform move2;
    glm::vec3 diamond = move.init(glm::vec3(0, 10, 1));
	glm::vec3 rec = move2.init(glm::vec3(1.7, 0.5, 1));

    glEnable(GL_TEXTURE_2D);
    Texture texture("res/Textures/Met.jpg");
    Texture checker("res/Textures/tilesSmall.png");
    
    double view[] = { left, right, bottom, top, nearPlane, farPlane };
	light(green, lightPos0, blue, lightPos1, white, lightPos2, light2Cutoff, lightPos2Dir, black, white);

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

        double lastTime = 0.0;
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        int x = 0, z = 0;
        //floor
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glShadeModel(GL_SMOOTH);
        
        {
            texture.bind();
            render.drawStart(view, glm::vec3(0, 1, 1), camTran, cameraFront, cameraUp, glm::vec3(0), NULL);
            drawRecTex(4,  16, 4);
            texture.unBind();
        }

        light(green, lightPos0, blue, lightPos1, white, lightPos2, light2Cutoff, lightPos2Dir, black, white);
        
        //cube 
        {
            checker.bind();
            render.drawStart(view, rec, camTran, cameraFront, cameraUp, glm::vec3(1, 0, 0), 0);
            drawRecTex(1, 0.5, 0.5);
            checker.unBind();
            
        }

        //Gem
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            render.draw(indices, sizeof(indices) / sizeof(indices[0]), greenTran, 
                view, diamond, camTran, cameraFront, cameraUp,
                glm::vec3(0, 1, 0), 0, 0);
            glDisable(GL_BLEND);
            diamond = move.transLinear(glm::vec3(0, 0.5, 1), 3);
            /*
            if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
                diamond = move.transLinear(glm::vec3(3, 5, 1), 7);
            if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
                diamond = move.transOut(glm::vec3(-4, 2, 1), 7);
            */

        }

        //Text
        {
            font.init();
            font.reshape(width, height);
            GLfloat white[3] = { 1.0, 1.0, 1.0, };
            font.printO("RYERS EMERALD OR SMN LIKE THAT", 300, 50, white);
            font.drawUI(imageData.pixel_data, 5, 5, 320, 240, white);
        }

       
        render.flush(window);

    }
    glfwTerminate();
    return 0;
}


void mouseInput(GLFWwindow* window, double xposIn, double yposIn) {
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
        glm::vec3 forward;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        forward.x = cos(glm::radians(yaw));
        forward.y = 0;
        forward.z = sin(glm::radians(yaw));

        cameraFront = glm::normalize(front);
		cameraForward = glm::normalize(forward);

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