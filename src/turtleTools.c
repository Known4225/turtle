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

#include "../include/turtleTools.h"

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
    255.0, 255.0, 255.0, // TT_COLOR_BACKGROUND (0)
    222.0, 222.0, 222.0, // TT_COLOR_BACKGROUND_HIGHLIGHT (3)
    230.0, 230.0, 230.0, // TT_COLOR_BACKGROUND_ALTERNATE (6)
    255.0, 255.0, 255.0, // TT_COLOR_BACKGROUND_COMPLEMENT (9)
    0.0, 0.0, 0.0,       // TT_COLOR_TEXT (12)
    150.0, 150.0, 150.0, // TT_COLOR_TEXT_HIGHLIGHT (15)
    0.0, 0.0, 0.0,       // TT_COLOR_TEXT_ALTERNATE (18)
    140.0, 140.0, 140.0, // TT_COLOR_TEXT_COMPLEMENT (21)
    180.0, 180.0, 180.0, // TT_COLOR_COMPONENT (24)
    160.0, 160.0, 160.0, // TT_COLOR_COMPONENT_HIGHLIGHT (27)
    116.0, 116.0, 116.0, // TT_COLOR_COMPONENT_ALTERNATE (30)
    222.0, 222.0, 222.0, // TT_COLOR_COMPONENT_COMPLEMENT (33)
    0.0, 255.0, 255.0,   // TT_COLOR_TERTIARY (36)
    0.0, 221.0, 233.0,   // TT_COLOR_TERTIARY_HIGHLIGHT (39)
    155.0, 199.0, 242.0, // TT_COLOR_TERTIARY_ALTERNATE (42)
    149.0, 217.0, 222.0, // TT_COLOR_TERTIARY_COMPLEMENT (45)
    250.0, 14.0, 0.0,    // TT_COLOR_RED (48)
    247.0, 69.0, 64.0,   // TT_COLOR_RED_ALTERNATE (51)
    247.0, 143.0, 64.0,  // TT_COLOR_ORANGE (54)
    247.0, 111.0, 64.0,  // TT_COLOR_ORANGE_ALTERNATE (57)
    250.0, 238.0, 0.0,   // TT_COLOR_YELLOW (60)
    250.0, 199.0, 0.0,   // TT_COLOR_YELLOW_ALTERNATE (63)
    19.0, 236.0, 48.0,   // TT_COLOR_GREEN (66)
    0.0, 185.0, 29.0,    // TT_COLOR_GREEN_ALTERNATE (69)
    5.0, 201.0, 242.0,   // TT_COLOR_CYAN (72)
    5.0, 242.0, 206.0,   // TT_COLOR_CYAN_ALTERNATE (75)
    40.0, 56.0, 245.0,   // TT_COLOR_BLUE (78)
    79.0, 98.0, 245.0,   // TT_COLOR_BLUE_ALTERNATE (81)
    142.0, 75.0, 245.0,  // TT_COLOR_PURPLE (84)
    167.0, 110.0, 244.0, // TT_COLOR_PURPLE_ALTERNATE (87)
    216.0, 63.0, 245.0,  // TT_COLOR_MAGENTA (90)
    213.0, 110.0, 245.0, // TT_COLOR_MAGENTA_ALTERNATE (93)
    245.0, 17.0, 233.0,  // TT_COLOR_PINK (96)
    250.0, 112.0, 228.0, // TT_COLOR_PINK_ALTERNATE (99)
    0.0, 0.0, 0.0,       // TT_COLOR_BLACK (102)
    7.0, 7.0, 7.0,       // TT_COLOR_BLACK_ALTERNATE (105)
    255.0, 255.0, 255.0, // TT_COLOR_WHITE (108)
    238.0, 238.0, 238.0, // TT_COLOR_WHITE_ALTERNATE (111)
    56.0, 56.0, 56.0,    // TT_COLOR_DARK_GREY (114)
    81.0, 81.0, 81.0,    // TT_COLOR_DARK_GREY_ALTERNATE (117)
    187.0, 187.0, 187.0, // TT_COLOR_LIGHT_GREY (120)
    199.0, 199.0, 199.0, // TT_COLOR_LIGHT_GREY_ALTERNATE (123)
};

void tt_setColor(int32_t index) {
    turtlePenColor(tt_themeColors[index], tt_themeColors[index + 1], tt_themeColors[index + 2]);
}

void turtleToolsSetTheme(tt_theme_name_t theme) {
    if (theme == TT_THEME_DARK) {
        tt_theme = theme;
        double tt_themeCopy[] = {
            30.0, 30.0, 30.0,    // TT_COLOR_BACKGROUND (0)
            200.0, 200.0, 200.0, // TT_COLOR_BACKGROUND_HIGHLIGHT (3)
            125.0, 125.0, 125.0, // TT_COLOR_BACKGROUND_ALTERNATE (6)
            238.0, 238.0, 238.0, // TT_COLOR_BACKGROUND_COMPLEMENT (9)
            200.0, 200.0, 200.0, // TT_COLOR_TEXT (12)
            80.0, 80.0, 80.0,    // TT_COLOR_TEXT_HIGHLIGHT (15)
            160.0, 160.0, 160.0, // TT_COLOR_TEXT_ALTERNATE (18)
            160.0, 160.0, 160.0, // TT_COLOR_TEXT_COMPLEMENT (21)
            60.0, 60.0, 60.0,    // TT_COLOR_COMPONENT (24)
            80.0, 80.0, 80.0,    // TT_COLOR_COMPONENT_HIGHLIGHT (27)
            10.0, 10.0, 10.0,    // TT_COLOR_COMPONENT_ALTERNATE (30)
            97.0, 97.0, 97.0,    // TT_COLOR_COMPONENT_COMPLEMENT (33)
            74.0, 198.0, 174.0,  // TT_COLOR_TERTIARY (36)
            136.0, 203.0, 213.0, // TT_COLOR_TERTIARY_HIGHLIGHT (39)
            155.0, 199.0, 190.0, // TT_COLOR_TERTIARY_ALTERNATE (42)
            149.0, 217.0, 204.0, // TT_COLOR_TERTIARY_COMPLEMENT (45)
            250.0, 14.0, 0.0,    // TT_COLOR_RED (48)
            247.0, 69.0, 64.0,   // TT_COLOR_RED_ALTERNATE (51)
            247.0, 143.0, 64.0,  // TT_COLOR_ORANGE (54)
            247.0, 111.0, 64.0,  // TT_COLOR_ORANGE_ALTERNATE (57)
            250.0, 238.0, 0.0,   // TT_COLOR_YELLOW (60)
            250.0, 199.0, 0.0,   // TT_COLOR_YELLOW_ALTERNATE (63)
            19.0, 236.0, 48.0,   // TT_COLOR_GREEN (66)
            0.0, 185.0, 29.0,    // TT_COLOR_GREEN_ALTERNATE (69)
            5.0, 201.0, 242.0,   // TT_COLOR_CYAN (72)
            5.0, 242.0, 206.0,   // TT_COLOR_CYAN_ALTERNATE (75)
            40.0, 56.0, 245.0,   // TT_COLOR_BLUE (78)
            79.0, 98.0, 245.0,   // TT_COLOR_BLUE_ALTERNATE (81)
            142.0, 75.0, 245.0,  // TT_COLOR_PURPLE (84)
            167.0, 110.0, 244.0, // TT_COLOR_PURPLE_ALTERNATE (87)
            216.0, 63.0, 245.0,  // TT_COLOR_MAGENTA (90)
            213.0, 110.0, 245.0, // TT_COLOR_MAGENTA_ALTERNATE (93)
            245.0, 17.0, 233.0,  // TT_COLOR_PINK (96)
            250.0, 112.0, 228.0, // TT_COLOR_PINK_ALTERNATE (99)
            0.0, 0.0, 0.0,       // TT_COLOR_BLACK (102)
            7.0, 7.0, 7.0,       // TT_COLOR_BLACK_ALTERNATE (105)
            255.0, 255.0, 255.0, // TT_COLOR_WHITE (108)
            238.0, 238.0, 238.0, // TT_COLOR_WHITE_ALTERNATE (111)
            56.0, 56.0, 56.0,    // TT_COLOR_DARK_GREY (114)
            81.0, 81.0, 81.0,    // TT_COLOR_DARK_GREY_ALTERNATE (117)
            187.0, 187.0, 187.0, // TT_COLOR_LIGHT_GREY (120)
            199.0, 199.0, 199.0, // TT_COLOR_LIGHT_GREY_ALTERNATE (123)
        };
        memcpy(tt_themeColors, tt_themeCopy, sizeof(tt_themeCopy));
    } else if (theme == TT_THEME_COLT) {
        tt_theme = theme;
        double tt_themeCopy[] = {
            36.0, 30.0, 32.0,    // TT_COLOR_BACKGROUND (0)
            154.0, 141.0, 133.0, // TT_COLOR_BACKGROUND_HIGHLIGHT (3)
            118.0, 107.0, 104.0, // TT_COLOR_BACKGROUND_ALTERNATE (6)
            189.0, 200.0, 203.0, // TT_COLOR_BACKGROUND_COMPLEMENT (9)
            189.0, 200.0, 203.0, // TT_COLOR_TEXT (12)
            143.0, 134.0, 134.0, // TT_COLOR_TEXT_HIGHLIGHT (15)
            154.0, 160.0, 160.0, // TT_COLOR_TEXT_ALTERNATE (18)
            52.0, 40.0, 40.0,    // TT_COLOR_TEXT_COMPLEMENT (21)
            75.0, 66.0, 66.0,    // TT_COLOR_COMPONENT (24)
            123.0, 112.0, 109.0, // TT_COLOR_COMPONENT_HIGHLIGHT (27)
            16.0, 8.0, 8.0,      // TT_COLOR_COMPONENT_ALTERNATE (30)
            97.0, 82.0, 73.0,    // TT_COLOR_COMPONENT_COMPLEMENT (33)
            241.0, 226.0, 212.0, // TT_COLOR_TERTIARY (36)
            82.0, 27.0, 13.0,    // TT_COLOR_TERTIARY_HIGHLIGHT (39)
            98.0, 43.0, 29.0,    // TT_COLOR_TERTIARY_ALTERNATE (42)
            80.0, 40.0, 0.0,     // TT_COLOR_TERTIARY_COMPLEMENT (45)
            250.0, 14.0, 0.0,    // TT_COLOR_RED (48)
            247.0, 69.0, 64.0,   // TT_COLOR_RED_ALTERNATE (51)
            247.0, 143.0, 64.0,  // TT_COLOR_ORANGE (54)
            247.0, 111.0, 64.0,  // TT_COLOR_ORANGE_ALTERNATE (57)
            250.0, 238.0, 0.0,   // TT_COLOR_YELLOW (60)
            250.0, 199.0, 0.0,   // TT_COLOR_YELLOW_ALTERNATE (63)
            19.0, 236.0, 48.0,   // TT_COLOR_GREEN (66)
            0.0, 185.0, 29.0,    // TT_COLOR_GREEN_ALTERNATE (69)
            5.0, 201.0, 242.0,   // TT_COLOR_CYAN (72)
            5.0, 242.0, 206.0,   // TT_COLOR_CYAN_ALTERNATE (75)
            40.0, 56.0, 245.0,   // TT_COLOR_BLUE (78)
            79.0, 98.0, 245.0,   // TT_COLOR_BLUE_ALTERNATE (81)
            142.0, 75.0, 245.0,  // TT_COLOR_PURPLE (84)
            167.0, 110.0, 244.0, // TT_COLOR_PURPLE_ALTERNATE (87)
            216.0, 63.0, 245.0,  // TT_COLOR_MAGENTA (90)
            213.0, 110.0, 245.0, // TT_COLOR_MAGENTA_ALTERNATE (93)
            245.0, 17.0, 177.0,  // TT_COLOR_PINK (96)
            250.0, 112.0, 228.0, // TT_COLOR_PINK_ALTERNATE (99)
            0.0, 0.0, 0.0,       // TT_COLOR_BLACK (102)
            7.0, 7.0, 7.0,       // TT_COLOR_BLACK_ALTERNATE (105)
            255.0, 255.0, 255.0, // TT_COLOR_WHITE (108)
            238.0, 238.0, 238.0, // TT_COLOR_WHITE_ALTERNATE (111)
            56.0, 56.0, 56.0,    // TT_COLOR_DARK_GREY (114)
            81.0, 81.0, 81.0,    // TT_COLOR_DARK_GREY_ALTERNATE (117)
            187.0, 187.0, 187.0, // TT_COLOR_LIGHT_GREY (120)
            199.0, 199.0, 199.0, // TT_COLOR_LIGHT_GREY_ALTERNATE (123)
        };
        memcpy(tt_themeColors, tt_themeCopy, sizeof(tt_themeCopy));
    } else if (theme == TT_THEME_NAVY) {
        tt_theme = theme;
        double tt_themeCopy[] = {
            212.0, 201.0, 190.0, // TT_COLOR_BACKGROUND (0)
            255.0, 253.0, 250.0, // TT_COLOR_BACKGROUND_HIGHLIGHT (3)
            241.0, 239.0, 236.0, // TT_COLOR_BACKGROUND_ALTERNATE (6)
            241.0, 239.0, 236.0, // TT_COLOR_BACKGROUND_COMPLEMENT (9)
            0.0, 0.0, 0.0,       // TT_COLOR_TEXT (12)
            112.0, 146.0, 182.0, // TT_COLOR_TEXT_HIGHLIGHT (15)
            241.0, 239.0, 236.0, // TT_COLOR_TEXT_ALTERNATE (18)
            74.0, 108.0, 144.0,  // TT_COLOR_TEXT_COMPLEMENT (21)
            18.0, 52.0, 88.0,    // TT_COLOR_COMPONENT (24)
            74.0, 108.0, 144.0,  // TT_COLOR_COMPONENT_HIGHLIGHT (27)
            47.0, 81.0, 117.0,   // TT_COLOR_COMPONENT_ALTERNATE (30)
            208.0, 206.0, 203.0, // TT_COLOR_COMPONENT_COMPLEMENT (33)
            255.0, 253.0, 250.0, // TT_COLOR_TERTIARY (36)
            185.0, 219.0, 255.0, // TT_COLOR_TERTIARY_HIGHLIGHT (39)
            145.0, 242.0, 255.0, // TT_COLOR_TERTIARY_ALTERNATE (42)
            38.0, 204.0, 216.0,  // TT_COLOR_TERTIARY_COMPLEMENT (45)
            250.0, 14.0, 0.0,    // TT_COLOR_RED (48)
            247.0, 69.0, 64.0,   // TT_COLOR_RED_ALTERNATE (51)
            247.0, 143.0, 64.0,  // TT_COLOR_ORANGE (54)
            247.0, 111.0, 64.0,  // TT_COLOR_ORANGE_ALTERNATE (57)
            250.0, 238.0, 0.0,   // TT_COLOR_YELLOW (60)
            250.0, 199.0, 0.0,   // TT_COLOR_YELLOW_ALTERNATE (63)
            19.0, 236.0, 48.0,   // TT_COLOR_GREEN (66)
            0.0, 185.0, 29.0,    // TT_COLOR_GREEN_ALTERNATE (69)
            5.0, 201.0, 242.0,   // TT_COLOR_CYAN (72)
            5.0, 242.0, 206.0,   // TT_COLOR_CYAN_ALTERNATE (75)
            40.0, 56.0, 245.0,   // TT_COLOR_BLUE (78)
            79.0, 98.0, 245.0,   // TT_COLOR_BLUE_ALTERNATE (81)
            142.0, 75.0, 245.0,  // TT_COLOR_PURPLE (84)
            167.0, 110.0, 244.0, // TT_COLOR_PURPLE_ALTERNATE (87)
            216.0, 63.0, 245.0,  // TT_COLOR_MAGENTA (90)
            213.0, 110.0, 245.0, // TT_COLOR_MAGENTA_ALTERNATE (93)
            245.0, 17.0, 233.0,  // TT_COLOR_PINK (96)
            250.0, 112.0, 228.0, // TT_COLOR_PINK_ALTERNATE (99)
            0.0, 0.0, 0.0,       // TT_COLOR_BLACK (102)
            7.0, 7.0, 7.0,       // TT_COLOR_BLACK_ALTERNATE (105)
            255.0, 255.0, 255.0, // TT_COLOR_WHITE (108)
            238.0, 238.0, 238.0, // TT_COLOR_WHITE_ALTERNATE (111)
            56.0, 56.0, 56.0,    // TT_COLOR_DARK_GREY (114)
            81.0, 81.0, 81.0,    // TT_COLOR_DARK_GREY_ALTERNATE (117)
            187.0, 187.0, 187.0, // TT_COLOR_LIGHT_GREY (120)
            199.0, 199.0, 199.0, // TT_COLOR_LIGHT_GREY_ALTERNATE (123)
        };
        memcpy(tt_themeColors, tt_themeCopy, sizeof(tt_themeCopy));
    } else {
        tt_theme = TT_THEME_LIGHT;
        double tt_themeCopy[] = {
                255.0, 255.0, 255.0, // TT_COLOR_BACKGROUND (0)
                222.0, 222.0, 222.0, // TT_COLOR_BACKGROUND_HIGHLIGHT (3)
                230.0, 230.0, 230.0, // TT_COLOR_BACKGROUND_ALTERNATE (6)
                255.0, 255.0, 255.0, // TT_COLOR_BACKGROUND_COMPLEMENT (9)
                0.0, 0.0, 0.0,       // TT_COLOR_TEXT (12)
                150.0, 150.0, 150.0, // TT_COLOR_TEXT_HIGHLIGHT (15)
                0.0, 0.0, 0.0,       // TT_COLOR_TEXT_ALTERNATE (18)
                140.0, 140.0, 140.0, // TT_COLOR_TEXT_COMPLEMENT (21)
                180.0, 180.0, 180.0, // TT_COLOR_COMPONENT (24)
                160.0, 160.0, 160.0, // TT_COLOR_COMPONENT_HIGHLIGHT (27)
                116.0, 116.0, 116.0, // TT_COLOR_COMPONENT_ALTERNATE (30)
                222.0, 222.0, 222.0, // TT_COLOR_COMPONENT_COMPLEMENT (33)
                0.0, 255.0, 255.0,   // TT_COLOR_TERTIARY (36)
                0.0, 221.0, 233.0,   // TT_COLOR_TERTIARY_HIGHLIGHT (39)
                155.0, 199.0, 242.0, // TT_COLOR_TERTIARY_ALTERNATE (42)
                149.0, 217.0, 222.0, // TT_COLOR_TERTIARY_COMPLEMENT (45)
                250.0, 14.0, 0.0,    // TT_COLOR_RED (48)
                247.0, 69.0, 64.0,   // TT_COLOR_RED_ALTERNATE (51)
                247.0, 143.0, 64.0,  // TT_COLOR_ORANGE (54)
                247.0, 111.0, 64.0,  // TT_COLOR_ORANGE_ALTERNATE (57)
                250.0, 238.0, 0.0,   // TT_COLOR_YELLOW (60)
                250.0, 199.0, 0.0,   // TT_COLOR_YELLOW_ALTERNATE (63)
                19.0, 236.0, 48.0,   // TT_COLOR_GREEN (66)
                0.0, 185.0, 29.0,    // TT_COLOR_GREEN_ALTERNATE (69)
                5.0, 201.0, 242.0,   // TT_COLOR_CYAN (72)
                5.0, 242.0, 206.0,   // TT_COLOR_CYAN_ALTERNATE (75)
                40.0, 56.0, 245.0,   // TT_COLOR_BLUE (78)
                79.0, 98.0, 245.0,   // TT_COLOR_BLUE_ALTERNATE (81)
                142.0, 75.0, 245.0,  // TT_COLOR_PURPLE (84)
                167.0, 110.0, 244.0, // TT_COLOR_PURPLE_ALTERNATE (87)
                216.0, 63.0, 245.0,  // TT_COLOR_MAGENTA (90)
                213.0, 110.0, 245.0, // TT_COLOR_MAGENTA_ALTERNATE (93)
                245.0, 17.0, 233.0,  // TT_COLOR_PINK (96)
                250.0, 112.0, 228.0, // TT_COLOR_PINK_ALTERNATE (99)
                0.0, 0.0, 0.0,       // TT_COLOR_BLACK (102)
                7.0, 7.0, 7.0,       // TT_COLOR_BLACK_ALTERNATE (105)
                255.0, 255.0, 255.0, // TT_COLOR_WHITE (108)
                238.0, 238.0, 238.0, // TT_COLOR_WHITE_ALTERNATE (111)
                56.0, 56.0, 56.0,    // TT_COLOR_DARK_GREY (114)
                81.0, 81.0, 81.0,    // TT_COLOR_DARK_GREY_ALTERNATE (117)
                187.0, 187.0, 187.0, // TT_COLOR_LIGHT_GREY (120)
                199.0, 199.0, 199.0, // TT_COLOR_LIGHT_GREY_ALTERNATE (123)
        };
        memcpy(tt_themeColors, tt_themeCopy, sizeof(tt_themeCopy));
    }
    turtleBgColor(tt_themeColors[TT_COLOR_BACKGROUND], tt_themeColors[TT_COLOR_BACKGROUND + 1], tt_themeColors[TT_COLOR_BACKGROUND + 2]);
}

/* ribbon */

/* ribbon variables */

tt_ribbon_t tt_ribbon;

/* initialise ribbon */
int32_t tt_ribbonInit(const char *filename) {
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
    return tt_ribbonInitInternal(configFile, defaultRibbonFile, fileExists);
}

/* initialise ribbon with a list instead of a config file */
int32_t tt_ribbonInitList(list_t *config) {
    /* load from config file */
    return tt_ribbonInitInternal(NULL, config, 0);
}

/* initialise ribbon */
int32_t tt_ribbonInitInternal(FILE *configFile, list_t *configList, int8_t fileExists) {
    /* enable ribbon */
    tt_enabled.ribbonEnabled = 1;
    if (tt_enabled.turtleToolsEnabled == 0) {
        tt_enabled.turtleToolsEnabled = 1;
        tt_elements.all = list_init();
    }
    tt_ribbon.element = TT_ELEMENT_RIBBON;
    tt_ribbon.enabled = TT_ELEMENT_ENABLED;
    tt_elementResetColor((void *) &tt_ribbon);
    /* set ribbon parameters */
    tt_ribbon.marginSize = (turtle.initbounds[3] - turtle.initbounds[1]) / 36.0; // number of pixels between different items in the ribbon (not affected by ribbonSize)
    tt_ribbon.mainselect[0] = -1;
    tt_ribbon.mainselect[1] = -1;
    tt_ribbon.mainselect[2] = -1;
    tt_ribbon.subselect[0] = -1;
    tt_ribbon.subselect[1] = -1;
    tt_ribbon.subselect[2] = -1;
    tt_ribbon.output[0] = 0;
    tt_ribbon.output[1] = -1;
    tt_ribbon.output[2] = -1;

    tt_ribbon.mouseDown = 0;

    tt_ribbon.bounds[0] = turtle.initbounds[0];
    tt_ribbon.bounds[1] = turtle.initbounds[1];
    tt_ribbon.bounds[2] = turtle.initbounds[2];
    tt_ribbon.bounds[3] = turtle.initbounds[3];

    tt_ribbon.ribbonSize = (turtle.initbounds[3] - turtle.initbounds[1]) / 360.0; // 1 is default for 640 by 360 coordiantes, below 1 is smaller, above 1 is larger (scales as a multiplier, 0.1 is 100x smaller than 10)
    if (tt_ribbon.options != NULL) {
        /* support reinitialising to change ribbon contents */
        list_free(tt_ribbon.options);
    }
    if (tt_ribbon.lengths != NULL) {
        list_free(tt_ribbon.lengths);
    }
    tt_ribbon.options = list_init();
    tt_ribbon.lengths = list_init();

    /* load ribbon options */
    char line[1024]; // maximum size of any list of options
    while ((fileExists == 0 && tt_ribbon.options -> length < configList -> length) || (fileExists == 1 && fgets(line, 1024, configFile) != NULL)) {
        if (fileExists == 0) {
            memcpy(line, configList -> data[tt_ribbon.options -> length].s, strlen(configList -> data[tt_ribbon.options -> length].s) + 1);
        }
        if (line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0'; // cull newline
        }
        list_append(tt_ribbon.options, (unitype) list_init(), 'r');
        char *item = strtok(line, ",");
        while (item != NULL) {
            if (item[0] == ' ') {
                item++; // cull leading space
            }
            list_append(tt_ribbon.options -> data[tt_ribbon.options -> length - 1].r, (unitype) item, 's');
            item = strtok(NULL, ",");
        }
    }
    if (fileExists) {
        fclose(configFile);
    }

    for (uint32_t i = 0; i < tt_ribbon.options -> length; i++) {
        list_append(tt_ribbon.lengths, (unitype) turtleTextGetStringLength(tt_ribbon.options -> data[i].r -> data[0].s, 7 * tt_ribbon.ribbonSize), 'd');
        double max = 0;
        for (uint32_t j = 1; j < tt_ribbon.options -> data[i].r -> length; j++) {
            double current = turtleTextGetStringLength(tt_ribbon.options -> data[i].r -> data[j].s, 7 * tt_ribbon.ribbonSize);
            if (current > max) {
                max = current;
            }
        }
        list_append(tt_ribbon.lengths, (unitype) max, 'd');
    }
    list_free(configList);
    return 0;
}

/* render ribbon */
void tt_ribbonUpdate() {
    tt_setColor(tt_ribbon.color[TT_COLOR_SLOT_RIBBON_TOP]);
    turtleRectangle(tt_ribbon.bounds[0], tt_ribbon.bounds[3] - tt_ribbon.ribbonSize * 10, tt_ribbon.bounds[2], tt_ribbon.bounds[3]);
    tt_setColor(tt_ribbon.color[TT_COLOR_SLOT_RIBBON_TEXT]);
    double cutoff = tt_ribbon.bounds[0] + tt_ribbon.marginSize;
    tt_ribbon.mainselect[0] = -1;
    tt_ribbon.subselect[0] = -1;
    for (uint32_t i = 0; i < tt_ribbon.options -> length; i++) {
        double prevCutoff = cutoff;
        if (i == (uint32_t) tt_ribbon.mainselect[2]) {
            double xLeft = prevCutoff - tt_ribbon.marginSize / 2.0;
            double xRight = prevCutoff + tt_ribbon.lengths -> data[i * 2 + 1].d + tt_ribbon.marginSize / 2.0;
            double yDown = tt_ribbon.bounds[3] - 10 * tt_ribbon.ribbonSize - 15 * tt_ribbon.ribbonSize * (tt_ribbon.options -> data[i].r -> length - 1) - tt_ribbon.marginSize / 2.0;
            tt_setColor(tt_ribbon.color[TT_COLOR_SLOT_RIBBON_DROPDOWN]);
            turtleRectangle(xLeft, tt_ribbon.bounds[3] - 10 * tt_ribbon.ribbonSize, xRight, yDown); // ribbon highlight
            for (uint32_t j = 1; j < tt_ribbon.options -> data[i].r -> length; j++) {
                if (tt_globals.elementLogicTypeOld <= TT_ELEMENT_RIBBON) {
                    if (turtle.mouseY > tt_ribbon.bounds[3] - 10 * tt_ribbon.ribbonSize - 15 * tt_ribbon.ribbonSize * j - tt_ribbon.marginSize / 4.0 && turtle.mouseY < tt_ribbon.bounds[3] - 10 * tt_ribbon.ribbonSize && turtle.mouseX > xLeft && turtle.mouseX < xRight && tt_ribbon.subselect[0] == -1) {
                        tt_setColor(tt_ribbon.color[TT_COLOR_SLOT_RIBBON_HOVER]);
                        turtleRectangle(xLeft, tt_ribbon.bounds[3] - 10 * tt_ribbon.ribbonSize - 15 * tt_ribbon.ribbonSize * (j - 1) - tt_ribbon.marginSize / 4.0, xRight, tt_ribbon.bounds[3] - 10 * tt_ribbon.ribbonSize - 15 * tt_ribbon.ribbonSize * j - tt_ribbon.marginSize / 3.0); // dropdown highlight
                        tt_ribbon.subselect[0] = j;
                    }
                }
                tt_setColor(tt_ribbon.color[TT_COLOR_SLOT_RIBBON_TEXT]);
                turtleTextWriteUnicode(tt_ribbon.options -> data[i].r -> data[j].s, prevCutoff, tt_ribbon.bounds[3] - 5.5 * tt_ribbon.ribbonSize - j * 15 * tt_ribbon.ribbonSize, 7 * tt_ribbon.ribbonSize, 0);
            }
        }
        cutoff += tt_ribbon.lengths -> data[i * 2].d + tt_ribbon.marginSize;
        if (tt_globals.elementLogicTypeOld <= TT_ELEMENT_RIBBON) {
            if (turtle.mouseY > tt_ribbon.bounds[3] - 10 * tt_ribbon.ribbonSize && turtle.mouseY < tt_ribbon.bounds[3] && turtle.mouseX > tt_ribbon.bounds[0] + tt_ribbon.marginSize / 2.0 && turtle.mouseX < cutoff - tt_ribbon.marginSize / 2.0 && tt_ribbon.mainselect[0] == -1) { // -217, -195, -164
                tt_setColor(tt_ribbon.color[TT_COLOR_SLOT_RIBBON_SELECT]);
                turtleRectangle(prevCutoff - tt_ribbon.marginSize / 2.0, tt_ribbon.bounds[3] - tt_ribbon.ribbonSize, cutoff - tt_ribbon.marginSize / 2.0, tt_ribbon.bounds[3] - 9 * tt_ribbon.ribbonSize); // render dropdown
                tt_ribbon.mainselect[0] = i;
                tt_globals.elementLogicType = TT_ELEMENT_RIBBON;
            }
        }
        tt_setColor(tt_ribbon.color[TT_COLOR_SLOT_RIBBON_TEXT]);
        turtleTextWriteUnicode(tt_ribbon.options -> data[i].r -> data[0].s, prevCutoff, tt_ribbon.bounds[3] - 5.5 * tt_ribbon.ribbonSize, 7 * tt_ribbon.ribbonSize, 0);
    }
    if (tt_globals.elementLogicTypeOld <= TT_ELEMENT_RIBBON) {
        if (tt_ribbon.mainselect[2] > -1) {
            tt_globals.elementLogicType = TT_ELEMENT_RIBBON;
        }
        if (turtleMouseDown()) { // this is hideous
            if (tt_ribbon.mouseDown == 0) {
                tt_ribbon.mouseDown = 1;
                if (tt_ribbon.subselect[0] == tt_ribbon.subselect[1] && tt_ribbon.subselect[0] != -1) {
                    tt_ribbon.subselect[2] = tt_ribbon.subselect[0];
                    tt_ribbon.output[0] = 1;
                    tt_ribbon.output[1] = tt_ribbon.mainselect[2];
                    tt_ribbon.output[2] = tt_ribbon.subselect[2];
                }
                if (tt_ribbon.mainselect[0] == tt_ribbon.mainselect[1]) {
                    if (tt_ribbon.mainselect[0] == tt_ribbon.mainselect[2]) {
                        tt_ribbon.mainselect[3] = -1;
                    } else {
                        tt_ribbon.mainselect[2] = tt_ribbon.mainselect[0];
                        tt_globals.elementLogicType = TT_ELEMENT_RIBBON;
                    }
                }
            }
        } else {
            if (tt_ribbon.mouseDown == 1) {
                if (tt_ribbon.subselect[0] != -1) {
                    tt_ribbon.subselect[2] = tt_ribbon.subselect[0];
                    tt_ribbon.output[0] = 1;
                    tt_ribbon.output[1] = tt_ribbon.mainselect[2];
                    tt_ribbon.output[2] = tt_ribbon.subselect[2];
                    tt_ribbon.mainselect[2] = -1;
                    tt_ribbon.subselect[2] = -1;
                }
            }
            if (tt_ribbon.mainselect[3] == -1 && tt_ribbon.mainselect[0] == tt_ribbon.mainselect[2]) {
                tt_ribbon.mainselect[2] = -1;
            }
            tt_ribbon.mainselect[3] = 0;
            tt_ribbon.mouseDown = 0;
            tt_ribbon.mainselect[1] = tt_ribbon.mainselect[0];
            tt_ribbon.subselect[1] = tt_ribbon.subselect[0];
        }
    }
}

/* popup */

/* popup variables */

tt_popup_t tt_popup;

/* initialise popup */
int32_t tt_popupInit(char *filename) {
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
    return tt_popupInitInternal(configFile, defaultPopupFile, fileExists);
}

/* initialise popup with a list instead of a config file */
int32_t tt_popupInitList(list_t *config) {
    return tt_popupInitInternal(NULL, config, 0);
}

/* initialise popup */
int32_t tt_popupInitInternal(FILE *configFile, list_t *configList, int8_t fileExists) {
    tt_enabled.popupEnabled = 1;
    if (tt_enabled.turtleToolsEnabled == 0) {
        tt_enabled.turtleToolsEnabled = 1;
        tt_elements.all = list_init();
    }
    tt_popup.element = TT_ELEMENT_POPUP;
    tt_popup.enabled = TT_ELEMENT_ENABLED;
    tt_elementResetColor((void *) &tt_popup);
    tt_popup.output[0] = 0;
    tt_popup.output[1] = -1;
    tt_popup.mouseDown = 0;
    tt_popup.size = (turtle.initbounds[3] - turtle.initbounds[1]) / 72;
    tt_popup.style = 0;

    char line[256] = {1, 0}; // maximum size of message or option
    /* read popup message */
    if (fileExists) {
        if (fgets(line, 256, configFile) != NULL) {
            if (line[strlen(line) - 1] == '\n') {
                line[strlen(line) - 1] = '\0'; // cull newline
            }
            tt_popup.message = strdup(line);
        }
    } else {
        tt_popup.message = strdup(configList -> data[0].s);
    }
    /* read popup options */
    double defaultPadding = (turtle.initbounds[2] - turtle.initbounds[0]) / 50;
    double buttonWidth = defaultPadding;
    tt_popup.options = list_init();
    while ((fileExists == 0 && tt_popup.options -> length < configList -> length - 1) || (fileExists == 1 && fgets(line, 256, configFile) != NULL)) {
        if (fileExists == 1) {
            if (line[strlen(line) - 1] == '\n') {
                line[strlen(line) - 1] = '\0'; // cull newline
            }
            list_append(tt_popup.options, (unitype) strdup(line), 's');
        } else {
            list_append(tt_popup.options, configList -> data[tt_popup.options -> length + 1], 's');
        }
        buttonWidth += turtleTextGetUnicodeLength(tt_popup.options -> data[tt_popup.options -> length - 1].s, tt_popup.size) + defaultPadding;
    }
    if (fileExists) {
        fclose(configFile);
    }
    list_free(configList);
    double centerY = (turtle.initbounds[1] + turtle.initbounds[3]) / 2;
    double height = (turtle.initbounds[3] - turtle.initbounds[1]) / 9;
    double centerX = (turtle.initbounds[0] + turtle.initbounds[2]) / 2;
    double messageWidth = turtleTextGetUnicodeLength(tt_popup.message, tt_popup.size) + defaultPadding;
    double width = messageWidth;
    if (messageWidth < buttonWidth) {
        width = buttonWidth;
    }
    tt_popup.minX = centerX - width / 2;
    tt_popup.minY = centerY - height / 2;
    tt_popup.maxX = centerX + width / 2;
    tt_popup.maxY = centerY + height / 2;
    return 0;
}

/* render popup */
void tt_popupUpdate() {
    if (turtle.close == 1) {
        tt_globals.elementLogicType = TT_ELEMENT_POPUP;
        tt_setColor(tt_popup.color[TT_COLOR_SLOT_POPUP_BOX]);
        turtleRectangle(tt_popup.minX, tt_popup.minY, tt_popup.maxX, tt_popup.maxY);
        double textX = tt_popup.minX + (tt_popup.maxX - tt_popup.minX) / 2;
        double textY = tt_popup.maxY - tt_popup.size * 2;
        tt_setColor(tt_popup.color[TT_COLOR_SLOT_POPUP_TEXT]);
        turtleTextWriteUnicode(tt_popup.message, textX, textY, tt_popup.size, 50);
        textY -= tt_popup.size * 4;
        double fullLength = 0;
        for (uint32_t i = 0; i < tt_popup.options -> length; i++) {
            fullLength += turtleTextGetStringLength(tt_popup.options -> data[i].s, tt_popup.size);
        }
        /* we have the length of the strings, now we pad with n + 1 padding regions */
        double padding = (tt_popup.maxX - tt_popup.minX - fullLength) / (tt_popup.options -> length + 1);
        textX = tt_popup.minX + padding;
        char flagged = 0;
        if (!turtleMouseDown() && tt_popup.mouseDown == 1) {
            flagged = 1; // flagged for mouse misbehaviour
        }
        for (uint32_t i = 0; i < tt_popup.options -> length; i++) {
            double strLen = turtleTextGetStringLength(tt_popup.options -> data[i].s, tt_popup.size);
            if (turtle.mouseX > textX - tt_popup.size && turtle.mouseX < textX + strLen + tt_popup.size && turtle.mouseY > textY - tt_popup.size && turtle.mouseY < textY + tt_popup.size) {
                tt_setColor(tt_popup.color[TT_COLOR_SLOT_POPUP_BUTTON_SELECT]);
                turtleRectangle(textX - tt_popup.size, textY - tt_popup.size, textX + tt_popup.size + strLen, textY + tt_popup.size);
                if (turtleMouseDown()) {
                    if (tt_popup.mouseDown == 0) {
                        tt_popup.mouseDown = 1;
                        if (tt_popup.output[0] == 0) {
                            tt_popup.output[1] = i;
                        }
                        tt_globals.elementLogicType = TT_ELEMENT_NONE;
                    }
                } else {
                    if (tt_popup.mouseDown == 1) {
                        tt_popup.mouseDown = 0;
                        if (tt_popup.output[1] == i) {
                            tt_popup.output[0] = 1;
                        }
                        tt_globals.elementLogicType = TT_ELEMENT_NONE;
                    }
                }
            } else {
                tt_setColor(tt_popup.color[TT_COLOR_SLOT_POPUP_BUTTON]);
                turtleRectangle(textX - tt_popup.size, textY - tt_popup.size, textX + tt_popup.size + strLen, textY + tt_popup.size);
            }
            tt_setColor(tt_popup.color[TT_COLOR_SLOT_POPUP_TEXT]);
            turtleTextWriteUnicode(tt_popup.options -> data[i].s, textX, textY, tt_popup.size, 0);
            textX += strLen + padding;
        }
        if (!turtleMouseDown() && tt_popup.mouseDown == 1 && flagged == 1) {
            tt_popup.mouseDown = 0;
            tt_popup.output[0] = 0;
            tt_popup.output[1] = -1;
            tt_globals.elementLogicType = TT_ELEMENT_NONE;
        }
    }
}

void tt_popupFree() {
    free(tt_popup.message);
}

/* UI tools */

tt_globals_t tt_globals;

int32_t tt_color_default[] = {
    /*         none                           button                         switch                            dial                           slider                          textbox                        dropdown                         scrollbar                      context                          ribbon                           popup               */
    0,                              TT_COLOR_TEXT_ALTERNATE,        TT_COLOR_TEXT_BASE,             TT_COLOR_TEXT_BASE,             TT_COLOR_TEXT_BASE,             TT_COLOR_TEXT_ALTERNATE,        TT_COLOR_TEXT_BASE,             0,                              TT_COLOR_TEXT_ALTERNATE,        TT_COLOR_TEXT_ALTERNATE,        TT_COLOR_TEXT_ALTERNATE,        
    0,                              TT_COLOR_COMPONENT,             TT_COLOR_TEXT_ALTERNATE,        TT_COLOR_TEXT_BASE,             TT_COLOR_COMPONENT_ALTERNATE,   TT_COLOR_COMPONENT_BASE,        TT_COLOR_TEXT_ALTERNATE,        TT_COLOR_COMPONENT_BASE,        TT_COLOR_COMPONENT_BASE,        TT_COLOR_COMPONENT_HIGHLIGHT,   TT_COLOR_COMPONENT_ALTERNATE,   
    0,                              TT_COLOR_COMPONENT_HIGHLIGHT,   TT_COLOR_COMPONENT_BASE,        TT_COLOR_BACKGROUND_BASE,       TT_COLOR_BACKGROUND_COMPLEMENT, TT_COLOR_TEXT_HIGHLIGHT,        TT_COLOR_COMPONENT_BASE,        TT_COLOR_COMPONENT_COMPLEMENT,  TT_COLOR_COMPONENT_HIGHLIGHT,   TT_COLOR_COMPONENT_HIGHLIGHT,   TT_COLOR_COMPONENT_HIGHLIGHT,   
    0,                              TT_COLOR_TEXT_BASE,             TT_COLOR_COMPONENT_HIGHLIGHT,   0,                              0,                              TT_COLOR_TEXT_ALTERNATE,        TT_COLOR_COMPONENT_HIGHLIGHT,   TT_COLOR_BACKGROUND_ALTERNATE,  0,                              TT_COLOR_COMPONENT,             TT_COLOR_COMPONENT,             
    0,                              TT_COLOR_COMPONENT_COMPLEMENT,  TT_COLOR_BACKGROUND_ALTERNATE,  0,                              0,                              TT_COLOR_BLUE,                  TT_COLOR_COMPONENT_HIGHLIGHT,   TT_COLOR_BACKGROUND_HIGHLIGHT,  0,                              TT_COLOR_COMPONENT,             0,                              
    0,                              0,                              TT_COLOR_TERTIARY_BASE,         0,                              0,                              0,                              TT_COLOR_TEXT_ALTERNATE,        0,                              0,                              0,                              0,                              
    0,                              0,                              0,                              0,                              0,                              0,                              0,                              0,                              0,                              0,                              0,                              
    0,                              0,                              0,                              0,                              0,                              0,                              0,                              0,                              0,                              0,                              0,                              
};

void tt_elementResetColor(void *elementp) {
    int32_t elementType = ((tt_button_t *) elementp) -> element;
    for (uint32_t i = 0; i < 8; i++) {
        ((tt_button_t *) elementp) -> color[i] = tt_color_default[i * TT_NUMBER_OF_ELEMENTS + elementType];
    }
}

int32_t tt_elementFree(void *elementp) {
    if (elementp == NULL) {
        return -1;
    }
    switch (((tt_button_t *) elementp) -> element) {
    case TT_ELEMENT_BUTTON:
        tt_buttonFree((tt_button_t *) elementp);
    break;
    case TT_ELEMENT_SWITCH:
        tt_switchFree((tt_switch_t *) elementp);
    break;
    case TT_ELEMENT_DIAL:
        tt_dialFree((tt_dial_t *) elementp);
    break;
    case TT_ELEMENT_SLIDER:
        tt_sliderFree((tt_slider_t *) elementp);
    break;
    case TT_ELEMENT_SCROLLBAR:
        tt_scrollbarFree((tt_scrollbar_t *) elementp);
    break;
    case TT_ELEMENT_CONTEXT:
        tt_contextFree((tt_context_t *) elementp);
    break;
    case TT_ELEMENT_DROPDOWN:
        tt_dropdownFree((tt_dropdown_t *) elementp);
    break;
    case TT_ELEMENT_TEXTBOX:
        tt_textboxFree((tt_textbox_t *) elementp);
    break;
    default:
        return -1;
    break;
    }
    return 0;
}

void tt_hideAllElements() {
    for (int32_t i = 0; i < tt_elements.all -> length; i++) {
        ((tt_button_t *) tt_elements.all -> data[i].p) -> enabled = TT_ELEMENT_HIDE;
    }
}

/* initialise UI elements */

/* create a button */
tt_button_t *tt_buttonInit(char *label, int8_t *variable, double x, double y, double size) {
    if (tt_enabled.buttonEnabled == 0) {
        tt_enabled.buttonEnabled = 1;
        tt_elements.buttons = list_init();
    }
    if (tt_enabled.turtleToolsEnabled == 0) {
        tt_enabled.turtleToolsEnabled = 1;
        tt_globals.elementLogicType = TT_ELEMENT_NONE;
        tt_globals.elementLogicIndex = -1;
        tt_globals.elementLogicTemp = -1;
        tt_elements.all = list_init();
    }
    tt_button_t *buttonp = calloc(1, sizeof(tt_button_t));
    buttonp -> element = TT_ELEMENT_BUTTON;
    buttonp -> enabled = TT_ELEMENT_ENABLED;
    buttonp -> ignored = TT_ELEMENT_NOT_IGNORED;
    if (label == NULL) {
        memcpy(buttonp -> label, "", strlen("") + 1);
    } else {
        memcpy(buttonp -> label, label, strlen(label) + 1);
    }
    tt_elementResetColor(buttonp);
    buttonp -> status = 0;
    buttonp -> x = x;
    buttonp -> y = y;
    buttonp -> size = size;
    if (variable != NULL) {
        *variable = 0; // button starts unpressed
    }
    buttonp -> variable = variable;
    buttonp -> shape = TT_BUTTON_SHAPE_RECTANGLE;
    buttonp -> align = TT_BUTTON_ALIGN_CENTER;
    list_append(tt_elements.buttons, (unitype) (void *) buttonp, 'p');
    list_append(tt_elements.all, (unitype) (void *) buttonp, 'l');
    return buttonp;
}

void tt_buttonFree(tt_button_t *buttonp) {
    list_remove(tt_elements.all, (unitype) (uint64_t) buttonp, 'l');
    list_remove(tt_elements.buttons, (unitype) (void *) buttonp, 'p');
}

/* create a switch */
tt_switch_t *tt_switchInit(char *label, int8_t *variable, double x, double y, double size) {
    if (tt_enabled.switchEnabled == 0) {
        tt_enabled.switchEnabled = 1;
        tt_elements.switches = list_init();
    }
    if (tt_enabled.turtleToolsEnabled == 0) {
        tt_enabled.turtleToolsEnabled = 1;
        tt_globals.elementLogicType = TT_ELEMENT_NONE;
        tt_globals.elementLogicIndex = -1;
        tt_globals.elementLogicTemp = -1;
        tt_elements.all = list_init();
    }
    tt_switch_t *switchp = calloc(1, sizeof(tt_switch_t));
    switchp -> element = TT_ELEMENT_SWITCH;
    switchp -> enabled = TT_ELEMENT_ENABLED;
    switchp -> ignored = TT_ELEMENT_NOT_IGNORED;
    if (label == NULL) {
        memcpy(switchp -> label, "", strlen("") + 1);
    } else {
        memcpy(switchp -> label, label, strlen(label) + 1);
    }
    tt_elementResetColor(switchp);
    switchp -> status = 0;
    switchp -> x = x;
    switchp -> y = y;
    switchp -> size = size;
    switchp -> variable = variable;
    switchp -> style = TT_SWITCH_STYLE_CLASSIC;
    switchp -> align = TT_SWITCH_ALIGN_CENTER;
    list_append(tt_elements.switches, (unitype) (void *) switchp, 'p');
    list_append(tt_elements.all, (unitype) (void *) switchp, 'l');
    return switchp;
}

void tt_switchFree(tt_switch_t *switchp) {
    list_remove(tt_elements.all, (unitype) (uint64_t) switchp, 'l');
    list_remove(tt_elements.switches, (unitype) (void *) switchp, 'p');
}

/* create a dial - make renderNumberFactor 0 to hide dial number */
tt_dial_t *tt_dialInit(char *label, double *variable, tt_dial_scale_t scale, double x, double y, double size, double bottom, double top, double renderNumberFactor) {
    if (tt_enabled.dialEnabled == 0) {
        tt_enabled.dialEnabled = 1;
        tt_elements.dials = list_init();
    }
    if (tt_enabled.turtleToolsEnabled == 0) {
        tt_enabled.turtleToolsEnabled = 1;
        tt_globals.elementLogicType = TT_ELEMENT_NONE;
        tt_globals.elementLogicIndex = -1;
        tt_globals.elementLogicTemp = -1;
        tt_elements.all = list_init();
    }
    tt_dial_t *dialp = calloc(1, sizeof(tt_dial_t));
    dialp -> element = TT_ELEMENT_DIAL;
    dialp -> enabled = TT_ELEMENT_ENABLED;
    dialp -> ignored = TT_ELEMENT_NOT_IGNORED;
    if (label == NULL) {
        memcpy(dialp -> label, "", strlen("") + 1);
    } else {
        memcpy(dialp -> label, label, strlen(label) + 1);
    }
    tt_elementResetColor(dialp);
    dialp -> status[0] = 0;
    dialp -> scale = scale;
    dialp -> x = x;
    dialp -> y = y;
    dialp -> size = size;
    dialp -> range[0] = bottom;
    dialp -> range[1] = top;
    dialp -> variable = variable;
    dialp -> renderNumberFactor = renderNumberFactor;
    if (variable == NULL) {
        dialp -> defaultValue = bottom;
    } else {
        dialp -> defaultValue = *variable;
    }
    list_append(tt_elements.dials, (unitype) (void *) dialp, 'p');
    list_append(tt_elements.all, (unitype) (void *) dialp, 'l');
    return dialp;
}

void tt_dialFree(tt_dial_t *dialp) {
    list_remove(tt_elements.all, (unitype) (uint64_t) dialp, 'l');
    list_remove(tt_elements.dials, (unitype) (void *) dialp, 'p');
}

/* create a slider - make renderNumberFactor 0 to hide slider number */
tt_slider_t *tt_sliderInit(char *label, double *variable, tt_slider_type_t type, tt_slider_align_t align, double x, double y, double size, double length, double bottom, double top, double renderNumberFactor) {
    if (tt_enabled.sliderEnabled == 0) {
        tt_enabled.sliderEnabled = 1;
        tt_elements.sliders = list_init();
    }
    if (tt_enabled.turtleToolsEnabled == 0) {
        tt_enabled.turtleToolsEnabled = 1;
        tt_globals.elementLogicType = TT_ELEMENT_NONE;
        tt_globals.elementLogicIndex = -1;
        tt_globals.elementLogicTemp = -1;
        tt_elements.all = list_init();
    }
    tt_slider_t *sliderp = calloc(1, sizeof(tt_slider_t));
    sliderp -> element = TT_ELEMENT_SLIDER;
    sliderp -> enabled = TT_ELEMENT_ENABLED;
    sliderp -> ignored = TT_ELEMENT_NOT_IGNORED;
    if (label == NULL) {
        memcpy(sliderp -> label, "", strlen("") + 1);
    } else {
        memcpy(sliderp -> label, label, strlen(label) + 1);
    }
    tt_elementResetColor(sliderp);
    sliderp -> status = 0;
    sliderp -> type = type;
    sliderp -> align = align;
    sliderp -> scale = TT_SLIDER_SCALE_LINEAR;
    sliderp -> x = x;
    sliderp -> y = y;
    sliderp -> size = size;
    sliderp -> length = length;
    sliderp -> range[0] = bottom;
    sliderp -> range[1] = top;
    sliderp -> variable = variable;
    sliderp -> renderNumberFactor = renderNumberFactor;
    if (variable == NULL) {
        sliderp -> defaultValue = bottom;
    } else {
        sliderp -> defaultValue = *variable;
    }
    list_append(tt_elements.sliders, (unitype) (void *) sliderp, 'p');
    list_append(tt_elements.all, (unitype) (void *) sliderp, 'l');
    return sliderp;
}

void tt_sliderFree(tt_slider_t *sliderp) {
    list_remove(tt_elements.all, (unitype) (uint64_t) sliderp, 'l');
    list_remove(tt_elements.sliders, (unitype) (void *) sliderp, 'p');
}

/* create a textbox */
tt_textbox_t *tt_textboxInit(char *label, char *variable, uint32_t maxCharacters, double x, double y, double size, double length) {
    if (tt_enabled.textboxEnabled == 0) {
        turtle.unicodeCallback = tt_textboxUnicodeCallback;
        turtle.keyCallback = tt_textboxKeyCallback;
        tt_enabled.textboxEnabled = 1;
        tt_elements.textboxes = list_init();
    }
    if (tt_enabled.turtleToolsEnabled == 0) {
        tt_enabled.turtleToolsEnabled = 1;
        tt_globals.elementLogicType = TT_ELEMENT_NONE;
        tt_globals.elementLogicIndex = -1;
        tt_globals.elementLogicTemp = -1;
        tt_elements.all = list_init();
    }
    tt_textbox_t *textboxp = calloc(1, sizeof(tt_textbox_t));
    textboxp -> element = TT_ELEMENT_TEXTBOX;
    textboxp -> enabled = TT_ELEMENT_ENABLED;
    textboxp -> ignored = TT_ELEMENT_NOT_IGNORED;
    if (label == NULL) {
        memcpy(textboxp -> label, "", strlen("") + 1);
    } else {
        memcpy(textboxp -> label, label, strlen(label) + 1);
    }
    tt_elementResetColor(textboxp);
    textboxp -> status = 0;
    textboxp -> align = TT_TEXTBOX_ALIGN_LEFT;
    textboxp -> x = x;
    textboxp -> y = y;
    textboxp -> size = size;
    textboxp -> length = length;
    if (variable == NULL) {
        textboxp -> text = calloc(maxCharacters + 5, 1);
    } else {
        textboxp -> text = variable;
    }
    textboxp -> maxCharacters = maxCharacters;
    textboxp -> editIndex = 0;
    textboxp -> lastKey = 0;
    textboxp -> keyTimeout = 0;
    textboxp -> renderPixelOffset = 0;
    textboxp -> renderStartingIndex = 0;
    textboxp -> renderNumCharacters = 0;
    list_append(tt_elements.textboxes, (unitype) (void *) textboxp, 'p');
    list_append(tt_elements.all, (unitype) (void *) textboxp, 'l');
    textboxp -> whitelist = NULL;
    textboxp -> blacklist = NULL;
    return textboxp;
}

void tt_textboxFree(tt_textbox_t *textboxp) {
    list_remove(tt_elements.all, (unitype) (uint64_t) textboxp, 'l');
    list_remove(tt_elements.textboxes, (unitype) (void *) textboxp, 'p');
}

void tt_dropdownCalculateMax(tt_dropdown_t *dropdownp) {
    dropdownp -> maxXfactor = 0;
    for (uint32_t i = 0; i < dropdownp -> options -> length; i++) {
        double stringLength = turtleTextGetStringLength(dropdownp -> options -> data[i].s, dropdownp -> size - 1);
        if (stringLength > dropdownp -> maxXfactor) {
            dropdownp -> maxXfactor = stringLength;
        }
    }
}

/* create a dropdown - use a list of strings for options */
tt_dropdown_t *tt_dropdownInit(char *label, list_t *options, int32_t *variable, tt_dropdown_align_t align, double x, double y, double size) {
    if (tt_enabled.dropdownEnabled == 0) {
        tt_enabled.dropdownEnabled = 1;
        tt_elements.dropdowns = list_init();
    }
    if (tt_enabled.turtleToolsEnabled == 0) {
        tt_enabled.turtleToolsEnabled = 1;
        tt_globals.elementLogicType = TT_ELEMENT_NONE;
        tt_globals.elementLogicIndex = -1;
        tt_globals.elementLogicTemp = -1;
        tt_elements.all = list_init();
    }
    tt_dropdown_t *dropdownp = calloc(1, sizeof(tt_dropdown_t));
    dropdownp -> element = TT_ELEMENT_DROPDOWN;
    dropdownp -> enabled = TT_ELEMENT_ENABLED;
    dropdownp -> ignored = TT_ELEMENT_NOT_IGNORED;
    if (label == NULL) {
        memcpy(dropdownp -> label, "", strlen("") + 1);
    } else {
        memcpy(dropdownp -> label, label, strlen(label) + 1);
    }
    tt_elementResetColor(dropdownp);
    dropdownp -> options = options;
    if (variable == NULL) {
        dropdownp -> index = 0;
    } else {
        dropdownp -> index = *variable;
    }
    dropdownp -> value = dropdownp -> index;
    dropdownp -> status = 0;
    dropdownp -> align = align;
    dropdownp -> direction = TT_DROPDOWN_DIRECTION_AUTO;
    dropdownp -> autoLowerBound = turtle.initbounds[1];
    dropdownp -> autoUpperBound = turtle.initbounds[3];
    dropdownp -> x = x;
    dropdownp -> y = y;
    dropdownp -> size = size;
    dropdownp -> variable = variable;
    tt_dropdownCalculateMax(dropdownp);
    list_append(tt_elements.dropdowns, (unitype) (void *) dropdownp, 'p');
    list_append(tt_elements.all, (unitype) (void *) dropdownp, 'l');
    return dropdownp;
}

void tt_dropdownFree(tt_dropdown_t *dropdownp) {
    list_free(dropdownp -> options);
    list_remove(tt_elements.all, (unitype) (uint64_t) dropdownp, 'l');
    list_remove(tt_elements.dropdowns, (unitype) (void *) dropdownp, 'p');
}

/* create a scrollbar */
tt_scrollbar_t *tt_scrollbarInit(double *variable, tt_scrollbar_type_t type, double x, double y, double size, double length, double barPercentage) {
    if (tt_enabled.scrollbarEnabled == 0) {
        tt_enabled.scrollbarEnabled = 1;
        tt_elements.scrollbars = list_init();
    }
    if (tt_enabled.turtleToolsEnabled == 0) {
        tt_enabled.turtleToolsEnabled = 1;
        tt_globals.elementLogicType = TT_ELEMENT_NONE;
        tt_globals.elementLogicIndex = -1;
        tt_globals.elementLogicTemp = -1;
        tt_elements.all = list_init();
    }
    tt_scrollbar_t *scrollbarp = calloc(1, sizeof(tt_scrollbar_t));
    scrollbarp -> element = TT_ELEMENT_SCROLLBAR;
    scrollbarp -> enabled = TT_ELEMENT_ENABLED;
    scrollbarp -> ignored = TT_ELEMENT_NOT_IGNORED;
    tt_elementResetColor(scrollbarp);
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

void tt_scrollbarFree(tt_scrollbar_t *scrollbarp) {
    list_remove(tt_elements.all, (unitype) (uint64_t) scrollbarp, 'l');
    list_remove(tt_elements.scrollbars, (unitype) (void *) scrollbarp, 'p');
}

void tt_contextCalculateMax(tt_context_t *contextp) {
    contextp -> maxXfactor = 0;
    for (uint32_t i = 0; i < contextp -> options -> length; i++) {
        double stringLength = turtleTextGetStringLength(contextp -> options -> data[i].s, contextp -> size - 1);
        if (stringLength > contextp -> maxXfactor) {
            contextp -> maxXfactor = stringLength;
        }
    }
}

/* create a context menu */
tt_context_t *tt_contextInit(list_t *options, int32_t *variable, double x, double y, double size) {
    if (tt_enabled.contextEnabled == 0) {
        tt_enabled.contextEnabled = 1;
        tt_elements.contexts = list_init();
    }
    if (tt_enabled.turtleToolsEnabled == 0) {
        tt_enabled.turtleToolsEnabled = 1;
        tt_globals.elementLogicType = TT_ELEMENT_NONE;
        tt_globals.elementLogicIndex = -1;
        tt_globals.elementLogicTemp = -1;
        tt_elements.all = list_init();
    }
    tt_context_t *contextp = calloc(1, sizeof(tt_context_t));
    contextp -> element = TT_ELEMENT_CONTEXT;
    contextp -> enabled = TT_ELEMENT_ENABLED;
    contextp -> ignored = TT_ELEMENT_NOT_IGNORED;
    tt_elementResetColor(contextp);
    contextp -> options = options;
    contextp -> index = -1;
    contextp -> value = -1;
    contextp -> status = 0;
    contextp -> x = x;
    contextp -> y = y;
    contextp -> size = size;
    if (variable != NULL) {
        *variable = -1;
    }
    contextp -> variable = variable;
    tt_contextCalculateMax(contextp);
    contextp -> direction = TT_CONTEXT_DIRECTION_AUTO;
    contextp -> autoLowerBound = turtle.initbounds[1];
    contextp -> autoRightBound = turtle.initbounds[2];
    list_append(tt_elements.contexts, (unitype) (void *) contextp, 'p');
    list_append(tt_elements.all, (unitype) (void *) contextp, 'l');
    return contextp;
}

void tt_contextFree(tt_context_t *contextp) {
    list_free(contextp -> options);
    list_remove(tt_elements.all, (unitype) (uint64_t) contextp, 'l');
    list_remove(tt_elements.contexts, (unitype) (void *) contextp, 'p');
}

void tt_buttonUpdate(tt_button_t *buttonp) {
    if (buttonp -> variable != NULL) {
        buttonp -> value = *buttonp -> variable;
    }
    if (buttonp -> enabled == TT_ELEMENT_HIDE) {
        buttonp -> status = 0;
        return;
    }
    double buttonLeftX = buttonp -> x;
    double buttonRightX = buttonp -> x;
    double buttonY = buttonp -> y;
    double buttonWidth = turtleTextGetUnicodeLength(buttonp -> label, buttonp -> size - 1) + buttonp -> size * 0.8;
    if (buttonp -> align == TT_BUTTON_ALIGN_CENTER) {
        buttonLeftX -= buttonWidth / 2;
        buttonRightX += buttonWidth / 2;
    } else if (buttonp -> align == TT_BUTTON_ALIGN_LEFT) {
        buttonRightX += buttonWidth;
    } else if (buttonp -> align == TT_BUTTON_ALIGN_RIGHT) {
        buttonLeftX -= buttonWidth;
    }
    double buttonHeight = buttonp -> size * 1.75;
    if (buttonp -> status > 0) {
        tt_setColor(buttonp -> color[TT_COLOR_SLOT_BUTTON_CLICKED]);
    } else if (buttonp -> status == 0) {
        tt_setColor(buttonp -> color[TT_COLOR_SLOT_BUTTON]);
    } else {
        tt_setColor(buttonp -> color[TT_COLOR_SLOT_BUTTON_SELECT]);
    }
    if (buttonp -> shape == TT_BUTTON_SHAPE_RECTANGLE) {
        turtleRectangle(buttonLeftX, buttonY - buttonHeight / 2, buttonRightX, buttonY + buttonHeight / 2);
    } else if (buttonp -> shape == TT_BUTTON_SHAPE_ROUNDED_RECTANGLE) {
        turtlePenSize(buttonp -> size);
        turtleGoto(buttonLeftX + buttonp -> size / 2, buttonY - buttonHeight / 2 + buttonp -> size / 2);
        turtlePenDown();
        turtleGoto(buttonRightX - buttonp -> size / 2, buttonY - buttonHeight / 2 + buttonp -> size / 2);
        turtleGoto(buttonRightX - buttonp -> size / 2, buttonY + buttonHeight / 2 - buttonp -> size / 2);
        turtleGoto(buttonLeftX + buttonp -> size / 2, buttonY + buttonHeight / 2 - buttonp -> size / 2);
        turtleGoto(buttonLeftX + buttonp -> size / 2, buttonY - buttonHeight / 2 + buttonp -> size / 2);
        turtlePenUp();
        turtleRectangle(buttonLeftX + buttonWidth / 4, buttonY - buttonHeight / 4, buttonRightX - buttonWidth / 4, buttonY + buttonHeight / 4);
    } else if (buttonp -> shape == TT_BUTTON_SHAPE_CIRCLE) {
        turtleGoto((buttonLeftX + buttonRightX) / 2, buttonY);
        turtlePenSize(buttonWidth);
        turtlePenDown();
        turtlePenUp();
    }
    tt_setColor(buttonp -> color[TT_COLOR_SLOT_BUTTON_TEXT]);
    if (buttonp -> shape == TT_BUTTON_SHAPE_TEXT) {
        if (buttonp -> status == 1) {
            tt_setColor(buttonp -> color[TT_COLOR_SLOT_BUTTON_SELECT]);
        } else if (buttonp -> status == -1) {
            tt_setColor(buttonp -> color[TT_COLOR_SLOT_BUTTON_TEXT]);
        } else {
            tt_setColor(buttonp -> color[TT_COLOR_SLOT_BUTTON_SELECTED_TEXT]);
        }
    }
    turtleTextWriteUnicode(buttonp -> label, (buttonLeftX + buttonRightX) / 2, buttonY, buttonp -> size - 1, 50);
    /* mouse */
    if (buttonp -> enabled == TT_ELEMENT_ENABLED && (tt_globals.elementLogicTypeOld < TT_ELEMENT_BUTTON || (tt_globals.elementLogicTypeOld == TT_ELEMENT_BUTTON && tt_globals.elementLogicIndexOld <= (int32_t) tt_globals.elementLogicTemp))) {
        if (turtleMouseDown()) {
            if (buttonp -> status == 2) {
                buttonp -> status = 1;
            }
            if (buttonp -> status == -1) {
                buttonp -> status = 2;
            }
        } else {
            if (buttonp -> shape == TT_BUTTON_SHAPE_CIRCLE) {
                if ((turtle.mouseX - (buttonLeftX + buttonRightX) / 2) * (turtle.mouseX - (buttonLeftX + buttonRightX) / 2) + (turtle.mouseY - buttonY) * (turtle.mouseY - buttonY) < buttonWidth * buttonWidth / 4) {
                    buttonp -> status = -1;
                    tt_globals.elementLogicType = TT_ELEMENT_BUTTON;
                    tt_globals.elementLogicIndex = tt_globals.elementLogicTemp;
                } else {
                    buttonp -> status = 0;
                }
            } else {
                if (turtle.mouseX > buttonLeftX && turtle.mouseX < buttonRightX && turtle.mouseY > buttonY - buttonHeight / 2 && turtle.mouseY < buttonY + buttonHeight / 2) {
                    buttonp -> status = -1;
                    tt_globals.elementLogicType = TT_ELEMENT_BUTTON;
                    tt_globals.elementLogicIndex = tt_globals.elementLogicTemp;
                } else {
                    buttonp -> status = 0;
                }
            }
        }
        if (buttonp -> status > 1) {
            buttonp -> value = 1;
            // buttonp -> status = 0;
        } else {
            if (buttonp -> status < 1) {
                buttonp -> value = 0;
            }
        }
    } else {
        buttonp -> status = 0;
    }
    if (buttonp -> variable != NULL) {
        *buttonp -> variable = buttonp -> value;
    }
}

void tt_switchUpdate(tt_switch_t *switchp) {
    if (switchp -> variable != NULL) {
        switchp -> value = *switchp -> variable;
    }
    if (switchp -> enabled == TT_ELEMENT_HIDE) {
        return;
    }
    double switchX = switchp -> x;
    double switchY = switchp -> y;
    if (switchp -> align == TT_SWITCH_ALIGN_LEFT) {
        switchX = switchp -> x + switchp -> size * 1.5;
    }
    if (switchp -> align == TT_SWITCH_ALIGN_RIGHT) {
        switchX = switchp -> x - switchp -> size * 1.5;
    }
    double switchClickLeft = switchX;
    double switchClickRight = switchX;
    double switchClickDown = switchY;
    double switchClickUp = switchY;
    if (switchp -> style == TT_SWITCH_STYLE_CLASSIC || switchp -> style == TT_SWITCH_STYLE_SIDESWIPE_LEFT || switchp -> style == TT_SWITCH_STYLE_SIDESWIPE_RIGHT) {
        /* render switch */
        if (switchp -> value) {
            tt_setColor(switchp -> color[TT_COLOR_SLOT_SWITCH_ON]);
        } else {
            tt_setColor(switchp -> color[TT_COLOR_SLOT_SWITCH_OFF]);
        }
        turtlePenSize(switchp -> size * 1.2);
        turtleGoto(switchX - switchp -> size * 0.8, switchY);
        turtlePenDown();
        turtleGoto(switchX + switchp -> size * 0.8, switchY);
        turtlePenUp();
        turtlePenSize(switchp -> size);
        if (switchp -> value) {
            tt_setColor(switchp -> color[TT_COLOR_SLOT_CIRCLE_ON]);
            turtleGoto(switchX + switchp -> size * 0.8, switchY);
        } else {
            tt_setColor(switchp -> color[TT_COLOR_SLOT_CIRCLE_OFF]);
            turtleGoto(switchX - switchp -> size * 0.8, switchY);
        }
        turtlePenDown();
        turtlePenUp();
        /* mouse parameters */
        if (switchp -> style == TT_SWITCH_STYLE_CLASSIC) {
            switchClickLeft = switchX - switchp -> size * 1.35;
            switchClickRight = switchX + switchp -> size * 1.35;
        } else if (switchp -> style == TT_SWITCH_STYLE_SIDESWIPE_LEFT) {
            double textLength = turtleTextGetUnicodeLength(switchp -> label, switchp -> size - 1);
            switchClickLeft = switchX - switchp -> size * 1.35;
            switchClickRight = switchX + switchp -> size * 2.2 + textLength;
        } else if (switchp -> style == TT_SWITCH_STYLE_SIDESWIPE_RIGHT) {
            double textLength = turtleTextGetUnicodeLength(switchp -> label, switchp -> size - 1);
            switchClickLeft = switchX - switchp -> size * 2 - textLength;
            switchClickRight = switchX + switchp -> size * 1.35;
        }
        switchClickDown = switchY - switchp -> size * 0.6;
        switchClickUp = switchY + switchp -> size * 0.6;
        /* render text */
        tt_setColor(switchp -> color[TT_COLOR_SLOT_SWITCH_TEXT]);
        if (switchp -> style == TT_SWITCH_STYLE_CLASSIC) {
            if (switchp -> align == TT_SWITCH_ALIGN_CENTER) {
                turtleTextWriteUnicode(switchp -> label, switchX, switchY + 1.6 * switchp -> size, switchp -> size - 1, 50);
            } else if (switchp -> align == TT_SWITCH_ALIGN_LEFT) {
                turtleTextWriteUnicode(switchp -> label, switchX - switchp -> size * 1.2, switchY + 1.6 * switchp -> size, switchp -> size - 1, 0);
            } else if (switchp -> align == TT_SWITCH_ALIGN_RIGHT) {
                turtleTextWriteUnicode(switchp -> label, switchX + switchp -> size * 1.2, switchY + 1.6 * switchp -> size, switchp -> size - 1, 100);
            }
        } else if (switchp -> style == TT_SWITCH_STYLE_SIDESWIPE_LEFT) {
            if (switchp -> status == 0) {
                tt_setColor(switchp -> color[TT_COLOR_SLOT_SWITCH_TEXT]);
            } else {
                tt_setColor(switchp -> color[TT_COLOR_SLOT_SWITCH_TEXT_HOVER]);
            }
            turtleTextWriteUnicode(switchp -> label, switchX + switchp -> size * 2, switchY, switchp -> size - 1, 0);
        } else if (switchp -> style == TT_SWITCH_STYLE_SIDESWIPE_RIGHT) {
            if (switchp -> status == 0) {
                tt_setColor(switchp -> color[TT_COLOR_SLOT_SWITCH_TEXT]);
            } else {
                tt_setColor(switchp -> color[TT_COLOR_SLOT_SWITCH_TEXT_HOVER]);
            }
            turtleTextWriteUnicode(switchp -> label, switchX - switchp -> size * 2, switchY, switchp -> size - 1, 100);
        }
    } else if (switchp -> style == TT_SWITCH_STYLE_CHECKBOX || switchp -> style == TT_SWITCH_STYLE_XBOX) {
        /* render box */
        tt_setColor(switchp -> color[TT_COLOR_SLOT_SWITCH_ON]);
        turtleRectangle(switchX - switchp -> size / 2, switchY - switchp -> size / 2, switchX + switchp -> size / 2, switchY + switchp -> size / 2);
        tt_setColor(switchp -> color[TT_COLOR_SLOT_SWITCH_OFF]);
        turtlePenSize(switchp -> size / 6);
        turtleGoto(switchX - switchp -> size / 2, switchY - switchp -> size / 2);
        turtlePenDown();
        turtleGoto(switchX + switchp -> size / 2, switchY - switchp -> size / 2);
        turtleGoto(switchX + switchp -> size / 2, switchY + switchp -> size / 2);
        turtleGoto(switchX - switchp -> size / 2, switchY + switchp -> size / 2);
        turtleGoto(switchX - switchp -> size / 2, switchY - switchp -> size / 2);
        turtlePenUp();
        if (switchp -> value) {
            if (switchp -> style == TT_SWITCH_STYLE_CHECKBOX) {
                /* render check */
                tt_setColor(switchp -> color[TT_COLOR_SLOT_CIRCLE_ON]);
                turtleGoto(switchX - switchp -> size / 2.5, switchY);
                turtlePenDown();
                turtleGoto(switchX, switchY - switchp -> size / 2.5);
                turtleGoto(switchX + switchp -> size / 2.5, switchY + switchp -> size / 2.5);
                turtlePenUp();
            } else {
                /* render X */
                tt_setColor(switchp -> color[TT_COLOR_SLOT_CIRCLE_ON]);
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
        double textLength = turtleTextGetUnicodeLength(switchp -> label, switchp -> size - 1);
        switchClickLeft = switchX - switchp -> size * 0.6;
        switchClickRight = switchX + switchp -> size * 1.2 + textLength;
        switchClickDown = switchY - switchp -> size * 0.6;
        switchClickUp = switchY + switchp -> size * 0.6;
        /* render text */
        if (switchp -> status == 0) {
            tt_setColor(switchp -> color[TT_COLOR_SLOT_SWITCH_TEXT]);
        } else {
            tt_setColor(switchp -> color[TT_COLOR_SLOT_SWITCH_TEXT_HOVER]);
        }
        turtleTextWriteUnicode(switchp -> label, switchX + switchp -> size, switchY, switchp -> size - 1, 0);
    }
    /* mouse */
    if (switchp -> enabled == TT_ELEMENT_ENABLED && (tt_globals.elementLogicTypeOld < TT_ELEMENT_SWITCH || (tt_globals.elementLogicTypeOld == TT_ELEMENT_SWITCH && tt_globals.elementLogicIndexOld <= (int32_t) tt_globals.elementLogicTemp))) {
        if (turtleMouseDown()) {
            if (switchp -> status < 0) {
                switchp -> status *= -1;
            }
        } else {
            if (turtle.mouseX > switchClickLeft && turtle.mouseX < switchClickRight && turtle.mouseY > switchClickDown && turtle.mouseY < switchClickUp) {
                switchp -> status = -1;
                tt_globals.elementLogicType = TT_ELEMENT_SWITCH;
                tt_globals.elementLogicIndex = tt_globals.elementLogicTemp;
            } else {
                switchp -> status = 0;
            }
        }
        if (switchp -> status == 1) {
            switchp -> value = !switchp -> value;
            switchp -> status = 2;
        }
    } else {
        switchp -> status = 0;
    }
    if (switchp -> variable != NULL) {
        *switchp -> variable = switchp -> value;
    }
}

/* angle between two coordinates (in degrees) */
double tt_angleBetween(double x1, double y1, double x2, double y2) {
    double output;
    if (y2 == y1) {
        if (x2 >= x1) {
            output = 90;
        } else {
            output = 270;
        }
    } else if (y2 - y1 < 0) {
        output = 180 + atan((x2 - x1) / (y2 - y1)) * 57.2958;
    } else {
        output = atan((x2 - x1) / (y2 - y1)) * 57.2958;
    }
    if (output < 0) {
        output += 360;
    }
    return output;
}

void tt_dialUpdate(tt_dial_t *dialp) {
    if (dialp -> variable != NULL) {
        dialp -> value = *dialp -> variable;
    }
    if (dialp -> enabled == TT_ELEMENT_HIDE) {
        return;
    }
    tt_setColor(dialp -> color[TT_COLOR_SLOT_DIAL_TEXT]);
    turtleTextWriteUnicode(dialp -> label, dialp -> x, dialp -> y + 1.9 * dialp -> size, dialp -> size - 1, 50);
    turtlePenSize(dialp -> size * 2);
    double dialX = dialp -> x;
    double dialY = dialp -> y;
    turtleGoto(dialX, dialY);
    tt_setColor(dialp -> color[TT_COLOR_SLOT_DIAL]);
    turtlePenDown();
    turtlePenUp();
    turtlePenSize(dialp -> size * 2 * 0.8);
    tt_setColor(dialp -> color[TT_COLOR_SLOT_DIAL_INNER]);
    turtlePenDown();
    turtlePenUp();
    tt_setColor(dialp -> color[TT_COLOR_SLOT_DIAL]);
    turtlePenSize(1);
    turtlePenDown();
    double dialAngle;
    if (dialp -> scale == TT_DIAL_SCALE_LOG) {
        dialAngle = pow(361, (dialp -> value - dialp -> range[0]) / (dialp -> range[1] - dialp -> range[0])) - 1;
    } else if (dialp -> scale == TT_DIAL_SCALE_LINEAR) {
        dialAngle = (dialp -> value - dialp -> range[0]) / (dialp -> range[1] - dialp -> range[0]) * 360;
    } else if (dialp -> scale == TT_DIAL_SCALE_EXP) {
        dialAngle = 360 * (log(((dialp -> value - dialp -> range[0]) / (dialp -> range[1] - dialp -> range[0])) * 360 + 1) / log(361));
    }
    turtleGoto(dialX + sin(dialAngle / 57.2958) * dialp -> size, dialY + cos(dialAngle / 57.2958) * dialp -> size);
    turtlePenUp();
    /* mouse */
    if (dialp -> enabled == TT_ELEMENT_ENABLED && (tt_globals.elementLogicTypeOld < TT_ELEMENT_DIAL || (tt_globals.elementLogicTypeOld == TT_ELEMENT_DIAL && tt_globals.elementLogicIndexOld <= (int32_t) tt_globals.elementLogicTemp))) {
        if (turtleMouseDown()) {
            if (dialp -> status[0] < 0) {
                tt_globals.dialAnchorX = dialX;
                tt_globals.dialAnchorY = dialY;
                dialp -> status[0] *= -1;
                dialp -> status[1] = turtle.mouseX - dialX;
            }
        } else if (turtleMouseRight()) {
            if (dialp -> status[0] < 0) {
                dialp -> value = dialp -> defaultValue;
            }
        } else {
            if (turtle.mouseX > dialX - dialp -> size && turtle.mouseX < dialX + dialp -> size && turtle.mouseY > dialY - dialp -> size && turtle.mouseY < dialY + dialp -> size) {
                dialp -> status[0] = -1;
                tt_globals.elementLogicType = TT_ELEMENT_DIAL;
                tt_globals.elementLogicIndex = tt_globals.elementLogicTemp;
            } else {
                dialp -> status[0] = 0;
            }
        }
        if (dialp -> status[0] > 0) {
            dialAngle = tt_angleBetween(tt_globals.dialAnchorX, tt_globals.dialAnchorY, turtle.mouseX, turtle.mouseY);
            if (turtle.mouseY < tt_globals.dialAnchorY) {
                dialp -> status[1] = turtle.mouseX - dialX;
            }
            if ((dialAngle < 0.000000001 || dialAngle > 180) && turtle.mouseY > tt_globals.dialAnchorY && dialp -> status[1] >= 0) {
                dialAngle = 0.000000001;
            }
            if ((dialAngle > 359.99999999 || dialAngle < 180) && turtle.mouseY > tt_globals.dialAnchorY && dialp -> status[1] < 0) {
                dialAngle = 359.99999999;
            }
            if (dialp -> scale == TT_DIAL_SCALE_LOG) {
                dialp -> value = round(dialp -> range[0] + (dialp -> range[1] - dialp -> range[0]) * (log(1 + dialAngle) / log(361)));
            } else if (dialp -> scale == TT_DIAL_SCALE_LINEAR) {
                dialp -> value = round(dialp -> range[0] + ((dialp -> range[1] - dialp -> range[0]) * dialAngle / 360));
            } else if (dialp -> scale == TT_DIAL_SCALE_EXP) {
                dialp -> value = round(dialp -> range[0] + (dialp -> range[1] - dialp -> range[0]) * ((pow(361, dialAngle / 360) - 1) / 360));
            }
        }
    } else {
        dialp -> status[0] = 0;
    }
    tt_setColor(dialp -> color[TT_COLOR_SLOT_DIAL_TEXT]);
    double rounded = round(dialp -> value * dialp -> renderNumberFactor);
    turtleTextWriteStringf(dialX + dialp -> size + 3, dialY, 4, 0, "%.0lf", rounded);
    if (dialp -> variable != NULL) {
        *dialp -> variable = dialp -> value;
    }
}

void tt_sliderUpdate(tt_slider_t *sliderp) {
    if (sliderp -> variable != NULL) {
        sliderp -> value = *sliderp -> variable;
    }
    if (sliderp -> enabled == TT_ELEMENT_HIDE) {
        return;
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
    if (sliderp -> type == TT_SLIDER_TYPE_HORIZONTAL) {
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
    } else if (sliderp -> type == TT_SLIDER_TYPE_VERTICAL) {
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
    tt_setColor(sliderp -> color[TT_COLOR_SLOT_SLIDER_TEXT]);
    turtleTextWriteUnicode(sliderp -> label, sliderp -> x + sliderOffsetXFactor, sliderp -> y + sliderOffsetYFactor, sliderp -> size - 1, sliderAlignFactor);
    turtlePenSize(sliderp -> size * 1.2);
    turtleGoto(sliderXLeft, sliderYLeft);
    tt_setColor(sliderp -> color[TT_COLOR_SLOT_SLIDER_BAR]);
    turtlePenDown();
    turtleGoto(sliderXRight, sliderYRight);
    turtlePenUp();
    turtlePenSize(sliderp -> size);
    tt_setColor(sliderp -> color[TT_COLOR_SLOT_SLIDER_CIRCLE]);
    if (sliderp -> type == TT_SLIDER_TYPE_HORIZONTAL) {
        if (sliderp -> scale == TT_SLIDER_SCALE_LINEAR) {
            turtleGoto(sliderXLeft + (sliderXRight - sliderXLeft) * (sliderp -> value - sliderp -> range[0]) / (sliderp -> range[1] - sliderp -> range[0]), sliderYLeft);
        } else if (sliderp -> scale == TT_SLIDER_SCALE_LOG) {
            turtleGoto(sliderXLeft + pow(sliderXRight - sliderXLeft + 1, (sliderp -> value - sliderp -> range[0]) / (sliderp -> range[1] - sliderp -> range[0])) - 1, sliderYLeft);
        } else if (sliderp -> scale == TT_SLIDER_SCALE_EXP) {
            turtleGoto(sliderXLeft + (sliderXRight - sliderXLeft) * (log(((sliderp -> value - sliderp -> range[0]) / (sliderp -> range[1] - sliderp -> range[0])) * (sliderXRight - sliderXLeft) + 1) / log((sliderXRight - sliderXLeft) + 1)), sliderYLeft);
        }
    } else if (sliderp -> type == TT_SLIDER_TYPE_VERTICAL) {
        if (sliderp -> scale == TT_SLIDER_SCALE_LINEAR) {
            turtleGoto(sliderXLeft, sliderYLeft + (sliderYRight - sliderYLeft) * (sliderp -> value - sliderp -> range[0]) / (sliderp -> range[1] - sliderp -> range[0]));
        } else if (sliderp -> scale == TT_SLIDER_SCALE_LOG) {
            turtleGoto(sliderXLeft, sliderYLeft + pow(sliderYRight - sliderYLeft + 1, (sliderp -> value - sliderp -> range[0]) / (sliderp -> range[1] - sliderp -> range[0])) - 1);
        } else if (sliderp -> scale == TT_SLIDER_SCALE_EXP) {
            turtleGoto(sliderXLeft, sliderYLeft + (sliderYRight - sliderYLeft) * (log(((sliderp -> value - sliderp -> range[0]) / (sliderp -> range[1] - sliderp -> range[0])) * (sliderYRight - sliderYLeft) + 1) / log((sliderYRight - sliderYLeft) + 1)));
        }
    }
    turtlePenDown();
    turtlePenUp();
    /* mouse */
    if (sliderp -> enabled == TT_ELEMENT_ENABLED && (tt_globals.elementLogicTypeOld < TT_ELEMENT_SLIDER || (tt_globals.elementLogicTypeOld == TT_ELEMENT_SLIDER && tt_globals.elementLogicIndexOld <= (int32_t) tt_globals.elementLogicTemp))) {
        if (turtleMouseDown()) {
            if (sliderp -> status < 0) {
                sliderp -> status *= -1;
            }
        } else if (turtleMouseRight()) {
            if (sliderp -> status < 0) {
                sliderp -> value = sliderp -> defaultValue;
            }
        } else {
            if (turtle.mouseX > sliderXLeft - sliderp -> size * 0.6 && turtle.mouseX < sliderXRight + sliderp -> size * 0.6 && turtle.mouseY > sliderYLeft - sliderp -> size * 0.6 && turtle.mouseY < sliderYRight + sliderp -> size * 0.6) {
                sliderp -> status = -1;
                tt_globals.elementLogicType = TT_ELEMENT_SLIDER;
                tt_globals.elementLogicIndex = tt_globals.elementLogicTemp;
            } else {
                sliderp -> status = 0;
            }
        }
        if (sliderp -> status > 0) {
            if (sliderp -> type == TT_SLIDER_TYPE_HORIZONTAL) {
                if (sliderp -> scale == TT_SLIDER_SCALE_LINEAR) {
                    sliderp -> value = round(sliderp -> range[0] + (turtle.mouseX - sliderXLeft) / sliderp -> length * (sliderp -> range[1] - sliderp -> range[0]));
                } else if (sliderp -> scale == TT_SLIDER_SCALE_LOG) {
                    if (turtle.mouseX - sliderXLeft < 0) {
                        sliderp -> value = sliderp -> range[0];
                    } else {
                        sliderp -> value = round(sliderp -> range[0] + (log(1 + turtle.mouseX - sliderXLeft) / log(sliderp -> length + 1)) * (sliderp -> range[1] - sliderp -> range[0]));
                    }
                } else if (sliderp -> scale == TT_SLIDER_SCALE_EXP) {
                    sliderp -> value = round(sliderp -> range[0] + ((pow(sliderp -> length + 1, (turtle.mouseX - sliderXLeft) / sliderp -> length) - 1) / sliderp -> length) * (sliderp -> range[1] - sliderp -> range[0]));
                }
            } else if (sliderp -> type == TT_SLIDER_TYPE_VERTICAL) {
                if (sliderp -> scale == TT_SLIDER_SCALE_LINEAR) {
                    sliderp -> value = round(sliderp -> range[0] + (turtle.mouseY - sliderYLeft) / sliderp -> length * (sliderp -> range[1] - sliderp -> range[0]));
                } else if (sliderp -> scale == TT_SLIDER_SCALE_LOG) {
                    if (turtle.mouseY - sliderYLeft < 0) {
                        sliderp -> value = sliderp -> range[0];
                    } else {
                        sliderp -> value = round(sliderp -> range[0] + (log(1 + turtle.mouseY - sliderYLeft) / log(sliderp -> length + 1)) * (sliderp -> range[1] - sliderp -> range[0]));
                    }
                } else if (sliderp -> scale == TT_SLIDER_SCALE_EXP) {
                    sliderp -> value = round(sliderp -> range[0] + ((pow(sliderp -> length + 1, (turtle.mouseY - sliderYLeft) / sliderp -> length) - 1) / sliderp -> length) * (sliderp -> range[1] - sliderp -> range[0]));
                }
            }
            if (sliderp -> value >= sliderp -> range[1]) {
                sliderp -> value = sliderp -> range[1];
            }
            if (sliderp -> value <= sliderp -> range[0]) {
                sliderp -> value = sliderp -> range[0];
            }
        }
    } else {
        sliderp -> status = 0;
    }
    if (sliderp -> renderNumberFactor != 0) {
        tt_setColor(sliderp -> color[TT_COLOR_SLOT_SLIDER_TEXT]);
        int32_t rounded = (int32_t) round(sliderp -> value * sliderp -> renderNumberFactor);
        turtleTextWriteStringf(sliderp -> x + sliderOffsetXFactorSmall, sliderp -> y + sliderOffsetYFactorSmall, 4, sliderAlignFactor, "%d", rounded);
    }
    if (sliderp -> variable != NULL) {
        *sliderp -> variable = sliderp -> value;
    }
}

void tt_textboxAddKey(tt_textbox_t *textboxp, int32_t key) {
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
        if (textboxp -> blacklist != NULL) {
            for (int32_t i = 0; i < textboxp -> blacklist -> length; i++) {
                if (strcmp((char *) buffer, textboxp -> blacklist -> data[i].s) == 0) {
                    return;
                }
            }
        }
        if (textboxp -> whitelist != NULL) {
            int32_t onWhitelist = 0;
            for (int32_t i = 0; i < textboxp -> whitelist -> length; i++) {
                if (strcmp((char *) buffer, textboxp -> whitelist -> data[i].s) == 0) {
                    onWhitelist = 1;
                    break;
                }
            }
            if (onWhitelist == 0) {
                return;
            }
        }
        strins(textboxp -> text, (char *) buffer, textboxp -> editIndex);
        textboxp -> editIndex += size;
    }
}

void tt_textboxUnicodeCallback(uint32_t codepoint) {
    for (uint32_t i = 0; i < tt_elements.textboxes -> length; i++) {
        tt_textbox_t *textboxp = (tt_textbox_t *) (tt_elements.textboxes -> data[i].p);
        if (textboxp -> status > 1) {
            tt_textboxAddKey(textboxp, codepoint);
            break;
        }
    }
}

void tt_textboxHandleOtherKey(tt_textbox_t *textboxp, int32_t key) {
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

void tt_textboxKeyCallback(int32_t key, int32_t scancode, int32_t action) {
    /* non-printable keys */
    if (action == GLFW_PRESS) {
        for (uint32_t i = 0; i < tt_elements.textboxes -> length; i++) {
            tt_textbox_t *textboxp = (tt_textbox_t *) (tt_elements.textboxes -> data[i].p);
            if (textboxp -> status > 1) {
                textboxp -> lastKey = key;
                textboxp -> keyTimeout = 50;
                tt_textboxHandleOtherKey(textboxp, key);
                break;
            }
        }
    }
}

int32_t tt_textboxCalculateMaximumCharacters(uint32_t *charlist, int32_t textLength, double size, double lengthPixels, int8_t sweepDirection, double *outputLength) {
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

void tt_textboxUpdate(tt_textbox_t *textboxp) {
    if (textboxp -> enabled == TT_ELEMENT_HIDE) {
        return;
    }
    /* handle keys */
    if (textboxp -> keyTimeout > 0) {
        textboxp -> keyTimeout--;
    }
    if (textboxp -> lastKey > 0) {
        if (turtleKeyPressed(textboxp -> lastKey)) {
            if (textboxp -> keyTimeout == 0) {
                textboxp -> keyTimeout = 4;
                tt_textboxHandleOtherKey(textboxp, textboxp -> lastKey);
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
    tt_setColor(textboxp -> color[TT_COLOR_SLOT_TEXTBOX_BOX]);
    turtleRectangle(textboxp -> x, textboxp -> y - textboxp -> size, textboxp -> x + textboxp -> length, textboxp -> y + textboxp -> size);
    if (textboxp -> status <= 0) {
        textboxp -> renderPixelOffset = textboxp -> size / 3;
        textboxp -> renderStartingIndex = 0;
        /* textbox idle */
        if (strlen(textboxp -> text) == 0) {
            /* render label */
            textboxp -> renderNumCharacters = 0;
            tt_setColor(textboxp -> color[TT_COLOR_SLOT_TEXTBOX_PHANTOM_TEXT]);
            turtleTextWriteUnicode(textboxp -> label, textboxp -> x + textboxp -> size / 2, textboxp -> y, textboxp -> size - 1, 0);
        } else {
            /* calculate rendered characters */
            double totalTextLength = turtleTextGetUnicodeLength(textboxp -> text, textboxp -> size - 1);
            if (totalTextLength < textboxp -> length - textboxp -> size / 1.5) {
                textboxp -> renderNumCharacters = strlen(textboxp -> text);
            } else {
                /* not all characters fit in textbox - retract text length */
                uint32_t textConverted[strlen(textboxp -> text) + 1];
                uint32_t characterLength = turtleTextConvertUnicode(textboxp -> text, textConverted);
                double dummy;
                textboxp -> renderNumCharacters = tt_textboxCalculateMaximumCharacters(textConverted, characterLength, textboxp -> size - 1, textboxp -> length - textboxp -> size * 1.2, -1, &dummy);
            }
        }
    } else if (textboxp -> status > 0) {
        /* editing text */
        /* calculate rendered characters */
        double totalTextLength = turtleTextGetUnicodeLength(textboxp -> text, textboxp -> size - 1);
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
                uint32_t characterLength = turtleTextConvertUnicode(textboxp -> text + textboxp -> editIndex, textConverted);
                double dummy;
                textboxp -> renderNumCharacters = tt_textboxCalculateMaximumCharacters(textConverted, characterLength, textboxp -> size - 1, textboxp -> length - textboxp -> size * 1.2, -1, &dummy);
            } else if (textboxp -> editIndex > textboxp -> renderStartingIndex + textboxp -> renderNumCharacters || (strlen(textboxp -> text) < textboxp -> renderStartingIndex + textboxp -> renderNumCharacters && strlen(textboxp -> text) == textboxp -> editIndex)) {
                /* set editIndex at the right side of box */
                char tempHold;
                tempHold = textboxp -> text[textboxp -> editIndex];
                textboxp -> text[textboxp -> editIndex] = '\0';
                uint32_t textConverted[strlen(textboxp -> text) + 1];
                uint32_t characterLength = turtleTextConvertUnicode(textboxp -> text, textConverted);
                double textPixelLength;
                textboxp -> renderStartingIndex = strlen(textboxp -> text) + tt_textboxCalculateMaximumCharacters(textConverted, characterLength, textboxp -> size - 1, textboxp -> length - textboxp -> size * 1.2, 1, &textPixelLength);
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
    tt_setColor(textboxp -> color[TT_COLOR_SLOT_TEXTBOX_TEXT]);
    turtleTextWriteUnicode(textboxp -> text + textboxp -> renderStartingIndex, textboxp -> x + textboxp -> renderPixelOffset, textboxp -> y, textboxp -> size - 1, 0);
    textboxp -> text[textboxp -> renderStartingIndex + textboxp -> renderNumCharacters] = tempHold;
    tt_setColor(textboxp -> color[TT_COLOR_SLOT_TEXTBOX_BOX]);
    turtleRectangle(textboxp -> x, textboxp -> y - textboxp -> size, textboxp -> x + textboxp -> size / 4, textboxp -> y + textboxp -> size);
    turtleRectangle(textboxp -> x + textboxp -> length, textboxp -> y - textboxp -> size, textboxp -> x + textboxp -> length - textboxp -> size / 4, textboxp -> y + textboxp -> size);
    if (textboxp -> status > 0 && textboxp -> status < 66) {
        char tempHold = textboxp -> text[textboxp -> editIndex];
        textboxp -> text[textboxp -> editIndex] = '\0';
        double textLength = turtleTextGetUnicodeLength(textboxp -> text + textboxp -> renderStartingIndex, textboxp -> size - 1);
        textboxp -> text[textboxp -> editIndex] = tempHold;
        tt_setColor(textboxp -> color[TT_COLOR_SLOT_TEXTBOX_LINE]);
        turtleRectangle(textboxp -> x + textboxp -> renderPixelOffset + textLength, textboxp -> y - textboxp -> size * 0.8, textboxp -> x + textboxp -> renderPixelOffset + textLength + 1, textboxp -> y + textboxp -> size * 0.8);
    }
    /* mouse */
    if (textboxp -> enabled == TT_ELEMENT_ENABLED && (tt_globals.elementLogicTypeOld < TT_ELEMENT_TEXTBOX || (tt_globals.elementLogicTypeOld == TT_ELEMENT_TEXTBOX && tt_globals.elementLogicIndexOld <= (int32_t) tt_globals.elementLogicTemp) || textboxp -> status > 1)) {
        if (turtle.mouseX > textboxp -> x && turtle.mouseX < textboxp -> x + textboxp -> length && turtle.mouseY > textboxp -> y - textboxp -> size && turtle.mouseY < textboxp -> y + textboxp -> size) {
            textboxp -> mouseOver = 1;
        } else {
            textboxp -> mouseOver = 0;
        }
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
                tt_globals.elementLogicType = TT_ELEMENT_HIGHEST;
                tt_globals.elementLogicIndex = TT_ELEMENT_TEXTBOX; // subverting expectations
            } else if (textboxp -> status < 2) {
                if (textboxp -> mouseOver) {
                    textboxp -> status = -1;
                    tt_globals.elementLogicType = TT_ELEMENT_TEXTBOX;
                    tt_globals.elementLogicIndex = tt_globals.elementLogicTemp;
                } else {
                    textboxp -> status = 0;
                }
            }
        }
    } else {
        textboxp -> status = 0;
        textboxp -> mouseOver = 0;
    }
}

void tt_dropdownUpdate(tt_dropdown_t *dropdownp) {
    if (dropdownp -> variable != NULL) {
        dropdownp -> index = *dropdownp -> variable;
        dropdownp -> value = *dropdownp -> variable;
    }
    if (dropdownp -> enabled == TT_ELEMENT_HIDE) {
        return;
    }
    /* render dropdown default position */
    double dropdownX = dropdownp -> x;
    double dropdownY = dropdownp -> y;
    double xfactor = turtleTextGetUnicodeLength(dropdownp -> options -> data[dropdownp -> index].s, dropdownp -> size - 1);
    double itemHeight = (dropdownp -> size * 1.8);
    double dropdownXFactor[2];
    double dropdownMaxXFactor[2];
    double dropdownAlignFactor;
    tt_setColor(dropdownp -> color[TT_COLOR_SLOT_DROPDOWN_TEXT]);
    if (dropdownp -> align == TT_DROPDOWN_ALIGN_LEFT) {
        dropdownXFactor[0] = dropdownX;
        dropdownXFactor[1] = dropdownX + xfactor + dropdownp -> size * 1.2;
        dropdownMaxXFactor[0] = dropdownX;
        dropdownMaxXFactor[1] = dropdownX + dropdownp -> maxXfactor + dropdownp -> size * 1.1;
        if (dropdownXFactor[1] + dropdownp -> size * 1.2 > dropdownMaxXFactor[1]) {
            dropdownMaxXFactor[1] = dropdownXFactor[1] + dropdownp -> size * 1.2;
        }
        dropdownAlignFactor = 0;
        turtleTextWriteUnicode(dropdownp -> label, dropdownX + dropdownp -> size / 5, dropdownY + 2 * dropdownp -> size, dropdownp -> size - 1, dropdownAlignFactor);
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
        turtleTextWriteUnicode(dropdownp -> label, dropdownX, dropdownY + 2 * dropdownp -> size, dropdownp -> size - 1, dropdownAlignFactor);
    } else if (dropdownp -> align == TT_DROPDOWN_ALIGN_RIGHT) {
        dropdownXFactor[0] = dropdownX - xfactor - dropdownp -> size * 2;
        dropdownXFactor[1] = dropdownX - dropdownp -> size;
        dropdownMaxXFactor[0] = dropdownX - dropdownp -> maxXfactor - dropdownp -> size * 2.2;
        dropdownMaxXFactor[1] = dropdownX;
        dropdownAlignFactor = 100;
        turtleTextWriteUnicode(dropdownp -> label, dropdownX - dropdownp -> size / 5, dropdownY + 2 * dropdownp -> size, dropdownp -> size - 1, dropdownAlignFactor);
    }
    int32_t dropdownDirection = dropdownp -> direction;
    if (dropdownDirection == TT_DROPDOWN_DIRECTION_AUTO) {
        if (dropdownY - dropdownp -> size * 0.9 - (dropdownp -> options -> length - 1) * itemHeight <= dropdownp -> autoLowerBound) {
            dropdownDirection = TT_DROPDOWN_DIRECTION_UP;
        } else {
            dropdownDirection = TT_DROPDOWN_DIRECTION_DOWN;
        }
    }
    if (dropdownDirection == TT_DROPDOWN_DIRECTION_AUTO_PREFER_UP) {
        if (dropdownY + dropdownp -> size * 0.9 + (dropdownp -> options -> length - 1) * itemHeight >= dropdownp -> autoUpperBound) {
            dropdownDirection = TT_DROPDOWN_DIRECTION_DOWN;
        } else {
            dropdownDirection = TT_DROPDOWN_DIRECTION_UP;
        }
    }
    int32_t directionRender = 1;
    if (dropdownDirection == TT_DROPDOWN_DIRECTION_UP) {
        directionRender = -1;
    }
    if (dropdownp -> status == -1) {
        tt_setColor(dropdownp -> color[TT_COLOR_SLOT_DROPDOWN_SELECT]);
        turtleRectangle(dropdownXFactor[0], dropdownY - dropdownp -> size * 0.9, dropdownXFactor[1] + dropdownp -> size, dropdownY + dropdownp -> size * 0.9);
    } else if (dropdownp -> status >= 1) {
        tt_setColor(dropdownp -> color[TT_COLOR_SLOT_DROPDOWN_BASE]);
        if (dropdownDirection == TT_DROPDOWN_DIRECTION_UP) {
            turtleRectangle(dropdownMaxXFactor[0], dropdownY + dropdownp -> size * 0.9 + (dropdownp -> options -> length - 1) * itemHeight, dropdownMaxXFactor[1], dropdownY - dropdownp -> size * 0.9);
        } else {
            turtleRectangle(dropdownMaxXFactor[0], dropdownY - dropdownp -> size * 0.9 - (dropdownp -> options -> length - 1) * itemHeight, dropdownMaxXFactor[1], dropdownY + dropdownp -> size * 0.9);
        }
    } else {
        tt_setColor(dropdownp -> color[TT_COLOR_SLOT_DROPDOWN_BASE]);
        turtleRectangle(dropdownXFactor[0], dropdownY - dropdownp -> size * 0.9, dropdownXFactor[1] + dropdownp -> size, dropdownY + dropdownp -> size * 0.9);
    }
    /* mouse */
    if (dropdownp -> enabled == TT_ELEMENT_ENABLED && (tt_globals.elementLogicTypeOld < TT_ELEMENT_DROPDOWN || (tt_globals.elementLogicTypeOld == TT_ELEMENT_DROPDOWN && tt_globals.elementLogicIndexOld <= (int32_t) tt_globals.elementLogicTemp) || dropdownp -> status > 0)) {
        if (dropdownp -> enabled == TT_ELEMENT_ENABLED && tt_ribbon.mainselect[2] == -1) {
            if (turtle.mouseX > dropdownXFactor[0] && turtle.mouseX < dropdownXFactor[1] + dropdownp -> size && turtle.mouseY >= dropdownY - dropdownp -> size * 0.9 && turtle.mouseY < dropdownY + dropdownp -> size * 0.9) {
                if (!turtleMouseDown() && dropdownp -> status == 0) {
                    dropdownp -> status = -1;
                    tt_globals.elementLogicType = TT_ELEMENT_DROPDOWN;
                    tt_globals.elementLogicIndex = tt_globals.elementLogicTemp;
                }
            } else {
                if (dropdownp -> status == -1) {
                    dropdownp -> status = 0;
                }
            }
            if (dropdownp -> status == -1) {
                if (turtleMouseDown()) {
                    dropdownp -> status = 1;
                    tt_globals.elementLogicType = TT_ELEMENT_HIGHEST;
                    tt_globals.elementLogicIndex = TT_ELEMENT_DROPDOWN; // subverting expectations
                }
            }
            if (dropdownp -> status == 1) {
                if (!turtleMouseDown()) {
                    if (turtle.mouseX > dropdownMaxXFactor[0] && turtle.mouseX < dropdownMaxXFactor[1] && ((directionRender == 1 && turtle.mouseY > dropdownY - dropdownp -> size * 0.9 - (dropdownp -> options -> length - 1) * itemHeight && turtle.mouseY <= dropdownY + dropdownp -> size * 0.9 - itemHeight) || (directionRender == -1 && turtle.mouseY < dropdownY + dropdownp -> size * 0.9 + (dropdownp -> options -> length - 1) * itemHeight && turtle.mouseY >= dropdownY - dropdownp -> size * 0.9 + itemHeight))) {
                        uint32_t selected = round((dropdownY - turtle.mouseY) / itemHeight);
                        if (directionRender == -1) {
                            selected = round((turtle.mouseY - dropdownY) / itemHeight);
                        }
                        if (selected != 0) {
                            if (dropdownp -> index >= selected) {
                                dropdownp -> index = selected - 1;
                                dropdownp -> value = selected - 1;
                            } else {
                                dropdownp -> index = selected;
                                dropdownp -> value = selected;
                            }
                        }
                        dropdownp -> status = -2;
                    } else {
                        dropdownp -> status = 2;
                    }
                }
            }
            if (dropdownp -> status == -2) {
                if (!turtleMouseDown()) {
                    dropdownp -> status = 0;
                }
            }
            if (dropdownp -> status == 2 || dropdownp -> status == 1) {
                tt_globals.elementLogicType = TT_ELEMENT_DROPDOWN;
                tt_globals.elementLogicIndex = tt_globals.elementLogicTemp;
                if (turtle.mouseX > dropdownMaxXFactor[0] && turtle.mouseX < dropdownMaxXFactor[1] && ((directionRender == 1 && turtle.mouseY > dropdownY - dropdownp -> size * 0.9 - (dropdownp -> options -> length - 1) * itemHeight && turtle.mouseY <= dropdownY + dropdownp -> size * 0.9) || (directionRender == -1 && turtle.mouseY < dropdownY + dropdownp -> size * 0.9 + (dropdownp -> options -> length - 1) * itemHeight && turtle.mouseY >= dropdownY - dropdownp -> size * 0.9))) {
                    uint32_t selected = round((dropdownY - turtle.mouseY) / itemHeight);
                    if (directionRender == -1) {
                        selected = dropdownp -> options -> length - round((turtle.mouseY - dropdownY) / itemHeight);
                    }
                    tt_setColor(dropdownp -> color[TT_COLOR_SLOT_DROPDOWN_HOVER]);
                    turtleRectangle(dropdownMaxXFactor[0], dropdownY - dropdownp -> size * 0.9 - (directionRender - 1) / 2.0 * dropdownp -> options -> length * itemHeight - selected * itemHeight, dropdownMaxXFactor[1], dropdownY + dropdownp -> size * 0.9 - (directionRender - 1) / 2.0 * dropdownp -> options -> length * itemHeight - selected * itemHeight);
                    if (turtleMouseDown() && dropdownp -> status == 2) {
                        if (selected >= dropdownp -> options -> length) {
                            selected = 0;
                        }
                        if (selected != 0) {
                            if (dropdownp -> index >= selected) {
                                dropdownp -> index = selected - 1;
                                dropdownp -> value = selected - 1;
                            } else {
                                dropdownp -> index = selected;
                                dropdownp -> value = selected;
                            }
                        }
                        dropdownp -> status = -2;
                    }
                } else {
                    if (turtleMouseDown() && dropdownp -> status == 2) {
                        dropdownp -> status = 0;
                    }
                }
                tt_setColor(dropdownp -> color[TT_COLOR_SLOT_DROPDOWN_TEXT_HOVER]);
                int32_t renderIndex = 1;
                for (uint32_t i = 0; i < dropdownp -> options -> length; i++) {
                    if (i != dropdownp -> index) {
                        if (dropdownp -> align == TT_DROPDOWN_ALIGN_LEFT) {
                            turtleTextWriteUnicode(dropdownp -> options -> data[i].s, dropdownMaxXFactor[0] + dropdownp -> size / 2, dropdownY - (directionRender - 1) / 2.0 * dropdownp -> options -> length * itemHeight - renderIndex * itemHeight, dropdownp -> size - 1, dropdownAlignFactor);
                        } else if (dropdownp -> align == TT_DROPDOWN_ALIGN_CENTER) {
                            turtleTextWriteUnicode(dropdownp -> options -> data[i].s, (dropdownMaxXFactor[0] + dropdownMaxXFactor[1]) / 2, dropdownY - (directionRender - 1) / 2.0 * dropdownp -> options -> length * itemHeight - renderIndex * itemHeight, dropdownp -> size - 1, dropdownAlignFactor);
                        } else if (dropdownp -> align == TT_DROPDOWN_ALIGN_RIGHT) {
                            turtleTextWriteUnicode(dropdownp -> options -> data[i].s, dropdownMaxXFactor[1] - dropdownp -> size * 1.58, dropdownY - (directionRender - 1) / 2.0 * dropdownp -> options -> length * itemHeight - renderIndex * itemHeight, dropdownp -> size - 1, dropdownAlignFactor);
                        }
                        renderIndex++;
                    }
                }
            }
        }
    } else {
        dropdownp -> status = 0;
    }
    tt_setColor(dropdownp -> color[TT_COLOR_SLOT_DROPDOWN_TEXT_HOVER]);
    if (dropdownp -> align == TT_DROPDOWN_ALIGN_LEFT) {
        turtleTextWriteUnicode(dropdownp -> options -> data[dropdownp -> index].s, dropdownXFactor[0] + dropdownp -> size / 2, dropdownY, dropdownp -> size - 1, dropdownAlignFactor);
    } else if (dropdownp -> align == TT_DROPDOWN_ALIGN_CENTER) {
        turtleTextWriteUnicode(dropdownp -> options -> data[dropdownp -> index].s, (dropdownXFactor[0] + dropdownXFactor[1]) / 2, dropdownY, dropdownp -> size - 1, dropdownAlignFactor);
    } else if (dropdownp -> align == TT_DROPDOWN_ALIGN_RIGHT) {
        turtleTextWriteUnicode(dropdownp -> options -> data[dropdownp -> index].s, dropdownXFactor[1] - dropdownp -> size * 0.55, dropdownY, dropdownp -> size - 1, dropdownAlignFactor);
    }
    tt_setColor(dropdownp -> color[TT_COLOR_SLOT_DROPDOWN_TRIANGLE]);
    if (dropdownp -> status >= 1) {
        turtleTriangle(dropdownXFactor[1] + dropdownp -> size * 0.4, dropdownY + dropdownp -> size * 0.4, dropdownXFactor[1] + dropdownp -> size * 0.4, dropdownY - dropdownp -> size * 0.4, dropdownXFactor[1] - dropdownp -> size * 0.2, dropdownY);
    } else {
        turtleTriangle(dropdownXFactor[1] + dropdownp -> size * 0.6, dropdownY + dropdownp -> size * 0.3, dropdownXFactor[1] - dropdownp -> size * 0.2, dropdownY + dropdownp -> size * 0.3, dropdownXFactor[1] + dropdownp -> size * 0.2, dropdownY - dropdownp -> size * 0.3);
    }
    if (dropdownp -> variable != NULL) {
        *dropdownp -> variable = dropdownp -> index;
    }
}

/*
scrollbar range of motion (coordinates):
scrollbar.length * (1 - scrollbar.barPercentage / 100)
tip: try to match the ratio of visible content to the scrollbar's barPercentage - if half of the content can be shown on one screen then make the barPercentage 50
*/
void tt_scrollbarUpdate(tt_scrollbar_t *scrollbarp) {
    if (scrollbarp -> variable != NULL) {
        scrollbarp -> value = *scrollbarp -> variable;
    }
    if (scrollbarp -> enabled == TT_ELEMENT_HIDE) {
        return;
    }
    if (scrollbarp -> type == TT_SCROLLBAR_TYPE_HORIZONTAL) {
        double scrollbarLeft = scrollbarp -> x - scrollbarp -> length / 2;
        double scrollbarRight = scrollbarp -> x + scrollbarp -> length / 2;
        double dragLeft = scrollbarLeft + scrollbarp -> value / 100 * (scrollbarp -> length * (1 - scrollbarp -> barPercentage / 100));
        double dragRight = dragLeft + (scrollbarp -> length * scrollbarp -> barPercentage / 100);
        turtlePenSize(scrollbarp -> size * 1);
        tt_setColor(scrollbarp -> color[TT_COLOR_SLOT_SCROLLBAR_BASE]);
        turtleGoto(scrollbarLeft, scrollbarp -> y);
        turtlePenDown();
        turtleGoto(scrollbarRight, scrollbarp -> y);
        turtlePenUp();
        turtlePenSize(scrollbarp -> size * 0.8);
        if (scrollbarp -> status == -1 && turtle.mouseX > dragLeft - scrollbarp -> size * 0.4 && turtle.mouseX < dragRight + scrollbarp -> size * 0.4) {
            tt_setColor(scrollbarp -> color[TT_COLOR_SLOT_SCROLLBAR_HOVER]);
        } else if (scrollbarp -> status > 0) {
            tt_setColor(scrollbarp -> color[TT_COLOR_SLOT_SCROLLBAR_CLICKED]);
        } else {
            tt_setColor(scrollbarp -> color[TT_COLOR_SLOT_SCROLLBAR_BAR]);
        }
        turtleGoto(dragLeft, scrollbarp -> y);
        turtlePenDown();
        turtleGoto(dragRight, scrollbarp -> y);
        turtlePenUp();
        if (scrollbarp -> enabled == TT_ELEMENT_ENABLED && (tt_globals.elementLogicTypeOld < TT_ELEMENT_SCROLLBAR || (tt_globals.elementLogicTypeOld == TT_ELEMENT_SCROLLBAR && tt_globals.elementLogicIndexOld <= (int32_t) tt_globals.elementLogicTemp) || scrollbarp -> status > 0)) {
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
                    tt_globals.elementLogicType = TT_ELEMENT_HIGHEST;
                    tt_globals.elementLogicIndex = TT_ELEMENT_SCROLLBAR; // subverting expectations
                }
            } else {
                if (turtle.mouseY > scrollbarp -> y - scrollbarp -> size * 0.5 && turtle.mouseY < scrollbarp -> y + scrollbarp -> size * 0.5 && turtle.mouseX < scrollbarRight && turtle.mouseX > scrollbarLeft) {
                    scrollbarp -> status = -1;
                    tt_globals.elementLogicType = TT_ELEMENT_SCROLLBAR;
                    tt_globals.elementLogicIndex = tt_globals.elementLogicTemp;
                } else {
                    scrollbarp -> status = 0;
                }
            }
            if (scrollbarp -> status > 0) {
                scrollbarp -> value = (turtle.mouseX - scrollbarLeft - tt_globals.barAnchor) / (scrollbarp -> length * (1 - scrollbarp -> barPercentage / 100)) * 100;
                if (scrollbarp -> value < 0) {
                    scrollbarp -> value = 0;
                }
                if (scrollbarp -> value > 100) {
                    scrollbarp -> value = 100;
                }
            }
        } else {
            scrollbarp -> status = 0;
        }
    } else if (scrollbarp -> type == TT_SCROLLBAR_TYPE_VERTICAL) {
        double scrollbarTop = scrollbarp -> y + scrollbarp -> length / 2;
        double scrollbarBottom = scrollbarp -> y - scrollbarp -> length / 2;
        double dragTop = scrollbarTop - scrollbarp -> value / 100 * (scrollbarp -> length * (1 - scrollbarp -> barPercentage / 100));
        double dragBottom = dragTop - (scrollbarp -> length * scrollbarp -> barPercentage / 100);
        turtlePenSize(scrollbarp -> size * 1);
        tt_setColor(scrollbarp -> color[TT_COLOR_SLOT_SCROLLBAR_BASE]);
        turtleGoto(scrollbarp -> x, scrollbarTop);
        turtlePenDown();
        turtleGoto(scrollbarp -> x, scrollbarBottom);
        turtlePenUp();
        turtlePenSize(scrollbarp -> size * 0.8);
        if (scrollbarp -> status == -1 && turtle.mouseY > dragBottom - scrollbarp -> size * 0.4 && turtle.mouseY < dragTop + scrollbarp -> size * 0.4) {
            tt_setColor(scrollbarp -> color[TT_COLOR_SLOT_SCROLLBAR_HOVER]);
        } else if (scrollbarp -> status > 0) {
            tt_setColor(scrollbarp -> color[TT_COLOR_SLOT_SCROLLBAR_CLICKED]);
        } else {
            tt_setColor(scrollbarp -> color[TT_COLOR_SLOT_SCROLLBAR_BAR]);
        }
        turtleGoto(scrollbarp -> x, dragTop);
        turtlePenDown();
        turtleGoto(scrollbarp -> x, dragBottom);
        turtlePenUp();
        /* mouse */
        if (scrollbarp -> enabled == TT_ELEMENT_ENABLED && (tt_globals.elementLogicTypeOld < TT_ELEMENT_SCROLLBAR || (tt_globals.elementLogicTypeOld == TT_ELEMENT_SCROLLBAR && tt_globals.elementLogicIndexOld <= (int32_t) tt_globals.elementLogicTemp) || scrollbarp -> status > 0)) {
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
                    tt_globals.elementLogicType = TT_ELEMENT_HIGHEST;
                    tt_globals.elementLogicIndex = TT_ELEMENT_SCROLLBAR; // subverting expectations
                }
            } else {
                if (turtle.mouseX > scrollbarp -> x - scrollbarp -> size * 0.5 && turtle.mouseX < scrollbarp -> x + scrollbarp -> size * 0.5 && turtle.mouseY > scrollbarBottom && turtle.mouseY < scrollbarTop) {
                    scrollbarp -> status = -1;
                    tt_globals.elementLogicType = TT_ELEMENT_SCROLLBAR;
                    tt_globals.elementLogicIndex = tt_globals.elementLogicTemp;
                } else {
                    scrollbarp -> status = 0;
                }
            }
            if (scrollbarp -> status > 0) {
                scrollbarp -> value = (scrollbarTop - turtle.mouseY - tt_globals.barAnchor) / (scrollbarp -> length * (1 - scrollbarp -> barPercentage / 100)) * 100;
                if (scrollbarp -> value < 0) {
                    scrollbarp -> value = 0;
                }
                if (scrollbarp -> value > 100) {
                    scrollbarp -> value = 100;
                }
            }
        } else {
            scrollbarp -> status = 0;
        }
    }
    if (scrollbarp -> variable != NULL) {
        *scrollbarp -> variable = scrollbarp -> value;
    }
}

void tt_contextUpdate(tt_context_t *contextp) {
    if (contextp -> variable != NULL) {
        contextp -> index = *contextp -> variable;
        contextp -> value = *contextp -> variable;
    }
    if (contextp -> enabled == TT_ELEMENT_HIDE) {
        return;
    }
    double itemHeight = (contextp -> size * 1.8);
    double contextTextX = contextp -> x;
    double contextTextY = contextp -> y - itemHeight / 2 - 2;
    /* determine direction */
    if (contextp -> direction == TT_CONTEXT_DIRECTION_AUTO) {
        if (contextTextX + contextp -> maxXfactor + contextp -> size / 1.25 >= contextp -> autoRightBound) {
            contextTextX = contextp -> x - contextp -> maxXfactor - contextp -> size / 1.25;
        }
        if (contextTextY - contextp -> size * 0.9 - (contextp -> options -> length - 1) * itemHeight - 2 <= contextp -> autoLowerBound) {
            contextTextY = contextp -> y + contextp -> size * 0.9 + (contextp -> options -> length - 1) * itemHeight + 2;
        }
    } else {
        if (contextp -> direction == TT_CONTEXT_DIRECTION_UP_LEFT || contextp -> direction == TT_CONTEXT_DIRECTION_DOWN_LEFT) {
            contextTextX = contextp -> x - contextp -> maxXfactor - contextp -> size / 1.25;
        }
        if (contextp -> direction == TT_CONTEXT_DIRECTION_UP_LEFT || contextp -> direction == TT_CONTEXT_DIRECTION_UP_RIGHT) {
            contextTextY = contextp -> y + contextp -> size * 0.9 + (contextp -> options -> length - 1) * itemHeight + 2;
        }
    }
    tt_setColor(contextp -> color[TT_COLOR_SLOT_CONTEXT_BASE]);
    turtleRectangle(contextTextX, contextTextY - contextp -> size * 0.9 - (contextp -> options -> length - 1) * itemHeight - 2, contextTextX + contextp -> maxXfactor + contextp -> size / 1.25, contextTextY + contextp -> size * 0.9 + 2);
    tt_setColor(contextp -> color[TT_COLOR_SLOT_CONTEXT_TEXT]);
    contextp -> index = -1;
    contextp -> value = -1;
    for (int32_t i = 0; i < contextp -> options -> length; i++) {
        if (turtle.mouseX > contextTextX && turtle.mouseX < contextTextX + contextp -> maxXfactor + contextp -> size / 1.25 && turtle.mouseY >= contextTextY - i * itemHeight - contextp -> size * 0.9 && turtle.mouseY < contextTextY - i * itemHeight + contextp -> size * 0.9) {
            tt_setColor(contextp -> color[TT_COLOR_SLOT_CONTEXT_SELECT]);
            turtleRectangle(contextTextX, contextTextY - i * itemHeight - contextp -> size * 0.9, contextTextX + contextp -> maxXfactor + contextp -> size / 1.25, contextTextY - i * itemHeight + contextp -> size * 0.9);
            tt_setColor(contextp -> color[TT_COLOR_SLOT_CONTEXT_TEXT]);
            contextp -> index = i;
            contextp -> value = i;
        }
        turtleTextWriteUnicode(contextp -> options -> data[i].s, contextTextX + contextp -> size / 2.5, contextTextY - i * itemHeight, contextp -> size - 1, 0);
    }
    if (contextp -> enabled == TT_ELEMENT_ENABLED && (tt_globals.elementLogicTypeOld < TT_ELEMENT_CONTEXT || (tt_globals.elementLogicTypeOld == TT_ELEMENT_CONTEXT && tt_globals.elementLogicIndexOld <= (int32_t) tt_globals.elementLogicTemp))) {
        tt_globals.elementLogicType = TT_ELEMENT_CONTEXT;
        tt_globals.elementLogicIndex = tt_globals.elementLogicTemp;
        if (turtleMouseDown()) {
            contextp -> enabled = TT_ELEMENT_HIDE;
        }
    }
    if (contextp -> variable != NULL) {
        *contextp -> variable = contextp -> index;
    }
}

void turtleToolsUpdate() {
    turtleToolsUpdateUI();
    char shapeSave = turtle.penshape;
    turtlePenShape("circle");
    if (tt_enabled.ribbonEnabled) {
        tt_ribbonUpdate();
    }
    if (tt_enabled.popupEnabled) {
        tt_popupUpdate();
    }
    turtle.penshape = shapeSave;
    tt_globals.elementLogicTypeOld = tt_globals.elementLogicType;
    tt_globals.elementLogicIndexOld = tt_globals.elementLogicIndex;
}

void turtleToolsUpdateUI() {
    tt_globals.elementLogicType = TT_ELEMENT_NONE;
    tt_globals.elementLogicIndex = -1;
    tt_globals.elementLogicTemp = -1;
    char shapeSave = turtle.penshape;
    turtlePenShape("circle");
    if (tt_enabled.buttonEnabled) {
        tt_globals.elementLogicTemp = 0;
        for (uint32_t i = 0; i < tt_elements.buttons -> length; i++) {
            if (((tt_button_t *) (tt_elements.buttons -> data[i].p)) -> ignored == TT_ELEMENT_IGNORED) {
                continue;
            }
            tt_buttonUpdate((tt_button_t *) (tt_elements.buttons -> data[i].p));
            tt_globals.elementLogicTemp++;
        }
    }
    if (tt_enabled.switchEnabled) {
        tt_globals.elementLogicTemp = 0;
        for (uint32_t i = 0; i < tt_elements.switches -> length; i++) {
            if (((tt_switch_t *) (tt_elements.switches -> data[i].p)) -> ignored == TT_ELEMENT_IGNORED) {
                continue;
            }
            tt_switchUpdate((tt_switch_t *) (tt_elements.switches -> data[i].p));
            tt_globals.elementLogicTemp++;
        }
    }
    if (tt_enabled.dialEnabled) {
        tt_globals.elementLogicTemp = 0;
        for (uint32_t i = 0; i < tt_elements.dials -> length; i++) {
            if (((tt_dial_t *) (tt_elements.dials -> data[i].p)) -> ignored == TT_ELEMENT_IGNORED) {
                continue;
            }
            tt_dialUpdate((tt_dial_t *) (tt_elements.dials -> data[i].p));
            tt_globals.elementLogicTemp++;
        }
    }
    if (tt_enabled.sliderEnabled) {
        tt_globals.elementLogicTemp = 0;
        for (uint32_t i = 0; i < tt_elements.sliders -> length; i++) {
            if (((tt_slider_t *) (tt_elements.sliders -> data[i].p)) -> ignored == TT_ELEMENT_IGNORED) {
                continue;
            }
            tt_sliderUpdate((tt_slider_t *) (tt_elements.sliders -> data[i].p));
            tt_globals.elementLogicTemp++;
        }
    }
    if (tt_enabled.textboxEnabled) {
        tt_globals.elementLogicTemp = 0;
        for (uint32_t i = 0; i < tt_elements.textboxes -> length; i++) {
            if (((tt_textbox_t *) (tt_elements.textboxes -> data[i].p)) -> ignored == TT_ELEMENT_IGNORED) {
                continue;
            }
            tt_textboxUpdate((tt_textbox_t *) (tt_elements.textboxes -> data[i].p));
            tt_globals.elementLogicTemp++;
        }
    }
    if (tt_enabled.dropdownEnabled) {
        tt_globals.elementLogicTemp = 0;
        for (uint32_t i = 0; i < tt_elements.dropdowns -> length; i++) {
            if (((tt_dropdown_t *) (tt_elements.dropdowns -> data[i].p)) -> ignored == TT_ELEMENT_IGNORED) {
                continue;
            }
            tt_dropdownUpdate((tt_dropdown_t *) (tt_elements.dropdowns -> data[i].p));
            tt_globals.elementLogicTemp++;
        }
    }
    if (tt_enabled.scrollbarEnabled) {
        tt_globals.elementLogicTemp = 0;
        for (uint32_t i = 0; i < tt_elements.scrollbars -> length; i++) {
            if (((tt_scrollbar_t *) (tt_elements.scrollbars -> data[i].p)) -> ignored == TT_ELEMENT_IGNORED) {
                continue;
            }
            tt_scrollbarUpdate((tt_scrollbar_t *) (tt_elements.scrollbars -> data[i].p));
            tt_globals.elementLogicTemp++;
        }
    }
    if (tt_enabled.contextEnabled) {
        tt_globals.elementLogicTemp = 0;
        for (uint32_t i = 0; i < tt_elements.contexts -> length; i++) {
            if (((tt_context_t *) (tt_elements.contexts -> data[i].p)) -> ignored == TT_ELEMENT_IGNORED) {
                continue;
            }
            tt_contextUpdate((tt_context_t *) (tt_elements.contexts -> data[i].p));
            tt_globals.elementLogicTemp++;
        }
    }
    turtle.penshape = shapeSave;
    tt_globals.elementLogicTypeOld = tt_globals.elementLogicType;
    tt_globals.elementLogicIndexOld = tt_globals.elementLogicIndex;
}

void turtleToolsUpdateRibbonPopup() {
    char shapeSave = turtle.penshape;
    turtlePenShape("circle");
    if (tt_enabled.ribbonEnabled) {
        tt_ribbonUpdate();
    }
    if (tt_enabled.popupEnabled) {
        tt_popupUpdate();
    }
    turtle.penshape = shapeSave;
    tt_globals.elementLogicTypeOld = tt_globals.elementLogicType;
    tt_globals.elementLogicIndexOld = tt_globals.elementLogicIndex;
}
