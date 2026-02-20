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

/* special function that can be called prior to turtleInit - this function condenses the window creation code boilerplate */
GLFWwindow *turtleCreateWindow(char *windowName);

/* special function that can be called prior to turtleInit - this function condenses the window creation code with icon boilerplate */
GLFWwindow *turtleCreateWindowIcon(char *windowName, char *filename);

typedef struct {
    GLFWwindow *window; // the window
    list_t *keyPressed; // global keyPressed and mousePressed list
    void (*keyCallback)(int32_t key, int32_t scancode, int32_t action);
    void (*unicodeCallback)(uint32_t codepoint);
    int8_t mousePressed[4]; // cached mouse variables
    int32_t screenbounds[2]; // list of screen bounds (pixels)
    int32_t lastscreenbounds[2]; // list of screen bounds last frame
    int32_t initscreenbounds[2]; // screenbounds at initialisation
    int32_t resizeMode; // TURTLE_RESIZE_MODE_PAD or TURTLE_RESIZE_MODE_STRETCH
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
    #else
    /* this bit exists so that there is no size difference between compiled and linked struct (in case you compile without textures but link library with textures) */
    void *bufferList;
    #endif /* TURTLE_ENABLE_TEXTURES */
    list_t *textureList; // filename, original width, original height, channels
    int32_t textureWidth; // turtle texture width (default 1024)
    int32_t textureHeight; // turtle texture height (default 1024)
    int32_t textureBuffer; // size of GPU glTex 2D Array (default 64)
    int32_t maxTextures; // size of GPU glTex 2D Array (default 64)
    uint32_t textureID; // openGL texture handle
    list_t *penPos; // a list of where to draw
    uint64_t penHash; // the penPos list is hashed and this hash is used to determine if any changes occured between frames
    uint32_t lastLength; // the penPos list's length is saved and if it is different from last frame we know we have to redraw
    uint16_t penshape; // 0 for circle, 1 for square, 2 for triangle
    uint8_t pen; // pen status (1 for down, 0 for up)
    uint8_t close; // close changes to 1 when the user clicks the x on the window
    uint8_t popupClose; // controls whether the window terminates on turtle.close
    double circleprez; // how precise circles are (specifically, the number of sides of a circle with diameter e, default: 9)
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
} turtle_t;

typedef enum {
    TURTLE_RESIZE_MODE_PAD = 0,
    TURTLE_RESIZE_MODE_STRETCH = 1,
} turtle_resize_mode_t;

/* stb_image_resize2 pixel_layout enum */
#ifndef STBIRDEF
typedef enum {
    STBIR_1CHANNEL = 1,
    STBIR_2CHANNEL = 2,
    STBIR_RGB      = 3,               // 3-chan, with order specified (for channel flipping)
    STBIR_BGR      = 0,               // 3-chan, with order specified (for channel flipping)
    STBIR_4CHANNEL = 5,

    STBIR_RGBA = 4,                   // alpha formats, where alpha is NOT premultiplied into color channels
    STBIR_BGRA = 6,
    STBIR_ARGB = 7,
    STBIR_ABGR = 8,
    STBIR_RA   = 9,
    STBIR_AR   = 10,

    STBIR_RGBA_PM = 11,               // alpha formats, where alpha is premultiplied into color channels
    STBIR_BGRA_PM = 12,
    STBIR_ARGB_PM = 13,
    STBIR_ABGR_PM = 14,
    STBIR_RA_PM   = 15,
    STBIR_AR_PM   = 16,

    STBIR_RGBA_NO_AW = 11,            // alpha formats, where NO alpha weighting is applied at all!
    STBIR_BGRA_NO_AW = 12,            //   these are just synonyms for the _PM flags (which also do
    STBIR_ARGB_NO_AW = 13,            //   no alpha weighting). These names just make it more clear
    STBIR_ABGR_NO_AW = 14,            //   for some folks).
    STBIR_RA_NO_AW   = 15,
    STBIR_AR_NO_AW   = 16,
} stbir_pixel_layout;
#endif

extern turtle_t turtle;

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

/* initialises the turtle module, supply coordinate bounds */
void turtleInit(GLFWwindow *window, double leftX, double bottomY, double rightX, double topY);

/* gets the mouse coordinates */
void turtleGetMouseCoords();

/* set the background color */
void turtleBgColor(uint8_t r, uint8_t g, uint8_t b);

/* set the pen color */
void turtlePenColor(uint8_t r, uint8_t g, uint8_t b);

/* set the pen color (with transparency) */
void turtlePenColorAlpha(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

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

/* texture handle */
typedef int32_t turtle_texture_t;

#ifdef TURTLE_ENABLE_TEXTURES
/* function to add a vertex to the turtle.bufferList */
void addVertex(double x, double y, double r, double g, double b, double a, double tx, double ty, double useTexture);

void turtleTextureRenderInternal(int32_t textureCode, double x1, double y1, double x2, double y2, double r, double g, double b, double rot, double xcenter, double ycenter, double xfact, double yfact);
#endif /* TURTLE_ENABLE_TEXTURES */

/* set pixel width and height of textures (determines how blurry pictures are, default 1024, 1024) - must be done BEFORE turtleInit */
void turtleSetTextureSize(int32_t width, int32_t height);

/* set maximum number of textures (default 64) - must be done BEFORE turtleInit */
void turtleSetMaxTextures(int32_t maxTextures);

/* set resize mode of turtle (default TURTLE_RESIZE_MODE_PAD) - must be done BEFORE turtleInit */
void turtleSetResizeMode(turtle_resize_mode_t resizeMode);

/* load a png, jpg, or bmp to GPU memory as a texture */
turtle_texture_t turtleTextureLoad(char *filename);

/* load data from an array - supported encodings: GL_RGB, GL_RGBA, GL_BGR, GL_BGRA, GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA */
turtle_texture_t turtleTextureLoadArray(uint8_t *array, uint32_t width, uint32_t height, uint32_t encoding);

/* load data from a list - supported encodings: GL_RGB, GL_RGBA, GL_BGR, GL_BGRA, GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA */
turtle_texture_t turtleTextureLoadList(list_t *list, uint32_t width, uint32_t height, uint32_t encoding);

/* load data from a list or array of uint8 (make one NULL) - supported encodings: GL_RGB, GL_RGBA, GL_BGR, GL_BGRA, GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA */
turtle_texture_t turtleTextureLoadListArrayInternal(list_t *list, uint8_t *array, uint32_t width, uint32_t height, uint32_t encoding);

/* get the original width of a loaded texture */
int32_t turtleTextureGetWidth(turtle_texture_t texture);

/* get the original height of a loaded texture */
int32_t turtleTextureGetHeight(turtle_texture_t texture);

/* print texture name, width, height, and channels */
void turtleTexturePrint(turtle_texture_t texture);

/* replace a texture with new data from a png, jpg, or bmp */
int32_t turtleTextureReplace(turtle_texture_t texture, char *filename);

/* replace a texture with new data from an array - supported encodings: GL_RGB, GL_RGBA, GL_BGR, GL_BGRA, GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA */
int32_t turtleTextureReplaceArray(turtle_texture_t texture, uint8_t *array, uint32_t width, uint32_t height, uint32_t encoding);

/* replace a texture with new data from a list - supported encodings: GL_RGB, GL_RGBA, GL_BGR, GL_BGRA, GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA */
int32_t turtleTextureReplaceList(turtle_texture_t texture, list_t *list, uint32_t width, uint32_t height, uint32_t encoding);

/* replace a texture with new data from a list or array of uint8 (make one NULL) - supported encodings: GL_RGB, GL_RGBA, GL_BGR, GL_BGRA, GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA */
int32_t turtleTextureReplaceListArrayInternal(turtle_texture_t texture, list_t *list, uint8_t *array, uint32_t width, uint32_t height, uint32_t encoding);

/* remove a texture from GPU memory */
int32_t turtleTextureUnload(turtle_texture_t texture);

/* remove all textures from GPU memory */
int32_t turtleTextureUnloadAll();

/* adds a (blit) rectangular texture */
void turtleTexture(turtle_texture_t texture, double x1, double y1, double x2, double y2, double rot, uint8_t r, uint8_t g, uint8_t b);

/* draws a circle at the specified x and y (coordinates) */
void turtleCircleRenderInternal(double x, double y, double rad, double r, double g, double b, double a, double xcenter, double ycenter, double xfact, double yfact, double prez);

/* draws a rectangle */
void turtleRectangleRenderInternal(double x1, double y1, double x2, double y2, double r, double g, double b, double a, double xcenter, double ycenter, double xfact, double yfact);

/* draws a triangle */
void turtleTriangleRenderInternal(double x1, double y1, double x2, double y2, double x3, double y3, double r, double g, double b, double a, double xcenter, double ycenter, double xfact, double yfact);

/* draws a quadrilateral */
void turtleQuadRenderInternal(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double r, double g, double b, double a, double xcenter, double ycenter, double xfact, double yfact);

/* adds a (blit) triangle to the pipeline (for better speed) */
void turtleTriangle(double x1, double y1, double x2, double y2, double x3, double y3);

void turtleTriangleColor(double x1, double y1, double x2, double y2, double x3, double y3, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

/* adds a (blit) quad to the pipeline (for better speed) */
void turtleQuad(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4);

void turtleQuadColor(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

/* adds a (blit) rectangle to the pipeline (uses quad interface) */
void turtleRectangle(double x1, double y1, double x2, double y2);

void turtleRectangleColor(double x1, double y1, double x2, double y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

/* adds a (blit) circle to the pipeline */
void turtleCircle(double x, double y, double radius);

void turtleCircleColor(double x, double y, double radius, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

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
