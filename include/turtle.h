/* 
openGL implementation of turtle and the turtletools module
features:
adjustable pen (size and colour)
resizable window support
keyboard and mouse presses
*/

#ifndef TURTLESET
#define TURTLESET // include guard
#include <math.h>
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif
#include "glad.h"
#include "glfw3.h"
#include "list.h"

extern void glColor4d(double r, double g, double b, double a); // genius tactic to stop compiler warnings
extern void glBegin(int type);
extern void glVertex2d(double x, double y);
extern void glEnd();
void turtleTexture(int textureCode, double x1, double y1, double x2, double y2, double rot, double r, double g, double b) {
    // No support for textures in turtle.h, use turtleTextures.h
}

typedef struct {
    GLFWwindow* window; // the window
    list_t *keyPressed; // global keyPressed and mousePressed list
    int8_t mousePressed[4]; // cached mouse variables
    int32_t screenbounds[2]; // list of screen bounds (pixels)
    int32_t lastscreenbounds[2]; // list of screen bounds last frame
    int32_t initscreenbounds[2]; // screenbounds at initialisation
    int32_t bounds[4]; // list of coordinate bounds (minX, minY, maxX, maxY)
    double mouseX; // mouseX and mouseY variables
    double mouseY;
    double scrollY;
    double mouseScaX;
    double mouseScaY;
    double mouseAbsX;
    double mouseAbsY;
    double x; // x and y position of the turtle
    double y;
    list_t *penPos; // a list of where to draw
    uint64_t penHash; // the penPos list is hashed and this hash is used to determine if any changes occured between frames
    int32_t lastLength; // the penPos list's length is saved and if it is different from last frame we know we have to redraw
    char pen; // pen status (1 for down, 0 for up)
    char penshape; // 0 for circle, 1 for square, 2 for triangle
    char close; // close changes to 1 when the user clicks the x on the window
    char shouldClose; // controls whether the window terminates on turtle.close
    double circleprez; // how precise circles are (specifically, the number of sides of a circle with diameter e)
    double pensize; // turtle pen size
    double penr;
    double peng;
    double penb;
    double pena;
    double currentColor[4]; // for reducing API color calls
} turtleglob; // all globals are conSTRUCTed here

turtleglob turtle;

/* run this to set the bounds of the window in coordinates */
void turtleSetWorldCoordinates(int32_t minX, int32_t minY, int32_t maxX, int32_t maxY) {
    glfwGetWindowSize(turtle.window, &turtle.screenbounds[0], &turtle.screenbounds[1]);
    turtle.initscreenbounds[0] = turtle.screenbounds[0];
    turtle.initscreenbounds[1] = turtle.screenbounds[1];
    turtle.bounds[0] = minX;
    turtle.bounds[1] = minY;
    turtle.bounds[2] = maxX;
    turtle.bounds[3] = maxY;
}

/* detect key presses */
void keySense(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods) {
    if (action == GLFW_PRESS) {
        list_append(turtle.keyPressed, (unitype) key, 'i');
    }
    if (action == GLFW_RELEASE) {
        list_remove(turtle.keyPressed, (unitype) key, 'i');
    }
}

/* detect mouse clicks */
void mouseSense(GLFWwindow* window, int32_t button, int32_t action, int32_t mods) {
    if (action == GLFW_PRESS) {
        switch(button) {
            case GLFW_MOUSE_BUTTON_LEFT:
            list_append(turtle.keyPressed, (unitype) "m1", 's');
            turtle.mousePressed[0] = 1;
            break;
            case GLFW_MOUSE_BUTTON_RIGHT:
            list_append(turtle.keyPressed, (unitype) "m2", 's');
            turtle.mousePressed[1] = 1;
            break;
            case GLFW_MOUSE_BUTTON_MIDDLE:
            list_append(turtle.keyPressed, (unitype) "m3", 's');
            turtle.mousePressed[2] = 1;
            break;
            default:
            break;
        }
    }
    if (action == GLFW_RELEASE) {
        switch(button) {
            case GLFW_MOUSE_BUTTON_LEFT:
            list_remove(turtle.keyPressed, (unitype) "m1", 's');
            turtle.mousePressed[0] = 0;
            break;
            case GLFW_MOUSE_BUTTON_RIGHT:
            list_remove(turtle.keyPressed, (unitype) "m2", 's');
            turtle.mousePressed[1] = 0;
            break;
            case GLFW_MOUSE_BUTTON_MIDDLE:
            list_remove(turtle.keyPressed, (unitype) "m3", 's');
            turtle.mousePressed[2] = 0;
            break;
            default:
            break;
        }
    }
}

/* detect scroll wheel */
void scrollSense(GLFWwindow* window, double xoffset, double yoffset) {
    turtle.scrollY = yoffset;
}

/* the behavior with the mouse wheel is different since it can't be "on" or "off" */
double turtleMouseWheel() {
    double temp = turtle.scrollY;
    turtle.scrollY = 0;
    return temp;
}

/* top level boolean output call to check if the key with code [key] is currently being held down. Uses the GLFW_KEY_X macros */
char turtleKeyPressed(int key) {
    return list_count(turtle.keyPressed, (unitype) key, 'c');
}

/* top level boolean output call to check if the left click button is currently being held down */
char turtleMouseDown() {
    return turtle.mousePressed[0];
}

/* top level boolean output call to check if the right click button is currently being held down */
char turtleMouseRight() {
    return turtle.mousePressed[1];
}

/* top level boolean output call to check if the middle mouse button is currently being held down */
char turtleMouseMiddle() {
    return turtle.mousePressed[2];
}

/* alternate duplicate of top level boolean output call to check if the middle mouse button is currently being held down */
char turtleMouseMid() {
    return list_count(turtle.keyPressed, (unitype) "m3", 's');
}

/* initializes the turtletools module */
void turtleInit(GLFWwindow* window, int minX, int minY, int maxX, int maxY) {
    gladLoadGL();
    glfwMakeContextCurrent(window); // various glfw things
    glEnable(GL_ALPHA);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
    glClearColor(1.0, 1.0, 1.0, 0.0); // white background by default
    turtle.window = window;
    turtle.close = 0;
    turtle.shouldClose = 0;
    turtle.keyPressed = list_init();
    turtle.lastscreenbounds[0] = 0;
    turtle.lastscreenbounds[1] = 0;
    turtle.penPos = list_init();
    turtle.penHash = 0;
    turtle.lastLength = 0;
    turtle.x = 0;
    turtle.y = 0;
    turtle.pensize = 1;
    turtle.penshape = 0;
    turtle.circleprez = 9; // default circleprez value
    turtle.pen = 0;
    turtle.penr = 0.0;
    turtle.peng = 0.0;
    turtle.penb = 0.0;
    turtle.pena = 0.0;
    for (int i = 0; i < 3; i++) {
        turtle.currentColor[i] = 0.0;
    }
    turtle.currentColor[3] = 1.0;
    turtleSetWorldCoordinates(minX, minY, maxX, maxY);
    glfwSetKeyCallback(window, keySense); // initiate mouse and keyboard detection
    glfwSetMouseButtonCallback(window, mouseSense);
    glfwSetScrollCallback(window, scrollSense);
}

/* gets the mouse coordinates */
void turtleGetMouseCoords() {
    glfwGetWindowSize(turtle.window, &turtle.screenbounds[0], &turtle.screenbounds[1]); // get screenbounds
    glfwGetCursorPos(turtle.window, &turtle.mouseAbsX, &turtle.mouseAbsY); // get mouse positions (absolute)
    turtle.mouseX = turtle.mouseAbsX;
    turtle.mouseY = turtle.mouseAbsY;
    turtle.mouseScaX = turtle.mouseAbsX;
    turtle.mouseScaY = turtle.mouseAbsY;
    turtle.mouseX -= (turtle.initscreenbounds[0] / 2) - ((turtle.bounds[2] + turtle.bounds[0]) / 2);
    turtle.mouseX *= ((double) (turtle.bounds[2] - turtle.bounds[0]) / (double) turtle.initscreenbounds[0]);
    turtle.mouseY -= (turtle.initscreenbounds[1] / 2) - ((turtle.bounds[3] + turtle.bounds[1]) / 2) + (turtle.screenbounds[1] - turtle.initscreenbounds[1]);
    turtle.mouseY *= ((double) (turtle.bounds[1] - turtle.bounds[3]) / (double) turtle.initscreenbounds[1]);
    turtle.mouseScaX -= (turtle.screenbounds[0] / 2) - ((turtle.bounds[2] + turtle.bounds[0]) / 2);
    turtle.mouseScaX *= ((double) (turtle.bounds[2] - turtle.bounds[0]) / (double) turtle.screenbounds[0]);
    turtle.mouseScaY -= (turtle.screenbounds[1] / 2) - ((turtle.bounds[3] + turtle.bounds[1]) / 2);
    turtle.mouseScaY *= ((double) (turtle.bounds[1] - turtle.bounds[3]) / (double) turtle.screenbounds[1]);
}

/* set the background color */
void turtleBgColor(double r, double g, double b) {
    glClearColor(r / 255, g / 255, b / 255, 0.0);
}

/* set the pen color */
void turtlePenColor(double r, double g, double b) {
    turtle.penr = r / 255;
    turtle.peng = g / 255;
    turtle.penb = b / 255;
    turtle.pena = 0.0;
}

/* set the pen color (with transparency) */
void turtlePenColorAlpha(double r, double g, double b, double a) {
    turtle.penr = r / 255;
    turtle.peng = g / 255;
    turtle.penb = b / 255;
    turtle.pena = a / 255;
}

/* set the pen size */
void turtlePenSize(double size) {
    turtle.pensize = size * 0.5; // ensures pensize matches pixel size (a pen size of 240 will be 240 coordinates long)
}

/* clears all the pen drawings */
void turtleClear() {
    // list_free(turtle.penPos);
    // turtle.penPos = list_init();
    turtle.penPos -> length = 0; // could be dangerous
}

/* pen down */
void turtlePenDown() {
    if (turtle.pen == 0) {
        turtle.pen = 1;
        char changed = 0;
        int32_t len = turtle.penPos -> length;
        if (len > 0) {
            unitype *ren = turtle.penPos -> data;
            if (ren[len - 9].d != turtle.x) {changed = 1;}
            if (ren[len - 8].d != turtle.y) {changed = 1;}
            if (ren[len - 7].d != turtle.pensize) {changed = 1;}
            if (ren[len - 6].d != turtle.penr) {changed = 1;}
            if (ren[len - 5].d != turtle.peng) {changed = 1;}
            if (ren[len - 4].d != turtle.penb) {changed = 1;}
            if (ren[len - 3].d != turtle.pena) {changed = 1;}
            if (ren[len - 2].h != turtle.penshape) {changed = 1;}
            if (ren[len - 1].d != turtle.circleprez) {changed = 1;}
        } else {
            changed = 1;
        }
        if (changed == 1) {
            list_append(turtle.penPos, (unitype) turtle.x, 'd');
            list_append(turtle.penPos, (unitype) turtle.y, 'd');
            list_append(turtle.penPos, (unitype) turtle.pensize, 'd');
            list_append(turtle.penPos, (unitype) turtle.penr, 'd');
            list_append(turtle.penPos, (unitype) turtle.peng, 'd');
            list_append(turtle.penPos, (unitype) turtle.penb, 'd');
            list_append(turtle.penPos, (unitype) turtle.pena, 'd');
            list_append(turtle.penPos, (unitype) turtle.penshape, 'h');
            list_append(turtle.penPos, (unitype) turtle.circleprez, 'd');
        }
    }
}

/* lift the pen */
void turtlePenUp() {
    if (turtle.pen == 1) {
        turtle.pen = 0;
        if (turtle.penPos -> length > 0 && turtle.penPos -> type[turtle.penPos -> length - 1] != 'c') {
            list_append(turtle.penPos, (unitype) 0, 'c');
            list_append(turtle.penPos, (unitype) 0, 'c');
            list_append(turtle.penPos, (unitype) 0, 'c');
            list_append(turtle.penPos, (unitype) 0, 'c');
            list_append(turtle.penPos, (unitype) 0, 'c');
            list_append(turtle.penPos, (unitype) 0, 'c');
            list_append(turtle.penPos, (unitype) 0, 'c');
            list_append(turtle.penPos, (unitype) 0, 'c');
            list_append(turtle.penPos, (unitype) 0, 'c');
        }
    }
}

/* set the pen shape ("circle", "square", "triangle", "none", or "connected") */
void turtlePenShape(char *selection) {
    if (strcmp(selection, "circle") == 0 || strcmp(selection, "Circle") == 0) {
        turtle.penshape = 0;
    }
    if (strcmp(selection, "square") == 0 || strcmp(selection, "Square") == 0) {
        turtle.penshape = 1;
    }
    if (strcmp(selection, "triangle") == 0 || strcmp(selection, "Triangle") == 0) {
        turtle.penshape = 2;
    }
    if (strcmp(selection, "none") == 0 || strcmp(selection, "None") == 0) {
        turtle.penshape = 3;
    }
    if (strcmp(selection, "connected") == 0 || strcmp(selection, "Connected") == 0) {
        turtle.penshape = 4;
    }
    if (strcmp(selection, "text") == 0 || strcmp(selection, "Text") == 0) {
        turtle.penshape = 5;
    }
}

/* set the circle precision */
void turtlePenPrez(double prez) {
    turtle.circleprez = prez;
}

/* moves the turtle to a coordinate */
void turtleGoto(double x, double y) {
    if (fabs(turtle.x - x) > 0.01 || fabs(turtle.y - y) > 0.01) {
        turtle.x = x;
        turtle.y = y;
        if (turtle.pen == 1) {
            char changed = 0;
            int32_t len = turtle.penPos -> length;
            if (len > 0) {
                unitype *ren = turtle.penPos -> data;
                if (ren[len - 9].d != turtle.x) {changed = 1;}
                if (ren[len - 8].d != turtle.y) {changed = 1;}
                if (ren[len - 7].d != turtle.pensize) {changed = 1;}
                if (ren[len - 6].d != turtle.penr) {changed = 1;}
                if (ren[len - 5].d != turtle.peng) {changed = 1;}
                if (ren[len - 4].d != turtle.penb) {changed = 1;}
                if (ren[len - 3].d != turtle.pena) {changed = 1;}
                if (ren[len - 2].h != turtle.penshape) {changed = 1;}
                if (ren[len - 1].d != turtle.circleprez) {changed = 1;}
            } else {
                changed = 1;
            }
            if (changed == 1) {
                list_append(turtle.penPos, (unitype) x, 'd');
                list_append(turtle.penPos, (unitype) y, 'd');
                list_append(turtle.penPos, (unitype) turtle.pensize, 'd');
                list_append(turtle.penPos, (unitype) turtle.penr, 'd');
                list_append(turtle.penPos, (unitype) turtle.peng, 'd');
                list_append(turtle.penPos, (unitype) turtle.penb, 'd');
                list_append(turtle.penPos, (unitype) turtle.pena, 'd');
                list_append(turtle.penPos, (unitype) turtle.penshape, 'h');
                list_append(turtle.penPos, (unitype) turtle.circleprez, 'd');
            }
        }
    }
}

/* draws a circle at the specified x and y (coordinates) */
void turtleCircleRender(double x, double y, double rad, double r, double g, double b, double a, double xfact, double yfact, double prez) {
    char colorChange = 0;
    if (r != turtle.currentColor[0]) {colorChange = 1;}
    if (g != turtle.currentColor[1]) {colorChange = 1;}
    if (b != turtle.currentColor[2]) {colorChange = 1;}
    if (a != turtle.currentColor[3]) {colorChange = 1;}
    if (colorChange == 1) {
        glColor4d(r, g, b, a);
        turtle.currentColor[0] = r;
        turtle.currentColor[1] = g;
        turtle.currentColor[2] = b;
        turtle.currentColor[3] = a;
    }
    glBegin(GL_TRIANGLE_FAN);
    for (double i = 0; i < prez; i++) {
        glVertex2d((x + rad * sin(2 * i * M_PI / prez)) * xfact, (y + rad * cos(2 * i * M_PI / prez)) * yfact);
    }
    glEnd();
}

/* draws a square */
void turtleSquareRender(double x1, double y1, double x2, double y2, double r, double g, double b, double a, double xfact, double yfact) {
    char colorChange = 0;
    if (r != turtle.currentColor[0]) {colorChange = 1;}
    if (g != turtle.currentColor[1]) {colorChange = 1;}
    if (b != turtle.currentColor[2]) {colorChange = 1;}
    if (a != turtle.currentColor[3]) {colorChange = 1;}
    if (colorChange == 1) {
        glColor4d(r, g, b, a);
        turtle.currentColor[0] = r;
        turtle.currentColor[1] = g;
        turtle.currentColor[2] = b;
        turtle.currentColor[3] = a;
    }
    glBegin(GL_TRIANGLE_FAN);
    glVertex2d(x1 * xfact, y1 * yfact);
    glVertex2d(x2 * xfact, y1 * yfact);
    glVertex2d(x2 * xfact, y2 * yfact);
    glVertex2d(x1 * xfact, y2 * yfact);
    glEnd();
}

/* draws a triangle */
void turtleTriangleRender(double x1, double y1, double x2, double y2, double x3, double y3, double r, double g, double b, double a, double xfact, double yfact) {
    char colorChange = 0;
    if (r != turtle.currentColor[0]) {colorChange = 1;}
    if (g != turtle.currentColor[1]) {colorChange = 1;}
    if (b != turtle.currentColor[2]) {colorChange = 1;}
    if (a != turtle.currentColor[3]) {colorChange = 1;}
    if (colorChange == 1) {
        glColor4d(r, g, b, a);
        turtle.currentColor[0] = r;
        turtle.currentColor[1] = g;
        turtle.currentColor[2] = b;
        turtle.currentColor[3] = a;
    }
    glBegin(GL_TRIANGLES);
    glVertex2d(x1 * xfact, y1 * yfact);
    glVertex2d(x2 * xfact, y2 * yfact);
    glVertex2d(x3 * xfact, y3 * yfact);
    glEnd();
}

/* adds a (blit) triangle to the pipeline (for better speed) */
void turtleTriangle(double x1, double y1, double x2, double y2, double x3, double y3, double r, double g, double b, double a) {
    list_append(turtle.penPos, (unitype) x1, 'd');
    list_append(turtle.penPos, (unitype) y1, 'd');
    list_append(turtle.penPos, (unitype) x2, 'd');
    list_append(turtle.penPos, (unitype) (r / 255), 'd');
    list_append(turtle.penPos, (unitype) (g / 255), 'd');
    list_append(turtle.penPos, (unitype) (b / 255), 'd');
    list_append(turtle.penPos, (unitype) (a / 255), 'd');
    list_append(turtle.penPos, (unitype) 66, 'h'); // blit triangle signifier
    list_append(turtle.penPos, (unitype) y2, 'd'); // some unconventional formatting but it works

    list_append(turtle.penPos, (unitype) x3, 'd');
    list_append(turtle.penPos, (unitype) y3, 'd');
    list_append(turtle.penPos, (unitype) 0, 'd'); // zero'd out (wasted space)
    list_append(turtle.penPos, (unitype) r, 'd'); // duplicate colour data (wasted space)
    list_append(turtle.penPos, (unitype) g, 'd');
    list_append(turtle.penPos, (unitype) b, 'd');
    list_append(turtle.penPos, (unitype) a, 'd');
    list_append(turtle.penPos, (unitype) 66, 'h'); // blit triangle signifier
    list_append(turtle.penPos, (unitype) 0, 'd'); // zero'd out (wasted space)
}

/* draws a quadrilateral */
void turtleQuadRender(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double r, double g, double b, double a, double xfact, double yfact) {
    char colorChange = 0;
    if (r != turtle.currentColor[0]) {colorChange = 1;}
    if (g != turtle.currentColor[1]) {colorChange = 1;}
    if (b != turtle.currentColor[2]) {colorChange = 1;}
    if (a != turtle.currentColor[3]) {colorChange = 1;}
    if (colorChange == 1) {
        glColor4d(r, g, b, a);
        turtle.currentColor[0] = r;
        turtle.currentColor[1] = g;
        turtle.currentColor[2] = b;
        turtle.currentColor[3] = a;
    }
    glBegin(GL_TRIANGLE_FAN);
    glVertex2d(x1 * xfact, y1 * yfact);
    glVertex2d(x2 * xfact, y2 * yfact);
    glVertex2d(x3 * xfact, y3 * yfact);
    glVertex2d(x4 * xfact, y4 * yfact);
    glEnd();
}

/* adds a (blit) quad to the pipeline (for better speed) */
void turtleQuad(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double r, double g, double b, double a) {
    list_append(turtle.penPos, (unitype) x1, 'd');
    list_append(turtle.penPos, (unitype) y1, 'd');
    list_append(turtle.penPos, (unitype) x2, 'd');
    list_append(turtle.penPos, (unitype) (r / 255), 'd');
    list_append(turtle.penPos, (unitype) (g / 255), 'd');
    list_append(turtle.penPos, (unitype) (b / 255), 'd');
    list_append(turtle.penPos, (unitype) (a / 255), 'd');
    list_append(turtle.penPos, (unitype) 67, 'h'); // blit quad signifier
    list_append(turtle.penPos, (unitype) y2, 'd'); // some unconventional formatting but it works

    list_append(turtle.penPos, (unitype) x3, 'd');
    list_append(turtle.penPos, (unitype) y3, 'd');
    list_append(turtle.penPos, (unitype) x4, 'd');
    list_append(turtle.penPos, (unitype) r, 'd'); // duplicate colour data (wasted space)
    list_append(turtle.penPos, (unitype) g, 'd');
    list_append(turtle.penPos, (unitype) b, 'd');
    list_append(turtle.penPos, (unitype) a, 'd');
    list_append(turtle.penPos, (unitype) 67, 'h'); // blit quad signifier
    list_append(turtle.penPos, (unitype) y4, 'd');
}

/* adds a (blit) rectangle to the pipeline (uses quad interface) */
void turtleRectangle(double x1, double y1, double x2, double y2, double r, double g, double b, double a) {
    list_append(turtle.penPos, (unitype) x1, 'd');
    list_append(turtle.penPos, (unitype) y1, 'd');
    list_append(turtle.penPos, (unitype) x2, 'd');
    list_append(turtle.penPos, (unitype) (r / 255), 'd');
    list_append(turtle.penPos, (unitype) (g / 255), 'd');
    list_append(turtle.penPos, (unitype) (b / 255), 'd');
    list_append(turtle.penPos, (unitype) (a / 255), 'd');
    list_append(turtle.penPos, (unitype) 67, 'h'); // blit quad signifier
    list_append(turtle.penPos, (unitype) y1, 'd'); // some unconventional formatting but it works

    list_append(turtle.penPos, (unitype) x2, 'd');
    list_append(turtle.penPos, (unitype) y2, 'd');
    list_append(turtle.penPos, (unitype) x1, 'd');
    list_append(turtle.penPos, (unitype) r, 'd'); // duplicate colour data (wasted space)
    list_append(turtle.penPos, (unitype) g, 'd');
    list_append(turtle.penPos, (unitype) b, 'd');
    list_append(turtle.penPos, (unitype) a, 'd');
    list_append(turtle.penPos, (unitype) 67, 'h'); // blit quad signifier
    list_append(turtle.penPos, (unitype) y2, 'd');
}

/* draws the turtle's path on the screen */
void turtleUpdate() {
    // used to have a feature that only redrew the screen if there have been any changes from last frame, but it has been removed.
    // opted to redraw every frame and not list_copy, an alternative is hashing the penPos list. An interesting idea for sure... for another time
    char changed = 0;
    int32_t len = turtle.penPos -> length;
    unitype *ren = turtle.penPos -> data;
    char *renType = turtle.penPos -> type;
    uint64_t oldHash = turtle.penHash;
    turtle.penHash = 0; // I don't use this but it's an idea: https://stackoverflow.com/questions/57455444/very-low-collision-non-cryptographic-hashing-function
    for (uint32_t i = 0; i < len; i++) {
        turtle.penHash += (uint64_t) turtle.penPos -> data[i].p; // simple addition hash. I know not technically safe since i cast all sizes to 8 byte, but it should still work
    }
    // printf("%lld %lld\n", oldHash, turtle.penHash);
    if (len != turtle.lastLength || oldHash != turtle.penHash) {
        changed = 1;
        turtle.lastLength = len;
    }
    if (changed) {
        double xfact = (turtle.bounds[2] - turtle.bounds[0]) / 2;
        double yfact = (turtle.bounds[3] - turtle.bounds[1]) / 2;
        xfact = 1 / xfact;
        yfact = 1 / yfact;
        double lastSize = -1;
        double lastPrez = -1;
        double precomputedLog = 5;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for (int i = 0; i < len; i += 9) {
            if (renType[i] == 'd') {
                switch (ren[i + 7].h) {
                    case 0:
                    if (!(lastSize == ren[i + 2].d) || !(lastPrez != ren[i + 8].d))
                        precomputedLog = ren[i + 8].d * log(2.71 + ren[i + 2].d);
                    lastSize = ren[i + 2].d;
                    lastPrez = ren[i + 8].d;
                    turtleCircleRender(ren[i].d, ren[i + 1].d, ren[i + 2].d, ren[i + 3].d, ren[i + 4].d, ren[i + 5].d, ren[i + 6].d, xfact, yfact, precomputedLog);
                    break;
                    case 1:
                    turtleSquareRender(ren[i].d - ren[i + 2].d, ren[i + 1].d - ren[i + 2].d, ren[i].d + ren[i + 2].d, ren[i + 1].d + ren[i + 2].d, ren[i + 3].d, ren[i + 4].d, ren[i + 5].d, ren[i + 6].d, xfact, yfact);
                    break;
                    case 2:
                    turtleTriangleRender(ren[i].d - ren[i + 2].d, ren[i + 1].d - ren[i + 2].d, ren[i].d + ren[i + 2].d, ren[i + 1].d - ren[i + 2].d, ren[i].d, ren[i + 1].d + ren[i + 2].d, ren[i + 3].d, ren[i + 4].d, ren[i + 5].d, ren[i + 6].d, xfact, yfact);
                    break;
                    case 5:
                    if (i - 9 < 0 || renType[i - 9] == 'c') {
                        if (!(lastSize == ren[i + 2].d) || !(lastPrez != ren[i + 8].d))
                            precomputedLog = ren[i + 8].d * log(2.71 + ren[i + 2].d);
                        lastSize = ren[i + 2].d;
                        lastPrez = ren[i + 8].d;
                        turtleCircleRender(ren[i].d, ren[i + 1].d, ren[i + 2].d, ren[i + 3].d, ren[i + 4].d, ren[i + 5].d, ren[i + 6].d, xfact, yfact, precomputedLog);
                    }
                    break;
                    default:
                    break;
                }
                if (i + 9 < len && renType[i + 9] == 'd' && ren[i + 7].h < 64 && (ren[i + 7].h == 4 || ren[i + 7].h == 5 || (fabs(ren[i].d - ren[i + 9].d) > ren[i + 2].d / 2 || fabs(ren[i + 1].d - ren[i + 10].d) > ren[i + 2].d / 2))) { // tests for next point continuity and also ensures that the next point is at sufficiently different coordinates
                    double dir = atan((ren[i + 9].d - ren[i].d) / (ren[i + 1].d - ren[i + 10].d));
                    double sinn = sin(dir + M_PI / 2);
                    double coss = cos(dir + M_PI / 2);
                    turtleQuadRender(ren[i].d + ren[i + 2].d * sinn, ren[i + 1].d - ren[i + 2].d * coss, ren[i + 9].d + ren[i + 2].d * sinn, ren[i + 10].d - ren[i + 2].d * coss, ren[i + 9].d - ren[i + 2].d * sinn, ren[i + 10].d + ren[i + 2].d * coss, ren[i].d - ren[i + 2].d * sinn, ren[i + 1].d + ren[i + 2].d * coss, ren[i + 3].d, ren[i + 4].d, ren[i + 5].d, ren[i + 6].d, xfact, yfact);
                    if ((ren[i + 7].h == 4 || ren[i + 7].h == 5) && i + 18 < len && renType[i + 18] == 'd') {
                        double dir2 = atan((ren[i + 18].d - ren[i + 9].d) / (ren[i + 10].d - ren[i + 19].d));
                        double sinn2 = sin(dir2 + M_PI / 2);
                        double coss2 = cos(dir2 + M_PI / 2);
                        turtleTriangleRender(ren[i + 9].d + ren[i + 2].d * sinn, ren[i + 10].d - ren[i + 2].d * coss, ren[i + 9].d - ren[i + 2].d * sinn, ren[i + 10].d + ren[i + 2].d * coss, ren[i + 9].d + ren[i + 11].d * sinn2, ren[i + 10].d - ren[i + 11].d * coss2, ren[i + 3].d, ren[i + 4].d, ren[i + 5].d, ren[i + 6].d, xfact, yfact); // in a perfect world the program would know which one of these triangles to render (to blend the segments)
                        turtleTriangleRender(ren[i + 9].d + ren[i + 2].d * sinn, ren[i + 10].d - ren[i + 2].d * coss, ren[i + 9].d - ren[i + 2].d * sinn, ren[i + 10].d + ren[i + 2].d * coss, ren[i + 9].d - ren[i + 11].d * sinn2, ren[i + 10].d + ren[i + 11].d * coss2, ren[i + 3].d, ren[i + 4].d, ren[i + 5].d, ren[i + 6].d, xfact, yfact); // however we live in a world where i am bad at math, so it just renders both no matter what (one has no effect)
                    }
                } else {
                    if (ren[i + 7].h == 4 && i > 8 && renType[i - 8] == 'c') {
                        if (!(lastSize == ren[i + 2].d) || !(lastPrez != ren[i + 8].d))
                            precomputedLog = ren[i + 8].d * log(2.71 + ren[i + 2].d);
                        lastSize = ren[i + 2].d;
                        lastPrez = ren[i + 8].d;
                        turtleCircleRender(ren[i].d, ren[i + 1].d, ren[i + 2].d, ren[i + 3].d, ren[i + 4].d, ren[i + 5].d, ren[i + 6].d, xfact, yfact, precomputedLog);
                    }
                    if (ren[i + 7].h == 5 && i > 8) {
                        if (!(lastSize == ren[i + 2].d) || !(lastPrez != ren[i + 8].d))
                            precomputedLog = ren[i + 8].d * log(2.71 + ren[i + 2].d);
                        lastSize = ren[i + 2].d;
                        lastPrez = ren[i + 8].d;
                        turtleCircleRender(ren[i].d, ren[i + 1].d, ren[i + 2].d, ren[i + 3].d, ren[i + 4].d, ren[i + 5].d, ren[i + 6].d, xfact, yfact, precomputedLog);
                    }
                }
                if (ren[i + 7].h == 64) { // blit circle

                }
                if (ren[i + 7].h == 66) { // blit triangle
                    turtleTriangleRender(ren[i].d, ren[i + 1].d, ren[i + 2].d, ren[i + 8].d, ren[i + 9].d, ren[i + 10].d, ren[i + 3].d, ren[i + 4].d, ren[i + 5].d, ren[i + 6].d, xfact, yfact);
                    i += 9;
                }
                if (ren[i + 7].h == 67) { // blit quad
                    turtleQuadRender(ren[i].d, ren[i + 1].d, ren[i + 2].d, ren[i + 8].d, ren[i + 9].d, ren[i + 10].d, ren[i + 11].d, ren[i + 17].d, ren[i + 3].d, ren[i + 4].d, ren[i + 5].d, ren[i + 6].d, xfact, yfact);
                    i += 9;
                }
            }
        }
        glfwSwapBuffers(turtle.window);
    }
    glfwPollEvents();
    if (glfwWindowShouldClose(turtle.window)) {
        turtle.close = 1;
        if (turtle.shouldClose) {
            glfwTerminate();
        }
        
    }
    
}

/* keeps the window open while doing nothing else (from python turtleMainLoop()) */
void turtleMainLoop() {
    while (turtle.close == 0) {
        turtleUpdate();
    }
}

/* free turtle memory */
void turtleFree() {
    list_free(turtle.keyPressed);
    list_free(turtle.penPos);
}
#endif