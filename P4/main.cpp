#include "Geometry.h"
#include "BoggleGUI.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/glext.h>
#include <OpenGL/gl.h>		// Header File For The OpenGL32 Library
#include <OpenGL/glu.h>		// Header File For The GLu32 Library
#elif defined(__LINUX__)
#include <GL/glut.h>
#else
#include "glut.h"
#endif

#include <cstdlib>
#include <cstdio>
#include <ctime>

// Holds the Boggle board, UI elements, pretty much everything.
static BoggleGUI gui;

void LoadGLTextures();
void SelectSize(int size);

// Initialize GLUT, set up a drawing window
bool Init(int argc, char* argv[], // For glutInit
          unsigned int mode,      // For glutInitDisplayMode
          int window_x,           // For glutInitWindowPosition
          int window_y,
          int window_width,       // For glutInitWindowSize
          int window_height,
          char* title) {          // For glutCreateWindow  

    glutInit                (&argc, argv);
    glutInitDisplayMode     (mode);         // GLUT_ + (SINGLE, RGB, DEPTH)
    glutInitWindowPosition  (window_x, window_y);
    glutInitWindowSize      (window_width, window_height);
    glutCreateWindow        (title);
    glEnable                (GL_DEPTH_TEST);
    glEnable                (GL_TEXTURE_2D);

    // Set the back sides of polygons to be invisible
    glCullFace              (GL_BACK);
    glEnable                (GL_CULL_FACE);

    srand(time(NULL));
    BoggleGUI::SaveWindowSize(window_width, window_height);
    gui.SetFilenames("boglex.txt", "bknd2.bmp", "face.bmp", "face-shaded.bmp");
    
    // The menu is disabled for now to improve performance on slow machines.
    const bool SizeMenu = false;
    if (SizeMenu) {
        glutCreateMenu(SelectSize);
        for (int i = 3; i <= 10; ++i) {
            char str[100];
            sprintf(str, "%dx%d board", i, i);
            glutAddMenuEntry(str, i);
        }
        glutAttachMenu(GLUT_RIGHT_BUTTON);    
    }
    
    int retval = gui.LoadData();
    if(!retval) {
      std::cerr << "Data file 'boglex.txt', 'bknd2.bmp', 'face.bmp', or 'face-shaded.bmp' not found in cwd.\nExiting..." << std::endl;
    }
    return retval; 
}
   
// Main redraw function
void Redraw() {

    // Clear the color and depth values for the pixels in the drawing window
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);  // Set editing matrix back to modelview    
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0,     // Camera position
              0.0, 0.0, 0.0,     // Camera target
              0.0, 1.0, 0.0);    // Camera up-vector    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    gui.Draw();
    glutSwapBuffers();
    glFlush();
    glPopMatrix();    
}

// Function called upon window resize
void ResizeWindow(int w, int h) {
    BoggleGUI::SaveWindowSize(w, h);
    glMatrixMode(GL_PROJECTION); // Start editing projection matrix
    glViewport(0, 0, w, h);
    glLoadIdentity();

    gluPerspective(40.0,         // Field of view, in degrees
                   w / float(h), // Aspect ratio
                   1.0,          // Z near plane
                   10.0);        // Z far plane

     glMatrixMode(GL_MODELVIEW);  // Set editing matrix back to modelview
 
     gluLookAt(0.0, 0.0, 5.0,     // Camera position
               0.0, 0.0, 0.0,     // Camera target
               0.0, 1.0, 0.0);    // Camera up-vector
}

// glutKeyboardFunc calls this function to handle keypresses.
void RespondToKeyboard(unsigned char key, int x, int y) {
    if (key == 27) // ESC key
        exit(1);
    else
        gui.AddHumanLetter(key);
}

// glutMouseFunc calls this function to handle mouse action.
void RespondToMouse(int button, int state, int x, int y) {
    gui.SetMouseState(MouseInfo(button, state, x, y));
}

void SelectSize(int size)
{
    gui.ResetBoard(size);
}

void Timer(int value) {
    static int counter = 0;
    if (++counter == 10) {
        gui.DecreaseSeconds();
        counter = 0;
    }
    gui.AddQueuedComputerWords();
    glutTimerFunc(100, Timer, 0);          // Called 1x/sec
    glutPostRedisplay();
}

int main (int argc, char* argv[]) {
    if (!Init(argc, argv, GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH,
              200, 200, 960, 540, (char*)"CSE 100 - Boggle Game"))
        return 1;
    
    glutKeyboardFunc(RespondToKeyboard);    // Handle ASCII keypresses
    glutMouseFunc(RespondToMouse);          // Handle mouse action
    glutDisplayFunc(Redraw);                // Main redraw function 
    glutTimerFunc(100, Timer, 0);           // Called 10x/sec    
    glutReshapeFunc(ResizeWindow);
    glutMainLoop();

    return 0;
}
