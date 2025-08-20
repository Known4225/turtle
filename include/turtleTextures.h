/*
████████╗██╗   ██╗██████╗ ████████╗██╗     ███████╗████████╗███████╗██╗  ██╗████████╗██╗   ██╗██████╗ ███████╗███████╗   ██╗  ██╗
╚══██╔══╝██║   ██║██╔══██╗╚══██╔══╝██║     ██╔════╝╚══██╔══╝██╔════╝╚██╗██╔╝╚══██╔══╝██║   ██║██╔══██╗██╔════╝██╔════╝   ██║  ██║
   ██║   ██║   ██║██████╔╝   ██║   ██║     █████╗     ██║   █████╗   ╚███╔╝    ██║   ██║   ██║██████╔╝█████╗  ███████╗   ███████║
   ██║   ██║   ██║██╔══██╗   ██║   ██║     ██╔══╝     ██║   ██╔══╝   ██╔██╗    ██║   ██║   ██║██╔══██╗██╔══╝  ╚════██║   ██╔══██║
   ██║   ╚██████╔╝██║  ██║   ██║   ███████╗███████╗   ██║   ███████╗██╔╝ ██╗   ██║   ╚██████╔╝██║  ██║███████╗███████║██╗██║  ██║
   ╚═╝    ╚═════╝ ╚═╝  ╚═╝   ╚═╝   ╚══════╝╚══════╝   ╚═╝   ╚══════╝╚═╝  ╚═╝   ╚═╝    ╚═════╝ ╚═╝  ╚═╝╚══════╝╚══════╝╚═╝╚═╝  ╚═╝
https://patorjk.com/software/taag/#p=display&f=ANSI%20Shadow

openGL implementation of turtle and the turtletools module
features:
adjustable pen (size and colour)
resizable window support
keyboard and mouse presses
texture support
*/

#ifndef TURTLE_TEXTURES_INTERNAL_H
#define TURTLE_TEXTURES_INTERNAL_H

#include <math.h>
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif
#include "glad.h"
#include "glfw3.h"
#include "list.h"
#include "bufferList.h"

#define BUFFER_OBJECT_SIZE 9

typedef struct GLFWwindow GLFWwindow;

typedef struct {
    GLFWwindow* window; // the window
    list_t *keyPressed; // global keyPressed and mousePressed list
    void (*keyCallback)(int32_t key, int32_t scancode, int32_t action);
    void (*unicodeCallback)(uint32_t codepoint);
    int8_t mousePressed[4]; // cached mouse variables
    int32_t screenbounds[2]; // list of screen bounds (pixels)
    int32_t lastscreenbounds[2]; // list of screen bounds last frame
    int32_t initscreenbounds[2]; // screenbounds at initialisation
    int32_t initbounds[4]; // list of coordinate bounds at initialisation (minX, minY, maxX, maxY)
    int32_t bounds[4]; // list of coordinate bounds (minX, minY, maxX, maxY)
    double centerAndScale[4]; // centerX, centerY, ratioX, ratioY
    double mouseX; // mouseX and mouseY variables
    double mouseY;
    double scrollY;
    double mouseAbsX;
    double mouseAbsY;
    double x; // x and y position of the turtle
    double y;
    list_t *penPos; // a list of where to draw
    bufferList_t *bufferList; // data passed to glBufferData
    uint64_t penHash; // the penPos list is hashed and this hash is used to determine if any changes occured between frames
    uint32_t lastLength; // the penPos list's length is saved and if it is different from last frame we know we have to redraw
    uint8_t pen; // pen status (1 for down, 0 for up)
    uint16_t penshape; // 0 for circle, 1 for square, 2 for triangle
    uint8_t close; // close changes to 1 when the user clicks the x on the window
    uint8_t popupClose; // controls whether the window terminates on turtle.close
    double circleprez; // how precise circles are (specifically, the number of sides of a circle with diameter e)
    double pensize; // turtle pen size
    double penr; // pen red (0 to 1)
    double peng; // pen green (0 to 1)
    double penb; // pen blue (0 to 1)
    double pena; // pen alpha (0 to 1)
    double currentColor[4]; // for reducing API color calls
} turtle_t; // all globals are conSTRUCTed here

extern turtle_t turtle;

/* run this to set the bounds of the window in coordinates */
void turtleSetWorldCoordinates(int32_t minX, int32_t minY, int32_t maxX, int32_t maxY);

/* detect key presses */
void keySense(GLFWwindow *window, int32_t key, int32_t scancode, int32_t action, int32_t mods);

/* detect mouse clicks */
void mouseSense(GLFWwindow* window, int32_t button, int32_t action, int32_t mods);

/* detect scroll wheel */
void scrollSense(GLFWwindow* window, double xoffset, double yoffset);

/* the behavior with the mouse wheel is different since it can't be "on" or "off" */
double turtleMouseWheel();

/* top level boolean output call to check if the key with code [key] is currently being held down. Uses the GLFW_KEY_X macros */
char turtleKeyPressed(int32_t key);

/* top level boolean output call to check if the left click button is currently being held down */
char turtleMouseDown();

/* top level boolean output call to check if the right click button is currently being held down */
char turtleMouseRight();

/* top level boolean output call to check if the middle mouse button is currently being held down */
char turtleMouseMiddle();

/* alternate duplicate of top level boolean output call to check if the middle mouse button is currently being held down */
char turtleMouseMid();

/* initializes the turtletools module */
void turtleInit(GLFWwindow* window, int32_t minX, int32_t minY, int32_t maxX, int32_t maxY);

/* gets the mouse coordinates */
void turtleGetMouseCoords();

/* set the background color */
void turtleBgColor(double r, double g, double b);

/* set the pen color */
void turtlePenColor(double r, double g, double b);

/* set the pen color (with transparency) */
void turtlePenColorAlpha(double r, double g, double b, double a);

/* set the pen size */
void turtlePenSize(double size);

/* clears all the pen drawings */
void turtleClear();

/* pen down */
void turtlePenDown();

/* lift the pen */
void turtlePenUp();

/* set the pen shape ("circle", "square", "triangle", "none", or "connected") */
void turtlePenShape(char *selection);

/* set the circle precision */
void turtlePenPrez(double prez);

/* moves the turtle to a coordinate */
void turtleGoto(double x, double y);

/* function to add a vertex to the turtle.bufferList */
void addVertex(double x, double y, double r, double g, double b, double a, double tx, double ty, double useTexture);

/* draws a circle at the specified x and y (coordinates) */
void turtleCircleRender(double x, double y, double rad, double r, double g, double b, double a, double xfact, double yfact, double prez);

/* draws a square */
void turtleSquareRender(double x1, double y1, double x2, double y2, double r, double g, double b, double a, double xfact, double yfact);

/* draws a triangle */
void turtleTriangleRender(double x1, double y1, double x2, double y2, double x3, double y3, double r, double g, double b, double a, double xfact, double yfact);

/* adds a (blit) triangle to the pipeline (for better speed) */
void turtleTriangle(double x1, double y1, double x2, double y2, double x3, double y3);

void turtleTriangleColor(double x1, double y1, double x2, double y2, double x3, double y3, double r, double g, double b, double a);

/* draws a quadrilateral */
void turtleQuadRender(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double r, double g, double b, double a, double xfact, double yfact);

/* adds a (blit) quad to the pipeline (for better speed) */
void turtleQuad(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4);

void turtleQuadColor(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double r, double g, double b, double a);

/* adds a (blit) rectangle to the pipeline (uses quad interface) */
void turtleRectangle(double x1, double y1, double x2, double y2);

void turtleRectangleColor(double x1, double y1, double x2, double y2, double r, double g, double b, double a);

void turtleTextureRender(int32_t textureCode, double x1, double y1, double x2, double y2, double r, double g, double b, double rot, double xfact, double yfact);

/* adds a (blit) rectangular texture */
void turtleTexture(int32_t textureCode, double x1, double y1, double x2, double y2, double rot, double r, double g, double b);

/* draws the turtle's path on the screen */
void turtleUpdate();

/* keeps the window open while doing nothing else */
void turtleMainLoop();

/* free turtle memory */
void turtleFree();

#endif /* TURTLE_TEXTURES_INTERNAL_H */