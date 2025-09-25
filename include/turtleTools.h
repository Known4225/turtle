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

#include "turtleText.h"

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
    int8_t contextEnabled;
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
    TT_ELEMENT_CONTEXT = 5,
    TT_ELEMENT_DROPDOWN = 6,
    TT_ELEMENT_TEXTBOX = 7,
    TT_NUMBER_OF_ELEMENTS = 8,
} tt_element_names_t;

typedef struct {
    list_t *all;
    list_t *buttons;
    list_t *switches;
    list_t *dials;
    list_t *sliders;
    list_t *scrollbars;
    list_t *contexts;
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

/* initialise ribbon with a list instead of a config file - this function frees the list so you don't have to */
int32_t ribbonInitList(list_t *config);

/* internal */
int32_t ribbonInitInternal(FILE *configFile, list_t *configList, int8_t fileExists);

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
    double size; // size of text
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
int32_t popupInit(char *filename);

/* initialise popup with a list instead of a config file - this function frees the list so you don't have to */
int32_t popupInitList(list_t *config);

/* internal */
int32_t popupInitInternal(FILE *configFile, list_t *configList, int8_t fileExists);

/* render popup */
void popupUpdate();

/* free popup */
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

#define TT_LABEL_LENGTH_LIMIT 128

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
    TT_CONTEXT_DIRECTION_UP_LEFT = 0,
    TT_CONTEXT_DIRECTION_UP_RIGHT = 1,
    TT_CONTEXT_DIRECTION_DOWN_LEFT = 1,
    TT_CONTEXT_DIRECTION_DOWN_RIGHT = 1,
} tt_context_direction_t;

/* context menu */
typedef struct {
    tt_element_names_t element;
    tt_element_enabled_t enabled;
    tt_color_override_t color;
    double x;
    double y;
    double size;
    int32_t *variable; // index of selected option
    list_t *options;
    int32_t index;
    int32_t status;
    tt_context_direction_t direction;
    double maxXfactor;
} tt_context_t;

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

void contextCalculateMax(tt_context_t *contextp);

tt_context_t *contextInit(list_t *options, int32_t *variable, double x, double y, double size);

void contextFree(tt_context_t *contextp);

void dropdownCalculateMax(tt_dropdown_t *dropdownp);

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
