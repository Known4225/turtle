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

typedef enum {
    TT_THEME_LIGHT = 0,
    TT_THEME_DARK = 1,
} tt_theme_t;

tt_theme_t tt_theme;

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
double tt_themeColors[96] = {
    200.0, 200.0, 200.0, // top bar color (0)
    140.0, 140.0, 140.0, // dropdown color (3)
    100.0, 100.0, 100.0, // select color (6)
    0.0, 0.0, 0.0,       // text color (9)
    200.0, 200.0, 200.0, // popup color (12)
    140.0, 140.0, 140.0, // boxes color (15)
    100.0, 100.0, 100.0, // boxes highlight color (18)
    0.0, 0.0, 0.0,       // text color (21)
    255.0, 255.0, 255.0, // background color (24)
    195.0, 195.0, 195.0, // window color (27)
    0.0, 0.0, 0.0,       // text color (30)
};

void turtleToolsLightTheme() { // light theme preset (default)
    tt_theme = TT_THEME_LIGHT;
    tt_themeColors[0] =  200.0; // top bar color
    tt_themeColors[1] =  200.0;
    tt_themeColors[2] =  200.0;
    tt_themeColors[3] =  140.0; // dropdown color
    tt_themeColors[4] =  140.0;
    tt_themeColors[5] =  140.0;
    tt_themeColors[6] =  100.0; // select color
    tt_themeColors[7] =  100.0;
    tt_themeColors[8] =  100.0;
    tt_themeColors[9] =  0.0;   // text color
    tt_themeColors[10] = 0.0;
    tt_themeColors[11] = 0.0;
    tt_themeColors[12] = 200.0; // popup color, 
    tt_themeColors[13] = 200.0;
    tt_themeColors[14] = 200.0;
    tt_themeColors[15] = 140.0; // boxes color,
    tt_themeColors[16] = 140.0;
    tt_themeColors[17] = 140.0;
    tt_themeColors[18] = 100.0; // boxes highlight color
    tt_themeColors[19] = 100.0;
    tt_themeColors[20] = 100.0;
    tt_themeColors[21] = 0.0;   // text color
    tt_themeColors[22] = 0.0;
    tt_themeColors[23] = 0.0;
    tt_themeColors[24] = 255.0; // background color
    tt_themeColors[25] = 255.0;
    tt_themeColors[26] = 255.0;
    tt_themeColors[27] = 195.0; // window color
    tt_themeColors[28] = 195.0;
    tt_themeColors[29] = 195.0;
    tt_themeColors[30] = 200.0; // text color
    tt_themeColors[31] = 200.0;
    tt_themeColors[32] = 200.0;
}

void turtleToolsDarkTheme() { // dark theme preset
    tt_theme = TT_THEME_DARK;
    tt_themeColors[0] =  70.0;  // top bar color
    tt_themeColors[1] =  70.0;
    tt_themeColors[2] =  70.0;
    tt_themeColors[3] =  80.0;  // dropdown color
    tt_themeColors[4] =  80.0;
    tt_themeColors[5] =  80.0;
    tt_themeColors[6] =  70.0;  // select color
    tt_themeColors[7] =  70.0;
    tt_themeColors[8] =  70.0;
    tt_themeColors[9] =  160.0; // text color
    tt_themeColors[10] = 160.0;
    tt_themeColors[11] = 160.0;
    tt_themeColors[12] = 10.0;  // popup color, 
    tt_themeColors[13] = 10.0;
    tt_themeColors[14] = 10.0;
    tt_themeColors[15] = 40.0;  // boxes color,
    tt_themeColors[16] = 40.0;
    tt_themeColors[17] = 40.0;
    tt_themeColors[18] = 60.0;  // boxes highlight color
    tt_themeColors[19] = 60.0;
    tt_themeColors[20] = 60.0;
    tt_themeColors[21] = 160.0; // text color
    tt_themeColors[22] = 160.0;
    tt_themeColors[23] = 160.0;
    tt_themeColors[24] = 60.0;  // background color
    tt_themeColors[25] = 60.0;
    tt_themeColors[26] = 60.0;
    tt_themeColors[27] = 10.0;  // window color
    tt_themeColors[28] = 10.0;
    tt_themeColors[29] = 10.0;
    tt_themeColors[30] = 200.0; // text color
    tt_themeColors[31] = 200.0;
    tt_themeColors[32] = 200.0;
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

int ribbonInit(GLFWwindow* window, const char *filename) { // read from config file
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

void ribbonUpdate() {
    char shapeSave = turtle.penshape;
    double sizeSave = turtle.pensize;
    turtlePenSize(20);
    turtlePenShape("square");
    turtleGetMouseCoords(); // get the mouse coordinates (turtle.mouseX, turtle.mouseY)
    turtleQuad(ribbonRender.bounds[0], ribbonRender.bounds[3] - 10, ribbonRender.bounds[2], ribbonRender.bounds[3] - 10, ribbonRender.bounds[2], ribbonRender.bounds[3], ribbonRender.bounds[0], ribbonRender.bounds[3], tt_themeColors[0], tt_themeColors[1], tt_themeColors[2], 50.0); // render ribbon
    turtlePenColor(tt_themeColors[9], tt_themeColors[10], tt_themeColors[11]); // text colour
    double cutoff = ribbonRender.bounds[0] + ribbonRender.marginSize;
    ribbonRender.mainselect[0] = -1;
    ribbonRender.subselect[0] = -1;
    for (uint32_t i = 0; i < ribbonRender.options -> length; i++) {
        double prevCutoff = cutoff;
        if (i == ribbonRender.mainselect[2]) {
            double xLeft = prevCutoff - ribbonRender.marginSize / 2.0;
            double xRight = prevCutoff + ribbonRender.lengths -> data[i * 2 + 1].d + ribbonRender.marginSize / 2.0;
            double yDown = ribbonRender.bounds[3] - 10 - 15 * (ribbonRender.options -> data[i].r -> length - 1) - ribbonRender.marginSize / 2.0;
            turtleQuad(xLeft, ribbonRender.bounds[3] - 10, xRight, ribbonRender.bounds[3] - 10, xRight, yDown, xLeft, yDown, tt_themeColors[3], tt_themeColors[4], tt_themeColors[5], 0.0); // ribbon highlight
            for (uint32_t j = 1; j < ribbonRender.options -> data[i].r -> length; j++) {
                if (turtle.mouseY > ribbonRender.bounds[3] - 10 - 15 * j - ribbonRender.marginSize / 4.0 && turtle.mouseY < ribbonRender.bounds[3] - 10 && turtle.mouseX > xLeft && turtle.mouseX < xRight && ribbonRender.subselect[0] == -1) {
                    turtleQuad(xLeft, ribbonRender.bounds[3] - 10 - 15 * (j - 1) - ribbonRender.marginSize / 4.0, xRight, ribbonRender.bounds[3] - 10 - 15 * (j - 1) - ribbonRender.marginSize / 4.0, xRight, ribbonRender.bounds[3] - 10 - 15 * j - ribbonRender.marginSize / 3.0, xLeft, ribbonRender.bounds[3] - 10 - 15 * j - ribbonRender.marginSize / 3.0, tt_themeColors[6], tt_themeColors[7], tt_themeColors[8], 0.0); // dropdown highlight
                    ribbonRender.subselect[0] = j;
                }
                turtleTextWriteString(ribbonRender.options -> data[i].r -> data[j].s, prevCutoff, 174.5 - j * 15, 7 * ribbonRender.ribbonSize, 0);
            }
        }
        cutoff += ribbonRender.lengths -> data[i * 2].d + ribbonRender.marginSize;
        if (turtle.mouseY > ribbonRender.bounds[3] - 10 && turtle.mouseY < ribbonRender.bounds[3] && turtle.mouseX > ribbonRender.bounds[0] + ribbonRender.marginSize / 2.0 && turtle.mouseX < cutoff - ribbonRender.marginSize / 2.0 && ribbonRender.mainselect[0] == -1) { // -217, -195, -164
            turtleQuad(prevCutoff - ribbonRender.marginSize / 2.0, 179, cutoff - ribbonRender.marginSize / 2.0, 179, cutoff - ribbonRender.marginSize / 2.0, 171, prevCutoff - ribbonRender.marginSize / 2.0, 171, tt_themeColors[3], tt_themeColors[4], tt_themeColors[5], 0.0); // render dropdown
            ribbonRender.mainselect[0] = i;
        }
        turtleTextWriteString(ribbonRender.options -> data[i].r -> data[0].s, prevCutoff, 174.5, 7 * ribbonRender.ribbonSize, 0);
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

int popupInit(const char *filename, double minX, double minY, double maxX, double maxY) {
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

void popupUpdate() {
    if (turtle.close == 1) {
        turtleQuad(popup.minX, popup.minY, popup.minX, popup.maxY, 
        popup.maxX, popup.maxY, popup.maxX, popup.minY, tt_themeColors[12], tt_themeColors[13], tt_themeColors[14], 0);
        double textSize = 5;
        double textX = popup.minX + (popup.maxX - popup.minX) / 2;
        double textY = popup.maxY - textSize * 2;
        turtlePenColor(tt_themeColors[21], tt_themeColors[22], tt_themeColors[23]);
        turtleTextWriteString(popup.message, textX, textY, textSize, 50);
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
                turtleQuad(textX - textSize, textY - textSize, textX + textSize + strLen, textY - textSize, 
                textX + textSize + strLen, textY + textSize, textX - textSize, textY + textSize, tt_themeColors[18], tt_themeColors[19], tt_themeColors[20], 0);
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
                        if (popup.output[1] == i) {
                            popup.output[0] = 1;
                        }
                    }
                }
            } else {
                turtleQuad(textX - textSize, textY - textSize, textX + textSize + strLen, textY - textSize, 
                textX + textSize + strLen, textY + textSize, textX - textSize, textY + textSize, tt_themeColors[15], tt_themeColors[16], tt_themeColors[17], 0);
            }
            turtleTextWriteString(popup.options -> data[i].s, textX, textY, textSize, 0);
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
} button_shape_t;

/* button */
typedef struct {
    char label[24];
    int status;
    button_shape_t shape;
    double size;
    double position[2]; // X, Y
    int *variable; // 1 if button is being pressed, 0 otherwise
} button_t;

typedef enum {
    DIAL_LINEAR = 0,
    DIAL_LOG = 1,
    DIAL_EXP = 2,
} dial_type_t;

/* dial */
typedef struct {
    char label[24];
    int status[2];
    dial_type_t type;
    double size;
    double position[2]; // X, Y
    double range[2];
    double renderNumberFactor; // multiply rendered variable by this amount
    double defaultValue;
    double *variable; // value of dial
} dial_t;

/* switch */
typedef struct {
    char label[24];
    int status;
    double size;
    double position[2]; // X, Y
    int *variable; // 1 if switch is flipped, 0 otherwise
} switch_t;

typedef enum {
    DROPDOWN_ALIGN_LEFT = 0,
    DROPDOWN_ALIGN_CENTER = 1,
    DROPDOWN_ALIGN_RIGHT = 2,
} dropdown_align_t;

/* dropdown */
typedef struct {
    list_t *options;
    char label[24];
    int index;
    int status;
    dropdown_align_t align;
    double size;
    double position[2]; // X, Y
    double maxXfactor;
    int *variable; // index of dropdown selected
} dropdown_t;

/* initialise UI elements */
button_t *buttonInit(char *label, int *variable, double xOffset, double yOffset, double size, button_shape_t shape) {
    if (tt_enabled.buttonEnabled == 0) {
        tt_enabled.buttonEnabled = 1;
        tt_elements.buttons = list_init();
    }
    button_t *buttonp = malloc(sizeof(button_t));
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

switch_t *switchInit(char *label, int *variable, double xOffset, double yOffset, double size) {
    if (tt_enabled.switchEnabled == 0) {
        tt_enabled.switchEnabled = 1;
        tt_elements.switches = list_init();
    }
    switch_t *switchp = malloc(sizeof(switch_t));
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

dial_t *dialInit(char *label, double *variable, dial_type_t type, double xOffset, double yOffset, double size, double bottom, double top, double renderNumberFactor) {
    if (tt_enabled.dialEnabled == 0) {
        tt_enabled.dialEnabled = 1;
        tt_elements.dials = list_init();
    }
    dial_t *dialp = malloc(sizeof(dial_t));
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

void dropdownCalculateMax(dropdown_t *dropdown) {
    dropdown -> maxXfactor = 0;
    for (int i = 0; i < dropdown -> options -> length; i++) {
        double stringLength = turtleTextGetStringLength(dropdown -> options -> data[i].s, dropdown -> size - 1);
        if (stringLength > dropdown -> maxXfactor) {
            dropdown -> maxXfactor = stringLength;
        }
    }
}

dropdown_t *dropdownInit(char *label, list_t *options, int *variable, double xOffset, double yOffset, double size, dropdown_align_t align) {
    if (tt_enabled.dropdownEnabled == 0) {
        tt_globals.dropdownLogicIndex = -1;
        tt_enabled.dropdownEnabled = 1;
        tt_elements.dropdowns = list_init();
    }
    dropdown_t *dropdownp = malloc(sizeof(dropdown_t));
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
    for (int i = 0; i < tt_elements.buttons -> length; i++) {
        button_t *button = (button_t *) (tt_elements.buttons -> data[i].p);
        double buttonX = button -> position[0];
        double buttonY = button -> position[1];
        double buttonWidth = turtleTextGetUnicodeLength(button -> label, button -> size);
        double buttonHeight = 14;
        if (button -> status == 0) {
            turtleRectangle(buttonX - buttonWidth / 2, buttonY - buttonHeight / 2, buttonX + buttonWidth / 2, buttonY + buttonHeight / 2, tt_themeColors[0], tt_themeColors[1], tt_themeColors[2], 0);
        } else {
            turtleRectangle(buttonX - buttonWidth / 2, buttonY - buttonHeight / 2, buttonX + buttonWidth / 2, buttonY + buttonHeight / 2, tt_themeColors[3], tt_themeColors[4], tt_themeColors[5], 0);
        }
        turtlePenColor(tt_themeColors[9], tt_themeColors[10], tt_themeColors[11]);
        turtleTextWriteUnicode(button -> label, buttonX, buttonY, button -> size - 1, 50);
        if (turtleMouseDown()) {
            if (button -> status < 0) {
                button -> status *= -1;
            }
        } else {
            if (turtle.mouseX > buttonX - buttonWidth / 2 && turtle.mouseX < buttonX + buttonWidth / 2 && turtle.mouseY > buttonY - buttonHeight / 2 && turtle.mouseY < buttonY + buttonHeight / 2) {
                button -> status = -1;
            } else {
                button -> status = 0;
            }
        }
        *(button -> variable) = 0;
        if (button -> status > 0) {
            *(button -> variable) = 1;
            button -> status = 0;
        }
    }
}

void switchUpdate() {
    for (int i = 0; i < tt_elements.switches -> length; i++) {
        switch_t *switchp = (switch_t *) (tt_elements.switches -> data[i].p);
        double switchX = switchp -> position[0];
        double switchY = switchp -> position[1];
        turtleTextWriteUnicode(switchp -> label, switchX, switchY + 15, switchp -> size - 1, 50);
        turtlePenColor(tt_themeColors[12], tt_themeColors[13], tt_themeColors[14]);
        turtlePenSize(switchp -> size * 1.2);
        turtleGoto(switchX - switchp -> size * 0.8, switchY);
        turtlePenDown();
        turtleGoto(switchX + switchp -> size * 0.8, switchY);
        turtlePenUp();
        turtlePenSize(switchp -> size);
        turtlePenColor(tt_themeColors[6], tt_themeColors[7], tt_themeColors[8]);
        if (*(switchp -> variable)) {
            turtleGoto(switchX + switchp -> size * 0.8, switchY);
        } else {
            turtleGoto(switchX - switchp -> size * 0.8, switchY);
        }
        turtlePenDown();
        turtlePenUp();
        if (turtleMouseDown()) {
            if (switchp -> status < 0) {
                switchp -> status *= -1;
            }
        } else {
            if (turtle.mouseX > switchX - switchp -> size && turtle.mouseX < switchX + switchp -> size && turtle.mouseY > switchY - switchp -> size && turtle.mouseY < switchY + switchp -> size) {
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
    for (int i = 0; i < tt_elements.dials -> length; i++) {
        dial_t *dialp = (dial_t *) (tt_elements.dials -> data[i].p);
        turtleTextWriteUnicode(dialp -> label, dialp -> position[0], dialp -> position[1] + 15, dialp -> size - 1, 50);
        turtlePenSize(dialp -> size * 2);
        double dialX = dialp -> position[0];
        double dialY = dialp -> position[1];
        turtleGoto(dialX, dialY);
        turtlePenDown();
        turtlePenUp();
        turtlePenSize(dialp -> size * 2 * 0.8);
        turtlePenColor(tt_themeColors[27], tt_themeColors[28], tt_themeColors[29]);
        turtlePenDown();
        turtlePenUp();
        turtlePenColor(tt_themeColors[30], tt_themeColors[31], tt_themeColors[32]);
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
        turtleTextWriteString(bubble, dialX + dialp -> size + 3, dialY, 4, 0);
    }
}

void dropdownUpdate() {
    int logicIndex = -1;
    for (int i = 0; i < tt_elements.dropdowns -> length; i++) {
        dropdown_t *dropdown = (dropdown_t *) (tt_elements.dropdowns -> data[i].p);
        /* render dropdown default position */
        double dropdownX = dropdown -> position[0];
        double dropdownY = dropdown -> position[1];
        if (strlen(dropdown -> label) > 0) {
            turtleTextWriteUnicode(dropdown -> label, dropdownX - 5, dropdownY + 15, dropdown -> size - 1, 50);
        }
        double xfactor = turtleTextGetUnicodeLength(dropdown -> options -> data[dropdown -> index].s, dropdown -> size - 1);
        double itemHeight = (dropdown -> size * 1.5);
        logicIndex = tt_globals.dropdownLogicIndex;
        if (dropdown -> status == -1) {
            turtleRectangle(dropdownX - dropdown -> size - xfactor, dropdownY - dropdown -> size * 0.7, dropdownX + dropdown -> size + 10, dropdownY + dropdown -> size * 0.7, tt_themeColors[12], tt_themeColors[13], tt_themeColors[14], 0);
        } else if (dropdown -> status >= 1) {
            turtleRectangle(dropdownX - dropdown -> size - dropdown -> maxXfactor, dropdownY - dropdown -> size * 0.7 - (dropdown -> options -> length - 1) * itemHeight, dropdownX + dropdown -> size + 10, dropdownY + dropdown -> size * 0.7, tt_themeColors[12], tt_themeColors[13], tt_themeColors[14], 0);
        } else {
            turtleRectangle(dropdownX - dropdown -> size - xfactor, dropdownY - dropdown -> size * 0.7, dropdownX + dropdown -> size + 10, dropdownY + dropdown -> size * 0.7, tt_themeColors[21], tt_themeColors[22], tt_themeColors[23], 0);
        }
        if (turtle.mouseX > dropdownX - dropdown -> size - xfactor && turtle.mouseX < dropdownX + dropdown -> size + 10 && turtle.mouseY > dropdownY - dropdown -> size * 0.7 && turtle.mouseY < dropdownY + dropdown -> size * 0.7) {
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
            if (i > logicIndex && turtleMouseDown()) {
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
            if (turtle.mouseX > dropdownX - dropdown -> size - dropdown -> maxXfactor && turtle.mouseX < dropdownX + dropdown -> size + 10 && turtle.mouseY > dropdownY - dropdown -> size * 0.7 - (dropdown -> options -> length - 1) * itemHeight && turtle.mouseY < dropdownY + dropdown -> size * 0.7) {
                int selected = round((dropdownY - turtle.mouseY) / itemHeight);
                turtleRectangle(dropdownX - dropdown -> size - dropdown -> maxXfactor, dropdownY - dropdown -> size * 0.7 - selected * itemHeight, dropdownX + dropdown -> size + 10, dropdownY + dropdown -> size * 0.7 - selected * itemHeight, tt_themeColors[21], tt_themeColors[22], tt_themeColors[23], 0);
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
            turtlePenColor(tt_themeColors[30], tt_themeColors[31], tt_themeColors[32]);
            int renderIndex = 1;
            for (int i = 0; i < dropdown -> options -> length; i++) {
                if (i != dropdown -> index) {
                    turtleTextWriteUnicode(dropdown -> options -> data[i].s, dropdownX, dropdownY - renderIndex * itemHeight, dropdown -> size - 1, 100);
                    renderIndex++;
                }
            }
        }
        if (dropdown -> status >= 1) {
            logicIndex = i;
        }
        tt_globals.dropdownLogicIndex = logicIndex;
        turtlePenColor(tt_themeColors[30], tt_themeColors[31], tt_themeColors[32]);
        turtleTextWriteUnicode(dropdown -> options -> data[dropdown -> index].s, dropdownX, dropdownY, dropdown -> size - 1, 100);
        if (dropdown -> status >= 1) {
            turtleTriangle(dropdownX + 11, dropdownY + 4, dropdownX + 11, dropdownY - 4, dropdownX + 5, dropdownY, tt_themeColors[9], tt_themeColors[10], tt_themeColors[11], 0);
        } else {
            turtleTriangle(dropdownX + 13, dropdownY + 3, dropdownX + 5, dropdownY + 3, dropdownX + 9, dropdownY - 3, tt_themeColors[9], tt_themeColors[10], tt_themeColors[11], 0);
        }
    }
}

void turtleToolsUpdate() {
    if (tt_enabled.ribbonEnabled) {
        ribbonUpdate();
    }
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
}

#endif