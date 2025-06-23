/* turtleTools library includes:
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

#ifndef TURTLETOOLS
#define TURTLETOOLS
#include "turtleText.h"

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
    memmove(dest + index + lenSource, dest + index, lenDest - index);
    memcpy(dest + index, source, lenSource);
    return dest;
}

char *strdel(char *dest, int32_t index, int32_t size) {
    int32_t len = strlen(dest);
    memmove(dest + index, dest + index + size, len - index - size);
    dest[len - size] = '\0';
    return dest;
}

typedef enum {
    TT_THEME_LIGHT = 0,
    TT_THEME_DARK = 1,
    TT_THEME_COLT = 2,
    TT_THEME_NAVY = 3,
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
    int8_t textboxEnabled;
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

tt_elements_t tt_elements;

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
    TT_COLOR_SCROLLBAR_BASE = 57,
    TT_COLOR_SCROLLBAR_BAR = 60,
    TT_COLOR_SCROLLBAR_HOVER = 63,
    TT_COLOR_SCROLLBAR_CLICKED = 66,
    TT_COLOR_DROPDOWN = 69,
    TT_COLOR_DROPDOWN_SELECT = 72,
    TT_COLOR_DROPDOWN_HOVER = 75,
    TT_COLOR_DROPDOWN_TRIANGLE = 78,
    TT_COLOR_TEXTBOX_BOX = 81,
    TT_COLOR_TEXTBOX_PHANTOM_TEXT = 84,
    TT_COLOR_TEXTBOX_LINE = 87,
    TT_COLOR_TEXTBOX_SELECT = 90,
} tt_theme_internal_t;

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
    200.0, 200.0, 200.0, // scrollbar bar base color (57)
    160.0, 160.0, 160.0, // scrollbar bar color (60)
    120.0, 120.0, 120.0, // scrollbar bar hover color (63)
    120.0, 120.0, 120.0, // scrollbar bar clicked color (66)
    160.0, 160.0, 160.0, // dropdown color (69)
    120.0, 120.0, 120.0, // dropdown select color (72)
    120.0, 120.0, 120.0, // dropdown hover color (75)
    100.0, 100.0, 100.0, // dropdown triangle color (78)
    160.0, 160.0, 160.0, // textbox color (81)
    200.0, 200.0, 200.0, // textbox phantom text color (84)
    120.0, 120.0, 120.0, // textbox line color (87)
    11.0, 87.0, 208.0,   // textbox select color (90)
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
            60.0, 60.0, 60.0,    // switch color off (33)
            80.0, 80.0, 80.0,    // switch circle color off (36)
            10.0, 10.0, 10.0,    // switch color on (39)
            50.0, 230.0, 30.0,   // switch circle color on (42)
            200.0, 200.0, 200.0, // dial color (45)
            30.0, 30.0, 30.0,    // dial inner circle color (48)
            10.0, 10.0, 10.0,    // slider bar color (51)
            230.0, 230.0, 230.0, // slider circle color (54)
            50.0, 50.0, 50.0,    // scrollbar bar base color (57)
            150.0, 150.0, 150.0, // scrollbar bar color (60)
            170.0, 170.0, 170.0, // scrollbar bar hover color (63)
            190.0, 190.0, 190.0, // scrollbar bar clicked color (66)
            60.0, 60.0, 60.0,    // dropdown color (69)
            80.0, 80.0, 80.0,    // dropdown select color (72)
            80.0, 80.0, 80.0,    // dropdown hover color (75)
            160.0, 160.0, 160.0, // dropdown triangle color (78)
            60.0, 60.0, 60.0,    // textbox color (81)
            80.0, 80.0, 80.0,    // textbox phantom text color (84)
            160.0, 160.0, 160.0, // textbox line color (87)
            11.0, 87.0, 208.0,   // textbox select color (90)
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
            68.0, 60.0, 60.0,    // switch color off (33)
            91.0, 80.0, 80.0,    // switch circle color off (36)
            69.0, 55.0, 53.0,    // switch color on (39)
            242.0, 242.0, 230.0, // switch circle color on (42)
            211.0, 211.0, 200.0, // dial color (45)
            36.0, 30.0, 32.0,    // dial inner circle color (48)
            69.0, 55.0, 53.0,    // slider bar color (51)
            242.0, 242.0, 230.0, // slider circle color (54)
            241.0, 239.0, 236.0, // scrollbar bar base color (57)
            69.0, 55.0, 53.0,    // scrollbar bar color (60)
            84.0, 70.0, 68.0,    // scrollbar bar hover color (63)
            84.0, 70.0, 68.0,    // scrollbar bar clicked color (66)
            68.0, 60.0, 60.0,    // dropdown color (69)
            92.0, 80.0, 80.0,    // dropdown select color (72)
            93.0, 80.0, 80.0,    // dropdown hover color (75)
            175.0, 171.0, 160.0, // dropdown triangle color (78)
            160.0, 160.0, 160.0, // textbox color (81)
            200.0, 200.0, 200.0, // textbox phantom text color (84)
            120.0, 120.0, 120.0, // textbox line color (87)
            11.0, 87.0, 208.0,   // textbox select color (90)
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
            18.0, 52.0, 88.0,    // switch color off (33)
            241.0, 239.0, 236.0, // switch circle color off (36)
            18.0, 52.0, 88.0,    // switch color on (39)
            241.0, 239.0, 236.0, // switch circle color on (42)
            3.0, 3.0, 3.0,       // dial color (45)
            212.0, 201.0, 190.0, // dial inner circle color (48)
            18.0, 52.0, 88.0,    // slider bar color (51)
            241.0, 239.0, 236.0, // slider circle color (54)
            241.0, 239.0, 236.0, // scrollbar bar base color (57)
            33.6, 161.9, 211.6,  // scrollbar bar color (60)
            50.0, 178.3, 228.0,  // scrollbar bar hover color (63)
            50.0, 178.3, 228.0,  // scrollbar bar clicked color (66)
            18.0, 52.0, 88.0,    // dropdown color (69)
            74.0, 108.0, 144.0,  // dropdown select color (72)
            74.0, 108.0, 144.0,  // dropdown hover color (75)
            241.0, 239.0, 236.0, // dropdown triangle color (78)
            160.0, 160.0, 160.0, // textbox color (81)
            200.0, 200.0, 200.0, // textbox phantom text color (84)
            120.0, 120.0, 120.0, // textbox line color (87)
            11.0, 87.0, 208.0,   // textbox select color (90)
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
            180.0, 180.0, 180.0, // switch color off (33)
            230.0, 230.0, 230.0, // switch circle color off (36)
            30.0, 30.0, 30.0,    // switch color on (39)
            0.0, 255.0, 255.0,   // switch circle color on (42)
            0.0, 0.0, 0.0,       // dial color (45)
            255.0, 255.0, 255.0, // dial inner circle color (48)
            120.0, 120.0, 120.0, // slider bar color (51)
            230.0, 230.0, 230.0, // slider circle color (54)
            200.0, 200.0, 200.0, // scrollbar bar base color (57)
            160.0, 160.0, 160.0, // scrollbar bar color (60)
            120.0, 120.0, 120.0, // scrollbar bar hover color (63)
            120.0, 120.0, 120.0, // scrollbar bar clicked color (66)
            160.0, 160.0, 160.0, // dropdown color (69)
            120.0, 120.0, 120.0, // dropdown select color (72)
            120.0, 120.0, 120.0, // dropdown hover color (75)
            100.0, 100.0, 100.0, // dropdown triangle color (78)
            160.0, 160.0, 160.0, // textbox color (81)
            200.0, 200.0, 200.0, // textbox phantom text color (84)
            120.0, 120.0, 120.0, // textbox line color (87)
            11.0, 87.0, 208.0,   // textbox select color (90)
        };
        memcpy(tt_themeColors, tt_themeCopy, sizeof(tt_themeCopy));
    }
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
    double barAnchor;
    int32_t dropdownLogicIndex;
} tt_globals_t;

tt_globals_t tt_globals;

typedef enum {
    TT_ELEMENT_ENABLED = 0,
    TT_ELEMENT_NO_MOUSE = 1,
    TT_ELEMENT_HIDE = 2,
} tt_element_enabled_t;

typedef enum {
    TT_BUTTON_SHAPE_RECTANGLE = 0,
    TT_BUTTON_SHAPE_ROUNDED_RECTANGLE = 1,
    TT_BUTTON_SHAPE_CIRCLE = 2,
} tt_button_shape_t;

/* button */
typedef struct {
    tt_element_names_t element;
    tt_element_enabled_t enabled;
    tt_color_override_t color;
    double x;
    double y;
    double size;
    int8_t *variable; // 1 if button is being pressed, 0 otherwise
    char label[24];
    int32_t status;
    tt_button_shape_t shape;
} tt_button_t;

typedef enum {
    TT_SWITCH_STYLE_CLASSIC = 0,
    TT_SWITCH_STYLE_CHECKBOX = 1,
    TT_SWITCH_STYLE_XBOX = 2,
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
    char label[24];
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
    tt_element_enabled_t enabled;
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
    char label[24];
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
    char label[24];
    int32_t status;
    tt_textbox_align_t align;
    double length;
    int32_t maxCharacters;
    int32_t editIndex;
    int32_t lastKey;
    int32_t keyTimeout;
} tt_textbox_t;

/* override colors with color array */
void tt_colorOverride(void *element, double *colors, uint32_t length) {
    ((tt_button_t *) element) -> color.colorOverride = 1;
    memcpy(((tt_button_t *) element) -> color.color, colors, length * sizeof(double));
}

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

int32_t tt_color_override_default[] = {
    /*  button                   switch                      dial                  slider                   scrollbar                   dropdown               textbox  */
    TT_COLOR_TEXT,          TT_COLOR_TEXT,              TT_COLOR_TEXT,       TT_COLOR_TEXT,          0,                          TT_COLOR_TEXT,              TT_COLOR_TEXT,
    TT_COLOR_BUTTON,        TT_COLOR_SWITCH_ON,         TT_COLOR_DIAL,       TT_COLOR_SLIDER_BAR,    TT_COLOR_SCROLLBAR_BASE,    TT_COLOR_TEXT_ALTERNATE,    TT_COLOR_TEXT,
    TT_COLOR_BUTTON_SELECT, TT_COLOR_SWITCH_OFF,        TT_COLOR_DIAL_INNER, TT_COLOR_SLIDER_CIRCLE, TT_COLOR_SCROLLBAR_HOVER,   TT_COLOR_DROPDOWN,          TT_COLOR_TEXT,
    0,                      TT_COLOR_SWITCH_CIRCLE_ON,  0,                   0,                      TT_COLOR_SCROLLBAR_CLICKED, TT_COLOR_DROPDOWN_SELECT,   TT_COLOR_TEXT,
    0,                      TT_COLOR_SWITCH_CIRCLE_OFF, 0,                   0,                      TT_COLOR_SCROLLBAR_BAR,     TT_COLOR_DROPDOWN_HOVER,    TT_COLOR_TEXT,
    0,                      0,                          0,                   0,                      0,                          TT_COLOR_DROPDOWN_TRIANGLE, TT_COLOR_TEXT,
    0,                      0,                          0,                   0,                      0,                          0,                          TT_COLOR_TEXT,
    0,                      0,                          0,                   0,                      0,                          0,                          TT_COLOR_TEXT,
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
    textboxp -> text = calloc(maxCharacters + 1, 1);
    textboxp -> maxCharacters = maxCharacters;
    textboxp -> editIndex = 0;
    textboxp -> lastKey = 0;
    textboxp -> keyTimeout = 0;
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
        if (buttonp -> status == 0) {
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
        }
        tt_internalColor(buttonp, TT_COLOR_TEXT_ALTERNATE, TT_COLOR_OVERRIDE_SLOT_0);
        turtleTextWriteUnicode((unsigned char *) buttonp -> label, buttonX, buttonY, buttonp -> size - 1, 50);
        if (buttonp -> enabled == TT_ELEMENT_ENABLED) {
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
}

void switchUpdate() {
    for (uint32_t i = 0; i < tt_elements.switches -> length; i++) {
        tt_switch_t *switchp = (tt_switch_t *) (tt_elements.switches -> data[i].p);
        if (switchp -> enabled == TT_ELEMENT_HIDE) {
            continue;
        }
        double switchX = switchp -> x;
        double switchY = switchp -> y;
        if (switchp -> style == TT_SWITCH_STYLE_CLASSIC) {
            tt_internalColor(switchp, TT_COLOR_TEXT, TT_COLOR_OVERRIDE_SLOT_0);
            turtleTextWriteUnicode((unsigned char *) switchp -> label, switchX, switchY + 1.6 * switchp -> size, switchp -> size - 1, 50);
            if (*(switchp -> variable)) {
                tt_internalColor(switchp, TT_COLOR_SWITCH_ON, TT_COLOR_OVERRIDE_SLOT_1);
            } else {
                tt_internalColor(switchp, TT_COLOR_SWITCH_OFF, TT_COLOR_OVERRIDE_SLOT_2);
            }
            turtlePenSize(switchp -> size * 1.2);
            turtleGoto(switchX - switchp -> size * 0.8, switchY);
            turtlePenDown();
            turtleGoto(switchX + switchp -> size * 0.8, switchY);
            turtlePenUp();
            turtlePenSize(switchp -> size);
            if (*(switchp -> variable)) {
                tt_internalColor(switchp, TT_COLOR_SWITCH_CIRCLE_ON, TT_COLOR_OVERRIDE_SLOT_3);
                turtleGoto(switchX + switchp -> size * 0.8, switchY);
            } else {
                tt_internalColor(switchp, TT_COLOR_SWITCH_CIRCLE_OFF, TT_COLOR_OVERRIDE_SLOT_4);
                turtleGoto(switchX - switchp -> size * 0.8, switchY);
            }
            turtlePenDown();
            turtlePenUp();
            /* mouse */
            if (switchp -> enabled == TT_ELEMENT_ENABLED) {
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
        } else if (switchp -> style == TT_SWITCH_STYLE_CHECKBOX || switchp -> style == TT_SWITCH_STYLE_XBOX) {
            tt_internalColor(switchp, TT_COLOR_SWITCH_OFF, TT_COLOR_OVERRIDE_SLOT_2);
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
                    /* check */
                } else {
                    /* X */
                    tt_internalColor(switchp, TT_COLOR_SWITCH_CIRCLE_ON, TT_COLOR_OVERRIDE_SLOT_3);
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
        if (dialp -> enabled == TT_ELEMENT_ENABLED) {
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
        if (sliderp -> enabled == TT_ELEMENT_ENABLED) {
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
            if (scrollbarp -> enabled == TT_ELEMENT_ENABLED) {
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
        if (dropdownp -> enabled == TT_ELEMENT_ENABLED) {
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

void textboxUpdate() {
    for (uint32_t i = 0; i < tt_elements.textboxes -> length; i++) {
        tt_textbox_t *textboxp = (tt_textbox_t *) (tt_elements.textboxes -> data[i].p);
        if (textboxp -> enabled == TT_ELEMENT_HIDE) {
            continue;
        }
        printf("editIndex: %d\n", textboxp -> editIndex);
        tt_internalColor(textboxp, TT_COLOR_TEXTBOX_BOX, TT_COLOR_OVERRIDE_SLOT_1);
        turtleRectangle(textboxp -> x, textboxp -> y - textboxp -> size, textboxp -> x + textboxp -> length, textboxp -> y + textboxp -> size);

        if (textboxp -> status <= 0) {
            if (strlen(textboxp -> text) == 0) {
                tt_internalColor(textboxp, TT_COLOR_TEXTBOX_PHANTOM_TEXT, TT_COLOR_OVERRIDE_SLOT_2);
                turtleTextWriteUnicode((unsigned char *) textboxp -> label, textboxp -> x + textboxp -> size / 2, textboxp -> y, textboxp -> size - 1, 0);
            } else {
                tt_internalColor(textboxp, TT_COLOR_TEXT_ALTERNATE, TT_COLOR_OVERRIDE_SLOT_0);
                double textLength = turtleTextGetUnicodeLength((unsigned char *) textboxp -> text, textboxp -> size - 1);
                if (textLength > textboxp -> length - textboxp -> size / 1.5) {
                    /* text is too big to fit in textbox */
                    int32_t j = 0;
                    while (textLength > textboxp -> length - textboxp -> size / 3) {
                        j++;
                        textLength = turtleTextGetUnicodeLength((unsigned char *) (textboxp -> text + j), textboxp -> size - 1);
                    }
                    turtleTextWriteUnicode((unsigned char *) (textboxp -> text + j), textboxp -> x + textboxp -> length - textboxp -> size / 3, textboxp -> y, textboxp -> size - 1, 100);
                    tt_internalColor(textboxp, TT_COLOR_TEXTBOX_BOX, TT_COLOR_OVERRIDE_SLOT_1);
                    turtleRectangle(textboxp -> x, textboxp -> y - textboxp -> size, textboxp -> x + textboxp -> size / 3, textboxp -> y + textboxp -> size);
                } else {
                    turtleTextWriteUnicode((unsigned char *) textboxp -> text, textboxp -> x + textboxp -> size / 3, textboxp -> y, textboxp -> size - 1, 0);
                }
            }
        } else if (textboxp -> status > 0) {
            tt_internalColor(textboxp, TT_COLOR_TEXT_ALTERNATE, TT_COLOR_OVERRIDE_SLOT_0);

            double textLength = turtleTextGetUnicodeLength((unsigned char *) textboxp -> text, textboxp -> size - 1);
            if (textLength > textboxp -> length - textboxp -> size / 1.5) {
                /* text is too big to fit in textbox */
                int32_t j = 0;
                while (textLength > textboxp -> length - textboxp -> size / 3) {
                    j++;
                    textLength = turtleTextGetUnicodeLength((unsigned char *) (textboxp -> text + j), textboxp -> size - 1);
                }
                turtleTextWriteUnicode((unsigned char *) (textboxp -> text + j), textboxp -> x + textboxp -> length - textboxp -> size / 3, textboxp -> y, textboxp -> size - 1, 100);
                if (textboxp -> status < 66) {
                    tt_internalColor(textboxp, TT_COLOR_TEXTBOX_LINE, TT_COLOR_OVERRIDE_SLOT_3);
                    turtleRectangle(textboxp -> x + textboxp -> length - textboxp -> size / 3, textboxp -> y - textboxp -> size * 0.8, textboxp -> x + textboxp -> length - textboxp -> size / 3 + 1, textboxp -> y + textboxp -> size * 0.8);
                }
                tt_internalColor(textboxp, TT_COLOR_TEXTBOX_BOX, TT_COLOR_OVERRIDE_SLOT_1);
                turtleRectangle(textboxp -> x, textboxp -> y - textboxp -> size, textboxp -> x + textboxp -> size / 3, textboxp -> y + textboxp -> size);
            } else {
                turtleTextWriteUnicode((unsigned char *) textboxp -> text, textboxp -> x + textboxp -> size / 3, textboxp -> y, textboxp -> size - 1, 0);
                char tempHold = textboxp -> text[textboxp -> editIndex];
                textboxp -> text[textboxp -> editIndex] = '\0';
                textLength = turtleTextGetUnicodeLength((unsigned char *) textboxp -> text, textboxp -> size - 1);
                textboxp -> text[textboxp -> editIndex] = tempHold;
                if (textboxp -> status < 66) {
                    tt_internalColor(textboxp, TT_COLOR_TEXTBOX_LINE, TT_COLOR_OVERRIDE_SLOT_3);
                    turtleRectangle(textboxp -> x + textboxp -> size / 3 + textLength, textboxp -> y - textboxp -> size * 0.8, textboxp -> x + textboxp -> size / 3 + textLength + 1, textboxp -> y + textboxp -> size * 0.8);
                }
            }
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
        }

        if (textboxp -> enabled == TT_ELEMENT_ENABLED) {
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

#endif