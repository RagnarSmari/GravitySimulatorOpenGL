#include <GLFW/glfw3.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <thread>
#include <chrono>
#include <mutex>
#include <SDL2/SDL.h>

const GLdouble RATIO = 0.8f / (1198396441000.0);
const GLdouble SECONDS_PER_FRAME = 2000;
const GLclampd tilt = 1.0 - 0.8f;
using namespace std;

mutex draw_mutex;
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


void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius, float red, float green, float blue){
    // Critical section!! draw one at a time
    draw_mutex.lock();
	int i;
    
	int triangleAmount = 12; //# of triangles used to draw circle
	
	//GLfloat radius = 0.8f; //radius
	GLfloat twicePi = 2.0f * M_PI;
	glBegin(GL_TRIANGLE_FAN);
        //glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_TEXTURE_2D);
        glColor3f(red,green,blue);
		glVertex2f(x, y); // center of circle
		for(i = 0; i <= triangleAmount;i++) { 
			glVertex2f(
		            x + (radius * cos(i *  twicePi / triangleAmount)), 
			    y + (radius * sin(i * twicePi / triangleAmount))
			);
		}
	glEnd();
    draw_mutex.unlock();
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
    GLdouble acc_x;
    GLdouble acc_y;
    float red;
    float green;
    float blue;
    Planet(GLdouble m1, GLdouble x1, GLdouble y1, GLdouble vel_x1, GLdouble vel_y1, float red1 = 1.0, float green1 = 1.0, float blue1 = 1.0){
        mass = m1;
        x = x1;
        y = y1;
        vel_x = vel_x1;
        vel_y = vel_y1;
        acc_x = 0.0;
        acc_y = 0.0;
        red = red1;
        green = green1;
        blue = blue1;
    }

    GLdouble get_size() {
        return mass / RATIO;
    };

    GLdouble get_distance(GLdouble x2, GLdouble y2) {
        return sqrt(pow((x - x2), 2) + pow((y - y2), 2));
    };

    void update_acceleration(vector<Planet*> solarSystem) {
        // Reset acceleration
        this->acc_x = 0.0;
        this->acc_y = 0.0;

        for(auto planet : solarSystem) {
            if (planet != this) {
                
                // Calculate force and acceleration relative to current planet
                float angle = getAngle(planet->x, this->x, planet->y, this->y);
                GLdouble distance = this->get_distance(planet->x, planet->y);
                GLdouble force = getForce(this->mass, planet->mass, distance);
                struct Coordinate forceVec = forceVector(force, angle);
                
                // Increment acceleration on x and y axis
                this->acc_x += forceVec.x / this->mass;
                this->acc_y += forceVec.y / this->mass;

                
            }
        }
    }

    void update_position() {
        GLdouble increase_x = ((this->vel_x * SECONDS_PER_FRAME) + 0.5 * this->acc_x * pow(SECONDS_PER_FRAME,2));
        GLdouble increase_y = ((this->vel_y * SECONDS_PER_FRAME) + 0.5 * this->acc_y * pow(SECONDS_PER_FRAME,2));
        
        // Update coordinate and velocity
        this->x += increase_x;
        this->y += increase_y;
        this->vel_x += (this->acc_x * SECONDS_PER_FRAME);
        this->vel_y += (this->acc_y * SECONDS_PER_FRAME);
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
        //drawFilledCircle(0.0f, 0.0f, 0.065);
        for(auto planet : allPlanets) {
            //drawFilledCircle(planet->x * RATIO, planet->y * RATIO, (sqrt(log10(planet->mass)))-4.700*0.01);
            //glClear(GL_COLOR_BUFFER_BIT);
            //glColor3f(planet->red,planet->green,planet->blue);
            GLdouble sixroot = pow(planet->mass, 1.0/8.0);
            GLdouble print_radius = max(sixroot * 0.0000035, 0.0001);
            drawFilledCircle(planet->x * RATIO, planet->y * RATIO * tilt, print_radius, planet->red, planet->green, planet->blue);
            //drawFilledCircle(planet->x * RATIO, planet->y * RATIO, 0.01, planet->red,planet->green,planet->blue);
        }
    }
    void update() {
        for(auto planet : allPlanets) {
            // Update position and reset next frame values
            planet->update_acceleration(allPlanets);
        }
        for(auto planet : allPlanets) {
            planet->update_position();
        }
    }
    // https://stackoverflow.com/questions/54551371/creating-thread-inside-a-for-loop-c
    void threaded_update(){
        thread threads[this->allPlanets.size()];
        int i = 0;
        for(auto planet : this->allPlanets){
            vector<Planet*> solar_system = this->allPlanets;
            threads[i] = thread(&Planet::update_acceleration, planet, solar_system);
            i++;
        }
        for(int i=0; i<this->allPlanets.size(); i++){
            threads[i].join();
        }
        for(auto planet : allPlanets) {
            planet->update_position();
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



int main(int argc, char** argv)
{
    glEnable(GL_COLOR_MATERIAL);
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
    GLdouble moon_vel_x = 29870.0 +1022.0;

    //halastjörnu braut:     
    // GLdouble earth_vel_y = 5000.0;
    // GLdouble earth_vel_x = 21000.0;//sqrt((G * sun_mass) / earth_x);
    // GLdouble earth_y = 0.0;
    // GLdouble earth_x = 149.11 * 1000000000;

    GLdouble mars_mass =  6.39 * pow(10, 23);
    GLdouble mars_y = 0.0;
    GLdouble mars_x = 230.0 * 1000000000.0;
    GLdouble mars_vel_x = 0.0;
    GLdouble mars_vel_y = 24072.0;
    //GLdouble mars_vel_x = 0.0;

    GLdouble venus_mass =  4.867 * pow(10, 24);
    GLdouble venus_x = 0.0;
    GLdouble venus_y = -108.41 * 1000000000.0;
    GLdouble venus_vel_y = 0.0;
    GLdouble venus_vel_x = 35021.561;
    //GLdouble venus_vel_x = 0.0;

    GLdouble mercury_mass =3.285 * pow(10, 23);;
    GLdouble mercury_y = 0.0;
    GLdouble mercury_x = 54.385 * 1000000000.0;
    GLdouble mercury_vel_x = 0.0;
    GLdouble mercury_vel_y = 47362.5;
    //GLdouble mercury_vel_x = 0.0;

//jupiter
//sun to jupiter = 744.7 million km
//jupiter's mass = 1.898 × 10^27 kg
//jupiter's speed = 13069
    GLdouble jupiter_mass = 1.898 * pow(10, 27);;
    GLdouble jupiter_x = 0.0;
    GLdouble jupiter_y = 778000000000.0;
    GLdouble jupiter_vel_y = 0.0;
    GLdouble jupiter_vel_x = 13069.0;
//saturn 
//1.4788 billion km
//mass = 5.683 × 10^26 kg
//speed = 9672
    GLdouble saturn_mass =5.683 * pow(10, 26);;
    GLdouble saturn_x = 0.0;
    GLdouble saturn_y = 1.4788 * 1000000000000.0;
    GLdouble saturn_vel_y = 0.0;
    GLdouble saturn_vel_x = 9672.0;
//uranus
//2.9479 billion km
//mass = 8.681 × 10^25 kg
//6800
    GLdouble uranus_mass =8.681 * pow(10, 25);;
    GLdouble uranus_x = 0.0;
    GLdouble uranus_y = 2.9479 * 1000000000000.0;
    GLdouble uranus_vel_y = 0.0;
    GLdouble uranus_vel_x = 6800.0;
//neptune
//4,498,396,441km
//mass = 1.024 × 10^26 kg
//5435
    GLdouble neptune_mass =1.024 * pow(10, 26);;
    GLdouble neptune_x = 0.0;
    GLdouble neptune_y = 4498396441000.0;
    GLdouble neptune_vel_y = 0.0;
    GLdouble neptune_vel_x = 5435.0;





    Universe *u = new Universe(sun_mass);
    Planet *sun = new Planet(sun_mass, 0.0, 0.0, 0.0, 0.0, 1.0f, 1.0f, 0.0f);
    Planet *earth = new Planet(earth_mass, earth_x, earth_y, earth_vel_x, earth_vel_y, 0.0f, 1.0f, 0.3f);
    Planet *mars = new Planet(mars_mass, mars_x, mars_y, mars_vel_x, mars_vel_y, 1.0f, 0.0f, 0.0f);
    Planet *venus = new Planet(venus_mass, venus_x, venus_y, venus_vel_x, venus_vel_y, 0.0f, 1.0f, 1.0f);
    Planet *mercury = new Planet(mercury_mass,mercury_x,mercury_y,mercury_vel_x,mercury_vel_y, 1.0f,1.0f,1.0f);
    Planet *moon = new Planet(moon_mass, moon_x, moon_y, moon_vel_x, moon_vel_y, 0.8f,0.8f,0.8f);
    Planet *jupiter = new Planet(jupiter_mass,jupiter_x,jupiter_y,jupiter_vel_x,jupiter_vel_y, 1.0f,0.0f,0.0f);
    Planet *saturn  = new Planet(saturn_mass, saturn_x, saturn_y, saturn_vel_x, saturn_vel_y, 1.0f,0.8f,0.1f);
    Planet *uranus = new Planet(uranus_mass, uranus_x, uranus_y, uranus_vel_x, uranus_vel_y, 0.0f, 0.3f,1.0f);
    Planet *neptune = new Planet(neptune_mass, neptune_x, neptune_y, neptune_vel_x, neptune_vel_y, 0.0f,0.1f,1.0f);
    u->addPlanet(sun);
    u->addPlanet(earth);
    u->addPlanet(mars);
    u->addPlanet(venus);
    u->addPlanet(mercury);
    u->addPlanet(moon);
    u->addPlanet(jupiter);
    u->addPlanet(saturn);
    u->addPlanet(uranus);
    u->addPlanet(neptune);
    if(argc > 2){
        cout << "made planet x!" << endl;
        GLdouble planet_X_mass;
        GLdouble planet_X_x;
        GLdouble planet_X_y;
        GLdouble planet_X_vel_y;
        GLdouble planet_X_vel_x;
        float planet_X_red;
        float planet_X_blue;
        float planet_X_green;
        planet_X_mass = (double)atof(argv[1]);
        planet_X_x = (double)atof(argv[2]);
        planet_X_y = (double)atof(argv[3]);
        planet_X_vel_x = (double)atof(argv[4]);
        planet_X_vel_y = (double)atof(argv[5]);
        planet_X_red = atof(argv[6]);
        planet_X_blue = atof(argv[7]);
        planet_X_green = atof(argv[8]);
        Planet *planet_X = new Planet(planet_X_mass, planet_X_x, planet_X_y, planet_X_vel_x, planet_X_vel_y, planet_X_red, planet_X_blue, planet_X_green);
        u->addPlanet(planet_X);

    }
    while (!glfwWindowShouldClose(window))
    {
        //glClear(GL_COLOR_BUFFER_BIT);
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        u->draw();
        u->threaded_update();
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