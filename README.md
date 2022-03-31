# GravitySimulatorOpenGL

Prerequisites:
Have OpenGL installed on computer
Run program on a Debian Linux distro, preferrably Ubuntu or Pop Os

Compiled as Library:
The project has been put into a library but is currently not usable.
(Currently in development, we finished our funding)

HOW TO COMPILE:
make
or
g++ -std=c++11 main.cpp -lglfw3 -lGLU -lGL -lX11 -pthread

HOW TO RUN:
./a.out

HOW TO ADD A PLANET:
command line arguments
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


INTERFACE:
(a)ngle: Toggle angle
(t)rail: Toggle trail
(1) Zoom in
(2) Zoom out
(3) Increase size of objects
(4) Decrease size of objects
(5) Increase speed of simulation
(6) Decrease speed of simulation
(s)table: Add a randomly generated celestial body in a (hopefully) stable orbit
(r)ogue: Add a randomly generated rogue planet, who knows what will happen

It can crash if you add too many planets, but we stil recommend doing it.
It gets a bit funky if you zoom, increase size or toggle angle with trail on.
It gets a bit weird if you increase the speed too much

Documentations:
https://docs.gl/

https://www.glfw.org/docs/latest/


How to install OpenGL on linux:
http://www.codebind.com/linux-tutorials/install-opengl-ubuntu-linux/

How to install OpenGL on MacOS:
https://en.wikibooks.org/wiki/OpenGL_Programming/Installation/Mac

How to install OpenGL on Windows: -- P.s this is pain, just install Linux :)
https://medium.com/swlh/setting-opengl-for-windows-d0b45062caf