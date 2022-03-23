#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
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
    return dist * cos(angle) * 1.8f;
}

GLfloat get_polar_y(GLfloat dist, GLfloat angle) {
    return dist * sin(angle) * 0.5;
}


int main(void)
{
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
    GLfloat dist = 0.5f;
    GLfloat angle1 = M_PI / 2;
    GLfloat angle2 = M_PI / 2;
    GLfloat speed1 = 1.0f;
    GLfloat speed2 = 1.7f; 
    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;
        glViewport(0, 0, width, height);
        drawFilledCircle(get_polar_x(dist, angle1), get_polar_y(dist, angle1), 0.006f * (2.0f - get_polar_y(dist, angle1)));
        drawFilledCircle(0.5f * get_polar_x(dist, angle2), 0.5f * get_polar_y(dist, angle2), 0.002f * (2.0f - get_polar_y(dist, angle2)));
        drawFilledCircle(0.0f, 0.0f, 0.05f);
        glfwSwapBuffers(window);
        glfwPollEvents();
        angle1 += 0.005f * speed1;
        angle2 += 0.005f * speed2;
        //dist -= 0.0001f;
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}