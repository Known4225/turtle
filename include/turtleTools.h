/* turtleTools library includes:
ribbon: customisable top bar
popup: 
dropdown
button
dial
switch
slider
text box (under development)
*/

#ifndef TURTLETOOLS
#define TURTLETOOLS
#include "turtleText.h"

int randomInt(int lowerBound, int upperBound) { // random integer between lower and upper bound (inclusive)
    return (rand() % (upperBound - lowerBound + 1) + lowerBound);
}

double randomDouble(double lowerBound, double upperBound) { // random double between lower and upper bound
    return (rand() * (upperBound - lowerBound) / RAND_MAX + lowerBound); // probably works idk
}

typedef enum {
    TT_THEME_LIGHT = 0,
    TT_THEME_DARK = 1,
} tt_theme_name_t;

tt_theme_name_t tt_theme;

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
} tt_enabled_t;

tt_enabled_t tt_enabled; // all start at 0 (global variable)

typedef enum {
    TT_ELEMENT_BUTTON = 0,
    TT_ELEMENT_SWITCH = 1,
    TT_ELEMENT_DIAL = 2,
    TT_ELEMENT_SLIDER = 3,
    TT_ELEMENT_SCROLLBAR = 4,
    TT_ELEMENT_DROPDOWN = 5,
    TT_ELEMENT_TEXTBOX = 6,
} tt_element_names_t;

typedef struct {
    list_t *all;
    list_t *buttons;
    list_t *switches;
    list_t *dials;
    list_t *sliders;
    list_t *scrollbars;
    list_t *dropdowns;
} tt_elements_t;

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
    180.0, 180.0, 180.0, // switch color off (33)
    230.0, 230.0, 230.0, // switch circle color off (36)
    30.0, 30.0, 30.0,    // switch color on (39)
    0.0, 255.0, 255.0,   // switch circle color on (42)
    0.0, 0.0, 0.0,       // dial color (45)
    255.0, 255.0, 255.0, // dial inner circle color (48)
    120.0, 120.0, 120.0, // slider bar color (51)
    230.0, 230.0, 230.0, // slider circle color (54)
    160.0, 160.0, 160.0, // dropdown color (57)
    120.0, 120.0, 120.0, // dropdown select color (60)
    120.0, 120.0, 120.0, // dropdown hover color (63)
    100.0, 100.0, 100.0, // dropdown triangle color (66)
};

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
    TT_COLOR_SWITCH_OFF = 33,
    TT_COLOR_SWITCH_CIRCLE_OFF = 36,
    TT_COLOR_SWITCH_ON = 39,
    TT_COLOR_SWITCH_CIRCLE_ON = 42,
    TT_COLOR_DIAL = 45,
    TT_COLOR_DIAL_INNER = 48,
    TT_COLOR_SLIDER_BAR = 51,
    TT_COLOR_SLIDER_CIRCLE = 54,
    TT_COLOR_DROPDOWN = 57,
    TT_COLOR_DROPDOWN_SELECT = 60,
    TT_COLOR_DROPDOWN_HOVER = 63,
    TT_COLOR_DROPDOWN_TRIANGLE = 66,
} tt_theme_internal_t;

void tt_setColor(int32_t index) {
    turtlePenColor(tt_themeColors[index], tt_themeColors[index + 1], tt_themeColors[index + 2]);
}

void turtleToolsLightTheme() { // light theme preset (default)
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
        180.0, 180.0, 180.0, // switch color off (33)
        230.0, 230.0, 230.0, // switch circle color off (36)
        30.0, 30.0, 30.0,    // switch color on (39)
        0.0, 255.0, 255.0,   // switch circle color on (42)
        0.0, 0.0, 0.0,       // dial color (45)
        255.0, 255.0, 255.0, // dial inner circle color (48)
        120.0, 120.0, 120.0, // slider bar color (51)
        230.0, 230.0, 230.0, // slider circle color (54)
        160.0, 160.0, 160.0, // dropdown color (57)
        120.0, 120.0, 120.0, // dropdown select color (60)
        120.0, 120.0, 120.0, // dropdown hover color (63)
        100.0, 100.0, 100.0, // dropdown triangle color (66)
    };
    memcpy(tt_themeColors, tt_themeCopy, sizeof(tt_themeCopy));
}

void turtleToolsDarkTheme() { // dark theme preset
    tt_theme = TT_THEME_DARK;
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
        60.0, 60.0, 60.0,    // switch color off (33)
        80.0, 80.0, 80.0,    // switch circle color off (36)
        10.0, 10.0, 10.0,    // switch color on (39)
        50.0, 230.0, 30.0,   // switch circle color on (42)
        200.0, 200.0, 200.0, // dial color (45)
        30.0, 30.0, 30.0,    // dial inner circle color (48)
        10.0, 10.0, 10.0,    // slider bar color (51)
        230.0, 230.0, 230.0, // slider circle color (54)
        60.0, 60.0, 60.0,    // dropdown color (57)
        80.0, 80.0, 80.0,    // dropdown select color (60)
        80.0, 80.0, 80.0,    // dropdown hover color (63)
        160.0, 160.0, 160.0, // dropdown triangle color (66)
    };
    memcpy(tt_themeColors, tt_themeCopy, sizeof(tt_themeCopy));
}

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

tt_ribbon_t ribbonRender;

/* initialise ribbon */
int32_t ribbonInit(const char *filename) {
    tt_enabled.ribbonEnabled = 1;
    if (tt_enabled.turtleToolsEnabled == 0) {
        tt_enabled.turtleToolsEnabled = 1;
        tt_elements.all = list_init();
    }
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

    FILE *configFile = fopen(filename, "r"); // load from config file
    if (configFile == NULL) {
        printf("Error: file %s not found\n", filename);
        return -1;
    }
    list_t *sublist = list_init();
    int32_t checksum = 0;
    char throw[256]; // maximum size of any option or sub-option (characters)
    int32_t j = 0;
    list_clear(sublist);
    while (checksum != EOF) {
        checksum = fscanf(configFile, "%[^,\n]%*c,", throw);
        int32_t whitespace = 0;
        if (throw[0] == ' ') {
            whitespace += 1;
        } else {
            if (j != 0) {
                list_t *appendList = list_init();
                list_copy(appendList, sublist);
                list_clear(sublist);
                list_append(ribbonRender.options, (unitype) appendList, 'r');
            }
        }
        list_append(sublist, (unitype) (throw + whitespace), 's');
        j++;
    }
    list_pop(sublist);
    list_t *appendList = list_init();
    list_copy(appendList, sublist);
    list_clear(sublist);
    list_append(ribbonRender.options, (unitype) appendList, 'r');
    list_free(sublist);
    fclose(configFile);

    for (uint32_t i = 0; i < ribbonRender.options -> length; i++) {
        list_append(ribbonRender.lengths, (unitype) turtleTextGetStringLength(ribbonRender.options -> data[i].r -> data[0].s, 7 * ribbonRender.ribbonSize), 'd');
        double max = 0;
        for (uint32_t j = 1; j < ribbonRender.options -> data[i].r -> length; j++) {
            double cur = turtleTextGetStringLength(ribbonRender.options -> data[i].r -> data[j].s, 7 * ribbonRender.ribbonSize);
            if (cur > max) {
                max = cur;
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
    FILE *configFile = fopen(filename, "r");
    if (configFile == NULL) {
        printf("Error: file %s not found\n", filename);
        return -1;
    }
    char throw[256] = {1, 0}; // maximum size of message or option
    char *checksum = fgets(throw, 256, configFile); // read message
    throw[strlen(throw) - 1] = '\0'; // cull newline
    popup.message = strdup(throw);
    popup.options = list_init();
    while (checksum != NULL) {
        checksum = fgets(throw, 256, configFile);
        if (checksum != NULL) {
            if (throw[strlen(throw) - 1] == '\n') {
                throw[strlen(throw) - 1] = '\0'; // cull newline
            }
            list_append(popup.options, (unitype) strdup(throw), 's');
        }
    }
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

typedef struct {
    int32_t colorOverride;
    double color[24];
} tt_color_override_t;

typedef struct {
    double dialAnchorX;
    double dialAnchorY;
    int32_t dropdownLogicIndex;
} tt_globals_t;

tt_globals_t tt_globals;

typedef enum {
    TT_BUTTON_SHAPE_RECTANGLE = 0,
    TT_BUTTON_SHAPE_ROUNDED_RECTANGLE = 1,
    TT_BUTTON_SHAPE_CIRCLE = 2,
} tt_button_shape_t;

/* button */
typedef struct {
    tt_element_names_t element;
    tt_color_override_t color;
    double x;
    double y;
    double size;
    int32_t *variable; // 1 if button is being pressed, 0 otherwise
    char label[24];
    int32_t status;
    tt_button_shape_t shape;
} tt_button_t;

/* switch */
typedef struct {
    tt_element_names_t element;
    tt_color_override_t color;
    double x;
    double y;
    double size;
    int32_t *variable; // 1 if switch is flipped, 0 otherwise
    char label[24];
    int32_t status;
} tt_switch_t;

typedef enum {
    TT_DIAL_LINEAR = 0,
    TT_DIAL_LOG = 1,
    TT_DIAL_EXP = 2,
} tt_dial_type_t;

/* dial */
typedef struct {
    tt_element_names_t element;
    tt_color_override_t color;
    double x;
    double y;
    double size;
    double *variable; // value of dial
    char label[24];
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
    tt_color_override_t color;
    double x;
    double y;
    double size;
    double *variable; // value of slider
    char label[24];
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
    tt_color_override_t color;
    double x;
    double y;
    double size;
    double *variable; // value of slider
    int32_t status;
    tt_slider_type_t type;
    double length;
    double barPercentage; // percentage of scrollbar occupied by bar
    double barAnchor;
} tt_scrollbar_t;

typedef enum {
    TT_DROPDOWN_ALIGN_LEFT = 0,
    TT_DROPDOWN_ALIGN_CENTER = 1,
    TT_DROPDOWN_ALIGN_RIGHT = 2,
} tt_dropdown_align_t;

/* dropdown */
typedef struct {
    tt_element_names_t element;
    tt_color_override_t color;
    double x;
    double y;
    double size;
    int32_t *variable; // index of dropdown selected
    char label[24];
    list_t *options;
    uint32_t index;
    int32_t status;
    tt_dropdown_align_t align;
    double maxXfactor;
} tt_dropdown_t;

/* override colors with color array */
void tt_colorOverride(void *element, double *colors, uint32_t length) {
    ((tt_button_t *) element) -> color.colorOverride = 1;
    memcpy(((tt_button_t *) element) -> color.color, colors, length * sizeof(double));
}

/* initialise UI elements */
tt_button_t *buttonInit(char *label, int32_t *variable, tt_button_shape_t shape, double x, double y, double size) {
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
    if (label == NULL) {
        memcpy(buttonp -> label, "", strlen("") + 1);
    } else {
        memcpy(buttonp -> label, label, strlen(label) + 1);
    }
    buttonp -> color.colorOverride = 0;
    buttonp -> status = 0;
    buttonp -> shape = shape;
    buttonp -> x = x;
    buttonp -> y = y;
    buttonp -> size = size;
    *variable = 0; // button starts unpressed
    buttonp -> variable = variable;
    list_append(tt_elements.buttons, (unitype) (void *) buttonp, 'p');
    list_append(tt_elements.all, (unitype) (void *) buttonp, 'p');
    return buttonp;
}

/* create a switch */
tt_switch_t *switchInit(char *label, int32_t *variable, double x, double y, double size) {
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
    if (label == NULL) {
        memcpy(switchp -> label, "", strlen("") + 1);
    } else {
        memcpy(switchp -> label, label, strlen(label) + 1);
    }
    switchp -> color.colorOverride = 0;
    switchp -> status = 0;
    switchp -> x = x;
    switchp -> y = y;
    switchp -> size = size;
    switchp -> variable = variable;
    list_append(tt_elements.switches, (unitype) (void *) switchp, 'p');
    list_append(tt_elements.all, (unitype) (void *) switchp, 'p');
    return switchp;
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
    if (label == NULL) {
        memcpy(dialp -> label, "", strlen("") + 1);
    } else {
        memcpy(dialp -> label, label, strlen(label) + 1);
    }
    dialp -> color.colorOverride = 0;
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
    list_append(tt_elements.all, (unitype) (void *) dialp, 'p');
    return dialp;
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
    if (label == NULL) {
        memcpy(sliderp -> label, "", strlen("") + 1);
    } else {
        memcpy(sliderp -> label, label, strlen(label) + 1);
    }
    sliderp -> color.colorOverride = 0;
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
    list_append(tt_elements.all, (unitype) (void *) sliderp, 'p');
    return sliderp;
}

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
    scrollbarp -> color.colorOverride = 0;
    scrollbarp -> status = 0;
    scrollbarp -> type = type;
    scrollbarp -> x = x;
    scrollbarp -> y = y;
    scrollbarp -> size = size;
    scrollbarp -> length = length;
    scrollbarp -> barPercentage = barPercentage;
    scrollbarp -> variable = variable;
    list_append(tt_elements.scrollbars, (unitype) (void *) scrollbarp, 'p');
    list_append(tt_elements.all, (unitype) (void *) scrollbarp, 'p');
    return scrollbarp;
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
    if (label == NULL) {
        memcpy(dropdownp -> label, "", strlen("") + 1);
    } else {
        memcpy(dropdownp -> label, label, strlen(label) + 1);
    }
    dropdownp -> color.colorOverride = 0;
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
    list_append(tt_elements.all, (unitype) (void *) dropdownp, 'p');
    return dropdownp;
}

void buttonUpdate() {
    for (uint32_t i = 0; i < tt_elements.buttons -> length; i++) {
        tt_button_t *buttonp = (tt_button_t *) (tt_elements.buttons -> data[i].p);
        double buttonX = buttonp -> x;
        double buttonY = buttonp -> y;
        double buttonWidth = turtleTextGetUnicodeLength((unsigned char *) buttonp -> label, buttonp -> size) * 1.1;
        double buttonHeight = buttonp -> size * 1.75;
        if (buttonp -> status == 0) {
            tt_setColor(TT_COLOR_BUTTON);
        } else {
            tt_setColor(TT_COLOR_BUTTON_SELECT);
        }
        turtleRectangle(buttonX - buttonWidth / 2, buttonY - buttonHeight / 2, buttonX + buttonWidth / 2, buttonY + buttonHeight / 2);
        tt_setColor(TT_COLOR_TEXT_ALTERNATE);
        turtleTextWriteUnicode((unsigned char *) buttonp -> label, buttonX, buttonY, buttonp -> size - 1, 50);
        if (turtleMouseDown()) {
            if (buttonp -> status < 0) {
                buttonp -> status *= -1;
            }
        } else {
            if (turtle.mouseX > buttonX - buttonWidth / 2 && turtle.mouseX < buttonX + buttonWidth / 2 && turtle.mouseY > buttonY - buttonHeight / 2 && turtle.mouseY < buttonY + buttonHeight / 2) {
                buttonp -> status = -1;
            } else {
                buttonp -> status = 0;
            }
        }
        *(buttonp -> variable) = 0;
        if (buttonp -> status > 0) {
            *(buttonp -> variable) = 1;
            buttonp -> status = 0;
        }
    }
}

void switchUpdate() {
    for (uint32_t i = 0; i < tt_elements.switches -> length; i++) {
        tt_switch_t *switchp = (tt_switch_t *) (tt_elements.switches -> data[i].p);
        double switchX = switchp -> x;
        double switchY = switchp -> y;
        tt_setColor(TT_COLOR_TEXT);
        turtleTextWriteUnicode((unsigned char *) switchp -> label, switchX, switchY + 1.6 * switchp -> size, switchp -> size - 1, 50);
        if (*(switchp -> variable)) {
            tt_setColor(TT_COLOR_SWITCH_ON);
        } else {
            tt_setColor(TT_COLOR_SWITCH_OFF);
        }
        turtlePenSize(switchp -> size * 1.2);
        turtleGoto(switchX - switchp -> size * 0.8, switchY);
        turtlePenDown();
        turtleGoto(switchX + switchp -> size * 0.8, switchY);
        turtlePenUp();
        turtlePenSize(switchp -> size);
        if (*(switchp -> variable)) {
            tt_setColor(TT_COLOR_SWITCH_CIRCLE_ON);
            turtleGoto(switchX + switchp -> size * 0.8, switchY);
        } else {
            tt_setColor(TT_COLOR_SWITCH_CIRCLE_OFF);
            turtleGoto(switchX - switchp -> size * 0.8, switchY);
        }
        turtlePenDown();
        turtlePenUp();
        if (turtleMouseDown()) {
            if (switchp -> status < 0) {
                switchp -> status *= -1;
            }
        } else {
            if (turtle.mouseX > switchX - switchp -> size * 1.35 && turtle.mouseX < switchX + switchp -> size * 1.35 && turtle.mouseY > switchY - switchp -> size * 0.6 && turtle.mouseY < switchY + switchp -> size * 0.6) {
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
        tt_setColor(TT_COLOR_TEXT);
        turtleTextWriteUnicode((unsigned char *) dialp -> label, dialp -> x, dialp -> y + 1.9 * dialp -> size, dialp -> size - 1, 50);
        turtlePenSize(dialp -> size * 2);
        double dialX = dialp -> x;
        double dialY = dialp -> y;
        turtleGoto(dialX, dialY);
        tt_setColor(TT_COLOR_DIAL);
        turtlePenDown();
        turtlePenUp();
        turtlePenSize(dialp -> size * 2 * 0.8);
        tt_setColor(TT_COLOR_DIAL_INNER);
        turtlePenDown();
        turtlePenUp();
        tt_setColor(TT_COLOR_DIAL);
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
        char bubble[24];
        double rounded = round(*(dialp -> variable) * dialp -> renderNumberFactor);
        sprintf(bubble, "%.0lf", rounded);
        tt_setColor(TT_COLOR_TEXT);
        turtleTextWriteString(bubble, dialX + dialp -> size + 3, dialY, 4, 0);
    }
}

void sliderUpdate() {
    for (uint32_t i = 0; i < tt_elements.sliders -> length; i++) {
        tt_slider_t *sliderp = (tt_slider_t *) (tt_elements.sliders -> data[i].p);
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
        if (sliderp -> color.colorOverride) {
            turtlePenColor(sliderp -> color.color[0], sliderp -> color.color[1], sliderp -> color.color[2]);
        } else {
            tt_setColor(TT_COLOR_TEXT);
        }
        turtleTextWriteUnicode((unsigned char *) sliderp -> label, sliderp -> x + sliderOffsetXFactor, sliderp -> y + sliderOffsetYFactor, sliderp -> size - 1, sliderAlignFactor);
        turtlePenSize(sliderp -> size * 1.2);
        turtleGoto(sliderXLeft, sliderYLeft);
        if (sliderp -> color.colorOverride) {
            turtlePenColor(sliderp -> color.color[3], sliderp -> color.color[4], sliderp -> color.color[5]);
        } else {
            tt_setColor(TT_COLOR_SLIDER_BAR);
        }
        turtlePenDown();
        turtleGoto(sliderXRight, sliderYRight);
        turtlePenUp();
        turtlePenSize(sliderp -> size);
        if (sliderp -> color.colorOverride) {
            turtlePenColor(sliderp -> color.color[6], sliderp -> color.color[7], sliderp -> color.color[8]);
        } else {
            tt_setColor(TT_COLOR_SLIDER_CIRCLE);
        }
        if (sliderp -> type == TT_SLIDER_HORIZONTAL) {
            turtleGoto(sliderXLeft + (sliderXRight - sliderXLeft) * (*(sliderp -> variable) - sliderp -> range[0]) / (sliderp -> range[1] - sliderp -> range[0]), sliderYLeft);
        } else if (sliderp -> type == TT_SLIDER_VERTICAL) {
            turtleGoto(sliderXLeft, sliderYLeft + (sliderYRight - sliderYLeft) * (*(sliderp -> variable) - sliderp -> range[0]) / (sliderp -> range[1] - sliderp -> range[0]));
        }
        turtlePenDown();
        turtlePenUp();
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
        if (sliderp -> renderNumberFactor != 0) {
            char bubble[24];
            double rounded = round(*(sliderp -> variable) * sliderp -> renderNumberFactor);
            sprintf(bubble, "%.0lf", rounded);
            if (sliderp -> color.colorOverride) {
                turtlePenColor(sliderp -> color.color[6], sliderp -> color.color[7], sliderp -> color.color[8]);
            } else {
                tt_setColor(TT_COLOR_TEXT);
            }
            turtleTextWriteString(bubble, sliderp -> x + sliderOffsetXFactorSmall, sliderp -> y + sliderOffsetYFactorSmall, 4, sliderAlignFactor);
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
        double scrollbarTop = scrollbarp -> y + scrollbarp -> length / 2;
        double scrollbarBottom = scrollbarp -> y - scrollbarp -> length / 2;
        double dragTop = scrollbarTop - (*(scrollbarp -> variable)) / 100 * (scrollbarp -> length * (1 - scrollbarp -> barPercentage / 100));
        double dragBottom = dragTop - (scrollbarp -> length * scrollbarp -> barPercentage / 100);
        if (scrollbarp -> type == TT_SLIDER_HORIZONTAL) {
            turtlePenSize(scrollbarp -> size * 1);
        } else if (scrollbarp -> type == TT_SLIDER_VERTICAL) {
            turtlePenSize(scrollbarp -> size * 1);
            if (scrollbarp -> color.colorOverride) {
                turtlePenColor(scrollbarp -> color.color[3], scrollbarp -> color.color[4], scrollbarp -> color.color[5]);
            } else {
                tt_setColor(TT_COLOR_SLIDER_BAR);
            }
            turtleGoto(scrollbarp -> x, scrollbarTop);
            turtlePenDown();
            turtleGoto(scrollbarp -> x, scrollbarBottom);
            turtlePenUp();
            turtlePenSize(scrollbarp -> size * 0.8);
            if (scrollbarp -> color.colorOverride) {
                turtlePenColor(scrollbarp -> color.color[6], scrollbarp -> color.color[7], scrollbarp -> color.color[8]);
            } else {
                tt_setColor(TT_COLOR_SLIDER_CIRCLE);
            }
            turtleGoto(scrollbarp -> x, dragTop);
            turtlePenDown();
            turtleGoto(scrollbarp -> x, dragBottom);
            turtlePenUp();
            if (turtleMouseDown()) {
                if (scrollbarp -> status < 0) {
                    scrollbarp -> barAnchor = (scrollbarp -> length * scrollbarp -> barPercentage / 100) / 2;
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
                *(scrollbarp -> variable) = (scrollbarTop - turtle.mouseY - scrollbarp -> barAnchor) / (scrollbarp -> length * (1 - scrollbarp -> barPercentage / 100)) * 100;
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

void dropdownUpdate() {
    int32_t logicIndex = -1;
    for (uint32_t i = 0; i < tt_elements.dropdowns -> length; i++) {
        tt_dropdown_t *dropdownp = (tt_dropdown_t *) (tt_elements.dropdowns -> data[i].p);
        /* render dropdown default position */
        double dropdownX = dropdownp -> x;
        double dropdownY = dropdownp -> y;
        double xfactor = turtleTextGetUnicodeLength((unsigned char *) dropdownp -> options -> data[dropdownp -> index].s, dropdownp -> size - 1);
        double itemHeight = (dropdownp -> size * 1.5);
        double dropdownXFactor[2];
        double dropdownMaxXFactor[2];
        double dropdownAlignFactor;
        if (strlen(dropdownp -> label) > 0) {
            tt_setColor(TT_COLOR_TEXT);
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
            tt_setColor(TT_COLOR_DROPDOWN_SELECT);
            turtleRectangle(dropdownXFactor[0], dropdownY - dropdownp -> size * 0.7, dropdownXFactor[1] + dropdownp -> size, dropdownY + dropdownp -> size * 0.7);
        } else if (dropdownp -> status >= 1) {
            tt_setColor(TT_COLOR_DROPDOWN);
            turtleRectangle(dropdownMaxXFactor[0], dropdownY - dropdownp -> size * 0.7 - (dropdownp -> options -> length - 1) * itemHeight, dropdownMaxXFactor[1], dropdownY + dropdownp -> size * 0.7);
        } else {
            tt_setColor(TT_COLOR_DROPDOWN);
            turtleRectangle(dropdownXFactor[0], dropdownY - dropdownp -> size * 0.7, dropdownXFactor[1] + dropdownp -> size, dropdownY + dropdownp -> size * 0.7);
        }
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
                tt_setColor(TT_COLOR_DROPDOWN_HOVER);
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
            tt_setColor(TT_COLOR_TEXT_ALTERNATE);
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
        tt_setColor(TT_COLOR_TEXT_ALTERNATE);
        if (dropdownp -> align == TT_DROPDOWN_ALIGN_LEFT) {
            turtleTextWriteUnicode((unsigned char *) dropdownp -> options -> data[dropdownp -> index].s, dropdownXFactor[0] + dropdownp -> size / 5, dropdownY, dropdownp -> size - 1, dropdownAlignFactor);
        } else if (dropdownp -> align == TT_DROPDOWN_ALIGN_CENTER) {
            turtleTextWriteUnicode((unsigned char *) dropdownp -> options -> data[dropdownp -> index].s, (dropdownXFactor[0] + dropdownXFactor[1]) / 2, dropdownY, dropdownp -> size - 1, dropdownAlignFactor);
        } else if (dropdownp -> align == TT_DROPDOWN_ALIGN_RIGHT) {
            turtleTextWriteUnicode((unsigned char *) dropdownp -> options -> data[dropdownp -> index].s, dropdownXFactor[1] - dropdownp -> size * 0.6, dropdownY, dropdownp -> size - 1, dropdownAlignFactor);
        }
        tt_setColor(TT_COLOR_DROPDOWN_TRIANGLE);
        if (dropdownp -> status >= 1) {
            turtleTriangle(dropdownXFactor[1] + dropdownp -> size * 0.4, dropdownY + dropdownp -> size * 0.4, dropdownXFactor[1] + dropdownp -> size * 0.4, dropdownY - dropdownp -> size * 0.4, dropdownXFactor[1] - dropdownp -> size * 0.2, dropdownY);
        } else {
            turtleTriangle(dropdownXFactor[1] + dropdownp -> size * 0.6, dropdownY + dropdownp -> size * 0.3, dropdownXFactor[1] - dropdownp -> size * 0.2, dropdownY + dropdownp -> size * 0.3, dropdownXFactor[1] + dropdownp -> size * 0.2, dropdownY - dropdownp -> size * 0.3);
        }
    }
}

void turtleToolsUpdate() {
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
    if (tt_enabled.scrollbarEnabled) {
        scrollbarUpdate();
    }
    if (tt_enabled.dropdownEnabled) {
        dropdownUpdate();
    }
    if (tt_enabled.ribbonEnabled) {
        ribbonUpdate();
    }
    if (tt_enabled.popupEnabled) {
        popupUpdate();
    }
}

void turtleToolsUpdateUI() {
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
    if (tt_enabled.scrollbarEnabled) {
        scrollbarUpdate();
    }
    if (tt_enabled.dropdownEnabled) {
        dropdownUpdate();
    }
}

void turtleToolsUpdateRibbonPopup() {
    if (tt_enabled.ribbonEnabled) {
        ribbonUpdate();
    }
    if (tt_enabled.popupEnabled) {
        popupUpdate();
    }
}

#endif