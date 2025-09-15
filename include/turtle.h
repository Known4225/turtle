/*
████████╗██╗   ██╗██████╗ ████████╗██╗     ███████╗   ██╗  ██╗
╚══██╔══╝██║   ██║██╔══██╗╚══██╔══╝██║     ██╔════╝   ██║  ██║
   ██║   ██║   ██║██████╔╝   ██║   ██║     █████╗     ███████║
   ██║   ██║   ██║██╔══██╗   ██║   ██║     ██╔══╝     ██╔══██║
   ██║   ╚██████╔╝██║  ██║   ██║   ███████╗███████╗██╗██║  ██║
   ╚═╝    ╚═════╝ ╚═╝  ╚═╝   ╚═╝   ╚══════╝╚══════╝╚═╝╚═╝  ╚═╝
https://patorjk.com/software/taag/#p=display&f=ANSI%20Shadow
*/

#ifndef TURTLE_INTERNAL_H
#define TURTLE_INTERNAL_H

#include <math.h>
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif
#include "glad.h"
#include "glfw3.h"
#include "list.h"
#ifdef TURTLE_ENABLE_TEXTURES
#include "bufferList.h"
#define BUFFER_OBJECT_SIZE 9
#endif /* TURTLE_ENABLE_TEXTURES */

/* required forward declarations (for packaging) */
extern void glColor4d(double r, double g, double b, double a); // genius tactic to stop compiler warnings
extern void glBegin(int type);
extern void glVertex2d(double x, double y);
extern void glEnd();
typedef struct GLFWwindow GLFWwindow;

typedef struct {
    GLFWwindow *window; // the window
    list_t *keyPressed; // global keyPressed and mousePressed list
    void (*keyCallback)(int32_t key, int32_t scancode, int32_t action);
    void (*unicodeCallback)(uint32_t codepoint);
    int8_t mousePressed[4]; // cached mouse variables
    int32_t screenbounds[2]; // list of screen bounds (pixels)
    int32_t lastscreenbounds[2]; // list of screen bounds last frame
    int32_t initscreenbounds[2]; // screenbounds at initialisation
    double initbounds[4]; // list of coordinate bounds at initialisation (leftX, bottomY, rightX, topY)
    double bounds[4]; // list of coordinate bounds (leftX, bottomY, rightX, topY)
    double centerAndScale[4]; // centerX, centerY, ratioX, ratioY
    double mouseX; // mouseX and mouseY variables
    double mouseY;
    double scrollY;
    double mouseAbsX;
    double mouseAbsY;
    double x; // x and y position of the turtle
    double y;
    #ifdef TURTLE_ENABLE_TEXTURES
    bufferList_t *bufferList; // resizable list to donate to GPU
    list_t *textureList; // list of texture filenames (set to "" for unloaded)
    int32_t textureWidth;
    int32_t textureHeight;
    #else
    /* this bit exists so that there is no size difference between compiled and linked struct (in case you compile without textures but link library with textures) */
    void *bufferList;
    list_t *textureList;
    int32_t textureWidth;
    int32_t textureHeight;
    #endif /* TURTLE_ENABLE_TEXTURES */
    list_t *penPos; // a list of where to draw
    uint64_t penHash; // the penPos list is hashed and this hash is used to determine if any changes occured between frames
    uint32_t lastLength; // the penPos list's length is saved and if it is different from last frame we know we have to redraw
    uint16_t penshape; // 0 for circle, 1 for square, 2 for triangle
    uint8_t pen; // pen status (1 for down, 0 for up)
    uint8_t close; // close changes to 1 when the user clicks the x on the window
    uint8_t popupClose; // controls whether the window terminates on turtle.close
    double circleprez; // how precise circles are (specifically, the number of sides of a circle with diameter e)
    double pensize; // turtle pen size
    double penr; // pen red (0 to 1)
    double peng; // pen green (0 to 1)
    double penb; // pen blue (0 to 1)
    double pena; // pen alpha (0 to 1)
    double currentColor[4]; // for reducing API color calls

    /* 3D variables */
    double cameraX;
    double cameraY;
    double cameraZ;
    double cameraFOV;
    double cameraDirectionLeftRight;
    double cameraDirectionUpDown;
} turtle_t; // all globals are conSTRUCTed here

extern turtle_t turtle;

/* initialises the turtle module */
void turtleInit(GLFWwindow *window, double leftX, double bottomY, double rightX, double topY);

/* run this to set the bounds of the window in coordinates */
void turtleSetWorldCoordinates(double leftX, double bottomY, double rightX, double topY);

/* detect character */
void unicodeSense(GLFWwindow *window, uint32_t codepoint);

/* detect key presses */
void keySense(GLFWwindow *window, int32_t key, int32_t scancode, int32_t action, int32_t mods);

/* detect mouse clicks */
void mouseSense(GLFWwindow *window, int32_t button, int32_t action, int32_t mods);

/* detect scroll wheel */
void scrollSense(GLFWwindow *window, double xoffset, double yoffset);

/* the behavior with the mouse wheel is different since it can't be "on" or "off" */
double turtleMouseWheel();

/* top level boolean output call to check if the key with code [key] is currently being held down. Uses the GLFW_KEY_X macros */
int8_t turtleKeyPressed(int32_t key);

/* top level boolean output call to check if the left click button is currently being held down */
int8_t turtleMouseDown();

/* top level boolean output call to check if the right click button is currently being held down */
int8_t turtleMouseRight();

/* top level boolean output call to check if the middle mouse button is currently being held down */
int8_t turtleMouseMiddle();

/* alternate duplicate of top level boolean output call to check if the middle mouse button is currently being held down */
int8_t turtleMouseMid();

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

typedef struct {
    int32_t id;
    int16_t width;
    int16_t height;
} turtle_texture_t;

#ifdef TURTLE_ENABLE_TEXTURES
/* function to add a vertex to the turtle.bufferList */
void addVertex(double x, double y, double r, double g, double b, double a, double tx, double ty, double useTexture);

void turtleTextureRenderInternal(int32_t textureCode, double x1, double y1, double x2, double y2, double r, double g, double b, double rot, double xcenter, double ycenter, double xfact, double yfact);
#endif /* TURTLE_ENABLE_TEXTURES */

/* load a png, jpg, or bmp to GPU memory as a texture */
turtle_texture_t turtleTextureLoad(char *filename);

/* load data from a list or array of uint8 (make one NULL) - use GL_RGB or GL_RGBA for encoding */
turtle_texture_t turtleTextureLoadList(list_t *list, uint8_t *array, uint32_t width, uint32_t height, uint32_t encoding);

/* remove a texture from GPU memory */
int32_t turtleTextureUnload(turtle_texture_t texture);

/* adds a (blit) rectangular texture */
void turtleTexture(turtle_texture_t texture, double x1, double y1, double x2, double y2, double rot, double r, double g, double b);

/* print texture information */
void turtlePrintTexture(turtle_texture_t texture);

/* draws a circle at the specified x and y (coordinates) */
void turtleCircleRenderInternal(double x, double y, double rad, double r, double g, double b, double a, double xcenter, double ycenter, double xfact, double yfact, double prez);

/* draws a square */
void turtleSquareRenderInternal(double x1, double y1, double x2, double y2, double r, double g, double b, double a, double xcenter, double ycenter, double xfact, double yfact);

/* draws a triangle */
void turtleTriangleRenderInternal(double x1, double y1, double x2, double y2, double x3, double y3, double r, double g, double b, double a, double xcenter, double ycenter, double xfact, double yfact);

/* draws a quadrilateral */
void turtleQuadRenderInternal(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double r, double g, double b, double a, double xcenter, double ycenter, double xfact, double yfact);

/* adds a (blit) triangle to the pipeline (for better speed) */
void turtleTriangle(double x1, double y1, double x2, double y2, double x3, double y3);

void turtleTriangleColor(double x1, double y1, double x2, double y2, double x3, double y3, double r, double g, double b, double a);

/* adds a (blit) quad to the pipeline (for better speed) */
void turtleQuad(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4);

void turtleQuadColor(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double r, double g, double b, double a);

/* adds a (blit) rectangle to the pipeline (uses quad interface) */
void turtleRectangle(double x1, double y1, double x2, double y2);

void turtleRectangleColor(double x1, double y1, double x2, double y2, double r, double g, double b, double a);

/* adds a (blit) circle to the pipeline */
void turtleCircle(double x, double y, double radius);

/* create a triangle in 3D */
void turtle3DTriangle(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3);

/* 3D -> 2D using perspective projection matrix */
void turtlePerspective(double x, double y, double z, double *xOut, double *yOut);

/* draws the turtle's path on the screen */
void turtleUpdate();

/* keeps the window open while doing nothing else (from python turtleMainLoop()) */
void turtleMainLoop();

/* free turtle memory */
void turtleFree();

#endif /* TURTLE_INTERNAL_H */
