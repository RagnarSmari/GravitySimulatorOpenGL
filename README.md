# GravitySimulatorOpenGL

Prerequisites:
Have OpenGL installed on computer
Run program on a Debian Linux distro, preferrably Ubuntu or Pop Os


HOW TO RUN:
g++ -std=c++11 main.cpp -lglfw3 -lGLU -lGL -lX11 -pthread

HOW TO ADD A PLANET:
add 8 arguments when calling the executable.
the arguments are in the follwing order:
    -planet's mass (kg as a double)
    -initial x coordinate (meters as a double)
    -initial y coordinate (meters as a double)
    -initial velocity on the x axis (meters/second as a double)
    -initial velocity on the y axis (meters/second as a double)
    -color value red (float from 1.0 = max to 0.0 = min)
    -color value blue (float from 1.0 = max to 0.0 = min)
    -color value green (float from 1.0 = max to 0.0 = min)

Example planets to add:
//rouge planet 
./a.out 639000000000000000000000.0 230000000000.0 230000000000.0 0.0 24072.0 1.0 1.0 1.0
//black hole
./a.out 63900000000000000000000000000000.0 230000000000.0 230000000000.0 0.0 24072.0 0.0 0.0 0.0
//haley's comet
./a.out 5972368223000000000000000.0 -149110000000.0 0.0 5000.0 21000.0 0.5 0.5 0.5
    // GLdouble earth_vel_y = 5000.0;
    // GLdouble earth_vel_x = 21000.0;//sqrt((G * sun_mass) / earth_x);
    // GLdouble earth_y = 0.0;
    // GLdouble earth_x = 149.11 * 1000000000;


Documentations:
https://docs.gl/

https://www.glfw.org/docs/latest/


GUI library:
https://github.com/ocornut/imgui/


sudo apt-get install libsdl2-image-dev  
sudo apt-get install libsdl2-mixer-dev  
sudo apt-get install libsdl2-ttf-dev 