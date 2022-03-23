// int psudo_main(){
//     /* 
//     ---- CREATE A NEW WINDOW ----
//     */
//     GLFWwindow* window; // Creates a new window

//     // Set the window size and name
//     window = glfwCreateWindow(1080, 1080, "The edge of space", NULL, NULL);

//     // Error handling
//     glfwSetErrorCallback(error_callback); 
//     if (!glfwInit())
//         exit(EXIT_FAILURE);

//     if (!window)
//     {
//         glfwTerminate();
//         exit(EXIT_FAILURE);
//     }

//     glfwMakeContextCurrent(window);
//     glfwSetKeyCallback(window, key_callback);

//     /* 
//     ---- CREATE UNIVERSE ----
//     */
//    Universe *myUniverse = new Universe;

//     /* 
//     ---- Add Some Planets to the universe ----
//     */
//         // First create some planets
//         // Maybe take in args so user can define how many planets he can create
//         for (int i = 0; i < 5; i++)
//         {
//             Planet *myPlanet = new Planet;
//             Universe->addPlanet(myPlanet);
//         }
        
//     /* 
//     ---- Draw the universe to the window ----
//     but how?
//     */
//     while (!glfwWindowShouldClose(window))
//     {
//        glClear(GL_COLOR_BUFFER_BIT);
//        int width, height;
//        glfwGetFramebufferSize(window, &width, &height);
//        for (int i = 0; i < Universe.allPlanets.size(); i++)
//        {
//            Universe.allPlanets[i].drawSelf();
//        }
//         glfwSwapBuffers(window);
//         glfwPollEvents();
//         glFlush();
//        break;
//     }
//     glfwDestroyWindow(window);
//     glfwTerminate();
//     exit(EXIT_SUCCESS);
   
// }