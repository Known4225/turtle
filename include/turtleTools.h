/* turtleTools library includes:
ribbon: customisable top bar
popup: 
dropdown
button
dial
switch
slider (under development)
text box (under development)
*/

#ifndef TURTLETOOLS
#define TURTLETOOLS
#include "turtleText.h"

typedef enum {
    TT_THEME_LIGHT = 0,
    TT_THEME_DARK = 1,
} tt_theme_name_t;

tt_theme_name_t tt_theme;

typedef struct {
    int8_t ribbonEnabled;
    int8_t popupEnabled;
    int8_t buttonEnabled;
    int8_t switchEnabled;
    int8_t dialEnabled;
    int8_t dropdownEnabled;
} tt_enabled_t;

tt_enabled_t tt_enabled; // all start at 0 (global variable)

/* default colours (light theme) */
double tt_themeColors[] = {
    0.0, 0.0, 0.0,       // text color (0)
    0.0, 0.0, 0.0,       // text color alternate (3)
    200.0, 200.0, 200.0, // ribbon top bar color (6)
    140.0, 140.0, 140.0, // ribbon dropdown color (9)
    100.0, 100.0, 100.0, // ribbon select color (12)
    200.0, 200.0, 200.0, // popup box color (15)
    140.0, 140.0, 140.0, // popup boxes color (18)
    100.0, 100.0, 100.0, // popup boxes select color (21)
    200.0, 200.0, 200.0, // button color (24)
    160.0, 160.0, 160.0, // button select color (27)
    120.0, 120.0, 120.0, // switch color off (30)
    230.0, 230.0, 230.0, // switch circle color off (33)
    120.0, 120.0, 120.0, // switch color on (36)
    230.0, 230.0, 230.0, // switch circle color on (39)
    0.0, 0.0, 0.0,       // dial color (42)
    255.0, 255.0, 255.0, // dial inner circle color (45)
    160.0, 160.0, 160.0, // dropdown color (48)
    120.0, 120.0, 120.0, // dropdown select color (51)
    120.0, 120.0, 120.0, // dropdown hover color (54)
    100.0, 100.0, 100.0, // dropdown triangle color (57)
};

typedef enum {
    TT_COLOR_TEXT = 0,
    TT_COLOR_TEXT_ALTERNATE = 3,
    TT_COLOR_RIBBON_TOP = 6,
    TT_COLOR_RIBBON_DROPDOWN = 9,
    TT_COLOR_RIBBON_SELECT = 12,
    TT_COLOR_POPUP_BOX = 15,
    TT_COLOR_POPUP_BUTTON = 18,
    TT_COLOR_POPUP_BUTTON_SELECT = 21,
    TT_COLOR_BUTTON = 24,
    TT_COLOR_BUTTON_SELECT = 27,
    TT_COLOR_SWITCH_OFF = 30,
    TT_COLOR_SWITCH_CIRCLE_OFF = 33,
    TT_COLOR_SWITCH_ON = 36,
    TT_COLOR_SWITCH_CIRCLE_ON = 39,
    TT_COLOR_DIAL = 42,
    TT_COLOR_DIAL_INNER = 45,
    TT_COLOR_DROPDOWN = 48,
    TT_COLOR_DROPDOWN_SELECT = 51,
    TT_COLOR_DROPDOWN_HOVER = 54,
    TT_COLOR_DROPDOWN_TRIANGLE = 57,
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
        100.0, 100.0, 100.0, // ribbon select color (12)
        200.0, 200.0, 200.0, // popup box color (15)
        140.0, 140.0, 140.0, // popup boxes color (18)
        100.0, 100.0, 100.0, // popup boxes select color (21)
        200.0, 200.0, 200.0, // button color (24)
        160.0, 160.0, 160.0, // button select color (27)
        120.0, 120.0, 120.0, // switch color off (30)
        160.0, 160.0, 160.0, // switch circle color off (33)
        60.0, 60.0, 60.0,    // switch color on (36)
        50.0, 230.0, 30.0, // switch circle color on (39)
        0.0, 0.0, 0.0,       // dial color (42)
        255.0, 255.0, 255.0, // dial inner circle color (45)
        160.0, 160.0, 160.0, // dropdown color (48)
        120.0, 120.0, 120.0, // dropdown select color (51)
        120.0, 120.0, 120.0, // dropdown hover color (54)
        100.0, 100.0, 100.0, // dropdown triangle color (57)
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
        70.0, 70.0, 70.0,    // ribbon select color (12)
        10.0, 10.0, 10.0,    // popup box color (15)
        40.0, 40.0, 40.0,    // popup boxes color (18)
        60.0, 60.0, 60.0,    // popup boxes select color (21)
        80.0, 80.0, 80.0,    // button color (24)
        100.0, 100.0, 100.0, // button select color (27)
        60.0, 60.0, 60.0,    // switch color off (30)
        80.0, 80.0, 80.0,    // switch circle color off (33)
        10.0, 10.0, 10.0,    // switch color on (36)
        50.0, 230.0, 30.0,   // switch circle color on (39)
        200.0, 200.0, 200.0, // dial color (42)
        30.0, 30.0, 30.0,    // dial inner circle color (45)
        60.0, 60.0, 60.0,    // dropdown color (48)
        80.0, 80.0, 80.0,    // dropdown select color (51)
        80.0, 80.0, 80.0,    // dropdown hover color (54)
        160.0, 160.0, 160.0, // dropdown triangle color (57)
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
int32_t ribbonInit(GLFWwindow* window, const char *filename) {
    tt_enabled.ribbonEnabled = 1;
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
                list_copy(sublist, appendList);
                list_clear(sublist);
                list_append(ribbonRender.options, (unitype) appendList, 'r');
            }
        }
        list_append(sublist, (unitype) (throw + whitespace), 's');
        j++;
    }
    list_pop(sublist);
    list_t *appendList = list_init();
    list_copy(sublist, appendList);
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
                    tt_setColor(TT_COLOR_RIBBON_SELECT);
                    turtleQuad(xLeft, ribbonRender.bounds[3] - 10 - 15 * (j - 1) - ribbonRender.marginSize / 4.0, xRight, ribbonRender.bounds[3] - 10 - 15 * (j - 1) - ribbonRender.marginSize / 4.0, xRight, ribbonRender.bounds[3] - 10 - 15 * j - ribbonRender.marginSize / 3.0, xLeft, ribbonRender.bounds[3] - 10 - 15 * j - ribbonRender.marginSize / 3.0); // dropdown highlight
                    ribbonRender.subselect[0] = j;
                }
                tt_setColor(TT_COLOR_TEXT_ALTERNATE);
                turtleTextWriteUnicode((unsigned char *) ribbonRender.options -> data[i].r -> data[j].s, prevCutoff, 174.5 - j * 15, 7 * ribbonRender.ribbonSize, 0);
            }
        }
        cutoff += ribbonRender.lengths -> data[i * 2].d + ribbonRender.marginSize;
        if (turtle.mouseY > ribbonRender.bounds[3] - 10 && turtle.mouseY < ribbonRender.bounds[3] && turtle.mouseX > ribbonRender.bounds[0] + ribbonRender.marginSize / 2.0 && turtle.mouseX < cutoff - ribbonRender.marginSize / 2.0 && ribbonRender.mainselect[0] == -1) { // -217, -195, -164
            tt_setColor(TT_COLOR_DROPDOWN);
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
        turtleQuad(popup.minX, popup.minY, popup.minX, popup.maxY, popup.maxX, popup.maxY, popup.maxX, popup.minY);
        double textSize = 5;
        double textX = popup.minX + (popup.maxX - popup.minX) / 2;
        double textY = popup.maxY - textSize * 2;
        tt_setColor(TT_COLOR_TEXT);
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
                turtleQuad(textX - textSize, textY - textSize, textX + textSize + strLen, textY - textSize, textX + textSize + strLen, textY + textSize, textX - textSize, textY + textSize);
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
                turtleQuad(textX - textSize, textY - textSize, textX + textSize + strLen, textY - textSize, textX + textSize + strLen, textY + textSize, textX - textSize, textY + textSize);
            }
            tt_setColor(TT_COLOR_TEXT);
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
    double dialAnchorX;
    double dialAnchorY;
    int32_t dropdownLogicIndex;
} tt_globals_t;

tt_globals_t tt_globals;

typedef struct {
    list_t *buttons;
    list_t *switches;
    list_t *dials;
    list_t *dropdowns;
} tt_elements_t;

tt_elements_t tt_elements;

typedef enum {
    BUTTON_SHAPE_RECTANGLE = 0,
    BUTTON_SHAPE_ROUNDED_RECTANGLE = 1,
    BUTTON_SHAPE_CIRCLE = 2,
} tt_button_shape_t;

/* button */
typedef struct {
    char label[24];
    int32_t status;
    tt_button_shape_t shape;
    double size;
    double position[2]; // X, Y
    int32_t *variable; // 1 if button is being pressed, 0 otherwise
} tt_button_t;

typedef enum {
    DIAL_LINEAR = 0,
    DIAL_LOG = 1,
    DIAL_EXP = 2,
} tt_dial_type_t;

/* dial */
typedef struct {
    char label[24];
    int32_t status[2];
    tt_dial_type_t type;
    double size;
    double position[2]; // X, Y
    double range[2];
    double renderNumberFactor; // multiply rendered variable by this amount
    double defaultValue;
    double *variable; // value of dial
} tt_dial_t;

/* switch */
typedef struct {
    char label[24];
    int32_t status;
    double size;
    double position[2]; // X, Y
    int32_t *variable; // 1 if switch is flipped, 0 otherwise
} tt_switch_t;

typedef enum {
    TT_DROPDOWN_ALIGN_LEFT = 0,
    TT_DROPDOWN_ALIGN_CENTER = 1,
    TT_DROPDOWN_ALIGN_RIGHT = 2,
} tt_dropdown_align_t;

/* dropdown */
typedef struct {
    list_t *options;
    char label[24];
    uint32_t index;
    int32_t status;
    tt_dropdown_align_t align;
    double size;
    double position[2]; // X, Y
    double maxXfactor;
    int32_t *variable; // index of dropdown selected
} tt_dropdown_t;

/* initialise UI elements */
tt_button_t *buttonInit(char *label, int32_t *variable, double xOffset, double yOffset, double size, tt_button_shape_t shape) {
    if (tt_enabled.buttonEnabled == 0) {
        tt_enabled.buttonEnabled = 1;
        tt_elements.buttons = list_init();
    }
    tt_button_t *buttonp = malloc(sizeof(tt_button_t));
    if (label == NULL) {
        memcpy(buttonp -> label, "", strlen("") + 1);
    } else {
        memcpy(buttonp -> label, label, strlen(label) + 1);
    }
    buttonp -> status = 0;
    buttonp -> shape = shape;
    buttonp -> position[0] = xOffset;
    buttonp -> position[1] = yOffset;
    buttonp -> size = size;
    buttonp -> variable = variable;
    list_append(tt_elements.buttons, (unitype) (void *) buttonp, 'p');
    return buttonp;
}

tt_switch_t *switchInit(char *label, int32_t *variable, double xOffset, double yOffset, double size) {
    if (tt_enabled.switchEnabled == 0) {
        tt_enabled.switchEnabled = 1;
        tt_elements.switches = list_init();
    }
    tt_switch_t *switchp = malloc(sizeof(tt_switch_t));
    if (label == NULL) {
        memcpy(switchp -> label, "", strlen("") + 1);
    } else {
        memcpy(switchp -> label, label, strlen(label) + 1);
    }
    switchp -> status = 0;
    switchp -> position[0] = xOffset;
    switchp -> position[1] = yOffset;
    switchp -> size = size;
    switchp -> variable = variable;
    list_append(tt_elements.switches, (unitype) (void *) switchp, 'p');
    return switchp;
}

tt_dial_t *dialInit(char *label, double *variable, tt_dial_type_t type, double xOffset, double yOffset, double size, double bottom, double top, double renderNumberFactor) {
    if (tt_enabled.dialEnabled == 0) {
        tt_enabled.dialEnabled = 1;
        tt_elements.dials = list_init();
    }
    tt_dial_t *dialp = malloc(sizeof(tt_dial_t));
    if (label == NULL) {
        memcpy(dialp -> label, "", strlen("") + 1);
    } else {
        memcpy(dialp -> label, label, strlen(label) + 1);
    }
    dialp -> status[0] = 0;
    dialp -> type = type;
    dialp -> position[0] = xOffset;
    dialp -> position[1] = yOffset;
    dialp -> size = size;
    dialp -> range[0] = bottom;
    dialp -> range[1] = top;
    dialp -> variable = variable;
    dialp -> renderNumberFactor = renderNumberFactor;
    dialp -> defaultValue = *variable;
    list_append(tt_elements.dials, (unitype) (void *) dialp, 'p');
    return dialp;
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

tt_dropdown_t *dropdownInit(char *label, list_t *options, int32_t *variable, double xOffset, double yOffset, double size, tt_dropdown_align_t align) {
    if (tt_enabled.dropdownEnabled == 0) {
        tt_globals.dropdownLogicIndex = -1;
        tt_enabled.dropdownEnabled = 1;
        tt_elements.dropdowns = list_init();
    }
    tt_dropdown_t *dropdownp = malloc(sizeof(tt_dropdown_t));
    if (label == NULL) {
        memcpy(dropdownp -> label, "", strlen("") + 1);
    } else {
        memcpy(dropdownp -> label, label, strlen(label) + 1);
    }
    dropdownp -> options = options;
    dropdownp -> index = *variable;
    dropdownp -> status = 0;
    dropdownp -> align = align;
    dropdownp -> position[0] = xOffset;
    dropdownp -> position[1] = yOffset;
    dropdownp -> size = size;
    dropdownp -> variable = variable;
    dropdownCalculateMax(dropdownp);
    list_append(tt_elements.dropdowns, (unitype) (void *) dropdownp, 'p');
    return dropdownp;
}

void buttonUpdate() {
    for (uint32_t i = 0; i < tt_elements.buttons -> length; i++) {
        tt_button_t *buttonp = (tt_button_t *) (tt_elements.buttons -> data[i].p);
        double buttonX = buttonp -> position[0];
        double buttonY = buttonp -> position[1];
        double buttonWidth = turtleTextGetUnicodeLength((unsigned char *) buttonp -> label, buttonp -> size) * 1.1;
        double buttonHeight = buttonp -> size * 1.75;
        if (buttonp -> status == 0) {
            tt_setColor(TT_COLOR_BUTTON);
        } else {
            tt_setColor(TT_COLOR_BUTTON_SELECT);
        }
        turtleRectangle(buttonX - buttonWidth / 2, buttonY - buttonHeight / 2, buttonX + buttonWidth / 2, buttonY + buttonHeight / 2);
        tt_setColor(TT_COLOR_TEXT);
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
        double switchX = switchp -> position[0];
        double switchY = switchp -> position[1];
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
        turtleTextWriteUnicode((unsigned char *) dialp -> label, dialp -> position[0], dialp -> position[1] + 1.9 * dialp -> size, dialp -> size - 1, 50);
        turtlePenSize(dialp -> size * 2);
        double dialX = dialp -> position[0];
        double dialY = dialp -> position[1];
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
        if (dialp -> type == DIAL_LOG) {
            dialAngle = pow(360, (*(dialp -> variable) - dialp -> range[0]) / (dialp -> range[1] - dialp -> range[0]));
        } else if (dialp -> type == DIAL_LINEAR) {
            dialAngle = (*(dialp -> variable) - dialp -> range[0]) / (dialp -> range[1] - dialp -> range[0]) * 360;
        } else if (dialp -> type == DIAL_EXP) {
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
        } else {
            if (turtle.mouseX > dialX - dialp -> size && turtle.mouseX < dialX + dialp -> size && turtle.mouseY > dialY - dialp -> size && turtle.mouseY < dialY + dialp -> size) {
                dialp -> status[0] = -1;
            } else {
                dialp -> status[0] = 0;
            }
        }
        if (turtleMouseRight()) {
            if (dialp -> status[0] < 0) {
                *(dialp -> variable) = dialp -> defaultValue;
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
            if (dialp -> type == DIAL_LOG) {
                *(dialp -> variable) = round(dialp -> range[0] + (dialp -> range[1] - dialp -> range[0]) * (log(dialAngle) / log(360)));
            } else if (dialp -> type == DIAL_LINEAR) {
                *(dialp -> variable) = round(dialp -> range[0] + ((dialp -> range[1] - dialp -> range[0]) * dialAngle / 360));
            } else if (dialp -> type == DIAL_EXP) {
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

void dropdownUpdate() {
    int32_t logicIndex = -1;
    for (uint32_t i = 0; i < tt_elements.dropdowns -> length; i++) {
        tt_dropdown_t *dropdown = (tt_dropdown_t *) (tt_elements.dropdowns -> data[i].p);
        /* render dropdown default position */
        double dropdownX = dropdown -> position[0];
        double dropdownY = dropdown -> position[1];
        double xfactor = turtleTextGetUnicodeLength((unsigned char *) dropdown -> options -> data[dropdown -> index].s, dropdown -> size - 1);
        double itemHeight = (dropdown -> size * 1.5);
        double dropdownXFactor[2];
        double dropdownMaxXFactor[2];
        double dropdownAlignFactor;
        if (strlen(dropdown -> label) > 0) {
            tt_setColor(TT_COLOR_TEXT);
            if (dropdown -> align == TT_DROPDOWN_ALIGN_LEFT) {
                dropdownXFactor[0] = dropdownX;
                dropdownXFactor[1] = dropdownX + xfactor + 10;
                dropdownMaxXFactor[0] = dropdownX;
                dropdownMaxXFactor[1] = dropdownX + dropdown -> maxXfactor + 4;
                if (dropdownXFactor[1] > dropdownMaxXFactor[1]) {
                    dropdownMaxXFactor[1] = dropdownXFactor[1] + 10;
                }
                dropdownAlignFactor = 0;
                turtleTextWriteUnicode((unsigned char *) dropdown -> label, dropdownX + 2, dropdownY + 1.6 * dropdown -> size, dropdown -> size - 1, dropdownAlignFactor);
            } else if (dropdown -> align == TT_DROPDOWN_ALIGN_RIGHT) {
                dropdownXFactor[0] = dropdownX - xfactor - 20;
                dropdownXFactor[1] = dropdownX - 10;
                dropdownMaxXFactor[0] = dropdownX - dropdown -> maxXfactor - 20;
                dropdownMaxXFactor[1] = dropdownX;
                dropdownAlignFactor = 100;
                turtleTextWriteUnicode((unsigned char *) dropdown -> label, dropdownX - 2, dropdownY + 1.6 * dropdown -> size, dropdown -> size - 1, dropdownAlignFactor);
            } else {
                dropdownXFactor[0] = dropdownX - xfactor / 2 - 10;
                dropdownXFactor[1] = dropdownX + xfactor / 2;
                dropdownMaxXFactor[0] = dropdownX - dropdown -> maxXfactor / 2 - 10;
                dropdownMaxXFactor[1] = dropdownX + dropdown -> maxXfactor / 2 - 5;
                if (dropdownXFactor[1] > dropdownMaxXFactor[1]) {
                    dropdownMaxXFactor[1] = dropdownXFactor[1] + 10;
                }
                dropdownAlignFactor = 50;
                turtleTextWriteUnicode((unsigned char *) dropdown -> label, dropdownX, dropdownY + 1.6 * dropdown -> size, dropdown -> size - 1, dropdownAlignFactor);
            }
        }
        logicIndex = tt_globals.dropdownLogicIndex;
        if (dropdown -> status == -1) {
            tt_setColor(TT_COLOR_DROPDOWN_SELECT);
            turtleRectangle(dropdownXFactor[0], dropdownY - dropdown -> size * 0.7, dropdownXFactor[1] + 10, dropdownY + dropdown -> size * 0.7);
        } else if (dropdown -> status >= 1) {
            tt_setColor(TT_COLOR_DROPDOWN);
            turtleRectangle(dropdownMaxXFactor[0], dropdownY - dropdown -> size * 0.7 - (dropdown -> options -> length - 1) * itemHeight, dropdownMaxXFactor[1], dropdownY + dropdown -> size * 0.7);
        } else {
            tt_setColor(TT_COLOR_DROPDOWN);
            turtleRectangle(dropdownXFactor[0], dropdownY - dropdown -> size * 0.7, dropdownXFactor[1] + 10, dropdownY + dropdown -> size * 0.7);
        }
        if (turtle.mouseX > dropdownXFactor[0] && turtle.mouseX < dropdownXFactor[1] + 10 && turtle.mouseY > dropdownY - dropdown -> size * 0.7 && turtle.mouseY < dropdownY + dropdown -> size * 0.7) {
            if (!turtleMouseDown() && dropdown -> status == 0) {
                dropdown -> status = -1;
                logicIndex = -1;
            }
        } else {
            if (dropdown -> status == -1) {
                dropdown -> status = 0;
                logicIndex = -1;
            }
        }
        if (dropdown -> status == -1) {
            if ((int32_t) i > logicIndex && turtleMouseDown()) {
                dropdown -> status = 1;
            }
        }
        if (dropdown -> status == 1) {
            if (!turtleMouseDown()) {
                dropdown -> status = 2;
                logicIndex = -1;
            }
        }
        if (dropdown -> status == -2) {
            if (!turtleMouseDown()) {
                dropdown -> status = 0;
                logicIndex = -1;
            }
        }

        if (dropdown -> status == 2 || dropdown -> status == 1) {
            if (turtle.mouseX > dropdownMaxXFactor[0] && turtle.mouseX < dropdownMaxXFactor[1] && turtle.mouseY > dropdownY - dropdown -> size * 0.7 - (dropdown -> options -> length - 1) * itemHeight && turtle.mouseY < dropdownY + dropdown -> size * 0.7) {
                uint32_t selected = round((dropdownY - turtle.mouseY) / itemHeight);
                tt_setColor(TT_COLOR_DROPDOWN_HOVER);
                turtleRectangle(dropdownMaxXFactor[0], dropdownY - dropdown -> size * 0.7 - selected * itemHeight, dropdownMaxXFactor[1], dropdownY + dropdown -> size * 0.7 - selected * itemHeight);
                if (turtleMouseDown() && dropdown -> status == 2) {
                    if (selected != 0) {
                        if (dropdown -> index >= selected) {
                            dropdown -> index = selected - 1;
                        } else {
                            dropdown -> index = selected;
                        }
                        *dropdown -> variable = dropdown -> index;
                    }
                    dropdown -> status = -2;
                }
            } else {
                if (turtleMouseDown()) {
                    dropdown -> status = 0;
                }
            }
            tt_setColor(TT_COLOR_TEXT);
            int32_t renderIndex = 1;
            for (uint32_t i = 0; i < dropdown -> options -> length; i++) {
                if (i != dropdown -> index) {
                    if (dropdown -> align == TT_DROPDOWN_ALIGN_LEFT) {
                        turtleTextWriteUnicode((unsigned char *) dropdown -> options -> data[i].s, dropdownMaxXFactor[0] + 2, dropdownY - renderIndex * itemHeight, dropdown -> size - 1, dropdownAlignFactor);
                    } else if (dropdown -> align == TT_DROPDOWN_ALIGN_RIGHT) {
                        turtleTextWriteUnicode((unsigned char *) dropdown -> options -> data[i].s, dropdownMaxXFactor[1] - 16, dropdownY - renderIndex * itemHeight, dropdown -> size - 1, dropdownAlignFactor);
                    } else {
                        turtleTextWriteUnicode((unsigned char *) dropdown -> options -> data[i].s, (dropdownMaxXFactor[0] + dropdownMaxXFactor[1]) / 2, dropdownY - renderIndex * itemHeight, dropdown -> size - 1, dropdownAlignFactor);
                    }
                    renderIndex++;
                }
            }
        }
        if (dropdown -> status >= 1) {
            logicIndex = i;
        }
        tt_globals.dropdownLogicIndex = logicIndex;
        tt_setColor(TT_COLOR_TEXT);
        if (dropdown -> align == TT_DROPDOWN_ALIGN_LEFT) {
            turtleTextWriteUnicode((unsigned char *) dropdown -> options -> data[dropdown -> index].s, dropdownXFactor[0] + 2, dropdownY, dropdown -> size - 1, dropdownAlignFactor);
        } else if (dropdown -> align == TT_DROPDOWN_ALIGN_RIGHT) {
            turtleTextWriteUnicode((unsigned char *) dropdown -> options -> data[dropdown -> index].s, dropdownXFactor[1] - 6, dropdownY, dropdown -> size - 1, dropdownAlignFactor);
        } else {
            turtleTextWriteUnicode((unsigned char *) dropdown -> options -> data[dropdown -> index].s, (dropdownXFactor[0] + dropdownXFactor[1]) / 2, dropdownY, dropdown -> size - 1, dropdownAlignFactor);
        }
        tt_setColor(TT_COLOR_DROPDOWN_TRIANGLE);
        if (dropdown -> status >= 1) {
            turtleTriangle(dropdownXFactor[1] + 4, dropdownY + 4, dropdownXFactor[1] + 4, dropdownY - 4, dropdownXFactor[1] - 2, dropdownY);
        } else {
            turtleTriangle(dropdownXFactor[1] + 6, dropdownY + 3, dropdownXFactor[1] - 2, dropdownY + 3, dropdownXFactor[1] + 2, dropdownY - 3);
        }
    }
}

void turtleToolsUpdate() {
    if (tt_enabled.popupEnabled) {
        popupUpdate();
    }
    if (tt_enabled.buttonEnabled) {
        buttonUpdate();
    }
    if (tt_enabled.switchEnabled) {
        switchUpdate();
    }
    if (tt_enabled.dialEnabled) {
        dialUpdate();
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

#endif