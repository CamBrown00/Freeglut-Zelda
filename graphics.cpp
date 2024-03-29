#include "graphics.h"
#include <ctime>
#include <iostream>
#include <memory>
#include <vector>
#include "linkObj.h"
using namespace std;

GLdouble width, height;
int wd;
vector<unique_ptr<GameObj>> gameObjs;

// The gameObjs vector is populated here
void init() {

    srand(time(0));
    width = 500;
    height = 500;

    // Link is always required to be the first object pushed into this vector
    gameObjs.push_back(make_unique<LinkObj>(1, 100, 100, 2));

}

/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(0.9882f, 0.84706f, 0.65882f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

/* Handler for window-repaint event. Call back when the window first appears and
 whenever the window needs to be re-painted. */
void display() {
    // Tell OpenGL to use the whole window for drawing
    glViewport(0, 0, width, height); // DO NOT CHANGE THIS LINE
    
    // Do an orthographic parallel projection with the coordinate
    // system set to first quadrant, limited by screen/window size
    glMatrixMode(GL_PROJECTION); // DO NOT CHANGE THIS LINE
    glLoadIdentity(); // DO NOT CHANGE THIS LINE
    glOrtho(0.0, width, height, 0.0, -1.f, 1.f); // DO NOT CHANGE THIS LINE

    // Clear the color buffer with current clearing color
    glClear(GL_COLOR_BUFFER_BIT); // DO NOT CHANGE THIS LINE
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // DO NOT CHANGE THIS LINE

    for (unique_ptr<GameObj> &gObj : gameObjs) {
        gObj->draw();
    }
    
    glFlush();  // Render now
}

// The kbd functions for each of the gameObjs in the vector are called here
void kbd(unsigned char key, int x, int y)
{
    // Call kbd functions for game objects
    for (unique_ptr<GameObj> &gObj : gameObjs) {
        gObj->kbd(key, x, y);
    }
    switch(key) {
        case 27: {
            glutDestroyWindow(wd);
            exit(0);
            break;
        }
    }
    glutPostRedisplay();
}

// The kbdUp functions for each of the gameObjs in the vector are called here
void kbdUp(unsigned char key, int x, int y) {
    // Call kbdUp functions for game objects
    for (unique_ptr<GameObj> &gObj : gameObjs) {
        gObj->kbdUp(key, x, y);
    }
    glutPostRedisplay();
}

// This timer handles playing the frames of link's sprites and resets them once the last frame is reached
void linkObjAnimationTimer(int dummy) {
    if (gameObjs[0]->getAnimationSpeed() > 0) {
        if (gameObjs[0]->getImageFrameIndex() < gameObjs[0]->getSprites()[gameObjs[0]->getSpriteIndex()].size() - 1) {
            gameObjs[0]->setImageFrameIndex(gameObjs[0]->getImageFrameIndex() + 1);
        } else {
            gameObjs[0]->setImageFrameIndex(0);
        }
    }
    
    glutPostRedisplay();
    glutTimerFunc(100, linkObjAnimationTimer, dummy);
}

// This timer handles the movement for link
void linkObjMoveTimer(int dummy) {
    gameObjs[0]->moveX(gameObjs[0]->getHSpd());
    gameObjs[0]->moveY(gameObjs[0]->getVSpd());

    glutPostRedisplay();
    glutTimerFunc(10, linkObjMoveTimer, dummy);
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
    
    init();
    
    glutInit(&argc, argv);          // Initialize GLUT
    
    glutInitDisplayMode(GLUT_RGBA);
    
    glutInitWindowSize((int)width, (int)height);
    glutInitWindowPosition(100, 100); // Position the window's initial top-left corner
    /* create the window and store the handle to it */
    wd = glutCreateWindow("The Legend of Zelda" /* title */ );
    
    // Register callback handler for window re-paint event
    glutDisplayFunc(display);
    
    // Our own OpenGL initialization
    initGL();
    
    // register keyboard press event processing function
    // works for numbers, letters, spacebar, etc.
    glutIgnoreKeyRepeat(1);
    glutKeyboardFunc(kbd);
    glutKeyboardUpFunc(kbdUp);
    
    // handles timer
    glutTimerFunc(0, linkObjAnimationTimer, 0);
    glutTimerFunc(0, linkObjMoveTimer, 0);

    // Enter the event-processing loop
    glutMainLoop();
    return 0;
}
