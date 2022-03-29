#include <GLFW/glfw3.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <thread>
#include <chrono>
const GLdouble RATIO = 0.8f / (230.0 * 1000000000.0);
const GLdouble SECONDS_PER_FRAME = 2000;
using namespace std;

const GLdouble G = 6.67e-11;

struct Coordinate {
    GLdouble x;
    GLdouble y;
};

static GLdouble getForce(GLdouble mass1,GLdouble mass2,GLdouble radius){
    return (G * ((mass1 * mass2)/(radius*radius)));
}

static struct Coordinate forceVector(GLdouble gravForce, GLdouble angle ){
    struct Coordinate force;
    force.x = gravForce * cos(angle);
    force.y = gravForce * sin(angle);
    return force;
}

static double getAngle(GLdouble x1, GLdouble x2, GLdouble y1, GLdouble y2){
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

    GLdouble get_distance(GLdouble x2, GLdouble y2) {
        return sqrt(pow((x - x2), 2) + pow((y - y2), 2));
    };

    void update(vector<Planet*> solarSystem) {
        for(auto planet : solarSystem) {
            if (planet != this) {
                //cout << "X: " << planet->x << ", Y:"<< planet->y << endl;
                //cout << "X_vel: " << planet->vel_x << ", Y_vel:"<< planet->vel_y << endl;
                float angle = getAngle(planet->x, this->x, planet->y, this->y);
                //cout << "Angle: " << angle*(180/M_PI) << endl;
                //cout << "Angle: " << angle << endl;
                GLdouble distance = this->get_distance(planet->x, planet->y);
                GLdouble force = getForce(this->mass, planet->mass, distance);
                //cout << "Force: " << force << endl;
                struct Coordinate forceVec = forceVector(force, angle);
                //cout << "Force x: " << forceVec.x << endl;
                //cout << "Force y: " << forceVec.y << endl;
                GLdouble acc_x = forceVec.x / this->mass;
                GLdouble acc_y = forceVec.y / this->mass;

                GLdouble increase_x = ((this->vel_x * SECONDS_PER_FRAME) + 0.5 * acc_x * pow(SECONDS_PER_FRAME,2));
                GLdouble increase_y = ((this->vel_y * SECONDS_PER_FRAME) + 0.5 * acc_y * pow(SECONDS_PER_FRAME,2));
                cout << "X increase: " << increase_x << endl;
                cout << "Y increase: " << increase_y << endl;
                
                this->x += increase_x;
                //planet->x += ((planet->vel_x * SECONDS_PER_FRAME));
                this->y += increase_y;
                //planet->y += ((planet->vel_y * SECONDS_PER_FRAME));
                this->vel_x += (acc_x * SECONDS_PER_FRAME);
                this->vel_y += (acc_y * SECONDS_PER_FRAME);
            }
        }
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
        drawFilledCircle(0.0f, 0.0f, 0.065f);
        for(auto planet : allPlanets) {
            //drawFilledCircle(planet->x * RATIO, planet->y * RATIO, planet->mass / (1.9885 * pow(10, 30) / 332950) * 0.025);
            drawFilledCircle(planet->x * RATIO, planet->y * RATIO, 0.01);
        }
    }
    void update() {
        //TODO implement ;^)
        // for(auto planet : allPlanets) {
            
        //     // cout << "X: " << planet->x << ", Y:"<< planet->y << endl;
        //     // //cout << "X_vel: " << planet->vel_x << ", Y_vel:"<< planet->vel_y << endl;
        //     float angle = getAngle(0.0f, planet->x, 0.0f, planet->y);
        //     // cout << "Angle: " << angle*(180/M_PI) << endl;
        //     // //cout << "Angle: " << angle << endl;
        //     GLdouble distance = planet->get_distance(0.0f, 0.0f);
        //     GLdouble force = getForce(central_mass, planet->mass, distance);
        //     // //cout << "Force: " << force << endl;
        //     struct Coordinate forceVec = forceVector(force, angle);
        //     // //cout << "Force x: " << forceVec.x << endl;
        //     // //cout << "Force y: " << forceVec.y << endl;
        //     GLdouble acc_x = forceVec.x / planet->mass;
        //     GLdouble acc_y = forceVec.y / planet->mass;

        //     GLdouble increase_x = ((planet->vel_x * SECONDS_PER_FRAME) + 0.5 * acc_x * pow(SECONDS_PER_FRAME,2));
        //     GLdouble increase_y = ((planet->vel_y * SECONDS_PER_FRAME) + 0.5 * acc_y * pow(SECONDS_PER_FRAME,2));
        //     // cout << "X increase: " << increase_x << endl;
        //     // cout << "Y increase: " << increase_y << endl;
            
        //     planet->x += increase_x;
        //     // planet->x += ((planet->vel_x * SECONDS_PER_FRAME));
        //     planet->y += increase_y;
        //     // planet->y += ((planet->vel_y * SECONDS_PER_FRAME));
        //     planet->vel_x += (acc_x * SECONDS_PER_FRAME);
        //     planet->vel_y += (acc_y * SECONDS_PER_FRAME);
        // }
        for(auto planet : allPlanets) {
            // Update position and reset next frame values
            planet->update(allPlanets);
        }

    }
    // https://stackoverflow.com/questions/54551371/creating-thread-inside-a-for-loop-c
    void threaded_update(){
        vector<std::thread*> ThreadVector;

        for(auto planet : allPlanets){
            ThreadVector.emplace_back([&]{planet->update(allPlanets);}); // Pass by reference here, make sure the object lifetime is correct
        }
        for(auto t : ThreadVector){
            t->join();
        }
    }
};

//sun to earth = 149.11 million km
//sun's mass = 1.989 × 10^30 kg
//earth's mass = 5.972 × 10^24 kg

//mars
//sun to mars = 230 million km
//mars's mass = 6.39 × 10^23
//mars's speed = 29772,864

//venus 
//sun to venus = 108.41 million km
//venus's mass = 4.867 × 10^24 kg
//venus's speed = 35021

//mercury
//sun to mercury = 54.385 million km
//mercury's mass = 3.285 × 10^23 kg
//mercury's speed = 47362.5

//jupiter
//sun to jupiter = 744.7 million km
//jupiter's mass = 1.898 × 10^27 kg
//jupiter's speed = 13069

//saturn 
//1.4788 billion km
//mass = 5.683 × 10^26 kg
//speed = 9672

//uranus
//2.9479 billion km
//mass = 8.681 × 10^25 kg
//6800

//neptune
//4,498,396,441km
//mass = 1.024 × 10^26 kg
//5435

//the moon
//384,400 km to earth
//1028 ms
//7.34767309 × 10^22

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
    GLdouble sun_mass = 1.9885 * pow(10, 30);
    //GLdouble earth_mass = sun_mass * (3.003 * pow(10, -6));
    GLdouble earth_mass = (sun_mass/332950);    
    GLdouble earth_x = 0.0;
    GLdouble earth_y = 149.11 * 1000000000;
    GLdouble earth_vel_y = 0.0;
    GLdouble earth_vel_x = 29870.0;

    GLdouble moon_mass = 7.34767309 * pow(10, 22);    
    GLdouble moon_x = 0.0;
    GLdouble moon_y = 149.11 * 1000000000 - 340000000;
    GLdouble moon_vel_y = 0.0;
    GLdouble moon_vel_x = 29870.0 - 2*1022.0;

    //halastjörnu braut:     
    //GLdouble earth_vel_y = 5000.0;
    //GLdouble earth_vel_x = 21000.0;//sqrt((G * sun_mass) / earth_x);
    //GLdouble earth_x = 0.0;
    //GLdouble earth_y = 149.11 * 1000000000;

    GLdouble mars_mass =  6.39 * pow(10, 23);
    GLdouble mars_x = 0.0;
    GLdouble mars_y = 230.0 * 1000000000.0;
    GLdouble mars_vel_y = 0.0;
    GLdouble mars_vel_x = 24072.0;
    //GLdouble mars_vel_x = 0.0;

    GLdouble venus_mass =  4.867 * pow(10, 24);
    GLdouble venus_x = 0.0;
    GLdouble venus_y = 108.41 * 1000000000.0;
    GLdouble venus_vel_y = 0.0;
    GLdouble venus_vel_x = 35021.561;
    //GLdouble venus_vel_x = 0.0;

    GLdouble mercury_mass =3.285 * pow(10, 23);;
    GLdouble mercury_x = 0.0;
    GLdouble mercury_y = 54.385 * 1000000000.0;
    GLdouble mercury_vel_y = 0.0;
    GLdouble mercury_vel_x = 47362.5;
    //GLdouble mercury_vel_x = 0.0;

//jupiter
//sun to jupiter = 744.7 million km
//jupiter's mass = 1.898 × 10^27 kg
//jupiter's speed = 13069
    GLdouble jupiter_mass =3.285 * pow(10, 23);;
    GLdouble jupiter_x = 0.0;
    GLdouble jupiter_y = 54.385 * 1000000000.0;
    GLdouble jupiter_vel_y = 0.0;
    GLdouble jupiter_vel_x = 47362.5;
//saturn 
//1.4788 billion km
//mass = 5.683 × 10^26 kg
//speed = 9672

//uranus
//2.9479 billion km
//mass = 8.681 × 10^25 kg
//6800

//neptune
//4,498,396,441km
//mass = 1.024 × 10^26 kg
//5435

    Universe *u = new Universe(sun_mass);
    Planet *sun = new Planet(sun_mass * 4, 0.0, 0.0, 0.0, 0.0);
    Planet *earth = new Planet(earth_mass, earth_x, earth_y, earth_vel_x, earth_vel_y);
    Planet *mars = new Planet(mars_mass, mars_x, mars_y, mars_vel_x, mars_vel_y);
    Planet *venus = new Planet(venus_mass, venus_x, venus_y, venus_vel_x, venus_vel_y);
    Planet *mercury = new Planet(mercury_mass,mercury_x,mercury_y,mercury_vel_x,mercury_vel_y);
    Planet *moon = new Planet(moon_mass, moon_x, moon_y, moon_vel_x, moon_vel_y);
    u->addPlanet(sun);
    u->addPlanet(earth);
    u->addPlanet(mars);
    u->addPlanet(venus);
    u->addPlanet(mercury);
    u->addPlanet(moon);
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
        //this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}