# GravitySimulatorOpenGL

Youtube Video explaining:
https://youtu.be/C7f5ABi6VSU


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
the arguments are in the follwing order:<br>
    -planet's mass (kg as a double)<br>
    -initial x coordinate (meters as a double)<br>
    -initial y coordinate (meters as a double)<br>
    -initial velocity on the x axis (meters/second as a double)<br>
    -initial velocity on the y axis (meters/second as a double)<br>
    -color value red (float from 1.0 = max to 0.0 = min)<br>
    -color value blue (float from 1.0 = max to 0.0 = min)<br>
    -color value green (float from 1.0 = max to 0.0 = min)<br>

Example planets to add:<br>
//rouge planet <br>
./a.out 639000000000000000000000.0 230000000000.0 230000000000.0 0.0 24072.0 1.0 1.0 1.0<br>
//black hole<br>
./a.out 63900000000000000000000000000000.0 230000000000.0 230000000000.0 0.0 24072.0 0.0 0.0 0.0<br>
//haley's comet<br>
./a.out 5972368223000000000000000.0 -149110000000.0 0.0 5000.0 21000.0 0.5 0.5 0.5<br>
    // GLdouble earth_vel_y = 5000.0;<br>
    // GLdouble earth_vel_x = 21000.0;//sqrt((G * sun_mass) / earth_x);<br>
    // GLdouble earth_y = 0.0;<br>
    // GLdouble earth_x = 149.11 * 1000000000;<br>


INTERFACE:<br>
(a)ngle: Toggle angle<br>
(t)rail: Toggle trail<br>
(1) Zoom in<br>
(2) Zoom out<br>
(3) Increase size of objects<br>
(4) Decrease size of objects<br>
(5) Increase speed of simulation<br>
(6) Decrease speed of simulation<br>
(s)table: Add a randomly generated celestial body in a (hopefully) stable orbit<br>
(r)ogue: Add a randomly generated rogue planet, who knows what will happen<br>

It can crash if you add too many planets, but we stil recommend doing it.<br>
It gets a bit funky if you zoom, increase size or toggle angle with trail on.<br>
It gets a bit weird if you increase the speed too much<br>

Documentations:<br>
https://docs.gl/<br>

https://www.glfw.org/docs/latest/<br>


How to install OpenGL on linux:<br>
http://www.codebind.com/linux-tutorials/install-opengl-ubuntu-linux/<br>

How to install OpenGL on MacOS:
https://en.wikibooks.org/wiki/OpenGL_Programming/Installation/Mac

How to install OpenGL on Windows: -- P.s this is pain, just install Linux :)
https://medium.com/swlh/setting-opengl-for-windows-d0b45062caf
