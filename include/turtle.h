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
#include "floatList.h"
#define BUFFER_OBJECT_SIZE 9
#endif /* TURTLE_ENABLE_TEXTURES */

/* required forward declarations (for packaging) */
extern void glColor4d(double r, double g, double b, double a);
extern void glBegin(int type);
extern void glVertex2d(double x, double y);
extern void glEnd();
typedef struct GLFWwindow GLFWwindow;

enum {
    TURTLE_WINDOW_DUMMY = -10000,
    TURTLE_WINDOW_DEFAULT_WIDTH, // 16.0 / 9.0 * TURTLE_WINDOW_DEFAULT_HEIGHT
    TURTLE_WINDOW_DEFAULT_HEIGHT, // 80% of primary monitor height
    TURTLE_WINDOW_MONITOR_WIDTH, // primary monitor width
    TURTLE_WINDOW_MONITOR_HEIGHT, // primary monitor height
};

/* special function that can be called prior to turtle_init() - this function condenses the window creation code boilerplate */
GLFWwindow *turtle_create_window(int32_t windowWidth, int32_t windowHeight, char *windowName);

/* special function that can be called prior to turtle_init() - this function condenses the window creation code with icon boilerplate */
GLFWwindow *turtle_create_window_icon(int32_t windowWidth, int32_t windowHeight, char *windowName, char *filename);

typedef struct {
    GLFWwindow *window; // the window
    list_t *keyPressed; // global keyPressed and mousePressed list
    void (*keyCallback)(int32_t key, int32_t scancode, int32_t action);
    void (*unicodeCallback)(uint32_t codepoint);
    int8_t mousePressed[4]; // cached mouse variables
    int32_t screenbounds[2]; // current window size (x, y) (pixels)
    int32_t lastscreenbounds[2]; // last frame window size (x, y) (pixels)
    int32_t initscreenbounds[2]; // window size (x, y) (pixels) at initialisation
    int32_t resizeMode; // TURTLE_RESIZE_MODE_PAD, TURTLE_RESIZE_MODE_STRETCH, TURTLE_RESIZE_MODE_PAD_NO_BARS (call turtle_set_resize_mode() prior to turtle_init() to change)
    double initbounds[4]; // list of coordinate bounds at initialisation (leftX, bottomY, rightX, topY)
    double bounds[4]; // list of coordinate bounds (leftX, bottomY, rightX, topY)
    double centerAndScale[4]; // centerX, centerY, ratioX, ratioY
    double aspect; // aspect ratio
    double mouseX; // coordinate x position of mouse cursor (must call turtle_get_mouse_coordinates() to update)
    double mouseY; // coordinate y position of mouse cursor (must call turtle_get_mouse_coordinates() to update)
    double scrollY; // call turtle_mouse_wheel to update
    double mouseAbsX; // absolute x position of mouse cursor (in pixels) (must call turtle_get_mouse_coordinates() to update)
    double mouseAbsY; // absolute y position of mouse cursor (in pixels) (must call turtle_get_mouse_coordinates() to update)
    double x; // coordinate x position of turtle
    double y; // coordinate y position of turtle
    #ifdef TURTLE_ENABLE_TEXTURES
    floatList_t *bufferList; // resizable list to donate to GPU
    #else
    /* this bit exists so that there is no size difference between compiled and linked struct (in case you compile without textures but link library with textures) */
    void *bufferList;
    #endif /* TURTLE_ENABLE_TEXTURES */
    list_t *textureList; // filename, original width, original height, channels
    int32_t textureWidth; // turtle texture width (default 1024) (call turtle_set_texture_size() prior to turtle_init() to change)
    int32_t textureHeight; // turtle texture height (default 1024) (call turtle_set_texture_size() prior to turtle_init() to change)
    int32_t textureBuffer; // size of GPU glTex 2D Array (default 64)
    int32_t maxTextures; // size of GPU glTex 2D Array (default 64) (call turtle_set_max_textures() prior to turtle_init() to change)
    uint32_t textureID; // openGL texture handle
    list_t *penPos; // a list of where to draw
    uint64_t penHash; // the penPos list is hashed and this hash is used to determine if any changes occured between frames
    uint32_t lastLength; // the penPos list's length is saved and if it is different from last frame we know we have to redraw
    uint16_t penshape; // 0 for circle, 1 for square, 2 for triangle
    uint8_t pen; // pen status (1 for down, 0 for up)
    uint8_t close; // close changes to 1 when the user clicks the x on the window
    uint8_t popupClose; // controls whether the window terminates on turtle.close
    uint8_t windowSpecial; // special features of window creation
    uint8_t forceUpdate; // toggle to skip check to see if screen has changed
    double circleprez; // how precise circles are (specifically, the number of sides of a circle with diameter e, default: 9)
    double pensize; // turtle pen size
    uint8_t red; // pen red (0 to 255)
    uint8_t green; // pen green (0 to 255)
    uint8_t blue; // pen blue (0 to 255)
    uint8_t alpha; // pen alpha (0 to 255)
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
    TURTLE_RESIZE_MODE_PAD_NO_BARS = 2,
} turtle_resize_mode_t;

typedef enum {
    TURTLE_IMAGE_RESIZE_SRGB = 0,
    TURTLE_IMAGE_RESIZE_LINEAR = 1,
    TURTLE_IMAGE_RESIZE_NEAREST = 2,
} turtle_image_resize_t;

typedef enum {
    TURTLE_PEN_SHAPE_CIRCLE = 0,
    TURTLE_PEN_SHAPE_SQUARE = 1,
    TURTLE_PEN_SHAPE_TRIANGLE = 2,
    TURTLE_PEN_SHAPE_NONE = 3,
    TURTLE_PEN_SHAPE_CONNECTED = 4,
    TURTLE_PEN_SHAPE_TEXT = 5,
} turtle_pen_shape_t;

extern turtle_t turtle;

/* run this to set the bounds of the window in coordinates */
void turtle_set_world_coordinates(double leftX, double bottomY, double rightX, double topY);

/* detect character */
void turtle_unicode_sense(GLFWwindow *window, uint32_t codepoint);

/* detect key presses */
void turtle_key_sense(GLFWwindow *window, int32_t key, int32_t scancode, int32_t action, int32_t mods);

/* detect mouse clicks */
void turtle_mouse_sense(GLFWwindow *window, int32_t button, int32_t action, int32_t mods);

/* detect scroll wheel */
void turtle_scroll_sense(GLFWwindow *window, double xoffset, double yoffset);

/* the behavior with the mouse wheel is different since it can't be "on" or "off" */
double turtle_mouse_wheel();

/* top level boolean output call to check if the key with code [key] is currently being held down. Uses the GLFW_KEY_X macros */
int8_t turtle_key_pressed(int32_t key);

/* top level boolean output call to check if the left click button is currently being held down */
int8_t turtle_mouse_down();

/* alternate duplicate of turtle_mouse_down() */
int8_t turtle_mouse_left();

/* top level boolean output call to check if the right click button is currently being held down */
int8_t turtle_mouse_right();

/* top level boolean output call to check if the middle mouse button is currently being held down */
int8_t turtle_mouse_middle();

/* alternate duplicate of turtle_mouse_middle() */
int8_t turtle_mouse_mid();

/* initialises the turtle module, supply coordinate bounds */
void turtle_init(GLFWwindow *window, double leftX, double bottomY, double rightX, double topY);

/* puts the mouse coordinates in turtle.mouseX and turtle.mouseY */
void turtle_get_mouse_coordinates();

/* set the background color */
void turtle_background_color(uint8_t r, uint8_t g, uint8_t b);

/* set the pen color */
void turtle_pen_color(uint8_t r, uint8_t g, uint8_t b);

/* set the pen color (with transparency) */
void turtle_pen_color_alpha(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

/* set the pen size */
void turtle_pen_size(double size);

/* clears all the pen drawings */
void turtle_clear();

/* pen down */
void turtle_pen_down();

/* lift the pen */
void turtle_pen_up();

/* set the pen shape ("circle", "square", "triangle", "none", or "connected") */
void turtle_pen_shape(turtle_pen_shape_t shape);

/* set the circle precision (default 9) */
void turtle_pen_prez(double prez);

/* moves the turtle to a coordinate */
void turtle_goto(double x, double y);

/* texture handle */
typedef int32_t turtle_texture_t;

#ifdef TURTLE_ENABLE_TEXTURES
/* function to add a vertex to the turtle.bufferList */
void turtle_add_vertex(double x, double y, double r, double g, double b, double a, double tx, double ty, double useTexture);

void turtle_texture_render_internal(int32_t textureCode, double x1, double y1, double x2, double y2, double r, double g, double b, double rot, double xcenter, double ycenter, double xfact, double yfact);
#endif /* TURTLE_ENABLE_TEXTURES */

/* set pixel width and height of textures (determines how much memory textures take in the GPU, default 1024, 1024) - must be done BEFORE turtle_init() */
void turtle_set_texture_size(int32_t width, int32_t height);

/* set maximum number of textures (default 32) - must be done BEFORE turtle_init() */
void turtle_set_max_textures(int32_t maxTextures);

/* set resize mode of turtle (TURTLE_RESIZE_MODE_PAD, TURTLE_RESIZE_MODE_STRETCH, or TURTLE_RESIZE_MODE_PAD_NO_BARS) (default TURTLE_RESIZE_MODE_PAD) - must be done BEFORE turtle_init() */
void turtle_set_resize_mode(turtle_resize_mode_t resizeMode);

/* resize an image - supported encodings: GL_RGB, GL_RGBA, GL_BGR, GL_BGRA, GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA, supported methods: TURTLE_IMAGE_RESIZE_SRGB, TURTLE_IMAGE_RESIZE_LINEAR, TURTLE_IMAGE_RESIZE_NEAREST */
uint8_t *turtle_image_resize(uint8_t *dest, uint32_t destWidth, uint32_t destHeight, uint32_t destEncoding, uint8_t *src, uint32_t srcWidth, uint32_t srcHeight, uint32_t srcEncoding, turtle_image_resize_t method);

/* load a png, jpg, or bmp to GPU memory as a texture */
turtle_texture_t turtle_texture_load(char *filename);

/* load data from an array - supported encodings: GL_RGB, GL_RGBA, GL_BGR, GL_BGRA, GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA */
turtle_texture_t turtle_texture_load_array(uint8_t *array, uint32_t width, uint32_t height, uint32_t encoding);

/* load data from a list - supported encodings: GL_RGB, GL_RGBA, GL_BGR, GL_BGRA, GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA */
turtle_texture_t turtle_texture_load_list(list_t *list, uint32_t width, uint32_t height, uint32_t encoding);

/* load data from a list or array of uint8 (make one NULL) - supported encodings: GL_RGB, GL_RGBA, GL_BGR, GL_BGRA, GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA */
turtle_texture_t turtle_texture_load_list_array_internal(list_t *list, uint8_t *array, uint32_t width, uint32_t height, uint32_t encoding);

/* get the original width of a loaded texture */
int32_t turtle_texture_get_width(turtle_texture_t texture);

/* get the original height of a loaded texture */
int32_t turtle_texture_get_height(turtle_texture_t texture);

/* print texture name, width, height, and channels */
void turtle_texture_print(turtle_texture_t texture);

/* replace a texture with new data from a png, jpg, or bmp */
int32_t turtle_texture_replace(turtle_texture_t texture, char *filename);

/* replace a texture with new data from an array - supported encodings: GL_RGB, GL_RGBA, GL_BGR, GL_BGRA, GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA */
int32_t turtle_texture_replace_array(turtle_texture_t texture, uint8_t *array, uint32_t width, uint32_t height, uint32_t encoding);

/* replace a texture with new data from a list - supported encodings: GL_RGB, GL_RGBA, GL_BGR, GL_BGRA, GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA */
int32_t turtle_texture_replace_list(turtle_texture_t texture, list_t *list, uint32_t width, uint32_t height, uint32_t encoding);

/* replace a texture with new data from a list or array of uint8 (make one NULL) - supported encodings: GL_RGB, GL_RGBA, GL_BGR, GL_BGRA, GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA */
int32_t turtle_texture_replace_list_array_internal(turtle_texture_t texture, list_t *list, uint8_t *array, uint32_t width, uint32_t height, uint32_t encoding);

/* remove a texture from GPU memory */
int32_t turtle_texture_unload(turtle_texture_t texture);

/* remove all textures from GPU memory */
int32_t turtle_texture_unload_all();

/* adds a (blit) rectangular texture */
void turtle_texture(turtle_texture_t texture, double x1, double y1, double x2, double y2, double rot);

void turtle_texture_color(turtle_texture_t texture, double x1, double y1, double x2, double y2, double rot, uint8_t r, uint8_t g, uint8_t b);

/* draws a circle at the specified x and y (coordinates) */
void turtle_circle_render_internal(double x, double y, double rad, double r, double g, double b, double a, double xcenter, double ycenter, double xfact, double yfact, double prez);

/* draws a rectangle */
void turtle_rectangle_render_internal(double x1, double y1, double x2, double y2, double r, double g, double b, double a, double xcenter, double ycenter, double xfact, double yfact);

/* draws a triangle */
void turtle_triangle_render_internal(double x1, double y1, double x2, double y2, double x3, double y3, double r, double g, double b, double a, double xcenter, double ycenter, double xfact, double yfact);

/* draws a quadrilateral */
void turtle_quad_render_internal(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double r, double g, double b, double a, double xcenter, double ycenter, double xfact, double yfact);

/* adds a (blit) triangle to the pipeline (for better speed) */
void turtle_triangle(double x1, double y1, double x2, double y2, double x3, double y3);

void turtle_triangle_color(double x1, double y1, double x2, double y2, double x3, double y3, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

/* adds a (blit) quad to the pipeline (for better speed) */
void turtle_quad(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4);

void turtle_quad_color(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

/* adds a (blit) rectangle to the pipeline (uses quad interface) */
void turtle_rectangle(double x1, double y1, double x2, double y2);

void turtle_rectangle_color(double x1, double y1, double x2, double y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

/* adds a (blit) circle to the pipeline */
void turtle_circle(double x, double y, double radius);

void turtle_circle_color(double x, double y, double radius, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

/* create a triangle in 3D */
void turtle_3D_Triangle(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3);

/* 3D -> 2D using perspective projection matrix */
void turtle_perspective(double x, double y, double z, double *xOut, double *yOut);

/* draws the turtle's path on the screen */
void turtle_update();

/* keeps the window open while doing nothing else (from python turtle.mainloop()) */
void turtle_main_loop();

/* free turtle memory */
void turtle_free();

#endif /* TURTLE_INTERNAL_H */
