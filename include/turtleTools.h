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

typedef enum {
    TT_ELEMENT_PRIORITY_NONE = 0,
    TT_ELEMENT_PRIORITY_BUTTON = 1,
    TT_ELEMENT_PRIORITY_SWITCH = 2,
    TT_ELEMENT_PRIORITY_DIAL = 3,
    TT_ELEMENT_PRIORITY_SLIDER = 4,
    TT_ELEMENT_PRIORITY_TEXTBOX = 5,
    TT_ELEMENT_PRIORITY_DROPDOWN = 6,
    TT_ELEMENT_PRIORITY_SCROLLBAR = 7,
    TT_ELEMENT_PRIORITY_CONTEXT = 8,
    TT_ELEMENT_PRIORITY_RIBBON = 9,
    TT_ELEMENT_PRIORITY_POPUP = 10,
} tt_element_priority_t;

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
    TT_COLOR_BACKGROUND = 0,
    TT_COLOR_BACKGROUND_BASE = 0,
    TT_COLOR_BACKGROUND_HIGHLIGHT = 3,
    TT_COLOR_BACKGROUND_ALTERNATE = 6,
    TT_COLOR_BACKGROUND_COMPLEMENT = 9,
    TT_COLOR_TEXT = 12,
    TT_COLOR_TEXT_BASE = 12,
    TT_COLOR_TEXT_HIGHLIGHT = 15,
    TT_COLOR_TEXT_ALTERNATE = 18,
    TT_COLOR_TEXT_COMPLEMENT = 21,
    TT_COLOR_COMPONENT = 24,
    TT_COLOR_COMPONENT_BASE = 24,
    TT_COLOR_COMPONENT_HIGHLIGHT = 27,
    TT_COLOR_COMPONENT_ALTERNATE = 30,
    TT_COLOR_COMPONENT_COMPLEMENT = 33,
    TT_COLOR_TERTIARY = 36,
    TT_COLOR_TERTIARY_BASE = 36,
    TT_COLOR_TERTIARY_HIGHLIGHT = 39,
    TT_COLOR_TERTIARY_ALTERNATE = 42,
    TT_COLOR_TERTIARY_COMPLEMENT = 45,
    TT_COLOR_RED = 48,
    TT_COLOR_RED_ALTERNATE = 51,
    TT_COLOR_ORANGE = 54,
    TT_COLOR_ORANGE_ALTERNATE = 57,
    TT_COLOR_YELLOW = 60,
    TT_COLOR_YELLOW_ALTERNATE = 63,
    TT_COLOR_GREEN = 66,
    TT_COLOR_GREEN_ALTERNATE = 69,
    TT_COLOR_CYAN = 72,
    TT_COLOR_CYAN_ALTERNATE = 75,
    TT_COLOR_BLUE = 78,
    TT_COLOR_BLUE_ALTERNATE = 81,
    TT_COLOR_PURPLE = 84,
    TT_COLOR_PURPLE_ALTERNATE = 87,
    TT_COLOR_MAGENTA = 90,
    TT_COLOR_MAGENTA_ALTERNATE = 93,
    TT_COLOR_PINK = 96,
    TT_COLOR_PINK_ALTERNATE = 99,
    TT_COLOR_BLACK = 102,
    TT_COLOR_BLACK_ALTERNATE = 105,
    TT_COLOR_WHITE = 108,
    TT_COLOR_WHITE_ALTERNATE = 111,
    TT_COLOR_DARK_GREY = 114,
    TT_COLOR_DARK_GREY_ALTERNATE = 117,
    TT_COLOR_LIGHT_GREY = 120,
    TT_COLOR_LIGHT_GREY_ALTERNATE = 123,
} tt_theme_internal_t;

/* default colours (light theme) */
extern double tt_themeColors[];

void tt_setColor(int32_t index);

void turtleToolsSetTheme(tt_theme_name_t theme);

extern int32_t tt_color_default[];

typedef enum {
    /* ribbon and popup (special cases) */
    TT_COLOR_SLOT_RIBBON_TOP = TT_COLOR_COMPONENT_HIGHLIGHT,
    TT_COLOR_SLOT_RIBBON_DROPDOWN = TT_COLOR_COMPONENT_HIGHLIGHT,
    TT_COLOR_SLOT_RIBBON_HOVER = TT_COLOR_COMPONENT,
    TT_COLOR_SLOT_RIBBON_SELECT = TT_COLOR_COMPONENT,
    TT_COLOR_SLOT_POPUP_BOX = TT_COLOR_COMPONENT_ALTERNATE,
    TT_COLOR_SLOT_POPUP_BUTTON_SELECT = TT_COLOR_COMPONENT_HIGHLIGHT,
    TT_COLOR_SLOT_POPUP_BUTTON = TT_COLOR_COMPONENT,
    /* button */
    TT_COLOR_SLOT_BUTTON_TEXT = 0,
    TT_COLOR_SLOT_BUTTON = 1,
    TT_COLOR_SLOT_BUTTON_SELECT = 2,
    TT_COLOR_SLOT_BUTTON_SELECTED_TEXT = 3,
    TT_COLOR_SLOT_BUTTON_CLICKED = 4,
    /* switch */
    TT_COLOR_SLOT_SWITCH_TEXT = 0,
    TT_COLOR_SLOT_SWITCH_TEXT_HOVER = 1,
    TT_COLOR_SLOT_SWITCH_OFF = 2,
    TT_COLOR_SLOT_SWITCH_ON = 3,
    TT_COLOR_SLOT_CIRCLE_OFF = 4,
    TT_COLOR_SLOT_CIRCLE_ON = 5,
    /* dial */
    TT_COLOR_SLOT_DIAL_TEXT = 0,
    TT_COLOR_SLOT_DIAL = 1,
    TT_COLOR_SLOT_DIAL_INNER = 2,
    /* slider */
    TT_COLOR_SLOT_SLIDER_TEXT = 0,
    TT_COLOR_SLOT_SLIDER_BAR = 1,
    TT_COLOR_SLOT_SLIDER_CIRCLE = 2,
    /* scrollbar */
    TT_COLOR_SLOT_SCROLLBAR_BASE = 1,
    TT_COLOR_SLOT_SCROLLBAR_BAR = 2,
    TT_COLOR_SLOT_SCROLLBAR_HOVER = 3,
    TT_COLOR_SLOT_SCROLLBAR_CLICKED = 4,
    /* context */
    TT_COLOR_SLOT_CONTEXT_TEXT = 0,
    TT_COLOR_SLOT_CONTEXT_BASE = 1,
    TT_COLOR_SLOT_CONTEXT_SELECT = 2,
    /* dropdown */
    TT_COLOR_SLOT_DROPDOWN_TEXT = 0,
    TT_COLOR_SLOT_DROPDOWN_TEXT_HOVER = 1,
    TT_COLOR_SLOT_DROPDOWN_BASE = 2,
    TT_COLOR_SLOT_DROPDOWN_SELECT = 3,
    TT_COLOR_SLOT_DROPDOWN_HOVER = 4,
    TT_COLOR_SLOT_DROPDOWN_TRIANGLE = 5,
    /* textbox */
    TT_COLOR_SLOT_TEXTBOX_TEXT = 0,
    TT_COLOR_SLOT_TEXTBOX_BOX = 1,
    TT_COLOR_SLOT_TEXTBOX_PHANTOM_TEXT = 2,
    TT_COLOR_SLOT_TEXTBOX_LINE = 3,
} tt_color_slots_t;

void elementResetColor(void *elementp, int32_t elementType);

/* ribbon */

/* ribbon variables */
typedef struct {
    uint8_t marginSize;
    int8_t mainselect[4]; // 0 - select, 1 - mouseHover, 2 - selected, 3 - premove close dropdown
    int8_t subselect[4]; // 0 - select, 1 - mouseHover, 2 - selected, 3 - free
    int8_t output[3]; // 0 - toggle, 1 - mainselect, 2 - subselect
    int8_t mouseDown; // keeps track of previous frame mouse information
    double bounds[4]; // list of coordinate bounds (minX, minY, maxX, maxY)
    double ribbonSize;
    list_t *options;
    list_t *lengths;
} tt_ribbon_t;

extern tt_ribbon_t tt_ribbon;

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

extern tt_popup_t tt_popup;

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
    double dialAnchorX;
    double dialAnchorY;
    double barAnchor;
    tt_element_priority_t elementLogicType;
    int32_t elementLogicIndex;
    tt_element_priority_t elementLogicTypeOld;
    int32_t elementLogicIndexOld;
} tt_globals_t;

extern tt_globals_t tt_globals;

typedef enum {
    TT_ELEMENT_ENABLED = 0,
    TT_ELEMENT_NO_MOUSE = 1,
    TT_ELEMENT_HIDE = 2,
} tt_element_enabled_t;

#define TT_LABEL_LENGTH_LIMIT 128

typedef enum {
    TT_BUTTON_SHAPE_RECTANGLE = 0,
    TT_BUTTON_SHAPE_ROUNDED_RECTANGLE = 1,
    TT_BUTTON_SHAPE_CIRCLE = 2,
    TT_BUTTON_SHAPE_TEXT = 3,
} tt_button_shape_t;

typedef enum {
    TT_BUTTON_ALIGN_LEFT = 0,
    TT_BUTTON_ALIGN_CENTER = 1,
    TT_BUTTON_ALIGN_RIGHT = 2,
} tt_button_align_t;

/* button */
typedef struct {
    tt_element_names_t element;
    tt_element_enabled_t enabled;
    int32_t color[8];
    double x;
    double y;
    double size;
    int8_t *variable; // 1 if button is being pressed, 0 otherwise
    char label[TT_LABEL_LENGTH_LIMIT];
    int32_t status;
    tt_button_shape_t shape;
    tt_button_align_t align;
} tt_button_t;

typedef enum {
    TT_SWITCH_STYLE_CLASSIC = 0,
    TT_SWITCH_STYLE_SIDESWIPE_LEFT = 1,
    TT_SWITCH_STYLE_SIDESWIPE_RIGHT = 2,
    TT_SWITCH_STYLE_CHECKBOX = 3,
    TT_SWITCH_STYLE_XBOX = 4,
} tt_switch_style_t;

typedef enum {
    TT_SWITCH_ALIGN_LEFT = 0,
    TT_SWITCH_ALIGN_CENTER = 1,
    TT_SWITCH_ALIGN_RIGHT = 2,
} tt_switch_align_t;

/* switch */
typedef struct {
    tt_element_names_t element;
    tt_element_enabled_t enabled;
    int32_t color[8];
    double x;
    double y;
    double size;
    int8_t *variable; // 1 if switch is flipped, 0 otherwise
    char label[TT_LABEL_LENGTH_LIMIT];
    int32_t status;
    tt_switch_style_t style;
    tt_switch_align_t align;
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
    int32_t color[8];
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
    int32_t color[8];
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
    int32_t color[8];
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
    TT_CONTEXT_DIRECTION_AUTO = 0,
    TT_CONTEXT_DIRECTION_UP_LEFT = 1,
    TT_CONTEXT_DIRECTION_UP_RIGHT = 2,
    TT_CONTEXT_DIRECTION_DOWN_LEFT = 3,
    TT_CONTEXT_DIRECTION_DOWN_RIGHT = 4,
} tt_context_direction_t;

/* context menu */
typedef struct {
    tt_element_names_t element;
    tt_element_enabled_t enabled;
    int32_t color[8];
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
    int32_t color[8];
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
    int32_t color[8];
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
