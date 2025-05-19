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

/* ribbon */

/* ribbon variables */
typedef struct {
    unsigned char marginSize;
    signed char mainselect[4]; // 0 - select, 1 - mouseHover, 2 - selected, 3 - premove close dropdown
    signed char subselect[4]; // 0 - select, 1 - mouseHover, 2 - selected, 3 - free
    signed char output[3]; // 0 - toggle, 1 - mainselect, 2 - subselect
    signed char mouseDown; // keeps track of previous frame mouse information
    int bounds[4]; // list of coordinate bounds (minX, minY, maxX, maxY)
    double ribbonSize;
    double colors[12]; // (0, 1, 2) - ribbon colour, (3, 4, 5) - ribbon highlight & dropdown colour, (6, 7, 8) - dropdown highlight colour, (9, 10, 11) - text colour
    list_t *options;
    list_t *lengths;
} turtleToolsRibbon;

turtleToolsRibbon ribbonRender;

int ribbonInit(GLFWwindow* window, const char *filename) { // read from config file
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
    /* default colours */
    ribbonRender.colors[0] = 200.0;
    ribbonRender.colors[1] = 200.0;
    ribbonRender.colors[2] = 200.0;
    ribbonRender.colors[3] = 140.0;
    ribbonRender.colors[4] = 140.0;
    ribbonRender.colors[5] = 140.0;
    ribbonRender.colors[6] = 100.0;
    ribbonRender.colors[7] = 100.0;
    ribbonRender.colors[8] = 100.0;
    ribbonRender.colors[9] = 0.0;
    ribbonRender.colors[10] = 0.0;
    ribbonRender.colors[11] = 0.0;

    ribbonRender.ribbonSize = 1; // 1 is default, below 1 is smaller, above 1 is larger (scales as a multiplier, 0.1 is 100x smaller than 10)
    ribbonRender.options = list_init();
    ribbonRender.lengths = list_init();

    FILE *configFile = fopen(filename, "r"); // load from config file
    if (configFile == NULL) {
        printf("Error: file %s not found\n", filename);
        return -1;
    }
    list_t *sublist = list_init();
    int checksum = 0;
    char throw[256]; // maximum size of any option or sub-option (characters)
    int j = 0;
    list_clear(sublist);
    while (checksum != EOF) {
        checksum = fscanf(configFile, "%[^,\n]%*c,", throw);
        char whitespace = 0;
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

    for (int i = 0; i < ribbonRender.options -> length; i++) {
        list_append(ribbonRender.lengths, (unitype) turtleTextGetStringLength(ribbonRender.options -> data[i].r -> data[0].s, 7 * ribbonRender.ribbonSize), 'd');
        double max = 0;
        for (int j = 1; j < ribbonRender.options -> data[i].r -> length; j++) {
            double cur = turtleTextGetStringLength(ribbonRender.options -> data[i].r -> data[j].s, 7 * ribbonRender.ribbonSize);
            if (cur > max) {
                max = cur;
            }
        }
        list_append(ribbonRender.lengths, (unitype) max, 'd');
    }
    return 0;
}

void ribbonLightTheme() { // light theme preset (default)
    ribbonRender.colors[0] = 200.0;
    ribbonRender.colors[1] = 200.0;
    ribbonRender.colors[2] = 200.0;
    ribbonRender.colors[3] = 140.0;
    ribbonRender.colors[4] = 140.0;
    ribbonRender.colors[5] = 140.0;
    ribbonRender.colors[6] = 100.0;
    ribbonRender.colors[7] = 100.0;
    ribbonRender.colors[8] = 100.0;
    ribbonRender.colors[9] = 0.0;
    ribbonRender.colors[10] = 0.0;
    ribbonRender.colors[11] = 0.0;
}

void ribbonDarkTheme() { // dark theme preset
    ribbonRender.colors[0] = 70.0; // top bar color
    ribbonRender.colors[1] = 70.0;
    ribbonRender.colors[2] = 70.0;
    ribbonRender.colors[3] = 80.0; // dropdown color
    ribbonRender.colors[4] = 80.0;
    ribbonRender.colors[5] = 80.0;
    ribbonRender.colors[6] = 70.0; // select color
    ribbonRender.colors[7] = 70.0;
    ribbonRender.colors[8] = 70.0;
    ribbonRender.colors[9] = 160.0; // text color
    ribbonRender.colors[10] = 160.0;
    ribbonRender.colors[11] = 160.0;
}

void ribbonUpdate() {
    char shapeSave = turtle.penshape;
    double sizeSave = turtle.pensize;
    turtlePenSize(20);
    turtlePenShape("square");
    turtleGetMouseCoords(); // get the mouse coordinates (turtle.mouseX, turtle.mouseY)
    turtleQuad(ribbonRender.bounds[0], ribbonRender.bounds[3] - 10, ribbonRender.bounds[2], ribbonRender.bounds[3] - 10, ribbonRender.bounds[2], ribbonRender.bounds[3], ribbonRender.bounds[0], ribbonRender.bounds[3], ribbonRender.colors[0], ribbonRender.colors[1], ribbonRender.colors[2], 50.0); // render ribbon
    turtlePenColor(ribbonRender.colors[9], ribbonRender.colors[10], ribbonRender.colors[11]); // text colour
    double cutoff = ribbonRender.bounds[0] + ribbonRender.marginSize;
    ribbonRender.mainselect[0] = -1;
    ribbonRender.subselect[0] = -1;
    for (int i = 0; i < ribbonRender.options -> length; i++) {
        double prevCutoff = cutoff;
        if (i == ribbonRender.mainselect[2]) {
            double xLeft = prevCutoff - ribbonRender.marginSize / 2.0;
            double xRight = prevCutoff + ribbonRender.lengths -> data[i * 2 + 1].d + ribbonRender.marginSize / 2.0;
            double yDown = ribbonRender.bounds[3] - 10 - 15 * (ribbonRender.options -> data[i].r -> length - 1) - ribbonRender.marginSize / 2.0;
            turtleQuad(xLeft, ribbonRender.bounds[3] - 10, xRight, ribbonRender.bounds[3] - 10, xRight, yDown, xLeft, yDown, ribbonRender.colors[3], ribbonRender.colors[4], ribbonRender.colors[5], 0.0); // ribbon highlight
            for (int j = 1; j < ribbonRender.options -> data[i].r -> length; j++) {
                if (turtle.mouseY > ribbonRender.bounds[3] - 10 - 15 * j - ribbonRender.marginSize / 4.0 && turtle.mouseY < ribbonRender.bounds[3] - 10 && turtle.mouseX > xLeft && turtle.mouseX < xRight && ribbonRender.subselect[0] == -1) {
                    turtleQuad(xLeft, ribbonRender.bounds[3] - 10 - 15 * (j - 1) - ribbonRender.marginSize / 4.0, xRight, ribbonRender.bounds[3] - 10 - 15 * (j - 1) - ribbonRender.marginSize / 4.0, xRight, ribbonRender.bounds[3] - 10 - 15 * j - ribbonRender.marginSize / 3.0, xLeft, ribbonRender.bounds[3] - 10 - 15 * j - ribbonRender.marginSize / 3.0, ribbonRender.colors[6], ribbonRender.colors[7], ribbonRender.colors[8], 0.0); // dropdown highlight
                    ribbonRender.subselect[0] = j;
                }
                turtleTextWriteString(ribbonRender.options -> data[i].r -> data[j].s, prevCutoff, 174.5 - j * 15, 7 * ribbonRender.ribbonSize, 0);
            }
        }
        cutoff += ribbonRender.lengths -> data[i * 2].d + ribbonRender.marginSize;
        if (turtle.mouseY > ribbonRender.bounds[3] - 10 && turtle.mouseY < ribbonRender.bounds[3] && turtle.mouseX > ribbonRender.bounds[0] + ribbonRender.marginSize / 2.0 && turtle.mouseX < cutoff - ribbonRender.marginSize / 2.0 && ribbonRender.mainselect[0] == -1) { // -217, -195, -164
            turtleQuad(prevCutoff - ribbonRender.marginSize / 2.0, 179, cutoff - ribbonRender.marginSize / 2.0, 179, cutoff - ribbonRender.marginSize / 2.0, 171, prevCutoff - ribbonRender.marginSize / 2.0, 171, ribbonRender.colors[3], ribbonRender.colors[4], ribbonRender.colors[5], 0.0); // render dropdown
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
    signed char style;
    signed char output[2]; // [toggle, select]
    signed char mouseDown;
    double colors[12]; // (0, 1, 2) - popup colour, (3, 4, 5) - boxes colour, (6, 7, 8) - boxes highlight colour, (9, 10, 11) - text colour
} turtleToolsPopup;

turtleToolsPopup popup;

int popupInit(const char *filename, double minX, double minY, double maxX, double maxY) {
    popup.minX = minX;
    popup.minY = minY;
    popup.maxX = maxX;
    popup.maxY = maxY;
    popup.output[0] = 0;
    popup.output[1] = -1;
    popup.mouseDown = 0;
    /* default colours */
    popup.colors[0] = 200.0;
    popup.colors[1] = 200.0;
    popup.colors[2] = 200.0;
    popup.colors[3] = 140.0;
    popup.colors[4] = 140.0;
    popup.colors[5] = 140.0;
    popup.colors[6] = 100.0;
    popup.colors[7] = 100.0;
    popup.colors[8] = 100.0;
    popup.colors[9] = 0.0;
    popup.colors[10] = 0.0;
    popup.colors[11] = 0.0;
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

/* light theme preset (default) */
void popupLightTheme() {
    popup.colors[0] = 200.0; // popup colour
    popup.colors[1] = 200.0;
    popup.colors[2] = 200.0;
    popup.colors[3] = 140.0; // boxes colour
    popup.colors[4] = 140.0;
    popup.colors[5] = 140.0;
    popup.colors[6] = 100.0; // highlight color
    popup.colors[7] = 100.0;
    popup.colors[8] = 100.0;
    popup.colors[9] = 0.0; // text color
    popup.colors[10] = 0.0;
    popup.colors[11] = 0.0;
}

/* dark theme preset */
void popupDarkTheme() {
    popup.colors[0] = 10.0; // popup colour
    popup.colors[1] = 10.0;
    popup.colors[2] = 10.0;
    popup.colors[3] = 40.0; // boxes colour
    popup.colors[4] = 40.0;
    popup.colors[5] = 40.0;
    popup.colors[6] = 60.0; // highlight color
    popup.colors[7] = 60.0;
    popup.colors[8] = 60.0;
    popup.colors[9] = 160.0; // text color
    popup.colors[10] = 160.0;
    popup.colors[11] = 160.0;
}

void popupUpdate() {
    if (turtle.close == 1) {
        turtleQuad(popup.minX, popup.minY, popup.minX, popup.maxY, 
        popup.maxX, popup.maxY, popup.maxX, popup.minY, popup.colors[0], popup.colors[1], popup.colors[2], 0);
        double textSize = 5;
        double textX = popup.minX + (popup.maxX - popup.minX) / 2;
        double textY = popup.maxY - textSize * 2;
        turtlePenColor(popup.colors[9], popup.colors[10], popup.colors[11]);
        turtleTextWriteString(popup.message, textX, textY, textSize, 50);
        textY -= textSize * 4;
        double fullLength = 0;
        for (int i = 0; i < popup.options -> length; i++) {
            fullLength += turtleTextGetStringLength(popup.options -> data[i].s, textSize);
        }
        /* we have the length of the strings, now we pad with n + 1 padding regions */
        double padThai = (popup.maxX - popup.minX - fullLength) / (popup.options -> length + 1);
        textX = popup.minX + padThai;
        char flagged = 0;
        if (!turtleMouseDown() && popup.mouseDown == 1) {
            flagged = 1; // flagged for mouse misbehaviour
        }
        for (int i = 0; i < popup.options -> length; i++) {
            double strLen = turtleTextGetStringLength(popup.options -> data[i].s, textSize);
            if (turtle.mouseX > textX - textSize && turtle.mouseX < textX + strLen + textSize &&
            turtle.mouseY > textY - textSize && turtle.mouseY < textY + textSize) {
                turtleQuad(textX - textSize, textY - textSize, textX + textSize + strLen, textY - textSize, 
                textX + textSize + strLen, textY + textSize, textX - textSize, textY + textSize, popup.colors[6], popup.colors[7], popup.colors[8], 0);
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
                textX + textSize + strLen, textY + textSize, textX - textSize, textY + textSize, popup.colors[3], popup.colors[4], popup.colors[5], 0);
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
#endif