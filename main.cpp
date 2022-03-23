#include <GLFW/glfw3.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <thread>

using namespace std;

const GLdouble G = 6.67e-11;

struct Coordinate {
    GLdouble x;
    GLdouble y;
};

static GLdouble getForce(GLdouble mass1,GLdouble mass2,GLdouble radius){
    return (G * ((mass1 * mass2)/(radius*radius)));
}

static struct Coordinate forceVector(GLdouble gravForce, float angle ){
    struct Coordinate force;
    force.x = gravForce * cos(angle);
    force.y = gravForce * sin(angle);
    return force;
}

static float getAngle(GLdouble x1, GLdouble x2, GLdouble y1, GLdouble y2){
    return atan2(y1 - y2, x1 - x2);
}

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key == GLFW_KEY_D && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (key == GLFW_KEY_C && action == GLFW_PRESS)
    {
        glClearColor(0.0, 0.0, 0.15, 0.5);
    }
}

void drawLine(GLfloat x, GLfloat y){
    glBegin(GL_LINES);
    glVertex2f(x, y);
    glVertex2f(x, y);
    glColor3f(1.0f,1.0f,0.0f);
    glEnd();
}


void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius){
	int i;
    
	int triangleAmount = 80; //# of triangles used to draw circle
	
	//GLfloat radius = 0.8f; //radius
	GLfloat twicePi = 2.0f * M_PI;
	
	glBegin(GL_TRIANGLE_FAN);
		glVertex2f(x, y); // center of circle
		for(i = 0; i <= triangleAmount;i++) { 
			glVertex2f(
		            x + (radius * cos(i *  twicePi / triangleAmount)), 
			    y + (radius * sin(i * twicePi / triangleAmount))
			);
		}
	glEnd();
}

GLfloat get_polar_x(GLfloat dist, GLfloat angle) {
    return dist * cos(angle);
}

GLfloat get_polar_y(GLfloat dist, GLfloat angle, float tilt) {
    return dist * sin(angle) * (1 - tilt);
}

class Planet{
public:
    GLdouble mass;
    GLdouble x;
    GLdouble y;
    GLdouble vel_x;
    GLdouble vel_y;
    Planet(GLdouble m1, GLdouble x1, GLdouble y1, GLdouble vel_x1, GLdouble vel_y1){
        mass = m1;
        x = x1;
        y = y1;
        vel_x = vel_x1;
        vel_y = vel_y1;
    }

    GLdouble get_radius() {
        return mass;
    };

    GLdouble get_distance(Planet other) {
        return sqrt(pow((x - other.x), 2) + pow((y - other.y), 2));
    };

    void update() {
        x = next_x;
        y = next_y;
        vel_x = next_vel_x;
        vel_y = next_vel_y;
    }
};

class Universe{
public:
    Universe(GLdouble mass){
        // Creates the inner most planet 
        central_mass = mass;
    }
    vector<Planet*> allPlanets;
    GLdouble central_mass;
    GLdouble get_central_radius() {
        return central_mass;
    };
    void addPlanet(Planet *newPlanet){
        // Adds a new planet to the universe
        allPlanets.push_back(newPlanet);
    };
    void draw() {
        drawFilledCircle(0.0f, 0.0f, central_mass);
        for(auto planet : allPlanets) {
            drawFilledCircle(planet->x, planet->y, planet->mass);
        }
    }
    void update() {
        //TODO implement ;^)
        for(auto planet : allPlanets) {
            // Set next frames values
            float angle = getAngle(0.0f, planet->x, 0.0f, planet->y)
        }
        for(auto planet : allPlanets) {
            // Update position and reset next frame values

        }
    }
};

//sun to earth = 149.11 million km
//sun's mass = 1.989 × 10^30 kg
//earth's mass = 5.972 × 10^24 kg



void interactiveWindow(){
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    window = glfwCreateWindow(1080, 1080, "Control Station", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
}


int main(void)
{
    GLclampd tilt = 0.0f;
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    window = glfwCreateWindow(1080, 1080, "The edge of space", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    Universe *u = new Universe(0.1f);
    Planet *earth = new Planet(0.05f, 0.5f, 0.5f, 1.0f, 0.0f);
    u->addPlanet(earth);
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        float ratio;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;
        u->draw();
        u->update();
        glViewport(0, 0, width, height);
        glfwSwapBuffers(window);
        glfwPollEvents();
        glFlush();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}