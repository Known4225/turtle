#ifndef TURTLE_H
#define TURTLE_H

/*
██╗     ██╗███████╗████████╗██╗  ██╗
██║     ██║██╔════╝╚══██╔══╝██║  ██║
██║     ██║███████╗   ██║   ███████║
██║     ██║╚════██║   ██║   ██╔══██║
███████╗██║███████║   ██║██╗██║  ██║
╚══════╝╚═╝╚══════╝   ╚═╝╚═╝╚═╝  ╚═╝
https://patorjk.com/software/taag/#p=display&f=ANSI%20Shadow
*/

#ifndef UNITYPE_LIST_H
#define UNITYPE_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

/*
21.04.23:
unitype list, supports a variety of types

example usage:
list_t *newList = list_init();
list_append(newList, (unitype) "hello", 's'); // add to list
list_append(newList, (unitype) "test", LIST_TYPE_STRING); // add to list using enum
list_print(newList);
> [hello, test]
list_insert(newList, (unitype) 128, 'i', 1);
list_insert(newList, (unitype) 12.0, 'd', 1);
list_print(newList);
> [hello, 12.0, 128, test]
printf("%s\n", newList -> data[3].s);
> test
printf("%d\n", newList -> length);
> 4 
list_delete(newList, 0);
list_print(newList);
> [12.0, 128, test]
list_clear(newList);
list_print(newList);
> []
list_free(newList);

Notes:
Strings added to the list will be "strdup"d - meaning that you can pass in stack allocated buffers and string literals. This does not apply to pointers added to the list which must be heap allocated
When calling list_clear() or list_free(), the list will free all strings, pointers, and lists within itself. If you don't want this to happen append the item to the list as a uint64
list_copy will make a copy of all strings, pointers, and lists - it will not use the same pointers (a list can be safely freed after being copied without causing effects to the copied list)
You must call list_init() when intending to copy a list - all lists must be initialised before any functions can be called on them (if your program is crashing - check to make sure you initialised all your lists)
*/

typedef enum {
    LIST_TYPE_BOOL = 'b',
    LIST_TYPE_CHAR = 'c',
    LIST_TYPE_INT8 = 'e',
    LIST_TYPE_UINT8 = 'b',
    LIST_TYPE_INT16 = 'j',
    LIST_TYPE_UINT16 = 'h',
    LIST_TYPE_INT32 = 'i',
    LIST_TYPE_UINT32 = 'u',
    LIST_TYPE_INT64 = 'm',
    LIST_TYPE_UINT64 = 'l',
    LIST_TYPE_FLOAT = 'f',
    LIST_TYPE_DOUBLE = 'd',
    LIST_TYPE_STRING = 's',
    LIST_TYPE_POINTER = 'p',
    LIST_TYPE_LIST = 'r',
} list_type_t;

struct list_f; // so basically im a good programmer
typedef struct list_f list_t;

typedef union { // supported types
    signed char ch;
    bool bo;
    int8_t c;
    uint8_t b;
    int16_t h;
    uint16_t hu;
    int32_t i;
    uint32_t u;
    int64_t li;
    uint64_t l;
    float f;
    double d; // i will stand by it. d should be double
    double lf;
    char *s;
    void *p;
    list_t *r;
    list_t *list;
} unitype;

struct list_f {
    uint32_t length;
    uint32_t realLength;
    int8_t *type;
    unitype *data;
};

/* create a list */
list_t *list_init();

/* append to list, must specify type */
void list_append(list_t *list, unitype data, char type);

/* inserts the item value at list[index] of the list */
void list_insert(list_t *list, int32_t index, unitype value, char type);

/* clears the list */
void list_clear(list_t *list);

/* pops the last item of the list off and returns it */
unitype list_pop(list_t *list);

/* deletes the item at list[index] of the list and returns it */
unitype list_delete(list_t *list, int32_t index);

/* deletes many items from the list spanning from [indexMin] to [indexMax - 1] */
void list_delete_range(list_t *list, uint32_t indexMin, uint32_t indexMax);

/* checks if two unitype items are equal */
int32_t unitype_check_equal(unitype item1, unitype item2, int8_t typeItem1, int8_t typeItem2);

/* returns the index of the first instance of the item in the list, returns -1 if not found */
int32_t list_find(list_t *list, unitype item, char type);

/* duplicate of list_find */
int32_t list_index(list_t *list, unitype item, char type);

/* counts how many instances of an item is found in the list */
uint32_t list_count(list_t *list, unitype item, char type);

/* sort list */
void list_sort(list_t *list);

/* deletes the first instance of the item from the list, returns the index the item was at, returns -1 and doesn't modify the list if not found */
int32_t list_remove(list_t *list, unitype item, char type);

/* prints a unitype item */
void unitype_fprint(FILE *fp, unitype item, char type);

/* copies one list to another (duplicates strings or pointers) */
void list_copy(list_t *dest, list_t *src);

/* prints the list without brackets */
void list_fprint_emb(FILE *fp, list_t *list);

/* prints the list */
void list_print(list_t *list);

/* prints the types of the list */
void list_print_type(list_t *list);

/* frees the list's data but not the list itself */
void list_free_lite(list_t *list);

/* frees the data used by the list */
void list_free(list_t *list);

#endif /* UNITYPE_LIST_H */

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
    int32_t initbounds[4]; // list of coordinate bounds at initialisation (minX, minY, maxX, maxY)
    int32_t bounds[4]; // list of coordinate bounds (minX, minY, maxX, maxY)
    double centerAndScale[4]; // centerX, centerY, ratioX, ratioY
    double mouseX; // mouseX and mouseY variables
    double mouseY;
    double scrollY;
    // double mouseScaX;
    // double mouseScaY;
    double mouseAbsX;
    double mouseAbsY;
    double x; // x and y position of the turtle
    double y;
    list_t *penPos; // a list of where to draw
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

    /* 3D variables */
    double cameraX;
    double cameraY;
    double cameraZ;
    double cameraFOV;
    double cameraDirectionLeftRight;
    double cameraDirectionUpDown;
} turtle_t; // all globals are conSTRUCTed here

extern turtle_t turtle;

/* run this to set the bounds of the window in coordinates */
void turtleSetWorldCoordinates(int32_t minX, int32_t minY, int32_t maxX, int32_t maxY);

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

/* initialises the turtle module */
void turtleInit(GLFWwindow *window, int32_t minX, int32_t minY, int32_t maxX, int32_t maxY);

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

/* draws a circle at the specified x and y (coordinates) */
void turtleCircleRender(double x, double y, double rad, double r, double g, double b, double a, double xcenter, double ycenter, double xfact, double yfact, double prez);

/* draws a square */
void turtleSquareRender(double x1, double y1, double x2, double y2, double r, double g, double b, double a, double xcenter, double ycenter, double xfact, double yfact);

/* draws a triangle */
void turtleTriangleRender(double x1, double y1, double x2, double y2, double x3, double y3, double r, double g, double b, double a, double xcenter, double ycenter, double xfact, double yfact);

/* draws a quadrilateral */
void turtleQuadRender(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double r, double g, double b, double a, double xcenter, double ycenter, double xfact, double yfact);

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

/*
████████╗██╗   ██╗██████╗ ████████╗██╗     ███████╗████████╗███████╗██╗  ██╗████████╗██╗  ██╗
╚══██╔══╝██║   ██║██╔══██╗╚══██╔══╝██║     ██╔════╝╚══██╔══╝██╔════╝╚██╗██╔╝╚══██╔══╝██║  ██║
   ██║   ██║   ██║██████╔╝   ██║   ██║     █████╗     ██║   █████╗   ╚███╔╝    ██║   ███████║
   ██║   ██║   ██║██╔══██╗   ██║   ██║     ██╔══╝     ██║   ██╔══╝   ██╔██╗    ██║   ██╔══██║
   ██║   ╚██████╔╝██║  ██║   ██║   ███████╗███████╗   ██║   ███████╗██╔╝ ██╗   ██║██╗██║  ██║
   ╚═╝    ╚═════╝ ╚═╝  ╚═╝   ╚═╝   ╚══════╝╚══════╝   ╚═╝   ╚══════╝╚═╝  ╚═╝   ╚═╝╚═╝╚═╝  ╚═╝
https://patorjk.com/software/taag/#p=display&f=ANSI%20Shadow
*/

#ifndef TURTLE_TEXT_H
#define TURTLE_TEXT_H

/* enabling this option will render the string "pppp" at a higher y than "PPPP" such that the center of their Y values will match */
// #define TURTLE_TEXT_DO_DYNAMIC_Y_CENTERING

/* enabling this option will make text rendering faster by culling the circles on ends of text - this might make the text look pretty bad */
// #define TURTLE_TEXT_FAST_PEN

/* enabling this option will add extra circles significantly decreasing the speed but increasing the render quality */
// #define TURTLE_TEXT_PRETTY_PEN

#include <stdarg.h>

/* turtleText variables */
typedef struct {
    int32_t bezierPrez; // precision for bezier curves
    int32_t bezierPrezCurrent;
    int32_t charCount; // number of supported characters
    uint32_t *supportedCharReference; // array containing links from (int) unicode values of characters to an index from 0 to (charCount - 1)
    int32_t *fontPointer; // array containing links from char indices (0 to (charCount - 1)) to their corresponding data position in fontData
    int32_t *fontData; // array containing packaged instructions on how to draw each character in the character set
    /*
    Format:
    maximum x coordinate
    maximum y coordinate
    minimum x coordinate
    minimum y coordinate
    outer loops {
        inner loops {
            coordinates
            ...
        }
        inner loops {
            coordinates
            ...
        }
    }
    */
} turtleText_t;

extern turtleText_t turtleText;

/* initialise values, must supply a font file (tgl) */
int32_t turtleTextInit(const char *filename);

/* render functions */

/* renders a quadratic bezier curve on the screen */
void renderBezier(double x1, double y1, double x2, double y2, double x3, double y3, int32_t prez);

/* renders a single character */
void renderChar(int32_t index, double x, double y, double size);

/* gets the length of a string in pixels on the screen */
double turtleTextGetLength(const uint32_t *text, int32_t textLength, double size);

/* gets the length of a string in pixels on the screen */
double turtleTextGetStringLength(const char *str, double size);

/* gets the length of a string in pixels on the screen */
double turtleTextGetStringLengthf(double size, const char *str, ...);

/* gets the length of a u-string in pixels on the screen */
double turtleTextGetUnicodeLength(const unsigned char *str, double size);

/* writes to the screen */
void turtleTextWrite(const uint32_t *text, int32_t textLength, double x, double y, double size, double align);

/* wrapper function for writing strings easier */
void turtleTextWriteString(const char *str, double x, double y, double size, double align);

/* formatted string */
void turtleTextWriteStringf(double x, double y, double size, double align, const char *str, ...);

/* wrapper function for unicode strings (UTF-8, u8"Hello World") */
void turtleTextWriteUnicode(const unsigned char *str, double x, double y, double size, double align);

/* formatted utf-8 string */
void turtleTextWriteUnicodef(double x, double y, double size, double align, const unsigned char *str, ...);

int32_t turtleTextConvertUnicode(const unsigned char *str, uint32_t *converted);

/* if the font file is not found, use the default font (kept here) */
void generateDefaultFont(list_t *generatedFont);

#endif /* TURTLE_TEXT_H */

/*
████████╗██╗   ██╗██████╗ ████████╗██╗     ███████╗████████╗ ██████╗  ██████╗ ██╗     ███████╗   ██╗  ██╗
╚══██╔══╝██║   ██║██╔══██╗╚══██╔══╝██║     ██╔════╝╚══██╔══╝██╔═══██╗██╔═══██╗██║     ██╔════╝   ██║  ██║
   ██║   ██║   ██║██████╔╝   ██║   ██║     █████╗     ██║   ██║   ██║██║   ██║██║     ███████╗   ███████║
   ██║   ██║   ██║██╔══██╗   ██║   ██║     ██╔══╝     ██║   ██║   ██║██║   ██║██║     ╚════██║   ██╔══██║
   ██║   ╚██████╔╝██║  ██║   ██║   ███████╗███████╗   ██║   ╚██████╔╝╚██████╔╝███████╗███████║██╗██║  ██║
   ╚═╝    ╚═════╝ ╚═╝  ╚═╝   ╚═╝   ╚══════╝╚══════╝   ╚═╝    ╚═════╝  ╚═════╝ ╚══════╝╚══════╝╚═╝╚═╝  ╚═╝
https://patorjk.com/software/taag/#p=display&f=ANSI%20Shadow
*/

#ifndef TURTLE_TOOLS_H
#define TURTLE_TOOLS_H


/* random integer between lower and upper bound (inclusive) */
int randomInt(int lowerBound, int upperBound);

/* random double between lower and upper bound */
double randomDouble(double lowerBound, double upperBound);

/* insert a string to an index in to string */
char *strins(char *dest, char *source, int32_t index);

/* delete a section of a string */
char *strdel(char *dest, int32_t index, int32_t size);

typedef enum {
    TT_THEME_LIGHT = 0,
    TT_THEME_DARK = 1,
    TT_THEME_COLT = 2,
    TT_THEME_NAVY = 3,
} tt_theme_name_t;

extern tt_theme_name_t tt_theme;

typedef struct {
    int8_t turtleToolsEnabled;
    int8_t ribbonEnabled;
    int8_t popupEnabled;
    int8_t buttonEnabled;
    int8_t switchEnabled;
    int8_t dialEnabled;
    int8_t sliderEnabled;
    int8_t scrollbarEnabled;
    int8_t dropdownEnabled;
    int8_t textboxEnabled;
} tt_enabled_t;

extern tt_enabled_t tt_enabled; // all start at 0 (global variable)

typedef enum {
    TT_ELEMENT_BUTTON = 0,
    TT_ELEMENT_SWITCH = 1,
    TT_ELEMENT_DIAL = 2,
    TT_ELEMENT_SLIDER = 3,
    TT_ELEMENT_SCROLLBAR = 4,
    TT_ELEMENT_DROPDOWN = 5,
    TT_ELEMENT_TEXTBOX = 6,
    TT_NUMBER_OF_ELEMENTS = 7,
} tt_element_names_t;

typedef struct {
    list_t *all;
    list_t *buttons;
    list_t *switches;
    list_t *dials;
    list_t *sliders;
    list_t *scrollbars;
    list_t *dropdowns;
    list_t *textboxes;
} tt_elements_t;

extern tt_elements_t tt_elements;

typedef enum {
    TT_COLOR_TEXT = 0,
    TT_COLOR_TEXT_ALTERNATE = 3,
    TT_COLOR_RIBBON_TOP = 6,
    TT_COLOR_RIBBON_DROPDOWN = 9,
    TT_COLOR_RIBBON_SELECT = 12,
    TT_COLOR_RIBBON_HOVER = 15,
    TT_COLOR_POPUP_BOX = 18,
    TT_COLOR_POPUP_BUTTON = 21,
    TT_COLOR_POPUP_BUTTON_SELECT = 24,
    TT_COLOR_BUTTON = 27,
    TT_COLOR_BUTTON_SELECT = 30,
    TT_COLOR_BUTTON_CLICKED = 33,
    TT_COLOR_SWITCH_TEXT_HOVER = 36,
    TT_COLOR_SWITCH_OFF = 39,
    TT_COLOR_SWITCH_CIRCLE_OFF = 42,
    TT_COLOR_SWITCH_ON = 45,
    TT_COLOR_SWITCH_CIRCLE_ON = 48,
    TT_COLOR_DIAL = 51,
    TT_COLOR_DIAL_INNER = 54,
    TT_COLOR_SLIDER_BAR = 57,
    TT_COLOR_SLIDER_CIRCLE = 60,
    TT_COLOR_SCROLLBAR_BASE = 63,
    TT_COLOR_SCROLLBAR_BAR = 66,
    TT_COLOR_SCROLLBAR_HOVER = 69,
    TT_COLOR_SCROLLBAR_CLICKED = 72,
    TT_COLOR_DROPDOWN = 75,
    TT_COLOR_DROPDOWN_SELECT = 78,
    TT_COLOR_DROPDOWN_HOVER = 81,
    TT_COLOR_DROPDOWN_TRIANGLE = 84,
    TT_COLOR_TEXTBOX_BOX = 87,
    TT_COLOR_TEXTBOX_PHANTOM_TEXT = 90,
    TT_COLOR_TEXTBOX_LINE = 93,
    TT_COLOR_TEXTBOX_SELECT = 96,
} tt_theme_internal_t;

/* default colours (light theme) */
extern double tt_themeColors[];

void tt_setColor(int32_t index);

void turtleToolsSetTheme(tt_theme_name_t theme);

/* ribbon */

/* ribbon variables */
typedef struct {
    uint8_t marginSize;
    int8_t mainselect[4]; // 0 - select, 1 - mouseHover, 2 - selected, 3 - premove close dropdown
    int8_t subselect[4]; // 0 - select, 1 - mouseHover, 2 - selected, 3 - free
    int8_t output[3]; // 0 - toggle, 1 - mainselect, 2 - subselect
    int8_t mouseDown; // keeps track of previous frame mouse information
    int32_t bounds[4]; // list of coordinate bounds (minX, minY, maxX, maxY)
    double ribbonSize;
    list_t *options;
    list_t *lengths;
} tt_ribbon_t;

extern tt_ribbon_t ribbonRender;

/* initialise ribbon */
int32_t ribbonInit(const char *filename);

/* render ribbon */
void ribbonUpdate();

/* popup */

/* popup variables */
typedef struct {
    char *message; // message displayed on the popup
    double minX; // left edge of box
    double minY; // bottom of box
    double maxX; // right edge of box
    double maxY; // top of box
    list_t *options; // list of popup box options
    /*
    style:
    0 - default
    1 to 50 - rounded rectangle (not implemented)
    51 - triangle (wacky)
    52 - gaussian integral
    */
    int8_t style;
    int8_t output[2]; // [toggle, select]
    int8_t mouseDown;
} tt_popup_t;

extern tt_popup_t popup;

/* initialise popup */
int32_t popupInit(const char *filename, double minX, double minY, double maxX, double maxY);

/* render popup */
void popupUpdate();

void popupFree();

/* UI tools */

typedef struct {
    int32_t colorOverride;
    double color[24];
} tt_color_override_t;

typedef struct {
    double dialAnchorX;
    double dialAnchorY;
    double barAnchor;
    int32_t dropdownLogicIndex;
} tt_globals_t;

extern tt_globals_t tt_globals;

typedef enum {
    TT_ELEMENT_ENABLED = 0,
    TT_ELEMENT_NO_MOUSE = 1,
    TT_ELEMENT_HIDE = 2,
} tt_element_enabled_t;

typedef enum {
    TT_BUTTON_SHAPE_RECTANGLE = 0,
    TT_BUTTON_SHAPE_ROUNDED_RECTANGLE = 1,
    TT_BUTTON_SHAPE_CIRCLE = 2,
    TT_BUTTON_SHAPE_TEXT = 3,
} tt_button_shape_t;

#define TT_LABEL_LENGTH_LIMIT 24

/* button */
typedef struct {
    tt_element_names_t element;
    tt_element_enabled_t enabled;
    tt_color_override_t color;
    double x;
    double y;
    double size;
    int8_t *variable; // 1 if button is being pressed, 0 otherwise
    char label[TT_LABEL_LENGTH_LIMIT];
    int32_t status;
    tt_button_shape_t shape;
} tt_button_t;

typedef enum {
    TT_SWITCH_STYLE_CLASSIC = 0,
    TT_SWITCH_STYLE_SIDESWIPE_LEFT = 1,
    TT_SWITCH_STYLE_SIDESWIPE_RIGHT = 2,
    TT_SWITCH_STYLE_CHECKBOX = 3,
    TT_SWITCH_STYLE_XBOX = 4,
} tt_switch_style_t;

/* switch */
typedef struct {
    tt_element_names_t element;
    tt_element_enabled_t enabled;
    tt_color_override_t color;
    double x;
    double y;
    double size;
    int8_t *variable; // 1 if switch is flipped, 0 otherwise
    char label[TT_LABEL_LENGTH_LIMIT];
    int32_t status;
    tt_switch_style_t style;
} tt_switch_t;

typedef enum {
    TT_DIAL_LINEAR = 0,
    TT_DIAL_LOG = 1,
    TT_DIAL_EXP = 2,
} tt_dial_type_t;

/* dial */
typedef struct {
    tt_element_names_t element;
    tt_element_enabled_t enabled;
    tt_color_override_t color;
    double x;
    double y;
    double size;
    double *variable; // value of dial
    char label[TT_LABEL_LENGTH_LIMIT];
    int32_t status[2];
    tt_dial_type_t type;
    double range[2];
    double renderNumberFactor; // multiply rendered variable by this amount
    double defaultValue;
} tt_dial_t;

typedef enum {
    TT_SLIDER_HORIZONTAL = 0,
    TT_SLIDER_VERTICAL = 1,
} tt_slider_type_t;

typedef enum {
    TT_SLIDER_ALIGN_LEFT = 0,
    TT_SLIDER_ALIGN_CENTER = 1,
    TT_SLIDER_ALIGN_RIGHT = 2,
} tt_slider_align_t;

/* slider */
typedef struct {
    tt_element_names_t element;
    tt_element_enabled_t enabled;
    tt_color_override_t color;
    double x;
    double y;
    double size;
    double *variable; // value of slider
    char label[TT_LABEL_LENGTH_LIMIT];
    int32_t status;
    tt_slider_type_t type;
    tt_slider_align_t align;
    double length;
    double range[2];
    double renderNumberFactor; // multiply rendered variable by this amount
    double defaultValue;
} tt_slider_t;

typedef enum {
    TT_SCROLLBAR_HORIZONTAL = 0,
    TT_SCROLLBAR_VERTICAL = 1,
} tt_scrollbar_type_t;

/* scrollbar */
typedef struct {
    tt_element_names_t element;
    tt_element_enabled_t enabled;
    tt_color_override_t color;
    double x;
    double y;
    double size;
    double *variable; // value of slider
    int32_t status;
    tt_scrollbar_type_t type;
    double length;
    double barPercentage; // percentage of scrollbar occupied by bar
} tt_scrollbar_t;

typedef enum {
    TT_DROPDOWN_ALIGN_LEFT = 0,
    TT_DROPDOWN_ALIGN_CENTER = 1,
    TT_DROPDOWN_ALIGN_RIGHT = 2,
} tt_dropdown_align_t;

/* dropdown */
typedef struct {
    tt_element_names_t element;
    tt_element_enabled_t enabled;
    tt_color_override_t color;
    double x;
    double y;
    double size;
    int32_t *variable; // index of dropdown selected
    char label[TT_LABEL_LENGTH_LIMIT];
    list_t *options;
    uint32_t index;
    int32_t status;
    tt_dropdown_align_t align;
    double maxXfactor;
} tt_dropdown_t;

typedef enum {
    TT_TEXTBOX_ALIGN_LEFT = 0,
    TT_TEXTBOX_ALIGN_CENTER = 1,
    TT_TEXTBOX_ALIGN_RIGHT = 2,
} tt_textbox_align_t;

typedef struct {
    tt_element_names_t element;
    tt_element_enabled_t enabled;
    tt_color_override_t color;
    double x;
    double y;
    double size;
    char *text;
    char label[TT_LABEL_LENGTH_LIMIT];
    int32_t status;
    tt_textbox_align_t align;
    double length;
    int32_t maxCharacters;
    int32_t editIndex;
    int32_t lastKey;
    int32_t keyTimeout;
    /* render variables */
    double renderPixelOffset;
    int32_t renderStartingIndex;
    int32_t renderNumCharacters;
} tt_textbox_t;

/* override colors with color array */
void tt_colorOverride(void *element, double *colors, uint32_t length);

typedef enum {
    TT_COLOR_OVERRIDE_SLOT_0 = 0,
    TT_COLOR_OVERRIDE_SLOT_1 = 3,
    TT_COLOR_OVERRIDE_SLOT_2 = 6,
    TT_COLOR_OVERRIDE_SLOT_3 = 9,
    TT_COLOR_OVERRIDE_SLOT_4 = 12,
    TT_COLOR_OVERRIDE_SLOT_5 = 15,
    TT_COLOR_OVERRIDE_SLOT_6 = 18,
    TT_COLOR_OVERRIDE_SLOT_7 = 21,
} tt_color_override_internal_t;

extern int32_t tt_color_override_default[];

void elementResetColor(void *elementp, int32_t elementType);

void tt_internalColor(void *elementp, int32_t colorIndex, int32_t overrideIndex);

/* initialise UI elements */

/* create a button */
tt_button_t *buttonInit(char *label, int8_t *variable, double x, double y, double size);

void buttonFree(tt_button_t *buttonp);

/* create a switch */
tt_switch_t *switchInit(char *label, int8_t *variable, double x, double y, double size);

/* create a dial - make renderNumberFactor 0 to hide dial number */
tt_dial_t *dialInit(char *label, double *variable, tt_dial_type_t type, double x, double y, double size, double bottom, double top, double renderNumberFactor);

void dialFree(tt_dial_t *dialp);

/* create a slider - make renderNumberFactor 0 to hide slider number */
tt_slider_t *sliderInit(char *label, double *variable, tt_slider_type_t type, tt_slider_align_t align, double x, double y, double size, double length, double bottom, double top, double renderNumberFactor);

void sliderFree(tt_slider_t *sliderp);

/* create a scrollbar */
tt_scrollbar_t *scrollbarInit(double *variable, tt_scrollbar_type_t type, double x, double y, double size, double length, double barPercentage);

void scrollbarFree(tt_scrollbar_t *scrollbarp);

void dropdownCalculateMax(tt_dropdown_t *dropdown);

/* create a dropdown - use a list of strings for options */
tt_dropdown_t *dropdownInit(char *label, list_t *options, int32_t *variable, tt_dropdown_align_t align, double x, double y, double size);

void dropdownFree(tt_dropdown_t *dropdownp);

/* create a textbox */
tt_textbox_t *textboxInit(char *label, uint32_t maxCharacters, double x, double y, double size, double length);

void textboxFree(tt_textbox_t *textboxp);

void buttonUpdate();

void switchUpdate();

/* angle between two coordinates (in degrees) */
double angleBetween(double x1, double y1, double x2, double y2);

void dialUpdate();

void sliderUpdate();

/*
scrollbar range of motion (coordinates):
scrollbar.length * (1 - scrollbar.barPercentage / 100)
tip: try to match the ratio of visible content to the scrollbar's barPercentage - if half of the content can be shown on one screen then make the barPercentage 50
*/
void scrollbarUpdate();

void dropdownUpdate();

void textboxAddKey(tt_textbox_t *textboxp, int32_t key);

void textboxUnicodeCallback(uint32_t codepoint);

void textboxHandleOtherKey(tt_textbox_t *textboxp, int32_t key);

void textboxKeyCallback(int32_t key, int32_t scancode, int32_t action);

int32_t textboxCalculateMaximumCharacters(uint32_t *charlist, int32_t textLength, double size, double lengthPixels, int8_t sweepDirection, double *outputLength);

void textboxUpdate();

void turtleToolsUpdate();

void turtleToolsUpdateUI();

void turtleToolsUpdateRibbonPopup();

#endif /* TURTLETOOLS_H */

/*
 ██████╗ ███████╗████████╗ ██████╗  ██████╗ ██╗     ███████╗   ██╗  ██╗
██╔═══██╗██╔════╝╚══██╔══╝██╔═══██╗██╔═══██╗██║     ██╔════╝   ██║  ██║
██║   ██║███████╗   ██║   ██║   ██║██║   ██║██║     ███████╗   ███████║
██║   ██║╚════██║   ██║   ██║   ██║██║   ██║██║     ╚════██║   ██╔══██║
╚██████╔╝███████║   ██║   ╚██████╔╝╚██████╔╝███████╗███████║██╗██║  ██║
 ╚═════╝ ╚══════╝   ╚═╝    ╚═════╝  ╚═════╝ ╚══════╝╚══════╝╚═╝╚═╝  ╚═╝
https://patorjk.com/software/taag/#p=display&f=ANSI%20Shadow
*/

#ifndef OS_TOOLS_H
#define OS_TOOLS_H


/* required forward declarations (for packaging) */
typedef struct GLFWcursor GLFWcursor;

typedef struct {
    GLFWwindow *osToolsWindow;
    GLFWcursor *standardCursors[6];
} osToolsGLFWObject;

typedef struct {
    const char *text; // clipboard text data (heap allocated)
} osToolsClipboardObject;

typedef struct {
    char executableFilepath[4096 + 1]; // filepath of executable
    char selectedFilename[4096 + 1]; // output filename - maximum filepath is 260 characters on windows and 4096 on linux
    char openOrSave; // 0 - open, 1 - save
    int32_t numExtensions; // number of extensions
    char **extensions; // array of allowed extensions (7 characters long max (cuz *.json;))
} osToolsFileDialogObject;

typedef struct {
    list_t *mappedFiles;
} osToolsMemmapObject;

/* global objects */
extern osToolsGLFWObject osToolsGLFW;
extern osToolsClipboardObject osToolsClipboard;
extern osToolsFileDialogObject osToolsFileDialog;
extern osToolsMemmapObject osToolsMemmap;

/* OS independent functions */
void osToolsIndependentInit(GLFWwindow *window);

/* returns clipboard text */
const char *osToolsClipboardGetText();

/* takes null terminated strings */
int32_t osToolsClipboardSetText(const char *input);

/*
GLFW_ARROW_CURSOR
GLFW_IBEAM_CURSOR
GLFW_CROSSHAIR_CURSOR
GLFW_HAND_CURSOR
GLFW_HRESIZE_CURSOR
GLFW_VRESIZE_CURSOR
*/
void osToolsSetCursor(uint32_t cursor);

void osToolsHideAndLockCursor();

void osToolsShowCursor();

typedef enum {
    OSTOOLS_CSV_ROW = 0,
    OSTOOLS_CSV_COLUMN = 1,
    OSTOOLS_CSV_FIELD_DOUBLE = 2,
    OSTOOLS_CSV_FIELD_INT = 3,
    OSTOOLS_CSV_FIELD_STRING = 4,
} osToolsCSV;

list_t *osToolsLoadInternal(char *filename, osToolsCSV rowOrColumn, char delimeter, osToolsCSV fieldType);

/* packages a CSV file into a list (headers are strings, all fields are doubles) - use OSTOOLS_CSV_ROW to put it in a list of lists where each list is a row of the CSV and use OSTOOLS_CSV_COLUMN to output a list of lists where each list is a column of the CSV */
list_t *osToolsLoadCSV(char *filename, osToolsCSV rowOrColumn);

/* packages a CSV file into a list (headers are strings, all fields are doubles) - use OSTOOLS_CSV_ROW to put it in a list of lists where each list is a row of the CSV and use OSTOOLS_CSV_COLUMN to output a list of lists where each list is a column of the CSV */
list_t *osToolsLoadCSVDouble(char *filename, osToolsCSV rowOrColumn);

/* packages a CSV file into a list (headers are strings, all fields are ints) - use OSTOOLS_CSV_ROW to put it in a list of lists where each list is a row of the CSV and use OSTOOLS_CSV_COLUMN to output a list of lists where each list is a column of the CSV */
list_t *osToolsLoadCSVInt(char *filename, osToolsCSV rowOrColumn);

/* packages a CSV file into a list (headers are strings, all fields are strings) - use OSTOOLS_CSV_ROW to put it in a list of lists where each list is a row of the CSV and use OSTOOLS_CSV_COLUMN to output a list of lists where each list is a column of the CSV */
list_t *osToolsLoadCSVString(char *filename, osToolsCSV rowOrColumn);

#ifdef OS_WINDOWS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <shobjidl.h>

typedef struct {
    char name[32];
    HANDLE comHandle;
} win32ComPortObject;

extern win32ComPortObject win32com;

/* opens a com port */
int win32comInit(win32ComPortObject *com, char *name);

/* returns number of bytes sent */
int win32comSend(win32ComPortObject *com, unsigned char *data, int length);

/* returns number of bytes received */
int win32comReceive(win32ComPortObject *com, unsigned char *buffer, int length);

/* closes a com port */
int win32comClose(win32ComPortObject *com);

#define WIN32TCP_NUM_SOCKETS 32

typedef struct {
    char *address;
    char *port;
    SOCKET connectSocket[WIN32TCP_NUM_SOCKETS];
    char socketOpen[WIN32TCP_NUM_SOCKETS];
} win32SocketObject;

extern win32SocketObject win32Socket;

int win32tcpInit(char *address, char *port);

SOCKET *win32tcpCreateSocket();

int win32tcpSend(SOCKET *socket, unsigned char *data, int length);

int win32tcpReceive(SOCKET *socket, unsigned char *buffer, int length);

int win32tcpReceive2(SOCKET *socket, unsigned char *buffer, int length);

void win32tcpDeinit();
#endif

#ifdef OS_LINUX
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#endif

int32_t osToolsInit(char argv0[], GLFWwindow *window);

void osToolsFileDialogAddExtension(char *extension);

int32_t osToolsFileDialogPrompt(char openOrSave, char *prename);

uint8_t *osToolsMapFile(char *filename, uint32_t *sizeOutput);

int32_t osToolsUnmapFile(uint8_t *data);

#endif /* OS_TOOLS_H */

#ifndef __khrplatform_h_
#define __khrplatform_h_

/*
** Copyright (c) 2008-2018 The Khronos Group Inc.
**
** Permission is hereby granted, free of charge, to any person obtaining a
** copy of this software and/or associated documentation files (the
** "Materials"), to deal in the Materials without restriction, including
** without limitation the rights to use, copy, modify, merge, publish,
** distribute, sublicense, and/or sell copies of the Materials, and to
** permit persons to whom the Materials are furnished to do so, subject to
** the following conditions:
**
** The above copyright notice and this permission notice shall be included
** in all copies or substantial portions of the Materials.
**
** THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
** IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
** CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
** TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
** MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.
*/

/* Khronos platform-specific types and definitions.
 *
 * The master copy of khrplatform.h is maintained in the Khronos EGL
 * Registry repository at https://github.com/KhronosGroup/EGL-Registry
 * The last semantic modification to khrplatform.h was at commit ID:
 *      67a3e0864c2d75ea5287b9f3d2eb74a745936692
 *
 * Adopters may modify this file to suit their platform. Adopters are
 * encouraged to submit platform specific modifications to the Khronos
 * group so that they can be included in future versions of this file.
 * Please submit changes by filing pull requests or issues on
 * the EGL Registry repository linked above.
 *
 *
 * See the Implementer's Guidelines for information about where this file
 * should be located on your system and for more details of its use:
 *    http://www.khronos.org/registry/implementers_guide.pdf
 *
 * This file should be included as
 *        #include <KHR/khrplatform.h>
 * by Khronos client API header files that use its types and defines.
 *
 * The types in khrplatform.h should only be used to define API-specific types.
 *
 * Types defined in khrplatform.h:
 *    khronos_int8_t              signed   8  bit
 *    khronos_uint8_t             unsigned 8  bit
 *    khronos_int16_t             signed   16 bit
 *    khronos_uint16_t            unsigned 16 bit
 *    khronos_int32_t             signed   32 bit
 *    khronos_uint32_t            unsigned 32 bit
 *    khronos_int64_t             signed   64 bit
 *    khronos_uint64_t            unsigned 64 bit
 *    khronos_intptr_t            signed   same number of bits as a pointer
 *    khronos_uintptr_t           unsigned same number of bits as a pointer
 *    khronos_ssize_t             signed   size
 *    khronos_usize_t             unsigned size
 *    khronos_float_t             signed   32 bit floating point
 *    khronos_time_ns_t           unsigned 64 bit time in nanoseconds
 *    khronos_utime_nanoseconds_t unsigned time interval or absolute time in
 *                                         nanoseconds
 *    khronos_stime_nanoseconds_t signed time interval in nanoseconds
 *    khronos_boolean_enum_t      enumerated boolean type. This should
 *      only be used as a base type when a client API's boolean type is
 *      an enum. Client APIs which use an integer or other type for
 *      booleans cannot use this as the base type for their boolean.
 *
 * Tokens defined in khrplatform.h:
 *
 *    KHRONOS_FALSE, KHRONOS_TRUE Enumerated boolean false/true values.
 *
 *    KHRONOS_SUPPORT_INT64 is 1 if 64 bit integers are supported; otherwise 0.
 *    KHRONOS_SUPPORT_FLOAT is 1 if floats are supported; otherwise 0.
 *
 * Calling convention macros defined in this file:
 *    KHRONOS_APICALL
 *    KHRONOS_APIENTRY
 *    KHRONOS_APIATTRIBUTES
 *
 * These may be used in function prototypes as:
 *
 *      KHRONOS_APICALL void KHRONOS_APIENTRY funcname(
 *                                  int arg1,
 *                                  int arg2) KHRONOS_APIATTRIBUTES;
 */

#if defined(__SCITECH_SNAP__) && !defined(KHRONOS_STATIC)
#   define KHRONOS_STATIC 1
#endif

/*-------------------------------------------------------------------------
 * Definition of KHRONOS_APICALL
 *-------------------------------------------------------------------------
 * This precedes the return type of the function in the function prototype.
 */
#if defined(KHRONOS_STATIC)
    /* If the preprocessor constant KHRONOS_STATIC is defined, make the
     * header compatible with static linking. */
#   define KHRONOS_APICALL
#elif defined(_WIN32)
#   define KHRONOS_APICALL __declspec(dllimport)
#elif defined (__SYMBIAN32__)
#   define KHRONOS_APICALL IMPORT_C
#elif defined(__ANDROID__)
#   define KHRONOS_APICALL __attribute__((visibility("default")))
#else
#   define KHRONOS_APICALL
#endif

/*-------------------------------------------------------------------------
 * Definition of KHRONOS_APIENTRY
 *-------------------------------------------------------------------------
 * This follows the return type of the function  and precedes the function
 * name in the function prototype.
 */
#if defined(_WIN32) && !defined(_WIN32_WCE) && !defined(__SCITECH_SNAP__)
    /* Win32 but not WinCE */
#   define KHRONOS_APIENTRY __stdcall
#else
#   define KHRONOS_APIENTRY
#endif

/*-------------------------------------------------------------------------
 * Definition of KHRONOS_APIATTRIBUTES
 *-------------------------------------------------------------------------
 * This follows the closing parenthesis of the function prototype arguments.
 */
#if defined (__ARMCC_2__)
#define KHRONOS_APIATTRIBUTES __softfp
#else
#define KHRONOS_APIATTRIBUTES
#endif

/*-------------------------------------------------------------------------
 * basic type definitions
 *-----------------------------------------------------------------------*/
#if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L) || defined(__GNUC__) || defined(__SCO__) || defined(__USLC__)


/*
 * Using <stdint.h>
 */
#include <stdint.h>
typedef int32_t                 khronos_int32_t;
typedef uint32_t                khronos_uint32_t;
typedef int64_t                 khronos_int64_t;
typedef uint64_t                khronos_uint64_t;
#define KHRONOS_SUPPORT_INT64   1
#define KHRONOS_SUPPORT_FLOAT   1
/*
 * To support platform where unsigned long cannot be used interchangeably with
 * inptr_t (e.g. CHERI-extended ISAs), we can use the stdint.h intptr_t.
 * Ideally, we could just use (u)intptr_t everywhere, but this could result in
 * ABI breakage if khronos_uintptr_t is changed from unsigned long to
 * unsigned long long or similar (this results in different C++ name mangling).
 * To avoid changes for existing platforms, we restrict usage of intptr_t to
 * platforms where the size of a pointer is larger than the size of long.
 */
#if defined(__SIZEOF_LONG__) && defined(__SIZEOF_POINTER__)
#if __SIZEOF_POINTER__ > __SIZEOF_LONG__
#define KHRONOS_USE_INTPTR_T
#endif
#endif

#elif defined(__VMS ) || defined(__sgi)

/*
 * Using <inttypes.h>
 */
#include <inttypes.h>
typedef int32_t                 khronos_int32_t;
typedef uint32_t                khronos_uint32_t;
typedef int64_t                 khronos_int64_t;
typedef uint64_t                khronos_uint64_t;
#define KHRONOS_SUPPORT_INT64   1
#define KHRONOS_SUPPORT_FLOAT   1

#elif defined(_WIN32) && !defined(__SCITECH_SNAP__)

/*
 * Win32
 */
typedef __int32                 khronos_int32_t;
typedef unsigned __int32        khronos_uint32_t;
typedef __int64                 khronos_int64_t;
typedef unsigned __int64        khronos_uint64_t;
#define KHRONOS_SUPPORT_INT64   1
#define KHRONOS_SUPPORT_FLOAT   1

#elif defined(__sun__) || defined(__digital__)

/*
 * Sun or Digital
 */
typedef int                     khronos_int32_t;
typedef unsigned int            khronos_uint32_t;
#if defined(__arch64__) || defined(_LP64)
typedef long int                khronos_int64_t;
typedef unsigned long int       khronos_uint64_t;
#else
typedef long long int           khronos_int64_t;
typedef unsigned long long int  khronos_uint64_t;
#endif /* __arch64__ */
#define KHRONOS_SUPPORT_INT64   1
#define KHRONOS_SUPPORT_FLOAT   1

#elif 0

/*
 * Hypothetical platform with no float or int64 support
 */
typedef int                     khronos_int32_t;
typedef unsigned int            khronos_uint32_t;
#define KHRONOS_SUPPORT_INT64   0
#define KHRONOS_SUPPORT_FLOAT   0

#else

/*
 * Generic fallback
 */
#include <stdint.h>
typedef int32_t                 khronos_int32_t;
typedef uint32_t                khronos_uint32_t;
typedef int64_t                 khronos_int64_t;
typedef uint64_t                khronos_uint64_t;
#define KHRONOS_SUPPORT_INT64   1
#define KHRONOS_SUPPORT_FLOAT   1

#endif


/*
 * Types that are (so far) the same on all platforms
 */
typedef signed   char          khronos_int8_t;
typedef unsigned char          khronos_uint8_t;
typedef signed   short int     khronos_int16_t;
typedef unsigned short int     khronos_uint16_t;

/*
 * Types that differ between LLP64 and LP64 architectures - in LLP64,
 * pointers are 64 bits, but 'long' is still 32 bits. Win64 appears
 * to be the only LLP64 architecture in current use.
 */
#ifdef KHRONOS_USE_INTPTR_T
typedef intptr_t               khronos_intptr_t;
typedef uintptr_t              khronos_uintptr_t;
#elif defined(_WIN64)
typedef signed   long long int khronos_intptr_t;
typedef unsigned long long int khronos_uintptr_t;
#else
typedef signed   long  int     khronos_intptr_t;
typedef unsigned long  int     khronos_uintptr_t;
#endif

#if defined(_WIN64)
typedef signed   long long int khronos_ssize_t;
typedef unsigned long long int khronos_usize_t;
#else
typedef signed   long  int     khronos_ssize_t;
typedef unsigned long  int     khronos_usize_t;
#endif

#if KHRONOS_SUPPORT_FLOAT
/*
 * Float type
 */
typedef          float         khronos_float_t;
#endif

#if KHRONOS_SUPPORT_INT64
/* Time types
 *
 * These types can be used to represent a time interval in nanoseconds or
 * an absolute Unadjusted System Time.  Unadjusted System Time is the number
 * of nanoseconds since some arbitrary system event (e.g. since the last
 * time the system booted).  The Unadjusted System Time is an unsigned
 * 64 bit value that wraps back to 0 every 584 years.  Time intervals
 * may be either signed or unsigned.
 */
typedef khronos_uint64_t       khronos_utime_nanoseconds_t;
typedef khronos_int64_t        khronos_stime_nanoseconds_t;
#endif

/*
 * Dummy value used to pad enum types to 32 bits.
 */
#ifndef KHRONOS_MAX_ENUM
#define KHRONOS_MAX_ENUM 0x7FFFFFFF
#endif

/*
 * Enumerated boolean type
 *
 * Values other than zero should be considered to be true.  Therefore
 * comparisons should not be made against KHRONOS_TRUE.
 */
typedef enum {
    KHRONOS_FALSE = 0,
    KHRONOS_TRUE  = 1,
    KHRONOS_BOOLEAN_ENUM_FORCE_SIZE = KHRONOS_MAX_ENUM
} khronos_boolean_enum_t;

#endif /* __khrplatform_h_ */

/*

    OpenGL loader generated by glad 0.1.36 on Mon May 22 03:56:55 2023.

    Language/Generator: C/C++
    Specification: gl
    APIs: gl=3.3
    Profile: core
    Extensions:
        
    Loader: True
    Local files: False
    Omit khrplatform: False
    Reproducible: False

    Commandline:
        --profile="core" --api="gl=3.3" --generator="c" --spec="gl" --extensions=""
    Online:
        https://glad.dav1d.de/#profile=core&language=c&specification=gl&loader=on&api=gl%3D3.3
*/


#ifndef __glad_h_
#define __glad_h_

#ifdef __gl_h_
#error OpenGL header already included, remove this include, glad already provides it
#endif
#define __gl_h_

#if defined(_WIN32) && !defined(APIENTRY) && !defined(__CYGWIN__) && !defined(__SCITECH_SNAP__)
#define APIENTRY __stdcall
#endif

#ifndef APIENTRY
#define APIENTRY
#endif
#ifndef APIENTRYP
#define APIENTRYP APIENTRY *
#endif

#ifndef GLAPIENTRY
#define GLAPIENTRY APIENTRY
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct gladGLversionStruct {
    int major;
    int minor;
};

typedef void* (* GLADloadproc)(const char *name);

#ifndef GLAPI
# if defined(GLAD_GLAPI_EXPORT)
#  if defined(_WIN32) || defined(__CYGWIN__)
#   if defined(GLAD_GLAPI_EXPORT_BUILD)
#    if defined(__GNUC__)
#     define GLAPI __attribute__ ((dllexport)) extern
#    else
#     define GLAPI __declspec(dllexport) extern
#    endif
#   else
#    if defined(__GNUC__)
#     define GLAPI __attribute__ ((dllimport)) extern
#    else
#     define GLAPI __declspec(dllimport) extern
#    endif
#   endif
#  elif defined(__GNUC__) && defined(GLAD_GLAPI_EXPORT_BUILD)
#   define GLAPI __attribute__ ((visibility ("default"))) extern
#  else
#   define GLAPI extern
#  endif
# else
#  define GLAPI extern
# endif
#endif

GLAPI struct gladGLversionStruct GLVersion;

GLAPI int gladLoadGL(void);

GLAPI int gladLoadGLLoader(GLADloadproc);

typedef unsigned int GLenum;
typedef unsigned char GLboolean;
typedef unsigned int GLbitfield;
typedef void GLvoid;
typedef khronos_int8_t GLbyte;
typedef khronos_uint8_t GLubyte;
typedef khronos_int16_t GLshort;
typedef khronos_uint16_t GLushort;
typedef int GLint;
typedef unsigned int GLuint;
typedef khronos_int32_t GLclampx;
typedef int GLsizei;
typedef khronos_float_t GLfloat;
typedef khronos_float_t GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef void *GLeglClientBufferEXT;
typedef void *GLeglImageOES;
typedef char GLchar;
typedef char GLcharARB;
#ifdef __APPLE__
typedef void *GLhandleARB;
#else
typedef unsigned int GLhandleARB;
#endif
typedef khronos_uint16_t GLhalf;
typedef khronos_uint16_t GLhalfARB;
typedef khronos_int32_t GLfixed;
typedef khronos_intptr_t GLintptr;
typedef khronos_intptr_t GLintptrARB;
typedef khronos_ssize_t GLsizeiptr;
typedef khronos_ssize_t GLsizeiptrARB;
typedef khronos_int64_t GLint64;
typedef khronos_int64_t GLint64EXT;
typedef khronos_uint64_t GLuint64;
typedef khronos_uint64_t GLuint64EXT;
typedef struct __GLsync *GLsync;
struct _cl_context;
struct _cl_event;
typedef void (APIENTRY *GLDEBUGPROC)(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam);
typedef void (APIENTRY *GLDEBUGPROCARB)(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam);
typedef void (APIENTRY *GLDEBUGPROCKHR)(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam);
typedef void (APIENTRY *GLDEBUGPROCAMD)(GLuint id,GLenum category,GLenum severity,GLsizei length,const GLchar *message,void *userParam);
typedef unsigned short GLhalfNV;
typedef GLintptr GLvdpauSurfaceNV;
typedef void (APIENTRY *GLVULKANPROCNV)(void);
#define GL_DEPTH_BUFFER_BIT 0x00000100
#define GL_STENCIL_BUFFER_BIT 0x00000400
#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_FALSE 0
#define GL_TRUE 1
#define GL_POINTS 0x0000
#define GL_LINES 0x0001
#define GL_LINE_LOOP 0x0002
#define GL_LINE_STRIP 0x0003
#define GL_TRIANGLES 0x0004
#define GL_TRIANGLE_STRIP 0x0005
#define GL_TRIANGLE_FAN 0x0006
#define GL_NEVER 0x0200
#define GL_LESS 0x0201
#define GL_EQUAL 0x0202
#define GL_LEQUAL 0x0203
#define GL_GREATER 0x0204
#define GL_NOTEQUAL 0x0205
#define GL_GEQUAL 0x0206
#define GL_ALWAYS 0x0207
#define GL_ZERO 0
#define GL_ONE 1
#define GL_SRC_COLOR 0x0300
#define GL_ONE_MINUS_SRC_COLOR 0x0301
#define GL_SRC_ALPHA 0x0302
#define GL_ONE_MINUS_SRC_ALPHA 0x0303
#define GL_DST_ALPHA 0x0304
#define GL_ONE_MINUS_DST_ALPHA 0x0305
#define GL_DST_COLOR 0x0306
#define GL_ONE_MINUS_DST_COLOR 0x0307
#define GL_SRC_ALPHA_SATURATE 0x0308
#define GL_NONE 0
#define GL_FRONT_LEFT 0x0400
#define GL_FRONT_RIGHT 0x0401
#define GL_BACK_LEFT 0x0402
#define GL_BACK_RIGHT 0x0403
#define GL_FRONT 0x0404
#define GL_BACK 0x0405
#define GL_LEFT 0x0406
#define GL_RIGHT 0x0407
#define GL_FRONT_AND_BACK 0x0408
#define GL_NO_ERROR 0
#define GL_INVALID_ENUM 0x0500
#define GL_INVALID_VALUE 0x0501
#define GL_INVALID_OPERATION 0x0502
#define GL_OUT_OF_MEMORY 0x0505
#define GL_CW 0x0900
#define GL_CCW 0x0901
#define GL_POINT_SIZE 0x0B11
#define GL_POINT_SIZE_RANGE 0x0B12
#define GL_POINT_SIZE_GRANULARITY 0x0B13
#define GL_LINE_SMOOTH 0x0B20
#define GL_LINE_WIDTH 0x0B21
#define GL_LINE_WIDTH_RANGE 0x0B22
#define GL_LINE_WIDTH_GRANULARITY 0x0B23
#define GL_POLYGON_MODE 0x0B40
#define GL_POLYGON_SMOOTH 0x0B41
#define GL_CULL_FACE 0x0B44
#define GL_CULL_FACE_MODE 0x0B45
#define GL_FRONT_FACE 0x0B46
#define GL_DEPTH_RANGE 0x0B70
#define GL_DEPTH_TEST 0x0B71
#define GL_DEPTH_WRITEMASK 0x0B72
#define GL_DEPTH_CLEAR_VALUE 0x0B73
#define GL_DEPTH_FUNC 0x0B74
#define GL_STENCIL_TEST 0x0B90
#define GL_STENCIL_CLEAR_VALUE 0x0B91
#define GL_STENCIL_FUNC 0x0B92
#define GL_STENCIL_VALUE_MASK 0x0B93
#define GL_STENCIL_FAIL 0x0B94
#define GL_STENCIL_PASS_DEPTH_FAIL 0x0B95
#define GL_STENCIL_PASS_DEPTH_PASS 0x0B96
#define GL_STENCIL_REF 0x0B97
#define GL_STENCIL_WRITEMASK 0x0B98
#define GL_VIEWPORT 0x0BA2
#define GL_DITHER 0x0BD0
#define GL_BLEND_DST 0x0BE0
#define GL_BLEND_SRC 0x0BE1
#define GL_BLEND 0x0BE2
#define GL_LOGIC_OP_MODE 0x0BF0
#define GL_DRAW_BUFFER 0x0C01
#define GL_READ_BUFFER 0x0C02
#define GL_SCISSOR_BOX 0x0C10
#define GL_SCISSOR_TEST 0x0C11
#define GL_COLOR_CLEAR_VALUE 0x0C22
#define GL_COLOR_WRITEMASK 0x0C23
#define GL_DOUBLEBUFFER 0x0C32
#define GL_STEREO 0x0C33
#define GL_LINE_SMOOTH_HINT 0x0C52
#define GL_POLYGON_SMOOTH_HINT 0x0C53
#define GL_UNPACK_SWAP_BYTES 0x0CF0
#define GL_UNPACK_LSB_FIRST 0x0CF1
#define GL_UNPACK_ROW_LENGTH 0x0CF2
#define GL_UNPACK_SKIP_ROWS 0x0CF3
#define GL_UNPACK_SKIP_PIXELS 0x0CF4
#define GL_UNPACK_ALIGNMENT 0x0CF5
#define GL_PACK_SWAP_BYTES 0x0D00
#define GL_PACK_LSB_FIRST 0x0D01
#define GL_PACK_ROW_LENGTH 0x0D02
#define GL_PACK_SKIP_ROWS 0x0D03
#define GL_PACK_SKIP_PIXELS 0x0D04
#define GL_PACK_ALIGNMENT 0x0D05
#define GL_MAX_TEXTURE_SIZE 0x0D33
#define GL_MAX_VIEWPORT_DIMS 0x0D3A
#define GL_SUBPIXEL_BITS 0x0D50
#define GL_TEXTURE_1D 0x0DE0
#define GL_TEXTURE_2D 0x0DE1
#define GL_TEXTURE_WIDTH 0x1000
#define GL_TEXTURE_HEIGHT 0x1001
#define GL_TEXTURE_BORDER_COLOR 0x1004
#define GL_DONT_CARE 0x1100
#define GL_FASTEST 0x1101
#define GL_NICEST 0x1102
#define GL_BYTE 0x1400
#define GL_UNSIGNED_BYTE 0x1401
#define GL_SHORT 0x1402
#define GL_UNSIGNED_SHORT 0x1403
#define GL_INT 0x1404
#define GL_UNSIGNED_INT 0x1405
#define GL_FLOAT 0x1406
#define GL_CLEAR 0x1500
#define GL_AND 0x1501
#define GL_AND_REVERSE 0x1502
#define GL_COPY 0x1503
#define GL_AND_INVERTED 0x1504
#define GL_NOOP 0x1505
#define GL_XOR 0x1506
#define GL_OR 0x1507
#define GL_NOR 0x1508
#define GL_EQUIV 0x1509
#define GL_INVERT 0x150A
#define GL_OR_REVERSE 0x150B
#define GL_COPY_INVERTED 0x150C
#define GL_OR_INVERTED 0x150D
#define GL_NAND 0x150E
#define GL_SET 0x150F
#define GL_TEXTURE 0x1702
#define GL_COLOR 0x1800
#define GL_DEPTH 0x1801
#define GL_STENCIL 0x1802
#define GL_STENCIL_INDEX 0x1901
#define GL_DEPTH_COMPONENT 0x1902
#define GL_RED 0x1903
#define GL_GREEN 0x1904
#define GL_BLUE 0x1905
#define GL_ALPHA 0x1906
#define GL_RGB 0x1907
#define GL_RGBA 0x1908
#define GL_POINT 0x1B00
#define GL_LINE 0x1B01
#define GL_FILL 0x1B02
#define GL_KEEP 0x1E00
#define GL_REPLACE 0x1E01
#define GL_INCR 0x1E02
#define GL_DECR 0x1E03
#define GL_VENDOR 0x1F00
#define GL_RENDERER 0x1F01
#define GL_VERSION 0x1F02
#define GL_EXTENSIONS 0x1F03
#define GL_NEAREST 0x2600
#define GL_LINEAR 0x2601
#define GL_NEAREST_MIPMAP_NEAREST 0x2700
#define GL_LINEAR_MIPMAP_NEAREST 0x2701
#define GL_NEAREST_MIPMAP_LINEAR 0x2702
#define GL_LINEAR_MIPMAP_LINEAR 0x2703
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_WRAP_S 0x2802
#define GL_TEXTURE_WRAP_T 0x2803
#define GL_REPEAT 0x2901
#define GL_COLOR_LOGIC_OP 0x0BF2
#define GL_POLYGON_OFFSET_UNITS 0x2A00
#define GL_POLYGON_OFFSET_POINT 0x2A01
#define GL_POLYGON_OFFSET_LINE 0x2A02
#define GL_POLYGON_OFFSET_FILL 0x8037
#define GL_POLYGON_OFFSET_FACTOR 0x8038
#define GL_TEXTURE_BINDING_1D 0x8068
#define GL_TEXTURE_BINDING_2D 0x8069
#define GL_TEXTURE_INTERNAL_FORMAT 0x1003
#define GL_TEXTURE_RED_SIZE 0x805C
#define GL_TEXTURE_GREEN_SIZE 0x805D
#define GL_TEXTURE_BLUE_SIZE 0x805E
#define GL_TEXTURE_ALPHA_SIZE 0x805F
#define GL_DOUBLE 0x140A
#define GL_PROXY_TEXTURE_1D 0x8063
#define GL_PROXY_TEXTURE_2D 0x8064
#define GL_R3_G3_B2 0x2A10
#define GL_RGB4 0x804F
#define GL_RGB5 0x8050
#define GL_RGB8 0x8051
#define GL_RGB10 0x8052
#define GL_RGB12 0x8053
#define GL_RGB16 0x8054
#define GL_RGBA2 0x8055
#define GL_RGBA4 0x8056
#define GL_RGB5_A1 0x8057
#define GL_RGBA8 0x8058
#define GL_RGB10_A2 0x8059
#define GL_RGBA12 0x805A
#define GL_RGBA16 0x805B
#define GL_UNSIGNED_BYTE_3_3_2 0x8032
#define GL_UNSIGNED_SHORT_4_4_4_4 0x8033
#define GL_UNSIGNED_SHORT_5_5_5_1 0x8034
#define GL_UNSIGNED_INT_8_8_8_8 0x8035
#define GL_UNSIGNED_INT_10_10_10_2 0x8036
#define GL_TEXTURE_BINDING_3D 0x806A
#define GL_PACK_SKIP_IMAGES 0x806B
#define GL_PACK_IMAGE_HEIGHT 0x806C
#define GL_UNPACK_SKIP_IMAGES 0x806D
#define GL_UNPACK_IMAGE_HEIGHT 0x806E
#define GL_TEXTURE_3D 0x806F
#define GL_PROXY_TEXTURE_3D 0x8070
#define GL_TEXTURE_DEPTH 0x8071
#define GL_TEXTURE_WRAP_R 0x8072
#define GL_MAX_3D_TEXTURE_SIZE 0x8073
#define GL_UNSIGNED_BYTE_2_3_3_REV 0x8362
#define GL_UNSIGNED_SHORT_5_6_5 0x8363
#define GL_UNSIGNED_SHORT_5_6_5_REV 0x8364
#define GL_UNSIGNED_SHORT_4_4_4_4_REV 0x8365
#define GL_UNSIGNED_SHORT_1_5_5_5_REV 0x8366
#define GL_UNSIGNED_INT_8_8_8_8_REV 0x8367
#define GL_UNSIGNED_INT_2_10_10_10_REV 0x8368
#define GL_BGR 0x80E0
#define GL_BGRA 0x80E1
#define GL_MAX_ELEMENTS_VERTICES 0x80E8
#define GL_MAX_ELEMENTS_INDICES 0x80E9
#define GL_CLAMP_TO_EDGE 0x812F
#define GL_TEXTURE_MIN_LOD 0x813A
#define GL_TEXTURE_MAX_LOD 0x813B
#define GL_TEXTURE_BASE_LEVEL 0x813C
#define GL_TEXTURE_MAX_LEVEL 0x813D
#define GL_SMOOTH_POINT_SIZE_RANGE 0x0B12
#define GL_SMOOTH_POINT_SIZE_GRANULARITY 0x0B13
#define GL_SMOOTH_LINE_WIDTH_RANGE 0x0B22
#define GL_SMOOTH_LINE_WIDTH_GRANULARITY 0x0B23
#define GL_ALIASED_LINE_WIDTH_RANGE 0x846E
#define GL_TEXTURE0 0x84C0
#define GL_TEXTURE1 0x84C1
#define GL_TEXTURE2 0x84C2
#define GL_TEXTURE3 0x84C3
#define GL_TEXTURE4 0x84C4
#define GL_TEXTURE5 0x84C5
#define GL_TEXTURE6 0x84C6
#define GL_TEXTURE7 0x84C7
#define GL_TEXTURE8 0x84C8
#define GL_TEXTURE9 0x84C9
#define GL_TEXTURE10 0x84CA
#define GL_TEXTURE11 0x84CB
#define GL_TEXTURE12 0x84CC
#define GL_TEXTURE13 0x84CD
#define GL_TEXTURE14 0x84CE
#define GL_TEXTURE15 0x84CF
#define GL_TEXTURE16 0x84D0
#define GL_TEXTURE17 0x84D1
#define GL_TEXTURE18 0x84D2
#define GL_TEXTURE19 0x84D3
#define GL_TEXTURE20 0x84D4
#define GL_TEXTURE21 0x84D5
#define GL_TEXTURE22 0x84D6
#define GL_TEXTURE23 0x84D7
#define GL_TEXTURE24 0x84D8
#define GL_TEXTURE25 0x84D9
#define GL_TEXTURE26 0x84DA
#define GL_TEXTURE27 0x84DB
#define GL_TEXTURE28 0x84DC
#define GL_TEXTURE29 0x84DD
#define GL_TEXTURE30 0x84DE
#define GL_TEXTURE31 0x84DF
#define GL_ACTIVE_TEXTURE 0x84E0
#define GL_MULTISAMPLE 0x809D
#define GL_SAMPLE_ALPHA_TO_COVERAGE 0x809E
#define GL_SAMPLE_ALPHA_TO_ONE 0x809F
#define GL_SAMPLE_COVERAGE 0x80A0
#define GL_SAMPLE_BUFFERS 0x80A8
#define GL_SAMPLES 0x80A9
#define GL_SAMPLE_COVERAGE_VALUE 0x80AA
#define GL_SAMPLE_COVERAGE_INVERT 0x80AB
#define GL_TEXTURE_CUBE_MAP 0x8513
#define GL_TEXTURE_BINDING_CUBE_MAP 0x8514
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X 0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X 0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y 0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y 0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z 0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z 0x851A
#define GL_PROXY_TEXTURE_CUBE_MAP 0x851B
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE 0x851C
#define GL_COMPRESSED_RGB 0x84ED
#define GL_COMPRESSED_RGBA 0x84EE
#define GL_TEXTURE_COMPRESSION_HINT 0x84EF
#define GL_TEXTURE_COMPRESSED_IMAGE_SIZE 0x86A0
#define GL_TEXTURE_COMPRESSED 0x86A1
#define GL_NUM_COMPRESSED_TEXTURE_FORMATS 0x86A2
#define GL_COMPRESSED_TEXTURE_FORMATS 0x86A3
#define GL_CLAMP_TO_BORDER 0x812D
#define GL_BLEND_DST_RGB 0x80C8
#define GL_BLEND_SRC_RGB 0x80C9
#define GL_BLEND_DST_ALPHA 0x80CA
#define GL_BLEND_SRC_ALPHA 0x80CB
#define GL_POINT_FADE_THRESHOLD_SIZE 0x8128
#define GL_DEPTH_COMPONENT16 0x81A5
#define GL_DEPTH_COMPONENT24 0x81A6
#define GL_DEPTH_COMPONENT32 0x81A7
#define GL_MIRRORED_REPEAT 0x8370
#define GL_MAX_TEXTURE_LOD_BIAS 0x84FD
#define GL_TEXTURE_LOD_BIAS 0x8501
#define GL_INCR_WRAP 0x8507
#define GL_DECR_WRAP 0x8508
#define GL_TEXTURE_DEPTH_SIZE 0x884A
#define GL_TEXTURE_COMPARE_MODE 0x884C
#define GL_TEXTURE_COMPARE_FUNC 0x884D
#define GL_BLEND_COLOR 0x8005
#define GL_BLEND_EQUATION 0x8009
#define GL_CONSTANT_COLOR 0x8001
#define GL_ONE_MINUS_CONSTANT_COLOR 0x8002
#define GL_CONSTANT_ALPHA 0x8003
#define GL_ONE_MINUS_CONSTANT_ALPHA 0x8004
#define GL_FUNC_ADD 0x8006
#define GL_FUNC_REVERSE_SUBTRACT 0x800B
#define GL_FUNC_SUBTRACT 0x800A
#define GL_MIN 0x8007
#define GL_MAX 0x8008
#define GL_BUFFER_SIZE 0x8764
#define GL_BUFFER_USAGE 0x8765
#define GL_QUERY_COUNTER_BITS 0x8864
#define GL_CURRENT_QUERY 0x8865
#define GL_QUERY_RESULT 0x8866
#define GL_QUERY_RESULT_AVAILABLE 0x8867
#define GL_ARRAY_BUFFER 0x8892
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_ARRAY_BUFFER_BINDING 0x8894
#define GL_ELEMENT_ARRAY_BUFFER_BINDING 0x8895
#define GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING 0x889F
#define GL_READ_ONLY 0x88B8
#define GL_WRITE_ONLY 0x88B9
#define GL_READ_WRITE 0x88BA
#define GL_BUFFER_ACCESS 0x88BB
#define GL_BUFFER_MAPPED 0x88BC
#define GL_BUFFER_MAP_POINTER 0x88BD
#define GL_STREAM_DRAW 0x88E0
#define GL_STREAM_READ 0x88E1
#define GL_STREAM_COPY 0x88E2
#define GL_STATIC_DRAW 0x88E4
#define GL_STATIC_READ 0x88E5
#define GL_STATIC_COPY 0x88E6
#define GL_DYNAMIC_DRAW 0x88E8
#define GL_DYNAMIC_READ 0x88E9
#define GL_DYNAMIC_COPY 0x88EA
#define GL_SAMPLES_PASSED 0x8914
#define GL_SRC1_ALPHA 0x8589
#define GL_BLEND_EQUATION_RGB 0x8009
#define GL_VERTEX_ATTRIB_ARRAY_ENABLED 0x8622
#define GL_VERTEX_ATTRIB_ARRAY_SIZE 0x8623
#define GL_VERTEX_ATTRIB_ARRAY_STRIDE 0x8624
#define GL_VERTEX_ATTRIB_ARRAY_TYPE 0x8625
#define GL_CURRENT_VERTEX_ATTRIB 0x8626
#define GL_VERTEX_PROGRAM_POINT_SIZE 0x8642
#define GL_VERTEX_ATTRIB_ARRAY_POINTER 0x8645
#define GL_STENCIL_BACK_FUNC 0x8800
#define GL_STENCIL_BACK_FAIL 0x8801
#define GL_STENCIL_BACK_PASS_DEPTH_FAIL 0x8802
#define GL_STENCIL_BACK_PASS_DEPTH_PASS 0x8803
#define GL_MAX_DRAW_BUFFERS 0x8824
#define GL_DRAW_BUFFER0 0x8825
#define GL_DRAW_BUFFER1 0x8826
#define GL_DRAW_BUFFER2 0x8827
#define GL_DRAW_BUFFER3 0x8828
#define GL_DRAW_BUFFER4 0x8829
#define GL_DRAW_BUFFER5 0x882A
#define GL_DRAW_BUFFER6 0x882B
#define GL_DRAW_BUFFER7 0x882C
#define GL_DRAW_BUFFER8 0x882D
#define GL_DRAW_BUFFER9 0x882E
#define GL_DRAW_BUFFER10 0x882F
#define GL_DRAW_BUFFER11 0x8830
#define GL_DRAW_BUFFER12 0x8831
#define GL_DRAW_BUFFER13 0x8832
#define GL_DRAW_BUFFER14 0x8833
#define GL_DRAW_BUFFER15 0x8834
#define GL_BLEND_EQUATION_ALPHA 0x883D
#define GL_MAX_VERTEX_ATTRIBS 0x8869
#define GL_VERTEX_ATTRIB_ARRAY_NORMALIZED 0x886A
#define GL_MAX_TEXTURE_IMAGE_UNITS 0x8872
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER 0x8B31
#define GL_MAX_FRAGMENT_UNIFORM_COMPONENTS 0x8B49
#define GL_MAX_VERTEX_UNIFORM_COMPONENTS 0x8B4A
#define GL_MAX_VARYING_FLOATS 0x8B4B
#define GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS 0x8B4C
#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS 0x8B4D
#define GL_SHADER_TYPE 0x8B4F
#define GL_FLOAT_VEC2 0x8B50
#define GL_FLOAT_VEC3 0x8B51
#define GL_FLOAT_VEC4 0x8B52
#define GL_INT_VEC2 0x8B53
#define GL_INT_VEC3 0x8B54
#define GL_INT_VEC4 0x8B55
#define GL_BOOL 0x8B56
#define GL_BOOL_VEC2 0x8B57
#define GL_BOOL_VEC3 0x8B58
#define GL_BOOL_VEC4 0x8B59
#define GL_FLOAT_MAT2 0x8B5A
#define GL_FLOAT_MAT3 0x8B5B
#define GL_FLOAT_MAT4 0x8B5C
#define GL_SAMPLER_1D 0x8B5D
#define GL_SAMPLER_2D 0x8B5E
#define GL_SAMPLER_3D 0x8B5F
#define GL_SAMPLER_CUBE 0x8B60
#define GL_SAMPLER_1D_SHADOW 0x8B61
#define GL_SAMPLER_2D_SHADOW 0x8B62
#define GL_DELETE_STATUS 0x8B80
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82
#define GL_VALIDATE_STATUS 0x8B83
#define GL_INFO_LOG_LENGTH 0x8B84
#define GL_ATTACHED_SHADERS 0x8B85
#define GL_ACTIVE_UNIFORMS 0x8B86
#define GL_ACTIVE_UNIFORM_MAX_LENGTH 0x8B87
#define GL_SHADER_SOURCE_LENGTH 0x8B88
#define GL_ACTIVE_ATTRIBUTES 0x8B89
#define GL_ACTIVE_ATTRIBUTE_MAX_LENGTH 0x8B8A
#define GL_FRAGMENT_SHADER_DERIVATIVE_HINT 0x8B8B
#define GL_SHADING_LANGUAGE_VERSION 0x8B8C
#define GL_CURRENT_PROGRAM 0x8B8D
#define GL_POINT_SPRITE_COORD_ORIGIN 0x8CA0
#define GL_LOWER_LEFT 0x8CA1
#define GL_UPPER_LEFT 0x8CA2
#define GL_STENCIL_BACK_REF 0x8CA3
#define GL_STENCIL_BACK_VALUE_MASK 0x8CA4
#define GL_STENCIL_BACK_WRITEMASK 0x8CA5
#define GL_PIXEL_PACK_BUFFER 0x88EB
#define GL_PIXEL_UNPACK_BUFFER 0x88EC
#define GL_PIXEL_PACK_BUFFER_BINDING 0x88ED
#define GL_PIXEL_UNPACK_BUFFER_BINDING 0x88EF
#define GL_FLOAT_MAT2x3 0x8B65
#define GL_FLOAT_MAT2x4 0x8B66
#define GL_FLOAT_MAT3x2 0x8B67
#define GL_FLOAT_MAT3x4 0x8B68
#define GL_FLOAT_MAT4x2 0x8B69
#define GL_FLOAT_MAT4x3 0x8B6A
#define GL_SRGB 0x8C40
#define GL_SRGB8 0x8C41
#define GL_SRGB_ALPHA 0x8C42
#define GL_SRGB8_ALPHA8 0x8C43
#define GL_COMPRESSED_SRGB 0x8C48
#define GL_COMPRESSED_SRGB_ALPHA 0x8C49
#define GL_COMPARE_REF_TO_TEXTURE 0x884E
#define GL_CLIP_DISTANCE0 0x3000
#define GL_CLIP_DISTANCE1 0x3001
#define GL_CLIP_DISTANCE2 0x3002
#define GL_CLIP_DISTANCE3 0x3003
#define GL_CLIP_DISTANCE4 0x3004
#define GL_CLIP_DISTANCE5 0x3005
#define GL_CLIP_DISTANCE6 0x3006
#define GL_CLIP_DISTANCE7 0x3007
#define GL_MAX_CLIP_DISTANCES 0x0D32
#define GL_MAJOR_VERSION 0x821B
#define GL_MINOR_VERSION 0x821C
#define GL_NUM_EXTENSIONS 0x821D
#define GL_CONTEXT_FLAGS 0x821E
#define GL_COMPRESSED_RED 0x8225
#define GL_COMPRESSED_RG 0x8226
#define GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT 0x00000001
#define GL_RGBA32F 0x8814
#define GL_RGB32F 0x8815
#define GL_RGBA16F 0x881A
#define GL_RGB16F 0x881B
#define GL_VERTEX_ATTRIB_ARRAY_INTEGER 0x88FD
#define GL_MAX_ARRAY_TEXTURE_LAYERS 0x88FF
#define GL_MIN_PROGRAM_TEXEL_OFFSET 0x8904
#define GL_MAX_PROGRAM_TEXEL_OFFSET 0x8905
#define GL_CLAMP_READ_COLOR 0x891C
#define GL_FIXED_ONLY 0x891D
#define GL_MAX_VARYING_COMPONENTS 0x8B4B
#define GL_TEXTURE_1D_ARRAY 0x8C18
#define GL_PROXY_TEXTURE_1D_ARRAY 0x8C19
#define GL_TEXTURE_2D_ARRAY 0x8C1A
#define GL_PROXY_TEXTURE_2D_ARRAY 0x8C1B
#define GL_TEXTURE_BINDING_1D_ARRAY 0x8C1C
#define GL_TEXTURE_BINDING_2D_ARRAY 0x8C1D
#define GL_R11F_G11F_B10F 0x8C3A
#define GL_UNSIGNED_INT_10F_11F_11F_REV 0x8C3B
#define GL_RGB9_E5 0x8C3D
#define GL_UNSIGNED_INT_5_9_9_9_REV 0x8C3E
#define GL_TEXTURE_SHARED_SIZE 0x8C3F
#define GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH 0x8C76
#define GL_TRANSFORM_FEEDBACK_BUFFER_MODE 0x8C7F
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS 0x8C80
#define GL_TRANSFORM_FEEDBACK_VARYINGS 0x8C83
#define GL_TRANSFORM_FEEDBACK_BUFFER_START 0x8C84
#define GL_TRANSFORM_FEEDBACK_BUFFER_SIZE 0x8C85
#define GL_PRIMITIVES_GENERATED 0x8C87
#define GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN 0x8C88
#define GL_RASTERIZER_DISCARD 0x8C89
#define GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS 0x8C8A
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS 0x8C8B
#define GL_INTERLEAVED_ATTRIBS 0x8C8C
#define GL_SEPARATE_ATTRIBS 0x8C8D
#define GL_TRANSFORM_FEEDBACK_BUFFER 0x8C8E
#define GL_TRANSFORM_FEEDBACK_BUFFER_BINDING 0x8C8F
#define GL_RGBA32UI 0x8D70
#define GL_RGB32UI 0x8D71
#define GL_RGBA16UI 0x8D76
#define GL_RGB16UI 0x8D77
#define GL_RGBA8UI 0x8D7C
#define GL_RGB8UI 0x8D7D
#define GL_RGBA32I 0x8D82
#define GL_RGB32I 0x8D83
#define GL_RGBA16I 0x8D88
#define GL_RGB16I 0x8D89
#define GL_RGBA8I 0x8D8E
#define GL_RGB8I 0x8D8F
#define GL_RED_INTEGER 0x8D94
#define GL_GREEN_INTEGER 0x8D95
#define GL_BLUE_INTEGER 0x8D96
#define GL_RGB_INTEGER 0x8D98
#define GL_RGBA_INTEGER 0x8D99
#define GL_BGR_INTEGER 0x8D9A
#define GL_BGRA_INTEGER 0x8D9B
#define GL_SAMPLER_1D_ARRAY 0x8DC0
#define GL_SAMPLER_2D_ARRAY 0x8DC1
#define GL_SAMPLER_1D_ARRAY_SHADOW 0x8DC3
#define GL_SAMPLER_2D_ARRAY_SHADOW 0x8DC4
#define GL_SAMPLER_CUBE_SHADOW 0x8DC5
#define GL_UNSIGNED_INT_VEC2 0x8DC6
#define GL_UNSIGNED_INT_VEC3 0x8DC7
#define GL_UNSIGNED_INT_VEC4 0x8DC8
#define GL_INT_SAMPLER_1D 0x8DC9
#define GL_INT_SAMPLER_2D 0x8DCA
#define GL_INT_SAMPLER_3D 0x8DCB
#define GL_INT_SAMPLER_CUBE 0x8DCC
#define GL_INT_SAMPLER_1D_ARRAY 0x8DCE
#define GL_INT_SAMPLER_2D_ARRAY 0x8DCF
#define GL_UNSIGNED_INT_SAMPLER_1D 0x8DD1
#define GL_UNSIGNED_INT_SAMPLER_2D 0x8DD2
#define GL_UNSIGNED_INT_SAMPLER_3D 0x8DD3
#define GL_UNSIGNED_INT_SAMPLER_CUBE 0x8DD4
#define GL_UNSIGNED_INT_SAMPLER_1D_ARRAY 0x8DD6
#define GL_UNSIGNED_INT_SAMPLER_2D_ARRAY 0x8DD7
#define GL_QUERY_WAIT 0x8E13
#define GL_QUERY_NO_WAIT 0x8E14
#define GL_QUERY_BY_REGION_WAIT 0x8E15
#define GL_QUERY_BY_REGION_NO_WAIT 0x8E16
#define GL_BUFFER_ACCESS_FLAGS 0x911F
#define GL_BUFFER_MAP_LENGTH 0x9120
#define GL_BUFFER_MAP_OFFSET 0x9121
#define GL_DEPTH_COMPONENT32F 0x8CAC
#define GL_DEPTH32F_STENCIL8 0x8CAD
#define GL_FLOAT_32_UNSIGNED_INT_24_8_REV 0x8DAD
#define GL_INVALID_FRAMEBUFFER_OPERATION 0x0506
#define GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING 0x8210
#define GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE 0x8211
#define GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE 0x8212
#define GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE 0x8213
#define GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE 0x8214
#define GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE 0x8215
#define GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE 0x8216
#define GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE 0x8217
#define GL_FRAMEBUFFER_DEFAULT 0x8218
#define GL_FRAMEBUFFER_UNDEFINED 0x8219
#define GL_DEPTH_STENCIL_ATTACHMENT 0x821A
#define GL_MAX_RENDERBUFFER_SIZE 0x84E8
#define GL_DEPTH_STENCIL 0x84F9
#define GL_UNSIGNED_INT_24_8 0x84FA
#define GL_DEPTH24_STENCIL8 0x88F0
#define GL_TEXTURE_STENCIL_SIZE 0x88F1
#define GL_TEXTURE_RED_TYPE 0x8C10
#define GL_TEXTURE_GREEN_TYPE 0x8C11
#define GL_TEXTURE_BLUE_TYPE 0x8C12
#define GL_TEXTURE_ALPHA_TYPE 0x8C13
#define GL_TEXTURE_DEPTH_TYPE 0x8C16
#define GL_UNSIGNED_NORMALIZED 0x8C17
#define GL_FRAMEBUFFER_BINDING 0x8CA6
#define GL_DRAW_FRAMEBUFFER_BINDING 0x8CA6
#define GL_RENDERBUFFER_BINDING 0x8CA7
#define GL_READ_FRAMEBUFFER 0x8CA8
#define GL_DRAW_FRAMEBUFFER 0x8CA9
#define GL_READ_FRAMEBUFFER_BINDING 0x8CAA
#define GL_RENDERBUFFER_SAMPLES 0x8CAB
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE 0x8CD0
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME 0x8CD1
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL 0x8CD2
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE 0x8CD3
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER 0x8CD4
#define GL_FRAMEBUFFER_COMPLETE 0x8CD5
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT 0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT 0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER 0x8CDB
#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER 0x8CDC
#define GL_FRAMEBUFFER_UNSUPPORTED 0x8CDD
#define GL_MAX_COLOR_ATTACHMENTS 0x8CDF
#define GL_COLOR_ATTACHMENT0 0x8CE0
#define GL_COLOR_ATTACHMENT1 0x8CE1
#define GL_COLOR_ATTACHMENT2 0x8CE2
#define GL_COLOR_ATTACHMENT3 0x8CE3
#define GL_COLOR_ATTACHMENT4 0x8CE4
#define GL_COLOR_ATTACHMENT5 0x8CE5
#define GL_COLOR_ATTACHMENT6 0x8CE6
#define GL_COLOR_ATTACHMENT7 0x8CE7
#define GL_COLOR_ATTACHMENT8 0x8CE8
#define GL_COLOR_ATTACHMENT9 0x8CE9
#define GL_COLOR_ATTACHMENT10 0x8CEA
#define GL_COLOR_ATTACHMENT11 0x8CEB
#define GL_COLOR_ATTACHMENT12 0x8CEC
#define GL_COLOR_ATTACHMENT13 0x8CED
#define GL_COLOR_ATTACHMENT14 0x8CEE
#define GL_COLOR_ATTACHMENT15 0x8CEF
#define GL_COLOR_ATTACHMENT16 0x8CF0
#define GL_COLOR_ATTACHMENT17 0x8CF1
#define GL_COLOR_ATTACHMENT18 0x8CF2
#define GL_COLOR_ATTACHMENT19 0x8CF3
#define GL_COLOR_ATTACHMENT20 0x8CF4
#define GL_COLOR_ATTACHMENT21 0x8CF5
#define GL_COLOR_ATTACHMENT22 0x8CF6
#define GL_COLOR_ATTACHMENT23 0x8CF7
#define GL_COLOR_ATTACHMENT24 0x8CF8
#define GL_COLOR_ATTACHMENT25 0x8CF9
#define GL_COLOR_ATTACHMENT26 0x8CFA
#define GL_COLOR_ATTACHMENT27 0x8CFB
#define GL_COLOR_ATTACHMENT28 0x8CFC
#define GL_COLOR_ATTACHMENT29 0x8CFD
#define GL_COLOR_ATTACHMENT30 0x8CFE
#define GL_COLOR_ATTACHMENT31 0x8CFF
#define GL_DEPTH_ATTACHMENT 0x8D00
#define GL_STENCIL_ATTACHMENT 0x8D20
#define GL_FRAMEBUFFER 0x8D40
#define GL_RENDERBUFFER 0x8D41
#define GL_RENDERBUFFER_WIDTH 0x8D42
#define GL_RENDERBUFFER_HEIGHT 0x8D43
#define GL_RENDERBUFFER_INTERNAL_FORMAT 0x8D44
#define GL_STENCIL_INDEX1 0x8D46
#define GL_STENCIL_INDEX4 0x8D47
#define GL_STENCIL_INDEX8 0x8D48
#define GL_STENCIL_INDEX16 0x8D49
#define GL_RENDERBUFFER_RED_SIZE 0x8D50
#define GL_RENDERBUFFER_GREEN_SIZE 0x8D51
#define GL_RENDERBUFFER_BLUE_SIZE 0x8D52
#define GL_RENDERBUFFER_ALPHA_SIZE 0x8D53
#define GL_RENDERBUFFER_DEPTH_SIZE 0x8D54
#define GL_RENDERBUFFER_STENCIL_SIZE 0x8D55
#define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE 0x8D56
#define GL_MAX_SAMPLES 0x8D57
#define GL_FRAMEBUFFER_SRGB 0x8DB9
#define GL_HALF_FLOAT 0x140B
#define GL_MAP_READ_BIT 0x0001
#define GL_MAP_WRITE_BIT 0x0002
#define GL_MAP_INVALIDATE_RANGE_BIT 0x0004
#define GL_MAP_INVALIDATE_BUFFER_BIT 0x0008
#define GL_MAP_FLUSH_EXPLICIT_BIT 0x0010
#define GL_MAP_UNSYNCHRONIZED_BIT 0x0020
#define GL_COMPRESSED_RED_RGTC1 0x8DBB
#define GL_COMPRESSED_SIGNED_RED_RGTC1 0x8DBC
#define GL_COMPRESSED_RG_RGTC2 0x8DBD
#define GL_COMPRESSED_SIGNED_RG_RGTC2 0x8DBE
#define GL_RG 0x8227
#define GL_RG_INTEGER 0x8228
#define GL_R8 0x8229
#define GL_R16 0x822A
#define GL_RG8 0x822B
#define GL_RG16 0x822C
#define GL_R16F 0x822D
#define GL_R32F 0x822E
#define GL_RG16F 0x822F
#define GL_RG32F 0x8230
#define GL_R8I 0x8231
#define GL_R8UI 0x8232
#define GL_R16I 0x8233
#define GL_R16UI 0x8234
#define GL_R32I 0x8235
#define GL_R32UI 0x8236
#define GL_RG8I 0x8237
#define GL_RG8UI 0x8238
#define GL_RG16I 0x8239
#define GL_RG16UI 0x823A
#define GL_RG32I 0x823B
#define GL_RG32UI 0x823C
#define GL_VERTEX_ARRAY_BINDING 0x85B5
#define GL_SAMPLER_2D_RECT 0x8B63
#define GL_SAMPLER_2D_RECT_SHADOW 0x8B64
#define GL_SAMPLER_BUFFER 0x8DC2
#define GL_INT_SAMPLER_2D_RECT 0x8DCD
#define GL_INT_SAMPLER_BUFFER 0x8DD0
#define GL_UNSIGNED_INT_SAMPLER_2D_RECT 0x8DD5
#define GL_UNSIGNED_INT_SAMPLER_BUFFER 0x8DD8
#define GL_TEXTURE_BUFFER 0x8C2A
#define GL_MAX_TEXTURE_BUFFER_SIZE 0x8C2B
#define GL_TEXTURE_BINDING_BUFFER 0x8C2C
#define GL_TEXTURE_BUFFER_DATA_STORE_BINDING 0x8C2D
#define GL_TEXTURE_RECTANGLE 0x84F5
#define GL_TEXTURE_BINDING_RECTANGLE 0x84F6
#define GL_PROXY_TEXTURE_RECTANGLE 0x84F7
#define GL_MAX_RECTANGLE_TEXTURE_SIZE 0x84F8
#define GL_R8_SNORM 0x8F94
#define GL_RG8_SNORM 0x8F95
#define GL_RGB8_SNORM 0x8F96
#define GL_RGBA8_SNORM 0x8F97
#define GL_R16_SNORM 0x8F98
#define GL_RG16_SNORM 0x8F99
#define GL_RGB16_SNORM 0x8F9A
#define GL_RGBA16_SNORM 0x8F9B
#define GL_SIGNED_NORMALIZED 0x8F9C
#define GL_PRIMITIVE_RESTART 0x8F9D
#define GL_PRIMITIVE_RESTART_INDEX 0x8F9E
#define GL_COPY_READ_BUFFER 0x8F36
#define GL_COPY_WRITE_BUFFER 0x8F37
#define GL_UNIFORM_BUFFER 0x8A11
#define GL_UNIFORM_BUFFER_BINDING 0x8A28
#define GL_UNIFORM_BUFFER_START 0x8A29
#define GL_UNIFORM_BUFFER_SIZE 0x8A2A
#define GL_MAX_VERTEX_UNIFORM_BLOCKS 0x8A2B
#define GL_MAX_GEOMETRY_UNIFORM_BLOCKS 0x8A2C
#define GL_MAX_FRAGMENT_UNIFORM_BLOCKS 0x8A2D
#define GL_MAX_COMBINED_UNIFORM_BLOCKS 0x8A2E
#define GL_MAX_UNIFORM_BUFFER_BINDINGS 0x8A2F
#define GL_MAX_UNIFORM_BLOCK_SIZE 0x8A30
#define GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS 0x8A31
#define GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS 0x8A32
#define GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS 0x8A33
#define GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT 0x8A34
#define GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH 0x8A35
#define GL_ACTIVE_UNIFORM_BLOCKS 0x8A36
#define GL_UNIFORM_TYPE 0x8A37
#define GL_UNIFORM_SIZE 0x8A38
#define GL_UNIFORM_NAME_LENGTH 0x8A39
#define GL_UNIFORM_BLOCK_INDEX 0x8A3A
#define GL_UNIFORM_OFFSET 0x8A3B
#define GL_UNIFORM_ARRAY_STRIDE 0x8A3C
#define GL_UNIFORM_MATRIX_STRIDE 0x8A3D
#define GL_UNIFORM_IS_ROW_MAJOR 0x8A3E
#define GL_UNIFORM_BLOCK_BINDING 0x8A3F
#define GL_UNIFORM_BLOCK_DATA_SIZE 0x8A40
#define GL_UNIFORM_BLOCK_NAME_LENGTH 0x8A41
#define GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS 0x8A42
#define GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES 0x8A43
#define GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER 0x8A44
#define GL_UNIFORM_BLOCK_REFERENCED_BY_GEOMETRY_SHADER 0x8A45
#define GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER 0x8A46
#define GL_INVALID_INDEX 0xFFFFFFFF
#define GL_CONTEXT_CORE_PROFILE_BIT 0x00000001
#define GL_CONTEXT_COMPATIBILITY_PROFILE_BIT 0x00000002
#define GL_LINES_ADJACENCY 0x000A
#define GL_LINE_STRIP_ADJACENCY 0x000B
#define GL_TRIANGLES_ADJACENCY 0x000C
#define GL_TRIANGLE_STRIP_ADJACENCY 0x000D
#define GL_PROGRAM_POINT_SIZE 0x8642
#define GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS 0x8C29
#define GL_FRAMEBUFFER_ATTACHMENT_LAYERED 0x8DA7
#define GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS 0x8DA8
#define GL_GEOMETRY_SHADER 0x8DD9
#define GL_GEOMETRY_VERTICES_OUT 0x8916
#define GL_GEOMETRY_INPUT_TYPE 0x8917
#define GL_GEOMETRY_OUTPUT_TYPE 0x8918
#define GL_MAX_GEOMETRY_UNIFORM_COMPONENTS 0x8DDF
#define GL_MAX_GEOMETRY_OUTPUT_VERTICES 0x8DE0
#define GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS 0x8DE1
#define GL_MAX_VERTEX_OUTPUT_COMPONENTS 0x9122
#define GL_MAX_GEOMETRY_INPUT_COMPONENTS 0x9123
#define GL_MAX_GEOMETRY_OUTPUT_COMPONENTS 0x9124
#define GL_MAX_FRAGMENT_INPUT_COMPONENTS 0x9125
#define GL_CONTEXT_PROFILE_MASK 0x9126
#define GL_DEPTH_CLAMP 0x864F
#define GL_QUADS_FOLLOW_PROVOKING_VERTEX_CONVENTION 0x8E4C
#define GL_FIRST_VERTEX_CONVENTION 0x8E4D
#define GL_LAST_VERTEX_CONVENTION 0x8E4E
#define GL_PROVOKING_VERTEX 0x8E4F
#define GL_TEXTURE_CUBE_MAP_SEAMLESS 0x884F
#define GL_MAX_SERVER_WAIT_TIMEOUT 0x9111
#define GL_OBJECT_TYPE 0x9112
#define GL_SYNC_CONDITION 0x9113
#define GL_SYNC_STATUS 0x9114
#define GL_SYNC_FLAGS 0x9115
#define GL_SYNC_FENCE 0x9116
#define GL_SYNC_GPU_COMMANDS_COMPLETE 0x9117
#define GL_UNSIGNALED 0x9118
#define GL_SIGNALED 0x9119
#define GL_ALREADY_SIGNALED 0x911A
#define GL_TIMEOUT_EXPIRED 0x911B
#define GL_CONDITION_SATISFIED 0x911C
#define GL_WAIT_FAILED 0x911D
#define GL_TIMEOUT_IGNORED 0xFFFFFFFFFFFFFFFF
#define GL_SYNC_FLUSH_COMMANDS_BIT 0x00000001
#define GL_SAMPLE_POSITION 0x8E50
#define GL_SAMPLE_MASK 0x8E51
#define GL_SAMPLE_MASK_VALUE 0x8E52
#define GL_MAX_SAMPLE_MASK_WORDS 0x8E59
#define GL_TEXTURE_2D_MULTISAMPLE 0x9100
#define GL_PROXY_TEXTURE_2D_MULTISAMPLE 0x9101
#define GL_TEXTURE_2D_MULTISAMPLE_ARRAY 0x9102
#define GL_PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY 0x9103
#define GL_TEXTURE_BINDING_2D_MULTISAMPLE 0x9104
#define GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY 0x9105
#define GL_TEXTURE_SAMPLES 0x9106
#define GL_TEXTURE_FIXED_SAMPLE_LOCATIONS 0x9107
#define GL_SAMPLER_2D_MULTISAMPLE 0x9108
#define GL_INT_SAMPLER_2D_MULTISAMPLE 0x9109
#define GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE 0x910A
#define GL_SAMPLER_2D_MULTISAMPLE_ARRAY 0x910B
#define GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY 0x910C
#define GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY 0x910D
#define GL_MAX_COLOR_TEXTURE_SAMPLES 0x910E
#define GL_MAX_DEPTH_TEXTURE_SAMPLES 0x910F
#define GL_MAX_INTEGER_SAMPLES 0x9110
#define GL_VERTEX_ATTRIB_ARRAY_DIVISOR 0x88FE
#define GL_SRC1_COLOR 0x88F9
#define GL_ONE_MINUS_SRC1_COLOR 0x88FA
#define GL_ONE_MINUS_SRC1_ALPHA 0x88FB
#define GL_MAX_DUAL_SOURCE_DRAW_BUFFERS 0x88FC
#define GL_ANY_SAMPLES_PASSED 0x8C2F
#define GL_SAMPLER_BINDING 0x8919
#define GL_RGB10_A2UI 0x906F
#define GL_TEXTURE_SWIZZLE_R 0x8E42
#define GL_TEXTURE_SWIZZLE_G 0x8E43
#define GL_TEXTURE_SWIZZLE_B 0x8E44
#define GL_TEXTURE_SWIZZLE_A 0x8E45
#define GL_TEXTURE_SWIZZLE_RGBA 0x8E46
#define GL_TIME_ELAPSED 0x88BF
#define GL_TIMESTAMP 0x8E28
#define GL_INT_2_10_10_10_REV 0x8D9F
#ifndef GL_VERSION_1_0
#define GL_VERSION_1_0 1
GLAPI int GLAD_GL_VERSION_1_0;
typedef void (APIENTRYP PFNGLCULLFACEPROC)(GLenum mode);
GLAPI PFNGLCULLFACEPROC glad_glCullFace;
#define glCullFace glad_glCullFace
typedef void (APIENTRYP PFNGLFRONTFACEPROC)(GLenum mode);
GLAPI PFNGLFRONTFACEPROC glad_glFrontFace;
#define glFrontFace glad_glFrontFace
typedef void (APIENTRYP PFNGLHINTPROC)(GLenum target, GLenum mode);
GLAPI PFNGLHINTPROC glad_glHint;
#define glHint glad_glHint
typedef void (APIENTRYP PFNGLLINEWIDTHPROC)(GLfloat width);
GLAPI PFNGLLINEWIDTHPROC glad_glLineWidth;
#define glLineWidth glad_glLineWidth
typedef void (APIENTRYP PFNGLPOINTSIZEPROC)(GLfloat size);
GLAPI PFNGLPOINTSIZEPROC glad_glPointSize;
#define glPointSize glad_glPointSize
typedef void (APIENTRYP PFNGLPOLYGONMODEPROC)(GLenum face, GLenum mode);
GLAPI PFNGLPOLYGONMODEPROC glad_glPolygonMode;
#define glPolygonMode glad_glPolygonMode
typedef void (APIENTRYP PFNGLSCISSORPROC)(GLint x, GLint y, GLsizei width, GLsizei height);
GLAPI PFNGLSCISSORPROC glad_glScissor;
#define glScissor glad_glScissor
typedef void (APIENTRYP PFNGLTEXPARAMETERFPROC)(GLenum target, GLenum pname, GLfloat param);
GLAPI PFNGLTEXPARAMETERFPROC glad_glTexParameterf;
#define glTexParameterf glad_glTexParameterf
typedef void (APIENTRYP PFNGLTEXPARAMETERFVPROC)(GLenum target, GLenum pname, const GLfloat *params);
GLAPI PFNGLTEXPARAMETERFVPROC glad_glTexParameterfv;
#define glTexParameterfv glad_glTexParameterfv
typedef void (APIENTRYP PFNGLTEXPARAMETERIPROC)(GLenum target, GLenum pname, GLint param);
GLAPI PFNGLTEXPARAMETERIPROC glad_glTexParameteri;
#define glTexParameteri glad_glTexParameteri
typedef void (APIENTRYP PFNGLTEXPARAMETERIVPROC)(GLenum target, GLenum pname, const GLint *params);
GLAPI PFNGLTEXPARAMETERIVPROC glad_glTexParameteriv;
#define glTexParameteriv glad_glTexParameteriv
typedef void (APIENTRYP PFNGLTEXIMAGE1DPROC)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void *pixels);
GLAPI PFNGLTEXIMAGE1DPROC glad_glTexImage1D;
#define glTexImage1D glad_glTexImage1D
typedef void (APIENTRYP PFNGLTEXIMAGE2DPROC)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
GLAPI PFNGLTEXIMAGE2DPROC glad_glTexImage2D;
#define glTexImage2D glad_glTexImage2D
typedef void (APIENTRYP PFNGLDRAWBUFFERPROC)(GLenum buf);
GLAPI PFNGLDRAWBUFFERPROC glad_glDrawBuffer;
#define glDrawBuffer glad_glDrawBuffer
typedef void (APIENTRYP PFNGLCLEARPROC)(GLbitfield mask);
GLAPI PFNGLCLEARPROC glad_glClear;
#define glClear glad_glClear
typedef void (APIENTRYP PFNGLCLEARCOLORPROC)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
GLAPI PFNGLCLEARCOLORPROC glad_glClearColor;
#define glClearColor glad_glClearColor
typedef void (APIENTRYP PFNGLCLEARSTENCILPROC)(GLint s);
GLAPI PFNGLCLEARSTENCILPROC glad_glClearStencil;
#define glClearStencil glad_glClearStencil
typedef void (APIENTRYP PFNGLCLEARDEPTHPROC)(GLdouble depth);
GLAPI PFNGLCLEARDEPTHPROC glad_glClearDepth;
#define glClearDepth glad_glClearDepth
typedef void (APIENTRYP PFNGLSTENCILMASKPROC)(GLuint mask);
GLAPI PFNGLSTENCILMASKPROC glad_glStencilMask;
#define glStencilMask glad_glStencilMask
typedef void (APIENTRYP PFNGLCOLORMASKPROC)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
GLAPI PFNGLCOLORMASKPROC glad_glColorMask;
#define glColorMask glad_glColorMask
typedef void (APIENTRYP PFNGLDEPTHMASKPROC)(GLboolean flag);
GLAPI PFNGLDEPTHMASKPROC glad_glDepthMask;
#define glDepthMask glad_glDepthMask
typedef void (APIENTRYP PFNGLDISABLEPROC)(GLenum cap);
GLAPI PFNGLDISABLEPROC glad_glDisable;
#define glDisable glad_glDisable
typedef void (APIENTRYP PFNGLENABLEPROC)(GLenum cap);
GLAPI PFNGLENABLEPROC glad_glEnable;
#define glEnable glad_glEnable
typedef void (APIENTRYP PFNGLFINISHPROC)(void);
GLAPI PFNGLFINISHPROC glad_glFinish;
#define glFinish glad_glFinish
typedef void (APIENTRYP PFNGLFLUSHPROC)(void);
GLAPI PFNGLFLUSHPROC glad_glFlush;
#define glFlush glad_glFlush
typedef void (APIENTRYP PFNGLBLENDFUNCPROC)(GLenum sfactor, GLenum dfactor);
GLAPI PFNGLBLENDFUNCPROC glad_glBlendFunc;
#define glBlendFunc glad_glBlendFunc
typedef void (APIENTRYP PFNGLLOGICOPPROC)(GLenum opcode);
GLAPI PFNGLLOGICOPPROC glad_glLogicOp;
#define glLogicOp glad_glLogicOp
typedef void (APIENTRYP PFNGLSTENCILFUNCPROC)(GLenum func, GLint ref, GLuint mask);
GLAPI PFNGLSTENCILFUNCPROC glad_glStencilFunc;
#define glStencilFunc glad_glStencilFunc
typedef void (APIENTRYP PFNGLSTENCILOPPROC)(GLenum fail, GLenum zfail, GLenum zpass);
GLAPI PFNGLSTENCILOPPROC glad_glStencilOp;
#define glStencilOp glad_glStencilOp
typedef void (APIENTRYP PFNGLDEPTHFUNCPROC)(GLenum func);
GLAPI PFNGLDEPTHFUNCPROC glad_glDepthFunc;
#define glDepthFunc glad_glDepthFunc
typedef void (APIENTRYP PFNGLPIXELSTOREFPROC)(GLenum pname, GLfloat param);
GLAPI PFNGLPIXELSTOREFPROC glad_glPixelStoref;
#define glPixelStoref glad_glPixelStoref
typedef void (APIENTRYP PFNGLPIXELSTOREIPROC)(GLenum pname, GLint param);
GLAPI PFNGLPIXELSTOREIPROC glad_glPixelStorei;
#define glPixelStorei glad_glPixelStorei
typedef void (APIENTRYP PFNGLREADBUFFERPROC)(GLenum src);
GLAPI PFNGLREADBUFFERPROC glad_glReadBuffer;
#define glReadBuffer glad_glReadBuffer
typedef void (APIENTRYP PFNGLREADPIXELSPROC)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels);
GLAPI PFNGLREADPIXELSPROC glad_glReadPixels;
#define glReadPixels glad_glReadPixels
typedef void (APIENTRYP PFNGLGETBOOLEANVPROC)(GLenum pname, GLboolean *data);
GLAPI PFNGLGETBOOLEANVPROC glad_glGetBooleanv;
#define glGetBooleanv glad_glGetBooleanv
typedef void (APIENTRYP PFNGLGETDOUBLEVPROC)(GLenum pname, GLdouble *data);
GLAPI PFNGLGETDOUBLEVPROC glad_glGetDoublev;
#define glGetDoublev glad_glGetDoublev
typedef GLenum (APIENTRYP PFNGLGETERRORPROC)(void);
GLAPI PFNGLGETERRORPROC glad_glGetError;
#define glGetError glad_glGetError
typedef void (APIENTRYP PFNGLGETFLOATVPROC)(GLenum pname, GLfloat *data);
GLAPI PFNGLGETFLOATVPROC glad_glGetFloatv;
#define glGetFloatv glad_glGetFloatv
typedef void (APIENTRYP PFNGLGETINTEGERVPROC)(GLenum pname, GLint *data);
GLAPI PFNGLGETINTEGERVPROC glad_glGetIntegerv;
#define glGetIntegerv glad_glGetIntegerv
typedef const GLubyte * (APIENTRYP PFNGLGETSTRINGPROC)(GLenum name);
GLAPI PFNGLGETSTRINGPROC glad_glGetString;
#define glGetString glad_glGetString
typedef void (APIENTRYP PFNGLGETTEXIMAGEPROC)(GLenum target, GLint level, GLenum format, GLenum type, void *pixels);
GLAPI PFNGLGETTEXIMAGEPROC glad_glGetTexImage;
#define glGetTexImage glad_glGetTexImage
typedef void (APIENTRYP PFNGLGETTEXPARAMETERFVPROC)(GLenum target, GLenum pname, GLfloat *params);
GLAPI PFNGLGETTEXPARAMETERFVPROC glad_glGetTexParameterfv;
#define glGetTexParameterfv glad_glGetTexParameterfv
typedef void (APIENTRYP PFNGLGETTEXPARAMETERIVPROC)(GLenum target, GLenum pname, GLint *params);
GLAPI PFNGLGETTEXPARAMETERIVPROC glad_glGetTexParameteriv;
#define glGetTexParameteriv glad_glGetTexParameteriv
typedef void (APIENTRYP PFNGLGETTEXLEVELPARAMETERFVPROC)(GLenum target, GLint level, GLenum pname, GLfloat *params);
GLAPI PFNGLGETTEXLEVELPARAMETERFVPROC glad_glGetTexLevelParameterfv;
#define glGetTexLevelParameterfv glad_glGetTexLevelParameterfv
typedef void (APIENTRYP PFNGLGETTEXLEVELPARAMETERIVPROC)(GLenum target, GLint level, GLenum pname, GLint *params);
GLAPI PFNGLGETTEXLEVELPARAMETERIVPROC glad_glGetTexLevelParameteriv;
#define glGetTexLevelParameteriv glad_glGetTexLevelParameteriv
typedef GLboolean (APIENTRYP PFNGLISENABLEDPROC)(GLenum cap);
GLAPI PFNGLISENABLEDPROC glad_glIsEnabled;
#define glIsEnabled glad_glIsEnabled
typedef void (APIENTRYP PFNGLDEPTHRANGEPROC)(GLdouble n, GLdouble f);
GLAPI PFNGLDEPTHRANGEPROC glad_glDepthRange;
#define glDepthRange glad_glDepthRange
typedef void (APIENTRYP PFNGLVIEWPORTPROC)(GLint x, GLint y, GLsizei width, GLsizei height);
GLAPI PFNGLVIEWPORTPROC glad_glViewport;
#define glViewport glad_glViewport
#endif
#ifndef GL_VERSION_1_1
#define GL_VERSION_1_1 1
GLAPI int GLAD_GL_VERSION_1_1;
typedef void (APIENTRYP PFNGLDRAWARRAYSPROC)(GLenum mode, GLint first, GLsizei count);
GLAPI PFNGLDRAWARRAYSPROC glad_glDrawArrays;
#define glDrawArrays glad_glDrawArrays
typedef void (APIENTRYP PFNGLDRAWELEMENTSPROC)(GLenum mode, GLsizei count, GLenum type, const void *indices);
GLAPI PFNGLDRAWELEMENTSPROC glad_glDrawElements;
#define glDrawElements glad_glDrawElements
typedef void (APIENTRYP PFNGLPOLYGONOFFSETPROC)(GLfloat factor, GLfloat units);
GLAPI PFNGLPOLYGONOFFSETPROC glad_glPolygonOffset;
#define glPolygonOffset glad_glPolygonOffset
typedef void (APIENTRYP PFNGLCOPYTEXIMAGE1DPROC)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
GLAPI PFNGLCOPYTEXIMAGE1DPROC glad_glCopyTexImage1D;
#define glCopyTexImage1D glad_glCopyTexImage1D
typedef void (APIENTRYP PFNGLCOPYTEXIMAGE2DPROC)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
GLAPI PFNGLCOPYTEXIMAGE2DPROC glad_glCopyTexImage2D;
#define glCopyTexImage2D glad_glCopyTexImage2D
typedef void (APIENTRYP PFNGLCOPYTEXSUBIMAGE1DPROC)(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
GLAPI PFNGLCOPYTEXSUBIMAGE1DPROC glad_glCopyTexSubImage1D;
#define glCopyTexSubImage1D glad_glCopyTexSubImage1D
typedef void (APIENTRYP PFNGLCOPYTEXSUBIMAGE2DPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
GLAPI PFNGLCOPYTEXSUBIMAGE2DPROC glad_glCopyTexSubImage2D;
#define glCopyTexSubImage2D glad_glCopyTexSubImage2D
typedef void (APIENTRYP PFNGLTEXSUBIMAGE1DPROC)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels);
GLAPI PFNGLTEXSUBIMAGE1DPROC glad_glTexSubImage1D;
#define glTexSubImage1D glad_glTexSubImage1D
typedef void (APIENTRYP PFNGLTEXSUBIMAGE2DPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
GLAPI PFNGLTEXSUBIMAGE2DPROC glad_glTexSubImage2D;
#define glTexSubImage2D glad_glTexSubImage2D
typedef void (APIENTRYP PFNGLBINDTEXTUREPROC)(GLenum target, GLuint texture);
GLAPI PFNGLBINDTEXTUREPROC glad_glBindTexture;
#define glBindTexture glad_glBindTexture
typedef void (APIENTRYP PFNGLDELETETEXTURESPROC)(GLsizei n, const GLuint *textures);
GLAPI PFNGLDELETETEXTURESPROC glad_glDeleteTextures;
#define glDeleteTextures glad_glDeleteTextures
typedef void (APIENTRYP PFNGLGENTEXTURESPROC)(GLsizei n, GLuint *textures);
GLAPI PFNGLGENTEXTURESPROC glad_glGenTextures;
#define glGenTextures glad_glGenTextures
typedef GLboolean (APIENTRYP PFNGLISTEXTUREPROC)(GLuint texture);
GLAPI PFNGLISTEXTUREPROC glad_glIsTexture;
#define glIsTexture glad_glIsTexture
#endif
#ifndef GL_VERSION_1_2
#define GL_VERSION_1_2 1
GLAPI int GLAD_GL_VERSION_1_2;
typedef void (APIENTRYP PFNGLDRAWRANGEELEMENTSPROC)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices);
GLAPI PFNGLDRAWRANGEELEMENTSPROC glad_glDrawRangeElements;
#define glDrawRangeElements glad_glDrawRangeElements
typedef void (APIENTRYP PFNGLTEXIMAGE3DPROC)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels);
GLAPI PFNGLTEXIMAGE3DPROC glad_glTexImage3D;
#define glTexImage3D glad_glTexImage3D
typedef void (APIENTRYP PFNGLTEXSUBIMAGE3DPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels);
GLAPI PFNGLTEXSUBIMAGE3DPROC glad_glTexSubImage3D;
#define glTexSubImage3D glad_glTexSubImage3D
typedef void (APIENTRYP PFNGLCOPYTEXSUBIMAGE3DPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
GLAPI PFNGLCOPYTEXSUBIMAGE3DPROC glad_glCopyTexSubImage3D;
#define glCopyTexSubImage3D glad_glCopyTexSubImage3D
#endif
#ifndef GL_VERSION_1_3
#define GL_VERSION_1_3 1
GLAPI int GLAD_GL_VERSION_1_3;
typedef void (APIENTRYP PFNGLACTIVETEXTUREPROC)(GLenum texture);
GLAPI PFNGLACTIVETEXTUREPROC glad_glActiveTexture;
#define glActiveTexture glad_glActiveTexture
typedef void (APIENTRYP PFNGLSAMPLECOVERAGEPROC)(GLfloat value, GLboolean invert);
GLAPI PFNGLSAMPLECOVERAGEPROC glad_glSampleCoverage;
#define glSampleCoverage glad_glSampleCoverage
typedef void (APIENTRYP PFNGLCOMPRESSEDTEXIMAGE3DPROC)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *data);
GLAPI PFNGLCOMPRESSEDTEXIMAGE3DPROC glad_glCompressedTexImage3D;
#define glCompressedTexImage3D glad_glCompressedTexImage3D
typedef void (APIENTRYP PFNGLCOMPRESSEDTEXIMAGE2DPROC)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data);
GLAPI PFNGLCOMPRESSEDTEXIMAGE2DPROC glad_glCompressedTexImage2D;
#define glCompressedTexImage2D glad_glCompressedTexImage2D
typedef void (APIENTRYP PFNGLCOMPRESSEDTEXIMAGE1DPROC)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void *data);
GLAPI PFNGLCOMPRESSEDTEXIMAGE1DPROC glad_glCompressedTexImage1D;
#define glCompressedTexImage1D glad_glCompressedTexImage1D
typedef void (APIENTRYP PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data);
GLAPI PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC glad_glCompressedTexSubImage3D;
#define glCompressedTexSubImage3D glad_glCompressedTexSubImage3D
typedef void (APIENTRYP PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data);
GLAPI PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC glad_glCompressedTexSubImage2D;
#define glCompressedTexSubImage2D glad_glCompressedTexSubImage2D
typedef void (APIENTRYP PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void *data);
GLAPI PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC glad_glCompressedTexSubImage1D;
#define glCompressedTexSubImage1D glad_glCompressedTexSubImage1D
typedef void (APIENTRYP PFNGLGETCOMPRESSEDTEXIMAGEPROC)(GLenum target, GLint level, void *img);
GLAPI PFNGLGETCOMPRESSEDTEXIMAGEPROC glad_glGetCompressedTexImage;
#define glGetCompressedTexImage glad_glGetCompressedTexImage
#endif
#ifndef GL_VERSION_1_4
#define GL_VERSION_1_4 1
GLAPI int GLAD_GL_VERSION_1_4;
typedef void (APIENTRYP PFNGLBLENDFUNCSEPARATEPROC)(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
GLAPI PFNGLBLENDFUNCSEPARATEPROC glad_glBlendFuncSeparate;
#define glBlendFuncSeparate glad_glBlendFuncSeparate
typedef void (APIENTRYP PFNGLMULTIDRAWARRAYSPROC)(GLenum mode, const GLint *first, const GLsizei *count, GLsizei drawcount);
GLAPI PFNGLMULTIDRAWARRAYSPROC glad_glMultiDrawArrays;
#define glMultiDrawArrays glad_glMultiDrawArrays
typedef void (APIENTRYP PFNGLMULTIDRAWELEMENTSPROC)(GLenum mode, const GLsizei *count, GLenum type, const void *const*indices, GLsizei drawcount);
GLAPI PFNGLMULTIDRAWELEMENTSPROC glad_glMultiDrawElements;
#define glMultiDrawElements glad_glMultiDrawElements
typedef void (APIENTRYP PFNGLPOINTPARAMETERFPROC)(GLenum pname, GLfloat param);
GLAPI PFNGLPOINTPARAMETERFPROC glad_glPointParameterf;
#define glPointParameterf glad_glPointParameterf
typedef void (APIENTRYP PFNGLPOINTPARAMETERFVPROC)(GLenum pname, const GLfloat *params);
GLAPI PFNGLPOINTPARAMETERFVPROC glad_glPointParameterfv;
#define glPointParameterfv glad_glPointParameterfv
typedef void (APIENTRYP PFNGLPOINTPARAMETERIPROC)(GLenum pname, GLint param);
GLAPI PFNGLPOINTPARAMETERIPROC glad_glPointParameteri;
#define glPointParameteri glad_glPointParameteri
typedef void (APIENTRYP PFNGLPOINTPARAMETERIVPROC)(GLenum pname, const GLint *params);
GLAPI PFNGLPOINTPARAMETERIVPROC glad_glPointParameteriv;
#define glPointParameteriv glad_glPointParameteriv
typedef void (APIENTRYP PFNGLBLENDCOLORPROC)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
GLAPI PFNGLBLENDCOLORPROC glad_glBlendColor;
#define glBlendColor glad_glBlendColor
typedef void (APIENTRYP PFNGLBLENDEQUATIONPROC)(GLenum mode);
GLAPI PFNGLBLENDEQUATIONPROC glad_glBlendEquation;
#define glBlendEquation glad_glBlendEquation
#endif
#ifndef GL_VERSION_1_5
#define GL_VERSION_1_5 1
GLAPI int GLAD_GL_VERSION_1_5;
typedef void (APIENTRYP PFNGLGENQUERIESPROC)(GLsizei n, GLuint *ids);
GLAPI PFNGLGENQUERIESPROC glad_glGenQueries;
#define glGenQueries glad_glGenQueries
typedef void (APIENTRYP PFNGLDELETEQUERIESPROC)(GLsizei n, const GLuint *ids);
GLAPI PFNGLDELETEQUERIESPROC glad_glDeleteQueries;
#define glDeleteQueries glad_glDeleteQueries
typedef GLboolean (APIENTRYP PFNGLISQUERYPROC)(GLuint id);
GLAPI PFNGLISQUERYPROC glad_glIsQuery;
#define glIsQuery glad_glIsQuery
typedef void (APIENTRYP PFNGLBEGINQUERYPROC)(GLenum target, GLuint id);
GLAPI PFNGLBEGINQUERYPROC glad_glBeginQuery;
#define glBeginQuery glad_glBeginQuery
typedef void (APIENTRYP PFNGLENDQUERYPROC)(GLenum target);
GLAPI PFNGLENDQUERYPROC glad_glEndQuery;
#define glEndQuery glad_glEndQuery
typedef void (APIENTRYP PFNGLGETQUERYIVPROC)(GLenum target, GLenum pname, GLint *params);
GLAPI PFNGLGETQUERYIVPROC glad_glGetQueryiv;
#define glGetQueryiv glad_glGetQueryiv
typedef void (APIENTRYP PFNGLGETQUERYOBJECTIVPROC)(GLuint id, GLenum pname, GLint *params);
GLAPI PFNGLGETQUERYOBJECTIVPROC glad_glGetQueryObjectiv;
#define glGetQueryObjectiv glad_glGetQueryObjectiv
typedef void (APIENTRYP PFNGLGETQUERYOBJECTUIVPROC)(GLuint id, GLenum pname, GLuint *params);
GLAPI PFNGLGETQUERYOBJECTUIVPROC glad_glGetQueryObjectuiv;
#define glGetQueryObjectuiv glad_glGetQueryObjectuiv
typedef void (APIENTRYP PFNGLBINDBUFFERPROC)(GLenum target, GLuint buffer);
GLAPI PFNGLBINDBUFFERPROC glad_glBindBuffer;
#define glBindBuffer glad_glBindBuffer
typedef void (APIENTRYP PFNGLDELETEBUFFERSPROC)(GLsizei n, const GLuint *buffers);
GLAPI PFNGLDELETEBUFFERSPROC glad_glDeleteBuffers;
#define glDeleteBuffers glad_glDeleteBuffers
typedef void (APIENTRYP PFNGLGENBUFFERSPROC)(GLsizei n, GLuint *buffers);
GLAPI PFNGLGENBUFFERSPROC glad_glGenBuffers;
#define glGenBuffers glad_glGenBuffers
typedef GLboolean (APIENTRYP PFNGLISBUFFERPROC)(GLuint buffer);
GLAPI PFNGLISBUFFERPROC glad_glIsBuffer;
#define glIsBuffer glad_glIsBuffer
typedef void (APIENTRYP PFNGLBUFFERDATAPROC)(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
GLAPI PFNGLBUFFERDATAPROC glad_glBufferData;
#define glBufferData glad_glBufferData
typedef void (APIENTRYP PFNGLBUFFERSUBDATAPROC)(GLenum target, GLintptr offset, GLsizeiptr size, const void *data);
GLAPI PFNGLBUFFERSUBDATAPROC glad_glBufferSubData;
#define glBufferSubData glad_glBufferSubData
typedef void (APIENTRYP PFNGLGETBUFFERSUBDATAPROC)(GLenum target, GLintptr offset, GLsizeiptr size, void *data);
GLAPI PFNGLGETBUFFERSUBDATAPROC glad_glGetBufferSubData;
#define glGetBufferSubData glad_glGetBufferSubData
typedef void * (APIENTRYP PFNGLMAPBUFFERPROC)(GLenum target, GLenum access);
GLAPI PFNGLMAPBUFFERPROC glad_glMapBuffer;
#define glMapBuffer glad_glMapBuffer
typedef GLboolean (APIENTRYP PFNGLUNMAPBUFFERPROC)(GLenum target);
GLAPI PFNGLUNMAPBUFFERPROC glad_glUnmapBuffer;
#define glUnmapBuffer glad_glUnmapBuffer
typedef void (APIENTRYP PFNGLGETBUFFERPARAMETERIVPROC)(GLenum target, GLenum pname, GLint *params);
GLAPI PFNGLGETBUFFERPARAMETERIVPROC glad_glGetBufferParameteriv;
#define glGetBufferParameteriv glad_glGetBufferParameteriv
typedef void (APIENTRYP PFNGLGETBUFFERPOINTERVPROC)(GLenum target, GLenum pname, void **params);
GLAPI PFNGLGETBUFFERPOINTERVPROC glad_glGetBufferPointerv;
#define glGetBufferPointerv glad_glGetBufferPointerv
#endif
#ifndef GL_VERSION_2_0
#define GL_VERSION_2_0 1
GLAPI int GLAD_GL_VERSION_2_0;
typedef void (APIENTRYP PFNGLBLENDEQUATIONSEPARATEPROC)(GLenum modeRGB, GLenum modeAlpha);
GLAPI PFNGLBLENDEQUATIONSEPARATEPROC glad_glBlendEquationSeparate;
#define glBlendEquationSeparate glad_glBlendEquationSeparate
typedef void (APIENTRYP PFNGLDRAWBUFFERSPROC)(GLsizei n, const GLenum *bufs);
GLAPI PFNGLDRAWBUFFERSPROC glad_glDrawBuffers;
#define glDrawBuffers glad_glDrawBuffers
typedef void (APIENTRYP PFNGLSTENCILOPSEPARATEPROC)(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
GLAPI PFNGLSTENCILOPSEPARATEPROC glad_glStencilOpSeparate;
#define glStencilOpSeparate glad_glStencilOpSeparate
typedef void (APIENTRYP PFNGLSTENCILFUNCSEPARATEPROC)(GLenum face, GLenum func, GLint ref, GLuint mask);
GLAPI PFNGLSTENCILFUNCSEPARATEPROC glad_glStencilFuncSeparate;
#define glStencilFuncSeparate glad_glStencilFuncSeparate
typedef void (APIENTRYP PFNGLSTENCILMASKSEPARATEPROC)(GLenum face, GLuint mask);
GLAPI PFNGLSTENCILMASKSEPARATEPROC glad_glStencilMaskSeparate;
#define glStencilMaskSeparate glad_glStencilMaskSeparate
typedef void (APIENTRYP PFNGLATTACHSHADERPROC)(GLuint program, GLuint shader);
GLAPI PFNGLATTACHSHADERPROC glad_glAttachShader;
#define glAttachShader glad_glAttachShader
typedef void (APIENTRYP PFNGLBINDATTRIBLOCATIONPROC)(GLuint program, GLuint index, const GLchar *name);
GLAPI PFNGLBINDATTRIBLOCATIONPROC glad_glBindAttribLocation;
#define glBindAttribLocation glad_glBindAttribLocation
typedef void (APIENTRYP PFNGLCOMPILESHADERPROC)(GLuint shader);
GLAPI PFNGLCOMPILESHADERPROC glad_glCompileShader;
#define glCompileShader glad_glCompileShader
typedef GLuint (APIENTRYP PFNGLCREATEPROGRAMPROC)(void);
GLAPI PFNGLCREATEPROGRAMPROC glad_glCreateProgram;
#define glCreateProgram glad_glCreateProgram
typedef GLuint (APIENTRYP PFNGLCREATESHADERPROC)(GLenum type);
GLAPI PFNGLCREATESHADERPROC glad_glCreateShader;
#define glCreateShader glad_glCreateShader
typedef void (APIENTRYP PFNGLDELETEPROGRAMPROC)(GLuint program);
GLAPI PFNGLDELETEPROGRAMPROC glad_glDeleteProgram;
#define glDeleteProgram glad_glDeleteProgram
typedef void (APIENTRYP PFNGLDELETESHADERPROC)(GLuint shader);
GLAPI PFNGLDELETESHADERPROC glad_glDeleteShader;
#define glDeleteShader glad_glDeleteShader
typedef void (APIENTRYP PFNGLDETACHSHADERPROC)(GLuint program, GLuint shader);
GLAPI PFNGLDETACHSHADERPROC glad_glDetachShader;
#define glDetachShader glad_glDetachShader
typedef void (APIENTRYP PFNGLDISABLEVERTEXATTRIBARRAYPROC)(GLuint index);
GLAPI PFNGLDISABLEVERTEXATTRIBARRAYPROC glad_glDisableVertexAttribArray;
#define glDisableVertexAttribArray glad_glDisableVertexAttribArray
typedef void (APIENTRYP PFNGLENABLEVERTEXATTRIBARRAYPROC)(GLuint index);
GLAPI PFNGLENABLEVERTEXATTRIBARRAYPROC glad_glEnableVertexAttribArray;
#define glEnableVertexAttribArray glad_glEnableVertexAttribArray
typedef void (APIENTRYP PFNGLGETACTIVEATTRIBPROC)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
GLAPI PFNGLGETACTIVEATTRIBPROC glad_glGetActiveAttrib;
#define glGetActiveAttrib glad_glGetActiveAttrib
typedef void (APIENTRYP PFNGLGETACTIVEUNIFORMPROC)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
GLAPI PFNGLGETACTIVEUNIFORMPROC glad_glGetActiveUniform;
#define glGetActiveUniform glad_glGetActiveUniform
typedef void (APIENTRYP PFNGLGETATTACHEDSHADERSPROC)(GLuint program, GLsizei maxCount, GLsizei *count, GLuint *shaders);
GLAPI PFNGLGETATTACHEDSHADERSPROC glad_glGetAttachedShaders;
#define glGetAttachedShaders glad_glGetAttachedShaders
typedef GLint (APIENTRYP PFNGLGETATTRIBLOCATIONPROC)(GLuint program, const GLchar *name);
GLAPI PFNGLGETATTRIBLOCATIONPROC glad_glGetAttribLocation;
#define glGetAttribLocation glad_glGetAttribLocation
typedef void (APIENTRYP PFNGLGETPROGRAMIVPROC)(GLuint program, GLenum pname, GLint *params);
GLAPI PFNGLGETPROGRAMIVPROC glad_glGetProgramiv;
#define glGetProgramiv glad_glGetProgramiv
typedef void (APIENTRYP PFNGLGETPROGRAMINFOLOGPROC)(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
GLAPI PFNGLGETPROGRAMINFOLOGPROC glad_glGetProgramInfoLog;
#define glGetProgramInfoLog glad_glGetProgramInfoLog
typedef void (APIENTRYP PFNGLGETSHADERIVPROC)(GLuint shader, GLenum pname, GLint *params);
GLAPI PFNGLGETSHADERIVPROC glad_glGetShaderiv;
#define glGetShaderiv glad_glGetShaderiv
typedef void (APIENTRYP PFNGLGETSHADERINFOLOGPROC)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
GLAPI PFNGLGETSHADERINFOLOGPROC glad_glGetShaderInfoLog;
#define glGetShaderInfoLog glad_glGetShaderInfoLog
typedef void (APIENTRYP PFNGLGETSHADERSOURCEPROC)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source);
GLAPI PFNGLGETSHADERSOURCEPROC glad_glGetShaderSource;
#define glGetShaderSource glad_glGetShaderSource
typedef GLint (APIENTRYP PFNGLGETUNIFORMLOCATIONPROC)(GLuint program, const GLchar *name);
GLAPI PFNGLGETUNIFORMLOCATIONPROC glad_glGetUniformLocation;
#define glGetUniformLocation glad_glGetUniformLocation
typedef void (APIENTRYP PFNGLGETUNIFORMFVPROC)(GLuint program, GLint location, GLfloat *params);
GLAPI PFNGLGETUNIFORMFVPROC glad_glGetUniformfv;
#define glGetUniformfv glad_glGetUniformfv
typedef void (APIENTRYP PFNGLGETUNIFORMIVPROC)(GLuint program, GLint location, GLint *params);
GLAPI PFNGLGETUNIFORMIVPROC glad_glGetUniformiv;
#define glGetUniformiv glad_glGetUniformiv
typedef void (APIENTRYP PFNGLGETVERTEXATTRIBDVPROC)(GLuint index, GLenum pname, GLdouble *params);
GLAPI PFNGLGETVERTEXATTRIBDVPROC glad_glGetVertexAttribdv;
#define glGetVertexAttribdv glad_glGetVertexAttribdv
typedef void (APIENTRYP PFNGLGETVERTEXATTRIBFVPROC)(GLuint index, GLenum pname, GLfloat *params);
GLAPI PFNGLGETVERTEXATTRIBFVPROC glad_glGetVertexAttribfv;
#define glGetVertexAttribfv glad_glGetVertexAttribfv
typedef void (APIENTRYP PFNGLGETVERTEXATTRIBIVPROC)(GLuint index, GLenum pname, GLint *params);
GLAPI PFNGLGETVERTEXATTRIBIVPROC glad_glGetVertexAttribiv;
#define glGetVertexAttribiv glad_glGetVertexAttribiv
typedef void (APIENTRYP PFNGLGETVERTEXATTRIBPOINTERVPROC)(GLuint index, GLenum pname, void **pointer);
GLAPI PFNGLGETVERTEXATTRIBPOINTERVPROC glad_glGetVertexAttribPointerv;
#define glGetVertexAttribPointerv glad_glGetVertexAttribPointerv
typedef GLboolean (APIENTRYP PFNGLISPROGRAMPROC)(GLuint program);
GLAPI PFNGLISPROGRAMPROC glad_glIsProgram;
#define glIsProgram glad_glIsProgram
typedef GLboolean (APIENTRYP PFNGLISSHADERPROC)(GLuint shader);
GLAPI PFNGLISSHADERPROC glad_glIsShader;
#define glIsShader glad_glIsShader
typedef void (APIENTRYP PFNGLLINKPROGRAMPROC)(GLuint program);
GLAPI PFNGLLINKPROGRAMPROC glad_glLinkProgram;
#define glLinkProgram glad_glLinkProgram
typedef void (APIENTRYP PFNGLSHADERSOURCEPROC)(GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
GLAPI PFNGLSHADERSOURCEPROC glad_glShaderSource;
#define glShaderSource glad_glShaderSource
typedef void (APIENTRYP PFNGLUSEPROGRAMPROC)(GLuint program);
GLAPI PFNGLUSEPROGRAMPROC glad_glUseProgram;
#define glUseProgram glad_glUseProgram
typedef void (APIENTRYP PFNGLUNIFORM1FPROC)(GLint location, GLfloat v0);
GLAPI PFNGLUNIFORM1FPROC glad_glUniform1f;
#define glUniform1f glad_glUniform1f
typedef void (APIENTRYP PFNGLUNIFORM2FPROC)(GLint location, GLfloat v0, GLfloat v1);
GLAPI PFNGLUNIFORM2FPROC glad_glUniform2f;
#define glUniform2f glad_glUniform2f
typedef void (APIENTRYP PFNGLUNIFORM3FPROC)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
GLAPI PFNGLUNIFORM3FPROC glad_glUniform3f;
#define glUniform3f glad_glUniform3f
typedef void (APIENTRYP PFNGLUNIFORM4FPROC)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
GLAPI PFNGLUNIFORM4FPROC glad_glUniform4f;
#define glUniform4f glad_glUniform4f
typedef void (APIENTRYP PFNGLUNIFORM1IPROC)(GLint location, GLint v0);
GLAPI PFNGLUNIFORM1IPROC glad_glUniform1i;
#define glUniform1i glad_glUniform1i
typedef void (APIENTRYP PFNGLUNIFORM2IPROC)(GLint location, GLint v0, GLint v1);
GLAPI PFNGLUNIFORM2IPROC glad_glUniform2i;
#define glUniform2i glad_glUniform2i
typedef void (APIENTRYP PFNGLUNIFORM3IPROC)(GLint location, GLint v0, GLint v1, GLint v2);
GLAPI PFNGLUNIFORM3IPROC glad_glUniform3i;
#define glUniform3i glad_glUniform3i
typedef void (APIENTRYP PFNGLUNIFORM4IPROC)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
GLAPI PFNGLUNIFORM4IPROC glad_glUniform4i;
#define glUniform4i glad_glUniform4i
typedef void (APIENTRYP PFNGLUNIFORM1FVPROC)(GLint location, GLsizei count, const GLfloat *value);
GLAPI PFNGLUNIFORM1FVPROC glad_glUniform1fv;
#define glUniform1fv glad_glUniform1fv
typedef void (APIENTRYP PFNGLUNIFORM2FVPROC)(GLint location, GLsizei count, const GLfloat *value);
GLAPI PFNGLUNIFORM2FVPROC glad_glUniform2fv;
#define glUniform2fv glad_glUniform2fv
typedef void (APIENTRYP PFNGLUNIFORM3FVPROC)(GLint location, GLsizei count, const GLfloat *value);
GLAPI PFNGLUNIFORM3FVPROC glad_glUniform3fv;
#define glUniform3fv glad_glUniform3fv
typedef void (APIENTRYP PFNGLUNIFORM4FVPROC)(GLint location, GLsizei count, const GLfloat *value);
GLAPI PFNGLUNIFORM4FVPROC glad_glUniform4fv;
#define glUniform4fv glad_glUniform4fv
typedef void (APIENTRYP PFNGLUNIFORM1IVPROC)(GLint location, GLsizei count, const GLint *value);
GLAPI PFNGLUNIFORM1IVPROC glad_glUniform1iv;
#define glUniform1iv glad_glUniform1iv
typedef void (APIENTRYP PFNGLUNIFORM2IVPROC)(GLint location, GLsizei count, const GLint *value);
GLAPI PFNGLUNIFORM2IVPROC glad_glUniform2iv;
#define glUniform2iv glad_glUniform2iv
typedef void (APIENTRYP PFNGLUNIFORM3IVPROC)(GLint location, GLsizei count, const GLint *value);
GLAPI PFNGLUNIFORM3IVPROC glad_glUniform3iv;
#define glUniform3iv glad_glUniform3iv
typedef void (APIENTRYP PFNGLUNIFORM4IVPROC)(GLint location, GLsizei count, const GLint *value);
GLAPI PFNGLUNIFORM4IVPROC glad_glUniform4iv;
#define glUniform4iv glad_glUniform4iv
typedef void (APIENTRYP PFNGLUNIFORMMATRIX2FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI PFNGLUNIFORMMATRIX2FVPROC glad_glUniformMatrix2fv;
#define glUniformMatrix2fv glad_glUniformMatrix2fv
typedef void (APIENTRYP PFNGLUNIFORMMATRIX3FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI PFNGLUNIFORMMATRIX3FVPROC glad_glUniformMatrix3fv;
#define glUniformMatrix3fv glad_glUniformMatrix3fv
typedef void (APIENTRYP PFNGLUNIFORMMATRIX4FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI PFNGLUNIFORMMATRIX4FVPROC glad_glUniformMatrix4fv;
#define glUniformMatrix4fv glad_glUniformMatrix4fv
typedef void (APIENTRYP PFNGLVALIDATEPROGRAMPROC)(GLuint program);
GLAPI PFNGLVALIDATEPROGRAMPROC glad_glValidateProgram;
#define glValidateProgram glad_glValidateProgram
typedef void (APIENTRYP PFNGLVERTEXATTRIB1DPROC)(GLuint index, GLdouble x);
GLAPI PFNGLVERTEXATTRIB1DPROC glad_glVertexAttrib1d;
#define glVertexAttrib1d glad_glVertexAttrib1d
typedef void (APIENTRYP PFNGLVERTEXATTRIB1DVPROC)(GLuint index, const GLdouble *v);
GLAPI PFNGLVERTEXATTRIB1DVPROC glad_glVertexAttrib1dv;
#define glVertexAttrib1dv glad_glVertexAttrib1dv
typedef void (APIENTRYP PFNGLVERTEXATTRIB1FPROC)(GLuint index, GLfloat x);
GLAPI PFNGLVERTEXATTRIB1FPROC glad_glVertexAttrib1f;
#define glVertexAttrib1f glad_glVertexAttrib1f
typedef void (APIENTRYP PFNGLVERTEXATTRIB1FVPROC)(GLuint index, const GLfloat *v);
GLAPI PFNGLVERTEXATTRIB1FVPROC glad_glVertexAttrib1fv;
#define glVertexAttrib1fv glad_glVertexAttrib1fv
typedef void (APIENTRYP PFNGLVERTEXATTRIB1SPROC)(GLuint index, GLshort x);
GLAPI PFNGLVERTEXATTRIB1SPROC glad_glVertexAttrib1s;
#define glVertexAttrib1s glad_glVertexAttrib1s
typedef void (APIENTRYP PFNGLVERTEXATTRIB1SVPROC)(GLuint index, const GLshort *v);
GLAPI PFNGLVERTEXATTRIB1SVPROC glad_glVertexAttrib1sv;
#define glVertexAttrib1sv glad_glVertexAttrib1sv
typedef void (APIENTRYP PFNGLVERTEXATTRIB2DPROC)(GLuint index, GLdouble x, GLdouble y);
GLAPI PFNGLVERTEXATTRIB2DPROC glad_glVertexAttrib2d;
#define glVertexAttrib2d glad_glVertexAttrib2d
typedef void (APIENTRYP PFNGLVERTEXATTRIB2DVPROC)(GLuint index, const GLdouble *v);
GLAPI PFNGLVERTEXATTRIB2DVPROC glad_glVertexAttrib2dv;
#define glVertexAttrib2dv glad_glVertexAttrib2dv
typedef void (APIENTRYP PFNGLVERTEXATTRIB2FPROC)(GLuint index, GLfloat x, GLfloat y);
GLAPI PFNGLVERTEXATTRIB2FPROC glad_glVertexAttrib2f;
#define glVertexAttrib2f glad_glVertexAttrib2f
typedef void (APIENTRYP PFNGLVERTEXATTRIB2FVPROC)(GLuint index, const GLfloat *v);
GLAPI PFNGLVERTEXATTRIB2FVPROC glad_glVertexAttrib2fv;
#define glVertexAttrib2fv glad_glVertexAttrib2fv
typedef void (APIENTRYP PFNGLVERTEXATTRIB2SPROC)(GLuint index, GLshort x, GLshort y);
GLAPI PFNGLVERTEXATTRIB2SPROC glad_glVertexAttrib2s;
#define glVertexAttrib2s glad_glVertexAttrib2s
typedef void (APIENTRYP PFNGLVERTEXATTRIB2SVPROC)(GLuint index, const GLshort *v);
GLAPI PFNGLVERTEXATTRIB2SVPROC glad_glVertexAttrib2sv;
#define glVertexAttrib2sv glad_glVertexAttrib2sv
typedef void (APIENTRYP PFNGLVERTEXATTRIB3DPROC)(GLuint index, GLdouble x, GLdouble y, GLdouble z);
GLAPI PFNGLVERTEXATTRIB3DPROC glad_glVertexAttrib3d;
#define glVertexAttrib3d glad_glVertexAttrib3d
typedef void (APIENTRYP PFNGLVERTEXATTRIB3DVPROC)(GLuint index, const GLdouble *v);
GLAPI PFNGLVERTEXATTRIB3DVPROC glad_glVertexAttrib3dv;
#define glVertexAttrib3dv glad_glVertexAttrib3dv
typedef void (APIENTRYP PFNGLVERTEXATTRIB3FPROC)(GLuint index, GLfloat x, GLfloat y, GLfloat z);
GLAPI PFNGLVERTEXATTRIB3FPROC glad_glVertexAttrib3f;
#define glVertexAttrib3f glad_glVertexAttrib3f
typedef void (APIENTRYP PFNGLVERTEXATTRIB3FVPROC)(GLuint index, const GLfloat *v);
GLAPI PFNGLVERTEXATTRIB3FVPROC glad_glVertexAttrib3fv;
#define glVertexAttrib3fv glad_glVertexAttrib3fv
typedef void (APIENTRYP PFNGLVERTEXATTRIB3SPROC)(GLuint index, GLshort x, GLshort y, GLshort z);
GLAPI PFNGLVERTEXATTRIB3SPROC glad_glVertexAttrib3s;
#define glVertexAttrib3s glad_glVertexAttrib3s
typedef void (APIENTRYP PFNGLVERTEXATTRIB3SVPROC)(GLuint index, const GLshort *v);
GLAPI PFNGLVERTEXATTRIB3SVPROC glad_glVertexAttrib3sv;
#define glVertexAttrib3sv glad_glVertexAttrib3sv
typedef void (APIENTRYP PFNGLVERTEXATTRIB4NBVPROC)(GLuint index, const GLbyte *v);
GLAPI PFNGLVERTEXATTRIB4NBVPROC glad_glVertexAttrib4Nbv;
#define glVertexAttrib4Nbv glad_glVertexAttrib4Nbv
typedef void (APIENTRYP PFNGLVERTEXATTRIB4NIVPROC)(GLuint index, const GLint *v);
GLAPI PFNGLVERTEXATTRIB4NIVPROC glad_glVertexAttrib4Niv;
#define glVertexAttrib4Niv glad_glVertexAttrib4Niv
typedef void (APIENTRYP PFNGLVERTEXATTRIB4NSVPROC)(GLuint index, const GLshort *v);
GLAPI PFNGLVERTEXATTRIB4NSVPROC glad_glVertexAttrib4Nsv;
#define glVertexAttrib4Nsv glad_glVertexAttrib4Nsv
typedef void (APIENTRYP PFNGLVERTEXATTRIB4NUBPROC)(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
GLAPI PFNGLVERTEXATTRIB4NUBPROC glad_glVertexAttrib4Nub;
#define glVertexAttrib4Nub glad_glVertexAttrib4Nub
typedef void (APIENTRYP PFNGLVERTEXATTRIB4NUBVPROC)(GLuint index, const GLubyte *v);
GLAPI PFNGLVERTEXATTRIB4NUBVPROC glad_glVertexAttrib4Nubv;
#define glVertexAttrib4Nubv glad_glVertexAttrib4Nubv
typedef void (APIENTRYP PFNGLVERTEXATTRIB4NUIVPROC)(GLuint index, const GLuint *v);
GLAPI PFNGLVERTEXATTRIB4NUIVPROC glad_glVertexAttrib4Nuiv;
#define glVertexAttrib4Nuiv glad_glVertexAttrib4Nuiv
typedef void (APIENTRYP PFNGLVERTEXATTRIB4NUSVPROC)(GLuint index, const GLushort *v);
GLAPI PFNGLVERTEXATTRIB4NUSVPROC glad_glVertexAttrib4Nusv;
#define glVertexAttrib4Nusv glad_glVertexAttrib4Nusv
typedef void (APIENTRYP PFNGLVERTEXATTRIB4BVPROC)(GLuint index, const GLbyte *v);
GLAPI PFNGLVERTEXATTRIB4BVPROC glad_glVertexAttrib4bv;
#define glVertexAttrib4bv glad_glVertexAttrib4bv
typedef void (APIENTRYP PFNGLVERTEXATTRIB4DPROC)(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
GLAPI PFNGLVERTEXATTRIB4DPROC glad_glVertexAttrib4d;
#define glVertexAttrib4d glad_glVertexAttrib4d
typedef void (APIENTRYP PFNGLVERTEXATTRIB4DVPROC)(GLuint index, const GLdouble *v);
GLAPI PFNGLVERTEXATTRIB4DVPROC glad_glVertexAttrib4dv;
#define glVertexAttrib4dv glad_glVertexAttrib4dv
typedef void (APIENTRYP PFNGLVERTEXATTRIB4FPROC)(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
GLAPI PFNGLVERTEXATTRIB4FPROC glad_glVertexAttrib4f;
#define glVertexAttrib4f glad_glVertexAttrib4f
typedef void (APIENTRYP PFNGLVERTEXATTRIB4FVPROC)(GLuint index, const GLfloat *v);
GLAPI PFNGLVERTEXATTRIB4FVPROC glad_glVertexAttrib4fv;
#define glVertexAttrib4fv glad_glVertexAttrib4fv
typedef void (APIENTRYP PFNGLVERTEXATTRIB4IVPROC)(GLuint index, const GLint *v);
GLAPI PFNGLVERTEXATTRIB4IVPROC glad_glVertexAttrib4iv;
#define glVertexAttrib4iv glad_glVertexAttrib4iv
typedef void (APIENTRYP PFNGLVERTEXATTRIB4SPROC)(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
GLAPI PFNGLVERTEXATTRIB4SPROC glad_glVertexAttrib4s;
#define glVertexAttrib4s glad_glVertexAttrib4s
typedef void (APIENTRYP PFNGLVERTEXATTRIB4SVPROC)(GLuint index, const GLshort *v);
GLAPI PFNGLVERTEXATTRIB4SVPROC glad_glVertexAttrib4sv;
#define glVertexAttrib4sv glad_glVertexAttrib4sv
typedef void (APIENTRYP PFNGLVERTEXATTRIB4UBVPROC)(GLuint index, const GLubyte *v);
GLAPI PFNGLVERTEXATTRIB4UBVPROC glad_glVertexAttrib4ubv;
#define glVertexAttrib4ubv glad_glVertexAttrib4ubv
typedef void (APIENTRYP PFNGLVERTEXATTRIB4UIVPROC)(GLuint index, const GLuint *v);
GLAPI PFNGLVERTEXATTRIB4UIVPROC glad_glVertexAttrib4uiv;
#define glVertexAttrib4uiv glad_glVertexAttrib4uiv
typedef void (APIENTRYP PFNGLVERTEXATTRIB4USVPROC)(GLuint index, const GLushort *v);
GLAPI PFNGLVERTEXATTRIB4USVPROC glad_glVertexAttrib4usv;
#define glVertexAttrib4usv glad_glVertexAttrib4usv
typedef void (APIENTRYP PFNGLVERTEXATTRIBPOINTERPROC)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
GLAPI PFNGLVERTEXATTRIBPOINTERPROC glad_glVertexAttribPointer;
#define glVertexAttribPointer glad_glVertexAttribPointer
#endif
#ifndef GL_VERSION_2_1
#define GL_VERSION_2_1 1
GLAPI int GLAD_GL_VERSION_2_1;
typedef void (APIENTRYP PFNGLUNIFORMMATRIX2X3FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI PFNGLUNIFORMMATRIX2X3FVPROC glad_glUniformMatrix2x3fv;
#define glUniformMatrix2x3fv glad_glUniformMatrix2x3fv
typedef void (APIENTRYP PFNGLUNIFORMMATRIX3X2FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI PFNGLUNIFORMMATRIX3X2FVPROC glad_glUniformMatrix3x2fv;
#define glUniformMatrix3x2fv glad_glUniformMatrix3x2fv
typedef void (APIENTRYP PFNGLUNIFORMMATRIX2X4FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI PFNGLUNIFORMMATRIX2X4FVPROC glad_glUniformMatrix2x4fv;
#define glUniformMatrix2x4fv glad_glUniformMatrix2x4fv
typedef void (APIENTRYP PFNGLUNIFORMMATRIX4X2FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI PFNGLUNIFORMMATRIX4X2FVPROC glad_glUniformMatrix4x2fv;
#define glUniformMatrix4x2fv glad_glUniformMatrix4x2fv
typedef void (APIENTRYP PFNGLUNIFORMMATRIX3X4FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI PFNGLUNIFORMMATRIX3X4FVPROC glad_glUniformMatrix3x4fv;
#define glUniformMatrix3x4fv glad_glUniformMatrix3x4fv
typedef void (APIENTRYP PFNGLUNIFORMMATRIX4X3FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI PFNGLUNIFORMMATRIX4X3FVPROC glad_glUniformMatrix4x3fv;
#define glUniformMatrix4x3fv glad_glUniformMatrix4x3fv
#endif
#ifndef GL_VERSION_3_0
#define GL_VERSION_3_0 1
GLAPI int GLAD_GL_VERSION_3_0;
typedef void (APIENTRYP PFNGLCOLORMASKIPROC)(GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
GLAPI PFNGLCOLORMASKIPROC glad_glColorMaski;
#define glColorMaski glad_glColorMaski
typedef void (APIENTRYP PFNGLGETBOOLEANI_VPROC)(GLenum target, GLuint index, GLboolean *data);
GLAPI PFNGLGETBOOLEANI_VPROC glad_glGetBooleani_v;
#define glGetBooleani_v glad_glGetBooleani_v
typedef void (APIENTRYP PFNGLGETINTEGERI_VPROC)(GLenum target, GLuint index, GLint *data);
GLAPI PFNGLGETINTEGERI_VPROC glad_glGetIntegeri_v;
#define glGetIntegeri_v glad_glGetIntegeri_v
typedef void (APIENTRYP PFNGLENABLEIPROC)(GLenum target, GLuint index);
GLAPI PFNGLENABLEIPROC glad_glEnablei;
#define glEnablei glad_glEnablei
typedef void (APIENTRYP PFNGLDISABLEIPROC)(GLenum target, GLuint index);
GLAPI PFNGLDISABLEIPROC glad_glDisablei;
#define glDisablei glad_glDisablei
typedef GLboolean (APIENTRYP PFNGLISENABLEDIPROC)(GLenum target, GLuint index);
GLAPI PFNGLISENABLEDIPROC glad_glIsEnabledi;
#define glIsEnabledi glad_glIsEnabledi
typedef void (APIENTRYP PFNGLBEGINTRANSFORMFEEDBACKPROC)(GLenum primitiveMode);
GLAPI PFNGLBEGINTRANSFORMFEEDBACKPROC glad_glBeginTransformFeedback;
#define glBeginTransformFeedback glad_glBeginTransformFeedback
typedef void (APIENTRYP PFNGLENDTRANSFORMFEEDBACKPROC)(void);
GLAPI PFNGLENDTRANSFORMFEEDBACKPROC glad_glEndTransformFeedback;
#define glEndTransformFeedback glad_glEndTransformFeedback
typedef void (APIENTRYP PFNGLBINDBUFFERRANGEPROC)(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
GLAPI PFNGLBINDBUFFERRANGEPROC glad_glBindBufferRange;
#define glBindBufferRange glad_glBindBufferRange
typedef void (APIENTRYP PFNGLBINDBUFFERBASEPROC)(GLenum target, GLuint index, GLuint buffer);
GLAPI PFNGLBINDBUFFERBASEPROC glad_glBindBufferBase;
#define glBindBufferBase glad_glBindBufferBase
typedef void (APIENTRYP PFNGLTRANSFORMFEEDBACKVARYINGSPROC)(GLuint program, GLsizei count, const GLchar *const*varyings, GLenum bufferMode);
GLAPI PFNGLTRANSFORMFEEDBACKVARYINGSPROC glad_glTransformFeedbackVaryings;
#define glTransformFeedbackVaryings glad_glTransformFeedbackVaryings
typedef void (APIENTRYP PFNGLGETTRANSFORMFEEDBACKVARYINGPROC)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name);
GLAPI PFNGLGETTRANSFORMFEEDBACKVARYINGPROC glad_glGetTransformFeedbackVarying;
#define glGetTransformFeedbackVarying glad_glGetTransformFeedbackVarying
typedef void (APIENTRYP PFNGLCLAMPCOLORPROC)(GLenum target, GLenum clamp);
GLAPI PFNGLCLAMPCOLORPROC glad_glClampColor;
#define glClampColor glad_glClampColor
typedef void (APIENTRYP PFNGLBEGINCONDITIONALRENDERPROC)(GLuint id, GLenum mode);
GLAPI PFNGLBEGINCONDITIONALRENDERPROC glad_glBeginConditionalRender;
#define glBeginConditionalRender glad_glBeginConditionalRender
typedef void (APIENTRYP PFNGLENDCONDITIONALRENDERPROC)(void);
GLAPI PFNGLENDCONDITIONALRENDERPROC glad_glEndConditionalRender;
#define glEndConditionalRender glad_glEndConditionalRender
typedef void (APIENTRYP PFNGLVERTEXATTRIBIPOINTERPROC)(GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer);
GLAPI PFNGLVERTEXATTRIBIPOINTERPROC glad_glVertexAttribIPointer;
#define glVertexAttribIPointer glad_glVertexAttribIPointer
typedef void (APIENTRYP PFNGLGETVERTEXATTRIBIIVPROC)(GLuint index, GLenum pname, GLint *params);
GLAPI PFNGLGETVERTEXATTRIBIIVPROC glad_glGetVertexAttribIiv;
#define glGetVertexAttribIiv glad_glGetVertexAttribIiv
typedef void (APIENTRYP PFNGLGETVERTEXATTRIBIUIVPROC)(GLuint index, GLenum pname, GLuint *params);
GLAPI PFNGLGETVERTEXATTRIBIUIVPROC glad_glGetVertexAttribIuiv;
#define glGetVertexAttribIuiv glad_glGetVertexAttribIuiv
typedef void (APIENTRYP PFNGLVERTEXATTRIBI1IPROC)(GLuint index, GLint x);
GLAPI PFNGLVERTEXATTRIBI1IPROC glad_glVertexAttribI1i;
#define glVertexAttribI1i glad_glVertexAttribI1i
typedef void (APIENTRYP PFNGLVERTEXATTRIBI2IPROC)(GLuint index, GLint x, GLint y);
GLAPI PFNGLVERTEXATTRIBI2IPROC glad_glVertexAttribI2i;
#define glVertexAttribI2i glad_glVertexAttribI2i
typedef void (APIENTRYP PFNGLVERTEXATTRIBI3IPROC)(GLuint index, GLint x, GLint y, GLint z);
GLAPI PFNGLVERTEXATTRIBI3IPROC glad_glVertexAttribI3i;
#define glVertexAttribI3i glad_glVertexAttribI3i
typedef void (APIENTRYP PFNGLVERTEXATTRIBI4IPROC)(GLuint index, GLint x, GLint y, GLint z, GLint w);
GLAPI PFNGLVERTEXATTRIBI4IPROC glad_glVertexAttribI4i;
#define glVertexAttribI4i glad_glVertexAttribI4i
typedef void (APIENTRYP PFNGLVERTEXATTRIBI1UIPROC)(GLuint index, GLuint x);
GLAPI PFNGLVERTEXATTRIBI1UIPROC glad_glVertexAttribI1ui;
#define glVertexAttribI1ui glad_glVertexAttribI1ui
typedef void (APIENTRYP PFNGLVERTEXATTRIBI2UIPROC)(GLuint index, GLuint x, GLuint y);
GLAPI PFNGLVERTEXATTRIBI2UIPROC glad_glVertexAttribI2ui;
#define glVertexAttribI2ui glad_glVertexAttribI2ui
typedef void (APIENTRYP PFNGLVERTEXATTRIBI3UIPROC)(GLuint index, GLuint x, GLuint y, GLuint z);
GLAPI PFNGLVERTEXATTRIBI3UIPROC glad_glVertexAttribI3ui;
#define glVertexAttribI3ui glad_glVertexAttribI3ui
typedef void (APIENTRYP PFNGLVERTEXATTRIBI4UIPROC)(GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
GLAPI PFNGLVERTEXATTRIBI4UIPROC glad_glVertexAttribI4ui;
#define glVertexAttribI4ui glad_glVertexAttribI4ui
typedef void (APIENTRYP PFNGLVERTEXATTRIBI1IVPROC)(GLuint index, const GLint *v);
GLAPI PFNGLVERTEXATTRIBI1IVPROC glad_glVertexAttribI1iv;
#define glVertexAttribI1iv glad_glVertexAttribI1iv
typedef void (APIENTRYP PFNGLVERTEXATTRIBI2IVPROC)(GLuint index, const GLint *v);
GLAPI PFNGLVERTEXATTRIBI2IVPROC glad_glVertexAttribI2iv;
#define glVertexAttribI2iv glad_glVertexAttribI2iv
typedef void (APIENTRYP PFNGLVERTEXATTRIBI3IVPROC)(GLuint index, const GLint *v);
GLAPI PFNGLVERTEXATTRIBI3IVPROC glad_glVertexAttribI3iv;
#define glVertexAttribI3iv glad_glVertexAttribI3iv
typedef void (APIENTRYP PFNGLVERTEXATTRIBI4IVPROC)(GLuint index, const GLint *v);
GLAPI PFNGLVERTEXATTRIBI4IVPROC glad_glVertexAttribI4iv;
#define glVertexAttribI4iv glad_glVertexAttribI4iv
typedef void (APIENTRYP PFNGLVERTEXATTRIBI1UIVPROC)(GLuint index, const GLuint *v);
GLAPI PFNGLVERTEXATTRIBI1UIVPROC glad_glVertexAttribI1uiv;
#define glVertexAttribI1uiv glad_glVertexAttribI1uiv
typedef void (APIENTRYP PFNGLVERTEXATTRIBI2UIVPROC)(GLuint index, const GLuint *v);
GLAPI PFNGLVERTEXATTRIBI2UIVPROC glad_glVertexAttribI2uiv;
#define glVertexAttribI2uiv glad_glVertexAttribI2uiv
typedef void (APIENTRYP PFNGLVERTEXATTRIBI3UIVPROC)(GLuint index, const GLuint *v);
GLAPI PFNGLVERTEXATTRIBI3UIVPROC glad_glVertexAttribI3uiv;
#define glVertexAttribI3uiv glad_glVertexAttribI3uiv
typedef void (APIENTRYP PFNGLVERTEXATTRIBI4UIVPROC)(GLuint index, const GLuint *v);
GLAPI PFNGLVERTEXATTRIBI4UIVPROC glad_glVertexAttribI4uiv;
#define glVertexAttribI4uiv glad_glVertexAttribI4uiv
typedef void (APIENTRYP PFNGLVERTEXATTRIBI4BVPROC)(GLuint index, const GLbyte *v);
GLAPI PFNGLVERTEXATTRIBI4BVPROC glad_glVertexAttribI4bv;
#define glVertexAttribI4bv glad_glVertexAttribI4bv
typedef void (APIENTRYP PFNGLVERTEXATTRIBI4SVPROC)(GLuint index, const GLshort *v);
GLAPI PFNGLVERTEXATTRIBI4SVPROC glad_glVertexAttribI4sv;
#define glVertexAttribI4sv glad_glVertexAttribI4sv
typedef void (APIENTRYP PFNGLVERTEXATTRIBI4UBVPROC)(GLuint index, const GLubyte *v);
GLAPI PFNGLVERTEXATTRIBI4UBVPROC glad_glVertexAttribI4ubv;
#define glVertexAttribI4ubv glad_glVertexAttribI4ubv
typedef void (APIENTRYP PFNGLVERTEXATTRIBI4USVPROC)(GLuint index, const GLushort *v);
GLAPI PFNGLVERTEXATTRIBI4USVPROC glad_glVertexAttribI4usv;
#define glVertexAttribI4usv glad_glVertexAttribI4usv
typedef void (APIENTRYP PFNGLGETUNIFORMUIVPROC)(GLuint program, GLint location, GLuint *params);
GLAPI PFNGLGETUNIFORMUIVPROC glad_glGetUniformuiv;
#define glGetUniformuiv glad_glGetUniformuiv
typedef void (APIENTRYP PFNGLBINDFRAGDATALOCATIONPROC)(GLuint program, GLuint color, const GLchar *name);
GLAPI PFNGLBINDFRAGDATALOCATIONPROC glad_glBindFragDataLocation;
#define glBindFragDataLocation glad_glBindFragDataLocation
typedef GLint (APIENTRYP PFNGLGETFRAGDATALOCATIONPROC)(GLuint program, const GLchar *name);
GLAPI PFNGLGETFRAGDATALOCATIONPROC glad_glGetFragDataLocation;
#define glGetFragDataLocation glad_glGetFragDataLocation
typedef void (APIENTRYP PFNGLUNIFORM1UIPROC)(GLint location, GLuint v0);
GLAPI PFNGLUNIFORM1UIPROC glad_glUniform1ui;
#define glUniform1ui glad_glUniform1ui
typedef void (APIENTRYP PFNGLUNIFORM2UIPROC)(GLint location, GLuint v0, GLuint v1);
GLAPI PFNGLUNIFORM2UIPROC glad_glUniform2ui;
#define glUniform2ui glad_glUniform2ui
typedef void (APIENTRYP PFNGLUNIFORM3UIPROC)(GLint location, GLuint v0, GLuint v1, GLuint v2);
GLAPI PFNGLUNIFORM3UIPROC glad_glUniform3ui;
#define glUniform3ui glad_glUniform3ui
typedef void (APIENTRYP PFNGLUNIFORM4UIPROC)(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
GLAPI PFNGLUNIFORM4UIPROC glad_glUniform4ui;
#define glUniform4ui glad_glUniform4ui
typedef void (APIENTRYP PFNGLUNIFORM1UIVPROC)(GLint location, GLsizei count, const GLuint *value);
GLAPI PFNGLUNIFORM1UIVPROC glad_glUniform1uiv;
#define glUniform1uiv glad_glUniform1uiv
typedef void (APIENTRYP PFNGLUNIFORM2UIVPROC)(GLint location, GLsizei count, const GLuint *value);
GLAPI PFNGLUNIFORM2UIVPROC glad_glUniform2uiv;
#define glUniform2uiv glad_glUniform2uiv
typedef void (APIENTRYP PFNGLUNIFORM3UIVPROC)(GLint location, GLsizei count, const GLuint *value);
GLAPI PFNGLUNIFORM3UIVPROC glad_glUniform3uiv;
#define glUniform3uiv glad_glUniform3uiv
typedef void (APIENTRYP PFNGLUNIFORM4UIVPROC)(GLint location, GLsizei count, const GLuint *value);
GLAPI PFNGLUNIFORM4UIVPROC glad_glUniform4uiv;
#define glUniform4uiv glad_glUniform4uiv
typedef void (APIENTRYP PFNGLTEXPARAMETERIIVPROC)(GLenum target, GLenum pname, const GLint *params);
GLAPI PFNGLTEXPARAMETERIIVPROC glad_glTexParameterIiv;
#define glTexParameterIiv glad_glTexParameterIiv
typedef void (APIENTRYP PFNGLTEXPARAMETERIUIVPROC)(GLenum target, GLenum pname, const GLuint *params);
GLAPI PFNGLTEXPARAMETERIUIVPROC glad_glTexParameterIuiv;
#define glTexParameterIuiv glad_glTexParameterIuiv
typedef void (APIENTRYP PFNGLGETTEXPARAMETERIIVPROC)(GLenum target, GLenum pname, GLint *params);
GLAPI PFNGLGETTEXPARAMETERIIVPROC glad_glGetTexParameterIiv;
#define glGetTexParameterIiv glad_glGetTexParameterIiv
typedef void (APIENTRYP PFNGLGETTEXPARAMETERIUIVPROC)(GLenum target, GLenum pname, GLuint *params);
GLAPI PFNGLGETTEXPARAMETERIUIVPROC glad_glGetTexParameterIuiv;
#define glGetTexParameterIuiv glad_glGetTexParameterIuiv
typedef void (APIENTRYP PFNGLCLEARBUFFERIVPROC)(GLenum buffer, GLint drawbuffer, const GLint *value);
GLAPI PFNGLCLEARBUFFERIVPROC glad_glClearBufferiv;
#define glClearBufferiv glad_glClearBufferiv
typedef void (APIENTRYP PFNGLCLEARBUFFERUIVPROC)(GLenum buffer, GLint drawbuffer, const GLuint *value);
GLAPI PFNGLCLEARBUFFERUIVPROC glad_glClearBufferuiv;
#define glClearBufferuiv glad_glClearBufferuiv
typedef void (APIENTRYP PFNGLCLEARBUFFERFVPROC)(GLenum buffer, GLint drawbuffer, const GLfloat *value);
GLAPI PFNGLCLEARBUFFERFVPROC glad_glClearBufferfv;
#define glClearBufferfv glad_glClearBufferfv
typedef void (APIENTRYP PFNGLCLEARBUFFERFIPROC)(GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil);
GLAPI PFNGLCLEARBUFFERFIPROC glad_glClearBufferfi;
#define glClearBufferfi glad_glClearBufferfi
typedef const GLubyte * (APIENTRYP PFNGLGETSTRINGIPROC)(GLenum name, GLuint index);
GLAPI PFNGLGETSTRINGIPROC glad_glGetStringi;
#define glGetStringi glad_glGetStringi
typedef GLboolean (APIENTRYP PFNGLISRENDERBUFFERPROC)(GLuint renderbuffer);
GLAPI PFNGLISRENDERBUFFERPROC glad_glIsRenderbuffer;
#define glIsRenderbuffer glad_glIsRenderbuffer
typedef void (APIENTRYP PFNGLBINDRENDERBUFFERPROC)(GLenum target, GLuint renderbuffer);
GLAPI PFNGLBINDRENDERBUFFERPROC glad_glBindRenderbuffer;
#define glBindRenderbuffer glad_glBindRenderbuffer
typedef void (APIENTRYP PFNGLDELETERENDERBUFFERSPROC)(GLsizei n, const GLuint *renderbuffers);
GLAPI PFNGLDELETERENDERBUFFERSPROC glad_glDeleteRenderbuffers;
#define glDeleteRenderbuffers glad_glDeleteRenderbuffers
typedef void (APIENTRYP PFNGLGENRENDERBUFFERSPROC)(GLsizei n, GLuint *renderbuffers);
GLAPI PFNGLGENRENDERBUFFERSPROC glad_glGenRenderbuffers;
#define glGenRenderbuffers glad_glGenRenderbuffers
typedef void (APIENTRYP PFNGLRENDERBUFFERSTORAGEPROC)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
GLAPI PFNGLRENDERBUFFERSTORAGEPROC glad_glRenderbufferStorage;
#define glRenderbufferStorage glad_glRenderbufferStorage
typedef void (APIENTRYP PFNGLGETRENDERBUFFERPARAMETERIVPROC)(GLenum target, GLenum pname, GLint *params);
GLAPI PFNGLGETRENDERBUFFERPARAMETERIVPROC glad_glGetRenderbufferParameteriv;
#define glGetRenderbufferParameteriv glad_glGetRenderbufferParameteriv
typedef GLboolean (APIENTRYP PFNGLISFRAMEBUFFERPROC)(GLuint framebuffer);
GLAPI PFNGLISFRAMEBUFFERPROC glad_glIsFramebuffer;
#define glIsFramebuffer glad_glIsFramebuffer
typedef void (APIENTRYP PFNGLBINDFRAMEBUFFERPROC)(GLenum target, GLuint framebuffer);
GLAPI PFNGLBINDFRAMEBUFFERPROC glad_glBindFramebuffer;
#define glBindFramebuffer glad_glBindFramebuffer
typedef void (APIENTRYP PFNGLDELETEFRAMEBUFFERSPROC)(GLsizei n, const GLuint *framebuffers);
GLAPI PFNGLDELETEFRAMEBUFFERSPROC glad_glDeleteFramebuffers;
#define glDeleteFramebuffers glad_glDeleteFramebuffers
typedef void (APIENTRYP PFNGLGENFRAMEBUFFERSPROC)(GLsizei n, GLuint *framebuffers);
GLAPI PFNGLGENFRAMEBUFFERSPROC glad_glGenFramebuffers;
#define glGenFramebuffers glad_glGenFramebuffers
typedef GLenum (APIENTRYP PFNGLCHECKFRAMEBUFFERSTATUSPROC)(GLenum target);
GLAPI PFNGLCHECKFRAMEBUFFERSTATUSPROC glad_glCheckFramebufferStatus;
#define glCheckFramebufferStatus glad_glCheckFramebufferStatus
typedef void (APIENTRYP PFNGLFRAMEBUFFERTEXTURE1DPROC)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
GLAPI PFNGLFRAMEBUFFERTEXTURE1DPROC glad_glFramebufferTexture1D;
#define glFramebufferTexture1D glad_glFramebufferTexture1D
typedef void (APIENTRYP PFNGLFRAMEBUFFERTEXTURE2DPROC)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
GLAPI PFNGLFRAMEBUFFERTEXTURE2DPROC glad_glFramebufferTexture2D;
#define glFramebufferTexture2D glad_glFramebufferTexture2D
typedef void (APIENTRYP PFNGLFRAMEBUFFERTEXTURE3DPROC)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
GLAPI PFNGLFRAMEBUFFERTEXTURE3DPROC glad_glFramebufferTexture3D;
#define glFramebufferTexture3D glad_glFramebufferTexture3D
typedef void (APIENTRYP PFNGLFRAMEBUFFERRENDERBUFFERPROC)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
GLAPI PFNGLFRAMEBUFFERRENDERBUFFERPROC glad_glFramebufferRenderbuffer;
#define glFramebufferRenderbuffer glad_glFramebufferRenderbuffer
typedef void (APIENTRYP PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)(GLenum target, GLenum attachment, GLenum pname, GLint *params);
GLAPI PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC glad_glGetFramebufferAttachmentParameteriv;
#define glGetFramebufferAttachmentParameteriv glad_glGetFramebufferAttachmentParameteriv
typedef void (APIENTRYP PFNGLGENERATEMIPMAPPROC)(GLenum target);
GLAPI PFNGLGENERATEMIPMAPPROC glad_glGenerateMipmap;
#define glGenerateMipmap glad_glGenerateMipmap
typedef void (APIENTRYP PFNGLBLITFRAMEBUFFERPROC)(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
GLAPI PFNGLBLITFRAMEBUFFERPROC glad_glBlitFramebuffer;
#define glBlitFramebuffer glad_glBlitFramebuffer
typedef void (APIENTRYP PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
GLAPI PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC glad_glRenderbufferStorageMultisample;
#define glRenderbufferStorageMultisample glad_glRenderbufferStorageMultisample
typedef void (APIENTRYP PFNGLFRAMEBUFFERTEXTURELAYERPROC)(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
GLAPI PFNGLFRAMEBUFFERTEXTURELAYERPROC glad_glFramebufferTextureLayer;
#define glFramebufferTextureLayer glad_glFramebufferTextureLayer
typedef void * (APIENTRYP PFNGLMAPBUFFERRANGEPROC)(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
GLAPI PFNGLMAPBUFFERRANGEPROC glad_glMapBufferRange;
#define glMapBufferRange glad_glMapBufferRange
typedef void (APIENTRYP PFNGLFLUSHMAPPEDBUFFERRANGEPROC)(GLenum target, GLintptr offset, GLsizeiptr length);
GLAPI PFNGLFLUSHMAPPEDBUFFERRANGEPROC glad_glFlushMappedBufferRange;
#define glFlushMappedBufferRange glad_glFlushMappedBufferRange
typedef void (APIENTRYP PFNGLBINDVERTEXARRAYPROC)(GLuint array);
GLAPI PFNGLBINDVERTEXARRAYPROC glad_glBindVertexArray;
#define glBindVertexArray glad_glBindVertexArray
typedef void (APIENTRYP PFNGLDELETEVERTEXARRAYSPROC)(GLsizei n, const GLuint *arrays);
GLAPI PFNGLDELETEVERTEXARRAYSPROC glad_glDeleteVertexArrays;
#define glDeleteVertexArrays glad_glDeleteVertexArrays
typedef void (APIENTRYP PFNGLGENVERTEXARRAYSPROC)(GLsizei n, GLuint *arrays);
GLAPI PFNGLGENVERTEXARRAYSPROC glad_glGenVertexArrays;
#define glGenVertexArrays glad_glGenVertexArrays
typedef GLboolean (APIENTRYP PFNGLISVERTEXARRAYPROC)(GLuint array);
GLAPI PFNGLISVERTEXARRAYPROC glad_glIsVertexArray;
#define glIsVertexArray glad_glIsVertexArray
#endif
#ifndef GL_VERSION_3_1
#define GL_VERSION_3_1 1
GLAPI int GLAD_GL_VERSION_3_1;
typedef void (APIENTRYP PFNGLDRAWARRAYSINSTANCEDPROC)(GLenum mode, GLint first, GLsizei count, GLsizei instancecount);
GLAPI PFNGLDRAWARRAYSINSTANCEDPROC glad_glDrawArraysInstanced;
#define glDrawArraysInstanced glad_glDrawArraysInstanced
typedef void (APIENTRYP PFNGLDRAWELEMENTSINSTANCEDPROC)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount);
GLAPI PFNGLDRAWELEMENTSINSTANCEDPROC glad_glDrawElementsInstanced;
#define glDrawElementsInstanced glad_glDrawElementsInstanced
typedef void (APIENTRYP PFNGLTEXBUFFERPROC)(GLenum target, GLenum internalformat, GLuint buffer);
GLAPI PFNGLTEXBUFFERPROC glad_glTexBuffer;
#define glTexBuffer glad_glTexBuffer
typedef void (APIENTRYP PFNGLPRIMITIVERESTARTINDEXPROC)(GLuint index);
GLAPI PFNGLPRIMITIVERESTARTINDEXPROC glad_glPrimitiveRestartIndex;
#define glPrimitiveRestartIndex glad_glPrimitiveRestartIndex
typedef void (APIENTRYP PFNGLCOPYBUFFERSUBDATAPROC)(GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
GLAPI PFNGLCOPYBUFFERSUBDATAPROC glad_glCopyBufferSubData;
#define glCopyBufferSubData glad_glCopyBufferSubData
typedef void (APIENTRYP PFNGLGETUNIFORMINDICESPROC)(GLuint program, GLsizei uniformCount, const GLchar *const*uniformNames, GLuint *uniformIndices);
GLAPI PFNGLGETUNIFORMINDICESPROC glad_glGetUniformIndices;
#define glGetUniformIndices glad_glGetUniformIndices
typedef void (APIENTRYP PFNGLGETACTIVEUNIFORMSIVPROC)(GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params);
GLAPI PFNGLGETACTIVEUNIFORMSIVPROC glad_glGetActiveUniformsiv;
#define glGetActiveUniformsiv glad_glGetActiveUniformsiv
typedef void (APIENTRYP PFNGLGETACTIVEUNIFORMNAMEPROC)(GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformName);
GLAPI PFNGLGETACTIVEUNIFORMNAMEPROC glad_glGetActiveUniformName;
#define glGetActiveUniformName glad_glGetActiveUniformName
typedef GLuint (APIENTRYP PFNGLGETUNIFORMBLOCKINDEXPROC)(GLuint program, const GLchar *uniformBlockName);
GLAPI PFNGLGETUNIFORMBLOCKINDEXPROC glad_glGetUniformBlockIndex;
#define glGetUniformBlockIndex glad_glGetUniformBlockIndex
typedef void (APIENTRYP PFNGLGETACTIVEUNIFORMBLOCKIVPROC)(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params);
GLAPI PFNGLGETACTIVEUNIFORMBLOCKIVPROC glad_glGetActiveUniformBlockiv;
#define glGetActiveUniformBlockiv glad_glGetActiveUniformBlockiv
typedef void (APIENTRYP PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC)(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName);
GLAPI PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC glad_glGetActiveUniformBlockName;
#define glGetActiveUniformBlockName glad_glGetActiveUniformBlockName
typedef void (APIENTRYP PFNGLUNIFORMBLOCKBINDINGPROC)(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);
GLAPI PFNGLUNIFORMBLOCKBINDINGPROC glad_glUniformBlockBinding;
#define glUniformBlockBinding glad_glUniformBlockBinding
#endif
#ifndef GL_VERSION_3_2
#define GL_VERSION_3_2 1
GLAPI int GLAD_GL_VERSION_3_2;
typedef void (APIENTRYP PFNGLDRAWELEMENTSBASEVERTEXPROC)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLint basevertex);
GLAPI PFNGLDRAWELEMENTSBASEVERTEXPROC glad_glDrawElementsBaseVertex;
#define glDrawElementsBaseVertex glad_glDrawElementsBaseVertex
typedef void (APIENTRYP PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices, GLint basevertex);
GLAPI PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC glad_glDrawRangeElementsBaseVertex;
#define glDrawRangeElementsBaseVertex glad_glDrawRangeElementsBaseVertex
typedef void (APIENTRYP PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLint basevertex);
GLAPI PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC glad_glDrawElementsInstancedBaseVertex;
#define glDrawElementsInstancedBaseVertex glad_glDrawElementsInstancedBaseVertex
typedef void (APIENTRYP PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC)(GLenum mode, const GLsizei *count, GLenum type, const void *const*indices, GLsizei drawcount, const GLint *basevertex);
GLAPI PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC glad_glMultiDrawElementsBaseVertex;
#define glMultiDrawElementsBaseVertex glad_glMultiDrawElementsBaseVertex
typedef void (APIENTRYP PFNGLPROVOKINGVERTEXPROC)(GLenum mode);
GLAPI PFNGLPROVOKINGVERTEXPROC glad_glProvokingVertex;
#define glProvokingVertex glad_glProvokingVertex
typedef GLsync (APIENTRYP PFNGLFENCESYNCPROC)(GLenum condition, GLbitfield flags);
GLAPI PFNGLFENCESYNCPROC glad_glFenceSync;
#define glFenceSync glad_glFenceSync
typedef GLboolean (APIENTRYP PFNGLISSYNCPROC)(GLsync sync);
GLAPI PFNGLISSYNCPROC glad_glIsSync;
#define glIsSync glad_glIsSync
typedef void (APIENTRYP PFNGLDELETESYNCPROC)(GLsync sync);
GLAPI PFNGLDELETESYNCPROC glad_glDeleteSync;
#define glDeleteSync glad_glDeleteSync
typedef GLenum (APIENTRYP PFNGLCLIENTWAITSYNCPROC)(GLsync sync, GLbitfield flags, GLuint64 timeout);
GLAPI PFNGLCLIENTWAITSYNCPROC glad_glClientWaitSync;
#define glClientWaitSync glad_glClientWaitSync
typedef void (APIENTRYP PFNGLWAITSYNCPROC)(GLsync sync, GLbitfield flags, GLuint64 timeout);
GLAPI PFNGLWAITSYNCPROC glad_glWaitSync;
#define glWaitSync glad_glWaitSync
typedef void (APIENTRYP PFNGLGETINTEGER64VPROC)(GLenum pname, GLint64 *data);
GLAPI PFNGLGETINTEGER64VPROC glad_glGetInteger64v;
#define glGetInteger64v glad_glGetInteger64v
typedef void (APIENTRYP PFNGLGETSYNCIVPROC)(GLsync sync, GLenum pname, GLsizei count, GLsizei *length, GLint *values);
GLAPI PFNGLGETSYNCIVPROC glad_glGetSynciv;
#define glGetSynciv glad_glGetSynciv
typedef void (APIENTRYP PFNGLGETINTEGER64I_VPROC)(GLenum target, GLuint index, GLint64 *data);
GLAPI PFNGLGETINTEGER64I_VPROC glad_glGetInteger64i_v;
#define glGetInteger64i_v glad_glGetInteger64i_v
typedef void (APIENTRYP PFNGLGETBUFFERPARAMETERI64VPROC)(GLenum target, GLenum pname, GLint64 *params);
GLAPI PFNGLGETBUFFERPARAMETERI64VPROC glad_glGetBufferParameteri64v;
#define glGetBufferParameteri64v glad_glGetBufferParameteri64v
typedef void (APIENTRYP PFNGLFRAMEBUFFERTEXTUREPROC)(GLenum target, GLenum attachment, GLuint texture, GLint level);
GLAPI PFNGLFRAMEBUFFERTEXTUREPROC glad_glFramebufferTexture;
#define glFramebufferTexture glad_glFramebufferTexture
typedef void (APIENTRYP PFNGLTEXIMAGE2DMULTISAMPLEPROC)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
GLAPI PFNGLTEXIMAGE2DMULTISAMPLEPROC glad_glTexImage2DMultisample;
#define glTexImage2DMultisample glad_glTexImage2DMultisample
typedef void (APIENTRYP PFNGLTEXIMAGE3DMULTISAMPLEPROC)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
GLAPI PFNGLTEXIMAGE3DMULTISAMPLEPROC glad_glTexImage3DMultisample;
#define glTexImage3DMultisample glad_glTexImage3DMultisample
typedef void (APIENTRYP PFNGLGETMULTISAMPLEFVPROC)(GLenum pname, GLuint index, GLfloat *val);
GLAPI PFNGLGETMULTISAMPLEFVPROC glad_glGetMultisamplefv;
#define glGetMultisamplefv glad_glGetMultisamplefv
typedef void (APIENTRYP PFNGLSAMPLEMASKIPROC)(GLuint maskNumber, GLbitfield mask);
GLAPI PFNGLSAMPLEMASKIPROC glad_glSampleMaski;
#define glSampleMaski glad_glSampleMaski
#endif
#ifndef GL_VERSION_3_3
#define GL_VERSION_3_3 1
GLAPI int GLAD_GL_VERSION_3_3;
typedef void (APIENTRYP PFNGLBINDFRAGDATALOCATIONINDEXEDPROC)(GLuint program, GLuint colorNumber, GLuint index, const GLchar *name);
GLAPI PFNGLBINDFRAGDATALOCATIONINDEXEDPROC glad_glBindFragDataLocationIndexed;
#define glBindFragDataLocationIndexed glad_glBindFragDataLocationIndexed
typedef GLint (APIENTRYP PFNGLGETFRAGDATAINDEXPROC)(GLuint program, const GLchar *name);
GLAPI PFNGLGETFRAGDATAINDEXPROC glad_glGetFragDataIndex;
#define glGetFragDataIndex glad_glGetFragDataIndex
typedef void (APIENTRYP PFNGLGENSAMPLERSPROC)(GLsizei count, GLuint *samplers);
GLAPI PFNGLGENSAMPLERSPROC glad_glGenSamplers;
#define glGenSamplers glad_glGenSamplers
typedef void (APIENTRYP PFNGLDELETESAMPLERSPROC)(GLsizei count, const GLuint *samplers);
GLAPI PFNGLDELETESAMPLERSPROC glad_glDeleteSamplers;
#define glDeleteSamplers glad_glDeleteSamplers
typedef GLboolean (APIENTRYP PFNGLISSAMPLERPROC)(GLuint sampler);
GLAPI PFNGLISSAMPLERPROC glad_glIsSampler;
#define glIsSampler glad_glIsSampler
typedef void (APIENTRYP PFNGLBINDSAMPLERPROC)(GLuint unit, GLuint sampler);
GLAPI PFNGLBINDSAMPLERPROC glad_glBindSampler;
#define glBindSampler glad_glBindSampler
typedef void (APIENTRYP PFNGLSAMPLERPARAMETERIPROC)(GLuint sampler, GLenum pname, GLint param);
GLAPI PFNGLSAMPLERPARAMETERIPROC glad_glSamplerParameteri;
#define glSamplerParameteri glad_glSamplerParameteri
typedef void (APIENTRYP PFNGLSAMPLERPARAMETERIVPROC)(GLuint sampler, GLenum pname, const GLint *param);
GLAPI PFNGLSAMPLERPARAMETERIVPROC glad_glSamplerParameteriv;
#define glSamplerParameteriv glad_glSamplerParameteriv
typedef void (APIENTRYP PFNGLSAMPLERPARAMETERFPROC)(GLuint sampler, GLenum pname, GLfloat param);
GLAPI PFNGLSAMPLERPARAMETERFPROC glad_glSamplerParameterf;
#define glSamplerParameterf glad_glSamplerParameterf
typedef void (APIENTRYP PFNGLSAMPLERPARAMETERFVPROC)(GLuint sampler, GLenum pname, const GLfloat *param);
GLAPI PFNGLSAMPLERPARAMETERFVPROC glad_glSamplerParameterfv;
#define glSamplerParameterfv glad_glSamplerParameterfv
typedef void (APIENTRYP PFNGLSAMPLERPARAMETERIIVPROC)(GLuint sampler, GLenum pname, const GLint *param);
GLAPI PFNGLSAMPLERPARAMETERIIVPROC glad_glSamplerParameterIiv;
#define glSamplerParameterIiv glad_glSamplerParameterIiv
typedef void (APIENTRYP PFNGLSAMPLERPARAMETERIUIVPROC)(GLuint sampler, GLenum pname, const GLuint *param);
GLAPI PFNGLSAMPLERPARAMETERIUIVPROC glad_glSamplerParameterIuiv;
#define glSamplerParameterIuiv glad_glSamplerParameterIuiv
typedef void (APIENTRYP PFNGLGETSAMPLERPARAMETERIVPROC)(GLuint sampler, GLenum pname, GLint *params);
GLAPI PFNGLGETSAMPLERPARAMETERIVPROC glad_glGetSamplerParameteriv;
#define glGetSamplerParameteriv glad_glGetSamplerParameteriv
typedef void (APIENTRYP PFNGLGETSAMPLERPARAMETERIIVPROC)(GLuint sampler, GLenum pname, GLint *params);
GLAPI PFNGLGETSAMPLERPARAMETERIIVPROC glad_glGetSamplerParameterIiv;
#define glGetSamplerParameterIiv glad_glGetSamplerParameterIiv
typedef void (APIENTRYP PFNGLGETSAMPLERPARAMETERFVPROC)(GLuint sampler, GLenum pname, GLfloat *params);
GLAPI PFNGLGETSAMPLERPARAMETERFVPROC glad_glGetSamplerParameterfv;
#define glGetSamplerParameterfv glad_glGetSamplerParameterfv
typedef void (APIENTRYP PFNGLGETSAMPLERPARAMETERIUIVPROC)(GLuint sampler, GLenum pname, GLuint *params);
GLAPI PFNGLGETSAMPLERPARAMETERIUIVPROC glad_glGetSamplerParameterIuiv;
#define glGetSamplerParameterIuiv glad_glGetSamplerParameterIuiv
typedef void (APIENTRYP PFNGLQUERYCOUNTERPROC)(GLuint id, GLenum target);
GLAPI PFNGLQUERYCOUNTERPROC glad_glQueryCounter;
#define glQueryCounter glad_glQueryCounter
typedef void (APIENTRYP PFNGLGETQUERYOBJECTI64VPROC)(GLuint id, GLenum pname, GLint64 *params);
GLAPI PFNGLGETQUERYOBJECTI64VPROC glad_glGetQueryObjecti64v;
#define glGetQueryObjecti64v glad_glGetQueryObjecti64v
typedef void (APIENTRYP PFNGLGETQUERYOBJECTUI64VPROC)(GLuint id, GLenum pname, GLuint64 *params);
GLAPI PFNGLGETQUERYOBJECTUI64VPROC glad_glGetQueryObjectui64v;
#define glGetQueryObjectui64v glad_glGetQueryObjectui64v
typedef void (APIENTRYP PFNGLVERTEXATTRIBDIVISORPROC)(GLuint index, GLuint divisor);
GLAPI PFNGLVERTEXATTRIBDIVISORPROC glad_glVertexAttribDivisor;
#define glVertexAttribDivisor glad_glVertexAttribDivisor
typedef void (APIENTRYP PFNGLVERTEXATTRIBP1UIPROC)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
GLAPI PFNGLVERTEXATTRIBP1UIPROC glad_glVertexAttribP1ui;
#define glVertexAttribP1ui glad_glVertexAttribP1ui
typedef void (APIENTRYP PFNGLVERTEXATTRIBP1UIVPROC)(GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
GLAPI PFNGLVERTEXATTRIBP1UIVPROC glad_glVertexAttribP1uiv;
#define glVertexAttribP1uiv glad_glVertexAttribP1uiv
typedef void (APIENTRYP PFNGLVERTEXATTRIBP2UIPROC)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
GLAPI PFNGLVERTEXATTRIBP2UIPROC glad_glVertexAttribP2ui;
#define glVertexAttribP2ui glad_glVertexAttribP2ui
typedef void (APIENTRYP PFNGLVERTEXATTRIBP2UIVPROC)(GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
GLAPI PFNGLVERTEXATTRIBP2UIVPROC glad_glVertexAttribP2uiv;
#define glVertexAttribP2uiv glad_glVertexAttribP2uiv
typedef void (APIENTRYP PFNGLVERTEXATTRIBP3UIPROC)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
GLAPI PFNGLVERTEXATTRIBP3UIPROC glad_glVertexAttribP3ui;
#define glVertexAttribP3ui glad_glVertexAttribP3ui
typedef void (APIENTRYP PFNGLVERTEXATTRIBP3UIVPROC)(GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
GLAPI PFNGLVERTEXATTRIBP3UIVPROC glad_glVertexAttribP3uiv;
#define glVertexAttribP3uiv glad_glVertexAttribP3uiv
typedef void (APIENTRYP PFNGLVERTEXATTRIBP4UIPROC)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
GLAPI PFNGLVERTEXATTRIBP4UIPROC glad_glVertexAttribP4ui;
#define glVertexAttribP4ui glad_glVertexAttribP4ui
typedef void (APIENTRYP PFNGLVERTEXATTRIBP4UIVPROC)(GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
GLAPI PFNGLVERTEXATTRIBP4UIVPROC glad_glVertexAttribP4uiv;
#define glVertexAttribP4uiv glad_glVertexAttribP4uiv
typedef void (APIENTRYP PFNGLVERTEXP2UIPROC)(GLenum type, GLuint value);
GLAPI PFNGLVERTEXP2UIPROC glad_glVertexP2ui;
#define glVertexP2ui glad_glVertexP2ui
typedef void (APIENTRYP PFNGLVERTEXP2UIVPROC)(GLenum type, const GLuint *value);
GLAPI PFNGLVERTEXP2UIVPROC glad_glVertexP2uiv;
#define glVertexP2uiv glad_glVertexP2uiv
typedef void (APIENTRYP PFNGLVERTEXP3UIPROC)(GLenum type, GLuint value);
GLAPI PFNGLVERTEXP3UIPROC glad_glVertexP3ui;
#define glVertexP3ui glad_glVertexP3ui
typedef void (APIENTRYP PFNGLVERTEXP3UIVPROC)(GLenum type, const GLuint *value);
GLAPI PFNGLVERTEXP3UIVPROC glad_glVertexP3uiv;
#define glVertexP3uiv glad_glVertexP3uiv
typedef void (APIENTRYP PFNGLVERTEXP4UIPROC)(GLenum type, GLuint value);
GLAPI PFNGLVERTEXP4UIPROC glad_glVertexP4ui;
#define glVertexP4ui glad_glVertexP4ui
typedef void (APIENTRYP PFNGLVERTEXP4UIVPROC)(GLenum type, const GLuint *value);
GLAPI PFNGLVERTEXP4UIVPROC glad_glVertexP4uiv;
#define glVertexP4uiv glad_glVertexP4uiv
typedef void (APIENTRYP PFNGLTEXCOORDP1UIPROC)(GLenum type, GLuint coords);
GLAPI PFNGLTEXCOORDP1UIPROC glad_glTexCoordP1ui;
#define glTexCoordP1ui glad_glTexCoordP1ui
typedef void (APIENTRYP PFNGLTEXCOORDP1UIVPROC)(GLenum type, const GLuint *coords);
GLAPI PFNGLTEXCOORDP1UIVPROC glad_glTexCoordP1uiv;
#define glTexCoordP1uiv glad_glTexCoordP1uiv
typedef void (APIENTRYP PFNGLTEXCOORDP2UIPROC)(GLenum type, GLuint coords);
GLAPI PFNGLTEXCOORDP2UIPROC glad_glTexCoordP2ui;
#define glTexCoordP2ui glad_glTexCoordP2ui
typedef void (APIENTRYP PFNGLTEXCOORDP2UIVPROC)(GLenum type, const GLuint *coords);
GLAPI PFNGLTEXCOORDP2UIVPROC glad_glTexCoordP2uiv;
#define glTexCoordP2uiv glad_glTexCoordP2uiv
typedef void (APIENTRYP PFNGLTEXCOORDP3UIPROC)(GLenum type, GLuint coords);
GLAPI PFNGLTEXCOORDP3UIPROC glad_glTexCoordP3ui;
#define glTexCoordP3ui glad_glTexCoordP3ui
typedef void (APIENTRYP PFNGLTEXCOORDP3UIVPROC)(GLenum type, const GLuint *coords);
GLAPI PFNGLTEXCOORDP3UIVPROC glad_glTexCoordP3uiv;
#define glTexCoordP3uiv glad_glTexCoordP3uiv
typedef void (APIENTRYP PFNGLTEXCOORDP4UIPROC)(GLenum type, GLuint coords);
GLAPI PFNGLTEXCOORDP4UIPROC glad_glTexCoordP4ui;
#define glTexCoordP4ui glad_glTexCoordP4ui
typedef void (APIENTRYP PFNGLTEXCOORDP4UIVPROC)(GLenum type, const GLuint *coords);
GLAPI PFNGLTEXCOORDP4UIVPROC glad_glTexCoordP4uiv;
#define glTexCoordP4uiv glad_glTexCoordP4uiv
typedef void (APIENTRYP PFNGLMULTITEXCOORDP1UIPROC)(GLenum texture, GLenum type, GLuint coords);
GLAPI PFNGLMULTITEXCOORDP1UIPROC glad_glMultiTexCoordP1ui;
#define glMultiTexCoordP1ui glad_glMultiTexCoordP1ui
typedef void (APIENTRYP PFNGLMULTITEXCOORDP1UIVPROC)(GLenum texture, GLenum type, const GLuint *coords);
GLAPI PFNGLMULTITEXCOORDP1UIVPROC glad_glMultiTexCoordP1uiv;
#define glMultiTexCoordP1uiv glad_glMultiTexCoordP1uiv
typedef void (APIENTRYP PFNGLMULTITEXCOORDP2UIPROC)(GLenum texture, GLenum type, GLuint coords);
GLAPI PFNGLMULTITEXCOORDP2UIPROC glad_glMultiTexCoordP2ui;
#define glMultiTexCoordP2ui glad_glMultiTexCoordP2ui
typedef void (APIENTRYP PFNGLMULTITEXCOORDP2UIVPROC)(GLenum texture, GLenum type, const GLuint *coords);
GLAPI PFNGLMULTITEXCOORDP2UIVPROC glad_glMultiTexCoordP2uiv;
#define glMultiTexCoordP2uiv glad_glMultiTexCoordP2uiv
typedef void (APIENTRYP PFNGLMULTITEXCOORDP3UIPROC)(GLenum texture, GLenum type, GLuint coords);
GLAPI PFNGLMULTITEXCOORDP3UIPROC glad_glMultiTexCoordP3ui;
#define glMultiTexCoordP3ui glad_glMultiTexCoordP3ui
typedef void (APIENTRYP PFNGLMULTITEXCOORDP3UIVPROC)(GLenum texture, GLenum type, const GLuint *coords);
GLAPI PFNGLMULTITEXCOORDP3UIVPROC glad_glMultiTexCoordP3uiv;
#define glMultiTexCoordP3uiv glad_glMultiTexCoordP3uiv
typedef void (APIENTRYP PFNGLMULTITEXCOORDP4UIPROC)(GLenum texture, GLenum type, GLuint coords);
GLAPI PFNGLMULTITEXCOORDP4UIPROC glad_glMultiTexCoordP4ui;
#define glMultiTexCoordP4ui glad_glMultiTexCoordP4ui
typedef void (APIENTRYP PFNGLMULTITEXCOORDP4UIVPROC)(GLenum texture, GLenum type, const GLuint *coords);
GLAPI PFNGLMULTITEXCOORDP4UIVPROC glad_glMultiTexCoordP4uiv;
#define glMultiTexCoordP4uiv glad_glMultiTexCoordP4uiv
typedef void (APIENTRYP PFNGLNORMALP3UIPROC)(GLenum type, GLuint coords);
GLAPI PFNGLNORMALP3UIPROC glad_glNormalP3ui;
#define glNormalP3ui glad_glNormalP3ui
typedef void (APIENTRYP PFNGLNORMALP3UIVPROC)(GLenum type, const GLuint *coords);
GLAPI PFNGLNORMALP3UIVPROC glad_glNormalP3uiv;
#define glNormalP3uiv glad_glNormalP3uiv
typedef void (APIENTRYP PFNGLCOLORP3UIPROC)(GLenum type, GLuint color);
GLAPI PFNGLCOLORP3UIPROC glad_glColorP3ui;
#define glColorP3ui glad_glColorP3ui
typedef void (APIENTRYP PFNGLCOLORP3UIVPROC)(GLenum type, const GLuint *color);
GLAPI PFNGLCOLORP3UIVPROC glad_glColorP3uiv;
#define glColorP3uiv glad_glColorP3uiv
typedef void (APIENTRYP PFNGLCOLORP4UIPROC)(GLenum type, GLuint color);
GLAPI PFNGLCOLORP4UIPROC glad_glColorP4ui;
#define glColorP4ui glad_glColorP4ui
typedef void (APIENTRYP PFNGLCOLORP4UIVPROC)(GLenum type, const GLuint *color);
GLAPI PFNGLCOLORP4UIVPROC glad_glColorP4uiv;
#define glColorP4uiv glad_glColorP4uiv
typedef void (APIENTRYP PFNGLSECONDARYCOLORP3UIPROC)(GLenum type, GLuint color);
GLAPI PFNGLSECONDARYCOLORP3UIPROC glad_glSecondaryColorP3ui;
#define glSecondaryColorP3ui glad_glSecondaryColorP3ui
typedef void (APIENTRYP PFNGLSECONDARYCOLORP3UIVPROC)(GLenum type, const GLuint *color);
GLAPI PFNGLSECONDARYCOLORP3UIVPROC glad_glSecondaryColorP3uiv;
#define glSecondaryColorP3uiv glad_glSecondaryColorP3uiv
#endif

#ifdef __cplusplus
}
#endif

#endif

/*************************************************************************
 * GLFW 3.3 - www.glfw.org
 * A library for OpenGL, window and input
 *------------------------------------------------------------------------
 * Copyright (c) 2002-2006 Marcus Geelnard
 * Copyright (c) 2006-2019 Camilla Löwy <elmindreda@glfw.org>
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would
 *    be appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not
 *    be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 *    distribution.
 *
 *************************************************************************/

#ifndef _glfw3_h_
#define _glfw3_h_

#ifdef __cplusplus
extern "C" {
#endif


/*************************************************************************
 * Doxygen documentation
 *************************************************************************/

/*! @file glfw3.h
 *  @brief The header of the GLFW 3 API.
 *
 *  This is the header file of the GLFW 3 API.  It defines all its types and
 *  declares all its functions.
 *
 *  For more information about how to use this file, see @ref build_include.
 */
/*! @defgroup context Context reference
 *  @brief Functions and types related to OpenGL and OpenGL ES contexts.
 *
 *  This is the reference documentation for OpenGL and OpenGL ES context related
 *  functions.  For more task-oriented information, see the @ref context_guide.
 */
/*! @defgroup vulkan Vulkan support reference
 *  @brief Functions and types related to Vulkan.
 *
 *  This is the reference documentation for Vulkan related functions and types.
 *  For more task-oriented information, see the @ref vulkan_guide.
 */
/*! @defgroup init Initialization, version and error reference
 *  @brief Functions and types related to initialization and error handling.
 *
 *  This is the reference documentation for initialization and termination of
 *  the library, version management and error handling.  For more task-oriented
 *  information, see the @ref intro_guide.
 */
/*! @defgroup input Input reference
 *  @brief Functions and types related to input handling.
 *
 *  This is the reference documentation for input related functions and types.
 *  For more task-oriented information, see the @ref input_guide.
 */
/*! @defgroup monitor Monitor reference
 *  @brief Functions and types related to monitors.
 *
 *  This is the reference documentation for monitor related functions and types.
 *  For more task-oriented information, see the @ref monitor_guide.
 */
/*! @defgroup window Window reference
 *  @brief Functions and types related to windows.
 *
 *  This is the reference documentation for window related functions and types,
 *  including creation, deletion and event polling.  For more task-oriented
 *  information, see the @ref window_guide.
 */


/*************************************************************************
 * Compiler- and platform-specific preprocessor work
 *************************************************************************/

/* If we are we on Windows, we want a single define for it.
 */
#if !defined(_WIN32) && (defined(__WIN32__) || defined(WIN32) || defined(__MINGW32__))
 #define _WIN32
#endif /* _WIN32 */

/* Include because most Windows GLU headers need wchar_t and
 * the macOS OpenGL header blocks the definition of ptrdiff_t by glext.h.
 * Include it unconditionally to avoid surprising side-effects.
 */
#include <stddef.h>

/* Include because it is needed by Vulkan and related functions.
 * Include it unconditionally to avoid surprising side-effects.
 */
#include <stdint.h>

#if defined(GLFW_INCLUDE_VULKAN)
  #include <vulkan/vulkan.h>
#endif /* Vulkan header */

/* The Vulkan header may have indirectly included windows.h (because of
 * VK_USE_PLATFORM_WIN32_KHR) so we offer our replacement symbols after it.
 */

/* It is customary to use APIENTRY for OpenGL function pointer declarations on
 * all platforms.  Additionally, the Windows OpenGL header needs APIENTRY.
 */
#if !defined(APIENTRY)
 #if defined(_WIN32)
  #define APIENTRY __stdcall
 #else
  #define APIENTRY
 #endif
 #define GLFW_APIENTRY_DEFINED
#endif /* APIENTRY */

/* Some Windows OpenGL headers need this.
 */
#if !defined(WINGDIAPI) && defined(_WIN32)
 #define WINGDIAPI __declspec(dllimport)
 #define GLFW_WINGDIAPI_DEFINED
#endif /* WINGDIAPI */

/* Some Windows GLU headers need this.
 */
#if !defined(CALLBACK) && defined(_WIN32)
 #define CALLBACK __stdcall
 #define GLFW_CALLBACK_DEFINED
#endif /* CALLBACK */

/* Include the chosen OpenGL or OpenGL ES headers.
 */
#if defined(GLFW_INCLUDE_ES1)

 #include <GLES/gl.h>
 #if defined(GLFW_INCLUDE_GLEXT)
  #include <GLES/glext.h>
 #endif

#elif defined(GLFW_INCLUDE_ES2)

 #include <GLES2/gl2.h>
 #if defined(GLFW_INCLUDE_GLEXT)
  #include <GLES2/gl2ext.h>
 #endif

#elif defined(GLFW_INCLUDE_ES3)

 #include <GLES3/gl3.h>
 #if defined(GLFW_INCLUDE_GLEXT)
  #include <GLES2/gl2ext.h>
 #endif

#elif defined(GLFW_INCLUDE_ES31)

 #include <GLES3/gl31.h>
 #if defined(GLFW_INCLUDE_GLEXT)
  #include <GLES2/gl2ext.h>
 #endif

#elif defined(GLFW_INCLUDE_ES32)

 #include <GLES3/gl32.h>
 #if defined(GLFW_INCLUDE_GLEXT)
  #include <GLES2/gl2ext.h>
 #endif

#elif defined(GLFW_INCLUDE_GLCOREARB)

 #if defined(__APPLE__)

  #include <OpenGL/gl3.h>
  #if defined(GLFW_INCLUDE_GLEXT)
   #include <OpenGL/gl3ext.h>
  #endif /*GLFW_INCLUDE_GLEXT*/

 #else /*__APPLE__*/

  #include <GL/glcorearb.h>
  #if defined(GLFW_INCLUDE_GLEXT)
   #include <GL/glext.h>
  #endif

 #endif /*__APPLE__*/

#elif defined(GLFW_INCLUDE_GLU)

 #if defined(__APPLE__)

  #if defined(GLFW_INCLUDE_GLU)
   #include <OpenGL/glu.h>
  #endif

 #else /*__APPLE__*/

  #if defined(GLFW_INCLUDE_GLU)
   #include <GL/glu.h>
  #endif

 #endif /*__APPLE__*/

#elif !defined(GLFW_INCLUDE_NONE) && \
      !defined(__gl_h_) && \
      !defined(__gles1_gl_h_) && \
      !defined(__gles2_gl2_h_) && \
      !defined(__gles2_gl3_h_) && \
      !defined(__gles2_gl31_h_) && \
      !defined(__gles2_gl32_h_) && \
      !defined(__gl_glcorearb_h_) && \
      !defined(__gl2_h_) /*legacy*/ && \
      !defined(__gl3_h_) /*legacy*/ && \
      !defined(__gl31_h_) /*legacy*/ && \
      !defined(__gl32_h_) /*legacy*/ && \
      !defined(__glcorearb_h_) /*legacy*/ && \
      !defined(__GL_H__) /*non-standard*/ && \
      !defined(__gltypes_h_) /*non-standard*/ && \
      !defined(__glee_h_) /*non-standard*/

 #if defined(__APPLE__)

  #if !defined(GLFW_INCLUDE_GLEXT)
   #define GL_GLEXT_LEGACY
  #endif
  #include <OpenGL/gl.h>

 #else /*__APPLE__*/

  #include <GL/gl.h>
  #if defined(GLFW_INCLUDE_GLEXT)
   #include <GL/glext.h>
  #endif

 #endif /*__APPLE__*/

#endif /* OpenGL and OpenGL ES headers */

#if defined(GLFW_DLL) && defined(_GLFW_BUILD_DLL)
 /* GLFW_DLL must be defined by applications that are linking against the DLL
  * version of the GLFW library.  _GLFW_BUILD_DLL is defined by the GLFW
  * configuration header when compiling the DLL version of the library.
  */
 #error "You must not have both GLFW_DLL and _GLFW_BUILD_DLL defined"
#endif

/* GLFWAPI is used to declare public API functions for export
 * from the DLL / shared library / dynamic library.
 */
#if defined(_WIN32) && defined(_GLFW_BUILD_DLL)
 /* We are building GLFW as a Win32 DLL */
 #define GLFWAPI __declspec(dllexport)
#elif defined(_WIN32) && defined(GLFW_DLL)
 /* We are calling a GLFW Win32 DLL */
 #define GLFWAPI __declspec(dllimport)
#elif defined(__GNUC__) && defined(_GLFW_BUILD_DLL)
 /* We are building GLFW as a Unix shared library */
 #define GLFWAPI __attribute__((visibility("default")))
#else
 #define GLFWAPI
#endif


/*************************************************************************
 * GLFW API tokens
 *************************************************************************/

/*! @name GLFW version macros
 *  @{ */
/*! @brief The major version number of the GLFW header.
 *
 *  The major version number of the GLFW header.  This is incremented when the
 *  API is changed in non-compatible ways.
 *  @ingroup init
 */
#define GLFW_VERSION_MAJOR          3
/*! @brief The minor version number of the GLFW header.
 *
 *  The minor version number of the GLFW header.  This is incremented when
 *  features are added to the API but it remains backward-compatible.
 *  @ingroup init
 */
#define GLFW_VERSION_MINOR          3
/*! @brief The revision number of the GLFW header.
 *
 *  The revision number of the GLFW header.  This is incremented when a bug fix
 *  release is made that does not contain any API changes.
 *  @ingroup init
 */
#define GLFW_VERSION_REVISION       8
/*! @} */

/*! @brief One.
 *
 *  This is only semantic sugar for the number 1.  You can instead use `1` or
 *  `true` or `_True` or `GL_TRUE` or `VK_TRUE` or anything else that is equal
 *  to one.
 *
 *  @ingroup init
 */
#define GLFW_TRUE                   1
/*! @brief Zero.
 *
 *  This is only semantic sugar for the number 0.  You can instead use `0` or
 *  `false` or `_False` or `GL_FALSE` or `VK_FALSE` or anything else that is
 *  equal to zero.
 *
 *  @ingroup init
 */
#define GLFW_FALSE                  0

/*! @name Key and button actions
 *  @{ */
/*! @brief The key or mouse button was released.
 *
 *  The key or mouse button was released.
 *
 *  @ingroup input
 */
#define GLFW_RELEASE                0
/*! @brief The key or mouse button was pressed.
 *
 *  The key or mouse button was pressed.
 *
 *  @ingroup input
 */
#define GLFW_PRESS                  1
/*! @brief The key was held down until it repeated.
 *
 *  The key was held down until it repeated.
 *
 *  @ingroup input
 */
#define GLFW_REPEAT                 2
/*! @} */

/*! @defgroup hat_state Joystick hat states
 *  @brief Joystick hat states.
 *
 *  See [joystick hat input](@ref joystick_hat) for how these are used.
 *
 *  @ingroup input
 *  @{ */
#define GLFW_HAT_CENTERED           0
#define GLFW_HAT_UP                 1
#define GLFW_HAT_RIGHT              2
#define GLFW_HAT_DOWN               4
#define GLFW_HAT_LEFT               8
#define GLFW_HAT_RIGHT_UP           (GLFW_HAT_RIGHT | GLFW_HAT_UP)
#define GLFW_HAT_RIGHT_DOWN         (GLFW_HAT_RIGHT | GLFW_HAT_DOWN)
#define GLFW_HAT_LEFT_UP            (GLFW_HAT_LEFT  | GLFW_HAT_UP)
#define GLFW_HAT_LEFT_DOWN          (GLFW_HAT_LEFT  | GLFW_HAT_DOWN)
/*! @} */

/*! @defgroup keys Keyboard keys
 *  @brief Keyboard key IDs.
 *
 *  See [key input](@ref input_key) for how these are used.
 *
 *  These key codes are inspired by the _USB HID Usage Tables v1.12_ (p. 53-60),
 *  but re-arranged to map to 7-bit ASCII for printable keys (function keys are
 *  put in the 256+ range).
 *
 *  The naming of the key codes follow these rules:
 *   - The US keyboard layout is used
 *   - Names of printable alpha-numeric characters are used (e.g. "A", "R",
 *     "3", etc.)
 *   - For non-alphanumeric characters, Unicode:ish names are used (e.g.
 *     "COMMA", "LEFT_SQUARE_BRACKET", etc.). Note that some names do not
 *     correspond to the Unicode standard (usually for brevity)
 *   - Keys that lack a clear US mapping are named "WORLD_x"
 *   - For non-printable keys, custom names are used (e.g. "F4",
 *     "BACKSPACE", etc.)
 *
 *  @ingroup input
 *  @{
 */

/* The unknown key */
#define GLFW_KEY_UNKNOWN            -1

/* Printable keys */
#define GLFW_KEY_SPACE              32
#define GLFW_KEY_APOSTROPHE         39  /* ' */
#define GLFW_KEY_COMMA              44  /* , */
#define GLFW_KEY_MINUS              45  /* - */
#define GLFW_KEY_PERIOD             46  /* . */
#define GLFW_KEY_SLASH              47  /* / */
#define GLFW_KEY_0                  48
#define GLFW_KEY_1                  49
#define GLFW_KEY_2                  50
#define GLFW_KEY_3                  51
#define GLFW_KEY_4                  52
#define GLFW_KEY_5                  53
#define GLFW_KEY_6                  54
#define GLFW_KEY_7                  55
#define GLFW_KEY_8                  56
#define GLFW_KEY_9                  57
#define GLFW_KEY_SEMICOLON          59  /* ; */
#define GLFW_KEY_EQUAL              61  /* = */
#define GLFW_KEY_A                  65
#define GLFW_KEY_B                  66
#define GLFW_KEY_C                  67
#define GLFW_KEY_D                  68
#define GLFW_KEY_E                  69
#define GLFW_KEY_F                  70
#define GLFW_KEY_G                  71
#define GLFW_KEY_H                  72
#define GLFW_KEY_I                  73
#define GLFW_KEY_J                  74
#define GLFW_KEY_K                  75
#define GLFW_KEY_L                  76
#define GLFW_KEY_M                  77
#define GLFW_KEY_N                  78
#define GLFW_KEY_O                  79
#define GLFW_KEY_P                  80
#define GLFW_KEY_Q                  81
#define GLFW_KEY_R                  82
#define GLFW_KEY_S                  83
#define GLFW_KEY_T                  84
#define GLFW_KEY_U                  85
#define GLFW_KEY_V                  86
#define GLFW_KEY_W                  87
#define GLFW_KEY_X                  88
#define GLFW_KEY_Y                  89
#define GLFW_KEY_Z                  90
#define GLFW_KEY_LEFT_BRACKET       91  /* [ */
#define GLFW_KEY_BACKSLASH          92  /* \ */
#define GLFW_KEY_RIGHT_BRACKET      93  /* ] */
#define GLFW_KEY_GRAVE_ACCENT       96  /* ` */
#define GLFW_KEY_WORLD_1            161 /* non-US #1 */
#define GLFW_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define GLFW_KEY_ESCAPE             256
#define GLFW_KEY_ENTER              257
#define GLFW_KEY_TAB                258
#define GLFW_KEY_BACKSPACE          259
#define GLFW_KEY_INSERT             260
#define GLFW_KEY_DELETE             261
#define GLFW_KEY_RIGHT              262
#define GLFW_KEY_LEFT               263
#define GLFW_KEY_DOWN               264
#define GLFW_KEY_UP                 265
#define GLFW_KEY_PAGE_UP            266
#define GLFW_KEY_PAGE_DOWN          267
#define GLFW_KEY_HOME               268
#define GLFW_KEY_END                269
#define GLFW_KEY_CAPS_LOCK          280
#define GLFW_KEY_SCROLL_LOCK        281
#define GLFW_KEY_NUM_LOCK           282
#define GLFW_KEY_PRINT_SCREEN       283
#define GLFW_KEY_PAUSE              284
#define GLFW_KEY_F1                 290
#define GLFW_KEY_F2                 291
#define GLFW_KEY_F3                 292
#define GLFW_KEY_F4                 293
#define GLFW_KEY_F5                 294
#define GLFW_KEY_F6                 295
#define GLFW_KEY_F7                 296
#define GLFW_KEY_F8                 297
#define GLFW_KEY_F9                 298
#define GLFW_KEY_F10                299
#define GLFW_KEY_F11                300
#define GLFW_KEY_F12                301
#define GLFW_KEY_F13                302
#define GLFW_KEY_F14                303
#define GLFW_KEY_F15                304
#define GLFW_KEY_F16                305
#define GLFW_KEY_F17                306
#define GLFW_KEY_F18                307
#define GLFW_KEY_F19                308
#define GLFW_KEY_F20                309
#define GLFW_KEY_F21                310
#define GLFW_KEY_F22                311
#define GLFW_KEY_F23                312
#define GLFW_KEY_F24                313
#define GLFW_KEY_F25                314
#define GLFW_KEY_KP_0               320
#define GLFW_KEY_KP_1               321
#define GLFW_KEY_KP_2               322
#define GLFW_KEY_KP_3               323
#define GLFW_KEY_KP_4               324
#define GLFW_KEY_KP_5               325
#define GLFW_KEY_KP_6               326
#define GLFW_KEY_KP_7               327
#define GLFW_KEY_KP_8               328
#define GLFW_KEY_KP_9               329
#define GLFW_KEY_KP_DECIMAL         330
#define GLFW_KEY_KP_DIVIDE          331
#define GLFW_KEY_KP_MULTIPLY        332
#define GLFW_KEY_KP_SUBTRACT        333
#define GLFW_KEY_KP_ADD             334
#define GLFW_KEY_KP_ENTER           335
#define GLFW_KEY_KP_EQUAL           336
#define GLFW_KEY_LEFT_SHIFT         340
#define GLFW_KEY_LEFT_CONTROL       341
#define GLFW_KEY_LEFT_ALT           342
#define GLFW_KEY_LEFT_SUPER         343
#define GLFW_KEY_RIGHT_SHIFT        344
#define GLFW_KEY_RIGHT_CONTROL      345
#define GLFW_KEY_RIGHT_ALT          346
#define GLFW_KEY_RIGHT_SUPER        347
#define GLFW_KEY_MENU               348

#define GLFW_KEY_LAST               GLFW_KEY_MENU

/*! @} */

/*! @defgroup mods Modifier key flags
 *  @brief Modifier key flags.
 *
 *  See [key input](@ref input_key) for how these are used.
 *
 *  @ingroup input
 *  @{ */

/*! @brief If this bit is set one or more Shift keys were held down.
 *
 *  If this bit is set one or more Shift keys were held down.
 */
#define GLFW_MOD_SHIFT           0x0001
/*! @brief If this bit is set one or more Control keys were held down.
 *
 *  If this bit is set one or more Control keys were held down.
 */
#define GLFW_MOD_CONTROL         0x0002
/*! @brief If this bit is set one or more Alt keys were held down.
 *
 *  If this bit is set one or more Alt keys were held down.
 */
#define GLFW_MOD_ALT             0x0004
/*! @brief If this bit is set one or more Super keys were held down.
 *
 *  If this bit is set one or more Super keys were held down.
 */
#define GLFW_MOD_SUPER           0x0008
/*! @brief If this bit is set the Caps Lock key is enabled.
 *
 *  If this bit is set the Caps Lock key is enabled and the @ref
 *  GLFW_LOCK_KEY_MODS input mode is set.
 */
#define GLFW_MOD_CAPS_LOCK       0x0010
/*! @brief If this bit is set the Num Lock key is enabled.
 *
 *  If this bit is set the Num Lock key is enabled and the @ref
 *  GLFW_LOCK_KEY_MODS input mode is set.
 */
#define GLFW_MOD_NUM_LOCK        0x0020

/*! @} */

/*! @defgroup buttons Mouse buttons
 *  @brief Mouse button IDs.
 *
 *  See [mouse button input](@ref input_mouse_button) for how these are used.
 *
 *  @ingroup input
 *  @{ */
#define GLFW_MOUSE_BUTTON_1         0
#define GLFW_MOUSE_BUTTON_2         1
#define GLFW_MOUSE_BUTTON_3         2
#define GLFW_MOUSE_BUTTON_4         3
#define GLFW_MOUSE_BUTTON_5         4
#define GLFW_MOUSE_BUTTON_6         5
#define GLFW_MOUSE_BUTTON_7         6
#define GLFW_MOUSE_BUTTON_8         7
#define GLFW_MOUSE_BUTTON_LAST      GLFW_MOUSE_BUTTON_8
#define GLFW_MOUSE_BUTTON_LEFT      GLFW_MOUSE_BUTTON_1
#define GLFW_MOUSE_BUTTON_RIGHT     GLFW_MOUSE_BUTTON_2
#define GLFW_MOUSE_BUTTON_MIDDLE    GLFW_MOUSE_BUTTON_3
/*! @} */

/*! @defgroup joysticks Joysticks
 *  @brief Joystick IDs.
 *
 *  See [joystick input](@ref joystick) for how these are used.
 *
 *  @ingroup input
 *  @{ */
#define GLFW_JOYSTICK_1             0
#define GLFW_JOYSTICK_2             1
#define GLFW_JOYSTICK_3             2
#define GLFW_JOYSTICK_4             3
#define GLFW_JOYSTICK_5             4
#define GLFW_JOYSTICK_6             5
#define GLFW_JOYSTICK_7             6
#define GLFW_JOYSTICK_8             7
#define GLFW_JOYSTICK_9             8
#define GLFW_JOYSTICK_10            9
#define GLFW_JOYSTICK_11            10
#define GLFW_JOYSTICK_12            11
#define GLFW_JOYSTICK_13            12
#define GLFW_JOYSTICK_14            13
#define GLFW_JOYSTICK_15            14
#define GLFW_JOYSTICK_16            15
#define GLFW_JOYSTICK_LAST          GLFW_JOYSTICK_16
/*! @} */

/*! @defgroup gamepad_buttons Gamepad buttons
 *  @brief Gamepad buttons.
 *
 *  See @ref gamepad for how these are used.
 *
 *  @ingroup input
 *  @{ */
#define GLFW_GAMEPAD_BUTTON_A               0
#define GLFW_GAMEPAD_BUTTON_B               1
#define GLFW_GAMEPAD_BUTTON_X               2
#define GLFW_GAMEPAD_BUTTON_Y               3
#define GLFW_GAMEPAD_BUTTON_LEFT_BUMPER     4
#define GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER    5
#define GLFW_GAMEPAD_BUTTON_BACK            6
#define GLFW_GAMEPAD_BUTTON_START           7
#define GLFW_GAMEPAD_BUTTON_GUIDE           8
#define GLFW_GAMEPAD_BUTTON_LEFT_THUMB      9
#define GLFW_GAMEPAD_BUTTON_RIGHT_THUMB     10
#define GLFW_GAMEPAD_BUTTON_DPAD_UP         11
#define GLFW_GAMEPAD_BUTTON_DPAD_RIGHT      12
#define GLFW_GAMEPAD_BUTTON_DPAD_DOWN       13
#define GLFW_GAMEPAD_BUTTON_DPAD_LEFT       14
#define GLFW_GAMEPAD_BUTTON_LAST            GLFW_GAMEPAD_BUTTON_DPAD_LEFT

#define GLFW_GAMEPAD_BUTTON_CROSS       GLFW_GAMEPAD_BUTTON_A
#define GLFW_GAMEPAD_BUTTON_CIRCLE      GLFW_GAMEPAD_BUTTON_B
#define GLFW_GAMEPAD_BUTTON_SQUARE      GLFW_GAMEPAD_BUTTON_X
#define GLFW_GAMEPAD_BUTTON_TRIANGLE    GLFW_GAMEPAD_BUTTON_Y
/*! @} */

/*! @defgroup gamepad_axes Gamepad axes
 *  @brief Gamepad axes.
 *
 *  See @ref gamepad for how these are used.
 *
 *  @ingroup input
 *  @{ */
#define GLFW_GAMEPAD_AXIS_LEFT_X        0
#define GLFW_GAMEPAD_AXIS_LEFT_Y        1
#define GLFW_GAMEPAD_AXIS_RIGHT_X       2
#define GLFW_GAMEPAD_AXIS_RIGHT_Y       3
#define GLFW_GAMEPAD_AXIS_LEFT_TRIGGER  4
#define GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER 5
#define GLFW_GAMEPAD_AXIS_LAST          GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER
/*! @} */

/*! @defgroup errors Error codes
 *  @brief Error codes.
 *
 *  See [error handling](@ref error_handling) for how these are used.
 *
 *  @ingroup init
 *  @{ */
/*! @brief No error has occurred.
 *
 *  No error has occurred.
 *
 *  @analysis Yay.
 */
#define GLFW_NO_ERROR               0
/*! @brief GLFW has not been initialized.
 *
 *  This occurs if a GLFW function was called that must not be called unless the
 *  library is [initialized](@ref intro_init).
 *
 *  @analysis Application programmer error.  Initialize GLFW before calling any
 *  function that requires initialization.
 */
#define GLFW_NOT_INITIALIZED        0x00010001
/*! @brief No context is current for this thread.
 *
 *  This occurs if a GLFW function was called that needs and operates on the
 *  current OpenGL or OpenGL ES context but no context is current on the calling
 *  thread.  One such function is @ref glfwSwapInterval.
 *
 *  @analysis Application programmer error.  Ensure a context is current before
 *  calling functions that require a current context.
 */
#define GLFW_NO_CURRENT_CONTEXT     0x00010002
/*! @brief One of the arguments to the function was an invalid enum value.
 *
 *  One of the arguments to the function was an invalid enum value, for example
 *  requesting @ref GLFW_RED_BITS with @ref glfwGetWindowAttrib.
 *
 *  @analysis Application programmer error.  Fix the offending call.
 */
#define GLFW_INVALID_ENUM           0x00010003
/*! @brief One of the arguments to the function was an invalid value.
 *
 *  One of the arguments to the function was an invalid value, for example
 *  requesting a non-existent OpenGL or OpenGL ES version like 2.7.
 *
 *  Requesting a valid but unavailable OpenGL or OpenGL ES version will instead
 *  result in a @ref GLFW_VERSION_UNAVAILABLE error.
 *
 *  @analysis Application programmer error.  Fix the offending call.
 */
#define GLFW_INVALID_VALUE          0x00010004
/*! @brief A memory allocation failed.
 *
 *  A memory allocation failed.
 *
 *  @analysis A bug in GLFW or the underlying operating system.  Report the bug
 *  to our [issue tracker](https://github.com/glfw/glfw/issues).
 */
#define GLFW_OUT_OF_MEMORY          0x00010005
/*! @brief GLFW could not find support for the requested API on the system.
 *
 *  GLFW could not find support for the requested API on the system.
 *
 *  @analysis The installed graphics driver does not support the requested
 *  API, or does not support it via the chosen context creation backend.
 *  Below are a few examples.
 *
 *  @par
 *  Some pre-installed Windows graphics drivers do not support OpenGL.  AMD only
 *  supports OpenGL ES via EGL, while Nvidia and Intel only support it via
 *  a WGL or GLX extension.  macOS does not provide OpenGL ES at all.  The Mesa
 *  EGL, OpenGL and OpenGL ES libraries do not interface with the Nvidia binary
 *  driver.  Older graphics drivers do not support Vulkan.
 */
#define GLFW_API_UNAVAILABLE        0x00010006
/*! @brief The requested OpenGL or OpenGL ES version is not available.
 *
 *  The requested OpenGL or OpenGL ES version (including any requested context
 *  or framebuffer hints) is not available on this machine.
 *
 *  @analysis The machine does not support your requirements.  If your
 *  application is sufficiently flexible, downgrade your requirements and try
 *  again.  Otherwise, inform the user that their machine does not match your
 *  requirements.
 *
 *  @par
 *  Future invalid OpenGL and OpenGL ES versions, for example OpenGL 4.8 if 5.0
 *  comes out before the 4.x series gets that far, also fail with this error and
 *  not @ref GLFW_INVALID_VALUE, because GLFW cannot know what future versions
 *  will exist.
 */
#define GLFW_VERSION_UNAVAILABLE    0x00010007
/*! @brief A platform-specific error occurred that does not match any of the
 *  more specific categories.
 *
 *  A platform-specific error occurred that does not match any of the more
 *  specific categories.
 *
 *  @analysis A bug or configuration error in GLFW, the underlying operating
 *  system or its drivers, or a lack of required resources.  Report the issue to
 *  our [issue tracker](https://github.com/glfw/glfw/issues).
 */
#define GLFW_PLATFORM_ERROR         0x00010008
/*! @brief The requested format is not supported or available.
 *
 *  If emitted during window creation, the requested pixel format is not
 *  supported.
 *
 *  If emitted when querying the clipboard, the contents of the clipboard could
 *  not be converted to the requested format.
 *
 *  @analysis If emitted during window creation, one or more
 *  [hard constraints](@ref window_hints_hard) did not match any of the
 *  available pixel formats.  If your application is sufficiently flexible,
 *  downgrade your requirements and try again.  Otherwise, inform the user that
 *  their machine does not match your requirements.
 *
 *  @par
 *  If emitted when querying the clipboard, ignore the error or report it to
 *  the user, as appropriate.
 */
#define GLFW_FORMAT_UNAVAILABLE     0x00010009
/*! @brief The specified window does not have an OpenGL or OpenGL ES context.
 *
 *  A window that does not have an OpenGL or OpenGL ES context was passed to
 *  a function that requires it to have one.
 *
 *  @analysis Application programmer error.  Fix the offending call.
 */
#define GLFW_NO_WINDOW_CONTEXT      0x0001000A
/*! @} */

/*! @addtogroup window
 *  @{ */
/*! @brief Input focus window hint and attribute
 *
 *  Input focus [window hint](@ref GLFW_FOCUSED_hint) or
 *  [window attribute](@ref GLFW_FOCUSED_attrib).
 */
#define GLFW_FOCUSED                0x00020001
/*! @brief Window iconification window attribute
 *
 *  Window iconification [window attribute](@ref GLFW_ICONIFIED_attrib).
 */
#define GLFW_ICONIFIED              0x00020002
/*! @brief Window resize-ability window hint and attribute
 *
 *  Window resize-ability [window hint](@ref GLFW_RESIZABLE_hint) and
 *  [window attribute](@ref GLFW_RESIZABLE_attrib).
 */
#define GLFW_RESIZABLE              0x00020003
/*! @brief Window visibility window hint and attribute
 *
 *  Window visibility [window hint](@ref GLFW_VISIBLE_hint) and
 *  [window attribute](@ref GLFW_VISIBLE_attrib).
 */
#define GLFW_VISIBLE                0x00020004
/*! @brief Window decoration window hint and attribute
 *
 *  Window decoration [window hint](@ref GLFW_DECORATED_hint) and
 *  [window attribute](@ref GLFW_DECORATED_attrib).
 */
#define GLFW_DECORATED              0x00020005
/*! @brief Window auto-iconification window hint and attribute
 *
 *  Window auto-iconification [window hint](@ref GLFW_AUTO_ICONIFY_hint) and
 *  [window attribute](@ref GLFW_AUTO_ICONIFY_attrib).
 */
#define GLFW_AUTO_ICONIFY           0x00020006
/*! @brief Window decoration window hint and attribute
 *
 *  Window decoration [window hint](@ref GLFW_FLOATING_hint) and
 *  [window attribute](@ref GLFW_FLOATING_attrib).
 */
#define GLFW_FLOATING               0x00020007
/*! @brief Window maximization window hint and attribute
 *
 *  Window maximization [window hint](@ref GLFW_MAXIMIZED_hint) and
 *  [window attribute](@ref GLFW_MAXIMIZED_attrib).
 */
#define GLFW_MAXIMIZED              0x00020008
/*! @brief Cursor centering window hint
 *
 *  Cursor centering [window hint](@ref GLFW_CENTER_CURSOR_hint).
 */
#define GLFW_CENTER_CURSOR          0x00020009
/*! @brief Window framebuffer transparency hint and attribute
 *
 *  Window framebuffer transparency
 *  [window hint](@ref GLFW_TRANSPARENT_FRAMEBUFFER_hint) and
 *  [window attribute](@ref GLFW_TRANSPARENT_FRAMEBUFFER_attrib).
 */
#define GLFW_TRANSPARENT_FRAMEBUFFER 0x0002000A
/*! @brief Mouse cursor hover window attribute.
 *
 *  Mouse cursor hover [window attribute](@ref GLFW_HOVERED_attrib).
 */
#define GLFW_HOVERED                0x0002000B
/*! @brief Input focus on calling show window hint and attribute
 *
 *  Input focus [window hint](@ref GLFW_FOCUS_ON_SHOW_hint) or
 *  [window attribute](@ref GLFW_FOCUS_ON_SHOW_attrib).
 */
#define GLFW_FOCUS_ON_SHOW          0x0002000C

/*! @brief Framebuffer bit depth hint.
 *
 *  Framebuffer bit depth [hint](@ref GLFW_RED_BITS).
 */
#define GLFW_RED_BITS               0x00021001
/*! @brief Framebuffer bit depth hint.
 *
 *  Framebuffer bit depth [hint](@ref GLFW_GREEN_BITS).
 */
#define GLFW_GREEN_BITS             0x00021002
/*! @brief Framebuffer bit depth hint.
 *
 *  Framebuffer bit depth [hint](@ref GLFW_BLUE_BITS).
 */
#define GLFW_BLUE_BITS              0x00021003
/*! @brief Framebuffer bit depth hint.
 *
 *  Framebuffer bit depth [hint](@ref GLFW_ALPHA_BITS).
 */
#define GLFW_ALPHA_BITS             0x00021004
/*! @brief Framebuffer bit depth hint.
 *
 *  Framebuffer bit depth [hint](@ref GLFW_DEPTH_BITS).
 */
#define GLFW_DEPTH_BITS             0x00021005
/*! @brief Framebuffer bit depth hint.
 *
 *  Framebuffer bit depth [hint](@ref GLFW_STENCIL_BITS).
 */
#define GLFW_STENCIL_BITS           0x00021006
/*! @brief Framebuffer bit depth hint.
 *
 *  Framebuffer bit depth [hint](@ref GLFW_ACCUM_RED_BITS).
 */
#define GLFW_ACCUM_RED_BITS         0x00021007
/*! @brief Framebuffer bit depth hint.
 *
 *  Framebuffer bit depth [hint](@ref GLFW_ACCUM_GREEN_BITS).
 */
#define GLFW_ACCUM_GREEN_BITS       0x00021008
/*! @brief Framebuffer bit depth hint.
 *
 *  Framebuffer bit depth [hint](@ref GLFW_ACCUM_BLUE_BITS).
 */
#define GLFW_ACCUM_BLUE_BITS        0x00021009
/*! @brief Framebuffer bit depth hint.
 *
 *  Framebuffer bit depth [hint](@ref GLFW_ACCUM_ALPHA_BITS).
 */
#define GLFW_ACCUM_ALPHA_BITS       0x0002100A
/*! @brief Framebuffer auxiliary buffer hint.
 *
 *  Framebuffer auxiliary buffer [hint](@ref GLFW_AUX_BUFFERS).
 */
#define GLFW_AUX_BUFFERS            0x0002100B
/*! @brief OpenGL stereoscopic rendering hint.
 *
 *  OpenGL stereoscopic rendering [hint](@ref GLFW_STEREO).
 */
#define GLFW_STEREO                 0x0002100C
/*! @brief Framebuffer MSAA samples hint.
 *
 *  Framebuffer MSAA samples [hint](@ref GLFW_SAMPLES).
 */
#define GLFW_SAMPLES                0x0002100D
/*! @brief Framebuffer sRGB hint.
 *
 *  Framebuffer sRGB [hint](@ref GLFW_SRGB_CAPABLE).
 */
#define GLFW_SRGB_CAPABLE           0x0002100E
/*! @brief Monitor refresh rate hint.
 *
 *  Monitor refresh rate [hint](@ref GLFW_REFRESH_RATE).
 */
#define GLFW_REFRESH_RATE           0x0002100F
/*! @brief Framebuffer double buffering hint.
 *
 *  Framebuffer double buffering [hint](@ref GLFW_DOUBLEBUFFER).
 */
#define GLFW_DOUBLEBUFFER           0x00021010

/*! @brief Context client API hint and attribute.
 *
 *  Context client API [hint](@ref GLFW_CLIENT_API_hint) and
 *  [attribute](@ref GLFW_CLIENT_API_attrib).
 */
#define GLFW_CLIENT_API             0x00022001
/*! @brief Context client API major version hint and attribute.
 *
 *  Context client API major version [hint](@ref GLFW_CONTEXT_VERSION_MAJOR_hint)
 *  and [attribute](@ref GLFW_CONTEXT_VERSION_MAJOR_attrib).
 */
#define GLFW_CONTEXT_VERSION_MAJOR  0x00022002
/*! @brief Context client API minor version hint and attribute.
 *
 *  Context client API minor version [hint](@ref GLFW_CONTEXT_VERSION_MINOR_hint)
 *  and [attribute](@ref GLFW_CONTEXT_VERSION_MINOR_attrib).
 */
#define GLFW_CONTEXT_VERSION_MINOR  0x00022003
/*! @brief Context client API revision number attribute.
 *
 *  Context client API revision number
 *  [attribute](@ref GLFW_CONTEXT_REVISION_attrib).
 */
#define GLFW_CONTEXT_REVISION       0x00022004
/*! @brief Context robustness hint and attribute.
 *
 *  Context client API revision number [hint](@ref GLFW_CONTEXT_ROBUSTNESS_hint)
 *  and [attribute](@ref GLFW_CONTEXT_ROBUSTNESS_attrib).
 */
#define GLFW_CONTEXT_ROBUSTNESS     0x00022005
/*! @brief OpenGL forward-compatibility hint and attribute.
 *
 *  OpenGL forward-compatibility [hint](@ref GLFW_OPENGL_FORWARD_COMPAT_hint)
 *  and [attribute](@ref GLFW_OPENGL_FORWARD_COMPAT_attrib).
 */
#define GLFW_OPENGL_FORWARD_COMPAT  0x00022006
/*! @brief Debug mode context hint and attribute.
 *
 *  Debug mode context [hint](@ref GLFW_OPENGL_DEBUG_CONTEXT_hint) and
 *  [attribute](@ref GLFW_OPENGL_DEBUG_CONTEXT_attrib).
 */
#define GLFW_OPENGL_DEBUG_CONTEXT   0x00022007
/*! @brief OpenGL profile hint and attribute.
 *
 *  OpenGL profile [hint](@ref GLFW_OPENGL_PROFILE_hint) and
 *  [attribute](@ref GLFW_OPENGL_PROFILE_attrib).
 */
#define GLFW_OPENGL_PROFILE         0x00022008
/*! @brief Context flush-on-release hint and attribute.
 *
 *  Context flush-on-release [hint](@ref GLFW_CONTEXT_RELEASE_BEHAVIOR_hint) and
 *  [attribute](@ref GLFW_CONTEXT_RELEASE_BEHAVIOR_attrib).
 */
#define GLFW_CONTEXT_RELEASE_BEHAVIOR 0x00022009
/*! @brief Context error suppression hint and attribute.
 *
 *  Context error suppression [hint](@ref GLFW_CONTEXT_NO_ERROR_hint) and
 *  [attribute](@ref GLFW_CONTEXT_NO_ERROR_attrib).
 */
#define GLFW_CONTEXT_NO_ERROR       0x0002200A
/*! @brief Context creation API hint and attribute.
 *
 *  Context creation API [hint](@ref GLFW_CONTEXT_CREATION_API_hint) and
 *  [attribute](@ref GLFW_CONTEXT_CREATION_API_attrib).
 */
#define GLFW_CONTEXT_CREATION_API   0x0002200B
/*! @brief Window content area scaling window
 *  [window hint](@ref GLFW_SCALE_TO_MONITOR).
 */
#define GLFW_SCALE_TO_MONITOR       0x0002200C
/*! @brief macOS specific
 *  [window hint](@ref GLFW_COCOA_RETINA_FRAMEBUFFER_hint).
 */
#define GLFW_COCOA_RETINA_FRAMEBUFFER 0x00023001
/*! @brief macOS specific
 *  [window hint](@ref GLFW_COCOA_FRAME_NAME_hint).
 */
#define GLFW_COCOA_FRAME_NAME         0x00023002
/*! @brief macOS specific
 *  [window hint](@ref GLFW_COCOA_GRAPHICS_SWITCHING_hint).
 */
#define GLFW_COCOA_GRAPHICS_SWITCHING 0x00023003
/*! @brief X11 specific
 *  [window hint](@ref GLFW_X11_CLASS_NAME_hint).
 */
#define GLFW_X11_CLASS_NAME         0x00024001
/*! @brief X11 specific
 *  [window hint](@ref GLFW_X11_CLASS_NAME_hint).
 */
#define GLFW_X11_INSTANCE_NAME      0x00024002
/*! @} */

#define GLFW_NO_API                          0
#define GLFW_OPENGL_API             0x00030001
#define GLFW_OPENGL_ES_API          0x00030002

#define GLFW_NO_ROBUSTNESS                   0
#define GLFW_NO_RESET_NOTIFICATION  0x00031001
#define GLFW_LOSE_CONTEXT_ON_RESET  0x00031002

#define GLFW_OPENGL_ANY_PROFILE              0
#define GLFW_OPENGL_CORE_PROFILE    0x00032001
#define GLFW_OPENGL_COMPAT_PROFILE  0x00032002

#define GLFW_CURSOR                 0x00033001
#define GLFW_STICKY_KEYS            0x00033002
#define GLFW_STICKY_MOUSE_BUTTONS   0x00033003
#define GLFW_LOCK_KEY_MODS          0x00033004
#define GLFW_RAW_MOUSE_MOTION       0x00033005

#define GLFW_CURSOR_NORMAL          0x00034001
#define GLFW_CURSOR_HIDDEN          0x00034002
#define GLFW_CURSOR_DISABLED        0x00034003

#define GLFW_ANY_RELEASE_BEHAVIOR            0
#define GLFW_RELEASE_BEHAVIOR_FLUSH 0x00035001
#define GLFW_RELEASE_BEHAVIOR_NONE  0x00035002

#define GLFW_NATIVE_CONTEXT_API     0x00036001
#define GLFW_EGL_CONTEXT_API        0x00036002
#define GLFW_OSMESA_CONTEXT_API     0x00036003

/*! @defgroup shapes Standard cursor shapes
 *  @brief Standard system cursor shapes.
 *
 *  See [standard cursor creation](@ref cursor_standard) for how these are used.
 *
 *  @ingroup input
 *  @{ */

/*! @brief The regular arrow cursor shape.
 *
 *  The regular arrow cursor.
 */
#define GLFW_ARROW_CURSOR           0x00036001
/*! @brief The text input I-beam cursor shape.
 *
 *  The text input I-beam cursor shape.
 */
#define GLFW_IBEAM_CURSOR           0x00036002
/*! @brief The crosshair shape.
 *
 *  The crosshair shape.
 */
#define GLFW_CROSSHAIR_CURSOR       0x00036003
/*! @brief The hand shape.
 *
 *  The hand shape.
 */
#define GLFW_HAND_CURSOR            0x00036004
/*! @brief The horizontal resize arrow shape.
 *
 *  The horizontal resize arrow shape.
 */
#define GLFW_HRESIZE_CURSOR         0x00036005
/*! @brief The vertical resize arrow shape.
 *
 *  The vertical resize arrow shape.
 */
#define GLFW_VRESIZE_CURSOR         0x00036006
/*! @} */

#define GLFW_CONNECTED              0x00040001
#define GLFW_DISCONNECTED           0x00040002

/*! @addtogroup init
 *  @{ */
/*! @brief Joystick hat buttons init hint.
 *
 *  Joystick hat buttons [init hint](@ref GLFW_JOYSTICK_HAT_BUTTONS).
 */
#define GLFW_JOYSTICK_HAT_BUTTONS   0x00050001
/*! @brief macOS specific init hint.
 *
 *  macOS specific [init hint](@ref GLFW_COCOA_CHDIR_RESOURCES_hint).
 */
#define GLFW_COCOA_CHDIR_RESOURCES  0x00051001
/*! @brief macOS specific init hint.
 *
 *  macOS specific [init hint](@ref GLFW_COCOA_MENUBAR_hint).
 */
#define GLFW_COCOA_MENUBAR          0x00051002
/*! @} */

#define GLFW_DONT_CARE              -1


/*************************************************************************
 * GLFW API types
 *************************************************************************/

/*! @brief Client API function pointer type.
 *
 *  Generic function pointer used for returning client API function pointers
 *  without forcing a cast from a regular pointer.
 *
 *  @sa @ref context_glext
 *  @sa @ref glfwGetProcAddress
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup context
 */
typedef void (*GLFWglproc)(void);

/*! @brief Vulkan API function pointer type.
 *
 *  Generic function pointer used for returning Vulkan API function pointers
 *  without forcing a cast from a regular pointer.
 *
 *  @sa @ref vulkan_proc
 *  @sa @ref glfwGetInstanceProcAddress
 *
 *  @since Added in version 3.2.
 *
 *  @ingroup vulkan
 */
typedef void (*GLFWvkproc)(void);

/*! @brief Opaque monitor object.
 *
 *  Opaque monitor object.
 *
 *  @see @ref monitor_object
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup monitor
 */
typedef struct GLFWmonitor GLFWmonitor;

/*! @brief Opaque window object.
 *
 *  Opaque window object.
 *
 *  @see @ref window_object
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup window
 */
typedef struct GLFWwindow GLFWwindow;

/*! @brief Opaque cursor object.
 *
 *  Opaque cursor object.
 *
 *  @see @ref cursor_object
 *
 *  @since Added in version 3.1.
 *
 *  @ingroup input
 */
typedef struct GLFWcursor GLFWcursor;

/*! @brief The function pointer type for error callbacks.
 *
 *  This is the function pointer type for error callbacks.  An error callback
 *  function has the following signature:
 *  @code
 *  void callback_name(int error_code, const char* description)
 *  @endcode
 *
 *  @param[in] error_code An [error code](@ref errors).  Future releases may add
 *  more error codes.
 *  @param[in] description A UTF-8 encoded string describing the error.
 *
 *  @pointer_lifetime The error description string is valid until the callback
 *  function returns.
 *
 *  @sa @ref error_handling
 *  @sa @ref glfwSetErrorCallback
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup init
 */
typedef void (* GLFWerrorfun)(int error_code, const char* description);

/*! @brief The function pointer type for window position callbacks.
 *
 *  This is the function pointer type for window position callbacks.  A window
 *  position callback function has the following signature:
 *  @code
 *  void callback_name(GLFWwindow* window, int xpos, int ypos)
 *  @endcode
 *
 *  @param[in] window The window that was moved.
 *  @param[in] xpos The new x-coordinate, in screen coordinates, of the
 *  upper-left corner of the content area of the window.
 *  @param[in] ypos The new y-coordinate, in screen coordinates, of the
 *  upper-left corner of the content area of the window.
 *
 *  @sa @ref window_pos
 *  @sa @ref glfwSetWindowPosCallback
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup window
 */
typedef void (* GLFWwindowposfun)(GLFWwindow* window, int xpos, int ypos);

/*! @brief The function pointer type for window size callbacks.
 *
 *  This is the function pointer type for window size callbacks.  A window size
 *  callback function has the following signature:
 *  @code
 *  void callback_name(GLFWwindow* window, int width, int height)
 *  @endcode
 *
 *  @param[in] window The window that was resized.
 *  @param[in] width The new width, in screen coordinates, of the window.
 *  @param[in] height The new height, in screen coordinates, of the window.
 *
 *  @sa @ref window_size
 *  @sa @ref glfwSetWindowSizeCallback
 *
 *  @since Added in version 1.0.
 *  @glfw3 Added window handle parameter.
 *
 *  @ingroup window
 */
typedef void (* GLFWwindowsizefun)(GLFWwindow* window, int width, int height);

/*! @brief The function pointer type for window close callbacks.
 *
 *  This is the function pointer type for window close callbacks.  A window
 *  close callback function has the following signature:
 *  @code
 *  void function_name(GLFWwindow* window)
 *  @endcode
 *
 *  @param[in] window The window that the user attempted to close.
 *
 *  @sa @ref window_close
 *  @sa @ref glfwSetWindowCloseCallback
 *
 *  @since Added in version 2.5.
 *  @glfw3 Added window handle parameter.
 *
 *  @ingroup window
 */
typedef void (* GLFWwindowclosefun)(GLFWwindow* window);

/*! @brief The function pointer type for window content refresh callbacks.
 *
 *  This is the function pointer type for window content refresh callbacks.
 *  A window content refresh callback function has the following signature:
 *  @code
 *  void function_name(GLFWwindow* window);
 *  @endcode
 *
 *  @param[in] window The window whose content needs to be refreshed.
 *
 *  @sa @ref window_refresh
 *  @sa @ref glfwSetWindowRefreshCallback
 *
 *  @since Added in version 2.5.
 *  @glfw3 Added window handle parameter.
 *
 *  @ingroup window
 */
typedef void (* GLFWwindowrefreshfun)(GLFWwindow* window);

/*! @brief The function pointer type for window focus callbacks.
 *
 *  This is the function pointer type for window focus callbacks.  A window
 *  focus callback function has the following signature:
 *  @code
 *  void function_name(GLFWwindow* window, int focused)
 *  @endcode
 *
 *  @param[in] window The window that gained or lost input focus.
 *  @param[in] focused `GLFW_TRUE` if the window was given input focus, or
 *  `GLFW_FALSE` if it lost it.
 *
 *  @sa @ref window_focus
 *  @sa @ref glfwSetWindowFocusCallback
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup window
 */
typedef void (* GLFWwindowfocusfun)(GLFWwindow* window, int focused);

/*! @brief The function pointer type for window iconify callbacks.
 *
 *  This is the function pointer type for window iconify callbacks.  A window
 *  iconify callback function has the following signature:
 *  @code
 *  void function_name(GLFWwindow* window, int iconified)
 *  @endcode
 *
 *  @param[in] window The window that was iconified or restored.
 *  @param[in] iconified `GLFW_TRUE` if the window was iconified, or
 *  `GLFW_FALSE` if it was restored.
 *
 *  @sa @ref window_iconify
 *  @sa @ref glfwSetWindowIconifyCallback
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup window
 */
typedef void (* GLFWwindowiconifyfun)(GLFWwindow* window, int iconified);

/*! @brief The function pointer type for window maximize callbacks.
 *
 *  This is the function pointer type for window maximize callbacks.  A window
 *  maximize callback function has the following signature:
 *  @code
 *  void function_name(GLFWwindow* window, int maximized)
 *  @endcode
 *
 *  @param[in] window The window that was maximized or restored.
 *  @param[in] maximized `GLFW_TRUE` if the window was maximized, or
 *  `GLFW_FALSE` if it was restored.
 *
 *  @sa @ref window_maximize
 *  @sa glfwSetWindowMaximizeCallback
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup window
 */
typedef void (* GLFWwindowmaximizefun)(GLFWwindow* window, int maximized);

/*! @brief The function pointer type for framebuffer size callbacks.
 *
 *  This is the function pointer type for framebuffer size callbacks.
 *  A framebuffer size callback function has the following signature:
 *  @code
 *  void function_name(GLFWwindow* window, int width, int height)
 *  @endcode
 *
 *  @param[in] window The window whose framebuffer was resized.
 *  @param[in] width The new width, in pixels, of the framebuffer.
 *  @param[in] height The new height, in pixels, of the framebuffer.
 *
 *  @sa @ref window_fbsize
 *  @sa @ref glfwSetFramebufferSizeCallback
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup window
 */
typedef void (* GLFWframebuffersizefun)(GLFWwindow* window, int width, int height);

/*! @brief The function pointer type for window content scale callbacks.
 *
 *  This is the function pointer type for window content scale callbacks.
 *  A window content scale callback function has the following signature:
 *  @code
 *  void function_name(GLFWwindow* window, float xscale, float yscale)
 *  @endcode
 *
 *  @param[in] window The window whose content scale changed.
 *  @param[in] xscale The new x-axis content scale of the window.
 *  @param[in] yscale The new y-axis content scale of the window.
 *
 *  @sa @ref window_scale
 *  @sa @ref glfwSetWindowContentScaleCallback
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup window
 */
typedef void (* GLFWwindowcontentscalefun)(GLFWwindow* window, float xscale, float yscale);

/*! @brief The function pointer type for mouse button callbacks.
 *
 *  This is the function pointer type for mouse button callback functions.
 *  A mouse button callback function has the following signature:
 *  @code
 *  void function_name(GLFWwindow* window, int button, int action, int mods)
 *  @endcode
 *
 *  @param[in] window The window that received the event.
 *  @param[in] button The [mouse button](@ref buttons) that was pressed or
 *  released.
 *  @param[in] action One of `GLFW_PRESS` or `GLFW_RELEASE`.  Future releases
 *  may add more actions.
 *  @param[in] mods Bit field describing which [modifier keys](@ref mods) were
 *  held down.
 *
 *  @sa @ref input_mouse_button
 *  @sa @ref glfwSetMouseButtonCallback
 *
 *  @since Added in version 1.0.
 *  @glfw3 Added window handle and modifier mask parameters.
 *
 *  @ingroup input
 */
typedef void (* GLFWmousebuttonfun)(GLFWwindow* window, int button, int action, int mods);

/*! @brief The function pointer type for cursor position callbacks.
 *
 *  This is the function pointer type for cursor position callbacks.  A cursor
 *  position callback function has the following signature:
 *  @code
 *  void function_name(GLFWwindow* window, double xpos, double ypos);
 *  @endcode
 *
 *  @param[in] window The window that received the event.
 *  @param[in] xpos The new cursor x-coordinate, relative to the left edge of
 *  the content area.
 *  @param[in] ypos The new cursor y-coordinate, relative to the top edge of the
 *  content area.
 *
 *  @sa @ref cursor_pos
 *  @sa @ref glfwSetCursorPosCallback
 *
 *  @since Added in version 3.0.  Replaces `GLFWmouseposfun`.
 *
 *  @ingroup input
 */
typedef void (* GLFWcursorposfun)(GLFWwindow* window, double xpos, double ypos);

/*! @brief The function pointer type for cursor enter/leave callbacks.
 *
 *  This is the function pointer type for cursor enter/leave callbacks.
 *  A cursor enter/leave callback function has the following signature:
 *  @code
 *  void function_name(GLFWwindow* window, int entered)
 *  @endcode
 *
 *  @param[in] window The window that received the event.
 *  @param[in] entered `GLFW_TRUE` if the cursor entered the window's content
 *  area, or `GLFW_FALSE` if it left it.
 *
 *  @sa @ref cursor_enter
 *  @sa @ref glfwSetCursorEnterCallback
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup input
 */
typedef void (* GLFWcursorenterfun)(GLFWwindow* window, int entered);

/*! @brief The function pointer type for scroll callbacks.
 *
 *  This is the function pointer type for scroll callbacks.  A scroll callback
 *  function has the following signature:
 *  @code
 *  void function_name(GLFWwindow* window, double xoffset, double yoffset)
 *  @endcode
 *
 *  @param[in] window The window that received the event.
 *  @param[in] xoffset The scroll offset along the x-axis.
 *  @param[in] yoffset The scroll offset along the y-axis.
 *
 *  @sa @ref scrolling
 *  @sa @ref glfwSetScrollCallback
 *
 *  @since Added in version 3.0.  Replaces `GLFWmousewheelfun`.
 *
 *  @ingroup input
 */
typedef void (* GLFWscrollfun)(GLFWwindow* window, double xoffset, double yoffset);

/*! @brief The function pointer type for keyboard key callbacks.
 *
 *  This is the function pointer type for keyboard key callbacks.  A keyboard
 *  key callback function has the following signature:
 *  @code
 *  void function_name(GLFWwindow* window, int key, int scancode, int action, int mods)
 *  @endcode
 *
 *  @param[in] window The window that received the event.
 *  @param[in] key The [keyboard key](@ref keys) that was pressed or released.
 *  @param[in] scancode The system-specific scancode of the key.
 *  @param[in] action `GLFW_PRESS`, `GLFW_RELEASE` or `GLFW_REPEAT`.  Future
 *  releases may add more actions.
 *  @param[in] mods Bit field describing which [modifier keys](@ref mods) were
 *  held down.
 *
 *  @sa @ref input_key
 *  @sa @ref glfwSetKeyCallback
 *
 *  @since Added in version 1.0.
 *  @glfw3 Added window handle, scancode and modifier mask parameters.
 *
 *  @ingroup input
 */
typedef void (* GLFWkeyfun)(GLFWwindow* window, int key, int scancode, int action, int mods);

/*! @brief The function pointer type for Unicode character callbacks.
 *
 *  This is the function pointer type for Unicode character callbacks.
 *  A Unicode character callback function has the following signature:
 *  @code
 *  void function_name(GLFWwindow* window, unsigned int codepoint)
 *  @endcode
 *
 *  @param[in] window The window that received the event.
 *  @param[in] codepoint The Unicode code point of the character.
 *
 *  @sa @ref input_char
 *  @sa @ref glfwSetCharCallback
 *
 *  @since Added in version 2.4.
 *  @glfw3 Added window handle parameter.
 *
 *  @ingroup input
 */
typedef void (* GLFWcharfun)(GLFWwindow* window, unsigned int codepoint);

/*! @brief The function pointer type for Unicode character with modifiers
 *  callbacks.
 *
 *  This is the function pointer type for Unicode character with modifiers
 *  callbacks.  It is called for each input character, regardless of what
 *  modifier keys are held down.  A Unicode character with modifiers callback
 *  function has the following signature:
 *  @code
 *  void function_name(GLFWwindow* window, unsigned int codepoint, int mods)
 *  @endcode
 *
 *  @param[in] window The window that received the event.
 *  @param[in] codepoint The Unicode code point of the character.
 *  @param[in] mods Bit field describing which [modifier keys](@ref mods) were
 *  held down.
 *
 *  @sa @ref input_char
 *  @sa @ref glfwSetCharModsCallback
 *
 *  @deprecated Scheduled for removal in version 4.0.
 *
 *  @since Added in version 3.1.
 *
 *  @ingroup input
 */
typedef void (* GLFWcharmodsfun)(GLFWwindow* window, unsigned int codepoint, int mods);

/*! @brief The function pointer type for path drop callbacks.
 *
 *  This is the function pointer type for path drop callbacks.  A path drop
 *  callback function has the following signature:
 *  @code
 *  void function_name(GLFWwindow* window, int path_count, const char* paths[])
 *  @endcode
 *
 *  @param[in] window The window that received the event.
 *  @param[in] path_count The number of dropped paths.
 *  @param[in] paths The UTF-8 encoded file and/or directory path names.
 *
 *  @pointer_lifetime The path array and its strings are valid until the
 *  callback function returns.
 *
 *  @sa @ref path_drop
 *  @sa @ref glfwSetDropCallback
 *
 *  @since Added in version 3.1.
 *
 *  @ingroup input
 */
typedef void (* GLFWdropfun)(GLFWwindow* window, int path_count, const char* paths[]);

/*! @brief The function pointer type for monitor configuration callbacks.
 *
 *  This is the function pointer type for monitor configuration callbacks.
 *  A monitor callback function has the following signature:
 *  @code
 *  void function_name(GLFWmonitor* monitor, int event)
 *  @endcode
 *
 *  @param[in] monitor The monitor that was connected or disconnected.
 *  @param[in] event One of `GLFW_CONNECTED` or `GLFW_DISCONNECTED`.  Future
 *  releases may add more events.
 *
 *  @sa @ref monitor_event
 *  @sa @ref glfwSetMonitorCallback
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup monitor
 */
typedef void (* GLFWmonitorfun)(GLFWmonitor* monitor, int event);

/*! @brief The function pointer type for joystick configuration callbacks.
 *
 *  This is the function pointer type for joystick configuration callbacks.
 *  A joystick configuration callback function has the following signature:
 *  @code
 *  void function_name(int jid, int event)
 *  @endcode
 *
 *  @param[in] jid The joystick that was connected or disconnected.
 *  @param[in] event One of `GLFW_CONNECTED` or `GLFW_DISCONNECTED`.  Future
 *  releases may add more events.
 *
 *  @sa @ref joystick_event
 *  @sa @ref glfwSetJoystickCallback
 *
 *  @since Added in version 3.2.
 *
 *  @ingroup input
 */
typedef void (* GLFWjoystickfun)(int jid, int event);

/*! @brief Video mode type.
 *
 *  This describes a single video mode.
 *
 *  @sa @ref monitor_modes
 *  @sa @ref glfwGetVideoMode
 *  @sa @ref glfwGetVideoModes
 *
 *  @since Added in version 1.0.
 *  @glfw3 Added refresh rate member.
 *
 *  @ingroup monitor
 */
typedef struct GLFWvidmode
{
    /*! The width, in screen coordinates, of the video mode.
     */
    int width;
    /*! The height, in screen coordinates, of the video mode.
     */
    int height;
    /*! The bit depth of the red channel of the video mode.
     */
    int redBits;
    /*! The bit depth of the green channel of the video mode.
     */
    int greenBits;
    /*! The bit depth of the blue channel of the video mode.
     */
    int blueBits;
    /*! The refresh rate, in Hz, of the video mode.
     */
    int refreshRate;
} GLFWvidmode;

/*! @brief Gamma ramp.
 *
 *  This describes the gamma ramp for a monitor.
 *
 *  @sa @ref monitor_gamma
 *  @sa @ref glfwGetGammaRamp
 *  @sa @ref glfwSetGammaRamp
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup monitor
 */
typedef struct GLFWgammaramp
{
    /*! An array of value describing the response of the red channel.
     */
    unsigned short* red;
    /*! An array of value describing the response of the green channel.
     */
    unsigned short* green;
    /*! An array of value describing the response of the blue channel.
     */
    unsigned short* blue;
    /*! The number of elements in each array.
     */
    unsigned int size;
} GLFWgammaramp;

/*! @brief Image data.
 *
 *  This describes a single 2D image.  See the documentation for each related
 *  function what the expected pixel format is.
 *
 *  @sa @ref cursor_custom
 *  @sa @ref window_icon
 *
 *  @since Added in version 2.1.
 *  @glfw3 Removed format and bytes-per-pixel members.
 *
 *  @ingroup window
 */
typedef struct GLFWimage
{
    /*! The width, in pixels, of this image.
     */
    int width;
    /*! The height, in pixels, of this image.
     */
    int height;
    /*! The pixel data of this image, arranged left-to-right, top-to-bottom.
     */
    unsigned char* pixels;
} GLFWimage;

/*! @brief Gamepad input state
 *
 *  This describes the input state of a gamepad.
 *
 *  @sa @ref gamepad
 *  @sa @ref glfwGetGamepadState
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup input
 */
typedef struct GLFWgamepadstate
{
    /*! The states of each [gamepad button](@ref gamepad_buttons), `GLFW_PRESS`
     *  or `GLFW_RELEASE`.
     */
    unsigned char buttons[15];
    /*! The states of each [gamepad axis](@ref gamepad_axes), in the range -1.0
     *  to 1.0 inclusive.
     */
    float axes[6];
} GLFWgamepadstate;


/*************************************************************************
 * GLFW API functions
 *************************************************************************/

/*! @brief Initializes the GLFW library.
 *
 *  This function initializes the GLFW library.  Before most GLFW functions can
 *  be used, GLFW must be initialized, and before an application terminates GLFW
 *  should be terminated in order to free any resources allocated during or
 *  after initialization.
 *
 *  If this function fails, it calls @ref glfwTerminate before returning.  If it
 *  succeeds, you should call @ref glfwTerminate before the application exits.
 *
 *  Additional calls to this function after successful initialization but before
 *  termination will return `GLFW_TRUE` immediately.
 *
 *  @return `GLFW_TRUE` if successful, or `GLFW_FALSE` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_PLATFORM_ERROR.
 *
 *  @remark @macos This function will change the current directory of the
 *  application to the `Contents/Resources` subdirectory of the application's
 *  bundle, if present.  This can be disabled with the @ref
 *  GLFW_COCOA_CHDIR_RESOURCES init hint.
 *
 *  @remark @x11 This function will set the `LC_CTYPE` category of the
 *  application locale according to the current environment if that category is
 *  still "C".  This is because the "C" locale breaks Unicode text input.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref intro_init
 *  @sa @ref glfwTerminate
 *
 *  @since Added in version 1.0.
 *
 *  @ingroup init
 */
GLFWAPI int glfwInit(void);

/*! @brief Terminates the GLFW library.
 *
 *  This function destroys all remaining windows and cursors, restores any
 *  modified gamma ramps and frees any other allocated resources.  Once this
 *  function is called, you must again call @ref glfwInit successfully before
 *  you will be able to use most GLFW functions.
 *
 *  If GLFW has been successfully initialized, this function should be called
 *  before the application exits.  If initialization fails, there is no need to
 *  call this function, as it is called by @ref glfwInit before it returns
 *  failure.
 *
 *  This function has no effect if GLFW is not initialized.
 *
 *  @errors Possible errors include @ref GLFW_PLATFORM_ERROR.
 *
 *  @remark This function may be called before @ref glfwInit.
 *
 *  @warning The contexts of any remaining windows must not be current on any
 *  other thread when this function is called.
 *
 *  @reentrancy This function must not be called from a callback.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref intro_init
 *  @sa @ref glfwInit
 *
 *  @since Added in version 1.0.
 *
 *  @ingroup init
 */
GLFWAPI void glfwTerminate(void);

/*! @brief Sets the specified init hint to the desired value.
 *
 *  This function sets hints for the next initialization of GLFW.
 *
 *  The values you set hints to are never reset by GLFW, but they only take
 *  effect during initialization.  Once GLFW has been initialized, any values
 *  you set will be ignored until the library is terminated and initialized
 *  again.
 *
 *  Some hints are platform specific.  These may be set on any platform but they
 *  will only affect their specific platform.  Other platforms will ignore them.
 *  Setting these hints requires no platform specific headers or functions.
 *
 *  @param[in] hint The [init hint](@ref init_hints) to set.
 *  @param[in] value The new value of the init hint.
 *
 *  @errors Possible errors include @ref GLFW_INVALID_ENUM and @ref
 *  GLFW_INVALID_VALUE.
 *
 *  @remarks This function may be called before @ref glfwInit.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa init_hints
 *  @sa glfwInit
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup init
 */
GLFWAPI void glfwInitHint(int hint, int value);

/*! @brief Retrieves the version of the GLFW library.
 *
 *  This function retrieves the major, minor and revision numbers of the GLFW
 *  library.  It is intended for when you are using GLFW as a shared library and
 *  want to ensure that you are using the minimum required version.
 *
 *  Any or all of the version arguments may be `NULL`.
 *
 *  @param[out] major Where to store the major version number, or `NULL`.
 *  @param[out] minor Where to store the minor version number, or `NULL`.
 *  @param[out] rev Where to store the revision number, or `NULL`.
 *
 *  @errors None.
 *
 *  @remark This function may be called before @ref glfwInit.
 *
 *  @thread_safety This function may be called from any thread.
 *
 *  @sa @ref intro_version
 *  @sa @ref glfwGetVersionString
 *
 *  @since Added in version 1.0.
 *
 *  @ingroup init
 */
GLFWAPI void glfwGetVersion(int* major, int* minor, int* rev);

/*! @brief Returns a string describing the compile-time configuration.
 *
 *  This function returns the compile-time generated
 *  [version string](@ref intro_version_string) of the GLFW library binary.  It
 *  describes the version, platform, compiler and any platform-specific
 *  compile-time options.  It should not be confused with the OpenGL or OpenGL
 *  ES version string, queried with `glGetString`.
 *
 *  __Do not use the version string__ to parse the GLFW library version.  The
 *  @ref glfwGetVersion function provides the version of the running library
 *  binary in numerical format.
 *
 *  @return The ASCII encoded GLFW version string.
 *
 *  @errors None.
 *
 *  @remark This function may be called before @ref glfwInit.
 *
 *  @pointer_lifetime The returned string is static and compile-time generated.
 *
 *  @thread_safety This function may be called from any thread.
 *
 *  @sa @ref intro_version
 *  @sa @ref glfwGetVersion
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup init
 */
GLFWAPI const char* glfwGetVersionString(void);

/*! @brief Returns and clears the last error for the calling thread.
 *
 *  This function returns and clears the [error code](@ref errors) of the last
 *  error that occurred on the calling thread, and optionally a UTF-8 encoded
 *  human-readable description of it.  If no error has occurred since the last
 *  call, it returns @ref GLFW_NO_ERROR (zero) and the description pointer is
 *  set to `NULL`.
 *
 *  @param[in] description Where to store the error description pointer, or `NULL`.
 *  @return The last error code for the calling thread, or @ref GLFW_NO_ERROR
 *  (zero).
 *
 *  @errors None.
 *
 *  @pointer_lifetime The returned string is allocated and freed by GLFW.  You
 *  should not free it yourself.  It is guaranteed to be valid only until the
 *  next error occurs or the library is terminated.
 *
 *  @remark This function may be called before @ref glfwInit.
 *
 *  @thread_safety This function may be called from any thread.
 *
 *  @sa @ref error_handling
 *  @sa @ref glfwSetErrorCallback
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup init
 */
GLFWAPI int glfwGetError(const char** description);

/*! @brief Sets the error callback.
 *
 *  This function sets the error callback, which is called with an error code
 *  and a human-readable description each time a GLFW error occurs.
 *
 *  The error code is set before the callback is called.  Calling @ref
 *  glfwGetError from the error callback will return the same value as the error
 *  code argument.
 *
 *  The error callback is called on the thread where the error occurred.  If you
 *  are using GLFW from multiple threads, your error callback needs to be
 *  written accordingly.
 *
 *  Because the description string may have been generated specifically for that
 *  error, it is not guaranteed to be valid after the callback has returned.  If
 *  you wish to use it after the callback returns, you need to make a copy.
 *
 *  Once set, the error callback remains set even after the library has been
 *  terminated.
 *
 *  @param[in] callback The new callback, or `NULL` to remove the currently set
 *  callback.
 *  @return The previously set callback, or `NULL` if no callback was set.
 *
 *  @callback_signature
 *  @code
 *  void callback_name(int error_code, const char* description)
 *  @endcode
 *  For more information about the callback parameters, see the
 *  [callback pointer type](@ref GLFWerrorfun).
 *
 *  @errors None.
 *
 *  @remark This function may be called before @ref glfwInit.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref error_handling
 *  @sa @ref glfwGetError
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup init
 */
GLFWAPI GLFWerrorfun glfwSetErrorCallback(GLFWerrorfun callback);

/*! @brief Returns the currently connected monitors.
 *
 *  This function returns an array of handles for all currently connected
 *  monitors.  The primary monitor is always first in the returned array.  If no
 *  monitors were found, this function returns `NULL`.
 *
 *  @param[out] count Where to store the number of monitors in the returned
 *  array.  This is set to zero if an error occurred.
 *  @return An array of monitor handles, or `NULL` if no monitors were found or
 *  if an [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @pointer_lifetime The returned array is allocated and freed by GLFW.  You
 *  should not free it yourself.  It is guaranteed to be valid only until the
 *  monitor configuration changes or the library is terminated.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref monitor_monitors
 *  @sa @ref monitor_event
 *  @sa @ref glfwGetPrimaryMonitor
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup monitor
 */
GLFWAPI GLFWmonitor** glfwGetMonitors(int* count);

/*! @brief Returns the primary monitor.
 *
 *  This function returns the primary monitor.  This is usually the monitor
 *  where elements like the task bar or global menu bar are located.
 *
 *  @return The primary monitor, or `NULL` if no monitors were found or if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @remark The primary monitor is always first in the array returned by @ref
 *  glfwGetMonitors.
 *
 *  @sa @ref monitor_monitors
 *  @sa @ref glfwGetMonitors
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup monitor
 */
GLFWAPI GLFWmonitor* glfwGetPrimaryMonitor(void);

/*! @brief Returns the position of the monitor's viewport on the virtual screen.
 *
 *  This function returns the position, in screen coordinates, of the upper-left
 *  corner of the specified monitor.
 *
 *  Any or all of the position arguments may be `NULL`.  If an error occurs, all
 *  non-`NULL` position arguments will be set to zero.
 *
 *  @param[in] monitor The monitor to query.
 *  @param[out] xpos Where to store the monitor x-coordinate, or `NULL`.
 *  @param[out] ypos Where to store the monitor y-coordinate, or `NULL`.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref monitor_properties
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup monitor
 */
GLFWAPI void glfwGetMonitorPos(GLFWmonitor* monitor, int* xpos, int* ypos);

/*! @brief Retrieves the work area of the monitor.
 *
 *  This function returns the position, in screen coordinates, of the upper-left
 *  corner of the work area of the specified monitor along with the work area
 *  size in screen coordinates. The work area is defined as the area of the
 *  monitor not occluded by the operating system task bar where present. If no
 *  task bar exists then the work area is the monitor resolution in screen
 *  coordinates.
 *
 *  Any or all of the position and size arguments may be `NULL`.  If an error
 *  occurs, all non-`NULL` position and size arguments will be set to zero.
 *
 *  @param[in] monitor The monitor to query.
 *  @param[out] xpos Where to store the monitor x-coordinate, or `NULL`.
 *  @param[out] ypos Where to store the monitor y-coordinate, or `NULL`.
 *  @param[out] width Where to store the monitor width, or `NULL`.
 *  @param[out] height Where to store the monitor height, or `NULL`.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref monitor_workarea
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup monitor
 */
GLFWAPI void glfwGetMonitorWorkarea(GLFWmonitor* monitor, int* xpos, int* ypos, int* width, int* height);

/*! @brief Returns the physical size of the monitor.
 *
 *  This function returns the size, in millimetres, of the display area of the
 *  specified monitor.
 *
 *  Some systems do not provide accurate monitor size information, either
 *  because the monitor
 *  [EDID](https://en.wikipedia.org/wiki/Extended_display_identification_data)
 *  data is incorrect or because the driver does not report it accurately.
 *
 *  Any or all of the size arguments may be `NULL`.  If an error occurs, all
 *  non-`NULL` size arguments will be set to zero.
 *
 *  @param[in] monitor The monitor to query.
 *  @param[out] widthMM Where to store the width, in millimetres, of the
 *  monitor's display area, or `NULL`.
 *  @param[out] heightMM Where to store the height, in millimetres, of the
 *  monitor's display area, or `NULL`.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @remark @win32 On Windows 8 and earlier the physical size is calculated from
 *  the current resolution and system DPI instead of querying the monitor EDID data.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref monitor_properties
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup monitor
 */
GLFWAPI void glfwGetMonitorPhysicalSize(GLFWmonitor* monitor, int* widthMM, int* heightMM);

/*! @brief Retrieves the content scale for the specified monitor.
 *
 *  This function retrieves the content scale for the specified monitor.  The
 *  content scale is the ratio between the current DPI and the platform's
 *  default DPI.  This is especially important for text and any UI elements.  If
 *  the pixel dimensions of your UI scaled by this look appropriate on your
 *  machine then it should appear at a reasonable size on other machines
 *  regardless of their DPI and scaling settings.  This relies on the system DPI
 *  and scaling settings being somewhat correct.
 *
 *  The content scale may depend on both the monitor resolution and pixel
 *  density and on user settings.  It may be very different from the raw DPI
 *  calculated from the physical size and current resolution.
 *
 *  @param[in] monitor The monitor to query.
 *  @param[out] xscale Where to store the x-axis content scale, or `NULL`.
 *  @param[out] yscale Where to store the y-axis content scale, or `NULL`.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref monitor_scale
 *  @sa @ref glfwGetWindowContentScale
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup monitor
 */
GLFWAPI void glfwGetMonitorContentScale(GLFWmonitor* monitor, float* xscale, float* yscale);

/*! @brief Returns the name of the specified monitor.
 *
 *  This function returns a human-readable name, encoded as UTF-8, of the
 *  specified monitor.  The name typically reflects the make and model of the
 *  monitor and is not guaranteed to be unique among the connected monitors.
 *
 *  @param[in] monitor The monitor to query.
 *  @return The UTF-8 encoded name of the monitor, or `NULL` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @pointer_lifetime The returned string is allocated and freed by GLFW.  You
 *  should not free it yourself.  It is valid until the specified monitor is
 *  disconnected or the library is terminated.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref monitor_properties
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup monitor
 */
GLFWAPI const char* glfwGetMonitorName(GLFWmonitor* monitor);

/*! @brief Sets the user pointer of the specified monitor.
 *
 *  This function sets the user-defined pointer of the specified monitor.  The
 *  current value is retained until the monitor is disconnected.  The initial
 *  value is `NULL`.
 *
 *  This function may be called from the monitor callback, even for a monitor
 *  that is being disconnected.
 *
 *  @param[in] monitor The monitor whose pointer to set.
 *  @param[in] pointer The new value.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function may be called from any thread.  Access is not
 *  synchronized.
 *
 *  @sa @ref monitor_userptr
 *  @sa @ref glfwGetMonitorUserPointer
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup monitor
 */
GLFWAPI void glfwSetMonitorUserPointer(GLFWmonitor* monitor, void* pointer);

/*! @brief Returns the user pointer of the specified monitor.
 *
 *  This function returns the current value of the user-defined pointer of the
 *  specified monitor.  The initial value is `NULL`.
 *
 *  This function may be called from the monitor callback, even for a monitor
 *  that is being disconnected.
 *
 *  @param[in] monitor The monitor whose pointer to return.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function may be called from any thread.  Access is not
 *  synchronized.
 *
 *  @sa @ref monitor_userptr
 *  @sa @ref glfwSetMonitorUserPointer
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup monitor
 */
GLFWAPI void* glfwGetMonitorUserPointer(GLFWmonitor* monitor);

/*! @brief Sets the monitor configuration callback.
 *
 *  This function sets the monitor configuration callback, or removes the
 *  currently set callback.  This is called when a monitor is connected to or
 *  disconnected from the system.
 *
 *  @param[in] callback The new callback, or `NULL` to remove the currently set
 *  callback.
 *  @return The previously set callback, or `NULL` if no callback was set or the
 *  library had not been [initialized](@ref intro_init).
 *
 *  @callback_signature
 *  @code
 *  void function_name(GLFWmonitor* monitor, int event)
 *  @endcode
 *  For more information about the callback parameters, see the
 *  [function pointer type](@ref GLFWmonitorfun).
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref monitor_event
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup monitor
 */
GLFWAPI GLFWmonitorfun glfwSetMonitorCallback(GLFWmonitorfun callback);

/*! @brief Returns the available video modes for the specified monitor.
 *
 *  This function returns an array of all video modes supported by the specified
 *  monitor.  The returned array is sorted in ascending order, first by color
 *  bit depth (the sum of all channel depths), then by resolution area (the
 *  product of width and height), then resolution width and finally by refresh
 *  rate.
 *
 *  @param[in] monitor The monitor to query.
 *  @param[out] count Where to store the number of video modes in the returned
 *  array.  This is set to zero if an error occurred.
 *  @return An array of video modes, or `NULL` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @pointer_lifetime The returned array is allocated and freed by GLFW.  You
 *  should not free it yourself.  It is valid until the specified monitor is
 *  disconnected, this function is called again for that monitor or the library
 *  is terminated.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref monitor_modes
 *  @sa @ref glfwGetVideoMode
 *
 *  @since Added in version 1.0.
 *  @glfw3 Changed to return an array of modes for a specific monitor.
 *
 *  @ingroup monitor
 */
GLFWAPI const GLFWvidmode* glfwGetVideoModes(GLFWmonitor* monitor, int* count);

/*! @brief Returns the current mode of the specified monitor.
 *
 *  This function returns the current video mode of the specified monitor.  If
 *  you have created a full screen window for that monitor, the return value
 *  will depend on whether that window is iconified.
 *
 *  @param[in] monitor The monitor to query.
 *  @return The current mode of the monitor, or `NULL` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @pointer_lifetime The returned array is allocated and freed by GLFW.  You
 *  should not free it yourself.  It is valid until the specified monitor is
 *  disconnected or the library is terminated.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref monitor_modes
 *  @sa @ref glfwGetVideoModes
 *
 *  @since Added in version 3.0.  Replaces `glfwGetDesktopMode`.
 *
 *  @ingroup monitor
 */
GLFWAPI const GLFWvidmode* glfwGetVideoMode(GLFWmonitor* monitor);

/*! @brief Generates a gamma ramp and sets it for the specified monitor.
 *
 *  This function generates an appropriately sized gamma ramp from the specified
 *  exponent and then calls @ref glfwSetGammaRamp with it.  The value must be
 *  a finite number greater than zero.
 *
 *  The software controlled gamma ramp is applied _in addition_ to the hardware
 *  gamma correction, which today is usually an approximation of sRGB gamma.
 *  This means that setting a perfectly linear ramp, or gamma 1.0, will produce
 *  the default (usually sRGB-like) behavior.
 *
 *  For gamma correct rendering with OpenGL or OpenGL ES, see the @ref
 *  GLFW_SRGB_CAPABLE hint.
 *
 *  @param[in] monitor The monitor whose gamma ramp to set.
 *  @param[in] gamma The desired exponent.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_INVALID_VALUE and @ref GLFW_PLATFORM_ERROR.
 *
 *  @remark @wayland Gamma handling is a privileged protocol, this function
 *  will thus never be implemented and emits @ref GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref monitor_gamma
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup monitor
 */
GLFWAPI void glfwSetGamma(GLFWmonitor* monitor, float gamma);

/*! @brief Returns the current gamma ramp for the specified monitor.
 *
 *  This function returns the current gamma ramp of the specified monitor.
 *
 *  @param[in] monitor The monitor to query.
 *  @return The current gamma ramp, or `NULL` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @remark @wayland Gamma handling is a privileged protocol, this function
 *  will thus never be implemented and emits @ref GLFW_PLATFORM_ERROR while
 *  returning `NULL`.
 *
 *  @pointer_lifetime The returned structure and its arrays are allocated and
 *  freed by GLFW.  You should not free them yourself.  They are valid until the
 *  specified monitor is disconnected, this function is called again for that
 *  monitor or the library is terminated.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref monitor_gamma
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup monitor
 */
GLFWAPI const GLFWgammaramp* glfwGetGammaRamp(GLFWmonitor* monitor);

/*! @brief Sets the current gamma ramp for the specified monitor.
 *
 *  This function sets the current gamma ramp for the specified monitor.  The
 *  original gamma ramp for that monitor is saved by GLFW the first time this
 *  function is called and is restored by @ref glfwTerminate.
 *
 *  The software controlled gamma ramp is applied _in addition_ to the hardware
 *  gamma correction, which today is usually an approximation of sRGB gamma.
 *  This means that setting a perfectly linear ramp, or gamma 1.0, will produce
 *  the default (usually sRGB-like) behavior.
 *
 *  For gamma correct rendering with OpenGL or OpenGL ES, see the @ref
 *  GLFW_SRGB_CAPABLE hint.
 *
 *  @param[in] monitor The monitor whose gamma ramp to set.
 *  @param[in] ramp The gamma ramp to use.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @remark The size of the specified gamma ramp should match the size of the
 *  current ramp for that monitor.
 *
 *  @remark @win32 The gamma ramp size must be 256.
 *
 *  @remark @wayland Gamma handling is a privileged protocol, this function
 *  will thus never be implemented and emits @ref GLFW_PLATFORM_ERROR.
 *
 *  @pointer_lifetime The specified gamma ramp is copied before this function
 *  returns.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref monitor_gamma
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup monitor
 */
GLFWAPI void glfwSetGammaRamp(GLFWmonitor* monitor, const GLFWgammaramp* ramp);

/*! @brief Resets all window hints to their default values.
 *
 *  This function resets all window hints to their
 *  [default values](@ref window_hints_values).
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_hints
 *  @sa @ref glfwWindowHint
 *  @sa @ref glfwWindowHintString
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup window
 */
GLFWAPI void glfwDefaultWindowHints(void);

/*! @brief Sets the specified window hint to the desired value.
 *
 *  This function sets hints for the next call to @ref glfwCreateWindow.  The
 *  hints, once set, retain their values until changed by a call to this
 *  function or @ref glfwDefaultWindowHints, or until the library is terminated.
 *
 *  Only integer value hints can be set with this function.  String value hints
 *  are set with @ref glfwWindowHintString.
 *
 *  This function does not check whether the specified hint values are valid.
 *  If you set hints to invalid values this will instead be reported by the next
 *  call to @ref glfwCreateWindow.
 *
 *  Some hints are platform specific.  These may be set on any platform but they
 *  will only affect their specific platform.  Other platforms will ignore them.
 *  Setting these hints requires no platform specific headers or functions.
 *
 *  @param[in] hint The [window hint](@ref window_hints) to set.
 *  @param[in] value The new value of the window hint.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_INVALID_ENUM.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_hints
 *  @sa @ref glfwWindowHintString
 *  @sa @ref glfwDefaultWindowHints
 *
 *  @since Added in version 3.0.  Replaces `glfwOpenWindowHint`.
 *
 *  @ingroup window
 */
GLFWAPI void glfwWindowHint(int hint, int value);

/*! @brief Sets the specified window hint to the desired value.
 *
 *  This function sets hints for the next call to @ref glfwCreateWindow.  The
 *  hints, once set, retain their values until changed by a call to this
 *  function or @ref glfwDefaultWindowHints, or until the library is terminated.
 *
 *  Only string type hints can be set with this function.  Integer value hints
 *  are set with @ref glfwWindowHint.
 *
 *  This function does not check whether the specified hint values are valid.
 *  If you set hints to invalid values this will instead be reported by the next
 *  call to @ref glfwCreateWindow.
 *
 *  Some hints are platform specific.  These may be set on any platform but they
 *  will only affect their specific platform.  Other platforms will ignore them.
 *  Setting these hints requires no platform specific headers or functions.
 *
 *  @param[in] hint The [window hint](@ref window_hints) to set.
 *  @param[in] value The new value of the window hint.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_INVALID_ENUM.
 *
 *  @pointer_lifetime The specified string is copied before this function
 *  returns.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_hints
 *  @sa @ref glfwWindowHint
 *  @sa @ref glfwDefaultWindowHints
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup window
 */
GLFWAPI void glfwWindowHintString(int hint, const char* value);

/*! @brief Creates a window and its associated context.
 *
 *  This function creates a window and its associated OpenGL or OpenGL ES
 *  context.  Most of the options controlling how the window and its context
 *  should be created are specified with [window hints](@ref window_hints).
 *
 *  Successful creation does not change which context is current.  Before you
 *  can use the newly created context, you need to
 *  [make it current](@ref context_current).  For information about the `share`
 *  parameter, see @ref context_sharing.
 *
 *  The created window, framebuffer and context may differ from what you
 *  requested, as not all parameters and hints are
 *  [hard constraints](@ref window_hints_hard).  This includes the size of the
 *  window, especially for full screen windows.  To query the actual attributes
 *  of the created window, framebuffer and context, see @ref
 *  glfwGetWindowAttrib, @ref glfwGetWindowSize and @ref glfwGetFramebufferSize.
 *
 *  To create a full screen window, you need to specify the monitor the window
 *  will cover.  If no monitor is specified, the window will be windowed mode.
 *  Unless you have a way for the user to choose a specific monitor, it is
 *  recommended that you pick the primary monitor.  For more information on how
 *  to query connected monitors, see @ref monitor_monitors.
 *
 *  For full screen windows, the specified size becomes the resolution of the
 *  window's _desired video mode_.  As long as a full screen window is not
 *  iconified, the supported video mode most closely matching the desired video
 *  mode is set for the specified monitor.  For more information about full
 *  screen windows, including the creation of so called _windowed full screen_
 *  or _borderless full screen_ windows, see @ref window_windowed_full_screen.
 *
 *  Once you have created the window, you can switch it between windowed and
 *  full screen mode with @ref glfwSetWindowMonitor.  This will not affect its
 *  OpenGL or OpenGL ES context.
 *
 *  By default, newly created windows use the placement recommended by the
 *  window system.  To create the window at a specific position, make it
 *  initially invisible using the [GLFW_VISIBLE](@ref GLFW_VISIBLE_hint) window
 *  hint, set its [position](@ref window_pos) and then [show](@ref window_hide)
 *  it.
 *
 *  As long as at least one full screen window is not iconified, the screensaver
 *  is prohibited from starting.
 *
 *  Window systems put limits on window sizes.  Very large or very small window
 *  dimensions may be overridden by the window system on creation.  Check the
 *  actual [size](@ref window_size) after creation.
 *
 *  The [swap interval](@ref buffer_swap) is not set during window creation and
 *  the initial value may vary depending on driver settings and defaults.
 *
 *  @param[in] width The desired width, in screen coordinates, of the window.
 *  This must be greater than zero.
 *  @param[in] height The desired height, in screen coordinates, of the window.
 *  This must be greater than zero.
 *  @param[in] title The initial, UTF-8 encoded window title.
 *  @param[in] monitor The monitor to use for full screen mode, or `NULL` for
 *  windowed mode.
 *  @param[in] share The window whose context to share resources with, or `NULL`
 *  to not share resources.
 *  @return The handle of the created window, or `NULL` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_INVALID_ENUM, @ref GLFW_INVALID_VALUE, @ref GLFW_API_UNAVAILABLE, @ref
 *  GLFW_VERSION_UNAVAILABLE, @ref GLFW_FORMAT_UNAVAILABLE and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @remark @win32 Window creation will fail if the Microsoft GDI software
 *  OpenGL implementation is the only one available.
 *
 *  @remark @win32 If the executable has an icon resource named `GLFW_ICON,` it
 *  will be set as the initial icon for the window.  If no such icon is present,
 *  the `IDI_APPLICATION` icon will be used instead.  To set a different icon,
 *  see @ref glfwSetWindowIcon.
 *
 *  @remark @win32 The context to share resources with must not be current on
 *  any other thread.
 *
 *  @remark @macos The OS only supports forward-compatible core profile contexts
 *  for OpenGL versions 3.2 and later.  Before creating an OpenGL context of
 *  version 3.2 or later you must set the
 *  [GLFW_OPENGL_FORWARD_COMPAT](@ref GLFW_OPENGL_FORWARD_COMPAT_hint) and
 *  [GLFW_OPENGL_PROFILE](@ref GLFW_OPENGL_PROFILE_hint) hints accordingly.
 *  OpenGL 3.0 and 3.1 contexts are not supported at all on macOS.
 *
 *  @remark @macos The GLFW window has no icon, as it is not a document
 *  window, but the dock icon will be the same as the application bundle's icon.
 *  For more information on bundles, see the
 *  [Bundle Programming Guide](https://developer.apple.com/library/mac/documentation/CoreFoundation/Conceptual/CFBundles/)
 *  in the Mac Developer Library.
 *
 *  @remark @macos The first time a window is created the menu bar is created.
 *  If GLFW finds a `MainMenu.nib` it is loaded and assumed to contain a menu
 *  bar.  Otherwise a minimal menu bar is created manually with common commands
 *  like Hide, Quit and About.  The About entry opens a minimal about dialog
 *  with information from the application's bundle.  Menu bar creation can be
 *  disabled entirely with the @ref GLFW_COCOA_MENUBAR init hint.
 *
 *  @remark @macos On OS X 10.10 and later the window frame will not be rendered
 *  at full resolution on Retina displays unless the
 *  [GLFW_COCOA_RETINA_FRAMEBUFFER](@ref GLFW_COCOA_RETINA_FRAMEBUFFER_hint)
 *  hint is `GLFW_TRUE` and the `NSHighResolutionCapable` key is enabled in the
 *  application bundle's `Info.plist`.  For more information, see
 *  [High Resolution Guidelines for OS X](https://developer.apple.com/library/mac/documentation/GraphicsAnimation/Conceptual/HighResolutionOSX/Explained/Explained.html)
 *  in the Mac Developer Library.  The GLFW test and example programs use
 *  a custom `Info.plist` template for this, which can be found as
 *  `CMake/MacOSXBundleInfo.plist.in` in the source tree.
 *
 *  @remark @macos When activating frame autosaving with
 *  [GLFW_COCOA_FRAME_NAME](@ref GLFW_COCOA_FRAME_NAME_hint), the specified
 *  window size and position may be overridden by previously saved values.
 *
 *  @remark @x11 Some window managers will not respect the placement of
 *  initially hidden windows.
 *
 *  @remark @x11 Due to the asynchronous nature of X11, it may take a moment for
 *  a window to reach its requested state.  This means you may not be able to
 *  query the final size, position or other attributes directly after window
 *  creation.
 *
 *  @remark @x11 The class part of the `WM_CLASS` window property will by
 *  default be set to the window title passed to this function.  The instance
 *  part will use the contents of the `RESOURCE_NAME` environment variable, if
 *  present and not empty, or fall back to the window title.  Set the
 *  [GLFW_X11_CLASS_NAME](@ref GLFW_X11_CLASS_NAME_hint) and
 *  [GLFW_X11_INSTANCE_NAME](@ref GLFW_X11_INSTANCE_NAME_hint) window hints to
 *  override this.
 *
 *  @remark @wayland Compositors should implement the xdg-decoration protocol
 *  for GLFW to decorate the window properly.  If this protocol isn't
 *  supported, or if the compositor prefers client-side decorations, a very
 *  simple fallback frame will be drawn using the wp_viewporter protocol.  A
 *  compositor can still emit close, maximize or fullscreen events, using for
 *  instance a keybind mechanism.  If neither of these protocols is supported,
 *  the window won't be decorated.
 *
 *  @remark @wayland A full screen window will not attempt to change the mode,
 *  no matter what the requested size or refresh rate.
 *
 *  @remark @wayland Screensaver inhibition requires the idle-inhibit protocol
 *  to be implemented in the user's compositor.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_creation
 *  @sa @ref glfwDestroyWindow
 *
 *  @since Added in version 3.0.  Replaces `glfwOpenWindow`.
 *
 *  @ingroup window
 */
GLFWAPI GLFWwindow* glfwCreateWindow(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share);

/*! @brief Destroys the specified window and its context.
 *
 *  This function destroys the specified window and its context.  On calling
 *  this function, no further callbacks will be called for that window.
 *
 *  If the context of the specified window is current on the main thread, it is
 *  detached before being destroyed.
 *
 *  @param[in] window The window to destroy.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @note The context of the specified window must not be current on any other
 *  thread when this function is called.
 *
 *  @reentrancy This function must not be called from a callback.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_creation
 *  @sa @ref glfwCreateWindow
 *
 *  @since Added in version 3.0.  Replaces `glfwCloseWindow`.
 *
 *  @ingroup window
 */
GLFWAPI void glfwDestroyWindow(GLFWwindow* window);

/*! @brief Checks the close flag of the specified window.
 *
 *  This function returns the value of the close flag of the specified window.
 *
 *  @param[in] window The window to query.
 *  @return The value of the close flag.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function may be called from any thread.  Access is not
 *  synchronized.
 *
 *  @sa @ref window_close
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup window
 */
GLFWAPI int glfwWindowShouldClose(GLFWwindow* window);

/*! @brief Sets the close flag of the specified window.
 *
 *  This function sets the value of the close flag of the specified window.
 *  This can be used to override the user's attempt to close the window, or
 *  to signal that it should be closed.
 *
 *  @param[in] window The window whose flag to change.
 *  @param[in] value The new value.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function may be called from any thread.  Access is not
 *  synchronized.
 *
 *  @sa @ref window_close
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup window
 */
GLFWAPI void glfwSetWindowShouldClose(GLFWwindow* window, int value);

/*! @brief Sets the title of the specified window.
 *
 *  This function sets the window title, encoded as UTF-8, of the specified
 *  window.
 *
 *  @param[in] window The window whose title to change.
 *  @param[in] title The UTF-8 encoded window title.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @remark @macos The window title will not be updated until the next time you
 *  process events.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_title
 *
 *  @since Added in version 1.0.
 *  @glfw3 Added window handle parameter.
 *
 *  @ingroup window
 */
GLFWAPI void glfwSetWindowTitle(GLFWwindow* window, const char* title);

/*! @brief Sets the icon for the specified window.
 *
 *  This function sets the icon of the specified window.  If passed an array of
 *  candidate images, those of or closest to the sizes desired by the system are
 *  selected.  If no images are specified, the window reverts to its default
 *  icon.
 *
 *  The pixels are 32-bit, little-endian, non-premultiplied RGBA, i.e. eight
 *  bits per channel with the red channel first.  They are arranged canonically
 *  as packed sequential rows, starting from the top-left corner.
 *
 *  The desired image sizes varies depending on platform and system settings.
 *  The selected images will be rescaled as needed.  Good sizes include 16x16,
 *  32x32 and 48x48.
 *
 *  @param[in] window The window whose icon to set.
 *  @param[in] count The number of images in the specified array, or zero to
 *  revert to the default window icon.
 *  @param[in] images The images to create the icon from.  This is ignored if
 *  count is zero.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_INVALID_VALUE and @ref GLFW_PLATFORM_ERROR.
 *
 *  @pointer_lifetime The specified image data is copied before this function
 *  returns.
 *
 *  @remark @macos The GLFW window has no icon, as it is not a document
 *  window, so this function does nothing.  The dock icon will be the same as
 *  the application bundle's icon.  For more information on bundles, see the
 *  [Bundle Programming Guide](https://developer.apple.com/library/mac/documentation/CoreFoundation/Conceptual/CFBundles/)
 *  in the Mac Developer Library.
 *
 *  @remark @wayland There is no existing protocol to change an icon, the
 *  window will thus inherit the one defined in the application's desktop file.
 *  This function always emits @ref GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_icon
 *
 *  @since Added in version 3.2.
 *
 *  @ingroup window
 */
GLFWAPI void glfwSetWindowIcon(GLFWwindow* window, int count, const GLFWimage* images);

/*! @brief Retrieves the position of the content area of the specified window.
 *
 *  This function retrieves the position, in screen coordinates, of the
 *  upper-left corner of the content area of the specified window.
 *
 *  Any or all of the position arguments may be `NULL`.  If an error occurs, all
 *  non-`NULL` position arguments will be set to zero.
 *
 *  @param[in] window The window to query.
 *  @param[out] xpos Where to store the x-coordinate of the upper-left corner of
 *  the content area, or `NULL`.
 *  @param[out] ypos Where to store the y-coordinate of the upper-left corner of
 *  the content area, or `NULL`.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @remark @wayland There is no way for an application to retrieve the global
 *  position of its windows, this function will always emit @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_pos
 *  @sa @ref glfwSetWindowPos
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup window
 */
GLFWAPI void glfwGetWindowPos(GLFWwindow* window, int* xpos, int* ypos);

/*! @brief Sets the position of the content area of the specified window.
 *
 *  This function sets the position, in screen coordinates, of the upper-left
 *  corner of the content area of the specified windowed mode window.  If the
 *  window is a full screen window, this function does nothing.
 *
 *  __Do not use this function__ to move an already visible window unless you
 *  have very good reasons for doing so, as it will confuse and annoy the user.
 *
 *  The window manager may put limits on what positions are allowed.  GLFW
 *  cannot and should not override these limits.
 *
 *  @param[in] window The window to query.
 *  @param[in] xpos The x-coordinate of the upper-left corner of the content area.
 *  @param[in] ypos The y-coordinate of the upper-left corner of the content area.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @remark @wayland There is no way for an application to set the global
 *  position of its windows, this function will always emit @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_pos
 *  @sa @ref glfwGetWindowPos
 *
 *  @since Added in version 1.0.
 *  @glfw3 Added window handle parameter.
 *
 *  @ingroup window
 */
GLFWAPI void glfwSetWindowPos(GLFWwindow* window, int xpos, int ypos);

/*! @brief Retrieves the size of the content area of the specified window.
 *
 *  This function retrieves the size, in screen coordinates, of the content area
 *  of the specified window.  If you wish to retrieve the size of the
 *  framebuffer of the window in pixels, see @ref glfwGetFramebufferSize.
 *
 *  Any or all of the size arguments may be `NULL`.  If an error occurs, all
 *  non-`NULL` size arguments will be set to zero.
 *
 *  @param[in] window The window whose size to retrieve.
 *  @param[out] width Where to store the width, in screen coordinates, of the
 *  content area, or `NULL`.
 *  @param[out] height Where to store the height, in screen coordinates, of the
 *  content area, or `NULL`.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_size
 *  @sa @ref glfwSetWindowSize
 *
 *  @since Added in version 1.0.
 *  @glfw3 Added window handle parameter.
 *
 *  @ingroup window
 */
GLFWAPI void glfwGetWindowSize(GLFWwindow* window, int* width, int* height);

/*! @brief Sets the size limits of the specified window.
 *
 *  This function sets the size limits of the content area of the specified
 *  window.  If the window is full screen, the size limits only take effect
 *  once it is made windowed.  If the window is not resizable, this function
 *  does nothing.
 *
 *  The size limits are applied immediately to a windowed mode window and may
 *  cause it to be resized.
 *
 *  The maximum dimensions must be greater than or equal to the minimum
 *  dimensions and all must be greater than or equal to zero.
 *
 *  @param[in] window The window to set limits for.
 *  @param[in] minwidth The minimum width, in screen coordinates, of the content
 *  area, or `GLFW_DONT_CARE`.
 *  @param[in] minheight The minimum height, in screen coordinates, of the
 *  content area, or `GLFW_DONT_CARE`.
 *  @param[in] maxwidth The maximum width, in screen coordinates, of the content
 *  area, or `GLFW_DONT_CARE`.
 *  @param[in] maxheight The maximum height, in screen coordinates, of the
 *  content area, or `GLFW_DONT_CARE`.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_INVALID_VALUE and @ref GLFW_PLATFORM_ERROR.
 *
 *  @remark If you set size limits and an aspect ratio that conflict, the
 *  results are undefined.
 *
 *  @remark @wayland The size limits will not be applied until the window is
 *  actually resized, either by the user or by the compositor.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_sizelimits
 *  @sa @ref glfwSetWindowAspectRatio
 *
 *  @since Added in version 3.2.
 *
 *  @ingroup window
 */
GLFWAPI void glfwSetWindowSizeLimits(GLFWwindow* window, int minwidth, int minheight, int maxwidth, int maxheight);

/*! @brief Sets the aspect ratio of the specified window.
 *
 *  This function sets the required aspect ratio of the content area of the
 *  specified window.  If the window is full screen, the aspect ratio only takes
 *  effect once it is made windowed.  If the window is not resizable, this
 *  function does nothing.
 *
 *  The aspect ratio is specified as a numerator and a denominator and both
 *  values must be greater than zero.  For example, the common 16:9 aspect ratio
 *  is specified as 16 and 9, respectively.
 *
 *  If the numerator and denominator is set to `GLFW_DONT_CARE` then the aspect
 *  ratio limit is disabled.
 *
 *  The aspect ratio is applied immediately to a windowed mode window and may
 *  cause it to be resized.
 *
 *  @param[in] window The window to set limits for.
 *  @param[in] numer The numerator of the desired aspect ratio, or
 *  `GLFW_DONT_CARE`.
 *  @param[in] denom The denominator of the desired aspect ratio, or
 *  `GLFW_DONT_CARE`.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_INVALID_VALUE and @ref GLFW_PLATFORM_ERROR.
 *
 *  @remark If you set size limits and an aspect ratio that conflict, the
 *  results are undefined.
 *
 *  @remark @wayland The aspect ratio will not be applied until the window is
 *  actually resized, either by the user or by the compositor.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_sizelimits
 *  @sa @ref glfwSetWindowSizeLimits
 *
 *  @since Added in version 3.2.
 *
 *  @ingroup window
 */
GLFWAPI void glfwSetWindowAspectRatio(GLFWwindow* window, int numer, int denom);

/*! @brief Sets the size of the content area of the specified window.
 *
 *  This function sets the size, in screen coordinates, of the content area of
 *  the specified window.
 *
 *  For full screen windows, this function updates the resolution of its desired
 *  video mode and switches to the video mode closest to it, without affecting
 *  the window's context.  As the context is unaffected, the bit depths of the
 *  framebuffer remain unchanged.
 *
 *  If you wish to update the refresh rate of the desired video mode in addition
 *  to its resolution, see @ref glfwSetWindowMonitor.
 *
 *  The window manager may put limits on what sizes are allowed.  GLFW cannot
 *  and should not override these limits.
 *
 *  @param[in] window The window to resize.
 *  @param[in] width The desired width, in screen coordinates, of the window
 *  content area.
 *  @param[in] height The desired height, in screen coordinates, of the window
 *  content area.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @remark @wayland A full screen window will not attempt to change the mode,
 *  no matter what the requested size.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_size
 *  @sa @ref glfwGetWindowSize
 *  @sa @ref glfwSetWindowMonitor
 *
 *  @since Added in version 1.0.
 *  @glfw3 Added window handle parameter.
 *
 *  @ingroup window
 */
GLFWAPI void glfwSetWindowSize(GLFWwindow* window, int width, int height);

/*! @brief Retrieves the size of the framebuffer of the specified window.
 *
 *  This function retrieves the size, in pixels, of the framebuffer of the
 *  specified window.  If you wish to retrieve the size of the window in screen
 *  coordinates, see @ref glfwGetWindowSize.
 *
 *  Any or all of the size arguments may be `NULL`.  If an error occurs, all
 *  non-`NULL` size arguments will be set to zero.
 *
 *  @param[in] window The window whose framebuffer to query.
 *  @param[out] width Where to store the width, in pixels, of the framebuffer,
 *  or `NULL`.
 *  @param[out] height Where to store the height, in pixels, of the framebuffer,
 *  or `NULL`.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_fbsize
 *  @sa @ref glfwSetFramebufferSizeCallback
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup window
 */
GLFWAPI void glfwGetFramebufferSize(GLFWwindow* window, int* width, int* height);

/*! @brief Retrieves the size of the frame of the window.
 *
 *  This function retrieves the size, in screen coordinates, of each edge of the
 *  frame of the specified window.  This size includes the title bar, if the
 *  window has one.  The size of the frame may vary depending on the
 *  [window-related hints](@ref window_hints_wnd) used to create it.
 *
 *  Because this function retrieves the size of each window frame edge and not
 *  the offset along a particular coordinate axis, the retrieved values will
 *  always be zero or positive.
 *
 *  Any or all of the size arguments may be `NULL`.  If an error occurs, all
 *  non-`NULL` size arguments will be set to zero.
 *
 *  @param[in] window The window whose frame size to query.
 *  @param[out] left Where to store the size, in screen coordinates, of the left
 *  edge of the window frame, or `NULL`.
 *  @param[out] top Where to store the size, in screen coordinates, of the top
 *  edge of the window frame, or `NULL`.
 *  @param[out] right Where to store the size, in screen coordinates, of the
 *  right edge of the window frame, or `NULL`.
 *  @param[out] bottom Where to store the size, in screen coordinates, of the
 *  bottom edge of the window frame, or `NULL`.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_size
 *
 *  @since Added in version 3.1.
 *
 *  @ingroup window
 */
GLFWAPI void glfwGetWindowFrameSize(GLFWwindow* window, int* left, int* top, int* right, int* bottom);

/*! @brief Retrieves the content scale for the specified window.
 *
 *  This function retrieves the content scale for the specified window.  The
 *  content scale is the ratio between the current DPI and the platform's
 *  default DPI.  This is especially important for text and any UI elements.  If
 *  the pixel dimensions of your UI scaled by this look appropriate on your
 *  machine then it should appear at a reasonable size on other machines
 *  regardless of their DPI and scaling settings.  This relies on the system DPI
 *  and scaling settings being somewhat correct.
 *
 *  On systems where each monitors can have its own content scale, the window
 *  content scale will depend on which monitor the system considers the window
 *  to be on.
 *
 *  @param[in] window The window to query.
 *  @param[out] xscale Where to store the x-axis content scale, or `NULL`.
 *  @param[out] yscale Where to store the y-axis content scale, or `NULL`.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_scale
 *  @sa @ref glfwSetWindowContentScaleCallback
 *  @sa @ref glfwGetMonitorContentScale
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup window
 */
GLFWAPI void glfwGetWindowContentScale(GLFWwindow* window, float* xscale, float* yscale);

/*! @brief Returns the opacity of the whole window.
 *
 *  This function returns the opacity of the window, including any decorations.
 *
 *  The opacity (or alpha) value is a positive finite number between zero and
 *  one, where zero is fully transparent and one is fully opaque.  If the system
 *  does not support whole window transparency, this function always returns one.
 *
 *  The initial opacity value for newly created windows is one.
 *
 *  @param[in] window The window to query.
 *  @return The opacity value of the specified window.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_transparency
 *  @sa @ref glfwSetWindowOpacity
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup window
 */
GLFWAPI float glfwGetWindowOpacity(GLFWwindow* window);

/*! @brief Sets the opacity of the whole window.
 *
 *  This function sets the opacity of the window, including any decorations.
 *
 *  The opacity (or alpha) value is a positive finite number between zero and
 *  one, where zero is fully transparent and one is fully opaque.
 *
 *  The initial opacity value for newly created windows is one.
 *
 *  A window created with framebuffer transparency may not use whole window
 *  transparency.  The results of doing this are undefined.
 *
 *  @param[in] window The window to set the opacity for.
 *  @param[in] opacity The desired opacity of the specified window.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_transparency
 *  @sa @ref glfwGetWindowOpacity
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup window
 */
GLFWAPI void glfwSetWindowOpacity(GLFWwindow* window, float opacity);

/*! @brief Iconifies the specified window.
 *
 *  This function iconifies (minimizes) the specified window if it was
 *  previously restored.  If the window is already iconified, this function does
 *  nothing.
 *
 *  If the specified window is a full screen window, GLFW restores the original
 *  video mode of the monitor.  The window's desired video mode is set again
 *  when the window is restored.
 *
 *  @param[in] window The window to iconify.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_iconify
 *  @sa @ref glfwRestoreWindow
 *  @sa @ref glfwMaximizeWindow
 *
 *  @since Added in version 2.1.
 *  @glfw3 Added window handle parameter.
 *
 *  @ingroup window
 */
GLFWAPI void glfwIconifyWindow(GLFWwindow* window);

/*! @brief Restores the specified window.
 *
 *  This function restores the specified window if it was previously iconified
 *  (minimized) or maximized.  If the window is already restored, this function
 *  does nothing.
 *
 *  If the specified window is an iconified full screen window, its desired
 *  video mode is set again for its monitor when the window is restored.
 *
 *  @param[in] window The window to restore.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_iconify
 *  @sa @ref glfwIconifyWindow
 *  @sa @ref glfwMaximizeWindow
 *
 *  @since Added in version 2.1.
 *  @glfw3 Added window handle parameter.
 *
 *  @ingroup window
 */
GLFWAPI void glfwRestoreWindow(GLFWwindow* window);

/*! @brief Maximizes the specified window.
 *
 *  This function maximizes the specified window if it was previously not
 *  maximized.  If the window is already maximized, this function does nothing.
 *
 *  If the specified window is a full screen window, this function does nothing.
 *
 *  @param[in] window The window to maximize.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @par Thread Safety
 *  This function may only be called from the main thread.
 *
 *  @sa @ref window_iconify
 *  @sa @ref glfwIconifyWindow
 *  @sa @ref glfwRestoreWindow
 *
 *  @since Added in GLFW 3.2.
 *
 *  @ingroup window
 */
GLFWAPI void glfwMaximizeWindow(GLFWwindow* window);

/*! @brief Makes the specified window visible.
 *
 *  This function makes the specified window visible if it was previously
 *  hidden.  If the window is already visible or is in full screen mode, this
 *  function does nothing.
 *
 *  By default, windowed mode windows are focused when shown
 *  Set the [GLFW_FOCUS_ON_SHOW](@ref GLFW_FOCUS_ON_SHOW_hint) window hint
 *  to change this behavior for all newly created windows, or change the
 *  behavior for an existing window with @ref glfwSetWindowAttrib.
 *
 *  @param[in] window The window to make visible.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @remark @wayland Because Wayland wants every frame of the desktop to be
 *  complete, this function does not immediately make the window visible.
 *  Instead it will become visible the next time the window framebuffer is
 *  updated after this call.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_hide
 *  @sa @ref glfwHideWindow
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup window
 */
GLFWAPI void glfwShowWindow(GLFWwindow* window);

/*! @brief Hides the specified window.
 *
 *  This function hides the specified window if it was previously visible.  If
 *  the window is already hidden or is in full screen mode, this function does
 *  nothing.
 *
 *  @param[in] window The window to hide.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_hide
 *  @sa @ref glfwShowWindow
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup window
 */
GLFWAPI void glfwHideWindow(GLFWwindow* window);

/*! @brief Brings the specified window to front and sets input focus.
 *
 *  This function brings the specified window to front and sets input focus.
 *  The window should already be visible and not iconified.
 *
 *  By default, both windowed and full screen mode windows are focused when
 *  initially created.  Set the [GLFW_FOCUSED](@ref GLFW_FOCUSED_hint) to
 *  disable this behavior.
 *
 *  Also by default, windowed mode windows are focused when shown
 *  with @ref glfwShowWindow. Set the
 *  [GLFW_FOCUS_ON_SHOW](@ref GLFW_FOCUS_ON_SHOW_hint) to disable this behavior.
 *
 *  __Do not use this function__ to steal focus from other applications unless
 *  you are certain that is what the user wants.  Focus stealing can be
 *  extremely disruptive.
 *
 *  For a less disruptive way of getting the user's attention, see
 *  [attention requests](@ref window_attention).
 *
 *  @param[in] window The window to give input focus.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @remark @wayland It is not possible for an application to bring its windows
 *  to front, this function will always emit @ref GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_focus
 *  @sa @ref window_attention
 *
 *  @since Added in version 3.2.
 *
 *  @ingroup window
 */
GLFWAPI void glfwFocusWindow(GLFWwindow* window);

/*! @brief Requests user attention to the specified window.
 *
 *  This function requests user attention to the specified window.  On
 *  platforms where this is not supported, attention is requested to the
 *  application as a whole.
 *
 *  Once the user has given attention, usually by focusing the window or
 *  application, the system will end the request automatically.
 *
 *  @param[in] window The window to request attention to.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @remark @macos Attention is requested to the application as a whole, not the
 *  specific window.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_attention
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup window
 */
GLFWAPI void glfwRequestWindowAttention(GLFWwindow* window);

/*! @brief Returns the monitor that the window uses for full screen mode.
 *
 *  This function returns the handle of the monitor that the specified window is
 *  in full screen on.
 *
 *  @param[in] window The window to query.
 *  @return The monitor, or `NULL` if the window is in windowed mode or an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_monitor
 *  @sa @ref glfwSetWindowMonitor
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup window
 */
GLFWAPI GLFWmonitor* glfwGetWindowMonitor(GLFWwindow* window);

/*! @brief Sets the mode, monitor, video mode and placement of a window.
 *
 *  This function sets the monitor that the window uses for full screen mode or,
 *  if the monitor is `NULL`, makes it windowed mode.
 *
 *  When setting a monitor, this function updates the width, height and refresh
 *  rate of the desired video mode and switches to the video mode closest to it.
 *  The window position is ignored when setting a monitor.
 *
 *  When the monitor is `NULL`, the position, width and height are used to
 *  place the window content area.  The refresh rate is ignored when no monitor
 *  is specified.
 *
 *  If you only wish to update the resolution of a full screen window or the
 *  size of a windowed mode window, see @ref glfwSetWindowSize.
 *
 *  When a window transitions from full screen to windowed mode, this function
 *  restores any previous window settings such as whether it is decorated,
 *  floating, resizable, has size or aspect ratio limits, etc.
 *
 *  @param[in] window The window whose monitor, size or video mode to set.
 *  @param[in] monitor The desired monitor, or `NULL` to set windowed mode.
 *  @param[in] xpos The desired x-coordinate of the upper-left corner of the
 *  content area.
 *  @param[in] ypos The desired y-coordinate of the upper-left corner of the
 *  content area.
 *  @param[in] width The desired with, in screen coordinates, of the content
 *  area or video mode.
 *  @param[in] height The desired height, in screen coordinates, of the content
 *  area or video mode.
 *  @param[in] refreshRate The desired refresh rate, in Hz, of the video mode,
 *  or `GLFW_DONT_CARE`.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @remark The OpenGL or OpenGL ES context will not be destroyed or otherwise
 *  affected by any resizing or mode switching, although you may need to update
 *  your viewport if the framebuffer size has changed.
 *
 *  @remark @wayland The desired window position is ignored, as there is no way
 *  for an application to set this property.
 *
 *  @remark @wayland Setting the window to full screen will not attempt to
 *  change the mode, no matter what the requested size or refresh rate.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_monitor
 *  @sa @ref window_full_screen
 *  @sa @ref glfwGetWindowMonitor
 *  @sa @ref glfwSetWindowSize
 *
 *  @since Added in version 3.2.
 *
 *  @ingroup window
 */
GLFWAPI void glfwSetWindowMonitor(GLFWwindow* window, GLFWmonitor* monitor, int xpos, int ypos, int width, int height, int refreshRate);

/*! @brief Returns an attribute of the specified window.
 *
 *  This function returns the value of an attribute of the specified window or
 *  its OpenGL or OpenGL ES context.
 *
 *  @param[in] window The window to query.
 *  @param[in] attrib The [window attribute](@ref window_attribs) whose value to
 *  return.
 *  @return The value of the attribute, or zero if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_INVALID_ENUM and @ref GLFW_PLATFORM_ERROR.
 *
 *  @remark Framebuffer related hints are not window attributes.  See @ref
 *  window_attribs_fb for more information.
 *
 *  @remark Zero is a valid value for many window and context related
 *  attributes so you cannot use a return value of zero as an indication of
 *  errors.  However, this function should not fail as long as it is passed
 *  valid arguments and the library has been [initialized](@ref intro_init).
 *
 *  @remark @wayland The Wayland protocol provides no way to check whether a
 *  window is iconfied, so @ref GLFW_ICONIFIED always returns `GLFW_FALSE`.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_attribs
 *  @sa @ref glfwSetWindowAttrib
 *
 *  @since Added in version 3.0.  Replaces `glfwGetWindowParam` and
 *  `glfwGetGLVersion`.
 *
 *  @ingroup window
 */
GLFWAPI int glfwGetWindowAttrib(GLFWwindow* window, int attrib);

/*! @brief Sets an attribute of the specified window.
 *
 *  This function sets the value of an attribute of the specified window.
 *
 *  The supported attributes are [GLFW_DECORATED](@ref GLFW_DECORATED_attrib),
 *  [GLFW_RESIZABLE](@ref GLFW_RESIZABLE_attrib),
 *  [GLFW_FLOATING](@ref GLFW_FLOATING_attrib),
 *  [GLFW_AUTO_ICONIFY](@ref GLFW_AUTO_ICONIFY_attrib) and
 *  [GLFW_FOCUS_ON_SHOW](@ref GLFW_FOCUS_ON_SHOW_attrib).
 *
 *  Some of these attributes are ignored for full screen windows.  The new
 *  value will take effect if the window is later made windowed.
 *
 *  Some of these attributes are ignored for windowed mode windows.  The new
 *  value will take effect if the window is later made full screen.
 *
 *  @param[in] window The window to set the attribute for.
 *  @param[in] attrib A supported window attribute.
 *  @param[in] value `GLFW_TRUE` or `GLFW_FALSE`.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_INVALID_ENUM, @ref GLFW_INVALID_VALUE and @ref GLFW_PLATFORM_ERROR.
 *
 *  @remark Calling @ref glfwGetWindowAttrib will always return the latest
 *  value, even if that value is ignored by the current mode of the window.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_attribs
 *  @sa @ref glfwGetWindowAttrib
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup window
 */
GLFWAPI void glfwSetWindowAttrib(GLFWwindow* window, int attrib, int value);

/*! @brief Sets the user pointer of the specified window.
 *
 *  This function sets the user-defined pointer of the specified window.  The
 *  current value is retained until the window is destroyed.  The initial value
 *  is `NULL`.
 *
 *  @param[in] window The window whose pointer to set.
 *  @param[in] pointer The new value.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function may be called from any thread.  Access is not
 *  synchronized.
 *
 *  @sa @ref window_userptr
 *  @sa @ref glfwGetWindowUserPointer
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup window
 */
GLFWAPI void glfwSetWindowUserPointer(GLFWwindow* window, void* pointer);

/*! @brief Returns the user pointer of the specified window.
 *
 *  This function returns the current value of the user-defined pointer of the
 *  specified window.  The initial value is `NULL`.
 *
 *  @param[in] window The window whose pointer to return.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function may be called from any thread.  Access is not
 *  synchronized.
 *
 *  @sa @ref window_userptr
 *  @sa @ref glfwSetWindowUserPointer
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup window
 */
GLFWAPI void* glfwGetWindowUserPointer(GLFWwindow* window);

/*! @brief Sets the position callback for the specified window.
 *
 *  This function sets the position callback of the specified window, which is
 *  called when the window is moved.  The callback is provided with the
 *  position, in screen coordinates, of the upper-left corner of the content
 *  area of the window.
 *
 *  @param[in] window The window whose callback to set.
 *  @param[in] callback The new callback, or `NULL` to remove the currently set
 *  callback.
 *  @return The previously set callback, or `NULL` if no callback was set or the
 *  library had not been [initialized](@ref intro_init).
 *
 *  @callback_signature
 *  @code
 *  void function_name(GLFWwindow* window, int xpos, int ypos)
 *  @endcode
 *  For more information about the callback parameters, see the
 *  [function pointer type](@ref GLFWwindowposfun).
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @remark @wayland This callback will never be called, as there is no way for
 *  an application to know its global position.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_pos
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup window
 */
GLFWAPI GLFWwindowposfun glfwSetWindowPosCallback(GLFWwindow* window, GLFWwindowposfun callback);

/*! @brief Sets the size callback for the specified window.
 *
 *  This function sets the size callback of the specified window, which is
 *  called when the window is resized.  The callback is provided with the size,
 *  in screen coordinates, of the content area of the window.
 *
 *  @param[in] window The window whose callback to set.
 *  @param[in] callback The new callback, or `NULL` to remove the currently set
 *  callback.
 *  @return The previously set callback, or `NULL` if no callback was set or the
 *  library had not been [initialized](@ref intro_init).
 *
 *  @callback_signature
 *  @code
 *  void function_name(GLFWwindow* window, int width, int height)
 *  @endcode
 *  For more information about the callback parameters, see the
 *  [function pointer type](@ref GLFWwindowsizefun).
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_size
 *
 *  @since Added in version 1.0.
 *  @glfw3 Added window handle parameter and return value.
 *
 *  @ingroup window
 */
GLFWAPI GLFWwindowsizefun glfwSetWindowSizeCallback(GLFWwindow* window, GLFWwindowsizefun callback);

/*! @brief Sets the close callback for the specified window.
 *
 *  This function sets the close callback of the specified window, which is
 *  called when the user attempts to close the window, for example by clicking
 *  the close widget in the title bar.
 *
 *  The close flag is set before this callback is called, but you can modify it
 *  at any time with @ref glfwSetWindowShouldClose.
 *
 *  The close callback is not triggered by @ref glfwDestroyWindow.
 *
 *  @param[in] window The window whose callback to set.
 *  @param[in] callback The new callback, or `NULL` to remove the currently set
 *  callback.
 *  @return The previously set callback, or `NULL` if no callback was set or the
 *  library had not been [initialized](@ref intro_init).
 *
 *  @callback_signature
 *  @code
 *  void function_name(GLFWwindow* window)
 *  @endcode
 *  For more information about the callback parameters, see the
 *  [function pointer type](@ref GLFWwindowclosefun).
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @remark @macos Selecting Quit from the application menu will trigger the
 *  close callback for all windows.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_close
 *
 *  @since Added in version 2.5.
 *  @glfw3 Added window handle parameter and return value.
 *
 *  @ingroup window
 */
GLFWAPI GLFWwindowclosefun glfwSetWindowCloseCallback(GLFWwindow* window, GLFWwindowclosefun callback);

/*! @brief Sets the refresh callback for the specified window.
 *
 *  This function sets the refresh callback of the specified window, which is
 *  called when the content area of the window needs to be redrawn, for example
 *  if the window has been exposed after having been covered by another window.
 *
 *  On compositing window systems such as Aero, Compiz, Aqua or Wayland, where
 *  the window contents are saved off-screen, this callback may be called only
 *  very infrequently or never at all.
 *
 *  @param[in] window The window whose callback to set.
 *  @param[in] callback The new callback, or `NULL` to remove the currently set
 *  callback.
 *  @return The previously set callback, or `NULL` if no callback was set or the
 *  library had not been [initialized](@ref intro_init).
 *
 *  @callback_signature
 *  @code
 *  void function_name(GLFWwindow* window);
 *  @endcode
 *  For more information about the callback parameters, see the
 *  [function pointer type](@ref GLFWwindowrefreshfun).
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_refresh
 *
 *  @since Added in version 2.5.
 *  @glfw3 Added window handle parameter and return value.
 *
 *  @ingroup window
 */
GLFWAPI GLFWwindowrefreshfun glfwSetWindowRefreshCallback(GLFWwindow* window, GLFWwindowrefreshfun callback);

/*! @brief Sets the focus callback for the specified window.
 *
 *  This function sets the focus callback of the specified window, which is
 *  called when the window gains or loses input focus.
 *
 *  After the focus callback is called for a window that lost input focus,
 *  synthetic key and mouse button release events will be generated for all such
 *  that had been pressed.  For more information, see @ref glfwSetKeyCallback
 *  and @ref glfwSetMouseButtonCallback.
 *
 *  @param[in] window The window whose callback to set.
 *  @param[in] callback The new callback, or `NULL` to remove the currently set
 *  callback.
 *  @return The previously set callback, or `NULL` if no callback was set or the
 *  library had not been [initialized](@ref intro_init).
 *
 *  @callback_signature
 *  @code
 *  void function_name(GLFWwindow* window, int focused)
 *  @endcode
 *  For more information about the callback parameters, see the
 *  [function pointer type](@ref GLFWwindowfocusfun).
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_focus
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup window
 */
GLFWAPI GLFWwindowfocusfun glfwSetWindowFocusCallback(GLFWwindow* window, GLFWwindowfocusfun callback);

/*! @brief Sets the iconify callback for the specified window.
 *
 *  This function sets the iconification callback of the specified window, which
 *  is called when the window is iconified or restored.
 *
 *  @param[in] window The window whose callback to set.
 *  @param[in] callback The new callback, or `NULL` to remove the currently set
 *  callback.
 *  @return The previously set callback, or `NULL` if no callback was set or the
 *  library had not been [initialized](@ref intro_init).
 *
 *  @callback_signature
 *  @code
 *  void function_name(GLFWwindow* window, int iconified)
 *  @endcode
 *  For more information about the callback parameters, see the
 *  [function pointer type](@ref GLFWwindowiconifyfun).
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @remark @wayland The XDG-shell protocol has no event for iconification, so
 *  this callback will never be called.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_iconify
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup window
 */
GLFWAPI GLFWwindowiconifyfun glfwSetWindowIconifyCallback(GLFWwindow* window, GLFWwindowiconifyfun callback);

/*! @brief Sets the maximize callback for the specified window.
 *
 *  This function sets the maximization callback of the specified window, which
 *  is called when the window is maximized or restored.
 *
 *  @param[in] window The window whose callback to set.
 *  @param[in] callback The new callback, or `NULL` to remove the currently set
 *  callback.
 *  @return The previously set callback, or `NULL` if no callback was set or the
 *  library had not been [initialized](@ref intro_init).
 *
 *  @callback_signature
 *  @code
 *  void function_name(GLFWwindow* window, int maximized)
 *  @endcode
 *  For more information about the callback parameters, see the
 *  [function pointer type](@ref GLFWwindowmaximizefun).
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_maximize
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup window
 */
GLFWAPI GLFWwindowmaximizefun glfwSetWindowMaximizeCallback(GLFWwindow* window, GLFWwindowmaximizefun callback);

/*! @brief Sets the framebuffer resize callback for the specified window.
 *
 *  This function sets the framebuffer resize callback of the specified window,
 *  which is called when the framebuffer of the specified window is resized.
 *
 *  @param[in] window The window whose callback to set.
 *  @param[in] callback The new callback, or `NULL` to remove the currently set
 *  callback.
 *  @return The previously set callback, or `NULL` if no callback was set or the
 *  library had not been [initialized](@ref intro_init).
 *
 *  @callback_signature
 *  @code
 *  void function_name(GLFWwindow* window, int width, int height)
 *  @endcode
 *  For more information about the callback parameters, see the
 *  [function pointer type](@ref GLFWframebuffersizefun).
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_fbsize
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup window
 */
GLFWAPI GLFWframebuffersizefun glfwSetFramebufferSizeCallback(GLFWwindow* window, GLFWframebuffersizefun callback);

/*! @brief Sets the window content scale callback for the specified window.
 *
 *  This function sets the window content scale callback of the specified window,
 *  which is called when the content scale of the specified window changes.
 *
 *  @param[in] window The window whose callback to set.
 *  @param[in] callback The new callback, or `NULL` to remove the currently set
 *  callback.
 *  @return The previously set callback, or `NULL` if no callback was set or the
 *  library had not been [initialized](@ref intro_init).
 *
 *  @callback_signature
 *  @code
 *  void function_name(GLFWwindow* window, float xscale, float yscale)
 *  @endcode
 *  For more information about the callback parameters, see the
 *  [function pointer type](@ref GLFWwindowcontentscalefun).
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref window_scale
 *  @sa @ref glfwGetWindowContentScale
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup window
 */
GLFWAPI GLFWwindowcontentscalefun glfwSetWindowContentScaleCallback(GLFWwindow* window, GLFWwindowcontentscalefun callback);

/*! @brief Processes all pending events.
 *
 *  This function processes only those events that are already in the event
 *  queue and then returns immediately.  Processing events will cause the window
 *  and input callbacks associated with those events to be called.
 *
 *  On some platforms, a window move, resize or menu operation will cause event
 *  processing to block.  This is due to how event processing is designed on
 *  those platforms.  You can use the
 *  [window refresh callback](@ref window_refresh) to redraw the contents of
 *  your window when necessary during such operations.
 *
 *  Do not assume that callbacks you set will _only_ be called in response to
 *  event processing functions like this one.  While it is necessary to poll for
 *  events, window systems that require GLFW to register callbacks of its own
 *  can pass events to GLFW in response to many window system function calls.
 *  GLFW will pass those events on to the application callbacks before
 *  returning.
 *
 *  Event processing is not required for joystick input to work.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @reentrancy This function must not be called from a callback.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref events
 *  @sa @ref glfwWaitEvents
 *  @sa @ref glfwWaitEventsTimeout
 *
 *  @since Added in version 1.0.
 *
 *  @ingroup window
 */
GLFWAPI void glfwPollEvents(void);

/*! @brief Waits until events are queued and processes them.
 *
 *  This function puts the calling thread to sleep until at least one event is
 *  available in the event queue.  Once one or more events are available,
 *  it behaves exactly like @ref glfwPollEvents, i.e. the events in the queue
 *  are processed and the function then returns immediately.  Processing events
 *  will cause the window and input callbacks associated with those events to be
 *  called.
 *
 *  Since not all events are associated with callbacks, this function may return
 *  without a callback having been called even if you are monitoring all
 *  callbacks.
 *
 *  On some platforms, a window move, resize or menu operation will cause event
 *  processing to block.  This is due to how event processing is designed on
 *  those platforms.  You can use the
 *  [window refresh callback](@ref window_refresh) to redraw the contents of
 *  your window when necessary during such operations.
 *
 *  Do not assume that callbacks you set will _only_ be called in response to
 *  event processing functions like this one.  While it is necessary to poll for
 *  events, window systems that require GLFW to register callbacks of its own
 *  can pass events to GLFW in response to many window system function calls.
 *  GLFW will pass those events on to the application callbacks before
 *  returning.
 *
 *  Event processing is not required for joystick input to work.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @reentrancy This function must not be called from a callback.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref events
 *  @sa @ref glfwPollEvents
 *  @sa @ref glfwWaitEventsTimeout
 *
 *  @since Added in version 2.5.
 *
 *  @ingroup window
 */
GLFWAPI void glfwWaitEvents(void);

/*! @brief Waits with timeout until events are queued and processes them.
 *
 *  This function puts the calling thread to sleep until at least one event is
 *  available in the event queue, or until the specified timeout is reached.  If
 *  one or more events are available, it behaves exactly like @ref
 *  glfwPollEvents, i.e. the events in the queue are processed and the function
 *  then returns immediately.  Processing events will cause the window and input
 *  callbacks associated with those events to be called.
 *
 *  The timeout value must be a positive finite number.
 *
 *  Since not all events are associated with callbacks, this function may return
 *  without a callback having been called even if you are monitoring all
 *  callbacks.
 *
 *  On some platforms, a window move, resize or menu operation will cause event
 *  processing to block.  This is due to how event processing is designed on
 *  those platforms.  You can use the
 *  [window refresh callback](@ref window_refresh) to redraw the contents of
 *  your window when necessary during such operations.
 *
 *  Do not assume that callbacks you set will _only_ be called in response to
 *  event processing functions like this one.  While it is necessary to poll for
 *  events, window systems that require GLFW to register callbacks of its own
 *  can pass events to GLFW in response to many window system function calls.
 *  GLFW will pass those events on to the application callbacks before
 *  returning.
 *
 *  Event processing is not required for joystick input to work.
 *
 *  @param[in] timeout The maximum amount of time, in seconds, to wait.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_INVALID_VALUE and @ref GLFW_PLATFORM_ERROR.
 *
 *  @reentrancy This function must not be called from a callback.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref events
 *  @sa @ref glfwPollEvents
 *  @sa @ref glfwWaitEvents
 *
 *  @since Added in version 3.2.
 *
 *  @ingroup window
 */
GLFWAPI void glfwWaitEventsTimeout(double timeout);

/*! @brief Posts an empty event to the event queue.
 *
 *  This function posts an empty event from the current thread to the event
 *  queue, causing @ref glfwWaitEvents or @ref glfwWaitEventsTimeout to return.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function may be called from any thread.
 *
 *  @sa @ref events
 *  @sa @ref glfwWaitEvents
 *  @sa @ref glfwWaitEventsTimeout
 *
 *  @since Added in version 3.1.
 *
 *  @ingroup window
 */
GLFWAPI void glfwPostEmptyEvent(void);

/*! @brief Returns the value of an input option for the specified window.
 *
 *  This function returns the value of an input option for the specified window.
 *  The mode must be one of @ref GLFW_CURSOR, @ref GLFW_STICKY_KEYS,
 *  @ref GLFW_STICKY_MOUSE_BUTTONS, @ref GLFW_LOCK_KEY_MODS or
 *  @ref GLFW_RAW_MOUSE_MOTION.
 *
 *  @param[in] window The window to query.
 *  @param[in] mode One of `GLFW_CURSOR`, `GLFW_STICKY_KEYS`,
 *  `GLFW_STICKY_MOUSE_BUTTONS`, `GLFW_LOCK_KEY_MODS` or
 *  `GLFW_RAW_MOUSE_MOTION`.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_INVALID_ENUM.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref glfwSetInputMode
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup input
 */
GLFWAPI int glfwGetInputMode(GLFWwindow* window, int mode);

/*! @brief Sets an input option for the specified window.
 *
 *  This function sets an input mode option for the specified window.  The mode
 *  must be one of @ref GLFW_CURSOR, @ref GLFW_STICKY_KEYS,
 *  @ref GLFW_STICKY_MOUSE_BUTTONS, @ref GLFW_LOCK_KEY_MODS or
 *  @ref GLFW_RAW_MOUSE_MOTION.
 *
 *  If the mode is `GLFW_CURSOR`, the value must be one of the following cursor
 *  modes:
 *  - `GLFW_CURSOR_NORMAL` makes the cursor visible and behaving normally.
 *  - `GLFW_CURSOR_HIDDEN` makes the cursor invisible when it is over the
 *    content area of the window but does not restrict the cursor from leaving.
 *  - `GLFW_CURSOR_DISABLED` hides and grabs the cursor, providing virtual
 *    and unlimited cursor movement.  This is useful for implementing for
 *    example 3D camera controls.
 *
 *  If the mode is `GLFW_STICKY_KEYS`, the value must be either `GLFW_TRUE` to
 *  enable sticky keys, or `GLFW_FALSE` to disable it.  If sticky keys are
 *  enabled, a key press will ensure that @ref glfwGetKey returns `GLFW_PRESS`
 *  the next time it is called even if the key had been released before the
 *  call.  This is useful when you are only interested in whether keys have been
 *  pressed but not when or in which order.
 *
 *  If the mode is `GLFW_STICKY_MOUSE_BUTTONS`, the value must be either
 *  `GLFW_TRUE` to enable sticky mouse buttons, or `GLFW_FALSE` to disable it.
 *  If sticky mouse buttons are enabled, a mouse button press will ensure that
 *  @ref glfwGetMouseButton returns `GLFW_PRESS` the next time it is called even
 *  if the mouse button had been released before the call.  This is useful when
 *  you are only interested in whether mouse buttons have been pressed but not
 *  when or in which order.
 *
 *  If the mode is `GLFW_LOCK_KEY_MODS`, the value must be either `GLFW_TRUE` to
 *  enable lock key modifier bits, or `GLFW_FALSE` to disable them.  If enabled,
 *  callbacks that receive modifier bits will also have the @ref
 *  GLFW_MOD_CAPS_LOCK bit set when the event was generated with Caps Lock on,
 *  and the @ref GLFW_MOD_NUM_LOCK bit when Num Lock was on.
 *
 *  If the mode is `GLFW_RAW_MOUSE_MOTION`, the value must be either `GLFW_TRUE`
 *  to enable raw (unscaled and unaccelerated) mouse motion when the cursor is
 *  disabled, or `GLFW_FALSE` to disable it.  If raw motion is not supported,
 *  attempting to set this will emit @ref GLFW_PLATFORM_ERROR.  Call @ref
 *  glfwRawMouseMotionSupported to check for support.
 *
 *  @param[in] window The window whose input mode to set.
 *  @param[in] mode One of `GLFW_CURSOR`, `GLFW_STICKY_KEYS`,
 *  `GLFW_STICKY_MOUSE_BUTTONS`, `GLFW_LOCK_KEY_MODS` or
 *  `GLFW_RAW_MOUSE_MOTION`.
 *  @param[in] value The new value of the specified input mode.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_INVALID_ENUM and @ref GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref glfwGetInputMode
 *
 *  @since Added in version 3.0.  Replaces `glfwEnable` and `glfwDisable`.
 *
 *  @ingroup input
 */
GLFWAPI void glfwSetInputMode(GLFWwindow* window, int mode, int value);

/*! @brief Returns whether raw mouse motion is supported.
 *
 *  This function returns whether raw mouse motion is supported on the current
 *  system.  This status does not change after GLFW has been initialized so you
 *  only need to check this once.  If you attempt to enable raw motion on
 *  a system that does not support it, @ref GLFW_PLATFORM_ERROR will be emitted.
 *
 *  Raw mouse motion is closer to the actual motion of the mouse across
 *  a surface.  It is not affected by the scaling and acceleration applied to
 *  the motion of the desktop cursor.  That processing is suitable for a cursor
 *  while raw motion is better for controlling for example a 3D camera.  Because
 *  of this, raw mouse motion is only provided when the cursor is disabled.
 *
 *  @return `GLFW_TRUE` if raw mouse motion is supported on the current machine,
 *  or `GLFW_FALSE` otherwise.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref raw_mouse_motion
 *  @sa @ref glfwSetInputMode
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup input
 */
GLFWAPI int glfwRawMouseMotionSupported(void);

/*! @brief Returns the layout-specific name of the specified printable key.
 *
 *  This function returns the name of the specified printable key, encoded as
 *  UTF-8.  This is typically the character that key would produce without any
 *  modifier keys, intended for displaying key bindings to the user.  For dead
 *  keys, it is typically the diacritic it would add to a character.
 *
 *  __Do not use this function__ for [text input](@ref input_char).  You will
 *  break text input for many languages even if it happens to work for yours.
 *
 *  If the key is `GLFW_KEY_UNKNOWN`, the scancode is used to identify the key,
 *  otherwise the scancode is ignored.  If you specify a non-printable key, or
 *  `GLFW_KEY_UNKNOWN` and a scancode that maps to a non-printable key, this
 *  function returns `NULL` but does not emit an error.
 *
 *  This behavior allows you to always pass in the arguments in the
 *  [key callback](@ref input_key) without modification.
 *
 *  The printable keys are:
 *  - `GLFW_KEY_APOSTROPHE`
 *  - `GLFW_KEY_COMMA`
 *  - `GLFW_KEY_MINUS`
 *  - `GLFW_KEY_PERIOD`
 *  - `GLFW_KEY_SLASH`
 *  - `GLFW_KEY_SEMICOLON`
 *  - `GLFW_KEY_EQUAL`
 *  - `GLFW_KEY_LEFT_BRACKET`
 *  - `GLFW_KEY_RIGHT_BRACKET`
 *  - `GLFW_KEY_BACKSLASH`
 *  - `GLFW_KEY_WORLD_1`
 *  - `GLFW_KEY_WORLD_2`
 *  - `GLFW_KEY_0` to `GLFW_KEY_9`
 *  - `GLFW_KEY_A` to `GLFW_KEY_Z`
 *  - `GLFW_KEY_KP_0` to `GLFW_KEY_KP_9`
 *  - `GLFW_KEY_KP_DECIMAL`
 *  - `GLFW_KEY_KP_DIVIDE`
 *  - `GLFW_KEY_KP_MULTIPLY`
 *  - `GLFW_KEY_KP_SUBTRACT`
 *  - `GLFW_KEY_KP_ADD`
 *  - `GLFW_KEY_KP_EQUAL`
 *
 *  Names for printable keys depend on keyboard layout, while names for
 *  non-printable keys are the same across layouts but depend on the application
 *  language and should be localized along with other user interface text.
 *
 *  @param[in] key The key to query, or `GLFW_KEY_UNKNOWN`.
 *  @param[in] scancode The scancode of the key to query.
 *  @return The UTF-8 encoded, layout-specific name of the key, or `NULL`.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @remark The contents of the returned string may change when a keyboard
 *  layout change event is received.
 *
 *  @pointer_lifetime The returned string is allocated and freed by GLFW.  You
 *  should not free it yourself.  It is valid until the library is terminated.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref input_key_name
 *
 *  @since Added in version 3.2.
 *
 *  @ingroup input
 */
GLFWAPI const char* glfwGetKeyName(int key, int scancode);

/*! @brief Returns the platform-specific scancode of the specified key.
 *
 *  This function returns the platform-specific scancode of the specified key.
 *
 *  If the key is `GLFW_KEY_UNKNOWN` or does not exist on the keyboard this
 *  method will return `-1`.
 *
 *  @param[in] key Any [named key](@ref keys).
 *  @return The platform-specific scancode for the key, or `-1` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_INVALID_ENUM and @ref GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function may be called from any thread.
 *
 *  @sa @ref input_key
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup input
 */
GLFWAPI int glfwGetKeyScancode(int key);

/*! @brief Returns the last reported state of a keyboard key for the specified
 *  window.
 *
 *  This function returns the last state reported for the specified key to the
 *  specified window.  The returned state is one of `GLFW_PRESS` or
 *  `GLFW_RELEASE`.  The action `GLFW_REPEAT` is only reported to the key callback.
 *
 *  If the @ref GLFW_STICKY_KEYS input mode is enabled, this function returns
 *  `GLFW_PRESS` the first time you call it for a key that was pressed, even if
 *  that key has already been released.
 *
 *  The key functions deal with physical keys, with [key tokens](@ref keys)
 *  named after their use on the standard US keyboard layout.  If you want to
 *  input text, use the Unicode character callback instead.
 *
 *  The [modifier key bit masks](@ref mods) are not key tokens and cannot be
 *  used with this function.
 *
 *  __Do not use this function__ to implement [text input](@ref input_char).
 *
 *  @param[in] window The desired window.
 *  @param[in] key The desired [keyboard key](@ref keys).  `GLFW_KEY_UNKNOWN` is
 *  not a valid key for this function.
 *  @return One of `GLFW_PRESS` or `GLFW_RELEASE`.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_INVALID_ENUM.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref input_key
 *
 *  @since Added in version 1.0.
 *  @glfw3 Added window handle parameter.
 *
 *  @ingroup input
 */
GLFWAPI int glfwGetKey(GLFWwindow* window, int key);

/*! @brief Returns the last reported state of a mouse button for the specified
 *  window.
 *
 *  This function returns the last state reported for the specified mouse button
 *  to the specified window.  The returned state is one of `GLFW_PRESS` or
 *  `GLFW_RELEASE`.
 *
 *  If the @ref GLFW_STICKY_MOUSE_BUTTONS input mode is enabled, this function
 *  returns `GLFW_PRESS` the first time you call it for a mouse button that was
 *  pressed, even if that mouse button has already been released.
 *
 *  @param[in] window The desired window.
 *  @param[in] button The desired [mouse button](@ref buttons).
 *  @return One of `GLFW_PRESS` or `GLFW_RELEASE`.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_INVALID_ENUM.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref input_mouse_button
 *
 *  @since Added in version 1.0.
 *  @glfw3 Added window handle parameter.
 *
 *  @ingroup input
 */
GLFWAPI int glfwGetMouseButton(GLFWwindow* window, int button);

/*! @brief Retrieves the position of the cursor relative to the content area of
 *  the window.
 *
 *  This function returns the position of the cursor, in screen coordinates,
 *  relative to the upper-left corner of the content area of the specified
 *  window.
 *
 *  If the cursor is disabled (with `GLFW_CURSOR_DISABLED`) then the cursor
 *  position is unbounded and limited only by the minimum and maximum values of
 *  a `double`.
 *
 *  The coordinate can be converted to their integer equivalents with the
 *  `floor` function.  Casting directly to an integer type works for positive
 *  coordinates, but fails for negative ones.
 *
 *  Any or all of the position arguments may be `NULL`.  If an error occurs, all
 *  non-`NULL` position arguments will be set to zero.
 *
 *  @param[in] window The desired window.
 *  @param[out] xpos Where to store the cursor x-coordinate, relative to the
 *  left edge of the content area, or `NULL`.
 *  @param[out] ypos Where to store the cursor y-coordinate, relative to the to
 *  top edge of the content area, or `NULL`.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref cursor_pos
 *  @sa @ref glfwSetCursorPos
 *
 *  @since Added in version 3.0.  Replaces `glfwGetMousePos`.
 *
 *  @ingroup input
 */
GLFWAPI void glfwGetCursorPos(GLFWwindow* window, double* xpos, double* ypos);

/*! @brief Sets the position of the cursor, relative to the content area of the
 *  window.
 *
 *  This function sets the position, in screen coordinates, of the cursor
 *  relative to the upper-left corner of the content area of the specified
 *  window.  The window must have input focus.  If the window does not have
 *  input focus when this function is called, it fails silently.
 *
 *  __Do not use this function__ to implement things like camera controls.  GLFW
 *  already provides the `GLFW_CURSOR_DISABLED` cursor mode that hides the
 *  cursor, transparently re-centers it and provides unconstrained cursor
 *  motion.  See @ref glfwSetInputMode for more information.
 *
 *  If the cursor mode is `GLFW_CURSOR_DISABLED` then the cursor position is
 *  unconstrained and limited only by the minimum and maximum values of
 *  a `double`.
 *
 *  @param[in] window The desired window.
 *  @param[in] xpos The desired x-coordinate, relative to the left edge of the
 *  content area.
 *  @param[in] ypos The desired y-coordinate, relative to the top edge of the
 *  content area.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @remark @wayland This function will only work when the cursor mode is
 *  `GLFW_CURSOR_DISABLED`, otherwise it will do nothing.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref cursor_pos
 *  @sa @ref glfwGetCursorPos
 *
 *  @since Added in version 3.0.  Replaces `glfwSetMousePos`.
 *
 *  @ingroup input
 */
GLFWAPI void glfwSetCursorPos(GLFWwindow* window, double xpos, double ypos);

/*! @brief Creates a custom cursor.
 *
 *  Creates a new custom cursor image that can be set for a window with @ref
 *  glfwSetCursor.  The cursor can be destroyed with @ref glfwDestroyCursor.
 *  Any remaining cursors are destroyed by @ref glfwTerminate.
 *
 *  The pixels are 32-bit, little-endian, non-premultiplied RGBA, i.e. eight
 *  bits per channel with the red channel first.  They are arranged canonically
 *  as packed sequential rows, starting from the top-left corner.
 *
 *  The cursor hotspot is specified in pixels, relative to the upper-left corner
 *  of the cursor image.  Like all other coordinate systems in GLFW, the X-axis
 *  points to the right and the Y-axis points down.
 *
 *  @param[in] image The desired cursor image.
 *  @param[in] xhot The desired x-coordinate, in pixels, of the cursor hotspot.
 *  @param[in] yhot The desired y-coordinate, in pixels, of the cursor hotspot.
 *  @return The handle of the created cursor, or `NULL` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_INVALID_VALUE and @ref GLFW_PLATFORM_ERROR.
 *
 *  @pointer_lifetime The specified image data is copied before this function
 *  returns.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref cursor_object
 *  @sa @ref glfwDestroyCursor
 *  @sa @ref glfwCreateStandardCursor
 *
 *  @since Added in version 3.1.
 *
 *  @ingroup input
 */
GLFWAPI GLFWcursor* glfwCreateCursor(const GLFWimage* image, int xhot, int yhot);

/*! @brief Creates a cursor with a standard shape.
 *
 *  Returns a cursor with a [standard shape](@ref shapes), that can be set for
 *  a window with @ref glfwSetCursor.
 *
 *  @param[in] shape One of the [standard shapes](@ref shapes).
 *  @return A new cursor ready to use or `NULL` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_INVALID_ENUM and @ref GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref cursor_object
 *  @sa @ref glfwCreateCursor
 *
 *  @since Added in version 3.1.
 *
 *  @ingroup input
 */
GLFWAPI GLFWcursor* glfwCreateStandardCursor(int shape);

/*! @brief Destroys a cursor.
 *
 *  This function destroys a cursor previously created with @ref
 *  glfwCreateCursor.  Any remaining cursors will be destroyed by @ref
 *  glfwTerminate.
 *
 *  If the specified cursor is current for any window, that window will be
 *  reverted to the default cursor.  This does not affect the cursor mode.
 *
 *  @param[in] cursor The cursor object to destroy.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @reentrancy This function must not be called from a callback.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref cursor_object
 *  @sa @ref glfwCreateCursor
 *
 *  @since Added in version 3.1.
 *
 *  @ingroup input
 */
GLFWAPI void glfwDestroyCursor(GLFWcursor* cursor);

/*! @brief Sets the cursor for the window.
 *
 *  This function sets the cursor image to be used when the cursor is over the
 *  content area of the specified window.  The set cursor will only be visible
 *  when the [cursor mode](@ref cursor_mode) of the window is
 *  `GLFW_CURSOR_NORMAL`.
 *
 *  On some platforms, the set cursor may not be visible unless the window also
 *  has input focus.
 *
 *  @param[in] window The window to set the cursor for.
 *  @param[in] cursor The cursor to set, or `NULL` to switch back to the default
 *  arrow cursor.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref cursor_object
 *
 *  @since Added in version 3.1.
 *
 *  @ingroup input
 */
GLFWAPI void glfwSetCursor(GLFWwindow* window, GLFWcursor* cursor);

/*! @brief Sets the key callback.
 *
 *  This function sets the key callback of the specified window, which is called
 *  when a key is pressed, repeated or released.
 *
 *  The key functions deal with physical keys, with layout independent
 *  [key tokens](@ref keys) named after their values in the standard US keyboard
 *  layout.  If you want to input text, use the
 *  [character callback](@ref glfwSetCharCallback) instead.
 *
 *  When a window loses input focus, it will generate synthetic key release
 *  events for all pressed keys.  You can tell these events from user-generated
 *  events by the fact that the synthetic ones are generated after the focus
 *  loss event has been processed, i.e. after the
 *  [window focus callback](@ref glfwSetWindowFocusCallback) has been called.
 *
 *  The scancode of a key is specific to that platform or sometimes even to that
 *  machine.  Scancodes are intended to allow users to bind keys that don't have
 *  a GLFW key token.  Such keys have `key` set to `GLFW_KEY_UNKNOWN`, their
 *  state is not saved and so it cannot be queried with @ref glfwGetKey.
 *
 *  Sometimes GLFW needs to generate synthetic key events, in which case the
 *  scancode may be zero.
 *
 *  @param[in] window The window whose callback to set.
 *  @param[in] callback The new key callback, or `NULL` to remove the currently
 *  set callback.
 *  @return The previously set callback, or `NULL` if no callback was set or the
 *  library had not been [initialized](@ref intro_init).
 *
 *  @callback_signature
 *  @code
 *  void function_name(GLFWwindow* window, int key, int scancode, int action, int mods)
 *  @endcode
 *  For more information about the callback parameters, see the
 *  [function pointer type](@ref GLFWkeyfun).
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref input_key
 *
 *  @since Added in version 1.0.
 *  @glfw3 Added window handle parameter and return value.
 *
 *  @ingroup input
 */
GLFWAPI GLFWkeyfun glfwSetKeyCallback(GLFWwindow* window, GLFWkeyfun callback);

/*! @brief Sets the Unicode character callback.
 *
 *  This function sets the character callback of the specified window, which is
 *  called when a Unicode character is input.
 *
 *  The character callback is intended for Unicode text input.  As it deals with
 *  characters, it is keyboard layout dependent, whereas the
 *  [key callback](@ref glfwSetKeyCallback) is not.  Characters do not map 1:1
 *  to physical keys, as a key may produce zero, one or more characters.  If you
 *  want to know whether a specific physical key was pressed or released, see
 *  the key callback instead.
 *
 *  The character callback behaves as system text input normally does and will
 *  not be called if modifier keys are held down that would prevent normal text
 *  input on that platform, for example a Super (Command) key on macOS or Alt key
 *  on Windows.
 *
 *  @param[in] window The window whose callback to set.
 *  @param[in] callback The new callback, or `NULL` to remove the currently set
 *  callback.
 *  @return The previously set callback, or `NULL` if no callback was set or the
 *  library had not been [initialized](@ref intro_init).
 *
 *  @callback_signature
 *  @code
 *  void function_name(GLFWwindow* window, unsigned int codepoint)
 *  @endcode
 *  For more information about the callback parameters, see the
 *  [function pointer type](@ref GLFWcharfun).
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref input_char
 *
 *  @since Added in version 2.4.
 *  @glfw3 Added window handle parameter and return value.
 *
 *  @ingroup input
 */
GLFWAPI GLFWcharfun glfwSetCharCallback(GLFWwindow* window, GLFWcharfun callback);

/*! @brief Sets the Unicode character with modifiers callback.
 *
 *  This function sets the character with modifiers callback of the specified
 *  window, which is called when a Unicode character is input regardless of what
 *  modifier keys are used.
 *
 *  The character with modifiers callback is intended for implementing custom
 *  Unicode character input.  For regular Unicode text input, see the
 *  [character callback](@ref glfwSetCharCallback).  Like the character
 *  callback, the character with modifiers callback deals with characters and is
 *  keyboard layout dependent.  Characters do not map 1:1 to physical keys, as
 *  a key may produce zero, one or more characters.  If you want to know whether
 *  a specific physical key was pressed or released, see the
 *  [key callback](@ref glfwSetKeyCallback) instead.
 *
 *  @param[in] window The window whose callback to set.
 *  @param[in] callback The new callback, or `NULL` to remove the currently set
 *  callback.
 *  @return The previously set callback, or `NULL` if no callback was set or an
 *  [error](@ref error_handling) occurred.
 *
 *  @callback_signature
 *  @code
 *  void function_name(GLFWwindow* window, unsigned int codepoint, int mods)
 *  @endcode
 *  For more information about the callback parameters, see the
 *  [function pointer type](@ref GLFWcharmodsfun).
 *
 *  @deprecated Scheduled for removal in version 4.0.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref input_char
 *
 *  @since Added in version 3.1.
 *
 *  @ingroup input
 */
GLFWAPI GLFWcharmodsfun glfwSetCharModsCallback(GLFWwindow* window, GLFWcharmodsfun callback);

/*! @brief Sets the mouse button callback.
 *
 *  This function sets the mouse button callback of the specified window, which
 *  is called when a mouse button is pressed or released.
 *
 *  When a window loses input focus, it will generate synthetic mouse button
 *  release events for all pressed mouse buttons.  You can tell these events
 *  from user-generated events by the fact that the synthetic ones are generated
 *  after the focus loss event has been processed, i.e. after the
 *  [window focus callback](@ref glfwSetWindowFocusCallback) has been called.
 *
 *  @param[in] window The window whose callback to set.
 *  @param[in] callback The new callback, or `NULL` to remove the currently set
 *  callback.
 *  @return The previously set callback, or `NULL` if no callback was set or the
 *  library had not been [initialized](@ref intro_init).
 *
 *  @callback_signature
 *  @code
 *  void function_name(GLFWwindow* window, int button, int action, int mods)
 *  @endcode
 *  For more information about the callback parameters, see the
 *  [function pointer type](@ref GLFWmousebuttonfun).
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref input_mouse_button
 *
 *  @since Added in version 1.0.
 *  @glfw3 Added window handle parameter and return value.
 *
 *  @ingroup input
 */
GLFWAPI GLFWmousebuttonfun glfwSetMouseButtonCallback(GLFWwindow* window, GLFWmousebuttonfun callback);

/*! @brief Sets the cursor position callback.
 *
 *  This function sets the cursor position callback of the specified window,
 *  which is called when the cursor is moved.  The callback is provided with the
 *  position, in screen coordinates, relative to the upper-left corner of the
 *  content area of the window.
 *
 *  @param[in] window The window whose callback to set.
 *  @param[in] callback The new callback, or `NULL` to remove the currently set
 *  callback.
 *  @return The previously set callback, or `NULL` if no callback was set or the
 *  library had not been [initialized](@ref intro_init).
 *
 *  @callback_signature
 *  @code
 *  void function_name(GLFWwindow* window, double xpos, double ypos);
 *  @endcode
 *  For more information about the callback parameters, see the
 *  [function pointer type](@ref GLFWcursorposfun).
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref cursor_pos
 *
 *  @since Added in version 3.0.  Replaces `glfwSetMousePosCallback`.
 *
 *  @ingroup input
 */
GLFWAPI GLFWcursorposfun glfwSetCursorPosCallback(GLFWwindow* window, GLFWcursorposfun callback);

/*! @brief Sets the cursor enter/leave callback.
 *
 *  This function sets the cursor boundary crossing callback of the specified
 *  window, which is called when the cursor enters or leaves the content area of
 *  the window.
 *
 *  @param[in] window The window whose callback to set.
 *  @param[in] callback The new callback, or `NULL` to remove the currently set
 *  callback.
 *  @return The previously set callback, or `NULL` if no callback was set or the
 *  library had not been [initialized](@ref intro_init).
 *
 *  @callback_signature
 *  @code
 *  void function_name(GLFWwindow* window, int entered)
 *  @endcode
 *  For more information about the callback parameters, see the
 *  [function pointer type](@ref GLFWcursorenterfun).
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref cursor_enter
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup input
 */
GLFWAPI GLFWcursorenterfun glfwSetCursorEnterCallback(GLFWwindow* window, GLFWcursorenterfun callback);

/*! @brief Sets the scroll callback.
 *
 *  This function sets the scroll callback of the specified window, which is
 *  called when a scrolling device is used, such as a mouse wheel or scrolling
 *  area of a touchpad.
 *
 *  The scroll callback receives all scrolling input, like that from a mouse
 *  wheel or a touchpad scrolling area.
 *
 *  @param[in] window The window whose callback to set.
 *  @param[in] callback The new scroll callback, or `NULL` to remove the
 *  currently set callback.
 *  @return The previously set callback, or `NULL` if no callback was set or the
 *  library had not been [initialized](@ref intro_init).
 *
 *  @callback_signature
 *  @code
 *  void function_name(GLFWwindow* window, double xoffset, double yoffset)
 *  @endcode
 *  For more information about the callback parameters, see the
 *  [function pointer type](@ref GLFWscrollfun).
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref scrolling
 *
 *  @since Added in version 3.0.  Replaces `glfwSetMouseWheelCallback`.
 *
 *  @ingroup input
 */
GLFWAPI GLFWscrollfun glfwSetScrollCallback(GLFWwindow* window, GLFWscrollfun callback);

/*! @brief Sets the path drop callback.
 *
 *  This function sets the path drop callback of the specified window, which is
 *  called when one or more dragged paths are dropped on the window.
 *
 *  Because the path array and its strings may have been generated specifically
 *  for that event, they are not guaranteed to be valid after the callback has
 *  returned.  If you wish to use them after the callback returns, you need to
 *  make a deep copy.
 *
 *  @param[in] window The window whose callback to set.
 *  @param[in] callback The new file drop callback, or `NULL` to remove the
 *  currently set callback.
 *  @return The previously set callback, or `NULL` if no callback was set or the
 *  library had not been [initialized](@ref intro_init).
 *
 *  @callback_signature
 *  @code
 *  void function_name(GLFWwindow* window, int path_count, const char* paths[])
 *  @endcode
 *  For more information about the callback parameters, see the
 *  [function pointer type](@ref GLFWdropfun).
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @remark @wayland File drop is currently unimplemented.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref path_drop
 *
 *  @since Added in version 3.1.
 *
 *  @ingroup input
 */
GLFWAPI GLFWdropfun glfwSetDropCallback(GLFWwindow* window, GLFWdropfun callback);

/*! @brief Returns whether the specified joystick is present.
 *
 *  This function returns whether the specified joystick is present.
 *
 *  There is no need to call this function before other functions that accept
 *  a joystick ID, as they all check for presence before performing any other
 *  work.
 *
 *  @param[in] jid The [joystick](@ref joysticks) to query.
 *  @return `GLFW_TRUE` if the joystick is present, or `GLFW_FALSE` otherwise.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_INVALID_ENUM and @ref GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref joystick
 *
 *  @since Added in version 3.0.  Replaces `glfwGetJoystickParam`.
 *
 *  @ingroup input
 */
GLFWAPI int glfwJoystickPresent(int jid);

/*! @brief Returns the values of all axes of the specified joystick.
 *
 *  This function returns the values of all axes of the specified joystick.
 *  Each element in the array is a value between -1.0 and 1.0.
 *
 *  If the specified joystick is not present this function will return `NULL`
 *  but will not generate an error.  This can be used instead of first calling
 *  @ref glfwJoystickPresent.
 *
 *  @param[in] jid The [joystick](@ref joysticks) to query.
 *  @param[out] count Where to store the number of axis values in the returned
 *  array.  This is set to zero if the joystick is not present or an error
 *  occurred.
 *  @return An array of axis values, or `NULL` if the joystick is not present or
 *  an [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_INVALID_ENUM and @ref GLFW_PLATFORM_ERROR.
 *
 *  @pointer_lifetime The returned array is allocated and freed by GLFW.  You
 *  should not free it yourself.  It is valid until the specified joystick is
 *  disconnected or the library is terminated.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref joystick_axis
 *
 *  @since Added in version 3.0.  Replaces `glfwGetJoystickPos`.
 *
 *  @ingroup input
 */
GLFWAPI const float* glfwGetJoystickAxes(int jid, int* count);

/*! @brief Returns the state of all buttons of the specified joystick.
 *
 *  This function returns the state of all buttons of the specified joystick.
 *  Each element in the array is either `GLFW_PRESS` or `GLFW_RELEASE`.
 *
 *  For backward compatibility with earlier versions that did not have @ref
 *  glfwGetJoystickHats, the button array also includes all hats, each
 *  represented as four buttons.  The hats are in the same order as returned by
 *  __glfwGetJoystickHats__ and are in the order _up_, _right_, _down_ and
 *  _left_.  To disable these extra buttons, set the @ref
 *  GLFW_JOYSTICK_HAT_BUTTONS init hint before initialization.
 *
 *  If the specified joystick is not present this function will return `NULL`
 *  but will not generate an error.  This can be used instead of first calling
 *  @ref glfwJoystickPresent.
 *
 *  @param[in] jid The [joystick](@ref joysticks) to query.
 *  @param[out] count Where to store the number of button states in the returned
 *  array.  This is set to zero if the joystick is not present or an error
 *  occurred.
 *  @return An array of button states, or `NULL` if the joystick is not present
 *  or an [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_INVALID_ENUM and @ref GLFW_PLATFORM_ERROR.
 *
 *  @pointer_lifetime The returned array is allocated and freed by GLFW.  You
 *  should not free it yourself.  It is valid until the specified joystick is
 *  disconnected or the library is terminated.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref joystick_button
 *
 *  @since Added in version 2.2.
 *  @glfw3 Changed to return a dynamic array.
 *
 *  @ingroup input
 */
GLFWAPI const unsigned char* glfwGetJoystickButtons(int jid, int* count);

/*! @brief Returns the state of all hats of the specified joystick.
 *
 *  This function returns the state of all hats of the specified joystick.
 *  Each element in the array is one of the following values:
 *
 *  Name                  | Value
 *  ----                  | -----
 *  `GLFW_HAT_CENTERED`   | 0
 *  `GLFW_HAT_UP`         | 1
 *  `GLFW_HAT_RIGHT`      | 2
 *  `GLFW_HAT_DOWN`       | 4
 *  `GLFW_HAT_LEFT`       | 8
 *  `GLFW_HAT_RIGHT_UP`   | `GLFW_HAT_RIGHT` \| `GLFW_HAT_UP`
 *  `GLFW_HAT_RIGHT_DOWN` | `GLFW_HAT_RIGHT` \| `GLFW_HAT_DOWN`
 *  `GLFW_HAT_LEFT_UP`    | `GLFW_HAT_LEFT` \| `GLFW_HAT_UP`
 *  `GLFW_HAT_LEFT_DOWN`  | `GLFW_HAT_LEFT` \| `GLFW_HAT_DOWN`
 *
 *  The diagonal directions are bitwise combinations of the primary (up, right,
 *  down and left) directions and you can test for these individually by ANDing
 *  it with the corresponding direction.
 *
 *  @code
 *  if (hats[2] & GLFW_HAT_RIGHT)
 *  {
 *      // State of hat 2 could be right-up, right or right-down
 *  }
 *  @endcode
 *
 *  If the specified joystick is not present this function will return `NULL`
 *  but will not generate an error.  This can be used instead of first calling
 *  @ref glfwJoystickPresent.
 *
 *  @param[in] jid The [joystick](@ref joysticks) to query.
 *  @param[out] count Where to store the number of hat states in the returned
 *  array.  This is set to zero if the joystick is not present or an error
 *  occurred.
 *  @return An array of hat states, or `NULL` if the joystick is not present
 *  or an [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_INVALID_ENUM and @ref GLFW_PLATFORM_ERROR.
 *
 *  @pointer_lifetime The returned array is allocated and freed by GLFW.  You
 *  should not free it yourself.  It is valid until the specified joystick is
 *  disconnected, this function is called again for that joystick or the library
 *  is terminated.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref joystick_hat
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup input
 */
GLFWAPI const unsigned char* glfwGetJoystickHats(int jid, int* count);

/*! @brief Returns the name of the specified joystick.
 *
 *  This function returns the name, encoded as UTF-8, of the specified joystick.
 *  The returned string is allocated and freed by GLFW.  You should not free it
 *  yourself.
 *
 *  If the specified joystick is not present this function will return `NULL`
 *  but will not generate an error.  This can be used instead of first calling
 *  @ref glfwJoystickPresent.
 *
 *  @param[in] jid The [joystick](@ref joysticks) to query.
 *  @return The UTF-8 encoded name of the joystick, or `NULL` if the joystick
 *  is not present or an [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_INVALID_ENUM and @ref GLFW_PLATFORM_ERROR.
 *
 *  @pointer_lifetime The returned string is allocated and freed by GLFW.  You
 *  should not free it yourself.  It is valid until the specified joystick is
 *  disconnected or the library is terminated.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref joystick_name
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup input
 */
GLFWAPI const char* glfwGetJoystickName(int jid);

/*! @brief Returns the SDL compatible GUID of the specified joystick.
 *
 *  This function returns the SDL compatible GUID, as a UTF-8 encoded
 *  hexadecimal string, of the specified joystick.  The returned string is
 *  allocated and freed by GLFW.  You should not free it yourself.
 *
 *  The GUID is what connects a joystick to a gamepad mapping.  A connected
 *  joystick will always have a GUID even if there is no gamepad mapping
 *  assigned to it.
 *
 *  If the specified joystick is not present this function will return `NULL`
 *  but will not generate an error.  This can be used instead of first calling
 *  @ref glfwJoystickPresent.
 *
 *  The GUID uses the format introduced in SDL 2.0.5.  This GUID tries to
 *  uniquely identify the make and model of a joystick but does not identify
 *  a specific unit, e.g. all wired Xbox 360 controllers will have the same
 *  GUID on that platform.  The GUID for a unit may vary between platforms
 *  depending on what hardware information the platform specific APIs provide.
 *
 *  @param[in] jid The [joystick](@ref joysticks) to query.
 *  @return The UTF-8 encoded GUID of the joystick, or `NULL` if the joystick
 *  is not present or an [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_INVALID_ENUM and @ref GLFW_PLATFORM_ERROR.
 *
 *  @pointer_lifetime The returned string is allocated and freed by GLFW.  You
 *  should not free it yourself.  It is valid until the specified joystick is
 *  disconnected or the library is terminated.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref gamepad
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup input
 */
GLFWAPI const char* glfwGetJoystickGUID(int jid);

/*! @brief Sets the user pointer of the specified joystick.
 *
 *  This function sets the user-defined pointer of the specified joystick.  The
 *  current value is retained until the joystick is disconnected.  The initial
 *  value is `NULL`.
 *
 *  This function may be called from the joystick callback, even for a joystick
 *  that is being disconnected.
 *
 *  @param[in] jid The joystick whose pointer to set.
 *  @param[in] pointer The new value.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function may be called from any thread.  Access is not
 *  synchronized.
 *
 *  @sa @ref joystick_userptr
 *  @sa @ref glfwGetJoystickUserPointer
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup input
 */
GLFWAPI void glfwSetJoystickUserPointer(int jid, void* pointer);

/*! @brief Returns the user pointer of the specified joystick.
 *
 *  This function returns the current value of the user-defined pointer of the
 *  specified joystick.  The initial value is `NULL`.
 *
 *  This function may be called from the joystick callback, even for a joystick
 *  that is being disconnected.
 *
 *  @param[in] jid The joystick whose pointer to return.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function may be called from any thread.  Access is not
 *  synchronized.
 *
 *  @sa @ref joystick_userptr
 *  @sa @ref glfwSetJoystickUserPointer
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup input
 */
GLFWAPI void* glfwGetJoystickUserPointer(int jid);

/*! @brief Returns whether the specified joystick has a gamepad mapping.
 *
 *  This function returns whether the specified joystick is both present and has
 *  a gamepad mapping.
 *
 *  If the specified joystick is present but does not have a gamepad mapping
 *  this function will return `GLFW_FALSE` but will not generate an error.  Call
 *  @ref glfwJoystickPresent to check if a joystick is present regardless of
 *  whether it has a mapping.
 *
 *  @param[in] jid The [joystick](@ref joysticks) to query.
 *  @return `GLFW_TRUE` if a joystick is both present and has a gamepad mapping,
 *  or `GLFW_FALSE` otherwise.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_INVALID_ENUM.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref gamepad
 *  @sa @ref glfwGetGamepadState
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup input
 */
GLFWAPI int glfwJoystickIsGamepad(int jid);

/*! @brief Sets the joystick configuration callback.
 *
 *  This function sets the joystick configuration callback, or removes the
 *  currently set callback.  This is called when a joystick is connected to or
 *  disconnected from the system.
 *
 *  For joystick connection and disconnection events to be delivered on all
 *  platforms, you need to call one of the [event processing](@ref events)
 *  functions.  Joystick disconnection may also be detected and the callback
 *  called by joystick functions.  The function will then return whatever it
 *  returns if the joystick is not present.
 *
 *  @param[in] callback The new callback, or `NULL` to remove the currently set
 *  callback.
 *  @return The previously set callback, or `NULL` if no callback was set or the
 *  library had not been [initialized](@ref intro_init).
 *
 *  @callback_signature
 *  @code
 *  void function_name(int jid, int event)
 *  @endcode
 *  For more information about the callback parameters, see the
 *  [function pointer type](@ref GLFWjoystickfun).
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref joystick_event
 *
 *  @since Added in version 3.2.
 *
 *  @ingroup input
 */
GLFWAPI GLFWjoystickfun glfwSetJoystickCallback(GLFWjoystickfun callback);

/*! @brief Adds the specified SDL_GameControllerDB gamepad mappings.
 *
 *  This function parses the specified ASCII encoded string and updates the
 *  internal list with any gamepad mappings it finds.  This string may
 *  contain either a single gamepad mapping or many mappings separated by
 *  newlines.  The parser supports the full format of the `gamecontrollerdb.txt`
 *  source file including empty lines and comments.
 *
 *  See @ref gamepad_mapping for a description of the format.
 *
 *  If there is already a gamepad mapping for a given GUID in the internal list,
 *  it will be replaced by the one passed to this function.  If the library is
 *  terminated and re-initialized the internal list will revert to the built-in
 *  default.
 *
 *  @param[in] string The string containing the gamepad mappings.
 *  @return `GLFW_TRUE` if successful, or `GLFW_FALSE` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_INVALID_VALUE.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref gamepad
 *  @sa @ref glfwJoystickIsGamepad
 *  @sa @ref glfwGetGamepadName
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup input
 */
GLFWAPI int glfwUpdateGamepadMappings(const char* string);

/*! @brief Returns the human-readable gamepad name for the specified joystick.
 *
 *  This function returns the human-readable name of the gamepad from the
 *  gamepad mapping assigned to the specified joystick.
 *
 *  If the specified joystick is not present or does not have a gamepad mapping
 *  this function will return `NULL` but will not generate an error.  Call
 *  @ref glfwJoystickPresent to check whether it is present regardless of
 *  whether it has a mapping.
 *
 *  @param[in] jid The [joystick](@ref joysticks) to query.
 *  @return The UTF-8 encoded name of the gamepad, or `NULL` if the
 *  joystick is not present, does not have a mapping or an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref GLFW_INVALID_ENUM.
 *
 *  @pointer_lifetime The returned string is allocated and freed by GLFW.  You
 *  should not free it yourself.  It is valid until the specified joystick is
 *  disconnected, the gamepad mappings are updated or the library is terminated.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref gamepad
 *  @sa @ref glfwJoystickIsGamepad
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup input
 */
GLFWAPI const char* glfwGetGamepadName(int jid);

/*! @brief Retrieves the state of the specified joystick remapped as a gamepad.
 *
 *  This function retrieves the state of the specified joystick remapped to
 *  an Xbox-like gamepad.
 *
 *  If the specified joystick is not present or does not have a gamepad mapping
 *  this function will return `GLFW_FALSE` but will not generate an error.  Call
 *  @ref glfwJoystickPresent to check whether it is present regardless of
 *  whether it has a mapping.
 *
 *  The Guide button may not be available for input as it is often hooked by the
 *  system or the Steam client.
 *
 *  Not all devices have all the buttons or axes provided by @ref
 *  GLFWgamepadstate.  Unavailable buttons and axes will always report
 *  `GLFW_RELEASE` and 0.0 respectively.
 *
 *  @param[in] jid The [joystick](@ref joysticks) to query.
 *  @param[out] state The gamepad input state of the joystick.
 *  @return `GLFW_TRUE` if successful, or `GLFW_FALSE` if no joystick is
 *  connected, it has no gamepad mapping or an [error](@ref error_handling)
 *  occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_INVALID_ENUM.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref gamepad
 *  @sa @ref glfwUpdateGamepadMappings
 *  @sa @ref glfwJoystickIsGamepad
 *
 *  @since Added in version 3.3.
 *
 *  @ingroup input
 */
GLFWAPI int glfwGetGamepadState(int jid, GLFWgamepadstate* state);

/*! @brief Sets the clipboard to the specified string.
 *
 *  This function sets the system clipboard to the specified, UTF-8 encoded
 *  string.
 *
 *  @param[in] window Deprecated.  Any valid window or `NULL`.
 *  @param[in] string A UTF-8 encoded string.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @pointer_lifetime The specified string is copied before this function
 *  returns.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref clipboard
 *  @sa @ref glfwGetClipboardString
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup input
 */
GLFWAPI void glfwSetClipboardString(GLFWwindow* window, const char* string);

/*! @brief Returns the contents of the clipboard as a string.
 *
 *  This function returns the contents of the system clipboard, if it contains
 *  or is convertible to a UTF-8 encoded string.  If the clipboard is empty or
 *  if its contents cannot be converted, `NULL` is returned and a @ref
 *  GLFW_FORMAT_UNAVAILABLE error is generated.
 *
 *  @param[in] window Deprecated.  Any valid window or `NULL`.
 *  @return The contents of the clipboard as a UTF-8 encoded string, or `NULL`
 *  if an [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_FORMAT_UNAVAILABLE and @ref GLFW_PLATFORM_ERROR.
 *
 *  @pointer_lifetime The returned string is allocated and freed by GLFW.  You
 *  should not free it yourself.  It is valid until the next call to @ref
 *  glfwGetClipboardString or @ref glfwSetClipboardString, or until the library
 *  is terminated.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref clipboard
 *  @sa @ref glfwSetClipboardString
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup input
 */
GLFWAPI const char* glfwGetClipboardString(GLFWwindow* window);

/*! @brief Returns the GLFW time.
 *
 *  This function returns the current GLFW time, in seconds.  Unless the time
 *  has been set using @ref glfwSetTime it measures time elapsed since GLFW was
 *  initialized.
 *
 *  This function and @ref glfwSetTime are helper functions on top of @ref
 *  glfwGetTimerFrequency and @ref glfwGetTimerValue.
 *
 *  The resolution of the timer is system dependent, but is usually on the order
 *  of a few micro- or nanoseconds.  It uses the highest-resolution monotonic
 *  time source on each supported platform.
 *
 *  @return The current time, in seconds, or zero if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function may be called from any thread.  Reading and
 *  writing of the internal base time is not atomic, so it needs to be
 *  externally synchronized with calls to @ref glfwSetTime.
 *
 *  @sa @ref time
 *
 *  @since Added in version 1.0.
 *
 *  @ingroup input
 */
GLFWAPI double glfwGetTime(void);

/*! @brief Sets the GLFW time.
 *
 *  This function sets the current GLFW time, in seconds.  The value must be
 *  a positive finite number less than or equal to 18446744073.0, which is
 *  approximately 584.5 years.
 *
 *  This function and @ref glfwGetTime are helper functions on top of @ref
 *  glfwGetTimerFrequency and @ref glfwGetTimerValue.
 *
 *  @param[in] time The new value, in seconds.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_INVALID_VALUE.
 *
 *  @remark The upper limit of GLFW time is calculated as
 *  floor((2<sup>64</sup> - 1) / 10<sup>9</sup>) and is due to implementations
 *  storing nanoseconds in 64 bits.  The limit may be increased in the future.
 *
 *  @thread_safety This function may be called from any thread.  Reading and
 *  writing of the internal base time is not atomic, so it needs to be
 *  externally synchronized with calls to @ref glfwGetTime.
 *
 *  @sa @ref time
 *
 *  @since Added in version 2.2.
 *
 *  @ingroup input
 */
GLFWAPI void glfwSetTime(double time);

/*! @brief Returns the current value of the raw timer.
 *
 *  This function returns the current value of the raw timer, measured in
 *  1&nbsp;/&nbsp;frequency seconds.  To get the frequency, call @ref
 *  glfwGetTimerFrequency.
 *
 *  @return The value of the timer, or zero if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function may be called from any thread.
 *
 *  @sa @ref time
 *  @sa @ref glfwGetTimerFrequency
 *
 *  @since Added in version 3.2.
 *
 *  @ingroup input
 */
GLFWAPI uint64_t glfwGetTimerValue(void);

/*! @brief Returns the frequency, in Hz, of the raw timer.
 *
 *  This function returns the frequency, in Hz, of the raw timer.
 *
 *  @return The frequency of the timer, in Hz, or zero if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function may be called from any thread.
 *
 *  @sa @ref time
 *  @sa @ref glfwGetTimerValue
 *
 *  @since Added in version 3.2.
 *
 *  @ingroup input
 */
GLFWAPI uint64_t glfwGetTimerFrequency(void);

/*! @brief Makes the context of the specified window current for the calling
 *  thread.
 *
 *  This function makes the OpenGL or OpenGL ES context of the specified window
 *  current on the calling thread.  A context must only be made current on
 *  a single thread at a time and each thread can have only a single current
 *  context at a time.
 *
 *  When moving a context between threads, you must make it non-current on the
 *  old thread before making it current on the new one.
 *
 *  By default, making a context non-current implicitly forces a pipeline flush.
 *  On machines that support `GL_KHR_context_flush_control`, you can control
 *  whether a context performs this flush by setting the
 *  [GLFW_CONTEXT_RELEASE_BEHAVIOR](@ref GLFW_CONTEXT_RELEASE_BEHAVIOR_hint)
 *  hint.
 *
 *  The specified window must have an OpenGL or OpenGL ES context.  Specifying
 *  a window without a context will generate a @ref GLFW_NO_WINDOW_CONTEXT
 *  error.
 *
 *  @param[in] window The window whose context to make current, or `NULL` to
 *  detach the current context.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_NO_WINDOW_CONTEXT and @ref GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function may be called from any thread.
 *
 *  @sa @ref context_current
 *  @sa @ref glfwGetCurrentContext
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup context
 */
GLFWAPI void glfwMakeContextCurrent(GLFWwindow* window);

/*! @brief Returns the window whose context is current on the calling thread.
 *
 *  This function returns the window whose OpenGL or OpenGL ES context is
 *  current on the calling thread.
 *
 *  @return The window whose context is current, or `NULL` if no window's
 *  context is current.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function may be called from any thread.
 *
 *  @sa @ref context_current
 *  @sa @ref glfwMakeContextCurrent
 *
 *  @since Added in version 3.0.
 *
 *  @ingroup context
 */
GLFWAPI GLFWwindow* glfwGetCurrentContext(void);

/*! @brief Swaps the front and back buffers of the specified window.
 *
 *  This function swaps the front and back buffers of the specified window when
 *  rendering with OpenGL or OpenGL ES.  If the swap interval is greater than
 *  zero, the GPU driver waits the specified number of screen updates before
 *  swapping the buffers.
 *
 *  The specified window must have an OpenGL or OpenGL ES context.  Specifying
 *  a window without a context will generate a @ref GLFW_NO_WINDOW_CONTEXT
 *  error.
 *
 *  This function does not apply to Vulkan.  If you are rendering with Vulkan,
 *  see `vkQueuePresentKHR` instead.
 *
 *  @param[in] window The window whose buffers to swap.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_NO_WINDOW_CONTEXT and @ref GLFW_PLATFORM_ERROR.
 *
 *  @remark __EGL:__ The context of the specified window must be current on the
 *  calling thread.
 *
 *  @thread_safety This function may be called from any thread.
 *
 *  @sa @ref buffer_swap
 *  @sa @ref glfwSwapInterval
 *
 *  @since Added in version 1.0.
 *  @glfw3 Added window handle parameter.
 *
 *  @ingroup window
 */
GLFWAPI void glfwSwapBuffers(GLFWwindow* window);

/*! @brief Sets the swap interval for the current context.
 *
 *  This function sets the swap interval for the current OpenGL or OpenGL ES
 *  context, i.e. the number of screen updates to wait from the time @ref
 *  glfwSwapBuffers was called before swapping the buffers and returning.  This
 *  is sometimes called _vertical synchronization_, _vertical retrace
 *  synchronization_ or just _vsync_.
 *
 *  A context that supports either of the `WGL_EXT_swap_control_tear` and
 *  `GLX_EXT_swap_control_tear` extensions also accepts _negative_ swap
 *  intervals, which allows the driver to swap immediately even if a frame
 *  arrives a little bit late.  You can check for these extensions with @ref
 *  glfwExtensionSupported.
 *
 *  A context must be current on the calling thread.  Calling this function
 *  without a current context will cause a @ref GLFW_NO_CURRENT_CONTEXT error.
 *
 *  This function does not apply to Vulkan.  If you are rendering with Vulkan,
 *  see the present mode of your swapchain instead.
 *
 *  @param[in] interval The minimum number of screen updates to wait for
 *  until the buffers are swapped by @ref glfwSwapBuffers.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_NO_CURRENT_CONTEXT and @ref GLFW_PLATFORM_ERROR.
 *
 *  @remark This function is not called during context creation, leaving the
 *  swap interval set to whatever is the default on that platform.  This is done
 *  because some swap interval extensions used by GLFW do not allow the swap
 *  interval to be reset to zero once it has been set to a non-zero value.
 *
 *  @remark Some GPU drivers do not honor the requested swap interval, either
 *  because of a user setting that overrides the application's request or due to
 *  bugs in the driver.
 *
 *  @thread_safety This function may be called from any thread.
 *
 *  @sa @ref buffer_swap
 *  @sa @ref glfwSwapBuffers
 *
 *  @since Added in version 1.0.
 *
 *  @ingroup context
 */
GLFWAPI void glfwSwapInterval(int interval);

/*! @brief Returns whether the specified extension is available.
 *
 *  This function returns whether the specified
 *  [API extension](@ref context_glext) is supported by the current OpenGL or
 *  OpenGL ES context.  It searches both for client API extension and context
 *  creation API extensions.
 *
 *  A context must be current on the calling thread.  Calling this function
 *  without a current context will cause a @ref GLFW_NO_CURRENT_CONTEXT error.
 *
 *  As this functions retrieves and searches one or more extension strings each
 *  call, it is recommended that you cache its results if it is going to be used
 *  frequently.  The extension strings will not change during the lifetime of
 *  a context, so there is no danger in doing this.
 *
 *  This function does not apply to Vulkan.  If you are using Vulkan, see @ref
 *  glfwGetRequiredInstanceExtensions, `vkEnumerateInstanceExtensionProperties`
 *  and `vkEnumerateDeviceExtensionProperties` instead.
 *
 *  @param[in] extension The ASCII encoded name of the extension.
 *  @return `GLFW_TRUE` if the extension is available, or `GLFW_FALSE`
 *  otherwise.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_NO_CURRENT_CONTEXT, @ref GLFW_INVALID_VALUE and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @thread_safety This function may be called from any thread.
 *
 *  @sa @ref context_glext
 *  @sa @ref glfwGetProcAddress
 *
 *  @since Added in version 1.0.
 *
 *  @ingroup context
 */
GLFWAPI int glfwExtensionSupported(const char* extension);

/*! @brief Returns the address of the specified function for the current
 *  context.
 *
 *  This function returns the address of the specified OpenGL or OpenGL ES
 *  [core or extension function](@ref context_glext), if it is supported
 *  by the current context.
 *
 *  A context must be current on the calling thread.  Calling this function
 *  without a current context will cause a @ref GLFW_NO_CURRENT_CONTEXT error.
 *
 *  This function does not apply to Vulkan.  If you are rendering with Vulkan,
 *  see @ref glfwGetInstanceProcAddress, `vkGetInstanceProcAddr` and
 *  `vkGetDeviceProcAddr` instead.
 *
 *  @param[in] procname The ASCII encoded name of the function.
 *  @return The address of the function, or `NULL` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_NO_CURRENT_CONTEXT and @ref GLFW_PLATFORM_ERROR.
 *
 *  @remark The address of a given function is not guaranteed to be the same
 *  between contexts.
 *
 *  @remark This function may return a non-`NULL` address despite the
 *  associated version or extension not being available.  Always check the
 *  context version or extension string first.
 *
 *  @pointer_lifetime The returned function pointer is valid until the context
 *  is destroyed or the library is terminated.
 *
 *  @thread_safety This function may be called from any thread.
 *
 *  @sa @ref context_glext
 *  @sa @ref glfwExtensionSupported
 *
 *  @since Added in version 1.0.
 *
 *  @ingroup context
 */
GLFWAPI GLFWglproc glfwGetProcAddress(const char* procname);

/*! @brief Returns whether the Vulkan loader and an ICD have been found.
 *
 *  This function returns whether the Vulkan loader and any minimally functional
 *  ICD have been found.
 *
 *  The availability of a Vulkan loader and even an ICD does not by itself guarantee that
 *  surface creation or even instance creation is possible.  Call @ref
 *  glfwGetRequiredInstanceExtensions to check whether the extensions necessary for Vulkan
 *  surface creation are available and @ref glfwGetPhysicalDevicePresentationSupport to
 *  check whether a queue family of a physical device supports image presentation.
 *
 *  @return `GLFW_TRUE` if Vulkan is minimally available, or `GLFW_FALSE`
 *  otherwise.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
 *
 *  @thread_safety This function may be called from any thread.
 *
 *  @sa @ref vulkan_support
 *
 *  @since Added in version 3.2.
 *
 *  @ingroup vulkan
 */
GLFWAPI int glfwVulkanSupported(void);

/*! @brief Returns the Vulkan instance extensions required by GLFW.
 *
 *  This function returns an array of names of Vulkan instance extensions required
 *  by GLFW for creating Vulkan surfaces for GLFW windows.  If successful, the
 *  list will always contain `VK_KHR_surface`, so if you don't require any
 *  additional extensions you can pass this list directly to the
 *  `VkInstanceCreateInfo` struct.
 *
 *  If Vulkan is not available on the machine, this function returns `NULL` and
 *  generates a @ref GLFW_API_UNAVAILABLE error.  Call @ref glfwVulkanSupported
 *  to check whether Vulkan is at least minimally available.
 *
 *  If Vulkan is available but no set of extensions allowing window surface
 *  creation was found, this function returns `NULL`.  You may still use Vulkan
 *  for off-screen rendering and compute work.
 *
 *  @param[out] count Where to store the number of extensions in the returned
 *  array.  This is set to zero if an error occurred.
 *  @return An array of ASCII encoded extension names, or `NULL` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_API_UNAVAILABLE.
 *
 *  @remark Additional extensions may be required by future versions of GLFW.
 *  You should check if any extensions you wish to enable are already in the
 *  returned array, as it is an error to specify an extension more than once in
 *  the `VkInstanceCreateInfo` struct.
 *
 *  @pointer_lifetime The returned array is allocated and freed by GLFW.  You
 *  should not free it yourself.  It is guaranteed to be valid only until the
 *  library is terminated.
 *
 *  @thread_safety This function may be called from any thread.
 *
 *  @sa @ref vulkan_ext
 *  @sa @ref glfwCreateWindowSurface
 *
 *  @since Added in version 3.2.
 *
 *  @ingroup vulkan
 */
GLFWAPI const char** glfwGetRequiredInstanceExtensions(uint32_t* count);

#if defined(VK_VERSION_1_0)

/*! @brief Returns the address of the specified Vulkan instance function.
 *
 *  This function returns the address of the specified Vulkan core or extension
 *  function for the specified instance.  If instance is set to `NULL` it can
 *  return any function exported from the Vulkan loader, including at least the
 *  following functions:
 *
 *  - `vkEnumerateInstanceExtensionProperties`
 *  - `vkEnumerateInstanceLayerProperties`
 *  - `vkCreateInstance`
 *  - `vkGetInstanceProcAddr`
 *
 *  If Vulkan is not available on the machine, this function returns `NULL` and
 *  generates a @ref GLFW_API_UNAVAILABLE error.  Call @ref glfwVulkanSupported
 *  to check whether Vulkan is at least minimally available.
 *
 *  This function is equivalent to calling `vkGetInstanceProcAddr` with
 *  a platform-specific query of the Vulkan loader as a fallback.
 *
 *  @param[in] instance The Vulkan instance to query, or `NULL` to retrieve
 *  functions related to instance creation.
 *  @param[in] procname The ASCII encoded name of the function.
 *  @return The address of the function, or `NULL` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED and @ref
 *  GLFW_API_UNAVAILABLE.
 *
 *  @pointer_lifetime The returned function pointer is valid until the library
 *  is terminated.
 *
 *  @thread_safety This function may be called from any thread.
 *
 *  @sa @ref vulkan_proc
 *
 *  @since Added in version 3.2.
 *
 *  @ingroup vulkan
 */
GLFWAPI GLFWvkproc glfwGetInstanceProcAddress(VkInstance instance, const char* procname);

/*! @brief Returns whether the specified queue family can present images.
 *
 *  This function returns whether the specified queue family of the specified
 *  physical device supports presentation to the platform GLFW was built for.
 *
 *  If Vulkan or the required window surface creation instance extensions are
 *  not available on the machine, or if the specified instance was not created
 *  with the required extensions, this function returns `GLFW_FALSE` and
 *  generates a @ref GLFW_API_UNAVAILABLE error.  Call @ref glfwVulkanSupported
 *  to check whether Vulkan is at least minimally available and @ref
 *  glfwGetRequiredInstanceExtensions to check what instance extensions are
 *  required.
 *
 *  @param[in] instance The instance that the physical device belongs to.
 *  @param[in] device The physical device that the queue family belongs to.
 *  @param[in] queuefamily The index of the queue family to query.
 *  @return `GLFW_TRUE` if the queue family supports presentation, or
 *  `GLFW_FALSE` otherwise.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_API_UNAVAILABLE and @ref GLFW_PLATFORM_ERROR.
 *
 *  @remark @macos This function currently always returns `GLFW_TRUE`, as the
 *  `VK_MVK_macos_surface` and `VK_EXT_metal_surface` extensions do not provide
 *  a `vkGetPhysicalDevice*PresentationSupport` type function.
 *
 *  @thread_safety This function may be called from any thread.  For
 *  synchronization details of Vulkan objects, see the Vulkan specification.
 *
 *  @sa @ref vulkan_present
 *
 *  @since Added in version 3.2.
 *
 *  @ingroup vulkan
 */
GLFWAPI int glfwGetPhysicalDevicePresentationSupport(VkInstance instance, VkPhysicalDevice device, uint32_t queuefamily);

/*! @brief Creates a Vulkan surface for the specified window.
 *
 *  This function creates a Vulkan surface for the specified window.
 *
 *  If the Vulkan loader or at least one minimally functional ICD were not found,
 *  this function returns `VK_ERROR_INITIALIZATION_FAILED` and generates a @ref
 *  GLFW_API_UNAVAILABLE error.  Call @ref glfwVulkanSupported to check whether
 *  Vulkan is at least minimally available.
 *
 *  If the required window surface creation instance extensions are not
 *  available or if the specified instance was not created with these extensions
 *  enabled, this function returns `VK_ERROR_EXTENSION_NOT_PRESENT` and
 *  generates a @ref GLFW_API_UNAVAILABLE error.  Call @ref
 *  glfwGetRequiredInstanceExtensions to check what instance extensions are
 *  required.
 *
 *  The window surface cannot be shared with another API so the window must
 *  have been created with the [client api hint](@ref GLFW_CLIENT_API_attrib)
 *  set to `GLFW_NO_API` otherwise it generates a @ref GLFW_INVALID_VALUE error
 *  and returns `VK_ERROR_NATIVE_WINDOW_IN_USE_KHR`.
 *
 *  The window surface must be destroyed before the specified Vulkan instance.
 *  It is the responsibility of the caller to destroy the window surface.  GLFW
 *  does not destroy it for you.  Call `vkDestroySurfaceKHR` to destroy the
 *  surface.
 *
 *  @param[in] instance The Vulkan instance to create the surface in.
 *  @param[in] window The window to create the surface for.
 *  @param[in] allocator The allocator to use, or `NULL` to use the default
 *  allocator.
 *  @param[out] surface Where to store the handle of the surface.  This is set
 *  to `VK_NULL_HANDLE` if an error occurred.
 *  @return `VK_SUCCESS` if successful, or a Vulkan error code if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_NOT_INITIALIZED, @ref
 *  GLFW_API_UNAVAILABLE, @ref GLFW_PLATFORM_ERROR and @ref GLFW_INVALID_VALUE
 *
 *  @remark If an error occurs before the creation call is made, GLFW returns
 *  the Vulkan error code most appropriate for the error.  Appropriate use of
 *  @ref glfwVulkanSupported and @ref glfwGetRequiredInstanceExtensions should
 *  eliminate almost all occurrences of these errors.
 *
 *  @remark @macos GLFW prefers the `VK_EXT_metal_surface` extension, with the
 *  `VK_MVK_macos_surface` extension as a fallback.  The name of the selected
 *  extension, if any, is included in the array returned by @ref
 *  glfwGetRequiredInstanceExtensions.
 *
 *  @remark @macos This function creates and sets a `CAMetalLayer` instance for
 *  the window content view, which is required for MoltenVK to function.
 *
 *  @thread_safety This function may be called from any thread.  For
 *  synchronization details of Vulkan objects, see the Vulkan specification.
 *
 *  @sa @ref vulkan_surface
 *  @sa @ref glfwGetRequiredInstanceExtensions
 *
 *  @since Added in version 3.2.
 *
 *  @ingroup vulkan
 */
GLFWAPI VkResult glfwCreateWindowSurface(VkInstance instance, GLFWwindow* window, const VkAllocationCallbacks* allocator, VkSurfaceKHR* surface);

#endif /*VK_VERSION_1_0*/


/*************************************************************************
 * Global definition cleanup
 *************************************************************************/

/* ------------------- BEGIN SYSTEM/COMPILER SPECIFIC -------------------- */

#ifdef GLFW_WINGDIAPI_DEFINED
 #undef WINGDIAPI
 #undef GLFW_WINGDIAPI_DEFINED
#endif

#ifdef GLFW_CALLBACK_DEFINED
 #undef CALLBACK
 #undef GLFW_CALLBACK_DEFINED
#endif

/* Some OpenGL related headers need GLAPIENTRY, but it is unconditionally
 * defined by some gl.h variants (OpenBSD) so define it after if needed.
 */
#ifndef GLAPIENTRY
 #define GLAPIENTRY APIENTRY
 #define GLFW_GLAPIENTRY_DEFINED
#endif

/* -------------------- END SYSTEM/COMPILER SPECIFIC --------------------- */


#ifdef __cplusplus
}
#endif

#endif /* _glfw3_h_ */




#ifdef TURTLE_IMPLEMENTATION
#define UNITYPE_LIST_IMPLEMENTATION
#define TURTLE_INTERNAL_IMPLEMENTATION
#define TURTLE_TEXT_IMPLEMENTATION
#define TURTLE_TOOLS_IMPLEMENTATION
#define OS_TOOLS_IMPLEMENTATION
#endif /* TURTLE_IMPLEMENTATION */

#ifdef TURTLE_INTERNAL_IMPLEMENTATION
#ifndef UNITYPE_LIST_IMPLEMENTATION
#error UNITYPE_LIST_IMPLEMENTATION not defined
#endif /* UNITYPE_LIST_IMPLEMENTATION */
#endif /* TURTLE_INTERNAL_IMPLEMENTATION */


#ifdef TURTLE_TEXT_IMPLEMENTATION
#ifndef TURTLE_INTERNAL_IMPLEMENTATION
#error TURTLE_INTERNAL_IMPLEMENTATION not defined
#endif /* TURTLE_INTERNAL_IMPLEMENTATION */
#endif /* TURTLE_TEXT_IMPLEMENTATION */


#ifdef TURTLE_TOOLS_IMPLEMENTATION
#ifndef TURTLE_TEXT_IMPLEMENTATION
#error TURTLE_TEXT_IMPLEMENTATION not defined
#endif /* TURTLE_TEXT_IMPLEMENTATION */
#endif /* TURTLE_TOOLS_IMPLEMENTATION */


#ifdef OS_TOOLS_IMPLEMENTATION
#ifndef UNITYPE_LIST_IMPLEMENTATION
#error UNITYPE_LIST_IMPLEMENTATION not defined
#endif /* UNITYPE_LIST_IMPLEMENTATION */
#endif /* OS_TOOLS_IMPLEMENTATION */

/*
███████╗ ██████╗ ██╗   ██╗██████╗  ██████╗███████╗
██╔════╝██╔═══██╗██║   ██║██╔══██╗██╔════╝██╔════╝
███████╗██║   ██║██║   ██║██████╔╝██║     █████╗  
╚════██║██║   ██║██║   ██║██╔══██╗██║     ██╔══╝  
███████║╚██████╔╝╚██████╔╝██║  ██║╚██████╗███████╗
╚══════╝ ╚═════╝  ╚═════╝ ╚═╝  ╚═╝ ╚═════╝╚══════╝
https://patorjk.com/software/taag/#p=display&f=ANSI%20Shadow
*/
#ifdef UNITYPE_LIST_IMPLEMENTATION
/*
██╗     ██╗███████╗████████╗ ██████╗
██║     ██║██╔════╝╚══██╔══╝██╔════╝
██║     ██║███████╗   ██║   ██║     
██║     ██║╚════██║   ██║   ██║     
███████╗██║███████║   ██║██╗╚██████╗
╚══════╝╚═╝╚══════╝   ╚═╝╚═╝ ╚═════╝
https://patorjk.com/software/taag/#p=display&f=ANSI%20Shadow

21.04.23:
unitype list, supports a variety of types

example usage:
list_t *newList = list_init();
list_append(newList, (unitype) "hello", 's'); // add to list
list_append(newList, (unitype) "test", LIST_TYPE_STRING); // add to list using enum
list_print(newList);
> [hello, test]
list_insert(newList, (unitype) 128, 'i', 1);
list_insert(newList, (unitype) 12.0, 'd', 1);
list_print(newList);
> [hello, 12.0, 128, test]
printf("%s\n", newList -> data[3].s);
> test
printf("%d\n", newList -> length);
> 4 
list_delete(newList, 0);
list_print(newList);
> [12.0, 128, test]
list_clear(newList);
list_print(newList);
> []
list_free(newList);

Notes:
Strings added to the list will be "strdup"d - meaning that you can pass in stack allocated buffers and string literals. This does not apply to pointers added to the list which must be heap allocated
When calling list_clear() or list_free(), the list will free all strings, pointers, and lists within itself. If you don't want this to happen append the item to the list as a uint64
list_copy will make a copy of all strings, pointers, and lists - it will not use the same pointers (a list can be safely freed after being copied without causing effects to the copied list)
You must call list_init() when intending to copy a list - all lists must be initialised before any functions can be called on them (if your program is crashing - check to make sure you initialised all your lists)
*/


/* create a list */
list_t *list_init() {
    list_t *list = malloc(sizeof(list_t));
    list -> length = 0;
    list -> realLength = 1;
    list -> type = calloc(1, sizeof(char));
    list -> data = calloc(1, sizeof(unitype));
    return list;
}

/* append to list, must specify type */
void list_append(list_t *list, unitype data, char type) {
    if (list -> realLength  <= list -> length) {
        list -> realLength *= 2;
        list -> type = realloc(list -> type, list -> realLength);
        list -> data = realloc(list -> data, list -> realLength * sizeof(unitype));
    }
    if (type == 'z') {
        list -> type[list -> length] = 's';
    } else {
        list -> type[list -> length] = type;
    }
    if (type == 's') {
        list -> data[list -> length].s = strdup(data.s);
    } else {
        list -> data[list -> length] = data;
    }
    list -> length += 1;
}

/* inserts the item value at list[index] of the list */
void list_insert(list_t *list, int32_t index, unitype value, char type) {
    if (list -> length == index) {
        list_append(list, value, type);
        return;
    }
    while (index < 0) {index += list -> length;}
    index %= list -> length;
    list_append(list, (unitype) 0, type);
    int32_t i;
    for (i = list -> length - 1; i > index; i--) {
        list -> data[i] = list -> data[i - 1];
        list -> type[i] = list -> type[i - 1];
    }
    list -> data[i] = value;
}

/* clears the list */
void list_clear(list_t *list) {
    list_free_lite(list);
    list -> length = 0;
    list -> realLength = 1;
    list -> type = calloc(1, sizeof(char));
    list -> data = calloc(1, sizeof(unitype));
}

/* pops the last item of the list off and returns it */
unitype list_pop(list_t *list) {
    if (list -> length > 0) {
        list -> length -= 1;
        unitype ret = list -> data[list -> length];
        if (list -> type[list -> length] == 'r') {
            list_free(list -> data[list -> length].r);
        }
        if (list -> type[list -> length] == 's' || list -> type[list -> length] == 'p') {
            free(list -> data[list -> length].p);
        }
        list -> type[list -> length] = (char) 0;
        list -> data[list -> length] = (unitype) 0;
        if (list -> length <= list -> realLength / 2 && list -> realLength > 1) {
            list -> realLength /= 2;
            list -> type = realloc(list -> type, list -> realLength);
            list -> data = realloc(list -> data, list -> realLength * sizeof(unitype));
        }
        return ret;
    } else {
        return (unitype) 0;
    }
}

/* deletes the item at list[index] of the list and returns it */
unitype list_delete(list_t *list, int32_t index) {
    while (index < 0) {index += list -> length;}
    index %= list -> length;
    unitype ret = list -> data[index];
    if (list -> type[index] == 'r') {
        list_free(list -> data[index].r);
    }
    if (list -> type[index] == 's' || list -> type[index] == 'p') {
        free(list -> data[index].p);
    }
    for (uint32_t i = index; i < list -> length - 1 ; i++) {
        list -> data[i] = list -> data[i + 1];
        list -> type[i] = list -> type[i + 1];
    }
    list -> length -= 1;
    list -> type[list -> length] = (char) 0;
    list -> data[list -> length] = (unitype) 0;
    if (list -> length <= list -> realLength / 2 && list -> realLength > 1) {
        list -> realLength /= 2;
        list -> type = realloc(list -> type, list -> realLength);
        list -> data = realloc(list -> data, list -> realLength * sizeof(unitype));
    }
    return ret;
}

/* deletes many items from the list spanning from [indexMin] to [indexMax - 1] */
void list_delete_range(list_t *list, uint32_t indexMin, uint32_t indexMax) {
    if (indexMin > indexMax) {
        uint32_t swap = indexMin;
        indexMin = indexMax;
        indexMax = swap;
    }
    int8_t zerod = 0; // edge case: "should've used list_clear"
    uint32_t difference = (indexMax - indexMin);
    list -> realLength = list -> length - difference;
    if (list -> realLength <= 1) {
        zerod = 1;
        list -> realLength = 1;
    }
    
    int8_t *newType = malloc(list -> realLength * sizeof(int8_t)); // no need to calloc we're gonna fill it all up anyway
    unitype *newData = malloc(list -> realLength * sizeof(unitype));
    for (uint32_t i = 0; i < indexMin; i++) {
        newType[i] = list -> type[i];
        newData[i] = list -> data[i];
    }
    for (uint32_t i = indexMax; i < list -> length; i++) {
        newType[i - difference] = list -> type[i];
        newData[i - difference] = list -> data[i];
    }
    list -> length = list -> realLength;
    if (zerod)
        list -> length = 0;
    free(list -> type);
    free(list -> data);
    list -> type = newType;
    list -> data = newData;
}

/* checks if two unitype items are equal */
int32_t unitype_check_equal(unitype item1, unitype item2, int8_t typeItem1, int8_t typeItem2) {
    if ((typeItem1 == 's' || typeItem2 == 's' || typeItem1 == 'p' || typeItem2 == 'p' || typeItem1 == 'r' || typeItem2 == 'r') && typeItem1 != typeItem2) {
        return 0;
    }

    switch (typeItem1) {
        case LIST_TYPE_CHAR:
            if (item1.c == item2.c) {return 1;}
            return 0;
        case LIST_TYPE_INT8:
        case LIST_TYPE_UINT8: // UINT8 or BOOL
            if (item1.b == item2.b) {return 1;}
            return 0;
        case LIST_TYPE_INT16:
        case LIST_TYPE_UINT16:
            if (item1.hu == item2.hu) {return 1;}
            return 0;
        case LIST_TYPE_INT32:
        case LIST_TYPE_UINT32:
            if (item1.u == item2.u) {return 1;}
            return 0;
        case LIST_TYPE_INT64:
        case LIST_TYPE_UINT64:
            if (item1.l == item2.l) {return 1;}
            return 0;
        case LIST_TYPE_FLOAT:
            if (item1.f == item2.f) {return 1;}
            return 0;
        case LIST_TYPE_DOUBLE:
            if (item1.d == item2.d) {return 1;}
            return 0;
        case LIST_TYPE_STRING:
            if (strcmp(item1.s, item2.s) == 0) {return 1;}
            return 0;
        case LIST_TYPE_POINTER:
            if (item1.p == item2.p) {return 1;} // questionable
            return 0;
        case LIST_TYPE_LIST:
            if (item1.r == item2.r) {return 1;} // questionable^2 (doesn't check if lists are equivalent/congruent, just compares memory location)
            return 0;
        default:
            printf("unitype_check_equal - type %d not recognized\n", typeItem1);
            return 0;
    }
}

/* returns the index of the first instance of the item in the list, returns -1 if not found */
int32_t list_find(list_t *list, unitype item, char type) {
    for (uint32_t i = 0; i < list -> length; i++) {
        if (unitype_check_equal(list -> data[i], item, list -> type[i], type)) {
            return i;
        }
    }
    return -1;
}

/* duplicate of list_find */
int32_t list_index(list_t *list, unitype item, char type) {
    for (uint32_t i = 0; i < list -> length; i++) {
        if (unitype_check_equal(list -> data[i], item, list -> type[i], type)) {
            return i;
        }
    }
    return -1;
}

/* counts how many instances of an item is found in the list */
uint32_t list_count(list_t *list, unitype item, char type) {
    uint32_t count = 0;
    for (uint32_t i = 0; i < list -> length; i++) {
        count += unitype_check_equal(list -> data[i], item, list -> type[i], type);
    }
    return count;
}

/* sort list */
void list_sort(list_t *list) {
    /* create min heap */
    int32_t temp;
    for (uint32_t i = 2; i < list -> length + 1; i++) {
        int32_t j = i;
        while (j > 1 && list -> data[j / 2 - 1].i > list -> data[j - 1].i) {
            temp = list -> data[j / 2 - 1].i;
            list -> data[j / 2 - 1].i = list -> data[j - 1].i;
            list -> data[j - 1].i = temp;
            j /= 2;
        }
    }
    /* heapsort */
    for (int32_t i = list -> length - 1; i > 0; i--) {
        temp = list -> data[0].i;
        list -> data[0].i = list -> data[i].i;
        list -> data[i].i = temp;
        int32_t j = 1;
        while ((j * 2 - 1 < i && list -> data[j - 1].i > list -> data[j * 2 - 1].i) || (j * 2 < i && list -> data[j - 1].i > list -> data[j * 2].i)) {
            if (list -> data[j * 2 - 1].i < list -> data[j * 2].i || j * 2 == i) {
                temp = list -> data[j - 1].i;
                list -> data[j - 1] = list -> data[j * 2 - 1];
                list -> data[j * 2 - 1].i = temp;
                j = j * 2;
            } else {
                temp = list -> data[j - 1].i;
                list -> data[j - 1].i = list -> data[j * 2].i;
                list -> data[j * 2].i = temp;
                j *= 2 + 1;
            }
        }
    }
}

/* deletes the first instance of the item from the list, returns the index the item was at, returns -1 and doesn't modify the list if not found */
int32_t list_remove(list_t *list, unitype item, char type) {
    for (uint32_t i = 0; i < list -> length; i++) {
        if (unitype_check_equal(list -> data[i], item, list -> type[i], type)) {
            list_delete(list, i);
            return i;
        }
    }
    return -1;
}

/* prints a unitype item */
void unitype_fprint(FILE *fp, unitype item, char type) {
    switch (type) {
        case LIST_TYPE_CHAR:
            fprintf(fp, "%c", item.c);
        break;
        case LIST_TYPE_INT8:
            fprintf(fp, "%hhi", item.b);
        break;
        case LIST_TYPE_UINT8: // UINT8 or BOOL
            fprintf(fp, "%hhu", item.b);
        break;
        case LIST_TYPE_INT16:
            fprintf(fp, "%hi", item.h);
        break;
        case LIST_TYPE_UINT16:
            fprintf(fp, "%hu", item.hu);
        break;
        case LIST_TYPE_INT32:
            fprintf(fp, "%d", item.i);
        break;
        case LIST_TYPE_UINT32:
            fprintf(fp, "%u", item.u);
        break;
        case LIST_TYPE_INT64:
            fprintf(fp, "%lli", item.li);
        break;
        case LIST_TYPE_UINT64:
            fprintf(fp, "%llu", item.l);
        break;
        case LIST_TYPE_FLOAT:
            fprintf(fp, "%f", item.f);
        break;
        case LIST_TYPE_DOUBLE:
            fprintf(fp, "%lf", item.d);
        break;
        case LIST_TYPE_STRING:
            fprintf(fp, "%s", item.s);
        break;
        case LIST_TYPE_POINTER:
            fprintf(fp, "%p", item.p);
        break;
        case LIST_TYPE_LIST:
            list_fprint_emb(fp, item.r);
        break;
        default:
            printf("unitype_fprint - type %d not recognized\n", type);
            return;
    }
}

/* copies one list to another (duplicates strings or pointers) */
void list_copy(list_t *dest, list_t *src) {
    list_free_lite(dest);
    dest -> type = calloc(src -> realLength, sizeof(int32_t));
    dest -> data = calloc(src -> realLength, sizeof(unitype));
    uint32_t len = src -> length;
    dest -> length = len;
    dest -> realLength = src -> realLength;
    for (uint32_t i = 0; i < len; i++) {
        dest -> type[i] = src -> type[i];
        if (src -> type[i] == 'r') {
            dest -> data[i] = (unitype) (void *) list_init();
            list_copy(dest -> data[i].r, src -> data[i].r);
        } else {
            if (src -> type[i] == 'p') {
                memcpy(dest -> data[i].p, src -> data[i].p, sizeof(unitype));
            } else {
                if (src -> type[i] == 's') {
                    dest -> data[i].s = strdup(src -> data[i].s);
                } else {
                    dest -> data[i] = src -> data[i];
                }
            }
        }
    }
}

/* prints the list without brackets */
void list_fprint_emb(FILE *fp, list_t *list) {
    fprintf(fp, "[");
    if (list -> length == 0) {
        fprintf(fp, "]");
        return;
    }
    for (uint32_t i = 0; i < list -> length; i++) {
        unitype_fprint(fp, list -> data[i], list -> type[i]);
        if (i == list -> length - 1) {
            fprintf(fp, "]");
        } else {
            fprintf(fp, ", ");
        }
    }
}

/* prints the list */
void list_print(list_t *list) {
    list_fprint_emb(stdout, list);
    printf("\n");
}

/* prints the types of the list */
void list_print_type(list_t *list) {
    printf("[");
    if (list -> length == 0) {
        printf("]\n");
        return;
    }
    for (uint32_t i = 0; i < list -> length; i++) {
        printf("%c", list -> type[i]);
        if (i == list -> length - 1) {
            printf("]\n");
        } else {
            printf(", ");
        }
    }
}

/* frees the list's data but not the list itself */
void list_free_lite(list_t *list) {
    for (uint32_t i = 0; i < list -> length; i++) {
        if (list -> type[i] == 'r') {
            list_free(list -> data[i].r);
        }
        if ((list -> type[i] == 's' || list -> type[i] == 'p') && list -> data[i].p != NULL) {
            free(list -> data[i].s);
        }
    }
    free(list -> type);
    free(list -> data);
}

/* frees the data used by the list */
void list_free(list_t *list) {
    list_free_lite(list);
    free(list);
}

#endif /* UNITYPE_LIST_IMPLEMENTATION */
#ifdef TURTLE_INTERNAL_IMPLEMENTATION
/*
████████╗██╗   ██╗██████╗ ████████╗██╗     ███████╗    ██████╗
╚══██╔══╝██║   ██║██╔══██╗╚══██╔══╝██║     ██╔════╝   ██╔════╝
   ██║   ██║   ██║██████╔╝   ██║   ██║     █████╗     ██║     
   ██║   ██║   ██║██╔══██╗   ██║   ██║     ██╔══╝     ██║     
   ██║   ╚██████╔╝██║  ██║   ██║   ███████╗███████╗██╗╚██████╗
   ╚═╝    ╚═════╝ ╚═╝  ╚═╝   ╚═╝   ╚══════╝╚══════╝╚═╝ ╚═════╝
https://patorjk.com/software/taag/#p=display&f=ANSI%20Shadow

openGL implementation of turtle and the turtletools module
features:
adjustable pen (size and colour)
resizable window support
keyboard and mouse presses
*/


turtle_t turtle;

/* run this to set the bounds of the window in coordinates */
void turtleSetWorldCoordinates(int32_t minX, int32_t minY, int32_t maxX, int32_t maxY) {
    glfwGetWindowSize(turtle.window, &turtle.screenbounds[0], &turtle.screenbounds[1]);
    turtle.centerAndScale[0] = (double) (maxX + minX) / 2;
    turtle.centerAndScale[1] = (double) (maxY + minY) / 2;
    turtle.centerAndScale[2] = (double) (maxX - minX) / 2 * turtle.screenbounds[0];
    turtle.centerAndScale[3] = (double) (maxY - minY) / 2 * turtle.screenbounds[1];
    turtle.initscreenbounds[0] = turtle.screenbounds[0];
    turtle.initscreenbounds[1] = turtle.screenbounds[1];
    turtle.initbounds[0] = minX;
    turtle.initbounds[1] = minY;
    turtle.initbounds[2] = maxX;
    turtle.initbounds[3] = maxY;
    memcpy(turtle.bounds, turtle.initbounds, 4 * sizeof(int32_t));
}

/* detect character */
void unicodeSense(GLFWwindow *window, uint32_t codepoint) {
    if (turtle.unicodeCallback != NULL) {
        turtle.unicodeCallback(codepoint);
    }
}

/* detect key presses */
void keySense(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods) {
    if (turtle.keyCallback != NULL) {
        turtle.keyCallback(key, scancode, action);
    }
    if (action == GLFW_PRESS) {
        list_append(turtle.keyPressed, (unitype) key, 'i');
    }
    if (action == GLFW_RELEASE) {
        list_remove(turtle.keyPressed, (unitype) key, 'i');
    }
}

/* detect mouse clicks */
void mouseSense(GLFWwindow *window, int32_t button, int32_t action, int32_t mods) {
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
int8_t turtleKeyPressed(int32_t key) {
    return list_count(turtle.keyPressed, (unitype) key, 'c');
}

/* top level boolean output call to check if the left click button is currently being held down */
int8_t turtleMouseDown() {
    return turtle.mousePressed[0];
}

/* top level boolean output call to check if the right click button is currently being held down */
int8_t turtleMouseRight() {
    return turtle.mousePressed[1];
}

/* top level boolean output call to check if the middle mouse button is currently being held down */
int8_t turtleMouseMiddle() {
    return turtle.mousePressed[2];
}

/* alternate duplicate of top level boolean output call to check if the middle mouse button is currently being held down */
int8_t turtleMouseMid() {
    return turtle.mousePressed[2];
}

/* initializes the turtletools module */
void turtleInit(GLFWwindow* window, int32_t minX, int32_t minY, int32_t maxX, int32_t maxY) {
    gladLoadGL();
    glfwMakeContextCurrent(window); // various glfw things
    glEnable(GL_ALPHA);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
    glClearColor(1.0, 1.0, 1.0, 0.0); // white background by default
    turtle.window = window;
    turtle.close = 0;
    turtle.popupClose = 0;
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
    for (uint8_t i = 0; i < 3; i++) {
        turtle.currentColor[i] = 0.0;
    }
    turtle.currentColor[3] = 1.0;
    /* 3D variables */
    turtle.cameraX = 0;
    turtle.cameraY = 0;
    turtle.cameraZ = 0;
    turtle.cameraFOV = 90;
    turtle.cameraDirectionLeftRight = 0;
    turtle.cameraDirectionUpDown = 0;

    turtleSetWorldCoordinates(minX, minY, maxX, maxY);
    turtle.keyCallback = NULL;
    turtle.unicodeCallback = NULL;
    glfwSetCharCallback(window, unicodeSense);
    glfwSetKeyCallback(window, keySense); // initiate mouse and keyboard detection
    glfwSetMouseButtonCallback(window, mouseSense);
    glfwSetScrollCallback(window, scrollSense);
}

/* gets the mouse coordinates */
void turtleGetMouseCoords() {
    glfwGetCursorPos(turtle.window, &turtle.mouseAbsX, &turtle.mouseAbsY); // get mouse positions (absolute)
    turtle.mouseX = turtle.mouseAbsX;
    turtle.mouseY = turtle.mouseAbsY;
    turtle.mouseX = (turtle.mouseAbsX - turtle.screenbounds[0] / 2) / turtle.screenbounds[0] * (turtle.initbounds[2] - turtle.initbounds[0]);
    turtle.mouseY = (turtle.mouseAbsY - turtle.screenbounds[1] / 2) / turtle.screenbounds[1] * (turtle.initbounds[1] - turtle.initbounds[3]);
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
        int8_t changed = 0;
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
            int8_t changed = 0;
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
void turtleCircleRender(double x, double y, double rad, double r, double g, double b, double a, double xcenter, double ycenter, double xfact, double yfact, double prez) {
    int8_t colorChange = 0;
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
        glVertex2d((x + rad * sin(2 * i * M_PI / prez)) * xfact + xcenter, (y + rad * cos(2 * i * M_PI / prez)) * yfact + ycenter);
    }
    glEnd();
}

/* draws a square */
void turtleSquareRender(double x1, double y1, double x2, double y2, double r, double g, double b, double a, double xcenter, double ycenter, double xfact, double yfact) {
    int8_t colorChange = 0;
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
    glVertex2d(x1 * xfact + xcenter, y1 * yfact + ycenter);
    glVertex2d(x2 * xfact + xcenter, y1 * yfact + ycenter);
    glVertex2d(x2 * xfact + xcenter, y2 * yfact + ycenter);
    glVertex2d(x1 * xfact + xcenter, y2 * yfact + ycenter);
    glEnd();
}

/* draws a triangle */
void turtleTriangleRender(double x1, double y1, double x2, double y2, double x3, double y3, double r, double g, double b, double a, double xcenter, double ycenter, double xfact, double yfact) {
    int8_t colorChange = 0;
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
    glVertex2d(x1 * xfact + xcenter, y1 * yfact + ycenter);
    glVertex2d(x2 * xfact + xcenter, y2 * yfact + ycenter);
    glVertex2d(x3 * xfact + xcenter, y3 * yfact + ycenter);
    glEnd();
}

/* draws a quadrilateral */
void turtleQuadRender(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double r, double g, double b, double a, double xcenter, double ycenter, double xfact, double yfact) {
    int8_t colorChange = 0;
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
    glVertex2d(x1 * xfact + xcenter, y1 * yfact + ycenter);
    glVertex2d(x2 * xfact + xcenter, y2 * yfact + ycenter);
    glVertex2d(x3 * xfact + xcenter, y3 * yfact + ycenter);
    glVertex2d(x4 * xfact + xcenter, y4 * yfact + ycenter);
    glEnd();
}

/* adds a (blit) triangle to the pipeline (for better speed) */
void turtleTriangle(double x1, double y1, double x2, double y2, double x3, double y3) {
    list_append(turtle.penPos, (unitype) x1, 'd');
    list_append(turtle.penPos, (unitype) y1, 'd');
    list_append(turtle.penPos, (unitype) x2, 'd');
    list_append(turtle.penPos, (unitype) turtle.penr, 'd');
    list_append(turtle.penPos, (unitype) turtle.peng, 'd');
    list_append(turtle.penPos, (unitype) turtle.penb, 'd');
    list_append(turtle.penPos, (unitype) turtle.pena, 'd');
    list_append(turtle.penPos, (unitype) 66, 'h'); // blit triangle signifier
    list_append(turtle.penPos, (unitype) y2, 'd'); // some unconventional formatting but it works

    list_append(turtle.penPos, (unitype) x3, 'd');
    list_append(turtle.penPos, (unitype) y3, 'd');
    list_append(turtle.penPos, (unitype) 0, 'd'); // zero'd out (wasted space)
    list_append(turtle.penPos, (unitype) turtle.penr, 'd'); // duplicate colour data (wasted space)
    list_append(turtle.penPos, (unitype) turtle.peng, 'd');
    list_append(turtle.penPos, (unitype) turtle.penb, 'd');
    list_append(turtle.penPos, (unitype) turtle.pena, 'd');
    list_append(turtle.penPos, (unitype) 66, 'h'); // blit triangle signifier
    list_append(turtle.penPos, (unitype) 0, 'd'); // zero'd out (wasted space)
}

void turtleTriangleColor(double x1, double y1, double x2, double y2, double x3, double y3, double r, double g, double b, double a) {
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

/* adds a (blit) quad to the pipeline (for better speed) */
void turtleQuad(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4) {
    list_append(turtle.penPos, (unitype) x1, 'd');
    list_append(turtle.penPos, (unitype) y1, 'd');
    list_append(turtle.penPos, (unitype) x2, 'd');
    list_append(turtle.penPos, (unitype) turtle.penr, 'd');
    list_append(turtle.penPos, (unitype) turtle.peng, 'd');
    list_append(turtle.penPos, (unitype) turtle.penb, 'd');
    list_append(turtle.penPos, (unitype) turtle.pena, 'd');
    list_append(turtle.penPos, (unitype) 67, 'h'); // blit quad signifier
    list_append(turtle.penPos, (unitype) y2, 'd'); // some unconventional formatting but it works

    list_append(turtle.penPos, (unitype) x3, 'd');
    list_append(turtle.penPos, (unitype) y3, 'd');
    list_append(turtle.penPos, (unitype) x4, 'd');
    list_append(turtle.penPos, (unitype) turtle.penr, 'd'); // duplicate colour data (wasted space)
    list_append(turtle.penPos, (unitype) turtle.peng, 'd');
    list_append(turtle.penPos, (unitype) turtle.penb, 'd');
    list_append(turtle.penPos, (unitype) turtle.pena, 'd');
    list_append(turtle.penPos, (unitype) 67, 'h'); // blit quad signifier
    list_append(turtle.penPos, (unitype) y4, 'd');
}

void turtleQuadColor(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double r, double g, double b, double a) {
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
void turtleRectangle(double x1, double y1, double x2, double y2) {
    list_append(turtle.penPos, (unitype) x1, 'd');
    list_append(turtle.penPos, (unitype) y1, 'd');
    list_append(turtle.penPos, (unitype) x2, 'd');
    list_append(turtle.penPos, (unitype) turtle.penr, 'd');
    list_append(turtle.penPos, (unitype) turtle.peng, 'd');
    list_append(turtle.penPos, (unitype) turtle.penb, 'd');
    list_append(turtle.penPos, (unitype) turtle.pena, 'd');
    list_append(turtle.penPos, (unitype) 67, 'h'); // blit quad signifier
    list_append(turtle.penPos, (unitype) y1, 'd'); // some unconventional formatting but it works

    list_append(turtle.penPos, (unitype) x2, 'd');
    list_append(turtle.penPos, (unitype) y2, 'd');
    list_append(turtle.penPos, (unitype) x1, 'd');
    list_append(turtle.penPos, (unitype) turtle.penr, 'd'); // duplicate colour data (wasted space)
    list_append(turtle.penPos, (unitype) turtle.peng, 'd');
    list_append(turtle.penPos, (unitype) turtle.penb, 'd');
    list_append(turtle.penPos, (unitype) turtle.pena, 'd');
    list_append(turtle.penPos, (unitype) 67, 'h'); // blit quad signifier
    list_append(turtle.penPos, (unitype) y2, 'd');
}

void turtleRectangleColor(double x1, double y1, double x2, double y2, double r, double g, double b, double a) {
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

/* adds a (blit) circle to the pipeline */
void turtleCircle(double x, double y, double radius) {
    list_append(turtle.penPos, (unitype) x, 'd');
    list_append(turtle.penPos, (unitype) y, 'd');
    list_append(turtle.penPos, (unitype) radius, 'd');
    list_append(turtle.penPos, (unitype) turtle.penr, 'd');
    list_append(turtle.penPos, (unitype) turtle.peng, 'd');
    list_append(turtle.penPos, (unitype) turtle.penb, 'd');
    list_append(turtle.penPos, (unitype) turtle.pena, 'd');
    list_append(turtle.penPos, (unitype) 64, 'h'); // blit circle signifier
    list_append(turtle.penPos, (unitype) turtle.circleprez, 'd');
}

/* create a triangle in 3D */
void turtle3DTriangle(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3) {
    list_append(turtle.penPos, (unitype) x1, 'd');
    list_append(turtle.penPos, (unitype) y1, 'd');
    list_append(turtle.penPos, (unitype) z1, 'd');
    list_append(turtle.penPos, (unitype) turtle.penr, 'd');
    list_append(turtle.penPos, (unitype) turtle.peng, 'd');
    list_append(turtle.penPos, (unitype) turtle.penb, 'd');
    list_append(turtle.penPos, (unitype) turtle.pena, 'd');
    list_append(turtle.penPos, (unitype) 130, 'h'); // blit 3D triangle signifier
    list_append(turtle.penPos, (unitype) x2, 'd');

    list_append(turtle.penPos, (unitype) y2, 'd');
    list_append(turtle.penPos, (unitype) z2, 'd');
    list_append(turtle.penPos, (unitype) x3, 'd');
    list_append(turtle.penPos, (unitype) y3, 'd');
    list_append(turtle.penPos, (unitype) z3, 'd');
    list_append(turtle.penPos, (unitype) 0, 'd');
    list_append(turtle.penPos, (unitype) 0, 'd');
    list_append(turtle.penPos, (unitype) 130, 'h'); // blit 3D triangle signifier
    list_append(turtle.penPos, (unitype) 0, 'd');
}

/* 3D -> 2D using perspective projection matrix */
void turtlePerspective(double x, double y, double z, double *xOut, double *yOut) {
    /* https://www.scratchapixel.com/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix/building-basic-perspective-projection-matrix.html */
    // double transform[16] = {
    //     1, 0, 0, 0,
    //     0, 1, 0, turtle.cameraDirectionLeftRight,
    //     0, 0, 1, turtle.cameraDirectionUpDown,
    //     0, 0, 0, 1,
    // };
    // x = x * transform[0 ] + y * transform[1 ] + z * transform[2 ] + transform[3 ];
    // y = x * transform[4 ] + y * transform[5 ] + z * transform[6 ] + transform[7 ];
    // z = x * transform[8 ] + y * transform[9 ] + z * transform[10] + transform[11];
    // double outWTest = x * transform[12] + y * transform[13] + z * transform[14] + transform[15];
    // x /= outWTest;
    // y /= outWTest;
    // z /= outWTest;
    // double scalingFactor = 1 / (tan((turtle.cameraFOV / 2) / 57.2958));
    // double near = 0.1;
    // double hypot = (x - turtle.cameraX) * (x - turtle.cameraX) + (y - turtle.cameraY) * (y - turtle.cameraY) + (z - turtle.cameraZ) * (z - turtle.cameraZ);
    // double far = 100;
    // // double far = hypot * sin((90 - turtle.cameraDirectionUpDown) / 57.2958) * sin((90 - turtle.cameraDirectionLeftRight) / 57.2958);
    // double threeThree = -far / (far - near);
    // double perspective[16] = {
    //     scalingFactor, 0,             0,                 0,
    //     0,             scalingFactor, 0,                 0,
    //     0,             0,             threeThree,       -1,
    //     0,             0,             threeThree * near, 0,
    // };
    // double outX = x * perspective[0 ] + y * perspective[1 ] + z * perspective[2 ] + perspective[3 ];
    // double outY = x * perspective[4 ] + y * perspective[5 ] + z * perspective[6 ] + perspective[7 ];
    // // double outZ = x * perspective[8 ] + y * perspective[9 ] + z * perspective[10] + perspective[11];
    // double outW = x * perspective[12] + y * perspective[13] + z * perspective[14] + perspective[15];
    // outX /= outW;
    // outY /= outW;
    // *xOut = outX * 640;
    // *yOut = outY * 360;
    // printf("%lf %lf\n", *xOut, *yOut);
}

/* draws the turtle's path on the screen, "this could all be a shader" */
void turtleUpdate() {
    /* bad fix to a niche problem part 1 */
    if (turtle.pen == 1) {
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
    /* used to have a feature that only redrew the screen if there have been any changes from last frame, but it has been removed.
       opted to redraw every frame and not list_copy, an alternative is hashing the penPos list. An interesting idea for sure... for another time */
    int8_t changed = 0;
    uint32_t len = turtle.penPos -> length;
    unitype *ren = turtle.penPos -> data;
    int8_t *renType = turtle.penPos -> type;
    uint64_t oldHash = turtle.penHash;
    turtle.penHash = 0; // I don't use this but it's an idea: https://stackoverflow.com/questions/57455444/very-low-collision-non-cryptographic-hashing-function
    for (uint32_t i = 0; i < len; i++) {
        turtle.penHash += (uint64_t) turtle.penPos -> data[i].p; // simple addition hash. I know not technically safe since i cast all sizes to 8 byte, but it should still work
    }
    if (len != turtle.lastLength || oldHash != turtle.penHash) {
        changed = 1;
        turtle.lastLength = len;
    }
    glfwGetWindowSize(turtle.window, &turtle.screenbounds[0], &turtle.screenbounds[1]);
    if (turtle.screenbounds[0] != turtle.lastscreenbounds[0] || turtle.screenbounds[1] != turtle.lastscreenbounds[1]) {
        changed = 1;
        turtle.lastscreenbounds[0] = turtle.screenbounds[0];
        turtle.lastscreenbounds[1] = turtle.screenbounds[1];
    }
    turtle.bounds[0] = turtle.centerAndScale[0] - turtle.centerAndScale[2] / turtle.screenbounds[0];
    turtle.bounds[2] = turtle.centerAndScale[0] + turtle.centerAndScale[2] / turtle.screenbounds[0];
    turtle.bounds[1] = turtle.centerAndScale[1] - turtle.centerAndScale[3] / turtle.screenbounds[1];
    turtle.bounds[3] = turtle.centerAndScale[1] + turtle.centerAndScale[3] / turtle.screenbounds[1];
    if (changed) {
        double xfact = 1.0 / ((turtle.bounds[2] - turtle.bounds[0]) / 2);
        double yfact = 1.0 / ((turtle.bounds[3] - turtle.bounds[1]) / 2);
        double xcenter = (double) turtle.screenbounds[0] / turtle.initscreenbounds[0] - 1;
        double ycenter = (double) turtle.screenbounds[1] / turtle.initscreenbounds[1] - 1;
        double lastSize = -1;
        double lastPrez = -1;
        double precomputedLog = 5;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for (int32_t i = 0; i < (int32_t) len; i += 9) {
            if (renType[i] == 'd') {
                switch (ren[i + 7].h) {
                case 0:
                    if (!(lastSize == ren[i + 2].d) || !(lastPrez != ren[i + 8].d)) {
                        precomputedLog = ren[i + 8].d * log(2.71 + ren[i + 2].d);
                    }
                    lastSize = ren[i + 2].d;
                    lastPrez = ren[i + 8].d;
                    turtleCircleRender(ren[i].d, ren[i + 1].d, ren[i + 2].d, ren[i + 3].d, ren[i + 4].d, ren[i + 5].d, ren[i + 6].d, xcenter, ycenter, xfact, yfact, precomputedLog);
                break;
                case 1:
                    turtleSquareRender(ren[i].d - ren[i + 2].d, ren[i + 1].d - ren[i + 2].d, ren[i].d + ren[i + 2].d, ren[i + 1].d + ren[i + 2].d, ren[i + 3].d, ren[i + 4].d, ren[i + 5].d, ren[i + 6].d, xcenter, ycenter, xfact, yfact);
                break;
                case 2:
                    turtleTriangleRender(ren[i].d - ren[i + 2].d, ren[i + 1].d - ren[i + 2].d, ren[i].d + ren[i + 2].d, ren[i + 1].d - ren[i + 2].d, ren[i].d, ren[i + 1].d + ren[i + 2].d, ren[i + 3].d, ren[i + 4].d, ren[i + 5].d, ren[i + 6].d, xcenter, ycenter, xfact, yfact);
                break;
                case 5:
                    if (i - 9 < 0 || renType[i - 9] == 'c') {
                        if (!(lastSize == ren[i + 2].d) || !(lastPrez != ren[i + 8].d)) {
                            precomputedLog = ren[i + 8].d * log(2.71 + ren[i + 2].d);
                        }
                        lastSize = ren[i + 2].d;
                        lastPrez = ren[i + 8].d;
                        turtleCircleRender(ren[i].d, ren[i + 1].d, ren[i + 2].d, ren[i + 3].d, ren[i + 4].d, ren[i + 5].d, ren[i + 6].d, xcenter, ycenter, xfact, yfact, precomputedLog);
                    }
                break;
                default:
                break;
                }
                if (i + 18 < (int32_t) len && renType[i + 9] == 'd' && ren[i + 7].h < 64 && (ren[i + 7].h == 4 || ren[i + 7].h == 5 || (fabs(ren[i].d - ren[i + 9].d) > ren[i + 2].d / 2 || fabs(ren[i + 1].d - ren[i + 10].d) > ren[i + 2].d / 2))) { // tests for next point continuity and also ensures that the next point is at sufficiently different coordinates
                    double dir = atan((ren[i + 9].d - ren[i].d) / (ren[i + 1].d - ren[i + 10].d));
                    double sinn = sin(dir + M_PI / 2);
                    double coss = cos(dir + M_PI / 2);
                    turtleQuadRender(ren[i].d + ren[i + 2].d * sinn, ren[i + 1].d - ren[i + 2].d * coss, ren[i + 9].d + ren[i + 2].d * sinn, ren[i + 10].d - ren[i + 2].d * coss, ren[i + 9].d - ren[i + 2].d * sinn, ren[i + 10].d + ren[i + 2].d * coss, ren[i].d - ren[i + 2].d * sinn, ren[i + 1].d + ren[i + 2].d * coss, ren[i + 3].d, ren[i + 4].d, ren[i + 5].d, ren[i + 6].d, xcenter, ycenter, xfact, yfact);
                    if ((ren[i + 7].h == 4 || ren[i + 7].h == 5) && i + 18 < (int32_t) len && renType[i + 18] == 'd') {
                        double dir2 = atan((ren[i + 18].d - ren[i + 9].d) / (ren[i + 10].d - ren[i + 19].d));
                        double sinn2 = sin(dir2 + M_PI / 2);
                        double coss2 = cos(dir2 + M_PI / 2);
                        turtleTriangleRender(ren[i + 9].d + ren[i + 2].d * sinn, ren[i + 10].d - ren[i + 2].d * coss, ren[i + 9].d - ren[i + 2].d * sinn, ren[i + 10].d + ren[i + 2].d * coss, ren[i + 9].d + ren[i + 11].d * sinn2, ren[i + 10].d - ren[i + 11].d * coss2, ren[i + 3].d, ren[i + 4].d, ren[i + 5].d, ren[i + 6].d, xcenter, ycenter, xfact, yfact); // in a perfect world the program would know which one of these triangles to render (to blend the segments)
                        turtleTriangleRender(ren[i + 9].d + ren[i + 2].d * sinn, ren[i + 10].d - ren[i + 2].d * coss, ren[i + 9].d - ren[i + 2].d * sinn, ren[i + 10].d + ren[i + 2].d * coss, ren[i + 9].d - ren[i + 11].d * sinn2, ren[i + 10].d + ren[i + 11].d * coss2, ren[i + 3].d, ren[i + 4].d, ren[i + 5].d, ren[i + 6].d, xcenter, ycenter, xfact, yfact); // however we live in a world where i am bad at math, so it just renders both no matter what (one has no effect)
                    }
                } else {
                    if (ren[i + 7].h == 4 && i > 8 && renType[i - 8] == 'c') {
                        if (!(lastSize == ren[i + 2].d) || !(lastPrez != ren[i + 8].d)) {
                            precomputedLog = ren[i + 8].d * log(2.71 + ren[i + 2].d);
                        }
                        lastSize = ren[i + 2].d;
                        lastPrez = ren[i + 8].d;
                        turtleCircleRender(ren[i].d, ren[i + 1].d, ren[i + 2].d, ren[i + 3].d, ren[i + 4].d, ren[i + 5].d, ren[i + 6].d, xcenter, ycenter, xfact, yfact, precomputedLog);
                    }
                    if (ren[i + 7].h == 5 && i > 8) {
                        if (!(lastSize == ren[i + 2].d) || !(lastPrez != ren[i + 8].d)) {
                            precomputedLog = ren[i + 8].d * log(2.71 + ren[i + 2].d);
                        }
                        lastSize = ren[i + 2].d;
                        lastPrez = ren[i + 8].d;
                        turtleCircleRender(ren[i].d, ren[i + 1].d, ren[i + 2].d, ren[i + 3].d, ren[i + 4].d, ren[i + 5].d, ren[i + 6].d, xcenter, ycenter, xfact, yfact, precomputedLog);
                    }
                }
                if (ren[i + 7].h == 64) { // blit circle
                    if (!(lastSize == ren[i + 2].d) || !(lastPrez != ren[i + 8].d)) {
                        precomputedLog = ren[i + 8].d * log(2.71 + ren[i + 2].d);
                    }
                    lastSize = ren[i + 2].d;
                    lastPrez = ren[i + 8].d;
                    turtleCircleRender(ren[i].d, ren[i + 1].d, ren[i + 2].d, ren[i + 3].d, ren[i + 4].d, ren[i + 5].d, ren[i + 6].d, xcenter, ycenter, xfact, yfact, precomputedLog);
                }
                if (ren[i + 7].h == 66) { // blit triangle
                    turtleTriangleRender(ren[i].d, ren[i + 1].d, ren[i + 2].d, ren[i + 8].d, ren[i + 9].d, ren[i + 10].d, ren[i + 3].d, ren[i + 4].d, ren[i + 5].d, ren[i + 6].d, xcenter, ycenter, xfact, yfact);
                    i += 9;
                }
                if (ren[i + 7].h == 67) { // blit quad
                    turtleQuadRender(ren[i].d, ren[i + 1].d, ren[i + 2].d, ren[i + 8].d, ren[i + 9].d, ren[i + 10].d, ren[i + 11].d, ren[i + 17].d, ren[i + 3].d, ren[i + 4].d, ren[i + 5].d, ren[i + 6].d, xcenter, ycenter, xfact, yfact);
                    i += 9;
                }
                if (ren[i + 7].h == 128) { // blit 3D sphere

                }
                if (ren[i + 7].h == 129) { // blit 3D circle

                }
                if (ren[i + 7].h == 130) { // blit 3D triangle
                    turtlePerspective(ren[i].d, ren[i + 1].d, ren[i + 2].d, &ren[i].d, &ren[i + 1].d);
                    turtlePerspective(ren[i + 8].d, ren[i + 9].d, ren[i + 10].d, &ren[i + 8].d, &ren[i + 9].d);
                    turtlePerspective(ren[i + 11].d, ren[i + 12].d, ren[i + 13].d, &ren[i + 11].d, &ren[i + 12].d);
                    turtleTriangleRender(ren[i].d, ren[i + 1].d, ren[i + 8].d, ren[i + 9].d, ren[i + 11].d, ren[i + 12].d, ren[i + 3].d, ren[i + 4].d, ren[i + 5].d, ren[i + 6].d, xcenter, ycenter, xfact, yfact);
                    i += 9;
                }
                if (ren[i + 7].h == 131) { // blit 3D quad

                }
            }
        }
        glfwSwapBuffers(turtle.window);
    }
    /* bad fix to niche problem part 2 */
    if (turtle.pen == 1) {
        for (int8_t i = 0; i < 9; i++) {
            list_pop(turtle.penPos);
        }
    }
    glfwPollEvents();
    if (glfwWindowShouldClose(turtle.window)) {
        turtle.close = 1;
        if (turtle.popupClose) {
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

#endif /* TURTLE_INTERNAL_IMPLEMENTATION */
#ifdef TURTLE_TEXT_IMPLEMENTATION
/* 
████████╗██╗   ██╗██████╗ ████████╗██╗     ███████╗████████╗███████╗██╗  ██╗████████╗ ██████╗
╚══██╔══╝██║   ██║██╔══██╗╚══██╔══╝██║     ██╔════╝╚══██╔══╝██╔════╝╚██╗██╔╝╚══██╔══╝██╔════╝
   ██║   ██║   ██║██████╔╝   ██║   ██║     █████╗     ██║   █████╗   ╚███╔╝    ██║   ██║     
   ██║   ██║   ██║██╔══██╗   ██║   ██║     ██╔══╝     ██║   ██╔══╝   ██╔██╗    ██║   ██║     
   ██║   ╚██████╔╝██║  ██║   ██║   ███████╗███████╗   ██║   ███████╗██╔╝ ██╗   ██║██╗╚██████╗
   ╚═╝    ╚═════╝ ╚═╝  ╚═╝   ╚═╝   ╚══════╝╚══════╝   ╚═╝   ╚══════╝╚═╝  ╚═╝   ╚═╝╚═╝ ╚═════╝
https://patorjk.com/software/taag/#p=display&f=ANSI%20Shadow

turtleText uses openGL and the turtle library to render text on the screen
*/


turtleText_t turtleText;

/* initialise values, must supply a font file (tgl) */
int32_t turtleTextInit(const char *filename) {
    turtlePenColor(0, 0, 0);
    turtleText.bezierPrez = 10;

    /* load file */
    char fileExists = 1;
    list_t *generatedFont = list_init();
    FILE *tgl = fopen(filename, "r");
    if (tgl == NULL) {
        printf("Error: could not open file %s, using default\n", filename);
        generateDefaultFont(generatedFont);
        fileExists = 0;
    }

    list_t *fontDataInit = list_init(); // these start as lists and become int arrays
    list_t *fontPointerInit = list_init();
    list_t *supportedCharReferenceInit = list_init();
    uint8_t parsedInt[12]; // max possible length of an int as a string is 11
    uint32_t fontPar;
    int32_t oldptr;
    int32_t strptr;

    char line[2048]; // maximum line length
    line[2047] = 0; // canary value
    turtleText.charCount = 0;
    while ((fileExists == 0 && turtleText.charCount < generatedFont -> length) || (fileExists == 1 && fgets(line, 2048, tgl) != NULL)) { // fgets to prevent overflows
        if (fileExists == 0) {
            memcpy(line, generatedFont -> data[turtleText.charCount].s, strlen(generatedFont -> data[turtleText.charCount].s) + 1);
        }
        if (line[2047] != 0) {
            printf("Error: line %d in file %s exceeded appropriate length\n", 0, filename);
            return -1;
        }

        oldptr = 0;
        strptr = 0;
        int32_t loops = 0;
        int32_t firstIndex = 0;
        int8_t widthProceed = 0;
        int32_t savedWidth = -2147483640;
        
        while (line[strptr] != '\n' && line[strptr] != '\0') {
            while (line[strptr] != ',' && line[strptr] != '\n' && line[strptr] != '\0') {
                parsedInt[strptr - oldptr] = line[strptr];
                strptr += 1;
            }
            parsedInt[strptr - oldptr] = '\0';
            if (oldptr == 0) {
                fontPar = 0; // parse as unicode char (basically just take the multibyte utf-8 encoding of the character and literal cast it to an uint32_t (maximum of 4 bytes per character in utf-8 (?)))
                if (strlen((char *) parsedInt) > 4) {
                    printf("Error: character at line %d too long for uint32_t\n", supportedCharReferenceInit -> length + 1);
                }
                for (int32_t i = strlen((char *) parsedInt); i > 0; i--) {
                    int32_t abri = (strlen((char *) parsedInt) - i);
                    fontPar += (uint32_t) parsedInt[abri] << ((i - 1) * 8);
                }
                if (fontPar == 0) { // exception for the comma character
                    fontPar = 44;
                    strptr += 1;
                }
                list_append(supportedCharReferenceInit, (unitype) fontPar, 'i'); // adds as an int but will typecast back to unsigned later, this might not work correctly but it also doesn't really matter
                list_append(fontPointerInit, (unitype) fontDataInit -> length, 'i');
                firstIndex = fontDataInit -> length;
                list_append(fontDataInit, (unitype) 1, 'i');
            } else {
                sscanf((char *) parsedInt, "%u", &fontPar); // parse as integer
                if (strcmp((char *) parsedInt, "w") == 0) { // w is a width specifier
                    widthProceed = 1;
                    loops -= 1; // counter loops, this field does not count
                } else if (widthProceed) {
                    savedWidth = fontPar;
                    widthProceed = 0;
                    loops -= 1; // counter loops, this field does not count
                } else if (strcmp((char *) parsedInt, "b") == 0) {
                    list_append(fontDataInit, (unitype) 140894115, 'i'); // all b's get converted to the integer 140894115 (chosen semi-randomly)
                } else {
                    list_append(fontDataInit, (unitype) fontPar, 'i'); // fontPar will double count when it encounters a b (idk why) so if there's a b we ignore the second fontPar (which is a duplicate of the previous one)
                }
                loops += 1;
            }
            if (line[strptr] != '\n' && line[strptr] != '\0') {
                strptr += 2;
            }
            oldptr = strptr;
        }
        fontDataInit -> data[firstIndex] = (unitype) loops;
        firstIndex += 1; // using firstIndex as iteration variable
        int32_t len1 = fontDataInit -> data[firstIndex].i;
        int32_t maximums[4] = {-2147483648, -2147483648, 2147483647, 2147483647}; // for describing bounding box of a character (positive X, positive Y, negative X, negative Y)
        
        /* good programmng alert */
        #define CHECKS_EMB(ind) \
            if (fontDataInit -> data[ind].i > maximums[0]) {\
                maximums[0] = fontDataInit -> data[ind].i;\
            }\
            if (fontDataInit -> data[ind].i < maximums[3]) {\
                maximums[2] = fontDataInit -> data[ind].i;\
            }\
            if (fontDataInit -> data[ind + 1].i > maximums[1]) {\
                maximums[1] = fontDataInit -> data[ind + 1].i;\
            }\
            if (fontDataInit -> data[ind + 1].i < maximums[3]) {\
                maximums[3] = fontDataInit -> data[ind + 1].i;\
            }
        for (int32_t i = 0; i < len1; i++) {
            firstIndex += 1;
            int32_t len2 = fontDataInit -> data[firstIndex].i;
            for (int32_t j = 0; j < len2; j++) {
                firstIndex += 1;
                if (fontDataInit -> data[firstIndex].i == 140894115) {
                    firstIndex += 1;
                    fontDataInit -> data[firstIndex] = (unitype) (fontDataInit -> data[firstIndex].i + 160);
                    fontDataInit -> data[firstIndex + 1] = (unitype) (fontDataInit -> data[firstIndex + 1].i + 100);
                    CHECKS_EMB(firstIndex);
                    firstIndex += 2;
                    fontDataInit -> data[firstIndex] = (unitype) (fontDataInit -> data[firstIndex].i + 160);
                    fontDataInit -> data[firstIndex + 1] = (unitype) (fontDataInit -> data[firstIndex + 1].i + 100);
                    CHECKS_EMB(firstIndex);
                    firstIndex += 1;
                    if (fontDataInit -> data[firstIndex + 1].i != 140894115) {
                        firstIndex += 1;
                        fontDataInit -> data[firstIndex] = (unitype) (fontDataInit -> data[firstIndex].i + 160);
                        fontDataInit -> data[firstIndex + 1] = (unitype) (fontDataInit -> data[firstIndex + 1].i + 100);
                        CHECKS_EMB(firstIndex);
                        firstIndex += 1;
                    }
                } else {
                    fontDataInit -> data[firstIndex] = (unitype) (fontDataInit -> data[firstIndex].i + 160);
                    fontDataInit -> data[firstIndex + 1] = (unitype) (fontDataInit -> data[firstIndex + 1].i + 100);
                    CHECKS_EMB(firstIndex);
                    firstIndex += 1;
                }
            }
        }
        /* maxX */
        if (savedWidth != -2147483640) {
            list_append(fontDataInit, (unitype) savedWidth, 'i');
        } else {
            if (maximums[0] < 0) {
                list_append(fontDataInit, (unitype) 90, 'i'); // default width of character
            } else {
                list_append(fontDataInit, (unitype) maximums[0], 'i');
            }
        }
        /* maxY */
        if (maximums[1] < 0) {
            if (turtleText.charCount == 0) {
                list_append(fontDataInit, (unitype) 0, 'i');
            } else {
                list_append(fontDataInit, (unitype) 120, 'i');
            }
        } else {
            list_append(fontDataInit, (unitype) maximums[1], 'i');
        }
        /* minX */
        if (maximums[2] > 0) {
            list_append(fontDataInit, (unitype) 0, 'i');
        } else {
            list_append(fontDataInit, (unitype) maximums[2], 'i');
        }
        /* minY */
        if (maximums[3] > 0) {
            list_append(fontDataInit, (unitype) 0, 'i');
        } else {
            list_append(fontDataInit, (unitype) maximums[3], 'i');
        }
        turtleText.charCount += 1;
        // printf("maxX: %d, maxY: %d, minX: %d, minY: %d\n", fontDataInit -> data[fontDataInit -> length - 4].i, fontDataInit -> data[fontDataInit -> length - 3].i, fontDataInit -> data[fontDataInit -> length - 2].i, fontDataInit -> data[fontDataInit -> length - 1].i);
    }
    list_append(fontPointerInit, (unitype) (int32_t) (fontDataInit -> length), 'i'); // last pointer
    // list_print(fontDataInit);
    turtleText.fontData = malloc(sizeof(int32_t) * fontDataInit -> length); // convert lists to arrays (could be optimised cuz we already have the -> data arrays but who really cares this runs once)
    for (uint32_t i = 0; i < fontDataInit -> length; i++) {
        turtleText.fontData[i] = fontDataInit -> data[i].i;
    }
    turtleText.fontPointer = malloc(sizeof(int32_t) * fontPointerInit -> length);
    for (uint32_t i = 0; i < fontPointerInit -> length; i++) {
        turtleText.fontPointer[i] = fontPointerInit -> data[i].i;
    }
    turtleText.supportedCharReference = malloc(sizeof(int32_t) * supportedCharReferenceInit -> length);
    for (uint32_t i = 0; i < supportedCharReferenceInit -> length; i++) {
        turtleText.supportedCharReference[i] = supportedCharReferenceInit -> data[i].i;
    }

    if (fileExists) {
        printf("%d characters loaded from %s\n", turtleText.charCount, filename);
    } else {
        printf("%d characters loaded from default font\n", turtleText.charCount);
    }

    list_free(fontDataInit);
    list_free(fontPointerInit);
    list_free(supportedCharReferenceInit);
    list_free(generatedFont);
    return 0;
}

/* render functions */

/* renders a quadratic bezier curve on the screen */
void renderBezier(double x1, double y1, double x2, double y2, double x3, double y3, int32_t prez) {
    turtleGoto(x1, y1);
    turtlePenDown();
    double iter1 = 1;
    double iter2 = 0;
    for (int32_t i = 0; i < prez; i++) {
        iter1 -= (double) 1 / prez;
        iter2 += (double) 1 / prez;
        double t1 = iter1 * iter1;
        double t2 = iter2 * iter2;
        double t3 = 2 * iter1 * iter2;
        turtleGoto(t1 * x1 + t3 * x2 + t2 * x3, t1 * y1 + t3 * y2 + t2 * y3);
    }
    turtleGoto(x3, y3);
}

/* renders a single character */
void renderChar(int32_t index, double x, double y, double size) {
    index += 1;
    int32_t len1 = turtleText.fontData[index];
    for (int32_t i = 0; i < len1; i++) {
        index += 1;
        if (turtle.pen == 1) {
            turtlePenUp();
        }
        int32_t len2 = turtleText.fontData[index];
        for (int32_t j = 0; j < len2; j++) {
            index += 1;
            if (turtleText.fontData[index] == 140894115) { // 140894115 is the b value (reserved)
                index += 4;
                if (turtleText.fontData[index + 1] != 140894115) {
                    renderBezier(x + turtleText.fontData[index - 3] * size, y + turtleText.fontData[index - 2] * size, x + turtleText.fontData[index - 1] * size, y + turtleText.fontData[index] * size, x + turtleText.fontData[index + 1] * size, y + turtleText.fontData[index + 2] * size, turtleText.bezierPrezCurrent);
                    index += 2;
                } else {
                    renderBezier(x + turtleText.fontData[index - 3] * size, y + turtleText.fontData[index - 2] * size, x + turtleText.fontData[index - 1] * size, y + turtleText.fontData[index] * size, x + turtleText.fontData[index + 2] * size, y + turtleText.fontData[index + 3] * size, turtleText.bezierPrezCurrent);
                }
            } else {
                index += 1;
                turtleGoto(x + turtleText.fontData[index - 1] * size, y + turtleText.fontData[index] * size);
            }
            turtlePenDown();
        }
    }
    turtlePenUp();
}

/* gets the length of a string in pixels on the screen */
double turtleTextGetLength(const uint32_t *text, int32_t textLength, double size) {
    if (textLength == 0) {
        return 0;
    }
    size /= 175;
    double xTrack = 0;
    for (int32_t i = 0; i < textLength; i++) {
        int32_t currentDataAddress = 0;
        for (int32_t j = 0; j < turtleText.charCount; j++) { // change to hashmap later
            if (turtleText.supportedCharReference[j] == text[i]) {
                currentDataAddress = j;
                break;
            }
        }
        xTrack += (turtleText.fontData[turtleText.fontPointer[currentDataAddress + 1] - 4] + 40) * size;
    }
    xTrack -= 40 * size;
    return xTrack;
}

/* gets the length of a string in pixels on the screen */
double turtleTextGetStringLength(const char *str, double size) {
    uint32_t len = strlen(str);
    uint32_t converted[len];
    for (uint32_t i = 0; i < len; i++) {
        converted[i] = (uint32_t) str[i];
    }
    return turtleTextGetLength(converted, len, size);
}

/* gets the length of a string in pixels on the screen */
double turtleTextGetStringLengthf(double size, const char *str, ...) {
    char buffer[1024];
    va_list args;
    va_start(args, str);
    vsnprintf(buffer, 1024, str, args);
    double out = turtleTextGetStringLength(buffer, size);
    va_end(args);
    return out;
}

/* gets the length of a u-string in pixels on the screen */
double turtleTextGetUnicodeLength(const unsigned char *str, double size) {
    int32_t len = strlen((char *) str);
    uint32_t converted[len];
    int32_t byteLength;
    int32_t i = 0;
    int32_t next = 0;
    while (i < len) {
        byteLength = 0;
        for (uint8_t j = 0; j < 8; j++) {
            unsigned char mask = 128 >> j;
            if (str[i] & mask) {
                byteLength += 1;
            } else {
                j = 8; // end loop
            }
        }
        if (byteLength == 0) { // case: ASCII character
            converted[next] = (uint32_t) str[i];
            byteLength = 1;
        } else { // case: multi-byte character
            uint32_t convert = 0;
            for (int32_t k = 0; k < byteLength; k++) {
                convert = convert << 8;
                convert += (uint32_t) str[i + k];
            }
            converted[next] = convert;
        }
        i += byteLength;
        next += 1;
    }
    return turtleTextGetLength(converted, next, size);
}

/* writes to the screen */
void turtleTextWrite(const uint32_t *text, int32_t textLength, double x, double y, double size, double align) {
    char saveShape = turtle.penshape;
    double saveSize = turtle.pensize;
    turtleText.bezierPrezCurrent = (int32_t) ceil(sqrt(size * turtleText.bezierPrez / 10));
    double xTrack = x;
    #ifdef TURTLE_TEXT_DO_DYNAMIC_Y_CENTERING
    double maxY = 0;
    double minY = 0;
    #endif
    size /= 175;
    turtlePenSize(20 * size);
    #if defined(TURTLE_TEXT_FAST_PEN) && !defined(TURTLE_TEXT_PRETTY_PEN)
    turtlePenShape("connected"); // fast
    #else
    #if !defined(TURTLE_TEXT_FAST_PEN) && defined(TURTLE_TEXT_PRETTY_PEN)
    turtlePenShape("circle"); // pretty
    #else
    turtlePenShape("text"); // dedicated setting that blends circle and connected
    #endif
    #endif
    list_t *xvals = list_init();
    list_t *dataIndStored = list_init();
    for (int32_t i = 0; i < textLength; i++) {
        int32_t currentDataAddress = 0;
        for (int32_t j = 0; j < turtleText.charCount; j++) { // change to hashmap later
            if (turtleText.supportedCharReference[j] == text[i]) {
                currentDataAddress = j;
                break;
            }
        }
        list_append(xvals, (unitype) xTrack, 'd');
        list_append(dataIndStored, (unitype) currentDataAddress, 'i');
        xTrack += (turtleText.fontData[turtleText.fontPointer[currentDataAddress + 1] - 4] + 40) * size;
        #ifdef TURTLE_TEXT_DO_DYNAMIC_Y_CENTERING
        if (maxY < turtleText.fontData[turtleText.fontPointer[currentDataAddress + 1] - 3]) {
            maxY = turtleText.fontData[turtleText.fontPointer[currentDataAddress + 1] - 3];
        }
        if (minY > turtleText.fontData[turtleText.fontPointer[currentDataAddress + 1] - 1]) {
            minY = turtleText.fontData[turtleText.fontPointer[currentDataAddress + 1] - 1];
        }
        #endif
    }
    xTrack -= 40 * size;
    #ifdef TURTLE_TEXT_DO_DYNAMIC_Y_CENTERING
    y -= (maxY + minY) / 2 * size;
    #else
    y -= 80 * size;
    #endif
    for (int32_t i = 0; i < textLength; i++) {
        renderChar(turtleText.fontPointer[dataIndStored -> data[i].i], xvals -> data[i].d - ((xTrack - x) * (align / 100)), y, size);
    }
    list_free(dataIndStored);
    list_free(xvals);
    turtle.penshape = saveShape; // restore the shape and size before the write
    turtle.pensize = saveSize;
}

/* wrapper function for writing strings easier */
void turtleTextWriteString(const char *str, double x, double y, double size, double align) {
    int32_t len = strlen(str);
    uint32_t converted[len];
    for (int32_t i = 0; i < len; i++) {
        converted[i] = (uint32_t) str[i];
    }
    turtleTextWrite(converted, len, x, y, size, align);
}

/* formatted string */
void turtleTextWriteStringf(double x, double y, double size, double align, const char *str, ...) {
    char buffer[1024];
    va_list args;
    va_start(args, str);
    vsnprintf(buffer, 1024, str, args);
    turtleTextWriteString(buffer, x, y, size, align);
    va_end(args);
}

/* wrapper function for unicode strings (UTF-8, u8"Hello World") */
void turtleTextWriteUnicode(const unsigned char *str, double x, double y, double size, double align) {
    int32_t len = strlen((char *) str);
    uint32_t converted[len];
    int32_t byteLength;
    int32_t i = 0;
    int32_t next = 0;
    while (i < len) {
        byteLength = 0;
        for (uint8_t j = 0; j < 8; j++) {
            uint8_t mask = 128 >> j;
            if (str[i] & mask) {
                byteLength += 1;
            } else {
                j = 8; // end loop
            }
        }
        if (byteLength == 0) { // case: ASCII character
            converted[next] = (uint32_t) str[i];
            byteLength = 1;
        } else { // case: multi-byte character
            uint32_t convert = 0;
            for (int32_t k = 0; k < byteLength; k++) {
                convert = convert << 8;
                convert += (uint32_t) str[i + k];
            }
            converted[next] = convert;
        }
        i += byteLength;
        next += 1;
    }
    turtleTextWrite(converted, next, x, y, size, align);
}

/* formatted utf-8 string */
void turtleTextWriteUnicodef(double x, double y, double size, double align, const unsigned char *str, ...) {
    char buffer[1024];
    va_list args;
    va_start(args, str);
    vsnprintf(buffer, 1024, str, args);
    turtleTextWriteUnicode(buffer, x, y, size, align);
    va_end(args);
}

int32_t turtleTextConvertUnicode(const unsigned char *str, uint32_t *converted) {
    int32_t len = strlen((char *) str);
    int32_t byteLength;
    int32_t i = 0;
    int32_t next = 0;
    while (i < len) {
        byteLength = 0;
        for (uint8_t j = 0; j < 8; j++) {
            uint8_t mask = 128 >> j;
            if (str[i] & mask) {
                byteLength += 1;
            } else {
                j = 8; // end loop
            }
        }
        if (byteLength == 0) { // case: ASCII character
            converted[next] = (uint32_t) str[i];
            byteLength = 1;
        } else { // case: multi-byte character
            uint32_t convert = 0;
            for (int32_t k = 0; k < byteLength; k++) {
                convert = convert << 8;
                convert += (uint32_t) str[i + k];
            }
            converted[next] = convert;
        }
        i += byteLength;
        next += 1;
    }
    return next;
}

/* if the font file is not found, use the default font (kept here) */
void generateDefaultFont(list_t *generatedFont) {
    list_append(generatedFont, (unitype) " , 0", 's');
    list_append(generatedFont, (unitype) "A, 2, 3, -160, -100, -100, 60, -40, -100, 2, -137, -40, -63, -40", 's');
    list_append(generatedFont, (unitype) "À, 3, 3, -160, -100, -100, 60, -40, -100, 2, -137, -40, -63, -40, 2, -100, 85, -120, 105", 's');
    list_append(generatedFont, (unitype) "Á, 3, 3, -160, -100, -100, 60, -40, -100, 2, -137, -40, -63, -40, 2, -100, 85, -80, 105", 's');
    list_append(generatedFont, (unitype) "Â, 3, 3, -160, -100, -100, 60, -40, -100, 2, -137, -40, -63, -40, 3, -130, 85, -100, 105, -70, 85", 's');
    list_append(generatedFont, (unitype) "Ă, 3, 3, -160, -100, -100, 60, -40, -100, 2, -137, -40, -63, -40, 2, b, -130, 105, -130, 85, b, -100, 85, -70, 85, -70, 105", 's');
    list_append(generatedFont, (unitype) "Ä, 4, 3, -160, -100, -100, 60, -40, -100, 2, -137, -40, -63, -40, 1, -120, 95, 1, -80, 95", 's');
    list_append(generatedFont, (unitype) "Ã, 3, 3, -160, -100, -100, 60, -40, -100, 2, -137, -40, -63, -40, 4, b, -135, 85, -135, 105, b, -120, 105, -110, 105, b, -100, 95, -90, 85, b, -80, 85, -65, 85, -65, 105", 's');
    list_append(generatedFont, (unitype) "Å, 3, 3, -160, -100, -100, 60, -40, -100, 2, -137, -40, -63, -40, 4, b, -100, 85, -80, 85, b, -80, 105, -80, 125, b, -100, 125, -120, 125, b, -120, 105, -120, 85, -100, 85", 's');
    list_append(generatedFont, (unitype) "Ā, 3, 3, -160, -100, -100, 60, -40, -100, 2, -137, -40, -63, -40, 2, -145, 95, -55, 95", 's');
    list_append(generatedFont, (unitype) "Ą, 2, 2, -137, -40, -63, -40, 4, -160, -100, -100, 60, b, -40, -100, -65, -105, b, -65, -125, -65, -150, -40, -145", 's');
    list_append(generatedFont, (unitype) "Æ, 4, 2, -118, -40, -50, -40, 4, -160, -100, -50, 60, -50, -100, 50, -100, 2, -50, -20, 40, -20, 2, -50, 60, 50, 60", 's');
    list_append(generatedFont, (unitype) "B, 2, 5, -160, -100, -160, 60, b, -110, 60, -60, 60, b, -60, 20, -60, -20, -110, -20, -160, -20, 3, b, -110, -20, -60, -20, b, -60, -60, -60, -100, -110, -100, -160, -100", 's');
    list_append(generatedFont, (unitype) "C, 1, 4, b, -50, 20, -60, 60, b, -100, 60, -160, 60, b, -160, -20, -160, -100, b, -100, -100, -60, -100, -50, -60", 's');
    list_append(generatedFont, (unitype) "Ć, 2, 4, b, -50, 20, -60, 60, b, -100, 60, -160, 60, b, -160, -20, -160, -100, b, -100, -100, -60, -100, -50, -60, 2, -100, 85, -80, 105", 's');
    list_append(generatedFont, (unitype) "Č, 2, 4, b, -50, 20, -60, 60, b, -100, 60, -160, 60, b, -160, -20, -160, -100, b, -100, -100, -60, -100, -50, -60, 3, -130, 105, -100, 85, -70, 105", 's');
    list_append(generatedFont, (unitype) "Ċ, 2, 4, b, -50, 20, -60, 60, b, -100, 60, -160, 60, b, -160, -20, -160, -100, b, -100, -100, -60, -100, -50, -60, 1, -100, 95", 's');
    list_append(generatedFont, (unitype) "Ç, 2, 4, b, -50, 20, -60, 60, b, -100, 60, -160, 60, b, -160, -20, -160, -100, b, -100, -100, -60, -100, -50, -60, 3, -100, -100, b, -103, -115, -80, -120, b, -80, -135, -80, -160, -105, -155", 's');
    list_append(generatedFont, (unitype) "D, 1, 5, -160, -100, -160, 60, b, -110, 60, -50, 60, b, -50, -20, -50, -100, -110, -100, -160, -100", 's');
    list_append(generatedFont, (unitype) "Ď, 2, 5, -160, -100, -160, 60, b, -110, 60, -50, 60, b, -50, -20, -50, -100, -110, -100, -160, -100, 3, -145, 105, -115, 85, -85, 105", 's');
    list_append(generatedFont, (unitype) "Đ, 2, 5, -160, -100, -160, 60, b, -110, 60, -50, 60, b, -50, -20, -50, -100, -110, -100, -160, -100, 2, -180, -20, -130, -20", 's');
    list_append(generatedFont, (unitype) "Ð, 2, 5, -160, -100, -160, 60, b, -110, 60, -50, 60, b, -50, -20, -50, -100, -110, -100, -160, -100, 2, -180, -20, -130, -20", 's');
    list_append(generatedFont, (unitype) "E, 2, 4, -70, -100, -160, -100, -160, 60, -70, 60, 2, -160, -20, -80, -20", 's');
    list_append(generatedFont, (unitype) "È, 3, 4, -70, -100, -160, -100, -160, 60, -70, 60, 2, -160, -20, -80, -20, 2, -115, 85, -135, 105", 's');
    list_append(generatedFont, (unitype) "É, 3, 4, -70, -100, -160, -100, -160, 60, -70, 60, 2, -160, -20, -80, -20, 2, -115, 85, -95, 105", 's');
    list_append(generatedFont, (unitype) "Ě, 3, 4, -70, -100, -160, -100, -160, 60, -70, 60, 2, -160, -20, -80, -20, 3, -145, 105, -115, 85, -85, 105", 's');
    list_append(generatedFont, (unitype) "Ê, 3, 4, -70, -100, -160, -100, -160, 60, -70, 60, 2, -160, -20, -80, -20, 3, -145, 85, -115, 105, -85, 85", 's');
    list_append(generatedFont, (unitype) "Ë, 4, 4, -70, -100, -160, -100, -160, 60, -70, 60, 2, -160, -20, -80, -20, 1, -135, 95, 1, -95, 95", 's');
    list_append(generatedFont, (unitype) "Ē, 3, 4, -70, -100, -160, -100, -160, 60, -70, 60, 2, -160, -20, -80, -20, 2, -145, 95, -85, 95", 's');
    list_append(generatedFont, (unitype) "Ė, 3, 4, -70, -100, -160, -100, -160, 60, -70, 60, 2, -160, -20, -80, -20, 1, -115, 95", 's');
    list_append(generatedFont, (unitype) "Ę, 2, 2, -160, -20, -80, -20, 5, -70, 60, -160, 60, -160, -100, b, -70, -100, -95, -105, b, -95, -125, -95, -150, -70, -145", 's');
    list_append(generatedFont, (unitype) "Ə, 1, 5, -30, -20, b, -160, -20, -160, -100, b, -95, -100, -30, -100, b, -30, -20, -30, 60, b, -95, 60, -125, 60, -140, 45", 's');
    list_append(generatedFont, (unitype) "F, 2, 3, -160, -100, -160, 60, -70, 60, 2, -160, -20, -80, -20", 's');
    list_append(generatedFont, (unitype) "G, 1, 6, b, -50, 20, -60, 60, b, -100, 60, -160, 60, b, -160, -20, -160, -100, b, -100, -100, -60, -100, -50, -60, -50, -20, -100, -20", 's');
    list_append(generatedFont, (unitype) "Ğ, 2, 6, b, -50, 20, -60, 60, b, -100, 60, -160, 60, b, -160, -20, -160, -100, b, -100, -100, -60, -100, -50, -60, -50, -20, -100, -20, 2, b, -135, 105, -135, 85, b, -105, 85, -75, 85, -75, 105", 's');
    list_append(generatedFont, (unitype) "Ġ, 2, 6, b, -50, 20, -60, 60, b, -100, 60, -160, 60, b, -160, -20, -160, -100, b, -100, -100, -60, -100, -50, -60, -50, -20, -100, -20, 1, -105, 95", 's');
    list_append(generatedFont, (unitype) "H, 3, 2, -160, -100, -160, 60, 2, -50, -100, -50, 60, 2, -160, -20, -50, -20", 's');
    list_append(generatedFont, (unitype) "Ħ, 4, 2, -160, -100, -160, 60, 2, -50, -100, -50, 60, 2, -160, -20, -50, -20, 2, -170, 20, -40, 20", 's');
    list_append(generatedFont, (unitype) "I, 1, 2, -160, -100, -160, 60", 's');
    list_append(generatedFont, (unitype) "Ì, 2, 2, -160, -100, -160, 60, 2, -160, 90, -170, 110", 's');
    list_append(generatedFont, (unitype) "Í, 2, 2, -160, -100, -160, 60, 2, -160, 90, -150, 110", 's');
    list_append(generatedFont, (unitype) "Î, 2, 2, -160, -100, -160, 60, 3, -180, 90, -160, 110, -140, 90", 's');
    list_append(generatedFont, (unitype) "Ï, 3, 2, -160, -100, -160, 60, 1, -180, 100, 1, -140, 100", 's');
    list_append(generatedFont, (unitype) "Ī, 2, 2, -160, -100, -160, 60, 2, -180, 100, -140, 100", 's');
    list_append(generatedFont, (unitype) "İ, 2, 2, -160, -100, -160, 60, 1, -160, 100", 's');
    list_append(generatedFont, (unitype) "Į, 1, 3, -160, 60, b, -160, -100, -170, -105, b, -170, -115, -170, -125, -160, -125", 's');
    list_append(generatedFont, (unitype) "J, 1, 3, -70, 60, b, -70, -50, -70, -100, b, -120, -100, -150, -100, -160, -70", 's');
    list_append(generatedFont, (unitype) "K, 3, 2, -160, -100, -160, 60, 2, -160, -30, -60, 60, 2, -60, -100, -140, -12", 's');
    list_append(generatedFont, (unitype) "Ķ, 4, 2, -160, -100, -160, 60, 2, -160, -30, -60, 60, 2, -60, -100, -140, -12, 1, b, -115, -105, -115, -120, -125, -125", 's');
    list_append(generatedFont, (unitype) "L, 1, 3, -160, 60, -160, -100, -70, -100", 's');
    list_append(generatedFont, (unitype) "Ĺ, 2, 3, -160, 60, -160, -100, -70, -100, 2, -160, 90, -140, 110", 's');
    list_append(generatedFont, (unitype) "Ľ, 2, 3, -160, 60, -160, -100, -70, -100, 1, b, -105, 40, -95, 55, -95, 70", 's');
    list_append(generatedFont, (unitype) "Ļ, 2, 3, -160, 60, -160, -100, -70, -100, 1, b, -115, -125, -115, -140, -125, -145", 's');
    list_append(generatedFont, (unitype) "Ł, 2, 3, -160, 60, -160, -100, -70, -100, 2, -180, -30, -135, -15", 's');
    list_append(generatedFont, (unitype) "Ŀ, 2, 3, -160, 60, -160, -100, -70, -100, 1, -120, -30", 's');
    list_append(generatedFont, (unitype) "M, 1, 5, -160, -100, -160, 60, -90, -100, -20, 60, -20, -100", 's');
    list_append(generatedFont, (unitype) "N, 1, 4, -160, -100, -160, 60, -50, -100, -50, 60", 's');
    list_append(generatedFont, (unitype) "Ń, 2, 4, -160, -100, -160, 60, -50, -100, -50, 60, 2, -105, 85, -85, 105", 's');
    list_append(generatedFont, (unitype) "Ň, 2, 4, -160, -100, -160, 60, -50, -100, -50, 60, 3, -135, 105, -105, 85, -75, 105", 's');
    list_append(generatedFont, (unitype) "Ñ, 2, 4, -160, -100, -160, 60, -50, -100, -50, 60, 4, b, -140, 85, -140, 105, b, -125, 105, -115, 105, b, -105, 95, -95, 85, b, -85, 85, -70, 85, -70, 105", 's');
    list_append(generatedFont, (unitype) "Ņ, 2, 4, -160, -100, -160, 60, -50, -100, -50, 60, 1, b, -105, -125, -105, -140, -115, -145", 's');
    list_append(generatedFont, (unitype) "O, 1, 4, b, -40, -20, -40, 60, b, -100, 60, -160, 60, b, -160, -20, -160, -100, b, -100, -100, -40, -100, -40, -20", 's');
    list_append(generatedFont, (unitype) "Ò, 2, 4, b, -40, -20, -40, 60, b, -100, 60, -160, 60, b, -160, -20, -160, -100, b, -100, -100, -40, -100, -40, -20, 2, -100, 85, -120, 105", 's');
    list_append(generatedFont, (unitype) "Ó, 2, 4, b, -40, -20, -40, 60, b, -100, 60, -160, 60, b, -160, -20, -160, -100, b, -100, -100, -40, -100, -40, -20, 2, -100, 85, -80, 105", 's');
    list_append(generatedFont, (unitype) "Ô, 2, 4, b, -40, -20, -40, 60, b, -100, 60, -160, 60, b, -160, -20, -160, -100, b, -100, -100, -40, -100, -40, -20, 3, -130, 85, -100, 105, -70, 85", 's');
    list_append(generatedFont, (unitype) "Ö, 3, 4, b, -40, -20, -40, 60, b, -100, 60, -160, 60, b, -160, -20, -160, -100, b, -100, -100, -40, -100, -40, -20, 1, -120, 95, 1, -80, 95", 's');
    list_append(generatedFont, (unitype) "Õ, 2, 4, b, -40, -20, -40, 60, b, -100, 60, -160, 60, b, -160, -20, -160, -100, b, -100, -100, -40, -100, -40, -20, 4, b, -135, 85, -135, 105, b, -120, 105, -110, 105, b, -100, 95, -90, 85, b, -80, 85, -65, 85, -65, 105", 's');
    list_append(generatedFont, (unitype) "Ő, 3, 4, b, -40, -20, -40, 60, b, -100, 60, -160, 60, b, -160, -20, -160, -100, b, -100, -100, -40, -100, -40, -20, 2, -130, 85, -115, 105, 2, -80, 85, -65, 105", 's');
    list_append(generatedFont, (unitype) "Ø, 2, 4, b, -40, -20, -40, 60, b, -100, 60, -160, 60, b, -160, -20, -160, -100, b, -100, -100, -40, -100, -40, -20, 2, -40, 60, -160, -100", 's');
    list_append(generatedFont, (unitype) "Œ, 2, 6, b, -50, -20, -50, 60, b, -105, 60, -160, 60, b, -160, -20, -160, -100, b, -105, -100, -50, -100, -50, -20, -50, 60, 30, 60, 4, 30, -100, -50, -100, -50, -20, 20, -20", 's');
    list_append(generatedFont, (unitype) "P, 1, 5, -160, -100, -160, 60, b, -100, 60, -60, 60, b, -60, 20, -60, -20, -100, -20, -160, -20", 's');
    list_append(generatedFont, (unitype) "Q, 2, 4, b, -40, -20, -40, 60, b, -100, 60, -160, 60, b, -160, -20, -160, -100, b, -100, -100, -40, -100, -40, -20, 2, -65, -90, -40, -110", 's');
    list_append(generatedFont, (unitype) "R, 2, 5, -160, -100, -160, 60, b, -100, 60, -60, 60, b, -60, 20, -60, -20, -100, -20, -160, -20, 2, -100, -20, -60, -100", 's');
    list_append(generatedFont, (unitype) "Ŕ, 3, 5, -160, -100, -160, 60, b, -100, 60, -60, 60, b, -60, 20, -60, -20, -100, -20, -160, -20, 2, -100, -20, -60, -100, 2, -125, 85, -105, 105", 's');
    list_append(generatedFont, (unitype) "Ř, 3, 5, -160, -100, -160, 60, b, -100, 60, -60, 60, b, -60, 20, -60, -20, -100, -20, -160, -20, 2, -100, -20, -60, -100, 3, -150, 105, -120, 85, -90, 105", 's');
    list_append(generatedFont, (unitype) "S, 1, 6, b, -60, 30, -70, 60, b, -110, 60, -160, 60, b, -160, 20, -160, -20, b, -110, -20, -60, -20, b, -60, -60, -60, -100, b, -110, -100, -150, -100, -160, -70", 's');
    list_append(generatedFont, (unitype) "Ś, 2, 6, b, -60, 30, -70, 60, b, -110, 60, -160, 60, b, -160, 20, -160, -20, b, -110, -20, -60, -20, b, -60, -60, -60, -100, b, -110, -100, -150, -100, -160, -70, 2, -110, 85, -90, 105", 's');
    list_append(generatedFont, (unitype) "Š, 2, 6, b, -60, 30, -70, 60, b, -110, 60, -160, 60, b, -160, 20, -160, -20, b, -110, -20, -60, -20, b, -60, -60, -60, -100, b, -110, -100, -150, -100, -160, -70, 3, -140, 105, -110, 85, -80, 105", 's');
    list_append(generatedFont, (unitype) "Ş, 2, 6, b, -60, 30, -70, 60, b, -110, 60, -160, 60, b, -160, 20, -160, -20, b, -110, -20, -60, -20, b, -60, -60, -60, -100, b, -110, -100, -150, -100, -160, -70, 3, -110, -100, b, -113, -115, -90, -120, b, -90, -135, -90, -160, -115, -150", 's');
    list_append(generatedFont, (unitype) "Ș, 2, 6, b, -60, 30, -70, 60, b, -110, 60, -160, 60, b, -160, 20, -160, -20, b, -110, -20, -60, -20, b, -60, -60, -60, -100, b, -110, -100, -150, -100, -160, -70, 1, b, -110, -125, -110, -140, -120, -145", 's');
    list_append(generatedFont, (unitype) "ẞ, 1, 5, -160, -100, b, -160, 15, -160, 60, b, -110, 60, -60, 60, -55, 30, b, -105, -20, -40, -15, b, -50, -70, -65, -110, -120, -95", 's');
    list_append(generatedFont, (unitype) "T, 2, 2, -160, 60, -40, 60, 2, -100, 60, -100, -100", 's');
    list_append(generatedFont, (unitype) "Ť, 3, 2, -160, 60, -40, 60, 2, -100, 60, -100, -100, 3, -130, 105, -100, 85, -70, 105", 's');
    list_append(generatedFont, (unitype) "Ț, 3, 2, -160, 60, -40, 60, 2, -100, 60, -100, -100, 1, b, -100, -125, -100, -140, -110, -145", 's');
    list_append(generatedFont, (unitype) "Þ, 2, 2, -160, -100, -160, 60, 4, -160, 20, b, -105, 20, -60, 20, b, -60, -20, -60, -60, -105, -60, -160, -60", 's');
    list_append(generatedFont, (unitype) "U, 1, 4, -160, 60, b, -160, -40, -160, -100, b, -105, -100, -50, -100, -50, -40, -50, 60", 's');
    list_append(generatedFont, (unitype) "Ù, 2, 4, -160, 60, b, -160, -40, -160, -100, b, -105, -100, -50, -100, -50, -40, -50, 60, 2, -105, 85, -125, 105", 's');
    list_append(generatedFont, (unitype) "Ú, 2, 4, -160, 60, b, -160, -40, -160, -100, b, -105, -100, -50, -100, -50, -40, -50, 60, 2, -105, 85, -85, 105", 's');
    list_append(generatedFont, (unitype) "Û, 2, 4, -160, 60, b, -160, -40, -160, -100, b, -105, -100, -50, -100, -50, -40, -50, 60, 3, -135, 105, -105, 85, -75, 105", 's');
    list_append(generatedFont, (unitype) "Ü, 3, 4, -160, 60, b, -160, -40, -160, -100, b, -105, -100, -50, -100, -50, -40, -50, 60, 1, -125, 95, 1, -85, 95", 's');
    list_append(generatedFont, (unitype) "Ů, 2, 4, -160, 60, b, -160, -40, -160, -100, b, -105, -100, -50, -100, -50, -40, -50, 60, 4, b, -105, 85, -85, 85, b, -85, 105, -85, 125, b, -105, 125, -125, 125, b, -125, 105, -125, 85, -105, 85", 's');
    list_append(generatedFont, (unitype) "Ű, 3, 4, -160, 60, b, -160, -40, -160, -100, b, -105, -100, -50, -100, -50, -40, -50, 60, 2, -135, 85, -120, 105, 2, -85, 85, -70, 105", 's');
    list_append(generatedFont, (unitype) "Ū, 2, 4, -160, 60, b, -160, -40, -160, -100, b, -105, -100, -50, -100, -50, -40, -50, 60, 2, -65, 95, -145, 95", 's');
    list_append(generatedFont, (unitype) "Ų, 2, 4, -160, 60, b, -160, -40, -160, -100, b, -105, -100, -50, -100, -50, -40, -50, 60, 2, b, -95, -100, -115, -105, b, -115, -125, -115, -155, -85, -145", 's');
    list_append(generatedFont, (unitype) "V, 1, 3, -160, 60, -100, -100, -40, 60", 's');
    list_append(generatedFont, (unitype) "W, 1, 5, -160, 60, -110, -100, -60, 60, -10, -100, 40, 60", 's');
    list_append(generatedFont, (unitype) "X, 2, 2, -160, -100, -50, 60, 2, -160, 60, -50, -100", 's');
    list_append(generatedFont, (unitype) "Y, 2, 3, -160, 60, -100, -30, -40, 60, 2, -100, -30, -100, -100", 's');
    list_append(generatedFont, (unitype) "Ý, 3, 3, -160, 60, -100, -30, -40, 60, 2, -100, -30, -100, -100, 2, -100, 85, -80, 105", 's');
    list_append(generatedFont, (unitype) "Z, 1, 4, -50, -100, -160, -100, -50, 60, -160, 60", 's');
    list_append(generatedFont, (unitype) "Ź, 2, 4, -50, -100, -160, -100, -50, 60, -160, 60, 2, -105, 85, -85, 105", 's');
    list_append(generatedFont, (unitype) "Ž, 2, 4, -50, -100, -160, -100, -50, 60, -160, 60, 3, -130, 105, -100, 85, -70, 105", 's');
    list_append(generatedFont, (unitype) "Ż, 2, 4, -50, -100, -160, -100, -50, 60, -160, 60, 1, -100, 95", 's');
    list_append(generatedFont, (unitype) "А, 2, 3, -160, -100, -100, 60, -40, -100, 2, -137, -40, -63, -40", 's');
    list_append(generatedFont, (unitype) "Б, 1, 6, -70, 60, -160, 60, -160, -100, b, -110, -100, -60, -100, b, -60, -60, -60, -20, -110, -20, -160, -20", 's');
    list_append(generatedFont, (unitype) "В, 2, 5, -160, -100, -160, 60, b, -110, 60, -60, 60, b, -60, 20, -60, -20, -110, -20, -160, -20, 3, b, -110, -20, -60, -20, b, -60, -60, -60, -100, -110, -100, -160, -100", 's');
    list_append(generatedFont, (unitype) "Г, 1, 3, -160, -100, -160, 60, -70, 60", 's');
    list_append(generatedFont, (unitype) "Ґ, 1, 4, -160, -100, -160, 60, -70, 60, -70, 80", 's');
    list_append(generatedFont, (unitype) "Ғ, 2, 3, -160, -100, -160, 60, -70, 60, 2, -180, -20, -120, -20", 's');
    list_append(generatedFont, (unitype) "Д, 2, 4, -160, -140, -160, -100, -20, -100, -20, -140, 3, -40, -100, -40, 60, b, -120, 60, -120, -75, -150, -100", 's');
    list_append(generatedFont, (unitype) "Ђ, 3, 2, -160, 60, -40, 60, 2, -105, -100, -105, 60, 3, b, -105, -30, -80, -20, b, -55, -20, -25, -20, b, -25, -55, -25, -100, -70, -100", 's');
    list_append(generatedFont, (unitype) "Е, 2, 4, -70, -100, -160, -100, -160, 60, -70, 60, 2, -160, -20, -80, -20", 's');
    list_append(generatedFont, (unitype) "Ё, 4, 4, -70, -100, -160, -100, -160, 60, -70, 60, 2, -160, -20, -80, -20, 1, -135, 90, 1, -95, 90", 's');
    list_append(generatedFont, (unitype) "Є, 2, 4, b, -50, 20, -60, 60, b, -100, 60, -160, 60, b, -160, -20, -160, -100, b, -100, -100, -60, -100, -50, -60, 2, -160, -20, -100, -20", 's');
    list_append(generatedFont, (unitype) "Ә, 1, 5, -30, -20, b, -160, -20, -160, -100, b, -95, -100, -30, -100, b, -30, -20, -30, 60, b, -95, 60, -125, 60, -140, 45", 's');
    list_append(generatedFont, (unitype) "Ж, 4, 4, -160, 60, -100, -20, -50, -20, 10, 60, 2, -160, -100, -100, -20, 2, -50, -20, 10, -100, 2, -75, -100, -75, 60", 's');
    list_append(generatedFont, (unitype) "Ӂ, 5, 4, -160, 60, -100, -20, -50, -20, 10, 60, 2, -160, -100, -100, -20, 2, -50, -20, 10, -100, 2, -75, -100, -75, 60, 2, b, -105, 105, -105, 85, b, -75, 85, -45, 85, -45, 105", 's');
    list_append(generatedFont, (unitype) "З, 1, 6, b, -160, 30, -145, 60, b, -110, 60, -55, 60, b, -55, 20, -55, -20, b, -115, -20, -55, -20, b, -55, -60, -55, -100, b, -110, -100, -145, -100, -160, -70", 's');
    list_append(generatedFont, (unitype) "И, 1, 4, -160, 60, -160, -100, -50, 60, -50, -100", 's');
    list_append(generatedFont, (unitype) "Ӣ, 2, 4, -160, 60, -160, -100, -50, 60, -50, -100, 2, -145, 85, -65, 85", 's');
    list_append(generatedFont, (unitype) "Й, 2, 4, -160, 60, -160, -100, -50, 60, -50, -100, 2, b, -135, 105, -135, 85, b, -105, 85, -75, 85, -75, 105", 's');
    list_append(generatedFont, (unitype) "І, 1, 2, -160, -100, -160, 60", 's');
    list_append(generatedFont, (unitype) "Ї, 3, 2, -160, -100, -160, 60, 1, -170, 100, 1, -150, 100", 's');
    list_append(generatedFont, (unitype) "Ј, 1, 3, -70, 60, b, -70, -50, -70, -100, b, -120, -100, -150, -100, -160, -70", 's');
    list_append(generatedFont, (unitype) "К, 3, 2, -160, -100, -160, 60, 3, -160, -20, -135, -20, -60, 60, 2, -134, -20, -60, -100", 's');
    list_append(generatedFont, (unitype) "Қ, 3, 2, -160, -100, -160, 60, 3, -160, -20, -135, -20, -60, 60, 4, -134, -20, -60, -100, -55, -100, -55, -125", 's');
    list_append(generatedFont, (unitype) "Ҝ, 4, 2, -160, -100, -160, 60, 3, -160, -20, -115, -20, -60, 60, 2, -115, -20, -60, -100, 2, -135, 15, -135, -55", 's');
    list_append(generatedFont, (unitype) "Л, 1, 4, b, -160, -100, -145, -100, -140, -75, -130, 60, -50, 60, -50, -100", 's');
    list_append(generatedFont, (unitype) "Љ, 1, 7, b, -160, -100, -145, -100, -140, -75, -130, 60, -50, 60, -50, -100, b, -10, -100, 40, -100, b, 40, -60, 40, -20, -10, -20, -50, -20", 's');
    list_append(generatedFont, (unitype) "М, 1, 5, -160, -100, -160, 60, -90, -100, -20, 60, -20, -100", 's');
    list_append(generatedFont, (unitype) "Н, 3, 2, -160, -100, -160, 60, 2, -160, -20, -50, -20, 2, -50, -100, -50, 60", 's');
    list_append(generatedFont, (unitype) "Ң, 3, 2, -160, -100, -160, 60, 2, -160, -20, -50, -20, 4, -50, 60, -50, -100, -45, -100, -45, -125", 's');
    list_append(generatedFont, (unitype) "Њ, 2, 2, -160, -100, -160, 60, 5, -160, -20, b, -10, -20, 40, -20, b, 40, -60, 40, -100, -10, -100, -50, -100, -50, 60", 's');
    list_append(generatedFont, (unitype) "О, 1, 4, b, -100, -100, -40, -100, b, -40, -20, -40, 60, b, -100, 60, -160, 60, b, -160, -20, -160, -100, -100, -100", 's');
    list_append(generatedFont, (unitype) "Ө, 1, 5, b, -40, -20, -40, 60, b, -100, 60, -160, 60, b, -160, -20, -160, -100, b, -100, -100, -40, -100, -40, -20, -160, -20", 's');
    list_append(generatedFont, (unitype) "П, 1, 4, -160, -100, -160, 60, -50, 60, -50, -100", 's');
    list_append(generatedFont, (unitype) "Р, 1, 5, -160, -100, -160, 60, b, -100, 60, -60, 60, b, -60, 20, -60, -20, -100, -20, -160, -20", 's');
    list_append(generatedFont, (unitype) "С, 1, 4, b, -50, 20, -60, 60, b, -100, 60, -160, 60, b, -160, -20, -160, -100, b, -100, -100, -60, -100, -50, -60", 's');
    list_append(generatedFont, (unitype) "Т, 2, 2, -100, -100, -100, 60, 2, -160, 60, -40, 60", 's');
    list_append(generatedFont, (unitype) "Ћ, 3, 2, -160, 60, -40, 60, 2, -105, 60, -105, -100, 3, b, -105, -30, -85, -20, b, -55, -20, -25, -20, -25, -55, -25, -100", 's');
    list_append(generatedFont, (unitype) "У, 2, 2, b, -145, -105, -130, -110, -120, -90, -50, 60, 2, -102, -50, -160, 60", 's');
    list_append(generatedFont, (unitype) "Ӯ, 3, 2, b, -145, -105, -130, -110, -120, -90, -50, 60, 2, -102, -50, -160, 60, 2, -145, 85, -65, 85", 's');
    list_append(generatedFont, (unitype) "Ү, 2, 3, -160, 60, -100, -30, -40, 60, 2, -100, -30, -100, -100", 's');
    list_append(generatedFont, (unitype) "Ұ, 3, 3, -160, 60, -100, -30, -40, 60, 2, -100, -30, -100, -100, 2, -150, -30, -50, -30", 's');
    list_append(generatedFont, (unitype) "Ў, 3, 2, b, -145, -105, -130, -110, -120, -90, -50, 60, 2, -102, -50, -160, 60, 2, b, -130, 105, -130, 85, b, -100, 85, -70, 85, -70, 105", 's');
    list_append(generatedFont, (unitype) "Ф, 2, 4, b, -90, 40, -160, 45, b, -160, -20, -160, -85, b, -90, -80, -20, -85, b, -20, -20, -20, 45, -90, 40, 2, -90, 60, -90, -100", 's');
    list_append(generatedFont, (unitype) "Х, 2, 2, -160, -100, -50, 60, 2, -160, 60, -50, -100", 's');
    list_append(generatedFont, (unitype) "Ҳ, 2, 2, -160, -100, -50, 60, 4, -160, 60, -50, -100, -45, -100, -45, -125", 's');
    list_append(generatedFont, (unitype) "Һ, 2, 2, -160, -100, -160, 40, 3, b, -160, -30, -140, -10, b, -110, -10, -60, -10, -60, -50, -60, -100", 's');
    list_append(generatedFont, (unitype) "Ц, 2, 4, -160, 60, -160, -100, -50, -100, -50, 60, 3, -50, -100, -45, -100, -45, -125", 's');
    list_append(generatedFont, (unitype) "Ч, 2, 2, -50, -100, -50, 60, 3, b, -50, -10, -70, -30, b, -110, -30, -160, -30, -160, 10, -160, 60", 's');
    list_append(generatedFont, (unitype) "Ҷ, 2, 3, -160, 60, b, -160, 10, -160, -30, b, -110, -30, -70, -30, -50, -10, 4, -50, 60, -50, -100, -45, -100, -45, -125", 's');
    list_append(generatedFont, (unitype) "Ҹ, 3, 2, -50, -100, -50, 60, 3, b, -50, -10, -70, -30, b, -110, -30, -160, -30, -160, 10, -160, 60, 2, -110, 0, -110, -60", 's');
    list_append(generatedFont, (unitype) "Џ, 2, 4, -160, 60, -160, -100, -50, -100, -50, 60, 2, -105, -100, -105, -125", 's');
    list_append(generatedFont, (unitype) "Ш, 2, 4, -160, 60, -160, -100, -85, -100, -85, 60, 3, -85, -100, -10, -100, -10, 60", 's');
    list_append(generatedFont, (unitype) "Щ, 3, 3, -160, 60, -160, -100, -10, -100, 2, -85, 60, -85, -100, 4, -10, 60, -10, -100, -5, -100, -5, -125", 's');
    list_append(generatedFont, (unitype) "Ъ, 1, 6, -160, 60, -135, 60, -135, -100, b, -85, -100, -35, -100, b, -35, -60, -35, -20, -85, -20, -135, -20", 's');
    list_append(generatedFont, (unitype) "Ы, 2, 5, -160, 60, -160, -100, b, -110, -100, -60, -100, b, -60, -60, -60, -20, -110, -20, -160, -20, 2, -30, -100, -30, 60", 's');
    list_append(generatedFont, (unitype) "Ь, 1, 5, -160, 60, -160, -100, b, -110, -100, -60, -100, b, -60, -60, -60, -20, -110, -20, -160, -20", 's');
    list_append(generatedFont, (unitype) "Э, 2, 4, b, -160, 20, -150, 60, b, -110, 60, -50, 60, b, -50, -20, -50, -100, b, -110, -100, -150, -100, -160, -60, 2, -50, -20, -110, -20", 's');
    list_append(generatedFont, (unitype) "Ю, 2, 2, -160, -100, -160, 60, 5, -160, -20, b, -120, -20, -120, -100, b, -60, -100, 0, -100, b, 0, -20, 0, 60, b, -60, 60, -120, 60, -120, -20", 's');
    list_append(generatedFont, (unitype) "Я, 2, 5, -60, -100, -60, 60, b, -120, 60, -160, 60, b, -160, 20, -160, -20, -120, -20, -60, -20, 2, -120, -20, -160, -100", 's');
    list_append(generatedFont, (unitype) "Α, 2, 3, -160, -100, -100, 60, -40, -100, 2, -137, -40, -63, -40", 's');
    list_append(generatedFont, (unitype) "Β, 2, 7, -160, -100, -160, 60, b, -110, 60, -60, 60, b, -60, 20, -60, -20, b, -110, -20, -60, -20, b, -60, -60, -60, -100, -110, -100, -160, -100, 2, -110, -20, -160, -20", 's');
    list_append(generatedFont, (unitype) "Γ, 1, 3, -160, -100, -160, 60, -70, 60", 's');
    list_append(generatedFont, (unitype) "Δ, 1, 4, -160, -100, -100, 60, -40, -100, -160, -100", 's');
    list_append(generatedFont, (unitype) "Ε, 2, 4, -70, -100, -160, -100, -160, 60, -70, 60, 2, -160, -20, -80, -20", 's');
    list_append(generatedFont, (unitype) "Ζ, 1, 4, -50, -100, -160, -100, -50, 60, -160, 60", 's');
    list_append(generatedFont, (unitype) "Η, 3, 2, -160, -100, -160, 60, 2, -160, -20, -50, -20, 2, -50, -100, -50, 60", 's');
    list_append(generatedFont, (unitype) "Θ, 2, 4, b, -100, -100, -40, -100, b, -40, -20, -40, 60, b, -100, 60, -160, 60, b, -160, -20, -160, -100, -100, -100, 2, -140, -20, -60, -20", 's');
    list_append(generatedFont, (unitype) "Ι, 1, 2, -160, -100, -160, 60", 's');
    list_append(generatedFont, (unitype) "Κ, 3, 2, -160, -100, -160, 60, 2, -160, -30, -60, 60, 2, -140, -12, -60, -100", 's');
    list_append(generatedFont, (unitype) "Λ, 1, 3, -160, -100, -100, 60, -40, -100", 's');
    list_append(generatedFont, (unitype) "Μ, 1, 5, -160, -100, -160, 60, -90, -100, -20, 60, -20, -100", 's');
    list_append(generatedFont, (unitype) "Ν, 1, 4, -160, -100, -160, 60, -50, -100, -50, 60", 's');
    list_append(generatedFont, (unitype) "Ξ, 3, 2, -60, -100, -160, -100, 2, -70, -20, -150, -20, 2, -60, 60, -160, 60", 's');
    list_append(generatedFont, (unitype) "Ο, 1, 4, b, -100, -100, -40, -100, b, -40, -20, -40, 60, b, -100, 60, -160, 60, b, -160, -20, -160, -100, -100, -100", 's');
    list_append(generatedFont, (unitype) "Π, 1, 4, -160, -100, -160, 60, -50, 60, -50, -100", 's');
    list_append(generatedFont, (unitype) "Ρ, 1, 5, -160, -100, -160, 60, b, -100, 60, -60, 60, b, -60, 20, -60, -20, -100, -20, -160, -20", 's');
    list_append(generatedFont, (unitype) "Σ, 1, 5, -60, 60, -160, 60, -100, -20, -160, -100, -60, -100", 's');
    list_append(generatedFont, (unitype) "Τ, 2, 2, -160, 60, -40, 60, 2, -100, 60, -100, -100", 's');
    list_append(generatedFont, (unitype) "Υ, 2, 3, -160, 60, -100, -30, -40, 60, 2, -100, -30, -100, -100", 's');
    list_append(generatedFont, (unitype) "Φ, 2, 2, -95, -100, -95, 60, 4, b, -95, -80, -30, -80, b, -30, -20, -30, 40, b, -95, 40, -160, 40, b, -160, -20, -160, -80, -95, -80", 's');
    list_append(generatedFont, (unitype) "Χ, 2, 2, -160, -100, -50, 60, 2, -160, 60, -50, -100", 's');
    list_append(generatedFont, (unitype) "Ψ, 2, 2, -100, -100, -100, 60, 2, b, -160, 60, -160, -30, b, -100, -30, -40, -30, -40, 60", 's');
    list_append(generatedFont, (unitype) "Ω, 1, 6, -160, -100, b, -115, -100, -160, -70, b, -160, -20, -160, 60, b, -100, 60, -40, 60, b, -40, -20, -40, -70, -85, -100, -40, -100", 's');
    list_append(generatedFont, (unitype) "a, 2, 3, b, -160, 0, -140, 20, b, -110, 20, -70, 20, -70, -10, -70, -100, 4, b, -70, -50, -90, -40, b, -120, -40, -160, -40, b, -160, -70, -160, -100, b, -120, -100, -80, -100, -70, -80", 's');
    list_append(generatedFont, (unitype) "à, 3, 3, b, -160, 0, -140, 20, b, -110, 20, -70, 20, -70, -10, -70, -100, 4, b, -70, -50, -90, -40, b, -120, -40, -160, -40, b, -160, -70, -160, -100, b, -120, -100, -80, -100, -70, -80, 2, -110, 45, -130, 65", 's');
    list_append(generatedFont, (unitype) "á, 3, 3, b, -160, 0, -140, 20, b, -110, 20, -70, 20, -70, -10, -70, -100, 4, b, -70, -50, -90, -40, b, -120, -40, -160, -40, b, -160, -70, -160, -100, b, -120, -100, -80, -100, -70, -80, 2, -110, 45, -90, 65", 's');
    list_append(generatedFont, (unitype) "â, 3, 3, b, -160, 0, -140, 20, b, -110, 20, -70, 20, -70, -10, -70, -100, 4, b, -70, -50, -90, -40, b, -120, -40, -160, -40, b, -160, -70, -160, -100, b, -120, -100, -80, -100, -70, -80, 3, -145, 45, -115, 65, -85, 45", 's');
    list_append(generatedFont, (unitype) "ă, 3, 3, b, -160, 0, -140, 20, b, -110, 20, -70, 20, -70, -10, -70, -100, 4, b, -70, -50, -90, -40, b, -120, -40, -160, -40, b, -160, -70, -160, -100, b, -120, -100, -80, -100, -70, -80, 2, b, -145, 65, -145, 45, b, -115, 45, -85, 45, -85, 65", 's');
    list_append(generatedFont, (unitype) "ä, 4, 3, b, -160, 0, -140, 20, b, -110, 20, -70, 20, -70, -10, -70, -100, 4, b, -70, -50, -90, -40, b, -120, -40, -160, -40, b, -160, -70, -160, -100, b, -120, -100, -80, -100, -70, -80, 1, -135, 55, 1, -95, 55", 's');
    list_append(generatedFont, (unitype) "ã, 3, 3, b, -160, 0, -140, 20, b, -110, 20, -70, 20, -70, -10, -70, -100, 4, b, -70, -50, -90, -40, b, -120, -40, -160, -40, b, -160, -70, -160, -100, b, -120, -100, -80, -100, -70, -80, 4, b, -150, 45, -150, 65, b, -135, 65, -125, 65, b, -115, 55, -105, 45, b, -95, 45, -80, 45, -80, 65", 's');
    list_append(generatedFont, (unitype) "å, 3, 3, b, -160, 0, -140, 20, b, -110, 20, -70, 20, -70, -10, -70, -100, 4, b, -70, -50, -90, -40, b, -120, -40, -160, -40, b, -160, -70, -160, -100, b, -120, -100, -80, -100, -70, -80, 4, b, -115, 45, -95, 45, b, -95, 65, -95, 85, b, -115, 85, -135, 85, b, -135, 65, -135, 45, -115, 45", 's');
    list_append(generatedFont, (unitype) "ā, 3, 3, b, -160, 0, -140, 20, b, -110, 20, -70, 20, -70, -10, -70, -100, 4, b, -70, -50, -90, -40, b, -120, -40, -160, -40, b, -160, -70, -160, -100, b, -120, -100, -80, -100, -70, -80, 2, -145, 55, -85, 55", 's');
    list_append(generatedFont, (unitype) "ą, 3, 3, b, -160, 0, -140, 20, b, -110, 20, -70, 20, -70, -10, -70, -100, 4, b, -70, -50, -90, -40, b, -120, -40, -160, -40, b, -160, -70, -160, -100, b, -120, -100, -80, -100, -70, -80, 2, b, -70, -100, -95, -105, b, -95, -125, -95, -150, -70, -145", 's');
    list_append(generatedFont, (unitype) "æ, 4, 3, b, -160, 0, -140, 20, b, -115, 20, -85, 20, -80, -10, -80, -80, 4, b, -80, -50, -90, -40, b, -125, -40, -160, -40, b, -160, -70, -160, -100, b, -125, -100, -90, -100, -80, -80, 3, -80, -40, b, 0, -40, 0, 20, b, -40, 20, -70, 20, -80, -10, 2, b, -80, -40, -85, -100, b, -35, -100, -20, -100, 0, -85", 's');
    list_append(generatedFont, (unitype) "b, 2, 2, -160, -100, -160, 60, 4, b, -160, -80, -150, -100, b, -115, -100, -70, -100, b, -70, -40, -70, 20, b, -115, 20, -150, 20, -160, 0", 's');
    list_append(generatedFont, (unitype) "c, 1, 4, b, -70, -10, -80, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -80, -100, -70, -70", 's');
    list_append(generatedFont, (unitype) "ć, 2, 4, b, -70, -10, -80, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -80, -100, -70, -70, 2, -110, 45, -90, 65", 's');
    list_append(generatedFont, (unitype) "č, 2, 4, b, -70, -10, -80, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -80, -100, -70, -70, 3, -145, 65, -115, 45, -85, 65", 's');
    list_append(generatedFont, (unitype) "ċ, 2, 4, b, -70, -10, -80, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -80, -100, -70, -70, 1, -115, 55", 's');
    list_append(generatedFont, (unitype) "ç, 2, 4, b, -70, -10, -80, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -80, -100, -70, -70, 3, -115, -100, b, -117, -115, -95, -120, b, -95, -135, -95, -160, -120, -155", 's');
    list_append(generatedFont, (unitype) "d, 2, 2, -70, -100, -70, 60, 4, b, -70, 0, -80, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -80, -100, -70, -80", 's');
    list_append(generatedFont, (unitype) "ď, 3, 2, -70, -100, -70, 60, 4, b, -70, 0, -80, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -80, -100, -70, -80, 1, b, -45, 30, -40, 40, -40, 60", 's');
    list_append(generatedFont, (unitype) "đ, 3, 2, -70, -100, -70, 60, 4, b, -70, 0, -80, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -80, -100, -70, -80, 2, -60, 50, -105, 50", 's');
    list_append(generatedFont, (unitype) "ð, 2, 5, b, -70, -45, -70, 5, b, -115, 5, -160, 5, b, -160, -45, -160, -100, b, -115, -100, -70, -100, b, -70, -45, -70, 30, -120, 60, 2, -80, 60, -120, 30", 's');
    list_append(generatedFont, (unitype) "e, 1, 5, -160, -40, b, -70, -40, -70, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -90, -100, -70, -85", 's');
    list_append(generatedFont, (unitype) "è, 2, 5, -160, -40, b, -70, -40, -70, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -90, -100, -70, -85, 2, -115, 45, -135, 65", 's');
    list_append(generatedFont, (unitype) "é, 2, 5, -160, -40, b, -70, -40, -70, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -90, -100, -70, -85, 2, -115, 45, -95, 65", 's');
    list_append(generatedFont, (unitype) "ě, 2, 5, -160, -40, b, -70, -40, -70, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -90, -100, -70, -85, 3, -145, 65, -115, 45, -85, 65", 's');
    list_append(generatedFont, (unitype) "ê, 2, 5, -160, -40, b, -70, -40, -70, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -90, -100, -70, -85, 3, -145, 45, -115, 65, -85, 45", 's');
    list_append(generatedFont, (unitype) "ë, 3, 5, -160, -40, b, -70, -40, -70, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -90, -100, -70, -85, 1, -135, 55, 1, -95, 55", 's');
    list_append(generatedFont, (unitype) "ē, 2, 5, -160, -40, b, -70, -40, -70, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -90, -100, -70, -85, 2, -145, 55, -85, 55", 's');
    list_append(generatedFont, (unitype) "ė, 2, 5, -160, -40, b, -70, -40, -70, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -90, -100, -70, -85, 1, -115, 55", 's');
    list_append(generatedFont, (unitype) "ę, 2, 5, -160, -40, b, -70, -40, -70, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -90, -100, -70, -85, 2, b, -90, -95, -120, -105, b, -120, -130, -120, -155, -90, -145", 's');
    list_append(generatedFont, (unitype) "ə, 1, 5, -70, -40, b, -160, -40, -160, -100, b, -115, -100, -70, -100, b, -70, -40, -70, 20, b, -115, 20, -140, 20, -160, 5", 's');
    list_append(generatedFont, (unitype) "f, 2, 2, -140, -100, b, -140, 40, -140, 60, -110, 55, 2, -160, 0, -115, 0", 's');
    list_append(generatedFont, (unitype) "g, 2, 3, -70, 20, b, -70, -120, -70, -160, b, -110, -160, -140, -160, -160, -140, 4, b, -70, 0, -80, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -80, -100, -70, -80", 's');
    list_append(generatedFont, (unitype) "ğ, 3, 3, -70, 20, b, -70, -120, -70, -160, b, -110, -160, -140, -160, -160, -140, 4, b, -70, 0, -80, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -80, -100, -70, -80, 2, b, -145, 65, -145, 45, b, -115, 45, -85, 45, -85, 65", 's');
    list_append(generatedFont, (unitype) "ġ, 3, 3, -70, 20, b, -70, -120, -70, -160, b, -110, -160, -140, -160, -160, -140, 4, b, -70, 0, -80, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -80, -100, -70, -80, 1, -115, 55", 's');
    list_append(generatedFont, (unitype) "h, 2, 2, -160, 60, -160, -100, 3, b, -160, -10, -150, 20, b, -115, 20, -70, 20, -70, -30, -70, -100", 's');
    list_append(generatedFont, (unitype) "ħ, 3, 2, -160, 60, -160, -100, 3, b, -160, -10, -150, 20, b, -115, 20, -70, 20, -70, -30, -70, -100, 2, -170, 45, -125, 45", 's');
    list_append(generatedFont, (unitype) "i, 2, 2, -160, -100, -160, 10, 1, -160, 50", 's');
    list_append(generatedFont, (unitype) "ì, 2, 2, -160, -100, -160, 10, 2, -160, 50, -170, 70", 's');
    list_append(generatedFont, (unitype) "í, 2, 2, -160, -100, -160, 10, 2, -160, 50, -150, 70", 's');
    list_append(generatedFont, (unitype) "î, 2, 2, -160, -100, -160, 10, 3, -180, 50, -160, 70, -140, 50", 's');
    list_append(generatedFont, (unitype) "ï, 3, 2, -160, -100, -160, 10, 1, -175, 50, 1, -145, 50", 's');
    list_append(generatedFont, (unitype) "ī, 2, 2, -160, -100, -160, 10, 2, -180, 50, -140, 50", 's');
    list_append(generatedFont, (unitype) "ı, 1, 2, -160, -100, -160, 10", 's');
    list_append(generatedFont, (unitype) "į, 2, 1, -160, 50, 3, -160, 10, b, -160, -100, -170, -105, b, -170, -115, -170, -125, -160, -125", 's');
    list_append(generatedFont, (unitype) "j, 2, 3, b, -180, -140, -170, -160, b, -150, -160, -120, -160, -120, -120, -120, 10, 1, -120, 50", 's');
    list_append(generatedFont, (unitype) "k, 3, 2, -160, -100, -160, 60, 2, -160, -60, -90, 10, 2, -140, -40, -90, -100", 's');
    list_append(generatedFont, (unitype) "ķ, 4, 2, -160, -100, -160, 60, 2, -160, -60, -90, 10, 2, -140, -40, -90, -100, 1, b, -130, -100, -130, -115, -140, -120", 's');
    list_append(generatedFont, (unitype) "l, 1, 2, -160, -100, -160, 60", 's');
    list_append(generatedFont, (unitype) "ĺ, 2, 2, -160, -100, -160, 60, 2, -160, 90, -150, 110", 's');
    list_append(generatedFont, (unitype) "ľ, 2, 2, -160, -100, -160, 60, 1, b, -135, 50, -130, 60, -130, 80", 's');
    list_append(generatedFont, (unitype) "ļ, 2, 2, -160, -100, -160, 60, 1, b, -160, -120, -160, -140, -165, -150", 's');
    list_append(generatedFont, (unitype) "ł, 2, 2, -160, -100, -160, 60, 2, -180, -25, -140, -5", 's');
    list_append(generatedFont, (unitype) "ŀ, 2, 2, -160, -100, -160, 60, 1, -135, -20", 's');
    list_append(generatedFont, (unitype) "m, 3, 2, -160, -100, -160, 20, 3, b, -160, 0, -150, 20, b, -120, 20, -80, 20, -80, -20, -80, -100, 3, b, -80, -20, -80, 20, b, -40, 20, 0, 20, 0, -20, 0, -100", 's');
    list_append(generatedFont, (unitype) "n, 2, 2, -160, -100, -160, 20, 3, b, -160, 0, -150, 20, b, -115, 20, -70, 20, -70, -20, -70, -100", 's');
    list_append(generatedFont, (unitype) "ń, 3, 2, -160, -100, -160, 20, 3, b, -160, 0, -150, 20, b, -115, 20, -70, 20, -70, -20, -70, -100, 2, -115, 45, -95, 65", 's');
    list_append(generatedFont, (unitype) "ň, 3, 2, -160, -100, -160, 20, 3, b, -160, 0, -150, 20, b, -115, 20, -70, 20, -70, -20, -70, -100, 3, -145, 65, -115, 45, -85, 65", 's');
    list_append(generatedFont, (unitype) "ñ, 3, 2, -160, -100, -160, 20, 3, b, -160, 0, -150, 20, b, -115, 20, -70, 20, -70, -20, -70, -100, 4, b, -150, 45, -150, 65, b, -135, 65, -125, 65, b, -115, 55, -105, 45, b, -95, 45, -80, 45, -80, 65", 's');
    list_append(generatedFont, (unitype) "ņ, 3, 2, -160, -100, -160, 20, 3, b, -160, 0, -150, 20, b, -115, 20, -70, 20, -70, -20, -70, -100, 1, b, -115, -100, -115, -120, -120, -130", 's');
    list_append(generatedFont, (unitype) "o, 1, 4, b, -115, -100, -70, -100, b, -70, -40, -70, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, -115, -100", 's');
    list_append(generatedFont, (unitype) "ò, 2, 4, b, -115, -100, -70, -100, b, -70, -40, -70, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, -115, -100, 2, -115, 45, -135, 65", 's');
    list_append(generatedFont, (unitype) "ó, 2, 4, b, -115, -100, -70, -100, b, -70, -40, -70, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, -115, -100, 2, -115, 45, -95, 65", 's');
    list_append(generatedFont, (unitype) "ô, 2, 4, b, -115, -100, -70, -100, b, -70, -40, -70, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, -115, -100, 3, -145, 45, -115, 65, -85, 45", 's');
    list_append(generatedFont, (unitype) "ö, 3, 4, b, -115, -100, -70, -100, b, -70, -40, -70, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, -115, -100, 1, -135, 55, 1, -95, 55", 's');
    list_append(generatedFont, (unitype) "õ, 2, 4, b, -115, -100, -70, -100, b, -70, -40, -70, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, -115, -100, 4, b, -150, 45, -150, 65, b, -135, 65, -125, 65, b, -115, 55, -105, 45, b, -95, 45, -80, 45, -80, 65", 's');
    list_append(generatedFont, (unitype) "ő, 3, 4, b, -115, -100, -70, -100, b, -70, -40, -70, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, -115, -100, 2, -140, 45, -120, 65, 2, -100, 45, -80, 65", 's');
    list_append(generatedFont, (unitype) "ø, 2, 4, b, -115, -100, -70, -100, b, -70, -40, -70, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, -115, -100, 2, -160, -100, -70, 20", 's');
    list_append(generatedFont, (unitype) "œ, 1, 8, b, -70, -40, -70, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -70, -100, -70, -40, b, 10, -40, 10, 20, b, -30, 20, -60, 20, -70, -10, b, -70, -40, -75, -100, b, -30, -100, -10, -100, 10, -85", 's');
    list_append(generatedFont, (unitype) "p, 2, 2, -160, -160, -160, 20, 4, b, -160, 0, -150, 20, b, -115, 20, -70, 20, b, -70, -40, -70, -100, b, -115, -100, -150, -100, -160, -80", 's');
    list_append(generatedFont, (unitype) "q, 2, 2, -70, 20, -70, -160, 4, b, -70, 0, -80, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -80, -100, -70, -80", 's');
    list_append(generatedFont, (unitype) "r, 2, 2, -160, -100, -160, 20, 2, b, -160, 0, -150, 20, b, -110, 20, -90, 20, -80, 10", 's');
    list_append(generatedFont, (unitype) "ŕ, 3, 2, -160, -100, -160, 20, 2, b, -160, 0, -150, 20, b, -110, 20, -90, 20, -80, 10, 2, -120, 45, -100, 65", 's');
    list_append(generatedFont, (unitype) "ř, 3, 2, -160, -100, -160, 20, 2, b, -160, 0, -150, 20, b, -110, 20, -90, 20, -80, 10, 3, -145, 65, -115, 45, -85, 65", 's');
    list_append(generatedFont, (unitype) "s, 1, 6, b, -70, 0, -80, 20, b, -115, 20, -160, 20, b, -160, -10, -160, -40, b, -115, -40, -70, -40, b, -70, -70, -70, -100, b, -115, -100, -150, -100, -160, -80", 's');
    list_append(generatedFont, (unitype) "ś, 2, 6, b, -70, 0, -80, 20, b, -115, 20, -160, 20, b, -160, -10, -160, -40, b, -115, -40, -70, -40, b, -70, -70, -70, -100, b, -115, -100, -150, -100, -160, -80, 2, -115, 45, -95, 65", 's');
    list_append(generatedFont, (unitype) "š, 2, 6, b, -70, 0, -80, 20, b, -115, 20, -160, 20, b, -160, -10, -160, -40, b, -115, -40, -70, -40, b, -70, -70, -70, -100, b, -115, -100, -150, -100, -160, -80, 3, -145, 65, -115, 45, -85, 65", 's');
    list_append(generatedFont, (unitype) "ş, 2, 6, b, -70, 0, -80, 20, b, -115, 20, -160, 20, b, -160, -10, -160, -40, b, -115, -40, -70, -40, b, -70, -70, -70, -100, b, -115, -100, -150, -100, -160, -80, 3, -115, -100, b, -117, -115, -95, -120, b, -95, -135, -95, -160, -120, -155", 's');
    list_append(generatedFont, (unitype) "ș, 2, 6, b, -70, 0, -80, 20, b, -115, 20, -160, 20, b, -160, -10, -160, -40, b, -115, -40, -70, -40, b, -70, -70, -70, -100, b, -115, -100, -150, -100, -160, -80, 1, b, -115, -110, -115, -125, -120, -140", 's');
    list_append(generatedFont, (unitype) "ß, 1, 7, -160, -100, b, -160, 15, -160, 60, b, -125, 60, -90, 60, b, -90, 25, -90, 5, b, -110, -10, -130, -25, b, -90, -45, -65, -55, b, -75, -85, -90, -110, -135, -95", 's');
    list_append(generatedFont, (unitype) "t, 2, 2, -140, 50, b, -140, -70, -140, -100, -110, -100, 2, -160, 20, -120, 20", 's');
    list_append(generatedFont, (unitype) "ť, 3, 2, -140, 50, b, -140, -70, -140, -100, -110, -100, 2, -160, 20, -120, 20, 1, b, -95, 20, -90, 30, -90, 50", 's');
    list_append(generatedFont, (unitype) "ț, 3, 2, -140, 50, b, -140, -70, -140, -100, -110, -100, 2, -160, 20, -120, 20, 1, b, -130, -110, -130, -125, -135, -140", 's');
    list_append(generatedFont, (unitype) "þ, 2, 2, -160, -140, -160, 60, 4, b, -160, 0, -150, 20, b, -115, 20, -70, 20, b, -70, -40, -70, -100, b, -115, -100, -150, -100, -160, -80", 's');
    list_append(generatedFont, (unitype) "u, 2, 2, -70, 20, -70, -100, 3, b, -70, -80, -80, -100, b, -115, -100, -160, -100, -160, -60, -160, 20", 's');
    list_append(generatedFont, (unitype) "ù, 3, 2, -70, 20, -70, -100, 3, b, -70, -80, -80, -100, b, -115, -100, -160, -100, -160, -60, -160, 20, 2, -115, 45, -135, 65", 's');
    list_append(generatedFont, (unitype) "ú, 3, 2, -70, 20, -70, -100, 3, b, -70, -80, -80, -100, b, -115, -100, -160, -100, -160, -60, -160, 20, 2, -115, 45, -95, 65", 's');
    list_append(generatedFont, (unitype) "û, 3, 2, -70, 20, -70, -100, 3, b, -70, -80, -80, -100, b, -115, -100, -160, -100, -160, -60, -160, 20, 3, -145, 45, -115, 65, -85, 45", 's');
    list_append(generatedFont, (unitype) "ü, 4, 2, -70, 20, -70, -100, 3, b, -70, -80, -80, -100, b, -115, -100, -160, -100, -160, -60, -160, 20, 1, -135, 55, 1, -95, 55", 's');
    list_append(generatedFont, (unitype) "ů, 3, 2, -70, 20, -70, -100, 3, b, -70, -80, -80, -100, b, -115, -100, -160, -100, -160, -60, -160, 20, 4, b, -115, 45, -95, 45, b, -95, 65, -95, 85, b, -115, 85, -135, 85, b, -135, 65, -135, 45, -115, 45", 's');
    list_append(generatedFont, (unitype) "ű, 4, 2, -70, 20, -70, -100, 3, b, -70, -80, -80, -100, b, -115, -100, -160, -100, -160, -60, -160, 20, 2, -100, 45, -80, 65, 2, -140, 45, -120, 65", 's');
    list_append(generatedFont, (unitype) "ū, 3, 2, -70, 20, -70, -100, 3, b, -70, -80, -80, -100, b, -115, -100, -160, -100, -160, -60, -160, 20, 2, -145, 55, -85, 55", 's');
    list_append(generatedFont, (unitype) "ų, 1, 6, -160, 20, b, -160, -60, -160, -100, b, -115, -100, -80, -100, -70, -80, -70, 20, b, -70, -100, -95, -105, b, -95, -125, -95, -150, -70, -145", 's');
    list_append(generatedFont, (unitype) "v, 1, 3, -160, 20, -115, -100, -70, 20", 's');
    list_append(generatedFont, (unitype) "w, 1, 5, -160, 20, -120, -100, -80, 20, -40, -100, 0, 20", 's');
    list_append(generatedFont, (unitype) "x, 2, 2, -160, 20, -70, -100, 2, -160, -100, -70, 20", 's');
    list_append(generatedFont, (unitype) "y, 2, 2, -70, 20, b, -130, -140, -140, -160, -160, -150, 2, -115, -99, -160, 20", 's');
    list_append(generatedFont, (unitype) "ý, 3, 2, -70, 20, b, -130, -140, -140, -160, -160, -150, 2, -115, -99, -160, 20, 2, -115, 45, -95, 65", 's');
    list_append(generatedFont, (unitype) "z, 1, 4, -70, -100, -160, -100, -70, 20, -160, 20", 's');
    list_append(generatedFont, (unitype) "ź, 2, 4, -70, -100, -160, -100, -70, 20, -160, 20, 2, -115, 45, -95, 65", 's');
    list_append(generatedFont, (unitype) "ž, 2, 4, -70, -100, -160, -100, -70, 20, -160, 20, 3, -145, 65, -115, 45, -85, 65", 's');
    list_append(generatedFont, (unitype) "ż, 2, 4, -70, -100, -160, -100, -70, 20, -160, 20, 1, -115, 55", 's');
    list_append(generatedFont, (unitype) "а, 2, 3, b, -160, 0, -140, 20, b, -110, 20, -70, 20, -70, -10, -70, -100, 4, b, -70, -50, -90, -40, b, -120, -40, -160, -40, b, -160, -70, -160, -100, b, -120, -100, -80, -100, -70, -80", 's');
    list_append(generatedFont, (unitype) "б, 1, 6, b, -160, -45, -160, 5, b, -115, 5, -70, 5, b, -70, -45, -70, -100, b, -115, -100, -160, -100, b, -160, -45, -160, 45, b, -105, 45, -85, 45, -80, 60", 's');
    list_append(generatedFont, (unitype) "в, 2, 5, -160, -100, b, -160, 20, -130, 20, b, -110, 20, -75, 20, b, -75, -10, -75, -40, -120, -40, -160, -40, 3, b, -120, -40, -75, -40, b, -75, -70, -75, -100, -110, -100, -160, -100", 's');
    list_append(generatedFont, (unitype) "г, 1, 3, -160, -100, -160, 20, -95, 20", 's');
    list_append(generatedFont, (unitype) "ґ, 1, 4, -160, -100, -160, 20, -95, 20, -95, 40", 's');
    list_append(generatedFont, (unitype) "ғ, 2, 3, -150, -100, -150, 20, -85, 20, 2, -160, -30, -130, -30", 's');
    list_append(generatedFont, (unitype) "д, 2, 4, -160, -135, -160, -100, -50, -100, -50, -135, 3, -65, -100, -65, 20, b, -135, 20, -135, -50, -145, -100", 's');
    list_append(generatedFont, (unitype) "ђ, 3, 2, -160, -100, -160, 60, 2, -180, 40, -120, 40, 3, b, -160, -10, -150, 20, b, -115, 20, -70, 20, -70, -30, b, -70, -100, -70, -130, -95, -125", 's');
    list_append(generatedFont, (unitype) "е, 1, 5, -160, -40, b, -70, -40, -70, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -90, -100, -70, -85", 's');
    list_append(generatedFont, (unitype) "ё, 3, 5, -160, -40, b, -70, -40, -70, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -90, -100, -70, -85, 1, -135, 55, 1, -95, 55", 's');
    list_append(generatedFont, (unitype) "є, 2, 4, b, -70, -10, -80, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -80, -100, -70, -70, 2, -160, -40, -110, -40", 's');
    list_append(generatedFont, (unitype) "ә, 1, 5, -70, -40, b, -160, -40, -160, -100, b, -115, -100, -70, -100, b, -70, -40, -70, 20, b, -115, 20, -140, 20, -160, 5", 's');
    list_append(generatedFont, (unitype) "ж, 4, 4, -160, -100, -115, -40, -75, -40, -30, -100, 2, -160, 20, -115, -40, 2, -75, -40, -30, 20, 2, -95, 20, -95, -100", 's');
    list_append(generatedFont, (unitype) "ӂ, 5, 4, -160, -100, -115, -40, -75, -40, -30, -100, 2, -160, 20, -115, -40, 2, -75, -40, -30, 20, 2, -95, 20, -95, -100, 2, b, -125, 65, -125, 45, b, -95, 45, -65, 45, -65, 65", 's');
    list_append(generatedFont, (unitype) "з, 1, 6, b, -160, -5, -155, 20, b, -125, 20, -85, 20, b, -85, -10, -85, -40, b, -120, -40, -85, -40, b, -85, -70, -85, -100, b, -125, -100, -155, -100, -160, -75", 's');
    list_append(generatedFont, (unitype) "и, 1, 4, -160, 20, -160, -100, -70, 20, -70, -100", 's');
    list_append(generatedFont, (unitype) "ӣ, 2, 4, -160, 20, -160, -100, -70, 20, -70, -100, 2, -145, 55, -85, 55", 's');
    list_append(generatedFont, (unitype) "й, 2, 4, -160, 20, -160, -100, -70, 20, -70, -100, 2, b, -145, 65, -145, 45, b, -115, 45, -85, 45, -85, 65", 's');
    list_append(generatedFont, (unitype) "і, 2, 2, -160, -100, -160, 10, 1, -160, 50", 's');
    list_append(generatedFont, (unitype) "ї, 3, 2, -160, -100, -160, 10, 1, -175, 50, 1, -145, 50", 's');
    list_append(generatedFont, (unitype) "ј, 2, 1, -120, 50, 3, -120, 10, b, -120, -120, -120, -160, b, -150, -160, -170, -160, -180, -140", 's');
    list_append(generatedFont, (unitype) "к, 3, 2, -160, -100, -160, 20, 3, -160, -40, -140, -40, -85, 20, 2, -140, -40, -85, -100", 's');
    list_append(generatedFont, (unitype) "қ, 3, 2, -160, -100, -160, 20, 3, -160, -40, -140, -40, -85, 20, 4, -140, -40, -85, -100, -80, -100, -80, -125", 's');
    list_append(generatedFont, (unitype) "ҝ, 4, 2, -160, -100, -160, 20, 3, -160, -40, -125, -40, -85, 20, 2, -125, -40, -85, -100, 2, -140, -15, -140, -65", 's');
    list_append(generatedFont, (unitype) "л, 1, 4, b, -160, -100, -145, -100, -140, -80, -130, 20, -70, 20, -70, -100", 's');
    list_append(generatedFont, (unitype) "љ, 1, 7, b, -160, -100, -145, -100, -140, -80, -130, 20, -70, 20, -70, -100, b, -30, -100, 5, -100, b, 5, -70, 5, -40, -30, -40, -70, -40", 's');
    list_append(generatedFont, (unitype) "м, 1, 5, -160, -100, -160, 20, -100, -100, -40, 20, -40, -100", 's');
    list_append(generatedFont, (unitype) "н, 3, 2, -160, -100, -160, 20, 2, -160, -40, -70, -40, 2, -70, -100, -70, 20", 's');
    list_append(generatedFont, (unitype) "ң, 3, 2, -160, -100, -160, 20, 2, -160, -40, -70, -40, 4, -70, 20, -70, -100, -65, -100, -65, -125", 's');
    list_append(generatedFont, (unitype) "њ, 2, 2, -160, -100, -160, 20, 5, -160, -40, b, -30, -40, 5, -40, b, 5, -70, 5, -100, -30, -100, -70, -100, -70, 20", 's');
    list_append(generatedFont, (unitype) "о, 1, 4, b, -115, -100, -70, -100, b, -70, -40, -70, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, -115, -100", 's');
    list_append(generatedFont, (unitype) "ө, 1, 5, b, -70, -40, -70, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -70, -100, -70, -40, -160, -40", 's');
    list_append(generatedFont, (unitype) "п, 1, 4, -160, -100, -160, 20, -70, 20, -70, -100", 's');
    list_append(generatedFont, (unitype) "р, 2, 2, -160, -160, -160, 20, 4, b, -160, 0, -150, 20, b, -115, 20, -70, 20, b, -70, -40, -70, -100, b, -115, -100, -150, -100, -160, -80", 's');
    list_append(generatedFont, (unitype) "с, 1, 4, b, -70, -10, -80, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -80, -100, -70, -70", 's');
    list_append(generatedFont, (unitype) "т, 2, 2, -160, 20, -70, 20, 2, -115, 20, -115, -100", 's');
    list_append(generatedFont, (unitype) "ћ, 3, 2, -160, 60, -160, -100, 3, b, -160, -10, -150, 20, b, -115, 20, -70, 20, -70, -30, -70, -100, 2, -170, 45, -125, 45", 's');
    list_append(generatedFont, (unitype) "у, 2, 2, -160, 20, -115, -100, 2, -70, 20, b, -130, -140, -140, -160, -160, -150", 's');
    list_append(generatedFont, (unitype) "ӯ, 3, 2, -160, 20, -115, -100, 2, -70, 20, b, -130, -140, -140, -160, -160, -150, 2, -145, 55, -85, 55", 's');
    list_append(generatedFont, (unitype) "ү, 2, 3, -160, 0, -115, -100, -115, -155, 2, -115, -100, -70, 0", 's');
    list_append(generatedFont, (unitype) "ұ, 3, 3, -160, 0, -115, -100, -115, -155, 2, -115, -100, -70, 0, 2, -150, -100, -80, -100", 's');
    list_append(generatedFont, (unitype) "ў, 3, 2, -160, 20, -115, -100, 2, -70, 20, b, -130, -140, -140, -160, -160, -150, 2, b, -145, 65, -145, 45, b, -115, 45, -85, 45, -85, 65", 's');
    list_append(generatedFont, (unitype) "ф, 2, 2, -100, -140, -100, 60, 4, b, -100, 10, -40, 40, b, -40, -40, -40, -120, b, -100, -90, -160, -120, b, -160, -40, -160, 40, -100, 10", 's');
    list_append(generatedFont, (unitype) "х, 2, 2, -160, -100, -70, 20, 2, -160, 20, -70, -100", 's');
    list_append(generatedFont, (unitype) "ҳ, 2, 2, -160, -100, -70, 20, 4, -160, 20, -70, -100, -65, -100, -65, -", 's');
    list_append(generatedFont, (unitype) "һ, 2, 2, -160, 60, -160, -100, 3, b, -160, -10, -150, 20, b, -115, 20, -70, 20, -70, -30, -70, -100", 's');
    list_append(generatedFont, (unitype) "ц, 2, 4, -160, 20, -160, -100, -70, -100, -70, 20, 3, -70, -100, -65, -100, -65, -125", 's');
    list_append(generatedFont, (unitype) "ч, 2, 2, -80, -100, -80, 20, 3, b, -80, -40, -95, -50, b, -120, -50, -160, -50, -160, -15, -160, 20", 's');
    list_append(generatedFont, (unitype) "ҷ, 2, 3, -160, 20, b, -160, -15, -160, -50, b, -120, -50, -95, -50, -80, -40, 4, -80, 20, -80, -100, -75, -100, -75, -125", 's');
    list_append(generatedFont, (unitype) "ҹ, 3, 2, -80, -100, -80, 20, 3, b, -80, -40, -95, -50, b, -120, -50, -160, -50, -160, -15, -160, 20, 2, -120, -20, -120, -80", 's');
    list_append(generatedFont, (unitype) "џ, 2, 4, -160, 20, -160, -100, -70, -100, -70, 20, 2, -115, -100, -115, -125", 's');
    list_append(generatedFont, (unitype) "ш, 3, 3, -160, 20, -160, -100, -20, -100, 2, -90, 20, -90, -100, 2, -20, 20, -20, -100", 's');
    list_append(generatedFont, (unitype) "щ, 3, 3, -160, 20, -160, -100, -20, -100, 2, -90, 20, -90, -100, 4, -20, 20, -20, -100, -15, -100, -15, -125", 's');
    list_append(generatedFont, (unitype) "ъ, 1, 6, -160, 20, -140, 20, -140, -100, b, -100, -100, -60, -100, b, -60, -70, -60, -40, -100, -40, -140, -40", 's');
    list_append(generatedFont, (unitype) "ы, 2, 5, -160, 20, -160, -100, b, -120, -100, -80, -100, b, -80, -70, -80, -40, -120, -40, -160, -40, 2, -55, -100, -55, 20", 's');
    list_append(generatedFont, (unitype) "ь, 1, 5, -160, 20, -160, -100, b, -120, -100, -80, -100, b, -80, -70, -80, -40, -120, -40, -160, -40", 's');
    list_append(generatedFont, (unitype) "э, 2, 4, b, -160, -10, -150, 20, b, -115, 20, -70, 20, b, -70, -40, -70, -100, b, -115, -100, -150, -100, -160, -70, 2, -70, -40, -120, -40", 's');
    list_append(generatedFont, (unitype) "ю, 2, 2, -160, 20, -160, -100, 5, -160, -40, b, -120, -40, -120, -100, b, -75, -100, -30, -100, b, -30, -40, -30, 20, b, -75, 20, -120, 20, -120, -40", 's');
    list_append(generatedFont, (unitype) "я, 2, 5, -70, -100, -70, 20, b, -120, 20, -160, 20, b, -160, -10, -160, -40, -130, -40, -70, -40, 2, -125, -40, -160, -100", 's');
    list_append(generatedFont, (unitype) "α, 2, 2, -70, 20, b, -70, -80, -70, -100, -50, -100, 4, b, -70, -10, -80, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -70, -100, -70, -70", 's');
    list_append(generatedFont, (unitype) "β, 1, 7, -160, -135, b, -160, 0, -160, 40, b, -120, 40, -85, 40, b, -85, 10, -85, -30, b, -130, -30, -75, -30, b, -75, -60, -75, -100, b, -115, -100, -140, -100, -160, -80", 's');
    list_append(generatedFont, (unitype) "γ, 2, 3, -160, 0, -115, -100, -70, 0, 2, -115, -100, -115, -155", 's');
    list_append(generatedFont, (unitype) "δ, 2, 4, b, -70, -50, -70, 0, b, -115, 0, -160, 0, b, -160, -50, -160, -100, b, -115, -100, -70, -100, -70, -50, 2, b, -92, -4, -145, 10, b, -145, 30, -150, 70, -95, 55", 's');
    list_append(generatedFont, (unitype) "ε, 1, 6, b, -70, -5, -75, 20, b, -115, 20, -160, 20, b, -160, -15, -160, -40, b, -110, -40, -160, -40, b, -160, -75, -160, -100, b, -115, -100, -75, -100, -70, -75", 's');
    list_append(generatedFont, (unitype) "ζ, 1, 4, -160, 40, b, -80, 40, -140, -20, b, -155, -55, -170, -100, b, -110, -100, -70, -100, -100, -135", 's');
    list_append(generatedFont, (unitype) "η, 2, 2, -160, -100, -160, 20, 3, b, -160, 0, -150, 20, b, -115, 20, -70, 20, -70, -20, -70, -155", 's');
    list_append(generatedFont, (unitype) "θ, 1, 5, b, -160, -25, -160, -100, b, -115, -100, -70, -100, b, -70, -25, -70, 50, b, -115, 50, -160, 50, -160, -25, -70, -25", 's');
    list_append(generatedFont, (unitype) "ι, 1, 2, -160, 20, b, -160, -70, -160, -105, -130, -100", 's');
    list_append(generatedFont, (unitype) "κ, 3, 2, -160, -100, -160, 20, 3, -160, -40, -140, -40, -85, 20, 2, -140, -40, -85, -100", 's');
    list_append(generatedFont, (unitype) "λ, 2, 2, b, -150, 50, -140, 50, -135, 40, b, -85, -90, -80, -100, -70, -100, 2, -120, 0, -160, -100", 's');
    list_append(generatedFont, (unitype) "μ, 2, 2, -70, 20, -70, -100, 4, b, -70, -80, -80, -100, b, -115, -100, -160, -100, -160, -60, -160, 20, -160, -155", 's');
    list_append(generatedFont, (unitype) "µ, 2, 2, -70, 20, -70, -100, 4, b, -70, -80, -80, -100, b, -115, -100, -160, -100, -160, -60, -160, 20, -160, -155", 's');
    list_append(generatedFont, (unitype) "ν, 1, 3, -160, 20, -115, -100, -70, 20", 's');
    list_append(generatedFont, (unitype) "ξ, 1, 6, b, -85, 30, -100, 40, b, -120, 40, -160, 40, b, -160, 5, -160, -30, b, -110, -30, -160, -30, b, -160, -65, -160, -100, b, -110, -100, -70, -100, -100, -135", 's');
    list_append(generatedFont, (unitype) "ο, 1, 4, b, -115, -100, -70, -100, b, -70, -40, -70, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, -115, -100", 's');
    list_append(generatedFont, (unitype) "π, 3, 2, -145, -100, -145, 20, 2, -160, 20, -60, 20, 2, -75, 20, b, -75, -80, -75, -105, -60, -98", 's');
    list_append(generatedFont, (unitype) "ρ, 1, 5, -160, -160, b, -160, 0, -150, 20, b, -115, 20, -70, 20, b, -70, -40, -70, -100, b, -115, -100, -150, -100, -160, -80", 's');
    list_append(generatedFont, (unitype) "σ, 1, 5, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -70, -100, b, -70, -40, -70, 20, -115, 20, -65, 20", 's');
    list_append(generatedFont, (unitype) "τ, 2, 2, -160, 20, -70, 20, 2, -115, 20, b, -115, -80, -115, -110, -85, -95", 's');
    list_append(generatedFont, (unitype) "υ, 1, 4, -160, 20, b, -160, -65, -160, -100, b, -120, -100, -80, -100, b, -80, -50, -80, 0, -100, 20", 's');
    list_append(generatedFont, (unitype) "φ, 1, 6, -100, -160, b, -100, -10, -100, 25, b, -70, 10, -40, -10, b, -40, -45, -40, -100, b, -100, -100, -160, -100, b, -160, -45, -160, 5, -130, 15", 's');
    list_append(generatedFont, (unitype) "χ, 2, 2, b, -160, 10, -150, 10, -145, 0, b, -80, -150, -75, -160, -65, -160, 2, -160, -160, -75, 10", 's');
    list_append(generatedFont, (unitype) "ψ, 2, 2, -100, -160, -100, 20, 4, -160, 20, b, -160, -40, -160, -100, b, -100, -100, -40, -100, b, -40, -40, -40, 0, -50, 20", 's');
    list_append(generatedFont, (unitype) "ω, 1, 6, b, -145, 20, -160, -5, b, -160, -40, -160, -100, b, -120, -100, -80, -100, b, -80, -30, -80, -100, b, -40, -100, 0, -100, b, 0, -40, 0, -5, -15, 20", 's');
    list_append(generatedFont, (unitype) "ς, 1, 4, b, -70, -10, -80, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -110, -100, -70, -100, -100, -135", 's');
    list_append(generatedFont, (unitype) "1, 1, 3, -160, 35, -120, 60, -120, -100", 's');
    list_append(generatedFont, (unitype) "2, 1, 4, -65, -100, b, -160, -100, -70, -15, b, -70, 25, -70, 60, b, -110, 60, -150, 60, -160, 25", 's');
    list_append(generatedFont, (unitype) "3, 1, 6, b, -160, 30, -150, 60, b, -115, 60, -70, 60, b, -70, 20, -70, -20, b, -120, -20, -70, -20, b, -70, -60, -70, -100, b, -115, -100, -150, -100, -160, -70", 's');
    list_append(generatedFont, (unitype) "4, 1, 4, -85, -100, -85, 60, -160, -60, -65, -60", 's');
    list_append(generatedFont, (unitype) "5, 1, 6, -75, 60, -150, 60, b, -160, -30, -140, -5, b, -115, -5, -70, -5, b, -70, -55, -70, -100, b, -115, -100, -150, -100, -160, -75", 's');
    list_append(generatedFont, (unitype) "6, 1, 5, b, -100, 60, -160, 40, b, -160, -55, -160, -100, b, -118, -100, -75, -100, b, -75, -55, -75, -10, b, -118, -10, -160, -10, -160, -55", 's');
    list_append(generatedFont, (unitype) "7, 1, 3, -160, 60, -70, 60, -130, -100", 's');
    list_append(generatedFont, (unitype) "8, 1, 8, b, -118, -15, -75, -15, b, -75, -60, -75, -100, b, -117, -100, -160, -100, b, -160, -60, -160, -15, b, -118, -15, -80, -15, b, -80, 20, -80, 60, b, -117, 60, -155, 60, b, -155, 20, -155, -15, -118, -15", 's');
    list_append(generatedFont, (unitype) "9, 1, 5, b, -140, -100, -75, -90, b, -75, 15, -75, 60, b, -118, 60, -160, 60, b, -160, 15, -160, -30, b, -118, -30, -75, -30, -75, 15", 's');
    list_append(generatedFont, (unitype) "0, 1, 4, b, -112, 60, -160, 60, b, -160, -20, -160, -100, b, -112, -100, -65, -100, b, -65, -20, -65, 60, -112, 60", 's');
    list_append(generatedFont, (unitype) "!, 2, 2, -160, 60, -160, -50, 1, -160, -100", 's');
    list_append(generatedFont, (unitype) "@, 1, 10, b, -35, -140, -55, -150, b, -80, -150, -160, -150, b, -160, -55, -160, 50, b, -80, 50, 0, 50, b, 0, -55, 0, -100, b, -30, -100, -50, -100, -55, -80, b, -50, -10, -75, 15, b, -100, -5, -120, -25, b, -120, -60, -120, -100, b, -90, -100, -70, -100, -55, -80", 's');
    list_append(generatedFont, (unitype) "#, 4, 2, -130, -100, -100, 50, 2, -90, -100, -60, 50, 2, -140, 0, -40, 0, 2, -150, -50, -50, -50", 's');
    list_append(generatedFont, (unitype) "$, 3, 6, b, -80, 35, -90, 60, b, -120, 60, -160, 60, b, -160, 20, -160, -20, b, -120, -20, -80, -20, b, -80, -60, -80, -100, b, -120, -100, -150, -100, -160, -75, 2, -120, 80, -120, 60, 2, -120, -120, -120, -100", 's');
    list_append(generatedFont, (unitype) "£, 2, 4, b, -55, 30, -60, 60, b, -100, 60, -145, 60, -145, 5, b, -140, -75, -140, -100, -160, -100, -50, -100, 2, -160, -30, -100, -30", 's');
    list_append(generatedFont, (unitype) "€, 3, 4, b, -60, 50, -70, 60, b, -90, 60, -140, 60, b, -140, -20, -140, -100, b, -90, -100, -70, -100, -60, -90, 2, -160, -5, -80, -5, 2, -160, -35, -80, -35", 's');
    list_append(generatedFont, (unitype) "₺, 3, 2, -130, 70, b, -130, -100, -60, -100, -60, -10, 2, -160, 10, -90, 50, 2, -160, -30, -90, 10", 's');
    list_append(generatedFont, (unitype) "₽, 2, 5, -140, -100, -140, 60, b, -80, 60, -40, 60, b, -40, 20, -40, -20, -80, -20, -160, -20, 2, -160, -60, -90, -60", 's');
    list_append(generatedFont, (unitype) "¥, 4, 3, -160, 60, -110, -30, -60, 60, 2, -110, -30, -110, -100, 2, -150, -30, -70, -30, 2, -150, -60, -70, -60", 's');
    list_append(generatedFont, (unitype) "₩, 3, 5, -160, 60, -120, -100, -80, 60, -40, -100, 0, 60, 2, -160, 0, 0, 0, 2, -160, -40, 0, -40", 's');
    list_append(generatedFont, (unitype) "₹, 2, 6, -60, 60, -160, 60, b, -120, 60, -80, 60, b, -80, 20, -80, -20, -120, -20, -160, -20, -80, -100, 2, -160, 20, -60, 20", 's');
    list_append(generatedFont, (unitype) "₣, 3, 3, -160, -100, -160, 60, -70, 60, 2, -160, -20, -80, -20, 2, -180, -60, -120, -60", 's');
    list_append(generatedFont, (unitype) "฿, 3, 7, -160, -100, -160, 60, b, -120, 60, -70, 60, b, -70, 20, -70, -20, b, -120, -20, -70, -20, b, -70, -60, -70, -100, -120, -100, -160, -100, 2, -120, -20, -160, -20, 2, -120, 80, -120, -120", 's');
    list_append(generatedFont, (unitype) "%, 3, 2, -140, -85, -60, 40, 4, b, -135, -10, -110, -10, b, -110, 20, -110, 50, b, -135, 50, -160, 50, b, -160, 20, -160, -10, -135, -10, 4, b, -65, -40, -90, -40, b, -90, -70, -90, -100, b, -65, -100, -40, -100, b, -40, -70, -40, -40, -65, -40", 's');
    list_append(generatedFont, (unitype) "^, 1, 3, -160, 30, -140, 60, -120, 30", 's');
    list_append(generatedFont, (unitype) "&, 1, 8, -40, -100, b, -140, 10, -160, 30, b, -160, 45, -160, 80, b, -120, 80, -85, 80, b, -85, 45, -85, 20, b, -110, 5, -160, -25, b, -160, -55, -160, -100, b, -110, -100, -50, -100, -50, -40", 's');
    list_append(generatedFont, (unitype) "*, 3, 3, -120, 60, -120, 25, -89, 37, 3, -100, -5, -120, 25, -140, -5, 2, -151, 37, -120, 25", 's');
    list_append(generatedFont, (unitype) "(, 1, 2, b, -120, 70, -160, 50, b, -160, -25, -160, -100, -120, -120", 's');
    list_append(generatedFont, (unitype) "), 1, 2, b, -160, 70, -120, 50, b, -120, -25, -120, -100, -160, -120", 's');
    list_append(generatedFont, (unitype) "`, 1, 2, -140, 30, -160, 60", 's');
    list_append(generatedFont, (unitype) "~, 1, 4, b, -160, -35, -160, -5, b, -132, -5, -115, -5, b, -105, -20, -95, -35, b, -78, -35, -50, -35, -50, -5", 's');
    list_append(generatedFont, (unitype) "-, 1, 2, -160, -20, -115, -20", 's');
    list_append(generatedFont, (unitype) "_, 1, 2, -160, -100, -70, -100", 's');
    list_append(generatedFont, (unitype) "=, 2, 2, -160, -40, -80, -40, 2, -160, 0, -80, 0", 's');
    list_append(generatedFont, (unitype) "+, 2, 2, -160, -20, -60, -20, 2, -110, -70, -110, 30", 's');
    list_append(generatedFont, (unitype) "[, 1, 4, -120, 70, -160, 70, -160, -120, -120, -120", 's');
    list_append(generatedFont, (unitype) "{, 1, 4, b, -120, 70, -150, 70, b, -140, 10, -135, -10, b, -160, -25, -135, -40, b, -140, -60, -150, -120, -120, -120", 's');
    list_append(generatedFont, (unitype) "], 1, 4, -160, 70, -120, 70, -120, -120, -160, -120", 's');
    list_append(generatedFont, (unitype) "}, 1, 4, b, -160, 70, -130, 70, b, -140, 10, -145, -10, b, -120, -25, -145, -40, b, -140, -60, -130, -120, -160, -120", 's');
    list_append(generatedFont, (unitype) "\\, 1, 2, -80, -110, -160, 60", 's');
    list_append(generatedFont, (unitype) "|, 1, 2, -160, 50, -160, -120", 's');
    list_append(generatedFont, (unitype) ";, 2, 1, -160, 10, 1, b, -160, -90, -160, -110, -170, -115", 's');
    list_append(generatedFont, (unitype) ":, 2, 1, -160, 10, 1, -160, -90", 's');
    list_append(generatedFont, (unitype) "‘, 1, 1, b, -150, 30, -160, 40, -160, 60", 's');
    list_append(generatedFont, (unitype) "', 1, 2, -160, 60, -160, 30", 's');
    list_append(generatedFont, (unitype) "’, 1, 1, b, -160, 30, -150, 40, -150, 60", 's');
    list_append(generatedFont, (unitype) "“, 2, 1, b, -150, 30, -160, 40, -160, 60, 1, b, -125, 30, -135, 40, -135, 60", 's');
    list_append(generatedFont, (unitype) "\", 2, 2, -160, 60, -160, 30, 2, -135, 60, -135, 30", 's');
    list_append(generatedFont, (unitype) "”, 2, 1, b, -160, 30, -150, 40, -150, 60, 1, b, -135, 30, -125, 40, -125, 60", 's');
    list_append(generatedFont, (unitype) ",, 1, 1, b, -160, -90, -160, -110, -170, -115", 's');
    list_append(generatedFont, (unitype) "<, 1, 3, -80, 25, -160, -20, -80, -65", 's');
    list_append(generatedFont, (unitype) "., 1, 1, -160, -100", 's');
    list_append(generatedFont, (unitype) ">, 1, 3, -160, 25, -80, -20, -160, -65", 's');
    list_append(generatedFont, (unitype) "/, 1, 2, -160, -110, -80, 60", 's');
    list_append(generatedFont, (unitype) "?, 2, 4, b, -160, 25, -155, 60, b, -120, 60, -80, 60, b, -80, 25, -80, 0, b, -100, -10, -120, -20, -120, -50, 1, -120, -100", 's');
    list_append(generatedFont, (unitype) "½, 3, 2, -140, -85, -60, 40, 3, -160, 35, -135, 50, -135, -10, 4, b, -80, -55, -75, -35, b, -60, -35, -40, -35, b, -40, -55, -40, -65, -80, -100, -35, -100", 's');
    list_append(generatedFont, (unitype) "¨, 2, 1, -160, 50, 1, -130, 50", 's');
}

#endif /* TURTLE_TEXT_IMPLEMENTATION */
#ifdef TURTLE_TOOLS_IMPLEMENTATION
/*
████████╗██╗   ██╗██████╗ ████████╗██╗     ███████╗████████╗ ██████╗  ██████╗ ██╗     ███████╗    ██████╗
╚══██╔══╝██║   ██║██╔══██╗╚══██╔══╝██║     ██╔════╝╚══██╔══╝██╔═══██╗██╔═══██╗██║     ██╔════╝   ██╔════╝
   ██║   ██║   ██║██████╔╝   ██║   ██║     █████╗     ██║   ██║   ██║██║   ██║██║     ███████╗   ██║     
   ██║   ██║   ██║██╔══██╗   ██║   ██║     ██╔══╝     ██║   ██║   ██║██║   ██║██║     ╚════██║   ██║     
   ██║   ╚██████╔╝██║  ██║   ██║   ███████╗███████╗   ██║   ╚██████╔╝╚██████╔╝███████╗███████║██╗╚██████╗
   ╚═╝    ╚═════╝ ╚═╝  ╚═╝   ╚═╝   ╚══════╝╚══════╝   ╚═╝    ╚═════╝  ╚═════╝ ╚══════╝╚══════╝╚═╝ ╚═════╝
https://patorjk.com/software/taag/#p=display&f=ANSI%20Shadow

turtleTools library includes:
ribbon: customisable top bar
popup
button
switch
dial
slider
scrollbar
dropdown
text box (under development)

TODO:
using the tab key to select different elements? And allowing them to be changed with the keyboard??
*/


int randomInt(int lowerBound, int upperBound) { // random integer between lower and upper bound (inclusive)
    return (rand() % (upperBound - lowerBound + 1) + lowerBound);
}

double randomDouble(double lowerBound, double upperBound) { // random double between lower and upper bound
    return (rand() * (upperBound - lowerBound) / RAND_MAX + lowerBound); // probably works idk
}

/* insert a string to an index in to string */
char *strins(char *dest, char *source, int32_t index) {
    int32_t lenDest = strlen(dest);
    int32_t lenSource = strlen(source);
    memmove(dest + index + lenSource, dest + index, lenDest - index + 1);
    memcpy(dest + index, source, lenSource);
    return dest;
}

/* delete a section of a string */
char *strdel(char *dest, int32_t index, int32_t size) {
    int32_t len = strlen(dest);
    memmove(dest + index, dest + index + size, len - index - size);
    for (uint32_t i = 0; i < size + 1; i++) {
        dest[len - size + i] = '\0';
    }
    return dest;
}

tt_theme_name_t tt_theme;
tt_enabled_t tt_enabled; // all start at 0 (global variable)

tt_elements_t tt_elements;

/* default colours (light theme) */
double tt_themeColors[] = {
    0.0, 0.0, 0.0,       // text color (0)
    0.0, 0.0, 0.0,       // text color alternate (3)
    200.0, 200.0, 200.0, // ribbon top bar color (6)
    140.0, 140.0, 140.0, // ribbon dropdown color (9)
    160.0, 160.0, 160.0, // ribbon select color (12)
    100.0, 100.0, 100.0, // ribbon hover color (15)
    200.0, 200.0, 200.0, // popup box color (18)
    140.0, 140.0, 140.0, // popup boxes color (21)
    100.0, 100.0, 100.0, // popup boxes select color (24)
    200.0, 200.0, 200.0, // button color (27)
    160.0, 160.0, 160.0, // button select color (30)
    180.0, 180.0, 180.0, // button clicked color (33)
    30.0, 30.0, 30.0,    // switch text hover (36)
    180.0, 180.0, 180.0, // switch color off (39)
    230.0, 230.0, 230.0, // switch circle color off (42)
    30.0, 30.0, 30.0,    // switch color on (45)
    0.0, 255.0, 255.0,   // switch circle color on (48)
    0.0, 0.0, 0.0,       // dial color (51)
    255.0, 255.0, 255.0, // dial inner circle color (54)
    120.0, 120.0, 120.0, // slider bar color (57)
    230.0, 230.0, 230.0, // slider circle color (60)
    200.0, 200.0, 200.0, // scrollbar bar base color (63)
    160.0, 160.0, 160.0, // scrollbar bar color (66)
    120.0, 120.0, 120.0, // scrollbar bar hover color (69)
    120.0, 120.0, 120.0, // scrollbar bar clicked color (72)
    160.0, 160.0, 160.0, // dropdown color (75)
    120.0, 120.0, 120.0, // dropdown select color (78)
    120.0, 120.0, 120.0, // dropdown hover color (81)
    100.0, 100.0, 100.0, // dropdown triangle color (84)
    200.0, 200.0, 200.0, // textbox color (87)
    180.0, 180.0, 180.0, // textbox phantom text color (90)
    0.0, 0.0, 0.0,       // textbox line color (93)
    11.0, 87.0, 208.0,   // textbox select color (96)
};

void tt_setColor(int32_t index) {
    turtlePenColor(tt_themeColors[index], tt_themeColors[index + 1], tt_themeColors[index + 2]);
}

void turtleToolsSetTheme(tt_theme_name_t theme) {
    if (theme == TT_THEME_DARK) {
        tt_theme = theme;
        double tt_themeCopy[] = {
            200.0, 200.0, 200.0, // text color (0)
            160.0, 160.0, 160.0, // text color alternate (3)
            70.0, 70.0, 70.0,    // ribbon top bar color (6)
            80.0, 80.0, 80.0,    // ribbon dropdown color (9)
            60.0, 60.0, 60.0,    // ribbon select color (12)
            70.0, 70.0, 70.0,    // ribbon hover color (15)
            10.0, 10.0, 10.0,    // popup box color (18)
            40.0, 40.0, 40.0,    // popup boxes color (21)
            60.0, 60.0, 60.0,    // popup boxes select color (24)
            80.0, 80.0, 80.0,    // button color (27)
            100.0, 100.0, 100.0, // button select color (30)
            90.0, 90.0, 90.0,    // button clicked color (33)
            160.0, 160.0, 160.0, // switch text hover (36)
            60.0, 60.0, 60.0,    // switch color off (39)
            80.0, 80.0, 80.0,    // switch circle color off (42)
            10.0, 10.0, 10.0,    // switch color on (45)
            50.0, 230.0, 30.0,   // switch circle color on (48)
            200.0, 200.0, 200.0, // dial color (51)
            30.0, 30.0, 30.0,    // dial inner circle color (54)
            10.0, 10.0, 10.0,    // slider bar color (57)
            230.0, 230.0, 230.0, // slider circle color (60)
            50.0, 50.0, 50.0,    // scrollbar bar base color (63)
            150.0, 150.0, 150.0, // scrollbar bar color (66)
            170.0, 170.0, 170.0, // scrollbar bar hover color (69)
            190.0, 190.0, 190.0, // scrollbar bar clicked color (72)
            60.0, 60.0, 60.0,    // dropdown color (75)
            80.0, 80.0, 80.0,    // dropdown select color (78)
            80.0, 80.0, 80.0,    // dropdown hover color (81)
            160.0, 160.0, 160.0, // dropdown triangle color (84)
            60.0, 60.0, 60.0,    // textbox color (87)
            80.0, 80.0, 80.0,    // textbox phantom text color (90)
            160.0, 160.0, 160.0, // textbox line color (93)
            11.0, 87.0, 208.0,   // textbox select color (96)
        };
        memcpy(tt_themeColors, tt_themeCopy, sizeof(tt_themeCopy));
    } else if (theme == TT_THEME_COLT) {
        tt_theme = theme;
        double tt_themeCopy[] = {
            189.0, 200.0, 203.0, // text color (0)
            154.0, 160.0, 160.0, // text color alternate (3)
            84.0, 73.0, 70.0,    // ribbon top bar color (6)
            93.0, 80.0, 80.0,    // ribbon dropdown color (9)
            69.0, 55.0, 53.0,    // ribbon select color (12)
            77.0, 70.0, 70.0,    // ribbon hover color (15)
            9.0, 10.0, 10.0,     // popup box color (18)
            52.0, 40.0, 40.0,    // popup boxes color (21)
            73.0, 60.0, 60.0,    // popup boxes select color (24)
            89.0, 80.0, 80.0,    // button color (27)
            111.0, 109.0, 100.0, // button select color (30)
            95.0, 93.0, 84.0,    // button clicked color (33)
            154.0, 160.0, 160.0, // switch text hover (36)
            68.0, 60.0, 60.0,    // switch color off (39)
            91.0, 80.0, 80.0,    // switch circle color off (42)
            69.0, 55.0, 53.0,    // switch color on (45)
            242.0, 242.0, 230.0, // switch circle color on (48)
            211.0, 211.0, 200.0, // dial color (51)
            36.0, 30.0, 32.0,    // dial inner circle color (54)
            69.0, 55.0, 53.0,    // slider bar color (57)
            242.0, 242.0, 230.0, // slider circle color (60)
            241.0, 239.0, 236.0, // scrollbar bar base color (63)
            69.0, 55.0, 53.0,    // scrollbar bar color (66)
            84.0, 70.0, 68.0,    // scrollbar bar hover color (69)
            84.0, 70.0, 68.0,    // scrollbar bar clicked color (72)
            68.0, 60.0, 60.0,    // dropdown color (75)
            92.0, 80.0, 80.0,    // dropdown select color (78)
            93.0, 80.0, 80.0,    // dropdown hover color (81)
            175.0, 171.0, 160.0, // dropdown triangle color (84)
            89.0, 80.0, 80.0,    // textbox color (87)
            143.0, 134.0, 134.0, // textbox phantom text color (90)
            154.0, 160.0, 160.0, // textbox line color (93)
            11.0, 87.0, 208.0,   // textbox select color (96)
        };
        memcpy(tt_themeColors, tt_themeCopy, sizeof(tt_themeCopy));
    } else if (theme == TT_THEME_NAVY) {
        tt_theme = theme;
        double tt_themeCopy[] = {
            3.0, 3.0, 3.0,       // text color (0)
            241.0, 239.0, 236.0, // text color alternate (3)
            21.2, 115.6, 160.2,  // ribbon top bar color (6)
            74.0, 108.0, 144.0,  // ribbon dropdown color (9)
            112.0, 146.0, 182.0, // ribbon select color (12)
            112.0, 146.0, 182.0, // ribbon hover color (15)
            18.0, 52.0, 88.0,    // popup box color (18)
            74.0, 108.0, 144.0,  // popup boxes color (21)
            126.0, 160.0, 196.0, // popup boxes select color (24)
            18.0, 52.0, 88.0,    // button color (27)
            74.0, 108.0, 144.0,  // button select color (30)
            47.0, 81.0, 117.0,   // button clicked color (33)
            18.0, 52.0, 88.0,    // switch text hover (36)
            18.0, 52.0, 88.0,    // switch color off (39)
            241.0, 239.0, 236.0, // switch circle color off (42)
            18.0, 52.0, 88.0,    // switch color on (45)
            241.0, 239.0, 236.0, // switch circle color on (48)
            3.0, 3.0, 3.0,       // dial color (51)
            212.0, 201.0, 190.0, // dial inner circle color (54)
            18.0, 52.0, 88.0,    // slider bar color (57)
            241.0, 239.0, 236.0, // slider circle color (60)
            241.0, 239.0, 236.0, // scrollbar bar base color (63)
            33.6, 161.9, 211.6,  // scrollbar bar color (66)
            50.0, 178.3, 228.0,  // scrollbar bar hover color (69)
            50.0, 178.3, 228.0,  // scrollbar bar clicked color (72)
            18.0, 52.0, 88.0,    // dropdown color (75)
            74.0, 108.0, 144.0,  // dropdown select color (78)
            74.0, 108.0, 144.0,  // dropdown hover color (81)
            241.0, 239.0, 236.0, // dropdown triangle color (84)
            18.0, 52.0, 88.0,    // textbox color (87)
            112.0, 146.0, 182.0, // textbox phantom text color (90)
            241.0, 239.0, 236.0, // textbox line color (93)
            11.0, 87.0, 208.0,   // textbox select color (96)
        };
        memcpy(tt_themeColors, tt_themeCopy, sizeof(tt_themeCopy));
    } else {
        tt_theme = TT_THEME_LIGHT;
        double tt_themeCopy[] = {
            0.0, 0.0, 0.0,       // text color (0)
            0.0, 0.0, 0.0,       // text color alternate (3)
            200.0, 200.0, 200.0, // ribbon top bar color (6)
            140.0, 140.0, 140.0, // ribbon dropdown color (9)
            160.0, 160.0, 160.0, // ribbon select color (12)
            100.0, 100.0, 100.0, // ribbon hover color (15)
            200.0, 200.0, 200.0, // popup box color (18)
            140.0, 140.0, 140.0, // popup boxes color (21)
            100.0, 100.0, 100.0, // popup boxes select color (24)
            200.0, 200.0, 200.0, // button color (27)
            160.0, 160.0, 160.0, // button select color (30)
            180.0, 180.0, 180.0, // button clicked color (33)
            30.0, 30.0, 30.0,    // switch text hover (36)
            180.0, 180.0, 180.0, // switch color off (39)
            230.0, 230.0, 230.0, // switch circle color off (42)
            30.0, 30.0, 30.0,    // switch color on (45)
            0.0, 255.0, 255.0,   // switch circle color on (48)
            0.0, 0.0, 0.0,       // dial color (51)
            255.0, 255.0, 255.0, // dial inner circle color (54)
            120.0, 120.0, 120.0, // slider bar color (57)
            230.0, 230.0, 230.0, // slider circle color (60)
            200.0, 200.0, 200.0, // scrollbar bar base color (63)
            160.0, 160.0, 160.0, // scrollbar bar color (66)
            120.0, 120.0, 120.0, // scrollbar bar hover color (69)
            120.0, 120.0, 120.0, // scrollbar bar clicked color (72)
            160.0, 160.0, 160.0, // dropdown color (75)
            120.0, 120.0, 120.0, // dropdown select color (78)
            120.0, 120.0, 120.0, // dropdown hover color (81)
            100.0, 100.0, 100.0, // dropdown triangle color (84)
            200.0, 200.0, 200.0, // textbox color (87)
            180.0, 180.0, 180.0, // textbox phantom text color (90)
            0.0, 0.0, 0.0,       // textbox line color (93)
            11.0, 87.0, 208.0,   // textbox select color (96)
        };
        memcpy(tt_themeColors, tt_themeCopy, sizeof(tt_themeCopy));
    }
}

/* ribbon */

/* ribbon variables */

tt_ribbon_t ribbonRender;

/* initialise ribbon */
int32_t ribbonInit(const char *filename) {
    /* enable ribbon */
    tt_enabled.ribbonEnabled = 1;
    if (tt_enabled.turtleToolsEnabled == 0) {
        tt_enabled.turtleToolsEnabled = 1;
        tt_elements.all = list_init();
    }
    /* set ribbon parameters */
    ribbonRender.marginSize = 10; // number of pixels between different items in the ribbon (not affected by ribbonSize)
    ribbonRender.mainselect[0] = -1;
    ribbonRender.mainselect[1] = -1;
    ribbonRender.mainselect[2] = -1;
    ribbonRender.subselect[0] = -1;
    ribbonRender.subselect[1] = -1;
    ribbonRender.subselect[2] = -1;
    ribbonRender.output[0] = 0;
    ribbonRender.output[1] = -1;
    ribbonRender.output[2] = -1;

    ribbonRender.mouseDown = 0;

    ribbonRender.bounds[0] = turtle.bounds[0];
    ribbonRender.bounds[1] = turtle.bounds[1];
    ribbonRender.bounds[2] = turtle.bounds[2];
    ribbonRender.bounds[3] = turtle.bounds[3];

    ribbonRender.ribbonSize = 1; // 1 is default, below 1 is smaller, above 1 is larger (scales as a multiplier, 0.1 is 100x smaller than 10)
    ribbonRender.options = list_init();
    ribbonRender.lengths = list_init();

    /* load from config file */
    char fileExists = 1;
    list_t *defaultRibbonFile = list_init();
    list_append(defaultRibbonFile, (unitype) "File, New, Save, Save As..., Open", 's');
    list_append(defaultRibbonFile, (unitype) "Edit, Undo, Redo, Cut, Copy, Paste", 's');
    list_append(defaultRibbonFile, (unitype) "View, Change Theme, GLFW", 's');
    FILE *configFile = fopen(filename, "r");
    if (configFile == NULL) {
        printf("Error: file %s not found\n", filename);
        fileExists = 0;
    }
    /* load ribbon options */
    char line[1024]; // maximum size of any list of options
    while ((fileExists == 0 && ribbonRender.options -> length < defaultRibbonFile -> length) || (fileExists == 1 && fgets(line, 1024, configFile) != NULL)) {
        if (fileExists == 0) {
            memcpy(line, defaultRibbonFile -> data[ribbonRender.options -> length].s, strlen(defaultRibbonFile -> data[ribbonRender.options -> length].s) + 1);
        }
        if (line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0'; // cull newline
        }
        list_append(ribbonRender.options, (unitype) list_init(), 'r');
        char *item = strtok(line, ",");
        while (item != NULL) {
            if (item[0] == ' ') {
                item++; // cull leading space
            }
            list_append(ribbonRender.options -> data[ribbonRender.options -> length - 1].r, (unitype) item, 's');
            item = strtok(NULL, ",");
        }
    }
    if (fileExists) {
        fclose(configFile);
    }

    for (uint32_t i = 0; i < ribbonRender.options -> length; i++) {
        list_append(ribbonRender.lengths, (unitype) turtleTextGetStringLength(ribbonRender.options -> data[i].r -> data[0].s, 7 * ribbonRender.ribbonSize), 'd');
        double max = 0;
        for (uint32_t j = 1; j < ribbonRender.options -> data[i].r -> length; j++) {
            double current = turtleTextGetStringLength(ribbonRender.options -> data[i].r -> data[j].s, 7 * ribbonRender.ribbonSize);
            if (current > max) {
                max = current;
            }
        }
        list_append(ribbonRender.lengths, (unitype) max, 'd');
    }
    return 0;
}

/* render ribbon */
void ribbonUpdate() {
    char shapeSave = turtle.penshape;
    double sizeSave = turtle.pensize;
    turtlePenSize(20);
    turtlePenShape("square");
    turtleGetMouseCoords(); // get the mouse coordinates (turtle.mouseX, turtle.mouseY)
    tt_setColor(TT_COLOR_RIBBON_TOP);
    turtleQuad(ribbonRender.bounds[0], ribbonRender.bounds[3] - 10, ribbonRender.bounds[2], ribbonRender.bounds[3] - 10, ribbonRender.bounds[2], ribbonRender.bounds[3], ribbonRender.bounds[0], ribbonRender.bounds[3]); // render ribbon
    tt_setColor(TT_COLOR_TEXT_ALTERNATE);
    double cutoff = ribbonRender.bounds[0] + ribbonRender.marginSize;
    ribbonRender.mainselect[0] = -1;
    ribbonRender.subselect[0] = -1;
    for (uint32_t i = 0; i < ribbonRender.options -> length; i++) {
        double prevCutoff = cutoff;
        if (i == (uint32_t) ribbonRender.mainselect[2]) {
            double xLeft = prevCutoff - ribbonRender.marginSize / 2.0;
            double xRight = prevCutoff + ribbonRender.lengths -> data[i * 2 + 1].d + ribbonRender.marginSize / 2.0;
            double yDown = ribbonRender.bounds[3] - 10 - 15 * (ribbonRender.options -> data[i].r -> length - 1) - ribbonRender.marginSize / 2.0;
            tt_setColor(TT_COLOR_RIBBON_DROPDOWN);
            turtleQuad(xLeft, ribbonRender.bounds[3] - 10, xRight, ribbonRender.bounds[3] - 10, xRight, yDown, xLeft, yDown); // ribbon highlight
            for (uint32_t j = 1; j < ribbonRender.options -> data[i].r -> length; j++) {
                if (turtle.mouseY > ribbonRender.bounds[3] - 10 - 15 * j - ribbonRender.marginSize / 4.0 && turtle.mouseY < ribbonRender.bounds[3] - 10 && turtle.mouseX > xLeft && turtle.mouseX < xRight && ribbonRender.subselect[0] == -1) {
                    tt_setColor(TT_COLOR_RIBBON_HOVER);
                    turtleQuad(xLeft, ribbonRender.bounds[3] - 10 - 15 * (j - 1) - ribbonRender.marginSize / 4.0, xRight, ribbonRender.bounds[3] - 10 - 15 * (j - 1) - ribbonRender.marginSize / 4.0, xRight, ribbonRender.bounds[3] - 10 - 15 * j - ribbonRender.marginSize / 3.0, xLeft, ribbonRender.bounds[3] - 10 - 15 * j - ribbonRender.marginSize / 3.0); // dropdown highlight
                    ribbonRender.subselect[0] = j;
                }
                tt_setColor(TT_COLOR_TEXT_ALTERNATE);
                turtleTextWriteUnicode((unsigned char *) ribbonRender.options -> data[i].r -> data[j].s, prevCutoff, 174.5 - j * 15, 7 * ribbonRender.ribbonSize, 0);
            }
        }
        cutoff += ribbonRender.lengths -> data[i * 2].d + ribbonRender.marginSize;
        if (turtle.mouseY > ribbonRender.bounds[3] - 10 && turtle.mouseY < ribbonRender.bounds[3] && turtle.mouseX > ribbonRender.bounds[0] + ribbonRender.marginSize / 2.0 && turtle.mouseX < cutoff - ribbonRender.marginSize / 2.0 && ribbonRender.mainselect[0] == -1) { // -217, -195, -164
            tt_setColor(TT_COLOR_RIBBON_SELECT);
            turtleQuad(prevCutoff - ribbonRender.marginSize / 2.0, 179, cutoff - ribbonRender.marginSize / 2.0, 179, cutoff - ribbonRender.marginSize / 2.0, 171, prevCutoff - ribbonRender.marginSize / 2.0, 171); // render dropdown
            ribbonRender.mainselect[0] = i;
        }
        tt_setColor(TT_COLOR_TEXT_ALTERNATE);
        turtleTextWriteUnicode((unsigned char *) ribbonRender.options -> data[i].r -> data[0].s, prevCutoff, 174.5, 7 * ribbonRender.ribbonSize, 0);
    }
    if (turtleMouseDown()) { // this is hideous
        if (ribbonRender.mouseDown == 0) {
            ribbonRender.mouseDown = 1;
            if (ribbonRender.subselect[0] == ribbonRender.subselect[1] && ribbonRender.subselect[0] != -1) {
                ribbonRender.subselect[2] = ribbonRender.subselect[0];
                ribbonRender.output[0] = 1;
                ribbonRender.output[1] = ribbonRender.mainselect[2];
                ribbonRender.output[2] = ribbonRender.subselect[2];
            }
            if (ribbonRender.mainselect[0] == ribbonRender.mainselect[1]) {
                if (ribbonRender.mainselect[0] == ribbonRender.mainselect[2]) {
                    ribbonRender.mainselect[3] = -1;
                } else {
                    ribbonRender.mainselect[2] = ribbonRender.mainselect[0];
                }
            }
        }
    } else {
        if (ribbonRender.mouseDown == 1) {
            if (ribbonRender.subselect[0] != -1) {
                ribbonRender.subselect[2] = ribbonRender.subselect[0];
                ribbonRender.output[0] = 1;
                ribbonRender.output[1] = ribbonRender.mainselect[2];
                ribbonRender.output[2] = ribbonRender.subselect[2];
                ribbonRender.mainselect[2] = -1;
                ribbonRender.subselect[2] = -1;
            }
        }
        if (ribbonRender.mainselect[3] == -1 && ribbonRender.mainselect[0] == ribbonRender.mainselect[2]) {
            ribbonRender.mainselect[2] = -1;
        }
        ribbonRender.mainselect[3] = 0;
        ribbonRender.mouseDown = 0;
        ribbonRender.mainselect[1] = ribbonRender.mainselect[0];
        ribbonRender.subselect[1] = ribbonRender.subselect[0];
    }
    turtle.penshape = shapeSave;
    turtle.pensize = sizeSave;
}

/* popup */

/* popup variables */

tt_popup_t popup;

/* initialise popup */
int32_t popupInit(const char *filename, double minX, double minY, double maxX, double maxY) {
    tt_enabled.popupEnabled = 1;
    if (tt_enabled.turtleToolsEnabled == 0) {
        tt_enabled.turtleToolsEnabled = 1;
        tt_elements.all = list_init();
    }
    popup.minX = minX;
    popup.minY = minY;
    popup.maxX = maxX;
    popup.maxY = maxY;
    popup.output[0] = 0;
    popup.output[1] = -1;
    popup.mouseDown = 0;
    popup.style = 0;
    /* read information from config file */
    char fileExists = 1;
    list_t *defaultPopupFile = list_init();
    list_append(defaultPopupFile, (unitype) "Are you sure you want to close?", 's');
    list_append(defaultPopupFile, (unitype) "Cancel", 's');
    list_append(defaultPopupFile, (unitype) "Close", 's');
    FILE *configFile = fopen(filename, "r");
    if (configFile == NULL) {
        printf("Error: file %s not found\n", filename);
        fileExists = 0;
    }
    char line[256] = {1, 0}; // maximum size of message or option
    /* read popup message */
    if (fileExists) {
        if (fgets(line, 256, configFile) != NULL) {
            if (line[strlen(line) - 1] == '\n') {
                line[strlen(line) - 1] = '\0'; // cull newline
            }
            popup.message = strdup(line);
        }
    } else {
        popup.message = strdup(defaultPopupFile -> data[0].s);
    }
    /* read popup options */
    popup.options = list_init();
    while ((fileExists == 0 && popup.options -> length < defaultPopupFile -> length - 1) || (fileExists == 1 && fgets(line, 256, configFile) != NULL)) {
        if (fileExists == 1) {
            if (line[strlen(line) - 1] == '\n') {
                line[strlen(line) - 1] = '\0'; // cull newline
            }
            list_append(popup.options, (unitype) strdup(line), 's');
        } else {
            list_append(popup.options, defaultPopupFile -> data[popup.options -> length + 1], 's');
        }
    }
    list_free(defaultPopupFile);
    return 0;
}

/* render popup */
void popupUpdate() {
    if (turtle.close == 1) {
        tt_setColor(TT_COLOR_POPUP_BOX);
        turtleRectangle(popup.minX, popup.minY, popup.maxX, popup.maxY);
        double textSize = 5;
        double textX = popup.minX + (popup.maxX - popup.minX) / 2;
        double textY = popup.maxY - textSize * 2;
        tt_setColor(TT_COLOR_TEXT_ALTERNATE);
        turtleTextWriteUnicode((unsigned char *) popup.message, textX, textY, textSize, 50);
        textY -= textSize * 4;
        double fullLength = 0;
        for (uint32_t i = 0; i < popup.options -> length; i++) {
            fullLength += turtleTextGetStringLength(popup.options -> data[i].s, textSize);
        }
        /* we have the length of the strings, now we pad with n + 1 padding regions */
        double padThai = (popup.maxX - popup.minX - fullLength) / (popup.options -> length + 1);
        textX = popup.minX + padThai;
        char flagged = 0;
        if (!turtleMouseDown() && popup.mouseDown == 1) {
            flagged = 1; // flagged for mouse misbehaviour
        }
        for (uint32_t i = 0; i < popup.options -> length; i++) {
            double strLen = turtleTextGetStringLength(popup.options -> data[i].s, textSize);
            if (turtle.mouseX > textX - textSize && turtle.mouseX < textX + strLen + textSize &&
            turtle.mouseY > textY - textSize && turtle.mouseY < textY + textSize) {
                tt_setColor(TT_COLOR_POPUP_BUTTON_SELECT);
                turtleRectangle(textX - textSize, textY - textSize, textX + textSize + strLen, textY + textSize);
                if (turtleMouseDown()) {
                    if (popup.mouseDown == 0) {
                        popup.mouseDown = 1;
                        if (popup.output[0] == 0) {
                            popup.output[1] = i;
                        }
                    }
                } else {
                    if (popup.mouseDown == 1) {
                        popup.mouseDown = 0;
                        if (popup.output[1] == (int32_t) i) {
                            popup.output[0] = 1;
                        }
                    }
                }
            } else {
                tt_setColor(TT_COLOR_POPUP_BUTTON);
                turtleRectangle(textX - textSize, textY - textSize, textX + textSize + strLen, textY + textSize);
            }
            tt_setColor(TT_COLOR_TEXT_ALTERNATE);
            turtleTextWriteUnicode((unsigned char *) popup.options -> data[i].s, textX, textY, textSize, 0);
            textX += strLen + padThai;
        }
        if (!turtleMouseDown() && popup.mouseDown == 1 && flagged == 1) {
            popup.mouseDown = 0;
            popup.output[0] = 0;
            popup.output[1] = -1;
        }
    }
}

void popupFree() {
    free(popup.message);
}

/* UI tools */

tt_globals_t tt_globals;

/* override colors with color array */
void tt_colorOverride(void *element, double *colors, uint32_t length) {
    ((tt_button_t *) element) -> color.colorOverride = 1;
    memcpy(((tt_button_t *) element) -> color.color, colors, length * sizeof(double));
}

int32_t tt_color_override_default[] = {
    /*  button                    switch                      dial                  slider                   scrollbar                   dropdown               textbox  */
    TT_COLOR_TEXT_ALTERNATE, TT_COLOR_TEXT,              TT_COLOR_TEXT,       TT_COLOR_TEXT,          0,                          TT_COLOR_TEXT,              TT_COLOR_TEXT_ALTERNATE,
    TT_COLOR_BUTTON,         TT_COLOR_SWITCH_TEXT_HOVER, TT_COLOR_DIAL,       TT_COLOR_SLIDER_BAR,    TT_COLOR_SCROLLBAR_BASE,    TT_COLOR_TEXT_ALTERNATE,    TT_COLOR_TEXTBOX_BOX,
    TT_COLOR_BUTTON_SELECT,  TT_COLOR_SWITCH_ON,         TT_COLOR_DIAL_INNER, TT_COLOR_SLIDER_CIRCLE, TT_COLOR_SCROLLBAR_HOVER,   TT_COLOR_DROPDOWN,          TT_COLOR_TEXTBOX_PHANTOM_TEXT,
    TT_COLOR_TEXT,           TT_COLOR_SWITCH_OFF,        0,                   0,                      TT_COLOR_SCROLLBAR_CLICKED, TT_COLOR_DROPDOWN_SELECT,   TT_COLOR_TEXTBOX_LINE,
    TT_COLOR_BUTTON_CLICKED, TT_COLOR_SWITCH_CIRCLE_ON,  0,                   0,                      TT_COLOR_SCROLLBAR_BAR,     TT_COLOR_DROPDOWN_HOVER,    TT_COLOR_TEXTBOX_SELECT,
    0,                       TT_COLOR_SWITCH_CIRCLE_OFF, 0,                   0,                      0,                          TT_COLOR_DROPDOWN_TRIANGLE, 0,
    0,                       0,                          0,                   0,                      0,                          0,                          0,
    0,                       0,                          0,                   0,                      0,                          0,                          0,
};

void elementResetColor(void *elementp, int32_t elementType) {
    for (uint32_t i = 0; i < 8; i += 3) {
        ((tt_button_t *) elementp) -> color.color[i] = tt_themeColors[tt_color_override_default[(i / 3) * TT_NUMBER_OF_ELEMENTS + elementType]];
        ((tt_button_t *) elementp) -> color.color[i + 1] = tt_themeColors[tt_color_override_default[(i / 3) * TT_NUMBER_OF_ELEMENTS + elementType] + 1];
        ((tt_button_t *) elementp) -> color.color[i + 2] = tt_themeColors[tt_color_override_default[(i / 3) * TT_NUMBER_OF_ELEMENTS + elementType] + 2];
    }
}

void tt_internalColor(void *elementp, int32_t colorIndex, int32_t overrideIndex) {
    if (((tt_button_t *) elementp) -> color.colorOverride) {
        turtlePenColor(((tt_button_t *) elementp) -> color.color[overrideIndex], ((tt_button_t *) elementp) -> color.color[overrideIndex + 1], ((tt_button_t *) elementp) -> color.color[overrideIndex + 2]);
    } else {
        turtlePenColor(tt_themeColors[colorIndex], tt_themeColors[colorIndex + 1], tt_themeColors[colorIndex + 2]);
    }
}

/* initialise UI elements */

/* create a button */
tt_button_t *buttonInit(char *label, int8_t *variable, double x, double y, double size) {
    if (tt_enabled.buttonEnabled == 0) {
        tt_enabled.buttonEnabled = 1;
        tt_elements.buttons = list_init();
    }
    if (tt_enabled.turtleToolsEnabled == 0) {
        tt_enabled.turtleToolsEnabled = 1;
        tt_elements.all = list_init();
    }
    tt_button_t *buttonp = malloc(sizeof(tt_button_t));
    buttonp -> element = TT_ELEMENT_BUTTON;
    buttonp -> enabled = TT_ELEMENT_ENABLED;
    if (label == NULL) {
        memcpy(buttonp -> label, "", strlen("") + 1);
    } else {
        memcpy(buttonp -> label, label, strlen(label) + 1);
    }
    buttonp -> color.colorOverride = 0;
    elementResetColor(buttonp, TT_ELEMENT_BUTTON);
    buttonp -> status = 0;
    buttonp -> shape = TT_BUTTON_SHAPE_RECTANGLE;
    buttonp -> x = x;
    buttonp -> y = y;
    buttonp -> size = size;
    *variable = 0; // button starts unpressed
    buttonp -> variable = variable;
    list_append(tt_elements.buttons, (unitype) (void *) buttonp, 'p');
    list_append(tt_elements.all, (unitype) (void *) buttonp, 'l');
    return buttonp;
}

void buttonFree(tt_button_t *buttonp) {
    list_remove(tt_elements.all, (unitype) (uint64_t) buttonp, 'l');
    list_remove(tt_elements.buttons, (unitype) (void *) buttonp, 'p');
}

/* create a switch */
tt_switch_t *switchInit(char *label, int8_t *variable, double x, double y, double size) {
    if (tt_enabled.switchEnabled == 0) {
        tt_enabled.switchEnabled = 1;
        tt_elements.switches = list_init();
    }
    if (tt_enabled.turtleToolsEnabled == 0) {
        tt_enabled.turtleToolsEnabled = 1;
        tt_elements.all = list_init();
    }
    tt_switch_t *switchp = malloc(sizeof(tt_switch_t));
    switchp -> element = TT_ELEMENT_SWITCH;
    switchp -> enabled = TT_ELEMENT_ENABLED;
    if (label == NULL) {
        memcpy(switchp -> label, "", strlen("") + 1);
    } else {
        memcpy(switchp -> label, label, strlen(label) + 1);
    }
    switchp -> color.colorOverride = 0;
    elementResetColor(switchp, TT_ELEMENT_SWITCH);
    switchp -> status = 0;
    switchp -> x = x;
    switchp -> y = y;
    switchp -> size = size;
    switchp -> variable = variable;
    switchp -> style = TT_SWITCH_STYLE_CLASSIC;
    list_append(tt_elements.switches, (unitype) (void *) switchp, 'p');
    list_append(tt_elements.all, (unitype) (void *) switchp, 'l');
    return switchp;
}

void switchFree(tt_switch_t *switchp) {
    list_remove(tt_elements.all, (unitype) (uint64_t) switchp, 'l');
    list_remove(tt_elements.switches, (unitype) (void *) switchp, 'p');
}

/* create a dial - make renderNumberFactor 0 to hide dial number */
tt_dial_t *dialInit(char *label, double *variable, tt_dial_type_t type, double x, double y, double size, double bottom, double top, double renderNumberFactor) {
    if (tt_enabled.dialEnabled == 0) {
        tt_enabled.dialEnabled = 1;
        tt_elements.dials = list_init();
    }
    if (tt_enabled.turtleToolsEnabled == 0) {
        tt_enabled.turtleToolsEnabled = 1;
        tt_elements.all = list_init();
    }
    tt_dial_t *dialp = malloc(sizeof(tt_dial_t));
    dialp -> element = TT_ELEMENT_DIAL;
    dialp -> enabled = TT_ELEMENT_ENABLED;
    if (label == NULL) {
        memcpy(dialp -> label, "", strlen("") + 1);
    } else {
        memcpy(dialp -> label, label, strlen(label) + 1);
    }
    dialp -> color.colorOverride = 0;
    elementResetColor(dialp, TT_ELEMENT_DIAL);
    dialp -> status[0] = 0;
    dialp -> type = type;
    dialp -> x = x;
    dialp -> y = y;
    dialp -> size = size;
    dialp -> range[0] = bottom;
    dialp -> range[1] = top;
    dialp -> variable = variable;
    dialp -> renderNumberFactor = renderNumberFactor;
    dialp -> defaultValue = *variable;
    list_append(tt_elements.dials, (unitype) (void *) dialp, 'p');
    list_append(tt_elements.all, (unitype) (void *) dialp, 'l');
    return dialp;
}

void dialFree(tt_dial_t *dialp) {
    list_remove(tt_elements.all, (unitype) (uint64_t) dialp, 'l');
    list_remove(tt_elements.dials, (unitype) (void *) dialp, 'p');
}

/* create a slider - make renderNumberFactor 0 to hide slider number */
tt_slider_t *sliderInit(char *label, double *variable, tt_slider_type_t type, tt_slider_align_t align, double x, double y, double size, double length, double bottom, double top, double renderNumberFactor) {
    if (tt_enabled.sliderEnabled == 0) {
        tt_enabled.sliderEnabled = 1;
        tt_elements.sliders = list_init();
    }
    if (tt_enabled.turtleToolsEnabled == 0) {
        tt_enabled.turtleToolsEnabled = 1;
        tt_elements.all = list_init();
    }
    tt_slider_t *sliderp = malloc(sizeof(tt_slider_t));
    sliderp -> element = TT_ELEMENT_SLIDER;
    sliderp -> enabled = TT_ELEMENT_ENABLED;
    if (label == NULL) {
        memcpy(sliderp -> label, "", strlen("") + 1);
    } else {
        memcpy(sliderp -> label, label, strlen(label) + 1);
    }
    sliderp -> color.colorOverride = 0;
    elementResetColor(sliderp, TT_ELEMENT_SLIDER);
    sliderp -> status = 0;
    sliderp -> type = type;
    sliderp -> align = align;
    sliderp -> x = x;
    sliderp -> y = y;
    sliderp -> size = size;
    sliderp -> length = length;
    sliderp -> range[0] = bottom;
    sliderp -> range[1] = top;
    sliderp -> variable = variable;
    sliderp -> renderNumberFactor = renderNumberFactor;
    sliderp -> defaultValue = *variable;
    list_append(tt_elements.sliders, (unitype) (void *) sliderp, 'p');
    list_append(tt_elements.all, (unitype) (void *) sliderp, 'l');
    return sliderp;
}

void sliderFree(tt_slider_t *sliderp) {
    list_remove(tt_elements.all, (unitype) (uint64_t) sliderp, 'l');
    list_remove(tt_elements.sliders, (unitype) (void *) sliderp, 'p');
}

/* create a scrollbar */
tt_scrollbar_t *scrollbarInit(double *variable, tt_scrollbar_type_t type, double x, double y, double size, double length, double barPercentage) {
    if (tt_enabled.scrollbarEnabled == 0) {
        tt_enabled.scrollbarEnabled = 1;
        tt_elements.scrollbars = list_init();
    }
    if (tt_enabled.turtleToolsEnabled == 0) {
        tt_enabled.turtleToolsEnabled = 1;
        tt_elements.all = list_init();
    }
    tt_scrollbar_t *scrollbarp = malloc(sizeof(tt_scrollbar_t));
    scrollbarp -> element = TT_ELEMENT_SCROLLBAR;
    scrollbarp -> enabled = TT_ELEMENT_ENABLED;
    scrollbarp -> color.colorOverride = 0;
    elementResetColor(scrollbarp, TT_ELEMENT_SCROLLBAR);
    scrollbarp -> status = 0;
    scrollbarp -> type = type;
    scrollbarp -> x = x;
    scrollbarp -> y = y;
    scrollbarp -> size = size;
    scrollbarp -> length = length;
    scrollbarp -> barPercentage = barPercentage;
    scrollbarp -> variable = variable;
    list_append(tt_elements.scrollbars, (unitype) (void *) scrollbarp, 'p');
    list_append(tt_elements.all, (unitype) (void *) scrollbarp, 'l');
    return scrollbarp;
}

void scrollbarFree(tt_scrollbar_t *scrollbarp) {
    list_remove(tt_elements.all, (unitype) (uint64_t) scrollbarp, 'l');
    list_remove(tt_elements.scrollbars, (unitype) (void *) scrollbarp, 'p');
}

void dropdownCalculateMax(tt_dropdown_t *dropdown) {
    dropdown -> maxXfactor = 0;
    for (uint32_t i = 0; i < dropdown -> options -> length; i++) {
        double stringLength = turtleTextGetStringLength(dropdown -> options -> data[i].s, dropdown -> size - 1);
        if (stringLength > dropdown -> maxXfactor) {
            dropdown -> maxXfactor = stringLength;
        }
    }
}

/* create a dropdown - use a list of strings for options */
tt_dropdown_t *dropdownInit(char *label, list_t *options, int32_t *variable, tt_dropdown_align_t align, double x, double y, double size) {
    if (tt_enabled.dropdownEnabled == 0) {
        tt_globals.dropdownLogicIndex = -1;
        tt_enabled.dropdownEnabled = 1;
        tt_elements.dropdowns = list_init();
    }
    if (tt_enabled.turtleToolsEnabled == 0) {
        tt_enabled.turtleToolsEnabled = 1;
        tt_elements.all = list_init();
    }
    tt_dropdown_t *dropdownp = malloc(sizeof(tt_dropdown_t));
    dropdownp -> element = TT_ELEMENT_DROPDOWN;
    dropdownp -> enabled = TT_ELEMENT_ENABLED;
    if (label == NULL) {
        memcpy(dropdownp -> label, "", strlen("") + 1);
    } else {
        memcpy(dropdownp -> label, label, strlen(label) + 1);
    }
    dropdownp -> color.colorOverride = 0;
    elementResetColor(dropdownp, TT_ELEMENT_DROPDOWN);
    dropdownp -> options = options;
    dropdownp -> index = *variable;
    dropdownp -> status = 0;
    dropdownp -> align = align;
    dropdownp -> x = x;
    dropdownp -> y = y;
    dropdownp -> size = size;
    dropdownp -> variable = variable;
    dropdownCalculateMax(dropdownp);
    list_append(tt_elements.dropdowns, (unitype) (void *) dropdownp, 'p');
    list_append(tt_elements.all, (unitype) (void *) dropdownp, 'l');
    return dropdownp;
}

void dropdownFree(tt_dropdown_t *dropdownp) {
    list_remove(tt_elements.all, (unitype) (uint64_t) dropdownp, 'l');
    list_remove(tt_elements.dropdowns, (unitype) (void *) dropdownp, 'p');
}

void textboxUnicodeCallback(uint32_t codepoint);
void textboxKeyCallback(int32_t key, int32_t scancode, int32_t action);

/* create a textbox */
tt_textbox_t *textboxInit(char *label, uint32_t maxCharacters, double x, double y, double size, double length) {
    if (tt_enabled.textboxEnabled == 0) {
        turtle.unicodeCallback = textboxUnicodeCallback;
        turtle.keyCallback = textboxKeyCallback;
        tt_enabled.textboxEnabled = 1;
        tt_elements.textboxes = list_init();
    }
    if (tt_enabled.turtleToolsEnabled == 0) {
        tt_enabled.turtleToolsEnabled = 1;
        tt_elements.all = list_init();
    }
    tt_textbox_t *textboxp = malloc(sizeof(tt_textbox_t));
    textboxp -> element = TT_ELEMENT_TEXTBOX;
    textboxp -> enabled = TT_ELEMENT_ENABLED;
    if (label == NULL) {
        memcpy(textboxp -> label, "", strlen("") + 1);
    } else {
        memcpy(textboxp -> label, label, strlen(label) + 1);
    }
    textboxp -> color.colorOverride = 0;
    elementResetColor(textboxp, TT_ELEMENT_TEXTBOX);
    textboxp -> status = 0;
    textboxp -> align = TT_TEXTBOX_ALIGN_LEFT;
    textboxp -> x = x;
    textboxp -> y = y;
    textboxp -> size = size;
    textboxp -> length = length;
    textboxp -> text = calloc(maxCharacters + 5, 1);
    textboxp -> maxCharacters = maxCharacters;
    textboxp -> editIndex = 0;
    textboxp -> lastKey = 0;
    textboxp -> keyTimeout = 0;
    textboxp -> renderPixelOffset = 0;
    textboxp -> renderStartingIndex = 0;
    textboxp -> renderNumCharacters = 0;
    list_append(tt_elements.textboxes, (unitype) (void *) textboxp, 'p');
    list_append(tt_elements.all, (unitype) (void *) textboxp, 'l');
    return textboxp;
}

void textboxFree(tt_textbox_t *textboxp) {
    list_remove(tt_elements.all, (unitype) (uint64_t) textboxp, 'l');
    list_remove(tt_elements.textboxes, (unitype) (void *) textboxp, 'p');
}

void buttonUpdate() {
    for (uint32_t i = 0; i < tt_elements.buttons -> length; i++) {
        tt_button_t *buttonp = (tt_button_t *) (tt_elements.buttons -> data[i].p);
        if (buttonp -> enabled == TT_ELEMENT_HIDE) {
            continue;
        }
        double buttonX = buttonp -> x;
        double buttonY = buttonp -> y;
        double buttonWidth = turtleTextGetUnicodeLength((unsigned char *) buttonp -> label, buttonp -> size) * 1.1;
        double buttonHeight = buttonp -> size * 1.75;
        if (buttonp -> status == 1) {
            tt_internalColor(buttonp, TT_COLOR_BUTTON_CLICKED, TT_COLOR_OVERRIDE_SLOT_4);
        } else if (buttonp -> status == 0) {
            tt_internalColor(buttonp, TT_COLOR_BUTTON, TT_COLOR_OVERRIDE_SLOT_1);
        } else {
            tt_internalColor(buttonp, TT_COLOR_BUTTON_SELECT, TT_COLOR_OVERRIDE_SLOT_2);
        }
        if (buttonp -> shape == TT_BUTTON_SHAPE_RECTANGLE) {
            turtleRectangle(buttonX - buttonWidth / 2, buttonY - buttonHeight / 2, buttonX + buttonWidth / 2, buttonY + buttonHeight / 2);
        } else if (buttonp -> shape == TT_BUTTON_SHAPE_ROUNDED_RECTANGLE) {
            turtlePenSize(buttonp -> size);
            turtleGoto(buttonX - buttonWidth / 2 + buttonp -> size / 2, buttonY - buttonHeight / 2 + buttonp -> size / 2);
            turtlePenDown();
            turtleGoto(buttonX + buttonWidth / 2 - buttonp -> size / 2, buttonY - buttonHeight / 2 + buttonp -> size / 2);
            turtleGoto(buttonX + buttonWidth / 2 - buttonp -> size / 2, buttonY + buttonHeight / 2 - buttonp -> size / 2);
            turtleGoto(buttonX - buttonWidth / 2 + buttonp -> size / 2, buttonY + buttonHeight / 2 - buttonp -> size / 2);
            turtleGoto(buttonX - buttonWidth / 2 + buttonp -> size / 2, buttonY - buttonHeight / 2 + buttonp -> size / 2);
            turtlePenUp();
            turtleRectangle(buttonX - buttonWidth / 4, buttonY - buttonHeight / 4, buttonX + buttonWidth / 4, buttonY + buttonHeight / 4);
        } else if (buttonp -> shape == TT_BUTTON_SHAPE_CIRCLE) {
            turtleGoto(buttonX, buttonY);
            turtlePenSize(buttonWidth);
            turtlePenDown();
            turtlePenUp();
        }
        tt_internalColor(buttonp, TT_COLOR_TEXT_ALTERNATE, TT_COLOR_OVERRIDE_SLOT_0);
        if (buttonp -> shape == TT_BUTTON_SHAPE_TEXT) {
            if (buttonp -> status == 0) {
                tt_internalColor(buttonp, TT_COLOR_TEXT, TT_COLOR_OVERRIDE_SLOT_3);
            } else {
                tt_internalColor(buttonp, TT_COLOR_BUTTON_SELECT, TT_COLOR_OVERRIDE_SLOT_2);
            }
        }
        turtleTextWriteUnicode((unsigned char *) buttonp -> label, buttonX, buttonY, buttonp -> size - 1, 50);
        /* mouse */
        if (buttonp -> enabled == TT_ELEMENT_ENABLED && ribbonRender.mainselect[2] == -1) {
            if (turtleMouseDown()) {
                if (buttonp -> status == 2) {
                    buttonp -> status = 1;
                }
                if (buttonp -> status == -1) {
                    buttonp -> status = 2;
                }
            } else {
                if (buttonp -> shape == TT_BUTTON_SHAPE_CIRCLE) {
                    if ((turtle.mouseX - buttonX) * (turtle.mouseX - buttonX) + (turtle.mouseY - buttonY) * (turtle.mouseY - buttonY) < buttonWidth * buttonWidth / 4) {
                        buttonp -> status = -1;
                    } else {
                        buttonp -> status = 0;
                    }
                } else {
                    if (turtle.mouseX > buttonX - buttonWidth / 2 && turtle.mouseX < buttonX + buttonWidth / 2 && turtle.mouseY > buttonY - buttonHeight / 2 && turtle.mouseY < buttonY + buttonHeight / 2) {
                        buttonp -> status = -1;
                    } else {
                        buttonp -> status = 0;
                    }
                }
            }
            if (buttonp -> status > 1) {
                *(buttonp -> variable) = 1;
                // buttonp -> status = 0;
            } else {
                if (buttonp -> status < 1) {
                    *(buttonp -> variable) = 0;
                }
            }
        }
    }
}

void switchUpdate() {
    for (uint32_t i = 0; i < tt_elements.switches -> length; i++) {
        tt_switch_t *switchp = (tt_switch_t *) (tt_elements.switches -> data[i].p);
        if (switchp -> enabled == TT_ELEMENT_HIDE) {
            continue;
        }
        double switchX = switchp -> x;
        double switchY = switchp -> y;
        double switchClickLeft = switchX;
        double switchClickRight = switchX;
        double switchClickDown = switchY;
        double switchClickUp = switchY;
        if (switchp -> style == TT_SWITCH_STYLE_CLASSIC || switchp -> style == TT_SWITCH_STYLE_SIDESWIPE_LEFT || switchp -> style == TT_SWITCH_STYLE_SIDESWIPE_RIGHT) {
            /* render switch */
            if (*(switchp -> variable)) {
                tt_internalColor(switchp, TT_COLOR_SWITCH_ON, TT_COLOR_OVERRIDE_SLOT_2);
            } else {
                tt_internalColor(switchp, TT_COLOR_SWITCH_OFF, TT_COLOR_OVERRIDE_SLOT_3);
            }
            turtlePenSize(switchp -> size * 1.2);
            turtleGoto(switchX - switchp -> size * 0.8, switchY);
            turtlePenDown();
            turtleGoto(switchX + switchp -> size * 0.8, switchY);
            turtlePenUp();
            turtlePenSize(switchp -> size);
            if (*(switchp -> variable)) {
                tt_internalColor(switchp, TT_COLOR_SWITCH_CIRCLE_ON, TT_COLOR_OVERRIDE_SLOT_4);
                turtleGoto(switchX + switchp -> size * 0.8, switchY);
            } else {
                tt_internalColor(switchp, TT_COLOR_SWITCH_CIRCLE_OFF, TT_COLOR_OVERRIDE_SLOT_5);
                turtleGoto(switchX - switchp -> size * 0.8, switchY);
            }
            turtlePenDown();
            turtlePenUp();
            /* mouse parameters */
            if (switchp -> style == TT_SWITCH_STYLE_CLASSIC) {
                switchClickLeft = switchX - switchp -> size * 1.35;
                switchClickRight = switchX + switchp -> size * 1.35;
            } else if (switchp -> style == TT_SWITCH_STYLE_SIDESWIPE_LEFT) {
                double textLength = turtleTextGetUnicodeLength((unsigned char *) switchp -> label, switchp -> size - 1);
                switchClickLeft = switchX - switchp -> size * 1.35;
                switchClickRight = switchX + switchp -> size * 2.2 + textLength;
            } else {
                double textLength = turtleTextGetUnicodeLength((unsigned char *) switchp -> label, switchp -> size - 1);
                switchClickLeft = switchX - switchp -> size * 2 - textLength;
                switchClickRight = switchX + switchp -> size * 1.35;
            }
            switchClickDown = switchY - switchp -> size * 0.6;
            switchClickUp = switchY + switchp -> size * 0.6;
            /* render text */
            tt_internalColor(switchp, TT_COLOR_TEXT, TT_COLOR_OVERRIDE_SLOT_0);
            if (switchp -> style == TT_SWITCH_STYLE_CLASSIC) {
                turtleTextWriteUnicode((unsigned char *) switchp -> label, switchX, switchY + 1.6 * switchp -> size, switchp -> size - 1, 50);
            } else if (switchp -> style == TT_SWITCH_STYLE_SIDESWIPE_LEFT) {
                if (switchp -> status == -1) {
                    tt_internalColor(switchp, TT_COLOR_SWITCH_TEXT_HOVER, TT_COLOR_OVERRIDE_SLOT_1);
                } else {
                    tt_internalColor(switchp, TT_COLOR_TEXT, TT_COLOR_OVERRIDE_SLOT_0);
                }
                turtleTextWriteUnicode((unsigned char *) switchp -> label, switchX + switchp -> size * 2, switchY, switchp -> size - 1, 0);
            } else {
                if (switchp -> status == -1) {
                    tt_internalColor(switchp, TT_COLOR_SWITCH_TEXT_HOVER, TT_COLOR_OVERRIDE_SLOT_1);
                } else {
                    tt_internalColor(switchp, TT_COLOR_TEXT, TT_COLOR_OVERRIDE_SLOT_0);
                }
                turtleTextWriteUnicode((unsigned char *) switchp -> label, switchX - switchp -> size * 2, switchY, switchp -> size - 1, 100);
            }
        } else if (switchp -> style == TT_SWITCH_STYLE_CHECKBOX || switchp -> style == TT_SWITCH_STYLE_XBOX) {
            /* render box */
            tt_internalColor(switchp, TT_COLOR_SWITCH_ON, TT_COLOR_OVERRIDE_SLOT_2);
            turtleRectangle(switchX - switchp -> size / 2, switchY - switchp -> size / 2, switchX + switchp -> size / 2, switchY + switchp -> size / 2);
            tt_internalColor(switchp, TT_COLOR_SWITCH_OFF, TT_COLOR_OVERRIDE_SLOT_3);
            turtlePenSize(switchp -> size / 6);
            turtleGoto(switchX - switchp -> size / 2, switchY - switchp -> size / 2);
            turtlePenDown();
            turtleGoto(switchX + switchp -> size / 2, switchY - switchp -> size / 2);
            turtleGoto(switchX + switchp -> size / 2, switchY + switchp -> size / 2);
            turtleGoto(switchX - switchp -> size / 2, switchY + switchp -> size / 2);
            turtleGoto(switchX - switchp -> size / 2, switchY - switchp -> size / 2);
            turtlePenUp();
            if (*(switchp -> variable)) {
                if (switchp -> style == TT_SWITCH_STYLE_CHECKBOX) {
                    /* render check */
                    tt_internalColor(switchp, TT_COLOR_SWITCH_CIRCLE_ON, TT_COLOR_OVERRIDE_SLOT_4);
                    turtleGoto(switchX - switchp -> size / 2.5, switchY);
                    turtlePenDown();
                    turtleGoto(switchX, switchY - switchp -> size / 2.5);
                    turtleGoto(switchX + switchp -> size / 2.5, switchY + switchp -> size / 2.5);
                    turtlePenUp();
                } else {
                    /* render X */
                    tt_internalColor(switchp, TT_COLOR_SWITCH_CIRCLE_ON, TT_COLOR_OVERRIDE_SLOT_4);
                    turtleGoto(switchX + switchp -> size / 2.5, switchY + switchp -> size / 2.5);
                    turtlePenDown();
                    turtleGoto(switchX - switchp -> size / 2.5, switchY - switchp -> size / 2.5);
                    turtlePenUp();
                    turtleGoto(switchX + switchp -> size / 2.5, switchY - switchp -> size / 2.5);
                    turtlePenDown();
                    turtleGoto(switchX - switchp -> size / 2.5, switchY + switchp -> size / 2.5);
                    turtlePenUp();
                }
            }
            /* mouse parameters - include text */
            double textLength = turtleTextGetUnicodeLength((unsigned char *) switchp -> label, switchp -> size - 1);
            switchClickLeft = switchX - switchp -> size * 0.6;
            switchClickRight = switchX + switchp -> size * 1.2 + textLength;
            switchClickDown = switchY - switchp -> size * 0.6;
            switchClickUp = switchY + switchp -> size * 0.6;
            /* render text */
            if (switchp -> status == -1) {
                tt_internalColor(switchp, TT_COLOR_SWITCH_TEXT_HOVER, TT_COLOR_OVERRIDE_SLOT_1);
            } else {
                tt_internalColor(switchp, TT_COLOR_TEXT, TT_COLOR_OVERRIDE_SLOT_0);
            }
            turtleTextWriteUnicode((unsigned char *) switchp -> label, switchX + switchp -> size, switchY, switchp -> size - 1, 0);
        }
        /* mouse */
        if (switchp -> enabled == TT_ELEMENT_ENABLED && ribbonRender.mainselect[2] == -1) {
            if (turtleMouseDown()) {
                if (switchp -> status < 0) {
                    switchp -> status *= -1;
                }
            } else {
                if (turtle.mouseX > switchClickLeft && turtle.mouseX < switchClickRight && turtle.mouseY > switchClickDown && turtle.mouseY < switchClickUp) {
                    switchp -> status = -1;
                } else {
                    switchp -> status = 0;
                }
            }
            if (switchp -> status > 0) {
                if (*(switchp -> variable)) {
                    *(switchp -> variable) = 0;
                } else {
                    *(switchp -> variable) = 1;
                }
                switchp -> status = 0;
            }
        }
    }
}

/* angle between two coordinates (in degrees) */
double angleBetween(double x1, double y1, double x2, double y2) {
    double output;
    if (y2 - y1 < 0) {
        output = 180 + atan((x2 - x1) / (y2 - y1)) * 57.2958;
    } else {
        output = atan((x2 - x1) / (y2 - y1)) * 57.2958;
    }
    if (output < 0) {
        output += 360;
    }
    return output;
}

void dialUpdate() {
    for (uint32_t i = 0; i < tt_elements.dials -> length; i++) {
        tt_dial_t *dialp = (tt_dial_t *) (tt_elements.dials -> data[i].p);
        if (dialp -> enabled == TT_ELEMENT_HIDE) {
            continue;
        }
        tt_internalColor(dialp, TT_COLOR_TEXT, TT_COLOR_OVERRIDE_SLOT_0);
        turtleTextWriteUnicode((unsigned char *) dialp -> label, dialp -> x, dialp -> y + 1.9 * dialp -> size, dialp -> size - 1, 50);
        turtlePenSize(dialp -> size * 2);
        double dialX = dialp -> x;
        double dialY = dialp -> y;
        turtleGoto(dialX, dialY);
        tt_internalColor(dialp, TT_COLOR_DIAL, TT_COLOR_OVERRIDE_SLOT_1);
        turtlePenDown();
        turtlePenUp();
        turtlePenSize(dialp -> size * 2 * 0.8);
        tt_internalColor(dialp, TT_COLOR_DIAL_INNER, TT_COLOR_OVERRIDE_SLOT_2);
        turtlePenDown();
        turtlePenUp();
        tt_internalColor(dialp, TT_COLOR_DIAL, TT_COLOR_OVERRIDE_SLOT_1);
        turtlePenSize(1);
        turtlePenDown();
        double dialAngle;
        if (dialp -> type == TT_DIAL_LOG) {
            dialAngle = pow(360, (*(dialp -> variable) - dialp -> range[0]) / (dialp -> range[1] - dialp -> range[0]));
        } else if (dialp -> type == TT_DIAL_LINEAR) {
            dialAngle = (*(dialp -> variable) - dialp -> range[0]) / (dialp -> range[1] - dialp -> range[0]) * 360;
        } else if (dialp -> type == TT_DIAL_EXP) {
            dialAngle = 360 * (log(((*(dialp -> variable) - dialp -> range[0]) / (dialp -> range[1] - dialp -> range[0])) * 360 + 1) / log(361));
        }
        turtleGoto(dialX + sin(dialAngle / 57.2958) * dialp -> size, dialY + cos(dialAngle / 57.2958) * dialp -> size);
        turtlePenUp();
        /* mouse */
        if (dialp -> enabled == TT_ELEMENT_ENABLED && ribbonRender.mainselect[2] == -1) {
            if (turtleMouseDown()) {
                if (dialp -> status[0] < 0) {
                    tt_globals.dialAnchorX = dialX;
                    tt_globals.dialAnchorY = dialY;
                    dialp -> status[0] *= -1;
                    dialp -> status[1] = turtle.mouseX - dialX;
                }
            } else if (turtleMouseRight()) {
                if (dialp -> status[0] < 0) {
                    *(dialp -> variable) = dialp -> defaultValue;
                }
            } else {
                if (turtle.mouseX > dialX - dialp -> size && turtle.mouseX < dialX + dialp -> size && turtle.mouseY > dialY - dialp -> size && turtle.mouseY < dialY + dialp -> size) {
                    dialp -> status[0] = -1;
                } else {
                    dialp -> status[0] = 0;
                }
            }
            if (dialp -> status[0] > 0) {
                dialAngle = angleBetween(tt_globals.dialAnchorX, tt_globals.dialAnchorY, turtle.mouseX, turtle.mouseY);
                if (turtle.mouseY < tt_globals.dialAnchorY) {
                    dialp -> status[1] = turtle.mouseX - dialX;
                }
                if ((dialAngle < 0.000000001 || dialAngle > 180) && turtle.mouseY > tt_globals.dialAnchorY && dialp -> status[1] >= 0) {
                    dialAngle = 0.000000001;
                }
                if ((dialAngle > 359.99999999 || dialAngle < 180) && turtle.mouseY > tt_globals.dialAnchorY && dialp -> status[1] < 0) {
                    dialAngle = 359.99999999;
                }
                if (dialp -> type == TT_DIAL_LOG) {
                    *(dialp -> variable) = round(dialp -> range[0] + (dialp -> range[1] - dialp -> range[0]) * (log(dialAngle) / log(360)));
                } else if (dialp -> type == TT_DIAL_LINEAR) {
                    *(dialp -> variable) = round(dialp -> range[0] + ((dialp -> range[1] - dialp -> range[0]) * dialAngle / 360));
                } else if (dialp -> type == TT_DIAL_EXP) {
                    *(dialp -> variable) = round(dialp -> range[0] + (dialp -> range[1] - dialp -> range[0]) * ((pow(361, dialAngle / 360) - 1) / 360));
                }
            }
        }
        tt_internalColor(dialp, TT_COLOR_TEXT, TT_COLOR_OVERRIDE_SLOT_0);
        double rounded = round(*(dialp -> variable) * dialp -> renderNumberFactor);
        turtleTextWriteStringf(dialX + dialp -> size + 3, dialY, 4, 0, "%.0lf", rounded);
    }
}

void sliderUpdate() {
    for (uint32_t i = 0; i < tt_elements.sliders -> length; i++) {
        tt_slider_t *sliderp = (tt_slider_t *) (tt_elements.sliders -> data[i].p);
        if (sliderp -> enabled == TT_ELEMENT_HIDE) {
            continue;
        }
        double sliderXLeft;
        double sliderYLeft;
        double sliderXRight;
        double sliderYRight;
        double sliderAlignFactor;
        double sliderOffsetXFactor = 0;
        double sliderOffsetYFactor = 0;
        double sliderOffsetXFactorSmall = 0;
        double sliderOffsetYFactorSmall = 0;
        if (sliderp -> type == TT_SLIDER_HORIZONTAL) {
            sliderOffsetYFactor = 1.4 * sliderp -> size;
            sliderOffsetYFactorSmall = -sliderp -> size * 1.2;
            if (sliderp -> align == TT_SLIDER_ALIGN_LEFT) {
                sliderXLeft = sliderp -> x;
                sliderYLeft = sliderp -> y;
                sliderXRight = sliderp -> x + sliderp -> length;
                sliderYRight = sliderp -> y;
                sliderAlignFactor = 0;
                sliderOffsetXFactor = -sliderp -> size * 0.4;
                sliderOffsetXFactorSmall = -sliderp -> size * 0.25;
            } else if (sliderp -> align == TT_SLIDER_ALIGN_CENTER) {
                sliderXLeft = sliderp -> x - sliderp -> length / 2;
                sliderYLeft = sliderp -> y;
                sliderXRight = sliderp -> x + sliderp -> length / 2;
                sliderYRight = sliderp -> y;
                sliderAlignFactor = 50;
            } else if (sliderp -> align == TT_SLIDER_ALIGN_RIGHT) {
                sliderXLeft = sliderp -> x - sliderp -> length;
                sliderYLeft = sliderp -> y;
                sliderXRight = sliderp -> x;
                sliderYRight = sliderp -> y;
                sliderAlignFactor = 100;
                sliderOffsetXFactor = sliderp -> size * 0.4;
                sliderOffsetXFactorSmall = sliderp -> size * 0.25;
            }
        } else if (sliderp -> type == TT_SLIDER_VERTICAL) {
            sliderOffsetYFactor = 1.4 * sliderp -> size + sliderp -> length / 2;
            if (sliderp -> align == TT_SLIDER_ALIGN_LEFT) {
                sliderXLeft = sliderp -> x;
                sliderYLeft = sliderp -> y - sliderp -> length / 2;
                sliderXRight = sliderp -> x;
                sliderYRight = sliderp -> y + sliderp -> length / 2;
                sliderAlignFactor = 0;
                sliderOffsetXFactor = -sliderp -> size * 0.4;
                sliderOffsetXFactorSmall = sliderp -> size * 1;
            } else if (sliderp -> align == TT_SLIDER_ALIGN_CENTER) {
                sliderXLeft = sliderp -> x;
                sliderYLeft = sliderp -> y - sliderp -> length / 2;
                sliderXRight = sliderp -> x;
                sliderYRight = sliderp -> y + sliderp -> length / 2;
                sliderAlignFactor = 50;
                sliderOffsetYFactorSmall = -1.2 * sliderp -> size - sliderp -> length / 2;
            } else if (sliderp -> align == TT_SLIDER_ALIGN_RIGHT) {
                sliderXLeft = sliderp -> x;
                sliderYLeft = sliderp -> y - sliderp -> length / 2;
                sliderXRight = sliderp -> x;
                sliderYRight = sliderp -> y + sliderp -> length / 2;
                sliderAlignFactor = 100;
                sliderOffsetXFactor = sliderp -> size * 0.4;
                sliderOffsetXFactorSmall = -sliderp -> size * 1;
            }
        }
        tt_internalColor(sliderp, TT_COLOR_TEXT, TT_COLOR_OVERRIDE_SLOT_0);
        turtleTextWriteUnicode((unsigned char *) sliderp -> label, sliderp -> x + sliderOffsetXFactor, sliderp -> y + sliderOffsetYFactor, sliderp -> size - 1, sliderAlignFactor);
        turtlePenSize(sliderp -> size * 1.2);
        turtleGoto(sliderXLeft, sliderYLeft);
        tt_internalColor(sliderp, TT_COLOR_SLIDER_BAR, TT_COLOR_OVERRIDE_SLOT_1);
        turtlePenDown();
        turtleGoto(sliderXRight, sliderYRight);
        turtlePenUp();
        turtlePenSize(sliderp -> size);
        tt_internalColor(sliderp, TT_COLOR_SLIDER_CIRCLE, TT_COLOR_OVERRIDE_SLOT_2);
        if (sliderp -> type == TT_SLIDER_HORIZONTAL) {
            turtleGoto(sliderXLeft + (sliderXRight - sliderXLeft) * (*(sliderp -> variable) - sliderp -> range[0]) / (sliderp -> range[1] - sliderp -> range[0]), sliderYLeft);
        } else if (sliderp -> type == TT_SLIDER_VERTICAL) {
            turtleGoto(sliderXLeft, sliderYLeft + (sliderYRight - sliderYLeft) * (*(sliderp -> variable) - sliderp -> range[0]) / (sliderp -> range[1] - sliderp -> range[0]));
        }
        turtlePenDown();
        turtlePenUp();
        /* mouse */
        if (sliderp -> enabled == TT_ELEMENT_ENABLED && ribbonRender.mainselect[2] == -1) {
            if (turtleMouseDown()) {
                if (sliderp -> status < 0) {
                    sliderp -> status *= -1;
                }
            } else if (turtleMouseRight()) {
                if (sliderp -> status < 0) {
                    *(sliderp -> variable) = sliderp -> defaultValue;
                }
            } else {
                if (turtle.mouseX > sliderXLeft - sliderp -> size * 0.6 && turtle.mouseX < sliderXRight + sliderp -> size * 0.6 && turtle.mouseY > sliderYLeft - sliderp -> size * 0.6 && turtle.mouseY < sliderYRight + sliderp -> size * 0.6) {
                    sliderp -> status = -1;
                } else {
                    sliderp -> status = 0;
                }
            }
            if (sliderp -> status > 0) {
                if (sliderp -> type == TT_SLIDER_HORIZONTAL) {
                    *(sliderp -> variable) = round(sliderp -> range[0] + (turtle.mouseX - sliderXLeft) / sliderp -> length * (sliderp -> range[1] - sliderp -> range[0]));
                } else if (sliderp -> type == TT_SLIDER_VERTICAL) {
                    *(sliderp -> variable) = round(sliderp -> range[0] + (turtle.mouseY - sliderYLeft) / sliderp -> length * (sliderp -> range[1] - sliderp -> range[0]));
                }
                if (*(sliderp -> variable) >= sliderp -> range[1]) {
                    *(sliderp -> variable) = sliderp -> range[1];
                }
                if (*(sliderp -> variable) <= sliderp -> range[0]) {
                    *(sliderp -> variable) = sliderp -> range[0];
                }
            }
        }
        if (sliderp -> renderNumberFactor != 0) {
            tt_internalColor(sliderp, TT_COLOR_TEXT, TT_COLOR_OVERRIDE_SLOT_0);
            double rounded = round(*(sliderp -> variable) * sliderp -> renderNumberFactor);
            turtleTextWriteStringf(sliderp -> x + sliderOffsetXFactorSmall, sliderp -> y + sliderOffsetYFactorSmall, 4, sliderAlignFactor, "%.0lf", rounded);
        }
    }
}

/*
scrollbar range of motion (coordinates):
scrollbar.length * (1 - scrollbar.barPercentage / 100)
tip: try to match the ratio of visible content to the scrollbar's barPercentage - if half of the content can be shown on one screen then make the barPercentage 50
*/
void scrollbarUpdate() {
    for (uint32_t i = 0; i < tt_elements.scrollbars -> length; i++) {
        tt_scrollbar_t *scrollbarp = (tt_scrollbar_t *) (tt_elements.scrollbars -> data[i].p);
        if (scrollbarp -> enabled == TT_ELEMENT_HIDE) {
            continue;
        }
        if (scrollbarp -> type == TT_SCROLLBAR_HORIZONTAL) {
            double scrollbarLeft = scrollbarp -> x - scrollbarp -> length / 2;
            double scrollbarRight = scrollbarp -> x + scrollbarp -> length / 2;
            double dragLeft = scrollbarLeft + (*(scrollbarp -> variable)) / 100 * (scrollbarp -> length * (1 - scrollbarp -> barPercentage / 100));
            double dragRight = dragLeft + (scrollbarp -> length * scrollbarp -> barPercentage / 100);
            turtlePenSize(scrollbarp -> size * 1);
            tt_internalColor(scrollbarp, TT_COLOR_SCROLLBAR_BASE, TT_COLOR_OVERRIDE_SLOT_1);
            turtleGoto(scrollbarLeft, scrollbarp -> y);
            turtlePenDown();
            turtleGoto(scrollbarRight, scrollbarp -> y);
            turtlePenUp();
            turtlePenSize(scrollbarp -> size * 0.8);
            if (scrollbarp -> status == -1 && turtle.mouseX > dragLeft - scrollbarp -> size * 0.4 && turtle.mouseX < dragRight + scrollbarp -> size * 0.4) {
                tt_internalColor(scrollbarp, TT_COLOR_SCROLLBAR_HOVER, TT_COLOR_OVERRIDE_SLOT_2);
            } else if (scrollbarp -> status > 0) {
                tt_internalColor(scrollbarp, TT_COLOR_SCROLLBAR_CLICKED, TT_COLOR_OVERRIDE_SLOT_3);
            } else {
                tt_internalColor(scrollbarp, TT_COLOR_SCROLLBAR_BAR, TT_COLOR_OVERRIDE_SLOT_4);
            }
            turtleGoto(dragLeft, scrollbarp -> y);
            turtlePenDown();
            turtleGoto(dragRight, scrollbarp -> y);
            turtlePenUp();
            if (scrollbarp -> enabled == TT_ELEMENT_ENABLED) {
                if (scrollbarp -> status == 2) {
                    tt_globals.barAnchor = turtle.mouseX - dragLeft;
                    scrollbarp -> status = 1;
                }
                if (turtleMouseDown()) {
                    if (scrollbarp -> status < 0) {
                        if (turtle.mouseX > dragLeft - scrollbarp -> size * 0.4 && turtle.mouseX < dragRight + scrollbarp -> size * 0.4) {
                            tt_globals.barAnchor = turtle.mouseX - dragLeft;
                        } else {
                            tt_globals.barAnchor = (scrollbarp -> length * scrollbarp -> barPercentage / 100) / 2;
                            scrollbarp -> status = -2;
                        }
                        scrollbarp -> status *= -1;
                    }
                } else {
                    if (turtle.mouseY > scrollbarp -> y - scrollbarp -> size * 0.5 && turtle.mouseY < scrollbarp -> y + scrollbarp -> size * 0.5 && turtle.mouseX < scrollbarRight && turtle.mouseX > scrollbarLeft) {
                        scrollbarp -> status = -1;
                    } else {
                        scrollbarp -> status = 0;
                    }
                }
                if (scrollbarp -> status > 0) {
                    *(scrollbarp -> variable) = (turtle.mouseX - scrollbarLeft - tt_globals.barAnchor) / (scrollbarp -> length * (1 - scrollbarp -> barPercentage / 100)) * 100;
                    if (*(scrollbarp -> variable) < 0) {
                        *(scrollbarp -> variable) = 0;
                    }
                    if (*(scrollbarp -> variable) > 100) {
                        *(scrollbarp -> variable) = 100;
                    }
                }
            }
        } else if (scrollbarp -> type == TT_SCROLLBAR_VERTICAL) {
            double scrollbarTop = scrollbarp -> y + scrollbarp -> length / 2;
            double scrollbarBottom = scrollbarp -> y - scrollbarp -> length / 2;
            double dragTop = scrollbarTop - (*(scrollbarp -> variable)) / 100 * (scrollbarp -> length * (1 - scrollbarp -> barPercentage / 100));
            double dragBottom = dragTop - (scrollbarp -> length * scrollbarp -> barPercentage / 100);
            turtlePenSize(scrollbarp -> size * 1);
            tt_internalColor(scrollbarp, TT_COLOR_SCROLLBAR_BASE, TT_COLOR_OVERRIDE_SLOT_1);
            turtleGoto(scrollbarp -> x, scrollbarTop);
            turtlePenDown();
            turtleGoto(scrollbarp -> x, scrollbarBottom);
            turtlePenUp();
            turtlePenSize(scrollbarp -> size * 0.8);
            if (scrollbarp -> status == -1 && turtle.mouseY > dragBottom - scrollbarp -> size * 0.4 && turtle.mouseY < dragTop + scrollbarp -> size * 0.4) {
                tt_internalColor(scrollbarp, TT_COLOR_SCROLLBAR_HOVER, TT_COLOR_OVERRIDE_SLOT_2);
            } else if (scrollbarp -> status > 0) {
                tt_internalColor(scrollbarp, TT_COLOR_SCROLLBAR_CLICKED, TT_COLOR_OVERRIDE_SLOT_3);
            } else {
                tt_internalColor(scrollbarp, TT_COLOR_SCROLLBAR_BAR, TT_COLOR_OVERRIDE_SLOT_4);
            }
            turtleGoto(scrollbarp -> x, dragTop);
            turtlePenDown();
            turtleGoto(scrollbarp -> x, dragBottom);
            turtlePenUp();
            /* mouse */
            if (scrollbarp -> enabled == TT_ELEMENT_ENABLED && ribbonRender.mainselect[2] == -1) {
                if (scrollbarp -> status == 2) {
                    tt_globals.barAnchor = dragTop - turtle.mouseY;
                    scrollbarp -> status = 1;
                }
                if (turtleMouseDown()) {
                    if (scrollbarp -> status < 0) {
                        if (turtle.mouseY > dragBottom - scrollbarp -> size * 0.4 && turtle.mouseY < dragTop + scrollbarp -> size * 0.4) {
                            tt_globals.barAnchor = dragTop - turtle.mouseY;
                        } else {
                            tt_globals.barAnchor = (scrollbarp -> length * scrollbarp -> barPercentage / 100) / 2;
                            scrollbarp -> status = -2;
                        }
                        scrollbarp -> status *= -1;
                    }
                } else {
                    if (turtle.mouseX > scrollbarp -> x - scrollbarp -> size * 0.5 && turtle.mouseX < scrollbarp -> x + scrollbarp -> size * 0.5 && turtle.mouseY > scrollbarBottom && turtle.mouseY < scrollbarTop) {
                        scrollbarp -> status = -1;
                    } else {
                        scrollbarp -> status = 0;
                    }
                }
                if (scrollbarp -> status > 0) {
                    *(scrollbarp -> variable) = (scrollbarTop - turtle.mouseY - tt_globals.barAnchor) / (scrollbarp -> length * (1 - scrollbarp -> barPercentage / 100)) * 100;
                    if (*(scrollbarp -> variable) < 0) {
                        *(scrollbarp -> variable) = 0;
                    }
                    if (*(scrollbarp -> variable) > 100) {
                        *(scrollbarp -> variable) = 100;
                    }
                }
            }
        }
    }
}

void dropdownUpdate() {
    int32_t logicIndex = -1;
    for (uint32_t i = 0; i < tt_elements.dropdowns -> length; i++) {
        tt_dropdown_t *dropdownp = (tt_dropdown_t *) (tt_elements.dropdowns -> data[i].p);
        if (dropdownp -> enabled == TT_ELEMENT_HIDE) {
            continue;
        }
        /* render dropdown default position */
        double dropdownX = dropdownp -> x;
        double dropdownY = dropdownp -> y;
        double xfactor = turtleTextGetUnicodeLength((unsigned char *) dropdownp -> options -> data[dropdownp -> index].s, dropdownp -> size - 1);
        double itemHeight = (dropdownp -> size * 1.5);
        double dropdownXFactor[2];
        double dropdownMaxXFactor[2];
        double dropdownAlignFactor;
        if (strlen(dropdownp -> label) > 0) {
            tt_internalColor(dropdownp, TT_COLOR_TEXT, TT_COLOR_OVERRIDE_SLOT_0);
            if (dropdownp -> align == TT_DROPDOWN_ALIGN_LEFT) {
                dropdownXFactor[0] = dropdownX;
                dropdownXFactor[1] = dropdownX + xfactor + dropdownp -> size;
                dropdownMaxXFactor[0] = dropdownX;
                dropdownMaxXFactor[1] = dropdownX + dropdownp -> maxXfactor + dropdownp -> size / 2.5;
                if (dropdownXFactor[1] > dropdownMaxXFactor[1]) {
                    dropdownMaxXFactor[1] = dropdownXFactor[1] + dropdownp -> size;
                }
                dropdownAlignFactor = 0;
                turtleTextWriteUnicode((unsigned char *) dropdownp -> label, dropdownX + dropdownp -> size / 5, dropdownY + 1.6 * dropdownp -> size, dropdownp -> size - 1, dropdownAlignFactor);
            } else if (dropdownp -> align == TT_DROPDOWN_ALIGN_CENTER) {
                dropdownXFactor[0] = dropdownX - xfactor / 2 - dropdownp -> size;
                dropdownXFactor[1] = dropdownX + xfactor / 2;
                dropdownMaxXFactor[0] = dropdownX - dropdownp -> maxXfactor / 2 - dropdownp -> size / 2;
                dropdownMaxXFactor[1] = dropdownX + dropdownp -> maxXfactor / 2 + dropdownp -> size / 2;
                if (dropdownXFactor[1] - dropdownXFactor[0] > dropdownMaxXFactor[1] - dropdownMaxXFactor[0] - dropdownp -> size) {
                    dropdownMaxXFactor[0] = dropdownXFactor[0];
                    dropdownMaxXFactor[1] = dropdownXFactor[1] + dropdownp -> size;
                }
                dropdownAlignFactor = 50;
                turtleTextWriteUnicode((unsigned char *) dropdownp -> label, dropdownX, dropdownY + 1.6 * dropdownp -> size, dropdownp -> size - 1, dropdownAlignFactor);
            } else if (dropdownp -> align == TT_DROPDOWN_ALIGN_RIGHT) {
                dropdownXFactor[0] = dropdownX - xfactor - dropdownp -> size * 2;
                dropdownXFactor[1] = dropdownX - dropdownp -> size;
                dropdownMaxXFactor[0] = dropdownX - dropdownp -> maxXfactor - dropdownp -> size * 2;
                dropdownMaxXFactor[1] = dropdownX;
                dropdownAlignFactor = 100;
                turtleTextWriteUnicode((unsigned char *) dropdownp -> label, dropdownX - dropdownp -> size / 5, dropdownY + 1.6 * dropdownp -> size, dropdownp -> size - 1, dropdownAlignFactor);
            }
        }
        logicIndex = tt_globals.dropdownLogicIndex;
        if ((int32_t) i > logicIndex && dropdownp -> status == -1) {
            tt_internalColor(dropdownp, TT_COLOR_DROPDOWN_SELECT, TT_COLOR_OVERRIDE_SLOT_3);
            turtleRectangle(dropdownXFactor[0], dropdownY - dropdownp -> size * 0.7, dropdownXFactor[1] + dropdownp -> size, dropdownY + dropdownp -> size * 0.7);
        } else if (dropdownp -> status >= 1) {
            tt_internalColor(dropdownp, TT_COLOR_DROPDOWN, TT_COLOR_OVERRIDE_SLOT_2);
            turtleRectangle(dropdownMaxXFactor[0], dropdownY - dropdownp -> size * 0.7 - (dropdownp -> options -> length - 1) * itemHeight, dropdownMaxXFactor[1], dropdownY + dropdownp -> size * 0.7);
        } else {
            tt_internalColor(dropdownp, TT_COLOR_DROPDOWN, TT_COLOR_OVERRIDE_SLOT_2);
            turtleRectangle(dropdownXFactor[0], dropdownY - dropdownp -> size * 0.7, dropdownXFactor[1] + dropdownp -> size, dropdownY + dropdownp -> size * 0.7);
        }
        /* mouse */
        if (dropdownp -> enabled == TT_ELEMENT_ENABLED && ribbonRender.mainselect[2] == -1) {
            if (turtle.mouseX > dropdownXFactor[0] && turtle.mouseX < dropdownXFactor[1] + dropdownp -> size && turtle.mouseY > dropdownY - dropdownp -> size * 0.7 && turtle.mouseY < dropdownY + dropdownp -> size * 0.7) {
                if (!turtleMouseDown() && dropdownp -> status == 0) {
                    dropdownp -> status = -1;
                    logicIndex = -1;
                }
            } else {
                if (dropdownp -> status == -1) {
                    dropdownp -> status = 0;
                    logicIndex = -1;
                }
            }
            if (dropdownp -> status == -1) {
                if ((int32_t) i > logicIndex && turtleMouseDown()) {
                    dropdownp -> status = 1;
                }
            }
            if (dropdownp -> status == 1) {
                if (!turtleMouseDown()) {
                    if (turtle.mouseX > dropdownMaxXFactor[0] && turtle.mouseX < dropdownMaxXFactor[1] && turtle.mouseY > dropdownY - dropdownp -> size * 0.7 - (dropdownp -> options -> length - 1) * itemHeight && turtle.mouseY < dropdownY + dropdownp -> size * 0.7 - itemHeight) {
                        uint32_t selected = round((dropdownY - turtle.mouseY) / itemHeight);
                        if (selected != 0) {
                            if (dropdownp -> index >= selected) {
                                dropdownp -> index = selected - 1;
                            } else {
                                dropdownp -> index = selected;
                            }
                            *dropdownp -> variable = dropdownp -> index;
                        }
                        dropdownp -> status = -2;
                    } else {
                        dropdownp -> status = 2;
                        logicIndex = -1;
                    }
                }
            }
            if (dropdownp -> status == -2) {
                if (!turtleMouseDown()) {
                    dropdownp -> status = 0;
                    logicIndex = -1;
                }
            }

            if (dropdownp -> status == 2 || dropdownp -> status == 1) {
                if (turtle.mouseX > dropdownMaxXFactor[0] && turtle.mouseX < dropdownMaxXFactor[1] && turtle.mouseY > dropdownY - dropdownp -> size * 0.7 - (dropdownp -> options -> length - 1) * itemHeight && turtle.mouseY < dropdownY + dropdownp -> size * 0.7) {
                    uint32_t selected = round((dropdownY - turtle.mouseY) / itemHeight);
                    tt_internalColor(dropdownp, TT_COLOR_DROPDOWN_HOVER, TT_COLOR_OVERRIDE_SLOT_4);
                    turtleRectangle(dropdownMaxXFactor[0], dropdownY - dropdownp -> size * 0.7 - selected * itemHeight, dropdownMaxXFactor[1], dropdownY + dropdownp -> size * 0.7 - selected * itemHeight);
                    if (turtleMouseDown() && dropdownp -> status == 2) {
                        if (selected != 0) {
                            if (dropdownp -> index >= selected) {
                                dropdownp -> index = selected - 1;
                            } else {
                                dropdownp -> index = selected;
                            }
                            *dropdownp -> variable = dropdownp -> index;
                        }
                        dropdownp -> status = -2;
                    }
                } else {
                    if (turtleMouseDown() && dropdownp -> status == 2) {
                        dropdownp -> status = 0;
                    }
                }
                tt_internalColor(dropdownp, TT_COLOR_TEXT_ALTERNATE, TT_COLOR_OVERRIDE_SLOT_1);
                int32_t renderIndex = 1;
                for (uint32_t i = 0; i < dropdownp -> options -> length; i++) {
                    if (i != dropdownp -> index) {
                        if (dropdownp -> align == TT_DROPDOWN_ALIGN_LEFT) {
                            turtleTextWriteUnicode((unsigned char *) dropdownp -> options -> data[i].s, dropdownMaxXFactor[0] + dropdownp -> size / 5, dropdownY - renderIndex * itemHeight, dropdownp -> size - 1, dropdownAlignFactor);
                        } else if (dropdownp -> align == TT_DROPDOWN_ALIGN_CENTER) {
                            turtleTextWriteUnicode((unsigned char *) dropdownp -> options -> data[i].s, (dropdownMaxXFactor[0] + dropdownMaxXFactor[1]) / 2, dropdownY - renderIndex * itemHeight, dropdownp -> size - 1, dropdownAlignFactor);
                        } else if (dropdownp -> align == TT_DROPDOWN_ALIGN_RIGHT) {
                            turtleTextWriteUnicode((unsigned char *) dropdownp -> options -> data[i].s, dropdownMaxXFactor[1] - dropdownp -> size * 1.6, dropdownY - renderIndex * itemHeight, dropdownp -> size - 1, dropdownAlignFactor);
                        }
                        renderIndex++;
                    }
                }
            }
            if (dropdownp -> status >= 1) {
                logicIndex = i;
            }
            tt_globals.dropdownLogicIndex = logicIndex;
        }
        tt_internalColor(dropdownp, TT_COLOR_TEXT_ALTERNATE, TT_COLOR_OVERRIDE_SLOT_1);
        if (dropdownp -> align == TT_DROPDOWN_ALIGN_LEFT) {
            turtleTextWriteUnicode((unsigned char *) dropdownp -> options -> data[dropdownp -> index].s, dropdownXFactor[0] + dropdownp -> size / 5, dropdownY, dropdownp -> size - 1, dropdownAlignFactor);
        } else if (dropdownp -> align == TT_DROPDOWN_ALIGN_CENTER) {
            turtleTextWriteUnicode((unsigned char *) dropdownp -> options -> data[dropdownp -> index].s, (dropdownXFactor[0] + dropdownXFactor[1]) / 2, dropdownY, dropdownp -> size - 1, dropdownAlignFactor);
        } else if (dropdownp -> align == TT_DROPDOWN_ALIGN_RIGHT) {
            turtleTextWriteUnicode((unsigned char *) dropdownp -> options -> data[dropdownp -> index].s, dropdownXFactor[1] - dropdownp -> size * 0.6, dropdownY, dropdownp -> size - 1, dropdownAlignFactor);
        }
        tt_internalColor(dropdownp, TT_COLOR_DROPDOWN_TRIANGLE, TT_COLOR_OVERRIDE_SLOT_5);
        if (dropdownp -> status >= 1) {
            turtleTriangle(dropdownXFactor[1] + dropdownp -> size * 0.4, dropdownY + dropdownp -> size * 0.4, dropdownXFactor[1] + dropdownp -> size * 0.4, dropdownY - dropdownp -> size * 0.4, dropdownXFactor[1] - dropdownp -> size * 0.2, dropdownY);
        } else {
            turtleTriangle(dropdownXFactor[1] + dropdownp -> size * 0.6, dropdownY + dropdownp -> size * 0.3, dropdownXFactor[1] - dropdownp -> size * 0.2, dropdownY + dropdownp -> size * 0.3, dropdownXFactor[1] + dropdownp -> size * 0.2, dropdownY - dropdownp -> size * 0.3);
        }
    }
}

void textboxAddKey(tt_textbox_t *textboxp, int32_t key) {
    int32_t len = strlen(textboxp -> text);
    /* https://stackoverflow.com/questions/42012563/convert-unicode-code-points-to-utf-8-and-utf-32 */
    uint32_t uKey = key;
    uint8_t buffer[5] = {0};
    int32_t size = 1;
    if (uKey <= 0x7F) {
        buffer[0] = uKey;
    } else if (uKey <= 0x7FF) {
        buffer[0] = 0xC0 | (uKey >> 6);            /* 110xxxxx */
        buffer[1] = 0x80 | (uKey & 0x3F);          /* 10xxxxxx */
        size = 2;
    } else if (uKey <= 0xFFFF) {
        buffer[0] = 0xE0 | (uKey >> 12);           /* 1110xxxx */
        buffer[1] = 0x80 | ((uKey >> 6) & 0x3F);   /* 10xxxxxx */
        buffer[2] = 0x80 | (uKey & 0x3F);          /* 10xxxxxx */
        size = 3;
    } else if (uKey <= 0x10FFFF) {
        buffer[0] = 0xF0 | (uKey >> 18);           /* 11110xxx */
        buffer[1] = 0x80 | ((uKey >> 12) & 0x3F);  /* 10xxxxxx */
        buffer[2] = 0x80 | ((uKey >> 6) & 0x3F);   /* 10xxxxxx */
        buffer[3] = 0x80 | (uKey & 0x3F);          /* 10xxxxxx */
        size = 4;
    }
    if (len < textboxp -> maxCharacters) {
        strins(textboxp -> text, (char *) buffer, textboxp -> editIndex);
        textboxp -> editIndex += size;
    }
}

void textboxUnicodeCallback(uint32_t codepoint) {
    for (uint32_t i = 0; i < tt_elements.textboxes -> length; i++) {
        tt_textbox_t *textboxp = (tt_textbox_t *) (tt_elements.textboxes -> data[i].p);
        if (textboxp -> status > 1) {
            textboxAddKey(textboxp, codepoint);
            break;
        }
    }
}

void textboxHandleOtherKey(tt_textbox_t *textboxp, int32_t key) {
    int32_t len = strlen(textboxp -> text);
    if (key == GLFW_KEY_BACKSPACE) {
        if (textboxp -> editIndex <= 0) {
            return;
        }
        int32_t size = 1;
        if (textboxp -> text[textboxp -> editIndex - 1] & 0b10000000) {
            while ((textboxp -> text[textboxp -> editIndex - 1] & 0b01000000) == 0) {
                textboxp -> editIndex--;
                size++;
            }
        }
        textboxp -> editIndex--;
        strdel(textboxp -> text, textboxp -> editIndex, size);
    } else if (key == GLFW_KEY_DELETE) {
        if (textboxp -> editIndex >= len) {
            return;
        }
        int32_t size = 1;
        if (textboxp -> text[textboxp -> editIndex] & 0b10000000) {
            if (textboxp -> text[textboxp -> editIndex] & 0b00100000) {
                if (textboxp -> text[textboxp -> editIndex] & 0b00010000) {
                    size++;
                }
                size++;
            }
            size++;
        }
        strdel(textboxp -> text, textboxp -> editIndex, size);
    } else if (key == GLFW_KEY_ENTER) {
        textboxp -> status = 0;
    } else if (key == GLFW_KEY_LEFT) {
        textboxp -> status = 2;
        if (textboxp -> editIndex <= 0) {
            return;
        }
        if (textboxp -> text[textboxp -> editIndex - 1] & 0b10000000) {
            while ((textboxp -> text[textboxp -> editIndex - 1] & 0b01000000) == 0) {
                textboxp -> editIndex--;
            }
        }
        textboxp -> editIndex--;
    } else if (key == GLFW_KEY_RIGHT) {
        textboxp -> status = 2;
        if (textboxp -> editIndex >= len) {
            return;
        }
        if (textboxp -> text[textboxp -> editIndex] & 0b10000000) {
            if (textboxp -> text[textboxp -> editIndex] & 0b00100000) {
                if (textboxp -> text[textboxp -> editIndex] & 0b00010000) {
                    textboxp -> editIndex++;
                }
                textboxp -> editIndex++;
            }
            textboxp -> editIndex++;
        }
        textboxp -> editIndex++;
    }
}

void textboxKeyCallback(int32_t key, int32_t scancode, int32_t action) {
    /* non-printable keys */
    if (action == GLFW_PRESS) {
        for (uint32_t i = 0; i < tt_elements.textboxes -> length; i++) {
            tt_textbox_t *textboxp = (tt_textbox_t *) (tt_elements.textboxes -> data[i].p);
            if (textboxp -> status > 1) {
                textboxp -> lastKey = key;
                textboxp -> keyTimeout = 50;
                textboxHandleOtherKey(textboxp, key);
                break;
            }
        }
    }
}

int32_t textboxCalculateMaximumCharacters(uint32_t *charlist, int32_t textLength, double size, double lengthPixels, int8_t sweepDirection, double *outputLength) {
    if (sweepDirection == -1) {
        /* sweep from the back to the front */
        size /= 175;
        double xTrack = 0;
        int32_t byteCount = 0;
        for (int32_t i = 0; i < textLength; i++) {
            int32_t currentDataAddress = 0;
            for (int32_t j = 0; j < turtleText.charCount; j++) {
                if (turtleText.supportedCharReference[j] == charlist[i]) {
                    currentDataAddress = j;
                    break;
                }
            }
            xTrack += (turtleText.fontData[turtleText.fontPointer[currentDataAddress + 1] - 4] + 40) * size;
            byteCount++;
            if (charlist[i] & 0x0000FF00) {
                byteCount++;
                if (charlist[i] & 0x00FF0000) {
                    byteCount++;
                    if (charlist[i] & 0xFF000000) {
                        byteCount++;
                    }
                }
            }
            if (xTrack - 40 * size > lengthPixels) {
                *outputLength = xTrack - 40 * size;
                return byteCount;
            }
        }
        *outputLength = xTrack - 40 * size;
        return byteCount;
    } else if (sweepDirection == 1) {
        /* sweep from the front to the back */
        size /= 175;
        double xTrack = 0;
        int32_t byteCount = 0;
        for (int32_t i = textLength - 1; i > 0; i--) {
            int32_t currentDataAddress = 0;
            for (int32_t j = 0; j < turtleText.charCount; j++) {
                if (turtleText.supportedCharReference[j] == charlist[i]) {
                    currentDataAddress = j;
                    break;
                }
            }
            xTrack += (turtleText.fontData[turtleText.fontPointer[currentDataAddress + 1] - 4] + 40) * size;
            byteCount--;
            if (charlist[i] & 0x0000FF00) {
                byteCount--;
                if (charlist[i] & 0x00FF0000) {
                    byteCount--;
                    if (charlist[i] & 0xFF000000) {
                        byteCount--;
                    }
                }
            }
            if (xTrack - 40 * size > lengthPixels) {
                *outputLength = xTrack - 40 * size;
                return byteCount;
            }
        }
        *outputLength = xTrack - 40 * size;
        return byteCount;
    }
    return 0;
}

void textboxUpdate() {
    for (uint32_t i = 0; i < tt_elements.textboxes -> length; i++) {
        tt_textbox_t *textboxp = (tt_textbox_t *) (tt_elements.textboxes -> data[i].p);
        if (textboxp -> enabled == TT_ELEMENT_HIDE) {
            continue;
        }

        /* handle keys */
        if (textboxp -> keyTimeout > 0) {
            textboxp -> keyTimeout--;
        }
        if (textboxp -> lastKey > 0) {
            if (turtleKeyPressed(textboxp -> lastKey)) {
                if (textboxp -> keyTimeout == 0) {
                    textboxp -> keyTimeout = 4;
                    textboxHandleOtherKey(textboxp, textboxp -> lastKey);
                }
            } else {
                textboxp -> lastKey = 0;
            }
        }
        if (textboxp -> status > 1) {
            textboxp -> status++;
            if (textboxp -> status > 128) {
                textboxp -> status = 2;
            }
        }

        tt_internalColor(textboxp, TT_COLOR_TEXTBOX_BOX, TT_COLOR_OVERRIDE_SLOT_1);
        turtleRectangle(textboxp -> x, textboxp -> y - textboxp -> size, textboxp -> x + textboxp -> length, textboxp -> y + textboxp -> size);

        if (textboxp -> status <= 0) {
            textboxp -> renderPixelOffset = textboxp -> size / 3;
            textboxp -> renderStartingIndex = 0;
            /* textbox idle */
            if (strlen(textboxp -> text) == 0) {
                /* render label */
                textboxp -> renderNumCharacters = 0;
                tt_internalColor(textboxp, TT_COLOR_TEXTBOX_PHANTOM_TEXT, TT_COLOR_OVERRIDE_SLOT_2);
                turtleTextWriteUnicode((unsigned char *) textboxp -> label, textboxp -> x + textboxp -> size / 2, textboxp -> y, textboxp -> size - 1, 0);
            } else {
                /* calculate rendered characters */
                double totalTextLength = turtleTextGetUnicodeLength((unsigned char *) textboxp -> text, textboxp -> size - 1);
                if (totalTextLength < textboxp -> length - textboxp -> size / 1.5) {
                    textboxp -> renderNumCharacters = strlen(textboxp -> text);
                } else {
                    /* not all characters fit in textbox - retract text length */
                    uint32_t textConverted[strlen(textboxp -> text) + 1];
                    uint32_t characterLength = turtleTextConvertUnicode((unsigned char *) textboxp -> text, textConverted);
                    double dummy;
                    textboxp -> renderNumCharacters = textboxCalculateMaximumCharacters(textConverted, characterLength, textboxp -> size - 1, textboxp -> length - textboxp -> size * 1.2, -1, &dummy);
                }
            }
        } else if (textboxp -> status > 0) {
            /* editing text */
            /* calculate rendered characters */
            double totalTextLength = turtleTextGetUnicodeLength((unsigned char *) textboxp -> text, textboxp -> size - 1);
            if (totalTextLength < textboxp -> length - textboxp -> size / 1.5) {
                textboxp -> renderStartingIndex = 0;
                textboxp -> renderPixelOffset = textboxp -> size / 3;
                textboxp -> renderNumCharacters = strlen(textboxp -> text);
            } else {
                /* not all characters fit in textbox - retract text length */
                if (textboxp -> editIndex < textboxp -> renderStartingIndex) {
                    /* set editIndex at the left side of box */
                    textboxp -> renderStartingIndex = textboxp -> editIndex;
                    textboxp -> renderPixelOffset = textboxp -> size / 3;
                    uint32_t textConverted[strlen(textboxp -> text) + 1];
                    uint32_t characterLength = turtleTextConvertUnicode((unsigned char *) (textboxp -> text + textboxp -> editIndex), textConverted);
                    double dummy;
                    textboxp -> renderNumCharacters = textboxCalculateMaximumCharacters(textConverted, characterLength, textboxp -> size - 1, textboxp -> length - textboxp -> size * 1.2, -1, &dummy);
                } else if (textboxp -> editIndex > textboxp -> renderStartingIndex + textboxp -> renderNumCharacters || (strlen(textboxp -> text) < textboxp -> renderStartingIndex + textboxp -> renderNumCharacters && strlen(textboxp -> text) == textboxp -> editIndex)) {
                    /* set editIndex at the right side of box */
                    char tempHold;
                    tempHold = textboxp -> text[textboxp -> editIndex];
                    textboxp -> text[textboxp -> editIndex] = '\0';
                    uint32_t textConverted[strlen(textboxp -> text) + 1];
                    uint32_t characterLength = turtleTextConvertUnicode((unsigned char *) textboxp -> text, textConverted);
                    double textPixelLength;
                    textboxp -> renderStartingIndex = strlen(textboxp -> text) + textboxCalculateMaximumCharacters(textConverted, characterLength, textboxp -> size - 1, textboxp -> length - textboxp -> size * 1.2, 1, &textPixelLength);
                    textboxp -> renderNumCharacters = strlen(textboxp -> text) - textboxp -> renderStartingIndex;
                    textboxp -> renderPixelOffset = textboxp -> length - textboxp -> size / 3 - textPixelLength;
                    textboxp -> text[textboxp -> editIndex] = tempHold;
                }
            }
        }

        /* draw text and occluding boxes */
        char tempHold;
        tempHold = textboxp -> text[textboxp -> renderStartingIndex + textboxp -> renderNumCharacters];
        textboxp -> text[textboxp -> renderStartingIndex + textboxp -> renderNumCharacters] = '\0';
        tt_internalColor(textboxp, TT_COLOR_TEXT_ALTERNATE, TT_COLOR_OVERRIDE_SLOT_0);
        turtleTextWriteUnicode((unsigned char *) (textboxp -> text + textboxp -> renderStartingIndex), textboxp -> x + textboxp -> renderPixelOffset, textboxp -> y, textboxp -> size - 1, 0);
        textboxp -> text[textboxp -> renderStartingIndex + textboxp -> renderNumCharacters] = tempHold;
        tt_internalColor(textboxp, TT_COLOR_TEXTBOX_BOX, TT_COLOR_OVERRIDE_SLOT_1);
        turtleRectangle(textboxp -> x, textboxp -> y - textboxp -> size, textboxp -> x + textboxp -> size / 4, textboxp -> y + textboxp -> size);
        turtleRectangle(textboxp -> x + textboxp -> length, textboxp -> y - textboxp -> size, textboxp -> x + textboxp -> length - textboxp -> size / 4, textboxp -> y + textboxp -> size);
        if (textboxp -> status > 0 && textboxp -> status < 66) {
            char tempHold = textboxp -> text[textboxp -> editIndex];
            textboxp -> text[textboxp -> editIndex] = '\0';
            double textLength = turtleTextGetUnicodeLength((unsigned char *) (textboxp -> text + textboxp -> renderStartingIndex), textboxp -> size - 1);
            textboxp -> text[textboxp -> editIndex] = tempHold;
            tt_internalColor(textboxp, TT_COLOR_TEXTBOX_LINE, TT_COLOR_OVERRIDE_SLOT_3);
            turtleRectangle(textboxp -> x + textboxp -> renderPixelOffset + textLength, textboxp -> y - textboxp -> size * 0.8, textboxp -> x + textboxp -> renderPixelOffset + textLength + 1, textboxp -> y + textboxp -> size * 0.8);
        }

        /* mouse */
        if (textboxp -> enabled == TT_ELEMENT_ENABLED && ribbonRender.mainselect[2] == -1) {
            if (turtleMouseDown()) {
                if (textboxp -> status < 0) {
                    textboxp -> editIndex = strlen(textboxp -> text);
                    textboxp -> status *= -1;
                }
                if (textboxp -> status > 1 && (turtle.mouseX < textboxp -> x || turtle.mouseX > textboxp -> x + textboxp -> length || turtle.mouseY < textboxp -> y - textboxp -> size || turtle.mouseY > textboxp -> y + textboxp -> size)) {
                    textboxp -> status = 0;
                }
            } else {
                if (textboxp -> status == 1) {
                    textboxp -> status = 2;
                } else if (textboxp -> status < 2) {
                    if (turtle.mouseX > textboxp -> x && turtle.mouseX < textboxp -> x + textboxp -> length && turtle.mouseY > textboxp -> y - textboxp -> size && turtle.mouseY < textboxp -> y + textboxp -> size) {
                        textboxp -> status = -1;
                    } else {
                        textboxp -> status = 0;
                    }
                }
            }
        }
    }
}

void turtleToolsUpdate() {
    char shapeSave = turtle.penshape;
    turtlePenShape("circle");
    if (tt_enabled.buttonEnabled) {
        buttonUpdate();
    }
    if (tt_enabled.switchEnabled) {
        switchUpdate();
    }
    if (tt_enabled.dialEnabled) {
        dialUpdate();
    }
    if (tt_enabled.sliderEnabled) {
        sliderUpdate();
    }
    if (tt_enabled.textboxEnabled) {
        textboxUpdate();
    }
    if (tt_enabled.dropdownEnabled) {
        dropdownUpdate();
    }
    if (tt_enabled.scrollbarEnabled) {
        scrollbarUpdate();
    }
    if (tt_enabled.ribbonEnabled) {
        ribbonUpdate();
    }
    if (tt_enabled.popupEnabled) {
        popupUpdate();
    }
    turtle.penshape = shapeSave;
}

void turtleToolsUpdateUI() {
    char shapeSave = turtle.penshape;
    turtlePenShape("circle");
    if (tt_enabled.buttonEnabled) {
        buttonUpdate();
    }
    if (tt_enabled.switchEnabled) {
        switchUpdate();
    }
    if (tt_enabled.dialEnabled) {
        dialUpdate();
    }
    if (tt_enabled.sliderEnabled) {
        sliderUpdate();
    }
    if (tt_enabled.textboxEnabled) {
        textboxUpdate();
    }
    if (tt_enabled.dropdownEnabled) {
        dropdownUpdate();
    }
    if (tt_enabled.scrollbarEnabled) {
        scrollbarUpdate();
    }
    turtle.penshape = shapeSave;
}

void turtleToolsUpdateRibbonPopup() {
    char shapeSave = turtle.penshape;
    turtlePenShape("circle");
    if (tt_enabled.ribbonEnabled) {
        ribbonUpdate();
    }
    if (tt_enabled.popupEnabled) {
        popupUpdate();
    }
    turtle.penshape = shapeSave;
}

#endif /* TURTLE_TOOLS_IMPLEMENTATION */
#ifdef OS_TOOLS_IMPLEMENTATION
/*
 ██████╗ ███████╗████████╗ ██████╗  ██████╗ ██╗     ███████╗    ██████╗
██╔═══██╗██╔════╝╚══██╔══╝██╔═══██╗██╔═══██╗██║     ██╔════╝   ██╔════╝
██║   ██║███████╗   ██║   ██║   ██║██║   ██║██║     ███████╗   ██║     
██║   ██║╚════██║   ██║   ██║   ██║██║   ██║██║     ╚════██║   ██║     
╚██████╔╝███████║   ██║   ╚██████╔╝╚██████╔╝███████╗███████║██╗╚██████╗
 ╚═════╝ ╚══════╝   ╚═╝    ╚═════╝  ╚═════╝ ╚══════╝╚══════╝╚═╝ ╚═════╝
https://patorjk.com/software/taag/#p=display&f=ANSI%20Shadow

Created by Ryan Srichai

a note on COM objects:
COM objects are C++ classes/structs
This means that it has methods
This is simulated in C via lpVtbl (vtable) which is an array of function pointers
Use the lpVtbl member of a struct to call methods via STRUCTNAME -> lpVtbl -> METHODNAME(args)
https://www.codeproject.com/Articles/13601/COM-in-plain-C

Under the hood, the pointer to the struct is also a pointer to an array of function pointers (lbVtbl)
The struct is therefore the size of the number of elements of the lbVtbl array * 8 plus the data in the struct which succeeds it

Whenever we call one of these methods, we have to pass in the object (which implicitly happens in OOP languages)
Actually, we pass in a pointer to the object, obviously i'm quite familiar with this

One more nuance is that whenever we pass a COM object in a function as an argument, it must always be &object
This is because in order to call methods we use object -> lpVtbl -> method, I mean we could use object.lpVtbl -> method but it's easier and allocates less stack memory to just use pointers

So whenever you take C++ COM object sample code, just follow this process:
change all the methods to -> lpVtbl -> methods
Add &obj as the first argument of every method
Change obj to &obj for all objects passed as arguments to functions or methods

That's it! (probably)

IFileDialog: https://learn.microsoft.com/en-us/windows/win32/api/shobjidl_core/nn-shobjidl_core-ifiledialog
Clipboard: https://learn.microsoft.com/en-us/windows/win32/dataxchg/clipboard
Cursor: https://learn.microsoft.com/en-us/windows/win32/learnwin32/setting-the-cursor-image
Sockets: https://learn.microsoft.com/en-us/windows/win32/winsock/tcp-ip-raw-sockets-2

idea: try glfwGetClipboardString and glfwSetClipboardString
*/


/* global objects */
osToolsGLFWObject osToolsGLFW;
osToolsClipboardObject osToolsClipboard;
osToolsFileDialogObject osToolsFileDialog;
osToolsMemmapObject osToolsMemmap;

/* OS independent functions */
void osToolsIndependentInit(GLFWwindow *window) {
    /* initialise glfw cursors */
    osToolsGLFW.osToolsWindow = window;
    osToolsGLFW.standardCursors[0] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    osToolsGLFW.standardCursors[1] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
    osToolsGLFW.standardCursors[2] = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
    osToolsGLFW.standardCursors[3] = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
    osToolsGLFW.standardCursors[4] = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
    osToolsGLFW.standardCursors[5] = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);

    /* initialise memmap module */
    osToolsMemmap.mappedFiles = list_init();
}

/* returns clipboard text */
const char *osToolsClipboardGetText() {
    osToolsClipboard.text = glfwGetClipboardString(osToolsGLFW.osToolsWindow);
    return osToolsClipboard.text;
}

/* takes null terminated strings */
int32_t osToolsClipboardSetText(const char *input) {
    glfwSetClipboardString(osToolsGLFW.osToolsWindow, input);
    return 0;
}

/*
GLFW_ARROW_CURSOR
GLFW_IBEAM_CURSOR
GLFW_CROSSHAIR_CURSOR
GLFW_HAND_CURSOR
GLFW_HRESIZE_CURSOR
GLFW_VRESIZE_CURSOR
*/
void osToolsSetCursor(uint32_t cursor) {
    switch (cursor) {
    case GLFW_ARROW_CURSOR:
        glfwSetCursor(osToolsGLFW.osToolsWindow, osToolsGLFW.standardCursors[0]);
    break;
    case GLFW_IBEAM_CURSOR:
        glfwSetCursor(osToolsGLFW.osToolsWindow, osToolsGLFW.standardCursors[1]);
    break;
    case GLFW_CROSSHAIR_CURSOR:
        glfwSetCursor(osToolsGLFW.osToolsWindow, osToolsGLFW.standardCursors[2]);
    break;
    case GLFW_HAND_CURSOR:
        glfwSetCursor(osToolsGLFW.osToolsWindow, osToolsGLFW.standardCursors[3]);
    break;
    case GLFW_HRESIZE_CURSOR:
        glfwSetCursor(osToolsGLFW.osToolsWindow, osToolsGLFW.standardCursors[4]);
    break;
    case GLFW_VRESIZE_CURSOR:
        glfwSetCursor(osToolsGLFW.osToolsWindow, osToolsGLFW.standardCursors[5]);
    break;
    default:
        glfwSetCursor(osToolsGLFW.osToolsWindow, osToolsGLFW.standardCursors[0]);
    break;
    }
}

void osToolsHideAndLockCursor() {
    glfwSetInputMode(osToolsGLFW.osToolsWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void osToolsShowCursor() {
    glfwSetInputMode(osToolsGLFW.osToolsWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

list_t *osToolsLoadInternal(char *filename, osToolsCSV rowOrColumn, char delimeter, osToolsCSV fieldType) {
    uint32_t fileSize;
    uint8_t *mappedFile = osToolsMapFile(filename, &fileSize);
    if (mappedFile == NULL) {
        return NULL;
    }
    list_t *outputList = list_init();
    /* process headers */
    list_append(outputList, (unitype) list_init(), 'r');
    uint32_t rightIndex = 0;
    uint32_t leftIndex = 0;
    if (fileSize > 3 && mappedFile[0] == 0xEF && mappedFile[1] == 0xBB && mappedFile[2] == 0xBF) {
        /* UTF8 with BOM */
        rightIndex = 3;
        leftIndex = 3;
    }
    int8_t inQuotes = 0;
    while (rightIndex < fileSize) {
        if (mappedFile[rightIndex] & 0b10000000) {
            /* case: unicode */
            if (mappedFile[rightIndex] & 0b11100000) {
                if (mappedFile[rightIndex] & 0b11110000) {
                    rightIndex++;
                }
                rightIndex++;
            }
            rightIndex += 2; // multi-byte character cannot be a comma, quote, or newline
        }
        if (mappedFile[rightIndex] == '"') {
            /* case: quote */
            inQuotes = !inQuotes;
            if (inQuotes) {
                // leftIndex = rightIndex + 1;
            }
        }
        /* case: comma */
        if (mappedFile[rightIndex] == delimeter && !inQuotes) {
            mappedFile[rightIndex] = '\0';
            if (rowOrColumn == OSTOOLS_CSV_ROW) {
                list_append(outputList -> data[0].r, (unitype) (char *) (mappedFile + leftIndex), 's');
            } else {
                list_append(outputList -> data[outputList -> length - 1].r, (unitype) (char *) (mappedFile + leftIndex), 's');
                list_append(outputList, (unitype) list_init(), 'r');
            }
            mappedFile[rightIndex] = delimeter;
            leftIndex = rightIndex + 1;
            while (mappedFile[leftIndex] == ' ') {
                rightIndex++;
                leftIndex++;
            }
        }
        if ((mappedFile[rightIndex] == '\n' || mappedFile[rightIndex] == '\r') && !inQuotes) {
            /* case: end of line */
            if (rightIndex != leftIndex) {
                char tempHold = mappedFile[rightIndex];
                mappedFile[rightIndex] = '\0';
                if (rowOrColumn == OSTOOLS_CSV_ROW) {
                    list_append(outputList -> data[0].r, (unitype) (char *) (mappedFile + leftIndex), 's');
                } else {
                    list_append(outputList -> data[outputList -> length - 1].r, (unitype) (char *) (mappedFile + leftIndex), 's');
                }
                mappedFile[rightIndex] = tempHold;
            } else {
                if (rowOrColumn == OSTOOLS_CSV_COLUMN) {
                    list_pop(outputList);
                }
            }
            break;
        }
        rightIndex++;
    }
    while (mappedFile[rightIndex] == '\r' || mappedFile[rightIndex] == '\n') {
        rightIndex++;
    }
    leftIndex = rightIndex;
    /* process data (all doubles) */
    char listType = 'd';
    switch (fieldType) {
    case OSTOOLS_CSV_FIELD_DOUBLE:
        listType = 'd';
    break;
    case OSTOOLS_CSV_FIELD_INT:
        listType = 'i';
    break;
    case OSTOOLS_CSV_FIELD_STRING:
        listType = 'z'; // zstrings avoid double malloc
    break;
    default:
        listType = 'd';
    break;
    }
    int32_t column = 0; // columns are 0-indexed for the convenience of this parser
    int32_t row = 2; // start at row 2 (rows are 1-indexed for printouts)
    if (rowOrColumn == OSTOOLS_CSV_ROW) {
        list_append(outputList, (unitype) list_init(), 'r');
    }
    inQuotes = 0;
    while (rightIndex < fileSize) {
        if (mappedFile[rightIndex] & 0b10000000) {
            /* case: unicode */
            if (mappedFile[rightIndex] & 0b11100000) {
                if (mappedFile[rightIndex] & 0b11110000) {
                    rightIndex++;
                }
                rightIndex++;
            }
            rightIndex += 2; // multi-byte character cannot be a comma, quote, or newline
        }
        if (mappedFile[rightIndex] == '"') {
            /* case: quote */
            inQuotes = !inQuotes;
            if (inQuotes) {
                // leftIndex = rightIndex + 1;
            }
        }
        if (mappedFile[rightIndex] == delimeter && !inQuotes) {
            /* case: comma */
            mappedFile[rightIndex] = '\0';
            unitype field;
            if (fieldType == OSTOOLS_CSV_FIELD_DOUBLE) {
                sscanf((char *) (mappedFile + leftIndex), "%lf", (double *) &field);
            } else if (fieldType == OSTOOLS_CSV_FIELD_INT) {
                sscanf((char *) (mappedFile + leftIndex), "%d", (int *) &field);
            } else if (fieldType == OSTOOLS_CSV_FIELD_STRING) {
                field.s = malloc(rightIndex - leftIndex + 1);
                strcpy(field.s, (char *) (mappedFile + leftIndex));
            }
            if (rowOrColumn == OSTOOLS_CSV_ROW) {
                list_append(outputList -> data[outputList -> length - 1].r, field, listType);
            } else if (rowOrColumn == OSTOOLS_CSV_COLUMN) {
                if (column < outputList -> length) {
                    list_append(outputList -> data[column].r, field, listType);
                } else {
                    printf("osToolsLoadInternal - more data columns than headers at row %d\n", row);
                }
            }
            mappedFile[rightIndex] = delimeter;
            leftIndex = rightIndex + 1;
            while (mappedFile[leftIndex] == ' ') {
                rightIndex++;
                leftIndex++;
            }
            column++;
        }
        if ((mappedFile[rightIndex] == '\n' || mappedFile[rightIndex] == '\r') && !inQuotes) {
            /* case: end of line */
            if (rightIndex != leftIndex) {
                char tempHold = mappedFile[rightIndex];
                mappedFile[rightIndex] = '\0';
                unitype field;
                if (fieldType == OSTOOLS_CSV_FIELD_DOUBLE) {
                    sscanf((char *) (mappedFile + leftIndex), "%lf", (double *) &field);
                } else if (fieldType == OSTOOLS_CSV_FIELD_INT) {
                    sscanf((char *) (mappedFile + leftIndex), "%d", (int *) &field);
                } else if (fieldType == OSTOOLS_CSV_FIELD_STRING) {
                    field.s = malloc(rightIndex - leftIndex + 1);
                    strcpy(field.s, (char *) (mappedFile + leftIndex));
                }
                if (rowOrColumn == OSTOOLS_CSV_ROW) {
                    list_append(outputList -> data[outputList -> length - 1].r, field, listType);
                } else if (rowOrColumn == OSTOOLS_CSV_COLUMN) {
                    if (column < outputList -> length) {
                        list_append(outputList -> data[column].r, field, listType);
                    } else {
                        printf("osToolsLoadInternal - more data columns than headers at row %d\n", row);
                    }
                }
                mappedFile[rightIndex] = tempHold;
            }
            while (mappedFile[rightIndex] == '\r' || mappedFile[rightIndex] == '\n') {
                rightIndex++;
            }
            leftIndex = rightIndex;
            rightIndex--;
            if (rowOrColumn == OSTOOLS_CSV_ROW) {
                list_append(outputList, (unitype) list_init(), 'r');
            }
            column = 0;
            row++;
        }
        rightIndex++;
    }
    /* catch: if the file doesn't end with a newline */
    if (leftIndex == fileSize) {
        if (rowOrColumn == OSTOOLS_CSV_ROW && mappedFile[fileSize - 1] != delimeter && mappedFile[fileSize - 1] != ' ') {
            list_pop(outputList);
        }
    } else {
        unitype field;
        if (fieldType == OSTOOLS_CSV_FIELD_DOUBLE) {
            sscanf((char *) (mappedFile + leftIndex), "%lf", (double *) &field);
        } else if (fieldType == OSTOOLS_CSV_FIELD_INT) {
            sscanf((char *) (mappedFile + leftIndex), "%d", (int *) &field);
        } else if (fieldType == OSTOOLS_CSV_FIELD_STRING) {
            field.s = malloc(rightIndex - leftIndex + 1);
            sscanf((char *) (mappedFile + leftIndex), "%s", field.s);
        }
        list_append(outputList -> data[outputList -> length - 1].r, field, listType);
    }
    osToolsUnmapFile(mappedFile);
    return outputList;
}

/* packages a CSV file into a list (headers are strings, all fields are doubles) - use OSTOOLS_CSV_ROW to put it in a list of lists where each list is a row of the CSV and use OSTOOLS_CSV_COLUMN to output a list of lists where each list is a column of the CSV */
list_t *osToolsLoadCSV(char *filename, osToolsCSV rowOrColumn) {
    return osToolsLoadInternal(filename, rowOrColumn, ',', OSTOOLS_CSV_FIELD_DOUBLE);
}

/* packages a CSV file into a list (headers are strings, all fields are doubles) - use OSTOOLS_CSV_ROW to put it in a list of lists where each list is a row of the CSV and use OSTOOLS_CSV_COLUMN to output a list of lists where each list is a column of the CSV */
list_t *osToolsLoadCSVDouble(char *filename, osToolsCSV rowOrColumn) {
    return osToolsLoadInternal(filename, rowOrColumn, ',', OSTOOLS_CSV_FIELD_DOUBLE);
}

/* packages a CSV file into a list (headers are strings, all fields are ints) - use OSTOOLS_CSV_ROW to put it in a list of lists where each list is a row of the CSV and use OSTOOLS_CSV_COLUMN to output a list of lists where each list is a column of the CSV */
list_t *osToolsLoadCSVInt(char *filename, osToolsCSV rowOrColumn) {
    return osToolsLoadInternal(filename, rowOrColumn, ',', OSTOOLS_CSV_FIELD_INT);
}

/* packages a CSV file into a list (headers are strings, all fields are strings) - use OSTOOLS_CSV_ROW to put it in a list of lists where each list is a row of the CSV and use OSTOOLS_CSV_COLUMN to output a list of lists where each list is a column of the CSV */
list_t *osToolsLoadCSVString(char *filename, osToolsCSV rowOrColumn) {
    return osToolsLoadInternal(filename, rowOrColumn, ',', OSTOOLS_CSV_FIELD_STRING);
}

#ifdef OS_WINDOWS

int32_t osToolsInit(char argv0[], GLFWwindow *window) {
    osToolsIndependentInit(window);
    /* get executable filepath */
    GetModuleFileNameA(NULL, osToolsFileDialog.executableFilepath, MAX_PATH);
    if (GetLastError() != ERROR_SUCCESS) {
        strcpy(osToolsFileDialog.executableFilepath, "null");
        printf("Error: Could not retrieve executable filepath\n");
    }
    int32_t index = strlen(osToolsFileDialog.executableFilepath) - 1;
    while (index > -1 && osToolsFileDialog.executableFilepath[index] != '\\' && osToolsFileDialog.executableFilepath[index] != '/') {
        index--;
    }
    osToolsFileDialog.executableFilepath[index + 1] = '\0';
    /* initialise file dialog */
    strcpy(osToolsFileDialog.selectedFilename, "null");
    osToolsFileDialog.openOrSave = 0; // open by default
    osToolsFileDialog.numExtensions = 0; // 0 means all extensions
    osToolsFileDialog.extensions = malloc(1 * sizeof(char *)); // malloc list

    /* initialise clipboard */
    osToolsClipboard.text = glfwGetClipboardString(osToolsGLFW.osToolsWindow);
    return 0;
}

void osToolsFileDialogAddExtension(char *extension) {
    if (strlen(extension) <= 4) {
        osToolsFileDialog.numExtensions += 1;
        osToolsFileDialog.extensions = realloc(osToolsFileDialog.extensions, osToolsFileDialog.numExtensions * sizeof(char *));
        osToolsFileDialog.extensions[osToolsFileDialog.numExtensions - 1] = strdup(extension);
    } else {
        printf("extension name: %s too long\n", extension);
    }
}

int32_t osToolsFileDialogPrompt(char openOrSave, char *filename) { // 0 - open, 1 - save, filename refers to autofill filename ("null" or empty string for no autofill)
    osToolsFileDialog.openOrSave = openOrSave;
    HRESULT hr = CoInitializeEx(NULL, 0); // https://learn.microsoft.com/en-us/windows/win32/api/objbase/ne-objbase-coinit
    if (SUCCEEDED(hr)) {
        IFileDialog *fileDialog;
        IShellItem *psiResult;
        PWSTR pszFilePath = NULL;
        hr = CoCreateInstance(&CLSID_FileOpenDialog, NULL, CLSCTX_ALL, &IID_IFileOpenDialog, (void**) &fileDialog);
        if (SUCCEEDED(hr)) {
            fileDialog -> lpVtbl -> SetOptions(fileDialog, 0); // https://learn.microsoft.com/en-us/windows/win32/api/shobjidl_core/ne-shobjidl_core-_fileopendialogoptions from my tests these don't seem to do anything

            /* configure autofill filename */
            if (openOrSave == 1 && strcmp(filename, "null") != 0) {
                int32_t i = 0;
                uint16_t prename[MAX_PATH + 1];
                while (filename[i] != '\0' && i < MAX_PATH + 1) {
                    prename[i] = filename[i]; // convert from char to WCHAR
                    i++;
                }
                prename[i] = '\0';
                fileDialog -> lpVtbl -> SetFileName(fileDialog, prename);
            }

            /* load file restrictions
            Info: each COMDLG creates one more entry to the dropdown to the right of the text box in the file dialog window
            You can only see files that are specified in the types on the current COMDLG_FILTERSPEC selected in the dropdown
            Thats why I shove all the types into one COMDLG_FILTERSPEC, because I want the user to be able to see all compatible files at once
            */
            if (osToolsFileDialog.numExtensions > 0) {
                COMDLG_FILTERSPEC *fileExtensions = malloc(sizeof(COMDLG_FILTERSPEC)); // just one filter
                WCHAR *buildFilter = malloc(10 * osToolsFileDialog.numExtensions * sizeof(WCHAR));
                int32_t j = 0;
                for (int32_t i = 0; i < osToolsFileDialog.numExtensions; i++) {
                    buildFilter[j] = (uint16_t) '*';
                    buildFilter[j + 1] = (uint16_t) '.';
                    j += 2;
                    for (uint32_t k = 0; k < strlen(osToolsFileDialog.extensions[i]) && k < 8; k++) {
                        buildFilter[j] = osToolsFileDialog.extensions[i][k];
                        j += 1;
                    }
                    buildFilter[j] = (uint16_t) ';';
                    j += 1;
                }
                buildFilter[j] = (uint16_t) '\0';
                (*fileExtensions).pszName = L"Specified Types";
                (*fileExtensions).pszSpec = buildFilter;
                fileDialog -> lpVtbl -> SetFileTypes(fileDialog, 1, fileExtensions);
                free(buildFilter);
                free(fileExtensions);
            }

            /* configure title and button text */
            if (openOrSave == 0) {
                /* open */
                fileDialog -> lpVtbl -> SetOkButtonLabel(fileDialog, L"Open");
                fileDialog -> lpVtbl -> SetTitle(fileDialog, L"Open");
            } else {
                /* save */
                fileDialog -> lpVtbl -> SetOkButtonLabel(fileDialog, L"Save");
                fileDialog -> lpVtbl -> SetTitle(fileDialog, L"Save");
            }

            /* execute */
            fileDialog -> lpVtbl -> Show(fileDialog, NULL); // opens window
            hr = fileDialog -> lpVtbl -> GetResult(fileDialog, &psiResult); // succeeds if a file is selected
            if (SUCCEEDED(hr)){
                hr = psiResult -> lpVtbl -> GetDisplayName(psiResult, SIGDN_FILESYSPATH, &pszFilePath); // extracts path name
                if (SUCCEEDED(hr)) {
                    int32_t i = 0;
                    /* convert from WCHAR to char */
                    while (pszFilePath[i] != '\0' && i < MAX_PATH + 1) {
                        osToolsFileDialog.selectedFilename[i] = pszFilePath[i];
                        i++;
                    }
                    osToolsFileDialog.selectedFilename[i] = '\0';
                    CoTaskMemFree(pszFilePath);
                    return 0;
                }
                psiResult -> lpVtbl -> Release(psiResult);
            }
            fileDialog -> lpVtbl -> Release(fileDialog);
        } else {
            printf("ERROR - HRESULT: %lx\n", hr);
        }
        CoUninitialize();
    }
    return -1;
}

uint8_t *osToolsMapFile(char *filename, uint32_t *sizeOutput) {
    HANDLE fileHandle = CreateFileA(filename, FILE_GENERIC_READ | FILE_GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (fileHandle == INVALID_HANDLE_VALUE) {
        printf("Could not open file %ld\n", GetLastError());
        *sizeOutput = 0;
        return NULL;
    }
    if (!GetFileSizeEx(fileHandle, (PLARGE_INTEGER) sizeOutput)) {
        printf("Failed to get size of file\n");
        CloseHandle(fileHandle);
        *sizeOutput = 0;
        return NULL;
    }
    HANDLE mappingHandle = CreateFileMappingA(fileHandle, NULL, PAGE_READWRITE, 0, 0, NULL);
    if (mappingHandle == NULL) {
        printf("Could not memory map file %ld\n", GetLastError());
        CloseHandle(fileHandle);
        *sizeOutput = 0;
        return NULL;
    }
    LPVOID address = MapViewOfFile(mappingHandle, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    if (address == NULL) {
        printf("Could not create map view of file %ld\n", GetLastError());
        CloseHandle(fileHandle);
        CloseHandle(mappingHandle);
        *sizeOutput = 0;
        return NULL;
    }
    list_append(osToolsMemmap.mappedFiles, (unitype) filename, 's'); // filename
    list_append(osToolsMemmap.mappedFiles, (unitype) fileHandle, 'l'); // file handle (uint64_t so no free on list_delete)
    list_append(osToolsMemmap.mappedFiles, (unitype) mappingHandle, 'l'); // mapping handle (uint64_t so no free on list_delete)
    list_append(osToolsMemmap.mappedFiles, (unitype) address, 'l'); // file data (uint64_t so no free on list_delete)
    return address;
}

int32_t osToolsUnmapFile(uint8_t *data) {
    UnmapViewOfFile(data);
    int32_t index = -1;
    for (uint32_t i = 0; i < osToolsMemmap.mappedFiles -> length; i += 4) {
        if (osToolsMemmap.mappedFiles -> data[i + 3].p == data) {
            index = i;
            break;
        }
    }
    if (index >= 0) {
        CloseHandle(osToolsMemmap.mappedFiles -> data[index + 1].p);
        CloseHandle(osToolsMemmap.mappedFiles -> data[index + 2].p);
        list_delete(osToolsMemmap.mappedFiles, index);
        list_delete(osToolsMemmap.mappedFiles, index);
        list_delete(osToolsMemmap.mappedFiles, index);
        list_delete(osToolsMemmap.mappedFiles, index);
        return 0;
    } else {
        printf("Could not find %p in memory mapped index\n", data);
        return -1;
    }
}

/*
windows COM port support
https://learn.microsoft.com/en-us/windows/win32/devio/configuring-a-communications-resource
*/

win32ComPortObject win32com;

/* opens a com port */
int32_t win32comInit(win32ComPortObject *com, char *name) {
    strcpy(com -> name, name);
    DCB dcb;
    BOOL fSuccess;
    /* https://support.microsoft.com/en-us/topic/howto-specify-serial-ports-larger-than-com9-db9078a5-b7b6-bf00-240f-f749ebfd913e */
    char comName[24] = "\\\\.\\";
    if (strlen(name) > 4) {
        strcpy(comName + strlen(comName), name);
    } else {
        strcpy(comName, name);
    }
    com -> comHandle = CreateFileA(comName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (com -> comHandle == INVALID_HANDLE_VALUE) {
        printf("Could not open com port %s, error %ld\n", name, GetLastError());
        return -1;
    } else {
        printf("Successfully opened port %s\n", name);
    }
    /* Initialize the DCB structure. */
    SecureZeroMemory(&dcb, sizeof(DCB));
    dcb.DCBlength = sizeof(DCB);
    /* Build on the current configuration by first retrieving all current */
    /* settings. */
    fSuccess = GetCommState(com -> comHandle, &dcb);
    if (!fSuccess) {
        /* Handle the error. */
        printf("GetCommState failed with error %ld.\n", GetLastError());
        return -1;
    }
    /* Fill in some DCB values and set the com state: 
       115200 bps, 8 data bits, no parity, and 1 stop bit. */
    dcb.BaudRate = CBR_115200;    // baud rate
    dcb.ByteSize = 8;             // data size, xmit and rcv
    dcb.Parity   = NOPARITY;      // parity bit
    dcb.StopBits = ONESTOPBIT;    // stop bit
    fSuccess = SetCommState(com -> comHandle, &dcb);
    if (!fSuccess) {
        /* Handle the error. */
        printf("SetCommState failed with error %ld.\n", GetLastError());
        return -1;
    }
    /* Get the comm config again. */
    fSuccess = GetCommState(com -> comHandle, &dcb);
    if (!fSuccess) {
        /* Handle the error. */
        printf("GetCommState failed with error %ld.\n", GetLastError());
        return -1;
    }
    return 0;
}

/* returns number of bytes sent */
int32_t win32comSend(win32ComPortObject *com, uint8_t *data, int32_t length) {
    /* https://www.codeproject.com/Articles/3061/Creating-a-Serial-communication-on-Win32#sending */
    DWORD bytes;
    if (WriteFile(com -> comHandle, data, length, &bytes, NULL) == 0) {
        printf("win32comSend failed with error %ld\n", GetLastError());
        return -1;
    }
    return bytes;
}

/* returns number of bytes received */
int32_t win32comReceive(win32ComPortObject *com, uint8_t *buffer, int32_t length) {
    DWORD bytes;
    if (ReadFile(com -> comHandle, buffer, length, &bytes, NULL) == 0) {
        printf("win32comReceive failed with error %ld\n", GetLastError());
        return -1;
    }
    return bytes;
}

/* closes a com port */
int32_t win32comClose(win32ComPortObject *com) {
    if (CloseHandle(com -> comHandle) == 0) {
        printf("win32comClosefailed with error %ld\n", GetLastError());
        return -1;
    }
    return 0;
}

/*
https://gist.github.com/mmozeiko/c0dfcc8fec527a90a02145d2cc0bfb6d
https://learn.microsoft.com/en-us/windows/win32/winsock/complete-server-code
https://learn.microsoft.com/en-us/windows/win32/winsock/complete-client-code
*/

win32SocketObject win32Socket;

int32_t win32tcpInit(char *address, char *port) {
    for (int32_t i = 0; i < WIN32TCP_NUM_SOCKETS; i++) {
        win32Socket.connectSocket[i] = 0;
        win32Socket.socketOpen[i] = 0;
    }
    win32Socket.address = address;
    win32Socket.port = port;
    char modifiable[strlen(address) + 1];
    strcpy(modifiable, address);
    char *check = strtok(modifiable, ".");
    int32_t segments = 0;
    uint8_t ipAddress[4] = {0};
    while (check != NULL) {
        if (segments > 3) {
            printf("Could not initialise win32tcp - invalid ip address\n");
            return 1;
        }
        int32_t segmentValue = atoi(check);
        if (segmentValue > 255 || segmentValue < 0) {
            printf("Could not initialise win32tcp - invalid ip address\n");
            return 1;
        }
        ipAddress[segments] = segmentValue;
        check = strtok(NULL, ".");
        segments++;
    }
    if (segments != 4) {
        printf("Could not initialise win32tcp - invalid ip address\n");
        return 1;
    }
    /* Initialize Winsock */
    WSADATA wsaData;
    int32_t status;
    status = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (status != 0) {
        return 1;
    }
    
    /* hints */
    struct addrinfo hints;
    struct addrinfo *result;
    struct addrinfo *ptr;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    /* Resolve the server address and port */
    status = getaddrinfo(address, port, &hints, &result);
    if (status != 0) {
        WSACleanup();
        return 1;
    }

    /* Attempt to connect to an address until one succeeds */
    for (ptr = result; ptr != NULL; ptr = ptr -> ai_next) {
        /* Create a SOCKET for connecting to server */
        win32Socket.connectSocket[0] = socket(ptr -> ai_family, ptr -> ai_socktype, ptr -> ai_protocol);
        if (win32Socket.connectSocket[0] == INVALID_SOCKET) {
            WSACleanup();
            return 1;
        }

        /* Connect to server */
        status = connect(win32Socket.connectSocket[0], ptr -> ai_addr, (int) ptr -> ai_addrlen);
        if (status == SOCKET_ERROR) {
            closesocket(win32Socket.connectSocket[0]);
            win32Socket.connectSocket[0] = INVALID_SOCKET;
            continue;
        }
        break;
    }

    // /* Send an initial buffer */
    // char sendbuf[2] = {12, 34};
    // status = send(win32Socket.connectSocket[0], sendbuf, 2, 0);
    // if (status == SOCKET_ERROR) {
    //     closesocket(win32Socket.connectSocket[0]);
    //     WSACleanup();
    //     return 1;
    // }

    // printf("Bytes Sent: %ld\n", status);

    /* shutdown the connection since no more data will be sent */
    status = shutdown(win32Socket.connectSocket[0], SD_SEND);
    if (status == SOCKET_ERROR) {
        closesocket(win32Socket.connectSocket[0]);
        WSACleanup();
        return 1;
    }

    /* Receive until the peer closes the connection */
    int32_t recvbuflen = 512;
    char recvbuf[recvbuflen];
    status = 1;
    while (status > 0) {
        status = recv(win32Socket.connectSocket[0], recvbuf, recvbuflen, 0);
        if (status > 0) {
            // printf("Bytes received: %d\n", status);
        } else if (status == 0) {
            // printf("Connection closed\n");
        } else {
            // printf("recv failed with error: %d\n", WSAGetLastError());
        }
    }

    /* cleanup */
    closesocket(win32Socket.connectSocket[0]);
    printf("Successfully connected to %d.%d.%d.%d\n", ipAddress[0], ipAddress[1], ipAddress[2], ipAddress[3]);
    return 0;
}

SOCKET *win32tcpCreateSocket() {
    /* define socket index */
    int32_t socketIndex = 0;
    for (int32_t i = 0; i < WIN32TCP_NUM_SOCKETS; i++) {
        if (win32Socket.socketOpen[i] == 0) {
            win32Socket.socketOpen[i] = 1;
            socketIndex = i;
            break;
        }
    }
    if (socketIndex == WIN32TCP_NUM_SOCKETS) {
        /* no sockets left */
        return NULL;
    }
    /* hints */
    struct addrinfo hints;
    struct addrinfo *result;
    struct addrinfo *ptr;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    /* Resolve the server address and port */
    int32_t status = getaddrinfo(win32Socket.address, win32Socket.port, &hints, &result);
    if (status != 0) {
        WSACleanup();
        return NULL;
    }

    /* Attempt to connect to an address until one succeeds */
    for (ptr = result; ptr != NULL; ptr = ptr -> ai_next) {
        /* Create a SOCKET for connecting to server */
        win32Socket.connectSocket[socketIndex] = socket(ptr -> ai_family, ptr -> ai_socktype, ptr -> ai_protocol);
        if (win32Socket.connectSocket[socketIndex] == INVALID_SOCKET) {
            WSACleanup();
            return NULL;
        }
        /* Connect to server */
        status = connect(win32Socket.connectSocket[socketIndex], ptr -> ai_addr, (int) ptr -> ai_addrlen);
        if (status == SOCKET_ERROR) {
            closesocket(win32Socket.connectSocket[socketIndex]);
            win32Socket.connectSocket[socketIndex] = INVALID_SOCKET;
            continue;
        }
        break;
    }
    // uint32_t mode;
    // printf("ioctlsocket %d\n", ioctlsocket(win32Socket.connectSocket[socketIndex], FIONBIO, &mode));
    return &win32Socket.connectSocket[socketIndex];
}

int32_t win32tcpSend(SOCKET *socket, uint8_t *data, int32_t length) {
    int32_t status = send(*socket, (const char *) data, length, 0 );
    if (status == SOCKET_ERROR) {
        closesocket(*socket);
        return 1;
    }
    return 0;
}

int32_t win32tcpReceive(SOCKET *socket, uint8_t *buffer, int32_t length) {
    int32_t status = 1;
    int32_t bytes = 0;
    while (status > 0) {
        status = recv(*socket, (char *) buffer, length, 0);
        if (status > 0) {
            // printf("Bytes received: %d\n", status);
        } else if (status == 0) {
            // printf("Connection closed\n");
        } else {
            // printf("recv failed with error: %d\n", WSAGetLastError());
        }
        bytes += status;
        if (bytes >= length) {
            return bytes;
        }
    }
    return bytes;
}

int32_t win32tcpReceive2(SOCKET *socket, uint8_t *buffer, int32_t length) {
    int32_t status = 1;
    status = recv(*socket, (char *) buffer, length, 0);
    if (status > 0) {
        // printf("Bytes received: %d\n", status);
    } else if (status == 0) {
        // printf("Connection closed\n");
    } else {
        // printf("recv failed with error: %d\n", WSAGetLastError());
    }
    return status;
}

void win32tcpDeinit() {
    WSACleanup();
}

#endif
#ifdef OS_LINUX

/* This is the zenity version of osToolsFileDialog.h, it's for linux */

/* 
a note on zenity:
FILE* filenameStream = popen("zenity --file-selection", "r"); (popen runs the first argument in the shell as a bash script)
this function returns a pointer to a stream of data that contains only the filepath
popen with (zenity --file-selection) will not return a FILE* to the location of the file. You cannot read the file from this FILE*, you must call fopen on the filename

additionally, filters can be added with
FILE* filenameStream = popen("zenity --file-selection --file-filter='Name | *.ext *.ext2 *.ext3'", "r");
This is similar to COMDLG_FILTERSPEC struct's pszName and pszSpec, so you can add more filter "profiles" by using multiple --file-filter tags in the command
*/

int32_t osToolsInit(char argv0[], GLFWwindow *window) {
    osToolsIndependentInit(window);
    /* get executable filepath */
    FILE *exStringFile = popen("pwd", "r");
    fscanf(exStringFile, "%s", osToolsFileDialog.executableFilepath);
    strcat(osToolsFileDialog.executableFilepath, "/");
    strcat(osToolsFileDialog.executableFilepath, argv0);
    
    int32_t index = strlen(osToolsFileDialog.executableFilepath) - 1;
    while (index > -1 && osToolsFileDialog.executableFilepath[index] != '/') {
        index--;
    }
    osToolsFileDialog.executableFilepath[index + 1] = '\0';

    /* initialise file dialog */
    strcpy(osToolsFileDialog.selectedFilename, "null");
    osToolsFileDialog.openOrSave = 0; // open by default
    osToolsFileDialog.numExtensions = 0; // 0 means all extensions
    osToolsFileDialog.extensions = malloc(1 * sizeof(char *)); // malloc list
    return 0;
}

void osToolsFileDialogAddExtension(char *extension) {
    if (strlen(extension) <= 4) {
        osToolsFileDialog.numExtensions += 1;
        osToolsFileDialog.extensions = realloc(osToolsFileDialog.extensions, osToolsFileDialog.numExtensions * 8);
        osToolsFileDialog.extensions[osToolsFileDialog.numExtensions - 1] = strdup(extension);
    } else {
        printf("extension name: %s too long\n", extension);
    }
}

int32_t osToolsFileDialogPrompt(char openOrSave, char *prename) { // 0 - open, 1 - save, prename refers to autofill filename ("null" or empty string for no autofill)
    char fullCommand[23 + 13 + 256 + 15 + 34 + 7 * osToolsFileDialog.numExtensions + 14 + 1]; // 23 for zenity --file-selection, 13 for --filename=', 256 for prename, 15 for --title='Open', 34 for --file-filter='Specified Types | , 7 for each extension, 14 for title, 1 for \0
    strcpy(fullCommand, "zenity --file-selection");
    /* configure autofill filename */
    if (openOrSave == 1 && strcmp(prename, "null") != 0) {
        strcat(fullCommand, " --filename='");
        strcat(fullCommand, prename);
        strcat(fullCommand, "'");
    }

    /* configure title */
    char title[16] = " --title='Open'";
    if (openOrSave == 1) {
        strcpy(title, " --title='Save'");
    }
    strcat(fullCommand, title);

    /* configure extensions */
    if (osToolsFileDialog.numExtensions > 0) {
        char buildFilter[7 * osToolsFileDialog.numExtensions + 1]; // last space is replaced with ' and followed by \0
        int32_t j = 0;
        for (int32_t i = 0; i < osToolsFileDialog.numExtensions; i++) {
            buildFilter[j] = '*';
            buildFilter[j + 1] = '.';
            j += 2;
            for (uint32_t k = 0; k < strlen(osToolsFileDialog.extensions[i]) && k < 8; k++) {
                buildFilter[j] = osToolsFileDialog.extensions[i][k];
                j += 1;
            }
            if (i != osToolsFileDialog.numExtensions - 1) { // dont add space if it's the last element
                buildFilter[j] = ' ';
                j += 1;
            }
        }
        buildFilter[j] = '\'';
        buildFilter[j + 1] = '\0';
        char filterName[35] = " --file-filter='Specified Types | ";
        strcat(fullCommand, filterName);
        strcat(fullCommand, buildFilter);
    }

    /* execute */
    FILE* filenameStream = popen(fullCommand, "r");
    if (fgets(osToolsFileDialog.selectedFilename, 4097, filenameStream) == NULL) { // adds a \n before \0 (?)
        strcpy(osToolsFileDialog.selectedFilename, "null");
        return -1;
    }
    for (uint32_t i = 0; i < 4096; i++) {
        if (osToolsFileDialog.selectedFilename[i] == '\n') {
            osToolsFileDialog.selectedFilename[i] = '\0'; // replace all newlines with null characters
        }
    }
    pclose(filenameStream);
    return 0;
}

uint8_t *osToolsMapFile(char *filename, uint32_t *sizeOutput) {
    int32_t fd = open(filename, O_RDWR);
    struct stat stats;
    if (fstat(fd, &stats) == -1) {
        printf("Could not get stats of file %s\n", filename);
        *sizeOutput = 0;
        return NULL;
    }
    *sizeOutput = stats.st_size;
    void *out = mmap(NULL, *sizeOutput, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (out == MAP_FAILED) {
        printf("Could not memory map file %s\n", filename);
        *sizeOutput = 0;
        return NULL;
    }
    list_append(osToolsMemmap.mappedFiles, (unitype) out, 'l');
    list_append(osToolsMemmap.mappedFiles, (unitype) *sizeOutput, 'i');
    return (uint8_t *) out;
}

int32_t osToolsUnmapFile(uint8_t *data) {
    int32_t index = -1;
    for (uint32_t i = 0; i < osToolsMemmap.mappedFiles -> length; i += 2) {
        if (osToolsMemmap.mappedFiles -> data[i].p == data) {
            index = i;
            break;
        }
    }
    if (index >= 0) {
        munmap(data, osToolsMemmap.mappedFiles -> data[index + 1].i);
        return 0;
    } else {
        printf("Could not find %p in memory mapped index\n", data);
        return -1;
    }
}

#endif

#endif /* OS_TOOLS_IMPLEMENTATION */


#endif /* TURTLE_H */
