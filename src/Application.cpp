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
#include "Model.h"

//WORLD PHYSICS
float grav = 0.01;
float ground = 0;

//PLAYER
float playerH = 3.0f;
float vsp = 0;
bool idle = true;
bool isGrounded;
bool isCrouched = false;
int ammo = 60;
float speed = 0.1;
float health = 100.0f;
bool death = false;

unsigned long debounceDelay = 10;

//GUN
bool shooting = false;
float range = 1000;
glm::vec3 rayCast;

//SCREEN SETTINGS
int width = 640, height = 480;
GLdouble nearPlane = 1.0, farPlane = 1000.0;
GLdouble left = -1, right = 1, bottom = -0.75, top = 0.75;
float screenScroll = 0.0f;
bool transition = false;

//OBJECT VARIABLES
double enemyHealth = 700.0f;
//Transformations
Transform move;
Transform move2;
Transform move3;
glm::vec3 diamond = move.init(glm::vec3(0, 1000, 1));
glm::vec3 rec = move3.init(glm::vec3(1.7, 0, -3));
glm::vec3 gunPos = move2.init(glm::vec3(0.8, -1, -1.2));


//CAMERA VARIABLES
glm::vec3 camTran = glm::vec3(0.0f, playerH, 8.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraForward = glm::vec3(0.0f, 0.0f, -1.0f);
bool firstMouse = true;
double yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
double pitch = 0.0f;
double lastX = width / 2.0;
double lastY = height / 2.0;
double fov = 45.0f;
int angle = 0;

//TIMING
double lastTime = 0.0;
double currentTime = glfwGetTime();
double deltaTime = currentTime - lastTime;

enum screen{
    MENU = 0,
    GAME,
    GAMEOVER,
    END
};
screen currentScreen = MENU;

void window_size_callback(GLFWwindow* window, int x, int y)
{
    width = x;
    height = y;
}

static void crouchCheck() {
    if (isCrouched) {
        playerH = 1.5f;
    }
    else {
        playerH = 3.0f;
    }
}
static bool boundCheck(float bound) {
    return camTran.x <= bound && camTran.x >= -bound
        && camTran.z <= bound && camTran.z >= -bound;
}

static void groundCheck() {
    if (!isGrounded ) {
        camTran.y += vsp;
        if(vsp > -1000.0f)
            vsp -= grav;
    }
    if (isGrounded) {
        camTran.y = playerH + ground;
        vsp = 0;
    }
}

bool hitBox(glm::vec3 bound, glm::vec3 obj, glm::vec3 first) {
    return glm::all(glm::lessThanEqual(first, obj + bound)) &&
        glm::all(glm::greaterThanEqual(first, obj - bound));
}

bool hit(glm::vec3 bound, glm::vec3 obj) {
    rayCast = camTran;
    for (int i = 0; i < range; i++) {
        rayCast += cameraFront * 0.1f;
        if (hitBox(bound, obj, rayCast)) return true;
    }
    return false;
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
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		idle = false;
    }
    else {
		idle = true;
    }

    //camTran.y = h; //lock y position
    
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && isGrounded) {
        vsp = 0.25;
        isGrounded = false;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        speed = 0.15;
    else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
        speed = 0.1;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		isCrouched = true;
    else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
		isCrouched = false;

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && ammo < 60) {
        ammo = 60;
        std::cout << "Reloaded!" << std::endl;
    }
     
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

static void drawFloorTex(float size, int repeat) {
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, 0, size);
    glTexCoord2f(repeat, 0.0f); glVertex3f(size, 0, size);
    glTexCoord2f(repeat, repeat); glVertex3f(size, 0, -size);
    glTexCoord2f(0.0f, repeat); glVertex3f(-size, 0, -size);
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

void mouseButton_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
		transition = true;
    }
}

static void displayEnemyHealth() {
    for(int i = 0; i < enemyHealth / 10; i++) {
		glBegin(GL_QUADS);
		glVertex2f(-0.9f + i * 0.02f, 0.9f);
		glVertex2f(-0.86f + i * 0.02f, 0.9f);
		glVertex2f(-0.86f + i * 0.02f, 0.95f);
		glVertex2f(-0.9f + i * 0.02f, 0.95f);
		glEnd();
	}
}

static void restart() {
   camTran = glm::vec3(0.0f, playerH, 8.0f);
   cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
   cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
   cameraForward = glm::vec3(0.0f, 0.0f, -1.0f);

    diamond = move.init(glm::vec3(0, 1000, 1));
    rec = move3.init(glm::vec3(1.7, 0, -3));
    gunPos = move2.init(glm::vec3(0.8, -1, -1.2));

    vsp = 0;
    death = false;

	enemyHealth = 700.0f;
	ammo = 60;
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
    float yellow[] = { 1.0f, 1.0f, 0.0f, 1.0f };
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

	//Renderer
    Render render;
    render.init(positions, colors);

    //Models
	Model gun;
    Model bot;

    //Textures
    glEnable(GL_TEXTURE_2D);
    Texture texture("res/Textures/Met.jpg");
    Texture checker("res/Textures/DAMN.jpg");
	Texture gunTex("res/Textures/dither_it_AK-47_type_II_noBG.png");
    Texture botTex("res/Textures/1_1767135590_red_metal.jpg");
	Texture skybox("res/Textures/NightDither.jpg");
	Texture menuTex("res/Textures/MenuScreen.png");
    Texture overTex("res/Textures/GameOver.png");
    Texture endTex("res/Textures/END.png");
    
    double view[] = { left, right, bottom, top, nearPlane, farPlane };
	//light(green, lightPos0, blue, lightPos1, white, lightPos2, light2Cutoff, lightPos2Dir, black, white);

    //Load Models
	gun.loadModel("res/mesh/gun/AK.obj");
    bot.loadModel("res/mesh/bot/PolyBotAnim.obj");

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {

        currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;

		//std::cout << "FPS: " << deltaTime << std::endl;
		//MENU SCREEN
		if(deltaTime > 1/60){
		lastTime = currentTime;
        if(currentScreen == MENU) {
            restart();
            /* Render here */
            render.clear();
            glfwSetWindowSizeCallback(window, window_size_callback);
            glfwSetWindowAspectRatio(window, 4, 3);
            glViewport(0, 0, width, height);
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, true);

            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
				transition = true;

            if (transition) screenScroll += height / 100;
            
			if (screenScroll >= height / 1.5) currentScreen = GAME;

            //Text
            {
                font.init();
                font.reshape(width, height);
                GLfloat white[3] = { 1.0, 1.0, 1.0, };
				//font.drawUI(menuTex.getLocalBuffer(), menuTex.getWidth(), menuTex.getHeight(), width / 2, height / 2, white);
				menuTex.bind();
				glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f); glVertex2f(0, 0 + screenScroll);
				glTexCoord2f(1.0f, 0.0f); glVertex2f(width, 0 + screenScroll);
				glTexCoord2f(1.0f, 1.0f); glVertex2f(width, height + screenScroll);
				glTexCoord2f(0.0f, 1.0f); glVertex2f(0, height + screenScroll);
				glEnd();
				menuTex.unBind();
                font.printO("LEFT CLICK TO CONTINUE", width / 3, height / 10, white);
            }

			render.flush(window);
        }

		//Main Game Screen
        else if(currentScreen == GAME) {
            /* Render here */
            render.clear();
            glfwSetWindowSizeCallback(window, window_size_callback);
            glfwSetWindowAspectRatio(window, 4, 3);
            glfwSetCursorPosCallback(window, mouseInput);
            isGrounded = camTran.y - ground <= playerH && boundCheck(40) && camTran.y > ground;
            processInput(window);
            transition = false;
			screenScroll = 0;

            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && ammo > 0) {
                ammo--;
                shooting = true;
                //std::cout << "Ammo: " << ammo << std::endl;
            }
            else {
                shooting = false;
            }

            glViewport(0, 0, width, height);

            angle++;

            crouchCheck();
            groundCheck();

            //floor
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_LIGHTING);
            glShadeModel(GL_SMOOTH);

            //skybox
            {
                skybox.bind();
                render.drawStart(view, glm::vec3(0, ground, 0), camTran / 10.0f, cameraFront, cameraUp, glm::vec3(0), NULL);
                //drawRecTex(4,  16, 4);
                drawCubeTex(500);
                skybox.unBind();
            }

            //Floor
            {
                texture.bind();
                render.drawStart(view, glm::vec3(0, ground, 0), camTran, cameraFront, cameraUp, glm::vec3(0), NULL);
                //drawRecTex(4,  16, 4);
                drawFloorTex(40, 8);
                texture.unBind();
            }

            light(yellow, lightPos0, blue, lightPos1, white, lightPos2, light2Cutoff, lightPos2Dir, black, white);

            //BOX 
            {
                botTex.bind();
                render.drawStart(view, rec, camTran, cameraFront, cameraUp, glm::vec3(0, 1, 0), (glm::atan((rec.x - camTran.x) / (rec.z - camTran.z)) * 45) - 90);
                bot.drawModel();
                botTex.unBind();
                if(enemyHealth > 0)
					rec = move3.transLinear(glm::vec3(camTran.x, 0, camTran.z), 3);
                if (hit(glm::vec3(1, 3.5, 1), rec) && enemyHealth > 0 && shooting) {
                    enemyHealth -= 1.0f;
                    //std::cout << "Pointing" << std::endl;
                    std::cout << "Enemy Health: " << enemyHealth << std::endl;
                }
                if (enemyHealth <= 0) {
                    rec = move3.transLinear(glm::vec3(rec.x, -5, rec.z), 3);
                }
            }

            if (hitBox(glm::vec3(2, 3.5, 2), rec, camTran) && enemyHealth > 0 || camTran.y < -200) {
                death = true;
            }

            //Gem
            {
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                render.draw(indices, sizeof(indices) / sizeof(indices[0]), greenTran,
                    view, diamond, camTran, cameraFront, cameraUp,
                    glm::vec3(0, 1, 0), 2, angle);
                glDisable(GL_BLEND);
                if (enemyHealth <= 0)
                    diamond = move.transOut(glm::vec3(0, 3, 1), 5);
                /*
                if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
                    diamond = move.transLinear(glm::vec3(3, 5, 1), 7);
                if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
                    diamond = move.transOut(glm::vec3(-4, 2, 1), 7);
                */

            }

            if (hitBox(glm::vec3(1, 1, 1), diamond, camTran)) {
                currentScreen = END;
            }

            //gun
            {

                if (!idle && !shooting) {
                    gunPos = move2.transSin(glm::vec3(0.8, -1, -1.0), glm::vec3(0.8, -1, -1.3), 10);
                }
                else if (shooting) {
                    gunPos = move2.transSin(glm::vec3(0.8, -1, -1.0), glm::vec3(0.8, -1, -1.3), 100);
                }
                else {
                    gunPos = move2.transSin(glm::vec3(0.8, -1, -1.0), glm::vec3(0.8, -0.9, -1.2), 2);
                }
                gunTex.bind();
                render.drawStartNoCam(view, gunPos, glm::vec3(0, 1, 0), 180);
                gun.drawModel();
                gunTex.unBind();
            }


            //Text and UI
            {
                font.init();
                font.reshape(width, height);
                GLfloat white[3] = { 1.0, 1.0, 1.0, };
                font.printO("ENEMY HEALTH: ", width / 9, height / 1.2, white);
                if(ammo <= 0)
                    font.printO("PRESS R TO RELOAD ", width / 2, height / 1.4, red);
                font.drawUI(imageData.pixel_data, 5, 5, width / 2, height / 2, white);
                render.drawStartNoCam(view, glm::vec3(0.5, -0.4, -1.0), glm::vec3(0), 0);
				displayEnemyHealth();
            }

            if (death) {
				currentScreen = GAMEOVER;
            }

            render.flush(window);
        }

		//Game Over Screen
        else if(currentScreen == GAMEOVER) {
            transition = false;
            /* Render here */
            render.clear();
            glfwSetWindowSizeCallback(window, window_size_callback);
            glfwSetWindowAspectRatio(window, 4, 3);
            glViewport(0, 0, width, height);
			lastTime = currentTime;
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, true);
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
                lastTime = currentTime;
                currentScreen = MENU;
            }
            //Text
            {
                font.init();
                font.reshape(width, height);
                GLfloat white[3] = { 1.0, 1.0, 1.0, };
                //font.drawUI(menuTex.getLocalBuffer(), menuTex.getWidth(), menuTex.getHeight(), width / 2, height / 2, white);
                overTex.bind();
                glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 0.0f); glVertex2f(0, 0);
                glTexCoord2f(1.0f, 0.0f); glVertex2f(width, 0);
                glTexCoord2f(1.0f, 1.0f); glVertex2f(width, height);
                glTexCoord2f(0.0f, 1.0f); glVertex2f(0, height);
                glEnd();
                overTex.unBind();
                font.printO("WOW, CAN YOU PLAY VIDEO GAMES", width / 3, height / 10, white);
            }
            render.flush(window);
		}

        else if (currentScreen == END) {
            transition = false;
            /* Render here */
            render.clear();
            glfwSetWindowSizeCallback(window, window_size_callback);
            glfwSetWindowAspectRatio(window, 4, 3);
            glViewport(0, 0, width, height);
            lastTime = currentTime;
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, true);
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
                lastTime = currentTime;
                currentScreen = MENU;
            }
            //Text
            {
                font.init();
                font.reshape(width, height);
                GLfloat white[3] = { 1.0, 1.0, 1.0, };
                //font.drawUI(menuTex.getLocalBuffer(), menuTex.getWidth(), menuTex.getHeight(), width / 2, height / 2, white);
                endTex.bind();
                glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 0.0f); glVertex2f(0, 0);
                glTexCoord2f(1.0f, 0.0f); glVertex2f(width, 0);
                glTexCoord2f(1.0f, 1.0f); glVertex2f(width, height);
                glTexCoord2f(0.0f, 1.0f); glVertex2f(0, height);
                glEnd();
                endTex.unBind();
                font.printO("YOU'RE WINNER", width / 3, height / 12, white);
            }
            render.flush(window);
        }
		}
    }
    glfwTerminate();
    return 0;
}


void mouseInput(GLFWwindow* window, double xposIn, double yposIn) {
    if (currentScreen == GAME) {

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
    else if (currentScreen == MENU || currentScreen == END || currentScreen == GAMEOVER)
    {
        // Unhides cursor since camera is not looking around anymore
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        // Makes sure the next time the camera looks around it doesn't jump
        firstMouse = true;
    }
}
