/*
████████╗██╗   ██╗██████╗ ████████╗██╗     ███████╗████████╗ ██████╗  ██████╗ ██╗     ███████╗    ██████╗
╚══██╔══╝██║   ██║██╔══██╗╚══██╔══╝██║     ██╔════╝╚══██╔══╝██╔═══██╗██╔═══██╗██║     ██╔════╝   ██╔════╝
   ██║   ██║   ██║██████╔╝   ██║   ██║     █████╗     ██║   ██║   ██║██║   ██║██║     ███████╗   ██║     
   ██║   ██║   ██║██╔══██╗   ██║   ██║     ██╔══╝     ██║   ██║   ██║██║   ██║██║     ╚════██║   ██║     
   ██║   ╚██████╔╝██║  ██║   ██║   ███████╗███████╗   ██║   ╚██████╔╝╚██████╔╝███████╗███████║██╗╚██████╗
   ╚═╝    ╚═════╝ ╚═╝  ╚═╝   ╚═╝   ╚══════╝╚══════╝   ╚═╝    ╚═════╝  ╚═════╝ ╚══════╝╚══════╝╚═╝ ╚═════╝
https://patorjk.com/software/taag/#p=display&f=ANSI%20Shadow

turtleTools library includes:
ribbon
popup
button
switch
dial
slider
scrollbar
dropdown
textbox
context
reader

TODO:
using the tab key to select different elements? And allowing them to be changed with the keyboard??
*/

#include "../include/turtle_tools.h"

/* UI element colours (in order listed in turtle_tools_element_names_t) */
int32_t turtleToolsColorDefault[] = {
    /*         none                           button                         switch                            dial                           slider                          textbox                        dropdown                         scrollbar                      context                       variable reader                   list reader                     ribbon                           popup               */
    0,                              TT_COLOR_TEXT_ALTERNATE,        TT_COLOR_TEXT_BASE,             TT_COLOR_TEXT_BASE,             TT_COLOR_TEXT_BASE,             TT_COLOR_TEXT_ALTERNATE,        TT_COLOR_TEXT_BASE,             0,                              TT_COLOR_TEXT_ALTERNATE,        TT_COLOR_BLACK,                 TT_COLOR_BLACK,                 TT_COLOR_TEXT_ALTERNATE,        TT_COLOR_TEXT_ALTERNATE,        
    0,                              TT_COLOR_COMPONENT,             TT_COLOR_TEXT_ALTERNATE,        TT_COLOR_TEXT_BASE,             TT_COLOR_COMPONENT_ALTERNATE,   TT_COLOR_COMPONENT_BASE,        TT_COLOR_TEXT_ALTERNATE,        TT_COLOR_COMPONENT_BASE,        TT_COLOR_COMPONENT_BASE,        TT_COLOR_WHITE,                 TT_COLOR_WHITE,                 TT_COLOR_COMPONENT_HIGHLIGHT,   TT_COLOR_COMPONENT_ALTERNATE,   
    0,                              TT_COLOR_COMPONENT_HIGHLIGHT,   TT_COLOR_COMPONENT_BASE,        0,                              TT_COLOR_BACKGROUND_COMPLEMENT, TT_COLOR_TEXT_HIGHLIGHT,        TT_COLOR_COMPONENT_BASE,        TT_COLOR_COMPONENT_COMPLEMENT,  TT_COLOR_COMPONENT_HIGHLIGHT,   TT_COLOR_LIGHT_GREY,            TT_COLOR_LIGHT_GREY,            TT_COLOR_COMPONENT_HIGHLIGHT,   TT_COLOR_COMPONENT_HIGHLIGHT,   
    0,                              TT_COLOR_TEXT_BASE,             TT_COLOR_COMPONENT_HIGHLIGHT,   0,                              0,                              TT_COLOR_TEXT_ALTERNATE,        TT_COLOR_COMPONENT_HIGHLIGHT,   TT_COLOR_BACKGROUND_ALTERNATE,  0,                              TT_COLOR_ORANGE,                TT_COLOR_RED_ALTERNATE,         TT_COLOR_COMPONENT,             TT_COLOR_COMPONENT,             
    0,                              TT_COLOR_COMPONENT_COMPLEMENT,  TT_COLOR_BACKGROUND_ALTERNATE,  0,                              0,                              TT_COLOR_BLUE,                  TT_COLOR_COMPONENT_HIGHLIGHT,   TT_COLOR_BACKGROUND_HIGHLIGHT,  0,                              0,                              TT_COLOR_WHITE_ALTERNATE,       TT_COLOR_COMPONENT,             0,                              
    0,                              0,                              TT_COLOR_TERTIARY_BASE,         0,                              0,                              0,                              TT_COLOR_TEXT_ALTERNATE,        0,                              0,                              0,                              TT_COLOR_DARK_GREY_ALTERNATE,   0,                              0,                              
    0,                              0,                              0,                              0,                              0,                              0,                              0,                              0,                              0,                              0,                              TT_COLOR_DARK_GREY,             0,                              0,                              
    0,                              0,                              0,                              0,                              0,                              0,                              0,                              0,                              0,                              0,                              TT_COLOR_DARK_GREY,             0,                              0,                              
};

int random_int(int lowerBound, int upperBound) { // random integer between lower and upper bound (inclusive)
    return (rand() % (upperBound - lowerBound + 1) + lowerBound);
}

double random_double(double lowerBound, double upperBound) { // random double between lower and upper bound
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
    for (int32_t i = 0; i < size + 1; i++) {
        dest[len - size + i] = '\0';
    }
    return dest;
}

turtle_tools_theme_t turtleToolsTheme;
turtle_tools_enabled_t turtleToolsEnabled; // all start at 0 (global variable)
turtle_tools_elements_t turtleToolsElements;

/* default colours (light theme) */
double turtleToolsThemeColors[] = {
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

void turtle_tools_set_color(turtle_tools_color_t color) {
    turtle_pen_color(turtleToolsThemeColors[color], turtleToolsThemeColors[color + 1], turtleToolsThemeColors[color + 2]);
}

void turtle_tools_set_theme(turtle_tools_theme_t theme) {
    if (theme == TT_THEME_DARK) {
        turtleToolsTheme = theme;
        double turtleToolsThemeCopy[] = {
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
        memcpy(turtleToolsThemeColors, turtleToolsThemeCopy, sizeof(turtleToolsThemeCopy));
    } else if (theme == TT_THEME_COLT) {
        turtleToolsTheme = theme;
        double turtleToolsThemeCopy[] = {
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
        memcpy(turtleToolsThemeColors, turtleToolsThemeCopy, sizeof(turtleToolsThemeCopy));
    } else if (theme == TT_THEME_NAVY) {
        turtleToolsTheme = theme;
        double turtleToolsThemeCopy[] = {
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
        memcpy(turtleToolsThemeColors, turtleToolsThemeCopy, sizeof(turtleToolsThemeCopy));
    } else {
        turtleToolsTheme = TT_THEME_LIGHT;
        double turtleToolsThemeCopy[] = {
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
        memcpy(turtleToolsThemeColors, turtleToolsThemeCopy, sizeof(turtleToolsThemeCopy));
    }
    turtle_background_color(turtleToolsThemeColors[TT_COLOR_BACKGROUND], turtleToolsThemeColors[TT_COLOR_BACKGROUND + 1], turtleToolsThemeColors[TT_COLOR_BACKGROUND + 2]);
}

/* ribbon */

/* ribbon variables */

turtle_tools_ribbon_t turtleToolsRibbon;

/* initialise ribbon */
int32_t turtle_tools_ribbon_init(const char *filename) {
    /* load from config file */
    char fileExists = 1;
    list_t *defaultRibbonFile = list_init();
    list_append(defaultRibbonFile, (unitype) "File, 📄 New, 📄 Save, 📄 Save As..., 📄 Open", 's');
    list_append(defaultRibbonFile, (unitype) "Edit, Undo, Redo, Cut, Copy, Paste", 's');
    list_append(defaultRibbonFile, (unitype) "View, Change Theme, GLFW", 's');
    FILE *configFile = fopen(filename, "r");
    if (configFile == NULL) {
        printf("Error: file %s not found\n", filename);
        fileExists = 0;
    }
    return turtle_tools_ribbon_init_internal(configFile, defaultRibbonFile, fileExists);
}

/* initialise ribbon with a list instead of a config file */
int32_t turtle_tools_ribbon_init_list(list_t *config) {
    /* load from config file */
    return turtle_tools_ribbon_init_internal(NULL, config, 0);
}

/* initialise ribbon */
int32_t turtle_tools_ribbon_init_internal(FILE *configFile, list_t *configList, int8_t fileExists) {
    /* enable ribbon */
    turtleToolsEnabled.ribbonEnabled = 1;
    if (turtleToolsEnabled.turtleToolsEnabled == 0) {
        turtleToolsEnabled.turtleToolsEnabled = 1;
        turtleToolsElements.all = list_init();
    }
    turtleToolsRibbon.element = TT_ELEMENT_RIBBON;
    turtleToolsRibbon.priority = turtleToolsRibbon.element;
    turtleToolsRibbon.enabled = TT_ELEMENT_ENABLED;
    turtle_tools_element_reset_color((void *) &turtleToolsRibbon);
    /* set ribbon parameters */
    turtleToolsRibbon.marginSize = (turtle.initbounds[3] - turtle.initbounds[1]) / 36.0; // number of pixels between different items in the ribbon (not affected by ribbonSize)
    turtleToolsRibbon.mainselect[0] = -1;
    turtleToolsRibbon.mainselect[1] = -1;
    turtleToolsRibbon.mainselect[2] = -1;
    turtleToolsRibbon.subselect[0] = -1;
    turtleToolsRibbon.subselect[1] = -1;
    turtleToolsRibbon.subselect[2] = -1;
    turtleToolsRibbon.output[0] = 0;
    turtleToolsRibbon.output[1] = -1;
    turtleToolsRibbon.output[2] = -1;

    turtleToolsRibbon.mouseDown = 0;

    turtleToolsRibbon.bounds[0] = turtle.initbounds[0];
    turtleToolsRibbon.bounds[1] = turtle.initbounds[1];
    turtleToolsRibbon.bounds[2] = turtle.initbounds[2];
    turtleToolsRibbon.bounds[3] = turtle.initbounds[3];

    turtleToolsRibbon.ribbonSize = (turtle.initbounds[3] - turtle.initbounds[1]) / 360.0; // 1 is default for 640 by 360 coordiantes, below 1 is smaller, above 1 is larger (scales as a multiplier, 0.1 is 100x smaller than 10)
    if (turtleToolsRibbon.options != NULL) {
        /* support reinitialising to change ribbon contents */
        list_free(turtleToolsRibbon.options);
    }
    if (turtleToolsRibbon.lengths != NULL) {
        list_free(turtleToolsRibbon.lengths);
    }
    turtleToolsRibbon.options = list_init();
    turtleToolsRibbon.lengths = list_init();

    /* load ribbon options */
    char line[1024]; // maximum size of any list of options
    while ((fileExists == 0 && turtleToolsRibbon.options -> length < configList -> length) || (fileExists == 1 && fgets(line, 1024, configFile) != NULL)) {
        if (fileExists == 0) {
            memcpy(line, configList -> data[turtleToolsRibbon.options -> length].s, strlen(configList -> data[turtleToolsRibbon.options -> length].s) + 1);
        }
        if (line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0'; // cull newline
        }
        list_append(turtleToolsRibbon.options, (unitype) list_init(), 'r');
        char *item = strtok(line, ",");
        while (item != NULL) {
            if (item[0] == ' ') {
                item++; // cull leading space
            }
            list_append(turtleToolsRibbon.options -> data[turtleToolsRibbon.options -> length - 1].r, (unitype) item, 's');
            item = strtok(NULL, ",");
        }
    }
    if (fileExists) {
        fclose(configFile);
    }

    for (int32_t i = 0; i < turtleToolsRibbon.options -> length; i++) {
        list_append(turtleToolsRibbon.lengths, (unitype) turtle_text_get_string_length(turtleToolsRibbon.options -> data[i].r -> data[0].s, 7 * turtleToolsRibbon.ribbonSize), 'd');
        double max = 0;
        for (int32_t j = 1; j < turtleToolsRibbon.options -> data[i].r -> length; j++) {
            double current = turtle_text_get_string_length(turtleToolsRibbon.options -> data[i].r -> data[j].s, 7 * turtleToolsRibbon.ribbonSize);
            if (current > max) {
                max = current;
            }
        }
        list_append(turtleToolsRibbon.lengths, (unitype) max, 'd');
    }
    list_free(configList);
    return 0;
}

/* render ribbon */
void turtle_tools_ribbon_update() {
    turtle_tools_set_color(turtleToolsRibbon.color[TT_COLOR_SLOT_RIBBON_TOP]);
    turtle_rectangle(turtleToolsRibbon.bounds[0], turtleToolsRibbon.bounds[3] - turtleToolsRibbon.ribbonSize * 10, turtleToolsRibbon.bounds[2], turtleToolsRibbon.bounds[3]);
    turtle_tools_set_color(turtleToolsRibbon.color[TT_COLOR_SLOT_RIBBON_TEXT]);
    double cutoff = turtleToolsRibbon.bounds[0] + turtleToolsRibbon.marginSize;
    turtleToolsRibbon.mainselect[0] = -1;
    turtleToolsRibbon.subselect[0] = -1;
    for (int32_t i = 0; i < turtleToolsRibbon.options -> length; i++) {
        double prevCutoff = cutoff;
        if (i == turtleToolsRibbon.mainselect[2]) {
            double xLeft = prevCutoff - turtleToolsRibbon.marginSize / 2.0;
            double xRight = prevCutoff + turtleToolsRibbon.lengths -> data[i * 2 + 1].d + turtleToolsRibbon.marginSize / 2.0;
            double yDown = turtleToolsRibbon.bounds[3] - 10 * turtleToolsRibbon.ribbonSize - 15 * turtleToolsRibbon.ribbonSize * (turtleToolsRibbon.options -> data[i].r -> length - 1) - turtleToolsRibbon.marginSize / 2.0;
            turtle_tools_set_color(turtleToolsRibbon.color[TT_COLOR_SLOT_RIBBON_DROPDOWN]);
            turtle_rectangle(xLeft, turtleToolsRibbon.bounds[3] - 10 * turtleToolsRibbon.ribbonSize, xRight, yDown); // ribbon highlight
            for (int32_t j = 1; j < turtleToolsRibbon.options -> data[i].r -> length; j++) {
                if (turtleToolsGlobals.elementLogicTypeOld <= TT_ELEMENT_RIBBON) {
                    if (turtle.mouseY > turtleToolsRibbon.bounds[3] - 10 * turtleToolsRibbon.ribbonSize - 15 * turtleToolsRibbon.ribbonSize * j - turtleToolsRibbon.marginSize / 4.0 && turtle.mouseY < turtleToolsRibbon.bounds[3] - 10 * turtleToolsRibbon.ribbonSize && turtle.mouseX > xLeft && turtle.mouseX < xRight && turtleToolsRibbon.subselect[0] == -1) {
                        turtle_tools_set_color(turtleToolsRibbon.color[TT_COLOR_SLOT_RIBBON_HOVER]);
                        turtle_rectangle(xLeft, turtleToolsRibbon.bounds[3] - 10 * turtleToolsRibbon.ribbonSize - 15 * turtleToolsRibbon.ribbonSize * (j - 1) - turtleToolsRibbon.marginSize / 4.0, xRight, turtleToolsRibbon.bounds[3] - 10 * turtleToolsRibbon.ribbonSize - 15 * turtleToolsRibbon.ribbonSize * j - turtleToolsRibbon.marginSize / 3.0); // dropdown highlight
                        turtleToolsRibbon.subselect[0] = j;
                    }
                }
                turtle_tools_set_color(turtleToolsRibbon.color[TT_COLOR_SLOT_RIBBON_TEXT]);
                turtle_text_write_unicode(turtleToolsRibbon.options -> data[i].r -> data[j].s, prevCutoff, turtleToolsRibbon.bounds[3] - 5.5 * turtleToolsRibbon.ribbonSize - j * 15 * turtleToolsRibbon.ribbonSize, 7 * turtleToolsRibbon.ribbonSize, 0);
            }
        }
        cutoff += turtleToolsRibbon.lengths -> data[i * 2].d + turtleToolsRibbon.marginSize;
        if (turtleToolsGlobals.elementLogicTypeOld <= TT_ELEMENT_RIBBON) {
            if (turtle.mouseY > turtleToolsRibbon.bounds[3] - 10 * turtleToolsRibbon.ribbonSize && turtle.mouseY < turtleToolsRibbon.bounds[3] && turtle.mouseX > turtleToolsRibbon.bounds[0] + turtleToolsRibbon.marginSize / 2.0 && turtle.mouseX < cutoff - turtleToolsRibbon.marginSize / 2.0 && turtleToolsRibbon.mainselect[0] == -1) { // -217, -195, -164
                turtle_tools_set_color(turtleToolsRibbon.color[TT_COLOR_SLOT_RIBBON_SELECT]);
                turtle_rectangle(prevCutoff - turtleToolsRibbon.marginSize / 2.0, turtleToolsRibbon.bounds[3] - turtleToolsRibbon.ribbonSize, cutoff - turtleToolsRibbon.marginSize / 2.0, turtleToolsRibbon.bounds[3] - 9 * turtleToolsRibbon.ribbonSize); // render dropdown
                turtleToolsRibbon.mainselect[0] = i;
                turtleToolsGlobals.elementLogicType = TT_ELEMENT_RIBBON;
            }
        }
        turtle_tools_set_color(turtleToolsRibbon.color[TT_COLOR_SLOT_RIBBON_TEXT]);
        turtle_text_write_unicode(turtleToolsRibbon.options -> data[i].r -> data[0].s, prevCutoff, turtleToolsRibbon.bounds[3] - 5.5 * turtleToolsRibbon.ribbonSize, 7 * turtleToolsRibbon.ribbonSize, 0);
    }
    if (turtleToolsGlobals.elementLogicTypeOld <= TT_ELEMENT_RIBBON) {
        if (turtleToolsRibbon.mainselect[2] > -1 || turtleToolsRibbon.subselect[2] > -1) {
            turtleToolsGlobals.elementLogicType = TT_ELEMENT_RIBBON;
        }
        if (turtle_mouse_down()) { // this is hideous
            if (turtleToolsRibbon.mouseDown == 0) {
                turtleToolsRibbon.mouseDown = 1;
                if (turtleToolsRibbon.subselect[0] == turtleToolsRibbon.subselect[1] && turtleToolsRibbon.subselect[0] != -1) {
                    turtleToolsRibbon.subselect[2] = turtleToolsRibbon.subselect[0];
                    turtleToolsRibbon.output[0] = 1;
                    turtleToolsRibbon.output[1] = turtleToolsRibbon.mainselect[2];
                    turtleToolsRibbon.output[2] = turtleToolsRibbon.subselect[2];
                }
                if (turtleToolsRibbon.mainselect[0] == turtleToolsRibbon.mainselect[1]) {
                    if (turtleToolsRibbon.mainselect[0] == turtleToolsRibbon.mainselect[2]) {
                        turtleToolsRibbon.mainselect[3] = -1;
                    } else {
                        turtleToolsRibbon.mainselect[2] = turtleToolsRibbon.mainselect[0];
                        turtleToolsGlobals.elementLogicType = TT_ELEMENT_RIBBON;
                    }
                }
            }
        } else {
            if (turtleToolsRibbon.mouseDown == 1) {
                if (turtleToolsRibbon.subselect[0] != -1) {
                    turtleToolsRibbon.subselect[2] = turtleToolsRibbon.subselect[0];
                    turtleToolsRibbon.output[0] = 1;
                    turtleToolsRibbon.output[1] = turtleToolsRibbon.mainselect[2];
                    turtleToolsRibbon.output[2] = turtleToolsRibbon.subselect[2];
                    turtleToolsRibbon.mainselect[2] = -1;
                }
                turtleToolsRibbon.subselect[2] = -1;
            }
            if (turtleToolsRibbon.mainselect[3] == -1 && turtleToolsRibbon.mainselect[0] == turtleToolsRibbon.mainselect[2]) {
                turtleToolsRibbon.mainselect[2] = -1;
            }
            turtleToolsRibbon.mainselect[3] = 0;
            turtleToolsRibbon.mouseDown = 0;
            turtleToolsRibbon.mainselect[1] = turtleToolsRibbon.mainselect[0];
            turtleToolsRibbon.subselect[1] = turtleToolsRibbon.subselect[0];
        }
    }
}

/* popup */

/* popup variables */

turtle_tools_popup_t turtleToolsPopup;

/* initialise popup */
int32_t turtle_tools_popup_init(char *filename) {
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
    return turtle_tools_popup_init_internal(configFile, defaultPopupFile, fileExists);
}

/* initialise popup with a list instead of a config file */
int32_t turtle_tools_popup_init_list(list_t *config) {
    return turtle_tools_popup_init_internal(NULL, config, 0);
}

/* initialise popup */
int32_t turtle_tools_popup_init_internal(FILE *configFile, list_t *configList, int8_t fileExists) {
    turtleToolsEnabled.popupEnabled = 1;
    if (turtleToolsEnabled.turtleToolsEnabled == 0) {
        turtleToolsEnabled.turtleToolsEnabled = 1;
        turtleToolsElements.all = list_init();
    }
    turtleToolsPopup.element = TT_ELEMENT_POPUP;
    turtleToolsPopup.priority = turtleToolsPopup.element;
    turtleToolsPopup.enabled = TT_ELEMENT_ENABLED;
    turtle_tools_element_reset_color((void *) &turtleToolsPopup);
    turtleToolsPopup.output[0] = 0;
    turtleToolsPopup.output[1] = -1;
    turtleToolsPopup.mouseDown = 0;
    turtleToolsPopup.size = (turtle.initbounds[3] - turtle.initbounds[1]) / 72;
    turtleToolsPopup.style = 0;

    char line[256] = {1, 0}; // maximum size of message or option
    /* read popup message */
    if (fileExists) {
        if (fgets(line, 256, configFile) != NULL) {
            if (line[strlen(line) - 1] == '\n') {
                line[strlen(line) - 1] = '\0'; // cull newline
            }
            turtleToolsPopup.message = strdup(line);
        }
    } else {
        turtleToolsPopup.message = strdup(configList -> data[0].s);
    }
    /* read popup options */
    double defaultPadding = (turtle.initbounds[2] - turtle.initbounds[0]) / 50;
    double buttonWidth = defaultPadding;
    turtleToolsPopup.options = list_init();
    while ((fileExists == 0 && turtleToolsPopup.options -> length < configList -> length - 1) || (fileExists == 1 && fgets(line, 256, configFile) != NULL)) {
        if (fileExists == 1) {
            if (line[strlen(line) - 1] == '\n') {
                line[strlen(line) - 1] = '\0'; // cull newline
            }
            list_append(turtleToolsPopup.options, (unitype) strdup(line), 's');
        } else {
            list_append(turtleToolsPopup.options, configList -> data[turtleToolsPopup.options -> length + 1], 's');
        }
        buttonWidth += turtle_text_get_unicode_length(turtleToolsPopup.options -> data[turtleToolsPopup.options -> length - 1].s, turtleToolsPopup.size) + defaultPadding;
    }
    if (fileExists) {
        fclose(configFile);
    }
    list_free(configList);
    double centerY = (turtle.initbounds[1] + turtle.initbounds[3]) / 2;
    double height = (turtle.initbounds[3] - turtle.initbounds[1]) / 9;
    double centerX = (turtle.initbounds[0] + turtle.initbounds[2]) / 2;
    double messageWidth = turtle_text_get_unicode_length(turtleToolsPopup.message, turtleToolsPopup.size) + defaultPadding;
    double width = messageWidth;
    if (messageWidth < buttonWidth) {
        width = buttonWidth;
    }
    turtleToolsPopup.minX = centerX - width / 2;
    turtleToolsPopup.minY = centerY - height / 2;
    turtleToolsPopup.maxX = centerX + width / 2;
    turtleToolsPopup.maxY = centerY + height / 2;
    return 0;
}

/* render popup */
void turtle_tools_popup_update() {
    if (turtle.close == 1) {
        turtleToolsGlobals.elementLogicType = TT_ELEMENT_POPUP;
        turtle_tools_set_color(turtleToolsPopup.color[TT_COLOR_SLOT_POPUP_BOX]);
        turtle_rectangle(turtleToolsPopup.minX, turtleToolsPopup.minY, turtleToolsPopup.maxX, turtleToolsPopup.maxY);
        double textX = turtleToolsPopup.minX + (turtleToolsPopup.maxX - turtleToolsPopup.minX) / 2;
        double textY = turtleToolsPopup.maxY - turtleToolsPopup.size * 2;
        turtle_tools_set_color(turtleToolsPopup.color[TT_COLOR_SLOT_POPUP_TEXT]);
        turtle_text_write_unicode(turtleToolsPopup.message, textX, textY, turtleToolsPopup.size, 50);
        textY -= turtleToolsPopup.size * 4;
        double fullLength = 0;
        for (int32_t i = 0; i < turtleToolsPopup.options -> length; i++) {
            fullLength += turtle_text_get_string_length(turtleToolsPopup.options -> data[i].s, turtleToolsPopup.size);
        }
        /* we have the length of the strings, now we pad with n + 1 padding regions */
        double padding = (turtleToolsPopup.maxX - turtleToolsPopup.minX - fullLength) / (turtleToolsPopup.options -> length + 1);
        textX = turtleToolsPopup.minX + padding;
        char flagged = 0;
        if (!turtle_mouse_down() && turtleToolsPopup.mouseDown == 1) {
            flagged = 1; // flagged for mouse misbehaviour
        }
        for (int32_t i = 0; i < turtleToolsPopup.options -> length; i++) {
            double strLen = turtle_text_get_string_length(turtleToolsPopup.options -> data[i].s, turtleToolsPopup.size);
            if (turtle.mouseX > textX - turtleToolsPopup.size && turtle.mouseX < textX + strLen + turtleToolsPopup.size && turtle.mouseY > textY - turtleToolsPopup.size && turtle.mouseY < textY + turtleToolsPopup.size) {
                turtle_tools_set_color(turtleToolsPopup.color[TT_COLOR_SLOT_POPUP_BUTTON_SELECT]);
                turtle_rectangle(textX - turtleToolsPopup.size, textY - turtleToolsPopup.size, textX + turtleToolsPopup.size + strLen, textY + turtleToolsPopup.size);
                if (turtle_mouse_down()) {
                    if (turtleToolsPopup.mouseDown == 0) {
                        turtleToolsPopup.mouseDown = 1;
                        if (turtleToolsPopup.output[0] == 0) {
                            turtleToolsPopup.output[1] = i;
                        }
                        // turtleToolsGlobals.elementLogicType = TT_ELEMENT_NONE;
                    }
                } else {
                    if (turtleToolsPopup.mouseDown == 1) {
                        turtleToolsPopup.mouseDown = 0;
                        if (turtleToolsPopup.output[1] == i) {
                            turtleToolsPopup.output[0] = 1;
                        }
                        // turtleToolsGlobals.elementLogicType = TT_ELEMENT_NONE;
                    }
                }
            } else {
                turtle_tools_set_color(turtleToolsPopup.color[TT_COLOR_SLOT_POPUP_BUTTON]);
                turtle_rectangle(textX - turtleToolsPopup.size, textY - turtleToolsPopup.size, textX + turtleToolsPopup.size + strLen, textY + turtleToolsPopup.size);
            }
            turtle_tools_set_color(turtleToolsPopup.color[TT_COLOR_SLOT_POPUP_TEXT]);
            turtle_text_write_unicode(turtleToolsPopup.options -> data[i].s, textX, textY, turtleToolsPopup.size, 0);
            textX += strLen + padding;
        }
        if (!turtle_mouse_down() && turtleToolsPopup.mouseDown == 1 && flagged == 1) {
            turtleToolsPopup.mouseDown = 0;
            turtleToolsPopup.output[0] = 0;
            turtleToolsPopup.output[1] = -1;
            // turtleToolsGlobals.elementLogicType = TT_ELEMENT_NONE;
        }
    }
}

void turtle_tools_popup_free() {
    free(turtleToolsPopup.message);
}

/* UI tools */

turtle_tools_globals_t turtleToolsGlobals;

void turtle_tools_element_reset_color(void *elementp) {
    int32_t elementType = ((turtle_tools_button_t *) elementp) -> element;
    for (int32_t i = 0; i < 8; i++) {
        ((turtle_tools_button_t *) elementp) -> color[i] = turtleToolsColorDefault[i * TT_NUMBER_OF_ELEMENTS + elementType];
    }
}

int32_t turtle_tools_element_free(void *elementp) {
    if (elementp == NULL) {
        return -1;
    }
    switch (((turtle_tools_button_t *) elementp) -> element) {
    case TT_ELEMENT_BUTTON:
        tt_buttonFree((turtle_tools_button_t *) elementp);
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
    case TT_ELEMENT_TEXTBOX:
        tt_textboxFree((tt_textbox_t *) elementp);
    break;
    case TT_ELEMENT_DROPDOWN:
        tt_dropdownFree((tt_dropdown_t *) elementp);
    break;
    case TT_ELEMENT_SCROLLBAR:
        tt_scrollbarFree((tt_scrollbar_t *) elementp);
    break;
    case TT_ELEMENT_CONTEXT:
        tt_contextFree((tt_context_t *) elementp);
    break;
    case TT_ELEMENT_VARIABLE_READER:
    // case TT_ELEMENT_LIST_READER:
        tt_readerFree((tt_reader_t *) elementp);
    break;
    default:
        return -1;
    break;
    }
    return 0;
}

void turtle_tools_hide_all_elements() {
    for (int32_t i = 0; i < turtleToolsElements.all -> length; i++) {
        ((turtle_tools_button_t *) turtleToolsElements.all -> data[i].p) -> enabled = TT_ELEMENT_HIDE;
    }
}

/* initialise UI elements */

void turtleToolsInit() {
    if (turtleToolsEnabled.turtleToolsEnabled == 0) {
        turtleToolsEnabled.turtleToolsEnabled = 1;
        turtleToolsGlobals.elementLogicType = TT_ELEMENT_NONE;
        turtleToolsGlobals.elementLogicIndex = -1;
        turtleToolsGlobals.elementLogicTemp = -1;
        turtleToolsElements.all = list_init();
    }
}

/* create a button */
turtle_tools_button_t *tt_buttonInit(char *label, int8_t *variable, double x, double y, double size) {
    if (turtleToolsEnabled.buttonEnabled == 0) {
        turtleToolsEnabled.buttonEnabled = 1;
        turtleToolsElements.buttons = list_init();
    }
    turtleToolsInit();
    turtle_tools_button_t *buttonp = calloc(1, sizeof(turtle_tools_button_t));
    buttonp -> element = TT_ELEMENT_BUTTON;
    buttonp -> priority = buttonp -> element;
    buttonp -> enabled = TT_ELEMENT_ENABLED;
    buttonp -> ignored = TT_ELEMENT_NOT_IGNORED;
    if (label == NULL) {
        memcpy(buttonp -> label, "", strlen("") + 1);
    } else {
        int32_t length = strlen(label) + 1;
        if (length > sizeof(buttonp -> label)) {
            length = sizeof(buttonp -> label) - 1;
            memcpy(buttonp -> label, label, length);
            buttonp -> label[length] = '\0';
        } else {
            memcpy(buttonp -> label, label, length);
        }
    }
    turtle_tools_element_reset_color(buttonp);
    buttonp -> status = TT_STATUS_IDLE;
    buttonp -> x = x;
    buttonp -> y = y;
    buttonp -> size = size;
    buttonp -> variable = variable;
    buttonp -> shape = TT_BUTTON_SHAPE_RECTANGLE;
    buttonp -> align = TT_BUTTON_ALIGN_CENTER;
    if (variable != NULL) {
        *variable = 0; // button starts unpressed
    }
    list_append(turtleToolsElements.buttons, (unitype) (void *) buttonp, 'p');
    list_append(turtleToolsElements.all, (unitype) (void *) buttonp, 'l');
    return buttonp;
}

void tt_buttonFree(turtle_tools_button_t *buttonp) {
    list_remove(turtleToolsElements.all, (unitype) (uint64_t) buttonp, 'l');
    list_remove(turtleToolsElements.buttons, (unitype) (void *) buttonp, 'p');
}

/* create a switch */
tt_switch_t *tt_switchInit(char *label, int8_t *variable, double x, double y, double size) {
    if (turtleToolsEnabled.switchEnabled == 0) {
        turtleToolsEnabled.switchEnabled = 1;
        turtleToolsElements.switches = list_init();
    }
    turtleToolsInit();
    tt_switch_t *switchp = calloc(1, sizeof(tt_switch_t));
    switchp -> element = TT_ELEMENT_SWITCH;
    switchp -> priority = switchp -> element;
    switchp -> enabled = TT_ELEMENT_ENABLED;
    switchp -> ignored = TT_ELEMENT_NOT_IGNORED;
    if (label == NULL) {
        memcpy(switchp -> label, "", strlen("") + 1);
    } else {
        int32_t length = strlen(label) + 1;
        if (length > sizeof(switchp -> label)) {
            length = sizeof(switchp -> label) - 1;
            memcpy(switchp -> label, label, length);
            switchp -> label[length] = '\0';
        } else {
            memcpy(switchp -> label, label, length);
        }
    }
    turtle_tools_element_reset_color(switchp);
    switchp -> status = TT_STATUS_IDLE;
    switchp -> x = x;
    switchp -> y = y;
    switchp -> size = size;
    switchp -> variable = variable;
    switchp -> style = TT_SWITCH_STYLE_CLASSIC;
    switchp -> align = TT_SWITCH_ALIGN_CENTER;
    if (variable == NULL) {
        switchp -> value = 0;
    } else {
        switchp -> value = *variable;
    }
    list_append(turtleToolsElements.switches, (unitype) (void *) switchp, 'p');
    list_append(turtleToolsElements.all, (unitype) (void *) switchp, 'l');
    return switchp;
}

void tt_switchFree(tt_switch_t *switchp) {
    list_remove(turtleToolsElements.all, (unitype) (uint64_t) switchp, 'l');
    list_remove(turtleToolsElements.switches, (unitype) (void *) switchp, 'p');
}

/* create a dial - make renderNumberFactor 0 to hide dial number */
tt_dial_t *tt_dialInit(char *label, int32_t *variable, tt_dial_scale_t scale, double x, double y, double size, int32_t bottom, int32_t top, char *render, double renderMultiplier) {
    if (turtleToolsEnabled.dialEnabled == 0) {
        turtleToolsEnabled.dialEnabled = 1;
        turtleToolsElements.dials = list_init();
    }
    turtleToolsInit();
    tt_dial_t *dialp = calloc(1, sizeof(tt_dial_t));
    dialp -> element = TT_ELEMENT_DIAL;
    dialp -> priority = dialp -> element;
    dialp -> enabled = TT_ELEMENT_ENABLED;
    dialp -> ignored = TT_ELEMENT_NOT_IGNORED;
    if (label == NULL) {
        memcpy(dialp -> label, "", strlen("") + 1);
    } else {
        int32_t stringLength = strlen(label) + 1;
        if (stringLength > sizeof(dialp -> label)) {
            stringLength = sizeof(dialp -> label) - 1;
            memcpy(dialp -> label, label, stringLength);
            dialp -> label[stringLength] = '\0';
        } else {
            memcpy(dialp -> label, label, stringLength);
        }
    }
    turtle_tools_element_reset_color(dialp);
    dialp -> status = TT_STATUS_IDLE;
    dialp -> scale = scale;
    dialp -> x = x;
    dialp -> y = y;
    dialp -> size = size;
    dialp -> range[0] = bottom;
    dialp -> range[1] = top;
    dialp -> variable = variable;
    if (render == NULL) {
        memcpy(dialp -> render, "%.0lf", strlen("%.0lf") + 1);
    } else {
        char renderEdited[sizeof(dialp -> render) * 5];
        int32_t initialLength = strlen(render);
        int32_t editedIndex = 0;
        for (int32_t i = 0; i < initialLength; i++) {
            if (i < initialLength - 1 && render[i] == '%' && (render[i + 1] == 'd' || render[i + 1] == 'i' || render[i + 1] == 'u' || render[i + 1] == 'h')) {
                strcat(renderEdited, "%0.lf");
                editedIndex = strlen(renderEdited);
                i++;
            } else {
                renderEdited[editedIndex] = render[i];
                editedIndex++;
            }
        }
        renderEdited[editedIndex] = '\0';
        int32_t stringLength = strlen(renderEdited) + 1;
        if (stringLength > sizeof(dialp -> render)) {
            stringLength = sizeof(dialp -> render) - 1;
            memcpy(dialp -> render, renderEdited, stringLength);
            dialp -> render[stringLength] = '\0';
        } else {
            memcpy(dialp -> render, renderEdited, stringLength);
        }
    }
    dialp -> renderMultiplier = renderMultiplier;
    if (variable == NULL) {
        dialp -> defaultValue = bottom;
    } else {
        dialp -> defaultValue = *variable;
    }
    dialp -> value = dialp -> defaultValue;
    list_append(turtleToolsElements.dials, (unitype) (void *) dialp, 'p');
    list_append(turtleToolsElements.all, (unitype) (void *) dialp, 'l');
    return dialp;
}

void tt_dialFree(tt_dial_t *dialp) {
    list_remove(turtleToolsElements.all, (unitype) (uint64_t) dialp, 'l');
    list_remove(turtleToolsElements.dials, (unitype) (void *) dialp, 'p');
}

/* create a slider - make renderNumberFactor 0 to hide slider number */
tt_slider_t *tt_sliderInit(char *label, int32_t *variable, tt_slider_type_t type, tt_slider_align_t align, double x, double y, double size, double length, int32_t bottom, int32_t top, char *render, double renderMultiplier) {
    if (turtleToolsEnabled.sliderEnabled == 0) {
        turtleToolsEnabled.sliderEnabled = 1;
        turtleToolsElements.sliders = list_init();
    }
    turtleToolsInit();
    tt_slider_t *sliderp = calloc(1, sizeof(tt_slider_t));
    sliderp -> element = TT_ELEMENT_SLIDER;
    sliderp -> priority = sliderp -> element;
    sliderp -> enabled = TT_ELEMENT_ENABLED;
    sliderp -> ignored = TT_ELEMENT_NOT_IGNORED;
    if (label == NULL) {
        memcpy(sliderp -> label, "", strlen("") + 1);
    } else {
        int32_t stringLength = strlen(label) + 1;
        if (stringLength > sizeof(sliderp -> label)) {
            stringLength = sizeof(sliderp -> label) - 1;
            memcpy(sliderp -> label, label, stringLength);
            sliderp -> label[stringLength] = '\0';
        } else {
            memcpy(sliderp -> label, label, stringLength);
        }
    }
    turtle_tools_element_reset_color(sliderp);
    sliderp -> status = TT_STATUS_IDLE;
    sliderp -> type = type;
    sliderp -> align = align;
    sliderp -> scale = TT_SLIDER_SCALE_LINEAR;
    sliderp -> style = TT_SLIDER_STYLE_CLASSIC;
    sliderp -> x = x;
    sliderp -> y = y;
    sliderp -> size = size;
    sliderp -> length = length;
    sliderp -> range[0] = bottom;
    sliderp -> range[1] = top;
    sliderp -> variable = variable;
    if (render == NULL) {
        memcpy(sliderp -> render, "%.0lf", strlen("%.0lf") + 1);
    } else {
        char renderEdited[sizeof(sliderp -> render) * 5];
        int32_t initialLength = strlen(render);
        int32_t editedIndex = 0;
        for (int32_t i = 0; i < initialLength; i++) {
            if (i < initialLength - 1 && render[i] == '%' && (render[i + 1] == 'd' || render[i + 1] == 'i' || render[i + 1] == 'u' || render[i + 1] == 'h')) {
                strcat(renderEdited, "%0.lf");
                editedIndex = strlen(renderEdited);
                i++;
            } else {
                renderEdited[editedIndex] = render[i];
                editedIndex++;
            }
        }
        renderEdited[editedIndex] = '\0';
        int32_t stringLength = strlen(renderEdited) + 1;
        if (stringLength > sizeof(sliderp -> render)) {
            stringLength = sizeof(sliderp -> render) - 1;
            memcpy(sliderp -> render, renderEdited, stringLength);
            sliderp -> render[stringLength] = '\0';
        } else {
            memcpy(sliderp -> render, renderEdited, stringLength);
        }
    }
    sliderp -> renderMultiplier = renderMultiplier;
    if (variable == NULL) {
        sliderp -> defaultValue = bottom;
    } else {
        sliderp -> defaultValue = *variable;
    }
    sliderp -> value = sliderp -> defaultValue;
    list_append(turtleToolsElements.sliders, (unitype) (void *) sliderp, 'p');
    list_append(turtleToolsElements.all, (unitype) (void *) sliderp, 'l');
    return sliderp;
}

void tt_sliderFree(tt_slider_t *sliderp) {
    list_remove(turtleToolsElements.all, (unitype) (uint64_t) sliderp, 'l');
    list_remove(turtleToolsElements.sliders, (unitype) (void *) sliderp, 'p');
}

/* create a textbox */
tt_textbox_t *tt_textboxInit(char *label, char *variable, int32_t maxCharacters, double x, double y, double size, double length) {
    if (turtleToolsEnabled.textboxEnabled == 0) {
        turtle.unicodeCallback = tt_textboxUnicodeCallback;
        turtle.keyCallback = tt_textboxKeyCallback;
        turtleToolsEnabled.textboxEnabled = 1;
        turtleToolsElements.textboxes = list_init();
    }
    turtleToolsInit();
    tt_textbox_t *textboxp = calloc(1, sizeof(tt_textbox_t));
    textboxp -> element = TT_ELEMENT_TEXTBOX;
    textboxp -> priority = textboxp -> element;
    textboxp -> enabled = TT_ELEMENT_ENABLED;
    textboxp -> ignored = TT_ELEMENT_NOT_IGNORED;
    if (label == NULL) {
        memcpy(textboxp -> label, "", strlen("") + 1);
    } else {
        int32_t length = strlen(label) + 1;
        if (length > sizeof(textboxp -> label)) {
            length = sizeof(textboxp -> label) - 1;
            memcpy(textboxp -> label, label, length);
            textboxp -> label[length] = '\0';
        } else {
            memcpy(textboxp -> label, label, length);
        }
    }
    turtle_tools_element_reset_color(textboxp);
    textboxp -> status = TT_STATUS_IDLE;
    textboxp -> mouseOver = 0;
    textboxp -> moveToTop = 0;
    textboxp -> count = 0;
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
    textboxp -> value = textboxp -> text;
    textboxp -> maxCharacters = maxCharacters;
    textboxp -> editIndex = 0;
    textboxp -> editIndexLength = 0;
    textboxp -> lastKey = 0;
    textboxp -> keyTimeout = 0;
    textboxp -> initialKeyTimeout = 48;
    textboxp -> heldKeyTimeout = 2;
    textboxp -> linePeriod = 132;
    textboxp -> doubleClickEditIndex = -1;
    textboxp -> doubleClickCount = -1;
    textboxp -> doubleClickTimeout = 24;
    textboxp -> editingMode = 0;
    textboxp -> renderPixelOffset = 0;
    textboxp -> renderStartingIndex = 0;
    textboxp -> renderNumCharacters = 0;
    list_append(turtleToolsElements.textboxes, (unitype) (void *) textboxp, 'p');
    list_append(turtleToolsElements.all, (unitype) (void *) textboxp, 'l');
    textboxp -> whitelist = NULL;
    textboxp -> blacklist = NULL;
    return textboxp;
}

void tt_textboxFree(tt_textbox_t *textboxp) {
    list_remove(turtleToolsElements.all, (unitype) (uint64_t) textboxp, 'l');
    list_remove(turtleToolsElements.textboxes, (unitype) (void *) textboxp, 'p');
}

void tt_dropdownCalculateMax(tt_dropdown_t *dropdownp) {
    dropdownp -> maxXfactor = 0;
    for (int32_t i = 0; i < dropdownp -> options -> length; i++) {
        double stringLength = turtle_text_get_string_length(dropdownp -> options -> data[i].s, dropdownp -> size - 1);
        if (stringLength > dropdownp -> maxXfactor) {
            dropdownp -> maxXfactor = stringLength;
        }
    }
}

/* create a dropdown - use a list of strings for options */
tt_dropdown_t *tt_dropdownInit(char *label, list_t *options, int32_t *variable, tt_dropdown_align_t align, double x, double y, double size) {
    if (turtleToolsEnabled.dropdownEnabled == 0) {
        turtleToolsEnabled.dropdownEnabled = 1;
        turtleToolsElements.dropdowns = list_init();
    }
    turtleToolsInit();
    tt_dropdown_t *dropdownp = calloc(1, sizeof(tt_dropdown_t));
    dropdownp -> element = TT_ELEMENT_DROPDOWN;
    dropdownp -> priority = dropdownp -> element;
    dropdownp -> enabled = TT_ELEMENT_ENABLED;
    dropdownp -> ignored = TT_ELEMENT_NOT_IGNORED;
    if (label == NULL) {
        memcpy(dropdownp -> label, "", strlen("") + 1);
    } else {
        int32_t length = strlen(label) + 1;
        if (length > sizeof(dropdownp -> label)) {
            length = sizeof(dropdownp -> label) - 1;
            memcpy(dropdownp -> label, label, length);
            dropdownp -> label[length] = '\0';
        } else {
            memcpy(dropdownp -> label, label, length);
        }
    }
    turtle_tools_element_reset_color(dropdownp);
    dropdownp -> options = options;
    if (variable == NULL) {
        dropdownp -> index = 0;
    } else {
        dropdownp -> index = *variable;
    }
    dropdownp -> value = dropdownp -> index;
    dropdownp -> status = TT_STATUS_IDLE;
    dropdownp -> align = align;
    dropdownp -> direction = TT_DROPDOWN_DIRECTION_AUTO;
    dropdownp -> moveToTop = 0;
    dropdownp -> autoLowerBound = turtle.initbounds[1];
    dropdownp -> autoUpperBound = turtle.initbounds[3];
    dropdownp -> x = x;
    dropdownp -> y = y;
    dropdownp -> size = size;
    dropdownp -> variable = variable;
    tt_dropdownCalculateMax(dropdownp);
    list_append(turtleToolsElements.dropdowns, (unitype) (void *) dropdownp, 'p');
    list_append(turtleToolsElements.all, (unitype) (void *) dropdownp, 'l');
    return dropdownp;
}

void tt_dropdownFree(tt_dropdown_t *dropdownp) {
    list_free(dropdownp -> options);
    list_remove(turtleToolsElements.all, (unitype) (uint64_t) dropdownp, 'l');
    list_remove(turtleToolsElements.dropdowns, (unitype) (void *) dropdownp, 'p');
}

/* create a scrollbar */
tt_scrollbar_t *tt_scrollbarInit(double *variable, tt_scrollbar_type_t type, double x, double y, double size, double length, double barPercentage) {
    if (turtleToolsEnabled.scrollbarEnabled == 0) {
        turtleToolsEnabled.scrollbarEnabled = 1;
        turtleToolsElements.scrollbars = list_init();
    }
    turtleToolsInit();
    tt_scrollbar_t *scrollbarp = calloc(1, sizeof(tt_scrollbar_t));
    scrollbarp -> element = TT_ELEMENT_SCROLLBAR;
    scrollbarp -> priority = scrollbarp -> element;
    scrollbarp -> enabled = TT_ELEMENT_ENABLED;
    scrollbarp -> ignored = TT_ELEMENT_NOT_IGNORED;
    turtle_tools_element_reset_color(scrollbarp);
    scrollbarp -> status = TT_STATUS_IDLE;
    scrollbarp -> type = type;
    scrollbarp -> x = x;
    scrollbarp -> y = y;
    scrollbarp -> size = size;
    scrollbarp -> length = length;
    scrollbarp -> barPercentage = barPercentage;
    scrollbarp -> variable = variable;
    list_append(turtleToolsElements.scrollbars, (unitype) (void *) scrollbarp, 'p');
    list_append(turtleToolsElements.all, (unitype) (void *) scrollbarp, 'l');
    return scrollbarp;
}

void tt_scrollbarFree(tt_scrollbar_t *scrollbarp) {
    list_remove(turtleToolsElements.all, (unitype) (uint64_t) scrollbarp, 'l');
    list_remove(turtleToolsElements.scrollbars, (unitype) (void *) scrollbarp, 'p');
}

void tt_contextCalculateMax(tt_context_t *contextp) {
    contextp -> maxXfactor = 0;
    for (int32_t i = 0; i < contextp -> options -> length; i++) {
        double stringLength = turtle_text_get_string_length(contextp -> options -> data[i].s, contextp -> size - 1);
        if (stringLength > contextp -> maxXfactor) {
            contextp -> maxXfactor = stringLength;
        }
    }
}

/* create a context menu */
tt_context_t *tt_contextInit(list_t *options, int32_t *variable, double x, double y, double size) {
    if (turtleToolsEnabled.contextEnabled == 0) {
        turtleToolsEnabled.contextEnabled = 1;
        turtleToolsElements.contexts = list_init();
    }
    turtleToolsInit();
    tt_context_t *contextp = calloc(1, sizeof(tt_context_t));
    contextp -> element = TT_ELEMENT_CONTEXT;
    contextp -> priority = contextp -> element;
    contextp -> enabled = TT_ELEMENT_ENABLED;
    contextp -> ignored = TT_ELEMENT_NOT_IGNORED;
    turtle_tools_element_reset_color(contextp);
    contextp -> options = options;
    contextp -> index = -1;
    contextp -> value = -1;
    contextp -> status = TT_STATUS_IDLE;
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
    list_append(turtleToolsElements.contexts, (unitype) (void *) contextp, 'p');
    list_append(turtleToolsElements.all, (unitype) (void *) contextp, 'l');
    return contextp;
}

void tt_contextFree(tt_context_t *contextp) {
    list_free(contextp -> options);
    list_remove(turtleToolsElements.all, (unitype) (uint64_t) contextp, 'l');
    list_remove(turtleToolsElements.contexts, (unitype) (void *) contextp, 'p');
}

tt_reader_t *tt_readerInit(char *label, unitype *variable, char type, double x, double y, double size) {
    if (turtleToolsEnabled.readerEnabled == 0) {
        turtleToolsEnabled.readerEnabled = 1;
        turtleToolsElements.readers = list_init();
    }
    turtleToolsInit();
    tt_reader_t *readerp = calloc(1, sizeof(tt_reader_t));
    if (type == UNITYPE_LIST) {
        readerp -> element = TT_ELEMENT_LIST_READER;
    } else {
        readerp -> element = TT_ELEMENT_VARIABLE_READER;
    }
    readerp -> priority = TT_ELEMENT_LIST_READER; // list and variable readers use the same priority
    readerp -> enabled = TT_ELEMENT_ENABLED;
    readerp -> ignored = TT_ELEMENT_NOT_IGNORED;
    if (label == NULL) {
        memcpy(readerp -> label, "", strlen("") + 1);
    } else {
        int32_t length = strlen(label) + 1;
        if (length > sizeof(readerp -> label)) {
            length = sizeof(readerp -> label) - 1;
            memcpy(readerp -> label, label, length);
            readerp -> label[length] = '\0';
        } else {
            memcpy(readerp -> label, label, length);
        }
    }
    turtle_tools_element_reset_color(readerp);
    readerp -> x = x;
    readerp -> y = y;
    readerp -> size = size;
    readerp -> status = TT_STATUS_IDLE;
    readerp -> variable = variable;
    readerp -> type = type;
    readerp -> resizing = 0;
    readerp -> moveToTop = 0;
    readerp -> scrollbarp = NULL;
    if (readerp -> element == TT_ELEMENT_LIST_READER) {
        readerp -> width = size * 13;
        readerp -> height = size * 20;
        list_t *list = (*(readerp -> variable)).r; // insane syntax
        double percentage = 100;
        if (list -> length > 20) {
            percentage = 100.0 / ((list -> length - 20) / 10);
        }
        readerp -> scrollbarp = tt_scrollbarInit(NULL, TT_SCROLLBAR_TYPE_VERTICAL, x + size * 5, y, size * 0.75, size * 10, percentage);
        readerp -> scrollbarp -> ignored = TT_ELEMENT_IGNORED; // this scrollbar is updated with the list reader to ensure it appears on top of the reader
        readerp -> scrollbarp -> priority = TT_ELEMENT_LIST_READER;
    }
    list_append(turtleToolsElements.readers, (unitype) (void *) readerp, 'p');
    list_append(turtleToolsElements.all, (unitype) (void *) readerp, 'l');
    return readerp;
}

void tt_readerFree(tt_reader_t *readerp) {
    if (readerp -> element == TT_ELEMENT_VARIABLE_READER) {
        list_remove(turtleToolsElements.all, (unitype) (uint64_t) readerp, 'l');
        list_remove(turtleToolsElements.readers, (unitype) (void *) readerp, 'p');
    } else if (readerp -> element == TT_ELEMENT_LIST_READER) {
        list_remove(turtleToolsElements.all, (unitype) (uint64_t) readerp -> scrollbarp, 'l');
        list_remove(turtleToolsElements.scrollbars, (unitype) (void *) readerp -> scrollbarp, 'p');
        list_remove(turtleToolsElements.all, (unitype) (uint64_t) readerp, 'l');
        list_remove(turtleToolsElements.readers, (unitype) (void *) readerp, 'p');
    }
}

void tt_buttonUpdate(turtle_tools_button_t *buttonp) {
    if (buttonp -> variable != NULL) {
        buttonp -> value = *buttonp -> variable;
    }
    if (buttonp -> enabled == TT_ELEMENT_HIDE) {
        if (buttonp -> status != TT_STATUS_BLOCKED) {
            buttonp -> status = TT_STATUS_IDLE;
        }
        buttonp -> value = 0;
        return;
    }
    double buttonLeftX = buttonp -> x;
    double buttonRightX = buttonp -> x;
    double buttonY = buttonp -> y;
    double buttonWidth = turtle_text_get_unicode_length(buttonp -> label, buttonp -> size - 1) + buttonp -> size * 0.8;
    if (buttonp -> align == TT_BUTTON_ALIGN_CENTER) {
        buttonLeftX -= buttonWidth / 2;
        buttonRightX += buttonWidth / 2;
    } else if (buttonp -> align == TT_BUTTON_ALIGN_LEFT) {
        buttonRightX += buttonWidth;
    } else if (buttonp -> align == TT_BUTTON_ALIGN_RIGHT) {
        buttonLeftX -= buttonWidth;
    }
    double buttonHeight = buttonp -> size * 1.75;
    switch (buttonp -> status) {
        case TT_STATUS_HOVER_FIRST_TICK:
        case TT_STATUS_HOVER:
        turtle_tools_set_color(buttonp -> color[TT_COLOR_SLOT_BUTTON_SELECT]);
        break;
        case TT_STATUS_CLICK_FIRST_TICK:
        case TT_STATUS_CLICK:
        turtle_tools_set_color(buttonp -> color[TT_COLOR_SLOT_BUTTON_CLICKED]);
        break;
        default:
        turtle_tools_set_color(buttonp -> color[TT_COLOR_SLOT_BUTTON]);
        break;
    }
    if (buttonp -> shape == TT_BUTTON_SHAPE_RECTANGLE) {
        turtle_rectangle(buttonLeftX, buttonY - buttonHeight / 2, buttonRightX, buttonY + buttonHeight / 2);
    } else if (buttonp -> shape == TT_BUTTON_SHAPE_ROUNDED_RECTANGLE) {
        turtle_pen_size(buttonp -> size);
        turtle_goto(buttonLeftX + buttonp -> size / 2, buttonY - buttonHeight / 2 + buttonp -> size / 2);
        turtle_pen_down();
        turtle_goto(buttonRightX - buttonp -> size / 2, buttonY - buttonHeight / 2 + buttonp -> size / 2);
        turtle_goto(buttonRightX - buttonp -> size / 2, buttonY + buttonHeight / 2 - buttonp -> size / 2);
        turtle_goto(buttonLeftX + buttonp -> size / 2, buttonY + buttonHeight / 2 - buttonp -> size / 2);
        turtle_goto(buttonLeftX + buttonp -> size / 2, buttonY - buttonHeight / 2 + buttonp -> size / 2);
        turtle_pen_up();
        turtle_rectangle(buttonLeftX + buttonp -> size / 2, buttonY - buttonHeight / 2 + buttonp -> size / 2, buttonRightX - buttonp -> size / 2, buttonY + buttonHeight / 2 - buttonp -> size / 2);
    } else if (buttonp -> shape == TT_BUTTON_SHAPE_CIRCLE) {
        turtle_goto((buttonLeftX + buttonRightX) / 2, buttonY);
        turtle_pen_size(buttonWidth);
        turtle_pen_down();
        turtle_pen_up();
    }
    turtle_tools_set_color(buttonp -> color[TT_COLOR_SLOT_BUTTON_TEXT]);
    if (buttonp -> shape == TT_BUTTON_SHAPE_TEXT) {
        switch (buttonp -> status) {
            case TT_STATUS_HOVER_FIRST_TICK:
            case TT_STATUS_HOVER:
            turtle_tools_set_color(buttonp -> color[TT_COLOR_SLOT_BUTTON_TEXT]);
            break;
            case TT_STATUS_CLICK_FIRST_TICK:
            case TT_STATUS_CLICK:
            turtle_tools_set_color(buttonp -> color[TT_COLOR_SLOT_BUTTON_CLICKED]);
            break;
            default:
            turtle_tools_set_color(buttonp -> color[TT_COLOR_SLOT_BUTTON_SELECTED_TEXT]);
            break;
        }
    }
    turtle_text_write_unicode(buttonp -> label, (buttonLeftX + buttonRightX) / 2, buttonY, buttonp -> size - 1, 50);
    /* mouse */
    if (buttonp -> enabled != TT_ELEMENT_ENABLED || turtleToolsGlobals.elementLogicTypeOld > buttonp -> priority || (turtleToolsGlobals.elementLogicTypeOld == buttonp -> priority && turtleToolsGlobals.elementLogicIndexOld > turtleToolsGlobals.elementLogicTemp)) {
        /* button not enabled or higher priority element is being interacted with */
        if (buttonp -> status != TT_STATUS_BLOCKED) {
            buttonp -> status = TT_STATUS_IDLE;
        }
        buttonp -> value = 0;
        goto LABEL_BUTTON_END;
    }
    LABEL_BUTTON_CHECK_HOVER:
    if (buttonp -> status != TT_STATUS_CLICK && buttonp -> status != TT_STATUS_BLOCKED && buttonp -> status != TT_STATUS_CLICK_FIRST_TICK) {
        if (buttonp -> shape == TT_BUTTON_SHAPE_CIRCLE) {
            /* circle hovering criteria */
            if ((turtle.mouseX - (buttonLeftX + buttonRightX) / 2) * (turtle.mouseX - (buttonLeftX + buttonRightX) / 2) + (turtle.mouseY - buttonY) * (turtle.mouseY - buttonY) < buttonWidth * buttonWidth / 4) {
                if (buttonp -> status == TT_STATUS_HOVER || buttonp -> status == TT_STATUS_HOVER_FIRST_TICK) {
                    /* hovering button */
                    buttonp -> status = TT_STATUS_HOVER;
                } else {
                    /* first tick hover */
                    buttonp -> status = TT_STATUS_HOVER_FIRST_TICK;
                }
            } else {
                buttonp -> status = TT_STATUS_IDLE;
            }
        } else {
            /* rectangle and rounded rectangle hovering criteria */
            if (turtle.mouseX > buttonLeftX && turtle.mouseX < buttonRightX && turtle.mouseY > buttonY - buttonHeight / 2 && turtle.mouseY < buttonY + buttonHeight / 2) {
                if (buttonp -> status == TT_STATUS_HOVER || buttonp -> status == TT_STATUS_HOVER_FIRST_TICK) {
                    /* hovering button */
                    buttonp -> status = TT_STATUS_HOVER;
                } else {
                    /* first tick hover */
                    buttonp -> status = TT_STATUS_HOVER_FIRST_TICK;
                }
            } else {
                buttonp -> status = TT_STATUS_IDLE;
            }
        }
    }
    if (turtle_mouse_down()) {
        if (buttonp -> status == TT_STATUS_HOVER || buttonp -> status == TT_STATUS_HOVER_FIRST_TICK) {
            /* first tick clicked */
            buttonp -> status = TT_STATUS_CLICK_FIRST_TICK;
        } else if (buttonp -> status == TT_STATUS_CLICK || buttonp -> status == TT_STATUS_CLICK_FIRST_TICK) {
            /* button is being held */
            buttonp -> status = TT_STATUS_CLICK;
        } else {
            /* button is blocked from interaction until mouse is unclicked */
            buttonp -> status = TT_STATUS_BLOCKED;
        }
    } else {
        if (buttonp -> status == TT_STATUS_CLICK || buttonp -> status == TT_STATUS_BLOCKED || buttonp -> status == TT_STATUS_CLICK_FIRST_TICK) {
            /* first tick unclicked */
            buttonp -> status = TT_STATUS_IDLE;
            goto LABEL_BUTTON_CHECK_HOVER; // done to avoid a single IDLE tick if mouse is hovering over button when unclicked
        }
    }
    if (buttonp -> status == TT_STATUS_CLICK_FIRST_TICK) {
        /* only set value on first tick so that it is "floating" if being held down */
        buttonp -> value = 1;
    } else {
        if (buttonp -> status != TT_STATUS_CLICK) {
            buttonp -> value = 0;
        }
    }
    if (buttonp -> status == TT_STATUS_HOVER || buttonp -> status == TT_STATUS_CLICK || buttonp -> status == TT_STATUS_HOVER_FIRST_TICK || buttonp -> status == TT_STATUS_CLICK_FIRST_TICK) {
        turtleToolsGlobals.elementLogicType = buttonp -> priority;
        turtleToolsGlobals.elementLogicIndex = turtleToolsGlobals.elementLogicTemp;
    }
    LABEL_BUTTON_END:
    if (buttonp -> variable != NULL) {
        *buttonp -> variable = buttonp -> value;
    }
}

void tt_switchUpdate(tt_switch_t *switchp) {
    if (switchp -> variable != NULL) {
        switchp -> value = *switchp -> variable;
    }
    if (switchp -> enabled == TT_ELEMENT_HIDE) {
        if (switchp -> status != TT_STATUS_BLOCKED) {
            switchp -> status = TT_STATUS_IDLE;
        }
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
    if (switchp -> style == TT_SWITCH_STYLE_CLASSIC || switchp -> style == TT_SWITCH_STYLE_SIDESWIPE) {
        /* render switch */
        if (switchp -> value) {
            turtle_tools_set_color(switchp -> color[TT_COLOR_SLOT_SWITCH_ON]);
        } else {
            turtle_tools_set_color(switchp -> color[TT_COLOR_SLOT_SWITCH_OFF]);
        }
        turtle_pen_size(switchp -> size * 1.2);
        turtle_goto(switchX - switchp -> size * 0.8, switchY);
        turtle_pen_down();
        turtle_goto(switchX + switchp -> size * 0.8, switchY);
        turtle_pen_up();
        turtle_pen_size(switchp -> size);
        if (switchp -> value) {
            turtle_tools_set_color(switchp -> color[TT_COLOR_SLOT_CIRCLE_ON]);
            turtle_goto(switchX + switchp -> size * 0.8, switchY);
        } else {
            turtle_tools_set_color(switchp -> color[TT_COLOR_SLOT_CIRCLE_OFF]);
            turtle_goto(switchX - switchp -> size * 0.8, switchY);
        }
        turtle_pen_down();
        turtle_pen_up();
        /* mouse parameters */
        if (switchp -> style == TT_SWITCH_STYLE_CLASSIC) {
            switchClickLeft = switchX - switchp -> size * 1.35;
            switchClickRight = switchX + switchp -> size * 1.35;
        } else if (switchp -> style == TT_SWITCH_STYLE_SIDESWIPE && (switchp -> align == TT_SWITCH_ALIGN_LEFT || switchp -> align == TT_SWITCH_ALIGN_CENTER)) {
            double textLength = turtle_text_get_unicode_length(switchp -> label, switchp -> size - 1);
            switchClickLeft = switchX - switchp -> size * 1.35;
            switchClickRight = switchX + switchp -> size * 2.2 + textLength;
        } else if (switchp -> style == TT_SWITCH_STYLE_SIDESWIPE && switchp -> align == TT_SWITCH_ALIGN_RIGHT) {
            double textLength = turtle_text_get_unicode_length(switchp -> label, switchp -> size - 1);
            switchClickLeft = switchX - switchp -> size * 2 - textLength;
            switchClickRight = switchX + switchp -> size * 1.35;
        }
        switchClickDown = switchY - switchp -> size * 0.6;
        switchClickUp = switchY + switchp -> size * 0.6;
        /* render text */
        turtle_tools_set_color(switchp -> color[TT_COLOR_SLOT_SWITCH_TEXT]);
        if (switchp -> style == TT_SWITCH_STYLE_CLASSIC) {
            if (switchp -> align == TT_SWITCH_ALIGN_CENTER) {
                turtle_text_write_unicode(switchp -> label, switchX, switchY + 1.6 * switchp -> size, switchp -> size - 1, 50);
            } else if (switchp -> align == TT_SWITCH_ALIGN_LEFT) {
                turtle_text_write_unicode(switchp -> label, switchX - switchp -> size * 1.2, switchY + 1.6 * switchp -> size, switchp -> size - 1, 0);
            } else if (switchp -> align == TT_SWITCH_ALIGN_RIGHT) {
                turtle_text_write_unicode(switchp -> label, switchX + switchp -> size * 1.2, switchY + 1.6 * switchp -> size, switchp -> size - 1, 100);
            }
        } else if (switchp -> style == TT_SWITCH_STYLE_SIDESWIPE && (switchp -> align == TT_SWITCH_ALIGN_LEFT || switchp -> align == TT_SWITCH_ALIGN_CENTER)) {
            if (switchp -> status == TT_STATUS_IDLE || switchp -> status == TT_STATUS_BLOCKED) {
                turtle_tools_set_color(switchp -> color[TT_COLOR_SLOT_SWITCH_TEXT]);
            } else {
                turtle_tools_set_color(switchp -> color[TT_COLOR_SLOT_SWITCH_TEXT_HOVER]);
            }
            turtle_text_write_unicode(switchp -> label, switchX + switchp -> size * 2, switchY, switchp -> size - 1, 0);
        } else if (switchp -> style == TT_SWITCH_STYLE_SIDESWIPE && switchp -> align == TT_SWITCH_ALIGN_RIGHT) {
            if (switchp -> status == TT_STATUS_IDLE || switchp -> status == TT_STATUS_BLOCKED) {
                turtle_tools_set_color(switchp -> color[TT_COLOR_SLOT_SWITCH_TEXT]);
            } else {
                turtle_tools_set_color(switchp -> color[TT_COLOR_SLOT_SWITCH_TEXT_HOVER]);
            }
            turtle_text_write_unicode(switchp -> label, switchX - switchp -> size * 2, switchY, switchp -> size - 1, 100);
        }
    } else if (switchp -> style == TT_SWITCH_STYLE_VERTICAL) {
        /* render switch */
        if (switchp -> value) {
            turtle_tools_set_color(switchp -> color[TT_COLOR_SLOT_SWITCH_ON]);
        } else {
            turtle_tools_set_color(switchp -> color[TT_COLOR_SLOT_SWITCH_OFF]);
        }
        turtle_pen_size(switchp -> size * 1.2);
        turtle_goto(switchX, switchY - switchp -> size * 0.8);
        turtle_pen_down();
        turtle_goto(switchX, switchY + switchp -> size * 0.8);
        turtle_pen_up();
        turtle_pen_size(switchp -> size);
        if (switchp -> value) {
            turtle_tools_set_color(switchp -> color[TT_COLOR_SLOT_CIRCLE_ON]);
            turtle_goto(switchX, switchY + switchp -> size * 0.8);
        } else {
            turtle_tools_set_color(switchp -> color[TT_COLOR_SLOT_CIRCLE_OFF]);
            turtle_goto(switchX, switchY - switchp -> size * 0.8);
        }
        turtle_pen_down();
        turtle_pen_up();
        /* mouse parameters */
        switchClickLeft = switchX - switchp -> size * 0.6;
        switchClickRight = switchX + switchp -> size * 0.6;
        switchClickDown = switchY - switchp -> size * 1.35;
        switchClickUp = switchY + switchp -> size * 1.35;
        /* render text */
        turtle_tools_set_color(switchp -> color[TT_COLOR_SLOT_SWITCH_TEXT]);
        if (switchp -> align == TT_SWITCH_ALIGN_CENTER) {
            turtle_text_write_unicode(switchp -> label, switchX, switchY + 2.2 * switchp -> size, switchp -> size - 1, 50);
        } else if (switchp -> align == TT_SWITCH_ALIGN_LEFT) {
            turtle_text_write_unicode(switchp -> label, switchX - switchp -> size * 1.2, switchY + 2.2 * switchp -> size, switchp -> size - 1, 0);
        } else if (switchp -> align == TT_SWITCH_ALIGN_RIGHT) {
            turtle_text_write_unicode(switchp -> label, switchX + switchp -> size * 1.2, switchY + 2.2 * switchp -> size, switchp -> size - 1, 100);
        }
    } else if (switchp -> style == TT_SWITCH_STYLE_CHECKBOX || switchp -> style == TT_SWITCH_STYLE_XBOX) {
        /* render box */
        turtle_tools_set_color(switchp -> color[TT_COLOR_SLOT_SWITCH_ON]);
        turtle_rectangle(switchX - switchp -> size / 2, switchY - switchp -> size / 2, switchX + switchp -> size / 2, switchY + switchp -> size / 2);
        turtle_tools_set_color(switchp -> color[TT_COLOR_SLOT_SWITCH_OFF]);
        turtle_pen_size(switchp -> size / 6);
        turtle_goto(switchX - switchp -> size / 2, switchY - switchp -> size / 2);
        turtle_pen_down();
        turtle_goto(switchX + switchp -> size / 2, switchY - switchp -> size / 2);
        turtle_goto(switchX + switchp -> size / 2, switchY + switchp -> size / 2);
        turtle_goto(switchX - switchp -> size / 2, switchY + switchp -> size / 2);
        turtle_goto(switchX - switchp -> size / 2, switchY - switchp -> size / 2);
        turtle_pen_up();
        if (switchp -> value) {
            if (switchp -> style == TT_SWITCH_STYLE_CHECKBOX) {
                /* render check */
                turtle_tools_set_color(switchp -> color[TT_COLOR_SLOT_CIRCLE_ON]);
                turtle_goto(switchX - switchp -> size / 2.5, switchY);
                turtle_pen_down();
                turtle_goto(switchX, switchY - switchp -> size / 2.5);
                turtle_goto(switchX + switchp -> size / 2.5, switchY + switchp -> size / 2.5);
                turtle_pen_up();
            } else if (switchp -> style == TT_SWITCH_STYLE_XBOX) {
                /* render X */
                turtle_tools_set_color(switchp -> color[TT_COLOR_SLOT_CIRCLE_ON]);
                turtle_goto(switchX + switchp -> size / 2.5, switchY + switchp -> size / 2.5);
                turtle_pen_down();
                turtle_goto(switchX - switchp -> size / 2.5, switchY - switchp -> size / 2.5);
                turtle_pen_up();
                turtle_goto(switchX + switchp -> size / 2.5, switchY - switchp -> size / 2.5);
                turtle_pen_down();
                turtle_goto(switchX - switchp -> size / 2.5, switchY + switchp -> size / 2.5);
                turtle_pen_up();
            }
        }
        /* mouse parameters - include text */
        double textLength = turtle_text_get_unicode_length(switchp -> label, switchp -> size - 1);
        switchClickLeft = switchX - switchp -> size * 0.6;
        switchClickRight = switchX + switchp -> size * 1.2 + textLength;
        switchClickDown = switchY - switchp -> size * 0.6;
        switchClickUp = switchY + switchp -> size * 0.6;
        /* render text */
        if (switchp -> status == TT_STATUS_IDLE || switchp -> status == TT_STATUS_BLOCKED) {
            turtle_tools_set_color(switchp -> color[TT_COLOR_SLOT_SWITCH_TEXT]);
        } else {
            turtle_tools_set_color(switchp -> color[TT_COLOR_SLOT_SWITCH_TEXT_HOVER]);
        }
        turtle_text_write_unicode(switchp -> label, switchX + switchp -> size, switchY, switchp -> size - 1, 0);
    } else if (switchp -> style == TT_SWITCH_STYLE_TRIANGLE) {
        if (switchp -> value) {
            turtle_tools_set_color(switchp -> color[TT_COLOR_SLOT_SWITCH_ON]);
            turtle_triangle(switchX - switchp -> size * 0.5, switchY - switchp -> size * 0.5, switchX + switchp -> size * 0.5, switchY - switchp -> size * 0.5, switchX + switchp -> size * 0.5, switchY + switchp -> size * 0.5);
        } else {
            turtle_tools_set_color(switchp -> color[TT_COLOR_SLOT_SWITCH_OFF]);
            turtle_triangle(switchX - switchp -> size * 0.5, switchY - switchp -> size * 0.5, switchX + switchp -> size * 0.3, switchY, switchX - switchp -> size * 0.5, switchY + switchp -> size * 0.5);
        }
        switchClickLeft = switchX - switchp -> size * 0.6;
        switchClickRight = switchX + switchp -> size * 0.6;
        switchClickDown = switchY - switchp -> size * 0.6;
        switchClickUp = switchY + switchp -> size * 0.6;
    }
    /* mouse */
    if (switchp -> enabled != TT_ELEMENT_ENABLED || turtleToolsGlobals.elementLogicTypeOld > switchp -> priority || (turtleToolsGlobals.elementLogicTypeOld == switchp -> priority && turtleToolsGlobals.elementLogicIndexOld > turtleToolsGlobals.elementLogicTemp)) {
        /* switch not enabled or higher priority element is being interacted with */
        if (switchp -> status != TT_STATUS_BLOCKED) {
            switchp -> status = TT_STATUS_IDLE;
        }
        goto LABEL_SWITCH_END;
    }
    LABEL_SWITCH_CHECK_HOVER:
    if (switchp -> status != TT_STATUS_CLICK && switchp -> status != TT_STATUS_BLOCKED && switchp -> status != TT_STATUS_CLICK_FIRST_TICK) {
        if (turtle.mouseX > switchClickLeft && turtle.mouseX < switchClickRight && turtle.mouseY > switchClickDown && turtle.mouseY < switchClickUp) {
            if (switchp -> status == TT_STATUS_HOVER || switchp -> status == TT_STATUS_HOVER_FIRST_TICK) {
                /* hovering switch */
                switchp -> status = TT_STATUS_HOVER;
            } else {
                /* first tick hover */
                switchp -> status = TT_STATUS_HOVER_FIRST_TICK;
            }
        } else {
            switchp -> status = TT_STATUS_IDLE;
        }
    }
    if (turtle_mouse_down()) {
        if (switchp -> status == TT_STATUS_HOVER || switchp -> status == TT_STATUS_HOVER_FIRST_TICK) {
            /* first tick clicked */
            switchp -> status = TT_STATUS_CLICK_FIRST_TICK;
        } else if (switchp -> status == TT_STATUS_CLICK || switchp -> status == TT_STATUS_CLICK_FIRST_TICK) {
            /* switch is being held */
            switchp -> status = TT_STATUS_CLICK;
        } else {
            /* switch is blocked from interaction until mouse is unclicked */
            switchp -> status = TT_STATUS_BLOCKED;
        }
    } else {
        if (switchp -> status == TT_STATUS_CLICK || switchp -> status == TT_STATUS_BLOCKED || switchp -> status == TT_STATUS_CLICK_FIRST_TICK) {
            /* first tick unclicked */
            switchp -> status = TT_STATUS_IDLE;
            goto LABEL_SWITCH_CHECK_HOVER; // done to avoid a single IDLE tick if mouse is hovering over switch when unclicked
        }
    }
    if (switchp -> status == TT_STATUS_CLICK_FIRST_TICK) {
        switchp -> value = !switchp -> value;
    }
    if (switchp -> status == TT_STATUS_HOVER || switchp -> status == TT_STATUS_CLICK || switchp -> status == TT_STATUS_HOVER_FIRST_TICK || switchp -> status == TT_STATUS_CLICK_FIRST_TICK) {
        turtleToolsGlobals.elementLogicType = switchp -> priority;
        turtleToolsGlobals.elementLogicIndex = turtleToolsGlobals.elementLogicTemp;
    }
    LABEL_SWITCH_END:
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
        if (dialp -> status != TT_STATUS_BLOCKED) {
            dialp -> status = TT_STATUS_IDLE;
        }
        return;
    }
    turtle_tools_set_color(dialp -> color[TT_COLOR_SLOT_DIAL_TEXT]);
    turtle_text_write_unicode(dialp -> label, dialp -> x, dialp -> y + 1.9 * dialp -> size, dialp -> size - 1, 50);
    double dialX = dialp -> x;
    double dialY = dialp -> y;
    turtle_pen_size(dialp -> size / 10);
    double circleSize = dialp -> size * 0.9;
    turtle_tools_set_color(dialp -> color[TT_COLOR_SLOT_DIAL]);
    /* draw circle */
    int32_t bezierPrezCurrent = (int32_t) ceil(sqrt(dialp -> size * turtleText.bezierPrez * 3));
    if (dialp -> style == TT_DIAL_STYLE_CLASSIC) {
        turtle_goto(dialX, dialY + circleSize);
        turtle_pen_down();
        double theta = 0;
        for (int32_t i = 0; i < bezierPrezCurrent; i++) {
            turtle_goto(dialX + circleSize * sin(theta), dialY + circleSize * cos(theta));
            theta += M_PI * 2 / bezierPrezCurrent;
        }
        turtle_goto(dialX, dialY + circleSize);
    } else if (dialp -> style == TT_DIAL_STYLE_SPEEDOMETER) {
        double theta = -135 / 57.2958;
        turtle_goto(dialX + circleSize * sin(theta), dialY + circleSize * cos(theta));
        turtle_pen_down();
        for (int32_t i = 0; i < bezierPrezCurrent; i++) {
            turtle_goto(dialX + circleSize * sin(theta), dialY + circleSize * cos(theta));
            theta += M_PI * 2 / bezierPrezCurrent * 0.75;
        }
        turtle_goto(dialX + circleSize * sin(theta), dialY + circleSize * cos(theta));
    }
    turtle_pen_up();
    turtle_goto(dialX, dialY);
    turtle_tools_set_color(dialp -> color[TT_COLOR_SLOT_DIAL]);
    turtle_pen_down();
    double dialAngle = 0.0;
    if (dialp -> style == TT_DIAL_STYLE_CLASSIC) {
        if (dialp -> scale == TT_DIAL_SCALE_LOG) {
            dialAngle = pow(361, ((double) dialp -> value - dialp -> range[0]) / (dialp -> range[1] - dialp -> range[0])) - 1;
        } else if (dialp -> scale == TT_DIAL_SCALE_LINEAR) {
            dialAngle = ((double) dialp -> value - dialp -> range[0]) / (dialp -> range[1] - dialp -> range[0]) * 360;
        } else if (dialp -> scale == TT_DIAL_SCALE_EXP) {
            dialAngle = 360 * (log((((double) dialp -> value - dialp -> range[0]) / (dialp -> range[1] - dialp -> range[0])) * 360 + 1) / log(361));
        }
    } else if (dialp -> style == TT_DIAL_STYLE_SPEEDOMETER) {
        if (dialp -> scale == TT_DIAL_SCALE_LOG) {
            dialAngle = pow(271, ((double) dialp -> value - dialp -> range[0]) / (dialp -> range[1] - dialp -> range[0])) - 1 - 135;
        } else if (dialp -> scale == TT_DIAL_SCALE_LINEAR) {
            dialAngle = ((double) dialp -> value - dialp -> range[0]) / (dialp -> range[1] - dialp -> range[0]) * 270 - 135;
        } else if (dialp -> scale == TT_DIAL_SCALE_EXP) {
            dialAngle = 270 * (log((((double) dialp -> value - dialp -> range[0]) / (dialp -> range[1] - dialp -> range[0])) * 270 + 1) / log(271)) - 135;
        }
    }
    turtle_goto(dialX + sin(dialAngle / 57.2958) * dialp -> size, dialY + cos(dialAngle / 57.2958) * dialp -> size);
    turtle_pen_up();
    /* mouse */
    if (dialp -> enabled != TT_ELEMENT_ENABLED || turtleToolsGlobals.elementLogicTypeOld > dialp -> priority || (turtleToolsGlobals.elementLogicTypeOld == dialp -> priority && turtleToolsGlobals.elementLogicIndexOld > turtleToolsGlobals.elementLogicTemp)) {
        /* dial not enabled or higher priority element is being interacted with */
        if (dialp -> status != TT_STATUS_BLOCKED) {
            dialp -> status = TT_STATUS_IDLE;
        }
        goto LABEL_DIAL_END;
    }
    LABEL_DIAL_CHECK_HOVER:
    if (dialp -> status != TT_STATUS_CLICK && dialp -> status != TT_STATUS_BLOCKED && dialp -> status != TT_STATUS_CLICK_FIRST_TICK) {
        if (turtle.mouseX > dialX - dialp -> size && turtle.mouseX < dialX + dialp -> size && turtle.mouseY > dialY - dialp -> size && turtle.mouseY < dialY + dialp -> size) {
            if (dialp -> status == TT_STATUS_HOVER || dialp -> status == TT_STATUS_HOVER_FIRST_TICK) {
                /* hovering dial */
                dialp -> status = TT_STATUS_HOVER;
            } else {
                /* first tick hover */
                dialp -> status = TT_STATUS_HOVER_FIRST_TICK;
            }
        } else {
            dialp -> status = TT_STATUS_IDLE;
        }
    }
    if (turtle_mouse_down()) {
        if (dialp -> status == TT_STATUS_HOVER || dialp -> status == TT_STATUS_HOVER_FIRST_TICK) {
            /* first tick clicked */
            dialp -> status = TT_STATUS_CLICK_FIRST_TICK;
            turtleToolsGlobals.dialAnchorX = dialX;
            turtleToolsGlobals.dialAnchorY = dialY;
            dialp -> mouseAnchor = turtle.mouseX - dialX;
        } else if (dialp -> status == TT_STATUS_CLICK || dialp -> status == TT_STATUS_CLICK_FIRST_TICK) {
            /* dial is being held */
            dialp -> status = TT_STATUS_CLICK;
        } else {
            /* dial is blocked from interaction until mouse is unclicked */
            dialp -> status = TT_STATUS_BLOCKED;
        }
    } else if (turtle_mouse_right()) {
        if (dialp -> status == TT_STATUS_HOVER || dialp -> status == TT_STATUS_HOVER_FIRST_TICK) {
            /* first tick right clicked */
            dialp -> value = dialp -> defaultValue;
            dialp -> status = TT_STATUS_BLOCKED;
        }
    } else {
        if (dialp -> status == TT_STATUS_CLICK || dialp -> status == TT_STATUS_BLOCKED || dialp -> status == TT_STATUS_CLICK_FIRST_TICK) {
            /* first tick unclicked */
            dialp -> status = TT_STATUS_IDLE;
            goto LABEL_DIAL_CHECK_HOVER; // done to avoid a single IDLE tick if mouse is hovering over dial when unclicked
        }
    }
    if (dialp -> status == TT_STATUS_CLICK || dialp -> status == TT_STATUS_CLICK_FIRST_TICK) {
        dialAngle = tt_angleBetween(turtleToolsGlobals.dialAnchorX, turtleToolsGlobals.dialAnchorY, turtle.mouseX, turtle.mouseY);
        if (dialp -> style == TT_DIAL_STYLE_CLASSIC) {
            if (turtle.mouseY < turtleToolsGlobals.dialAnchorY) {
                dialp -> mouseAnchor = turtle.mouseX - dialX;
            }
            if ((dialAngle < 0.000000001 || dialAngle > 180) && turtle.mouseY > turtleToolsGlobals.dialAnchorY && dialp -> mouseAnchor >= 0) {
                dialAngle = 0.000000001;
            }
            if ((dialAngle > 359.99999999 || dialAngle < 180) && turtle.mouseY > turtleToolsGlobals.dialAnchorY && dialp -> mouseAnchor < 0) {
                dialAngle = 359.99999999;
            }
            if (dialp -> scale == TT_DIAL_SCALE_LOG) {
                dialp -> value = round(dialp -> range[0] + (dialp -> range[1] - dialp -> range[0]) * (log(1 + dialAngle) / log(361)));
            } else if (dialp -> scale == TT_DIAL_SCALE_LINEAR) {
                dialp -> value = round(dialp -> range[0] + ((dialp -> range[1] - dialp -> range[0]) * dialAngle / 360));
            } else if (dialp -> scale == TT_DIAL_SCALE_EXP) {
                dialp -> value = round(dialp -> range[0] + (dialp -> range[1] - dialp -> range[0]) * ((pow(361, dialAngle / 360) - 1) / 360));
            }
        } else if (dialp -> style == TT_DIAL_STYLE_SPEEDOMETER) {
            if (turtle.mouseY > turtleToolsGlobals.dialAnchorY) {
                dialp -> mouseAnchor = turtle.mouseX - dialX;
            }
            if (dialAngle < 225 && turtle.mouseY < turtleToolsGlobals.dialAnchorY && dialp -> mouseAnchor < 0) {
                dialAngle = 225.000000001;
            }
            if (dialAngle > 135 && turtle.mouseY < turtleToolsGlobals.dialAnchorY && dialp -> mouseAnchor >= 0) {
                dialAngle = 134.99999999;
            }
            dialAngle += 135;
            if (dialAngle > 360) {
                dialAngle -= 360;
            }
            if (dialp -> scale == TT_DIAL_SCALE_LOG) {
                dialp -> value = round(dialp -> range[0] + (dialp -> range[1] - dialp -> range[0]) * (log(1 + dialAngle) / log(271)));
            } else if (dialp -> scale == TT_DIAL_SCALE_LINEAR) {
                dialp -> value = round(dialp -> range[0] + ((dialp -> range[1] - dialp -> range[0]) * dialAngle / 270));
            } else if (dialp -> scale == TT_DIAL_SCALE_EXP) {
                dialp -> value = round(dialp -> range[0] + (dialp -> range[1] - dialp -> range[0]) * ((pow(271, dialAngle / 270) - 1) / 270));
            }
        }
    }
    if (dialp -> status == TT_STATUS_HOVER || dialp -> status == TT_STATUS_CLICK || dialp -> status == TT_STATUS_HOVER_FIRST_TICK || dialp -> status == TT_STATUS_CLICK_FIRST_TICK) {
        turtleToolsGlobals.elementLogicType = dialp -> priority;
        turtleToolsGlobals.elementLogicIndex = turtleToolsGlobals.elementLogicTemp;
    }
    LABEL_DIAL_END:
    if (dialp -> renderMultiplier != 0) {
        turtle_tools_set_color(dialp -> color[TT_COLOR_SLOT_DIAL_TEXT]);
        if (dialp -> style == TT_DIAL_STYLE_CLASSIC) {
            turtle_text_write_stringf(dialX + dialp -> size * 1.3, dialY, dialp -> size / 2, 0, dialp -> render, dialp -> value * dialp -> renderMultiplier);
        } else if (dialp -> style == TT_DIAL_STYLE_SPEEDOMETER) {
            turtle_text_write_stringf(dialX, dialY - dialp -> size * 1.2, dialp -> size / 2, 50, dialp -> render, dialp -> value * dialp -> renderMultiplier);
        }
    }
    if (dialp -> variable != NULL) {
        *dialp -> variable = dialp -> value;
    }
}

void tt_sliderUpdate(tt_slider_t *sliderp) {
    if (sliderp -> variable != NULL) {
        sliderp -> value = *sliderp -> variable;
    }
    if (sliderp -> enabled == TT_ELEMENT_HIDE) {
        if (sliderp -> status != TT_STATUS_BLOCKED) {
            sliderp -> status = TT_STATUS_IDLE;
        }
        return;
    }
    double sliderXLeft = 0;
    double sliderYLeft = 0;
    double sliderXRight = 0;
    double sliderYRight = 0;
    double sliderAlignFactor = 0;
    double sliderRotateFactor = 0;
    double sliderOffsetXFactor = 0;
    double sliderOffsetYFactor = 0;
    double sliderOffsetXFactorSmall = 0;
    double sliderOffsetYFactorSmall = 0;
    if (sliderp -> type == TT_SLIDER_TYPE_HORIZONTAL) {
        if (sliderp -> style != TT_SLIDER_STYLE_SIDESWIPE) {
            sliderOffsetYFactor = 1.4 * sliderp -> size;
            sliderOffsetYFactorSmall = -sliderp -> size * 1.2;
        }
        if (sliderp -> align == TT_SLIDER_ALIGN_LEFT) {
            sliderXLeft = sliderp -> x;
            sliderYLeft = sliderp -> y;
            sliderXRight = sliderp -> x + sliderp -> length;
            sliderYRight = sliderp -> y;
            if (sliderp -> style == TT_SLIDER_STYLE_SIDESWIPE) {
                sliderAlignFactor = 100;
                sliderOffsetXFactor = -sliderp -> size;
                sliderOffsetXFactorSmall = sliderp -> length + sliderp -> size;
            } else {
                sliderAlignFactor = 0;
                sliderOffsetXFactor = -sliderp -> size * 0.4;
                sliderOffsetXFactorSmall = -sliderp -> size * 0.25;
            }
        } else if (sliderp -> align == TT_SLIDER_ALIGN_CENTER) {
            sliderXLeft = sliderp -> x - sliderp -> length / 2;
            sliderYLeft = sliderp -> y;
            sliderXRight = sliderp -> x + sliderp -> length / 2;
            sliderYRight = sliderp -> y;
            if (sliderp -> style == TT_SLIDER_STYLE_SIDESWIPE) {
                sliderAlignFactor = 100;
                sliderOffsetXFactor = -sliderp -> length / 2 - sliderp -> size;
                sliderOffsetXFactorSmall = sliderp -> length / 2 + sliderp -> size;
            } else {
                sliderAlignFactor = 50;
            }
        } else if (sliderp -> align == TT_SLIDER_ALIGN_RIGHT) {
            sliderXLeft = sliderp -> x - sliderp -> length;
            sliderYLeft = sliderp -> y;
            sliderXRight = sliderp -> x;
            sliderYRight = sliderp -> y;
            if (sliderp -> style == TT_SLIDER_STYLE_SIDESWIPE) {
                sliderAlignFactor = 0;
                sliderOffsetXFactor = sliderp -> size;
                sliderOffsetXFactorSmall = -sliderp -> length - sliderp -> size;
            } else {
                sliderAlignFactor = 100;
                sliderOffsetXFactor = sliderp -> size * 0.4;
                sliderOffsetXFactorSmall = sliderp -> size * 0.25;
            }
        }
    } else if (sliderp -> type == TT_SLIDER_TYPE_VERTICAL) {
        sliderOffsetYFactor = 1.4 * sliderp -> size + sliderp -> length / 2;
        if (sliderp -> align == TT_SLIDER_ALIGN_LEFT) {
            sliderXLeft = sliderp -> x;
            sliderYLeft = sliderp -> y - sliderp -> length / 2;
            sliderXRight = sliderp -> x;
            sliderYRight = sliderp -> y + sliderp -> length / 2;
            if (sliderp -> style == TT_SLIDER_STYLE_SIDESWIPE) {
                sliderAlignFactor = 100;
                sliderRotateFactor = 90;
                sliderOffsetYFactor = sliderp -> length / 2 + sliderp -> size;
                sliderOffsetYFactorSmall = -sliderp -> length / 2 - sliderp -> size;
            } else if (sliderp -> style == TT_SLIDER_STYLE_COMPACT) {
                sliderAlignFactor = 50;
                sliderRotateFactor = -90;
                sliderOffsetYFactor = 0;
                sliderOffsetXFactor = -sliderp -> size * 1.4;
                sliderOffsetYFactorSmall = -1.2 * sliderp -> size - sliderp -> length / 2;
            } else {
                sliderAlignFactor = 0;
                sliderOffsetXFactor = -sliderp -> size * 0.4;
                sliderOffsetXFactorSmall = sliderp -> size * 1;
            }
        } else if (sliderp -> align == TT_SLIDER_ALIGN_CENTER) {
            sliderXLeft = sliderp -> x;
            sliderYLeft = sliderp -> y - sliderp -> length / 2;
            sliderXRight = sliderp -> x;
            sliderYRight = sliderp -> y + sliderp -> length / 2;
            if (sliderp -> style == TT_SLIDER_STYLE_SIDESWIPE) {
                sliderAlignFactor = 100;
                sliderRotateFactor = 90;
                sliderOffsetYFactor = sliderp -> length / 2 + sliderp -> size;
                sliderOffsetYFactorSmall = -sliderp -> length / 2 - sliderp -> size;
            } else if (sliderp -> style == TT_SLIDER_STYLE_COMPACT) {
                sliderAlignFactor = 50;
                sliderRotateFactor = -90;
                sliderOffsetYFactor = 0;
                sliderOffsetXFactor = -sliderp -> size * 1.4;
                sliderOffsetYFactorSmall = -1.2 * sliderp -> size - sliderp -> length / 2;
            } else {
                sliderAlignFactor = 50;
                sliderOffsetYFactorSmall = -1.2 * sliderp -> size - sliderp -> length / 2;
            }
        } else if (sliderp -> align == TT_SLIDER_ALIGN_RIGHT) {
            sliderXLeft = sliderp -> x;
            sliderYLeft = sliderp -> y - sliderp -> length / 2;
            sliderXRight = sliderp -> x;
            sliderYRight = sliderp -> y + sliderp -> length / 2;
            if (sliderp -> style == TT_SLIDER_STYLE_SIDESWIPE) {
                sliderAlignFactor = 0;
                sliderRotateFactor = 90; // -90
                sliderOffsetYFactor = -sliderp -> length / 2 - sliderp -> size;
                sliderOffsetYFactorSmall = sliderp -> length / 2 + sliderp -> size;
            } else if (sliderp -> style == TT_SLIDER_STYLE_COMPACT) {
                sliderAlignFactor = 50;
                sliderRotateFactor = 90;
                sliderOffsetYFactor = 0;
                sliderOffsetXFactor = sliderp -> size * 1.4;
                sliderOffsetYFactorSmall = -1.2 * sliderp -> size - sliderp -> length / 2;
            } else {
                sliderAlignFactor = 100;
                sliderOffsetXFactor = sliderp -> size * 0.4;
                sliderOffsetXFactorSmall = -sliderp -> size * 1;
            }
        }
    }
    turtle_tools_set_color(sliderp -> color[TT_COLOR_SLOT_SLIDER_TEXT]);
    if (sliderRotateFactor != 0) {
        turtle_text_write_unicode_rotated(sliderp -> label, sliderp -> x + sliderOffsetXFactor, sliderp -> y + sliderOffsetYFactor, sliderp -> size - 1, sliderAlignFactor, sliderRotateFactor);
    } else {
        turtle_text_write_unicode(sliderp -> label, sliderp -> x + sliderOffsetXFactor, sliderp -> y + sliderOffsetYFactor, sliderp -> size - 1, sliderAlignFactor);
    }
    if (sliderp -> style == TT_SLIDER_STYLE_SIDESWIPE) {
        sliderAlignFactor = 100 - sliderAlignFactor;
    }
    if (sliderp -> style == TT_SLIDER_STYLE_COMPACT) {
        sliderRotateFactor = 0;
    }
    turtle_pen_size(sliderp -> size * 1.2);
    turtle_goto(sliderXLeft, sliderYLeft);
    turtle_tools_set_color(sliderp -> color[TT_COLOR_SLOT_SLIDER_BAR]);
    turtle_pen_down();
    turtle_goto(sliderXRight, sliderYRight);
    turtle_pen_up();
    turtle_pen_size(sliderp -> size);
    turtle_tools_set_color(sliderp -> color[TT_COLOR_SLOT_SLIDER_CIRCLE]);
    if (sliderp -> type == TT_SLIDER_TYPE_HORIZONTAL) {
        if (sliderp -> scale == TT_SLIDER_SCALE_LINEAR) {
            turtle_goto(sliderXLeft + (sliderXRight - sliderXLeft) * (sliderp -> value - sliderp -> range[0]) / (sliderp -> range[1] - sliderp -> range[0]), sliderYLeft);
        } else if (sliderp -> scale == TT_SLIDER_SCALE_LOG) {
            turtle_goto(sliderXLeft + pow(sliderXRight - sliderXLeft + 1, ((double) sliderp -> value - sliderp -> range[0]) / (sliderp -> range[1] - sliderp -> range[0])) - 1, sliderYLeft);
        } else if (sliderp -> scale == TT_SLIDER_SCALE_EXP) {
            turtle_goto(sliderXLeft + (sliderXRight - sliderXLeft) * (log((((double) sliderp -> value - sliderp -> range[0]) / (sliderp -> range[1] - sliderp -> range[0])) * (sliderXRight - sliderXLeft) + 1) / log((sliderXRight - sliderXLeft) + 1)), sliderYLeft);
        }
    } else if (sliderp -> type == TT_SLIDER_TYPE_VERTICAL) {
        if (sliderp -> scale == TT_SLIDER_SCALE_LINEAR) {
            turtle_goto(sliderXLeft, sliderYLeft + (sliderYRight - sliderYLeft) * (sliderp -> value - sliderp -> range[0]) / (sliderp -> range[1] - sliderp -> range[0]));
        } else if (sliderp -> scale == TT_SLIDER_SCALE_LOG) {
            turtle_goto(sliderXLeft, sliderYLeft + pow(sliderYRight - sliderYLeft + 1, ((double) sliderp -> value - sliderp -> range[0]) / (sliderp -> range[1] - sliderp -> range[0])) - 1);
        } else if (sliderp -> scale == TT_SLIDER_SCALE_EXP) {
            turtle_goto(sliderXLeft, sliderYLeft + (sliderYRight - sliderYLeft) * (log((((double) sliderp -> value - sliderp -> range[0]) / (sliderp -> range[1] - sliderp -> range[0])) * (sliderYRight - sliderYLeft) + 1) / log((sliderYRight - sliderYLeft) + 1)));
        }
    }
    turtle_pen_down();
    turtle_pen_up();
    /* mouse */
    if (sliderp -> enabled != TT_ELEMENT_ENABLED || turtleToolsGlobals.elementLogicTypeOld > sliderp -> priority || (turtleToolsGlobals.elementLogicTypeOld == sliderp -> priority && turtleToolsGlobals.elementLogicIndexOld > turtleToolsGlobals.elementLogicTemp)) {
        /* slider not enabled or higher priority element is being interacted with */
        if (sliderp -> status != TT_STATUS_BLOCKED) {
            sliderp -> status = TT_STATUS_IDLE;
        }
        goto LABEL_SLIDER_END;
    }
    LABEL_SLIDER_CHECK_HOVER:
    if (sliderp -> status != TT_STATUS_CLICK && sliderp -> status != TT_STATUS_BLOCKED && sliderp -> status != TT_STATUS_CLICK_FIRST_TICK) {
        if (turtle.mouseX > sliderXLeft - sliderp -> size * 0.6 && turtle.mouseX < sliderXRight + sliderp -> size * 0.6 && turtle.mouseY > sliderYLeft - sliderp -> size * 0.6 && turtle.mouseY < sliderYRight + sliderp -> size * 0.6) {
            if (sliderp -> status == TT_STATUS_HOVER || sliderp -> status == TT_STATUS_HOVER_FIRST_TICK) {
                /* hovering slider */
                sliderp -> status = TT_STATUS_HOVER;
            } else {
                /* first tick hover */
                sliderp -> status = TT_STATUS_HOVER_FIRST_TICK;
            }
        } else {
            sliderp -> status = TT_STATUS_IDLE;
        }
    }
    if (turtle_mouse_down()) {
        if (sliderp -> status == TT_STATUS_HOVER || sliderp -> status == TT_STATUS_HOVER_FIRST_TICK) {
            /* first tick clicked */
            sliderp -> status = TT_STATUS_CLICK_FIRST_TICK;
        } else if (sliderp -> status == TT_STATUS_CLICK || sliderp -> status == TT_STATUS_CLICK_FIRST_TICK) {
            /* slider is being held */
            sliderp -> status = TT_STATUS_CLICK;
        } else {
            /* slider is blocked from interaction until mouse is unclicked */
            sliderp -> status = TT_STATUS_BLOCKED;
        }
    } else if (turtle_mouse_right()) {
        if (sliderp -> status == TT_STATUS_HOVER || sliderp -> status == TT_STATUS_HOVER_FIRST_TICK) {
            /* first tick right clicked */
            sliderp -> value = sliderp -> defaultValue;
            sliderp -> status = TT_STATUS_BLOCKED;
        }
    } else {
        if (sliderp -> status == TT_STATUS_CLICK || sliderp -> status == TT_STATUS_BLOCKED || sliderp -> status == TT_STATUS_CLICK_FIRST_TICK) {
            /* first tick unclicked */
            sliderp -> status = TT_STATUS_IDLE;
            goto LABEL_SLIDER_CHECK_HOVER; // done to avoid a single IDLE tick if mouse is hovering over slider when unclicked
        }
    }
    if (sliderp -> status == TT_STATUS_CLICK || sliderp -> status == TT_STATUS_CLICK_FIRST_TICK) {
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
    if (sliderp -> status == TT_STATUS_HOVER || sliderp -> status == TT_STATUS_CLICK || sliderp -> status == TT_STATUS_HOVER_FIRST_TICK || sliderp -> status == TT_STATUS_CLICK_FIRST_TICK) {
        turtleToolsGlobals.elementLogicType = sliderp -> priority;
        turtleToolsGlobals.elementLogicIndex = turtleToolsGlobals.elementLogicTemp;
    }
    LABEL_SLIDER_END:
    if (sliderp -> renderMultiplier != 0) {
        turtle_tools_set_color(sliderp -> color[TT_COLOR_SLOT_SLIDER_TEXT]);
        if (sliderRotateFactor != 0) {
            turtle_text_write_stringf_rotated(sliderp -> x + sliderOffsetXFactorSmall, sliderp -> y + sliderOffsetYFactorSmall, sliderp -> size / 2, sliderAlignFactor, sliderRotateFactor, sliderp -> render, sliderp -> value * sliderp -> renderMultiplier);
        } else {
            turtle_text_write_stringf(sliderp -> x + sliderOffsetXFactorSmall, sliderp -> y + sliderOffsetYFactorSmall, sliderp -> size / 2, sliderAlignFactor, sliderp -> render, sliderp -> value * sliderp -> renderMultiplier);
        }
    }
    if (sliderp -> variable != NULL) {
        *sliderp -> variable = sliderp -> value;
    }
}

void tt_textboxAddKey(tt_textbox_t *textboxp, int32_t key) {
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
    if (textboxp -> editIndexLength != 0) {
        if (textboxp -> editIndexLength < 0) {
            textboxp -> editIndex += textboxp -> editIndexLength;
            textboxp -> editIndexLength *= -1;
        }
        strdel(textboxp -> text, textboxp -> editIndex, textboxp -> editIndexLength);
        textboxp -> editIndexLength = 0;
    }
    int32_t len = strlen(textboxp -> text);
    if (len < textboxp -> maxCharacters) {
        strins(textboxp -> text, (char *) buffer, textboxp -> editIndex);
        textboxp -> editIndex += size;
    }
}

void tt_textboxUnicodeCallback(uint32_t codepoint) {
    for (int32_t i = 0; i < turtleToolsElements.textboxes -> length; i++) {
        tt_textbox_t *textboxp = (tt_textbox_t *) (turtleToolsElements.textboxes -> data[i].p);
        if (textboxp -> status == TT_STATUS_CLICK || textboxp -> status == TT_STATUS_OPEN || textboxp -> status == TT_STATUS_CLICK_FIRST_TICK || textboxp -> status == TT_STATUS_OPEN_FIRST_TICK) {
            tt_textboxAddKey(textboxp, codepoint);
            break;
        }
    }
}

void tt_textboxHandleOtherKey(tt_textbox_t *textboxp, int32_t key) {
    int32_t len = strlen(textboxp -> text);
    if (key == GLFW_KEY_A && turtle_key_pressed(GLFW_KEY_LEFT_CONTROL)) {
        /* select all */
        textboxp -> editIndex = 0;
        textboxp -> editIndexLength = strlen(textboxp -> text);
        return;
    }
    if (key == GLFW_KEY_BACKSPACE) {
        if (textboxp -> editIndex < 0 || (textboxp -> editIndex == 0 && textboxp -> editIndexLength == 0)) {
            return;
        }
        if (textboxp -> editIndexLength == 0) {
            int32_t size = 1;
            if (textboxp -> text[textboxp -> editIndex - 1] & 0b10000000) {
                while ((textboxp -> text[textboxp -> editIndex - 1] & 0b01000000) == 0) {
                    textboxp -> editIndex--;
                    size++;
                }
            }
            textboxp -> editIndex--;
            strdel(textboxp -> text, textboxp -> editIndex, size);
        } else {
            if (textboxp -> editIndexLength < 0) {
                textboxp -> editIndex += textboxp -> editIndexLength;
                textboxp -> editIndexLength *= -1;
            }
            strdel(textboxp -> text, textboxp -> editIndex, textboxp -> editIndexLength);
            textboxp -> editIndexLength = 0;
        }
    } else if (key == GLFW_KEY_DELETE) {
        if (textboxp -> editIndex >= len) {
            return;
        }
        if (textboxp -> editIndexLength == 0) {
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
        } else {
            if (textboxp -> editIndexLength < 0) {
                textboxp -> editIndex += textboxp -> editIndexLength;
                textboxp -> editIndexLength *= -1;
            }
            strdel(textboxp -> text, textboxp -> editIndex, textboxp -> editIndexLength);
            textboxp -> editIndexLength = 0;
            }
    } else if (key == GLFW_KEY_ENTER) {
        textboxp -> status = TT_STATUS_IDLE;
    } else if (key == GLFW_KEY_LEFT) {
        textboxp -> count = 1;
        if (textboxp -> editIndex <= 0) {
            return;
        }
        if (turtle_key_pressed(GLFW_KEY_LEFT_SHIFT) || turtle_key_pressed(GLFW_KEY_RIGHT_SHIFT)) {
            if (textboxp -> editIndex + textboxp -> editIndexLength > 0) {
                if (textboxp -> text[textboxp -> editIndex + textboxp -> editIndexLength - 1] & 0b10000000) {
                    while ((textboxp -> text[textboxp -> editIndex + textboxp -> editIndexLength - 1] & 0b01000000) == 0) {
                        textboxp -> editIndexLength--;
                    }
                }
                textboxp -> editIndexLength--;
            }
        } else {
            if (textboxp -> editIndexLength != 0) {
                if (textboxp -> editIndexLength < 0) {
                    textboxp -> editIndex += textboxp -> editIndexLength;
                }
                textboxp -> editIndexLength = 0;
            } else {
                if (textboxp -> text[textboxp -> editIndex - 1] & 0b10000000) {
                    while ((textboxp -> text[textboxp -> editIndex - 1] & 0b01000000) == 0) {
                        textboxp -> editIndex--;
                    }
                }
                textboxp -> editIndex--;
            }
        }
    } else if (key == GLFW_KEY_RIGHT) {
        textboxp -> count = 1;
        if (textboxp -> editIndex >= len) {
            textboxp -> editIndexLength = 0;
            return;
        }
        if (turtle_key_pressed(GLFW_KEY_LEFT_SHIFT) || turtle_key_pressed(GLFW_KEY_RIGHT_SHIFT)) {
            if (textboxp -> editIndex + textboxp -> editIndexLength < strlen(textboxp -> text)) {
                if (textboxp -> text[textboxp -> editIndex + textboxp -> editIndexLength] & 0b10000000) {
                    if (textboxp -> text[textboxp -> editIndex + textboxp -> editIndexLength] & 0b00100000) {
                        if (textboxp -> text[textboxp -> editIndex + textboxp -> editIndexLength] & 0b00010000) {
                            textboxp -> editIndexLength++;
                        }
                        textboxp -> editIndexLength++;
                    }
                    textboxp -> editIndexLength++;
                }
                textboxp -> editIndexLength++;
            }
        } else {
            if (textboxp -> editIndexLength != 0) {
                if (textboxp -> editIndexLength > 0) {
                    textboxp -> editIndex += textboxp -> editIndexLength;
                }
                textboxp -> editIndexLength = 0;
            } else {
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
    }
}

void tt_textboxKeyCallback(int32_t key, int32_t scancode, int32_t action) {
    /* non-printable keys */
    if (action == GLFW_PRESS) {
        for (int32_t i = 0; i < turtleToolsElements.textboxes -> length; i++) {
            tt_textbox_t *textboxp = (tt_textbox_t *) (turtleToolsElements.textboxes -> data[i].p);
            if (textboxp -> status == TT_STATUS_CLICK || textboxp -> status == TT_STATUS_OPEN || textboxp -> status == TT_STATUS_CLICK_FIRST_TICK || textboxp -> status == TT_STATUS_OPEN_FIRST_TICK) {
                textboxp -> lastKey = key;
                textboxp -> keyTimeout = textboxp -> initialKeyTimeout;
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

int32_t tt_textboxCalculateIndexFromPosition(tt_textbox_t *textboxp, double position) {
    uint32_t textConverted[strlen(textboxp -> text) + 1];
    uint32_t characterLength = turtle_text_convert_unicode(textboxp -> text + textboxp -> renderStartingIndex, textConverted);
    int32_t index;
    double startingPx = position - (textboxp -> x + textboxp -> renderPixelOffset + textboxp -> size / 10 + turtle_text_get_length(textConverted, 1, textboxp -> size - 1) / 2);
    if (startingPx > 0) {
        double dummy;
        index = tt_textboxCalculateMaximumCharacters(textConverted, characterLength, textboxp -> size - 1, startingPx, -1, &dummy) + textboxp -> renderStartingIndex;
    } else {
        index = textboxp -> renderStartingIndex;
    }
    if (index < 0) {
        index = 0;
    }
    if (index > strlen(textboxp -> text)) {
        index = strlen(textboxp -> text);
    }
    return index;
}

void tt_textboxUpdate(tt_textbox_t *textboxp) {
    if (textboxp -> enabled == TT_ELEMENT_HIDE) {
        if (textboxp -> status != TT_STATUS_BLOCKED) {
            textboxp -> status = TT_STATUS_IDLE;
        }
        textboxp -> mouseOver = 0;
        return;
    }
    /* handle keys */
    if (textboxp -> keyTimeout > 0) {
        textboxp -> keyTimeout--;
    }
    if (textboxp -> lastKey > 0) {
        if (turtle_key_pressed(textboxp -> lastKey)) {
            if (textboxp -> keyTimeout == 0) {
                textboxp -> keyTimeout = textboxp -> heldKeyTimeout;
                tt_textboxHandleOtherKey(textboxp, textboxp -> lastKey);
            }
        } else {
            textboxp -> lastKey = 0;
        }
    }
    if (textboxp -> count > 0) {
        textboxp -> count++;
        if (textboxp -> count > textboxp -> linePeriod) {
            textboxp -> count = 1;
        }
    }
    turtle_tools_set_color(textboxp -> color[TT_COLOR_SLOT_TEXTBOX_BOX]);
    turtle_rectangle(textboxp -> x, textboxp -> y - textboxp -> size, textboxp -> x + textboxp -> length, textboxp -> y + textboxp -> size);
    if (textboxp -> status == TT_STATUS_IDLE || textboxp -> status == TT_STATUS_BLOCKED || textboxp -> status == TT_STATUS_HOVER || textboxp -> status == TT_STATUS_HOVER_FIRST_TICK) {
        textboxp -> renderPixelOffset = textboxp -> size / 3;
        textboxp -> renderStartingIndex = 0;
        /* textbox idle */
        if (strlen(textboxp -> text) == 0) {
            /* render label */
            textboxp -> renderNumCharacters = 0;
            turtle_tools_set_color(textboxp -> color[TT_COLOR_SLOT_TEXTBOX_PHANTOM_TEXT]);
            turtle_text_write_unicode(textboxp -> label, textboxp -> x + textboxp -> size / 2, textboxp -> y, textboxp -> size - 1, 0);
        } else {
            /* calculate rendered characters */
            double totalTextLength = turtle_text_get_unicode_length(textboxp -> text, textboxp -> size - 1);
            if (totalTextLength < textboxp -> length - textboxp -> size / 1.5) {
                textboxp -> renderNumCharacters = strlen(textboxp -> text);
            } else {
                /* not all characters fit in textbox - retract text length */
                uint32_t textConverted[strlen(textboxp -> text) + 1];
                uint32_t characterLength = turtle_text_convert_unicode(textboxp -> text, textConverted);
                double dummy;
                textboxp -> renderNumCharacters = tt_textboxCalculateMaximumCharacters(textConverted, characterLength, textboxp -> size - 1, textboxp -> length - textboxp -> size * 1.2, -1, &dummy);
            }
        }
    } else if (textboxp -> status == TT_STATUS_CLICK || textboxp -> status == TT_STATUS_OPEN || textboxp -> status == TT_STATUS_CLICK_FIRST_TICK || textboxp -> status == TT_STATUS_OPEN_FIRST_TICK) {
        /* editing text */
        /* calculate rendered characters */
        double totalTextLength = turtle_text_get_unicode_length(textboxp -> text, textboxp -> size - 1);
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
                uint32_t characterLength = turtle_text_convert_unicode(textboxp -> text + textboxp -> editIndex, textConverted);
                double dummy;
                textboxp -> renderNumCharacters = tt_textboxCalculateMaximumCharacters(textConverted, characterLength, textboxp -> size - 1, textboxp -> length - textboxp -> size * 1.2, -1, &dummy);
            } else if (textboxp -> editIndex > textboxp -> renderStartingIndex + textboxp -> renderNumCharacters || (strlen(textboxp -> text) < textboxp -> renderStartingIndex + textboxp -> renderNumCharacters && strlen(textboxp -> text) == textboxp -> editIndex)) {
                /* set editIndex at the right side of box */
                char tempHold;
                tempHold = textboxp -> text[textboxp -> editIndex];
                textboxp -> text[textboxp -> editIndex] = '\0';
                uint32_t textConverted[strlen(textboxp -> text) + 1];
                uint32_t characterLength = turtle_text_convert_unicode(textboxp -> text, textConverted);
                double textPixelLength;
                textboxp -> renderStartingIndex = strlen(textboxp -> text) + tt_textboxCalculateMaximumCharacters(textConverted, characterLength, textboxp -> size - 1, textboxp -> length - textboxp -> size * 1.2, 1, &textPixelLength);
                textboxp -> renderNumCharacters = strlen(textboxp -> text) - textboxp -> renderStartingIndex;
                textboxp -> renderPixelOffset = textboxp -> length - textboxp -> size / 3 - textPixelLength;
                textboxp -> text[textboxp -> editIndex] = tempHold;
            }
        }
    }
    /* draw text and occluding boxes */
    if (textboxp -> editIndexLength != 0) {
        double highlightLeft;
        double highlightRight;
        char tempHold;
        if (textboxp -> editIndex == textboxp -> renderStartingIndex) {
            highlightLeft = -textboxp -> size / 10; // i dont know why this is necessary
        } else {
            tempHold = textboxp -> text[textboxp -> editIndex];
            textboxp -> text[textboxp -> editIndex] = '\0';
            highlightLeft = turtle_text_get_unicode_length(textboxp -> text + textboxp -> renderStartingIndex, textboxp -> size - 1) + (textboxp -> editIndexLength > 0) * textboxp -> size / 10;
            textboxp -> text[textboxp -> editIndex] = tempHold;
        }
        
        if (textboxp -> editIndex + textboxp -> editIndexLength == textboxp -> renderStartingIndex) {
            highlightRight = -textboxp -> size / 10; // i dont know why this is necessary
        } else {
            tempHold = textboxp -> text[textboxp -> editIndex + textboxp -> editIndexLength];
            textboxp -> text[textboxp -> editIndex + textboxp -> editIndexLength] = '\0';
            highlightRight = turtle_text_get_unicode_length(textboxp -> text + textboxp -> renderStartingIndex, textboxp -> size - 1) + (textboxp -> editIndexLength < 0) * textboxp -> size / 10;
            textboxp -> text[textboxp -> editIndex + textboxp -> editIndexLength] = tempHold;
        }
        turtle_tools_set_color(textboxp -> color[TT_COLOR_SLOT_TEXTBOX_HIGHLIGHT]);
        turtle_rectangle(textboxp -> x + textboxp -> renderPixelOffset + highlightLeft, textboxp -> y - textboxp -> size * 0.8, textboxp -> x + textboxp -> renderPixelOffset + highlightRight, textboxp -> y + textboxp -> size * 0.8);
    }
    char tempHold;
    tempHold = textboxp -> text[textboxp -> renderStartingIndex + textboxp -> renderNumCharacters];
    textboxp -> text[textboxp -> renderStartingIndex + textboxp -> renderNumCharacters] = '\0';
    turtle_tools_set_color(textboxp -> color[TT_COLOR_SLOT_TEXTBOX_TEXT]);
    turtle_text_write_unicode(textboxp -> text + textboxp -> renderStartingIndex, textboxp -> x + textboxp -> renderPixelOffset, textboxp -> y, textboxp -> size - 1, 0);
    textboxp -> text[textboxp -> renderStartingIndex + textboxp -> renderNumCharacters] = tempHold;
    turtle_tools_set_color(textboxp -> color[TT_COLOR_SLOT_TEXTBOX_BOX]);
    turtle_rectangle(textboxp -> x, textboxp -> y - textboxp -> size, textboxp -> x + textboxp -> size / 4, textboxp -> y + textboxp -> size);
    turtle_rectangle(textboxp -> x + textboxp -> length, textboxp -> y - textboxp -> size, textboxp -> x + textboxp -> length - textboxp -> size / 4, textboxp -> y + textboxp -> size);
    if ((textboxp -> status == TT_STATUS_CLICK || textboxp -> status == TT_STATUS_OPEN || textboxp -> status == TT_STATUS_CLICK_FIRST_TICK || textboxp -> status == TT_STATUS_OPEN_FIRST_TICK) && textboxp -> count <= textboxp -> linePeriod / 2 && textboxp -> editIndexLength == 0) {
        double textLength;
        if (textboxp -> editIndex + textboxp -> editIndexLength == textboxp -> renderStartingIndex) {
            textLength = -textboxp -> size / 10; // i dont know why this is necessary
        } else {
            char tempHold = textboxp -> text[textboxp -> editIndex];
            textboxp -> text[textboxp -> editIndex] = '\0';
            textLength = turtle_text_get_unicode_length(textboxp -> text + textboxp -> renderStartingIndex, textboxp -> size - 1);
            textboxp -> text[textboxp -> editIndex] = tempHold;
        }
        turtle_tools_set_color(textboxp -> color[TT_COLOR_SLOT_TEXTBOX_LINE]);
        turtle_rectangle(textboxp -> x + textboxp -> renderPixelOffset + textLength, textboxp -> y - textboxp -> size * 0.8, textboxp -> x + textboxp -> renderPixelOffset + textLength + textboxp -> size / 10, textboxp -> y + textboxp -> size * 0.8);
    }
    /* mouse */
    if (turtle.mouseX > textboxp -> x && turtle.mouseX < textboxp -> x + textboxp -> length && turtle.mouseY > textboxp -> y - textboxp -> size && turtle.mouseY < textboxp -> y + textboxp -> size) {
        textboxp -> mouseOver = 1;
    } else {
        textboxp -> mouseOver = 0;
    }
    if (textboxp -> enabled != TT_ELEMENT_ENABLED || turtleToolsGlobals.elementLogicTypeOld > textboxp -> priority || (turtleToolsGlobals.elementLogicTypeOld == textboxp -> priority && turtleToolsGlobals.elementLogicIndexOld > turtleToolsGlobals.elementLogicTemp)) {
        /* textbox not enabled or higher priority element is being interacted with */
        if (textboxp -> status == TT_STATUS_OPEN || textboxp -> status == TT_STATUS_OPEN_FIRST_TICK) {
            goto LABEL_TEXTBOX_CHECK_HOVER;
        }
        // if (textboxp -> status != TT_STATUS_BLOCKED) {
            textboxp -> status = TT_STATUS_IDLE;
        // }
        textboxp -> mouseOver = 0;
        return;
    }
    LABEL_TEXTBOX_CHECK_HOVER:
    if (textboxp -> status != TT_STATUS_OPEN && textboxp -> status != TT_STATUS_CLICK && textboxp -> status != TT_STATUS_BLOCKED && textboxp -> status != TT_STATUS_OPEN_FIRST_TICK && textboxp -> status != TT_STATUS_CLICK_FIRST_TICK) {
        if (textboxp -> mouseOver) {
            if (textboxp -> status == TT_STATUS_HOVER || textboxp -> status == TT_STATUS_HOVER_FIRST_TICK) {
                /* hovering textbox */
                textboxp -> status = TT_STATUS_HOVER;
            } else {
                /* first tick hover */
                textboxp -> status = TT_STATUS_HOVER_FIRST_TICK;
            }
        } else {
            textboxp -> status = TT_STATUS_IDLE;
        }
    }
    if (turtle_mouse_down()) {
        if (textboxp -> status == TT_STATUS_HOVER || (textboxp -> status == TT_STATUS_OPEN && textboxp -> mouseOver) || textboxp -> status == TT_STATUS_HOVER_FIRST_TICK || (textboxp -> status == TT_STATUS_OPEN_FIRST_TICK && textboxp -> mouseOver)) {
            /* first tick clicked */
            textboxp -> count = 1;
            textboxp -> editIndex = tt_textboxCalculateIndexFromPosition(textboxp, turtle.mouseX);
            textboxp -> editIndexLength = 0;
            int32_t index = list_find(turtleToolsElements.textboxes, (unitype) (void *) textboxp, 'p');
            if (index != -1 && index != turtleToolsElements.textboxes -> length - 1) {
                turtleToolsElements.textboxes -> type[index] = 'l'; // switch to l to avoid free
                list_delete(turtleToolsElements.textboxes, index);
                list_append(turtleToolsElements.textboxes, (unitype) (void *) textboxp, 'p');
                textboxp -> moveToTop = 1;
            }
            textboxp -> status = TT_STATUS_CLICK_FIRST_TICK;
            if (textboxp -> doubleClickCount == -1) {
                textboxp -> doubleClickCount = 0;
                textboxp -> doubleClickEditIndex = textboxp -> editIndex;
            }
        } else if (textboxp -> status == TT_STATUS_CLICK || textboxp -> status == TT_STATUS_CLICK_FIRST_TICK) {
            /* textbox is being held */
            textboxp -> status = TT_STATUS_CLICK;
            textboxp -> moveToTop = 0;
            if (textboxp -> editingMode == 0) {
                int32_t boundIndex = tt_textboxCalculateIndexFromPosition(textboxp, turtle.mouseX);
                textboxp -> editIndexLength = boundIndex - textboxp -> editIndex;
                if (textboxp -> editIndex + textboxp -> editIndexLength > textboxp -> renderStartingIndex + textboxp -> renderNumCharacters) {
                    textboxp -> editIndexLength = textboxp -> renderStartingIndex + textboxp -> renderNumCharacters - textboxp -> editIndex;
                }
            }
            if (textboxp -> editIndexLength != 0) {
                textboxp -> doubleClickCount = -1;
            }
        } else if (textboxp -> mouseOver && (textboxp -> status == TT_STATUS_OPEN || textboxp -> status == TT_STATUS_OPEN_FIRST_TICK)) {
            /* textbox is open (affirm) */
            textboxp -> status = TT_STATUS_OPEN;
        } else {
            /* textbox is blocked from interaction until mouse is unclicked */
            textboxp -> editIndexLength = 0;
            textboxp -> status = TT_STATUS_BLOCKED;
        }
        if (textboxp -> doubleClickCount > textboxp -> doubleClickTimeout && textboxp -> doubleClickCount <= textboxp -> doubleClickTimeout * 2 && textboxp -> doubleClickEditIndex == textboxp -> editIndex && textboxp -> status != TT_STATUS_BLOCKED) {
            /* double clicked */
            textboxp -> editIndex = 0;
            textboxp -> editIndexLength = strlen(textboxp -> text);
            textboxp -> doubleClickCount = -1;
            textboxp -> editingMode = 2;
        }
        if (textboxp -> doubleClickCount >= 0) {
            textboxp -> doubleClickCount++;
            if (textboxp -> doubleClickCount > textboxp -> doubleClickTimeout) {
                textboxp -> doubleClickCount = -1;
            }
        }
    } else {
        if (textboxp -> status == TT_STATUS_CLICK || textboxp -> status == TT_STATUS_CLICK_FIRST_TICK) {
            /* first tick unclicked */
            textboxp -> status = TT_STATUS_OPEN_FIRST_TICK;
        } else if (textboxp -> status == TT_STATUS_OPEN || textboxp -> status == TT_STATUS_OPEN_FIRST_TICK) {
            /* textbox is open */
            textboxp -> status = TT_STATUS_OPEN;
        } else if (textboxp -> status == TT_STATUS_BLOCKED) {
            textboxp -> status = TT_STATUS_IDLE;
            goto LABEL_TEXTBOX_CHECK_HOVER; // done to avoid a single IDLE tick if mouse is hovering over textbox when unclicked
        }
        if (textboxp -> doubleClickCount >= 0 && textboxp -> doubleClickCount <= textboxp -> doubleClickTimeout) {
            textboxp -> doubleClickCount = textboxp -> doubleClickTimeout;
        } else if (textboxp -> doubleClickCount > textboxp -> doubleClickTimeout * 2) {
            textboxp -> doubleClickCount = -1;
        }
        if (textboxp -> doubleClickCount >= 0) {
            textboxp -> doubleClickCount++;
        }
        textboxp -> editingMode = 0;
    }
    if (textboxp -> status == TT_STATUS_OPEN || textboxp -> status == TT_STATUS_HOVER || textboxp -> status == TT_STATUS_CLICK || textboxp -> status == TT_STATUS_OPEN_FIRST_TICK || textboxp -> status == TT_STATUS_HOVER_FIRST_TICK || textboxp -> status == TT_STATUS_CLICK_FIRST_TICK) {
        turtleToolsGlobals.elementLogicType = textboxp -> priority;
        turtleToolsGlobals.elementLogicIndex = turtleToolsGlobals.elementLogicTemp;
    }
}

void tt_dropdownUpdate(tt_dropdown_t *dropdownp) {
    if (dropdownp -> variable != NULL) {
        dropdownp -> index = *dropdownp -> variable;
        dropdownp -> value = *dropdownp -> variable;
    }
    if (dropdownp -> enabled == TT_ELEMENT_HIDE) {
        if (dropdownp -> status != TT_STATUS_BLOCKED) {
            dropdownp -> status = TT_STATUS_IDLE;
        }
        return;
    }
    if (dropdownp -> options -> length == 0) {
        list_append(dropdownp -> options, (unitype) "None", 's'); // program will crash if dropdown -> options -> length is 0
    }
    /* render dropdown default position */
    double dropdownX = dropdownp -> x;
    double dropdownY = dropdownp -> y;
    double xfactor = turtle_text_get_unicode_length(dropdownp -> options -> data[dropdownp -> index].s, dropdownp -> size - 1);
    double itemHeight = (dropdownp -> size * 1.8);
    double dropdownXFactor[2];
    double dropdownMaxXFactor[2];
    double dropdownAlignFactor;
    /* render dropdown label */
    turtle_tools_set_color(dropdownp -> color[TT_COLOR_SLOT_DROPDOWN_TEXT_LABEL]);
    if (dropdownp -> align == TT_DROPDOWN_ALIGN_LEFT) {
        dropdownXFactor[0] = dropdownX;
        dropdownXFactor[1] = dropdownX + xfactor + dropdownp -> size * 1.2;
        dropdownMaxXFactor[0] = dropdownX;
        dropdownMaxXFactor[1] = dropdownX + dropdownp -> maxXfactor + dropdownp -> size * 1.1;
        if (dropdownXFactor[1] + dropdownp -> size * 1.2 > dropdownMaxXFactor[1]) {
            dropdownMaxXFactor[1] = dropdownXFactor[1] + dropdownp -> size * 1.2;
        }
        dropdownAlignFactor = 0;
        turtle_text_write_unicode(dropdownp -> label, dropdownX + dropdownp -> size / 5, dropdownY + 2 * dropdownp -> size, dropdownp -> size - 1, dropdownAlignFactor);
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
        turtle_text_write_unicode(dropdownp -> label, dropdownX, dropdownY + 2 * dropdownp -> size, dropdownp -> size - 1, dropdownAlignFactor);
    } else if (dropdownp -> align == TT_DROPDOWN_ALIGN_RIGHT) {
        dropdownXFactor[0] = dropdownX - xfactor - dropdownp -> size * 2;
        dropdownXFactor[1] = dropdownX - dropdownp -> size;
        dropdownMaxXFactor[0] = dropdownX - dropdownp -> maxXfactor - dropdownp -> size * 2.2;
        dropdownMaxXFactor[1] = dropdownX;
        dropdownAlignFactor = 100;
        turtle_text_write_unicode(dropdownp -> label, dropdownX - dropdownp -> size / 5, dropdownY + 2 * dropdownp -> size, dropdownp -> size - 1, dropdownAlignFactor);
    }
    /* determine dropdown direction */
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
    /* render dropdown */
    if (dropdownp -> status == TT_STATUS_HOVER || dropdownp -> status == TT_STATUS_HOVER_FIRST_TICK) {
        turtle_tools_set_color(dropdownp -> color[TT_COLOR_SLOT_DROPDOWN_SELECT]);
        turtle_rectangle(dropdownXFactor[0], dropdownY - dropdownp -> size * 0.9, dropdownXFactor[1] + dropdownp -> size, dropdownY + dropdownp -> size * 0.9);
    } else if (dropdownp -> status == TT_STATUS_OPEN || dropdownp -> status == TT_STATUS_CLICK || dropdownp -> status == TT_STATUS_OPEN_FIRST_TICK || dropdownp -> status == TT_STATUS_CLICK_FIRST_TICK) {
        turtle_tools_set_color(dropdownp -> color[TT_COLOR_SLOT_DROPDOWN_BASE]);
        if (dropdownDirection == TT_DROPDOWN_DIRECTION_UP) {
            turtle_rectangle(dropdownMaxXFactor[0], dropdownY + dropdownp -> size * 0.9 + (dropdownp -> options -> length - 1) * itemHeight, dropdownMaxXFactor[1], dropdownY - dropdownp -> size * 0.9);
        } else {
            turtle_rectangle(dropdownMaxXFactor[0], dropdownY - dropdownp -> size * 0.9 - (dropdownp -> options -> length - 1) * itemHeight, dropdownMaxXFactor[1], dropdownY + dropdownp -> size * 0.9);
        }
    } else {
        turtle_tools_set_color(dropdownp -> color[TT_COLOR_SLOT_DROPDOWN_BASE]);
        turtle_rectangle(dropdownXFactor[0], dropdownY - dropdownp -> size * 0.9, dropdownXFactor[1] + dropdownp -> size, dropdownY + dropdownp -> size * 0.9);
    }
    /* mouse */
    if (dropdownp -> enabled != TT_ELEMENT_ENABLED || turtleToolsGlobals.elementLogicTypeOld > dropdownp -> priority || (turtleToolsGlobals.elementLogicTypeOld == dropdownp -> priority && turtleToolsGlobals.elementLogicIndexOld > turtleToolsGlobals.elementLogicTemp)) {
        /* dropdown not enabled or higher priority element is being interacted with */
        if (dropdownp -> status == TT_STATUS_OPEN || dropdownp -> status == TT_STATUS_OPEN_FIRST_TICK) {
            goto LABEL_DROPDOWN_CHECK_HOVER;
        }
        if (dropdownp -> status != TT_STATUS_BLOCKED) {
            dropdownp -> status = TT_STATUS_IDLE;
        }
        goto LABEL_DROPDOWN_END;
    }
    if (turtleToolsRibbon.mainselect[2] != -1) {
        /* fixes to dropdown/ribbon interaction */
        goto LABEL_DROPDOWN_END;
    }
    LABEL_DROPDOWN_CHECK_HOVER:
    if (dropdownp -> status != TT_STATUS_CLICK && dropdownp -> status != TT_STATUS_OPEN && dropdownp -> status != TT_STATUS_OPEN_CLICK && dropdownp -> status != TT_STATUS_BLOCKED && dropdownp -> status != TT_STATUS_CLICK_FIRST_TICK && dropdownp -> status != TT_STATUS_OPEN_FIRST_TICK && dropdownp -> status != TT_STATUS_OPEN_CLICK_FIRST_TICK) {
        if (turtle.mouseX > dropdownXFactor[0] && turtle.mouseX < dropdownXFactor[1] + dropdownp -> size && turtle.mouseY >= dropdownY - dropdownp -> size * 0.9 && turtle.mouseY < dropdownY + dropdownp -> size * 0.9) {
            if (dropdownp -> status == TT_STATUS_HOVER || dropdownp -> status == TT_STATUS_HOVER_FIRST_TICK) {
                /* hovering dropdown */
                dropdownp -> status = TT_STATUS_HOVER;
            } else {
                /* first tick hover */
                dropdownp -> status = TT_STATUS_HOVER_FIRST_TICK;
            }
        } else {
            dropdownp -> status = TT_STATUS_IDLE;
        }
    }
    if (turtle_mouse_down()) {
        if (dropdownp -> status == TT_STATUS_OPEN || dropdownp -> status == TT_STATUS_OPEN_FIRST_TICK) {
            /* first tick clicked (open) */
            int32_t selected = round((dropdownY - turtle.mouseY) / itemHeight);
            if (directionRender == -1) {
                selected = dropdownp -> options -> length - round((turtle.mouseY - dropdownY) / itemHeight);
                if (selected == 0) {
                    selected = -1;
                }
                if (selected == dropdownp -> options -> length) {
                    selected = 0;
                }
            }
            if (turtle.mouseX > dropdownMaxXFactor[0] && turtle.mouseX < dropdownMaxXFactor[1] && selected >= 0 && selected < dropdownp -> options -> length) {
                if (selected == 0) {
                    dropdownp -> status = TT_STATUS_OPEN_CLICK_FIRST_TICK;
                } else {
                    if (dropdownp -> index >= selected) {
                        dropdownp -> index = selected - 1;
                        dropdownp -> value = selected - 1;
                    } else {
                        dropdownp -> index = selected;
                        dropdownp -> value = selected;
                    }
                    dropdownp -> status = TT_STATUS_OPEN_CLICK_FIRST_TICK;
                }
            } else {
                dropdownp -> status = TT_STATUS_IDLE;
                goto LABEL_DROPDOWN_CHECK_HOVER;
            }
        } else if (dropdownp -> status == TT_STATUS_OPEN_CLICK || dropdownp -> status == TT_STATUS_OPEN_CLICK_FIRST_TICK) {
            /* dropdown is being held (open) */
            dropdownp -> status = TT_STATUS_OPEN_CLICK;
        } else if (dropdownp -> status == TT_STATUS_HOVER || dropdownp -> status == TT_STATUS_HOVER_FIRST_TICK) {
            /* first tick clicked */
            int32_t index = list_find(turtleToolsElements.dropdowns, (unitype) (void *) dropdownp, 'p');
            if (index != -1 && index != turtleToolsElements.dropdowns -> length - 1) {
                turtleToolsElements.dropdowns -> type[index] = 'l'; // switch to l to avoid free
                list_delete(turtleToolsElements.dropdowns, index);
                list_append(turtleToolsElements.dropdowns, (unitype) (void *) dropdownp, 'p');
                dropdownp -> moveToTop = 1;
            }
            dropdownp -> status = TT_STATUS_CLICK_FIRST_TICK;
        } else if (dropdownp -> status == TT_STATUS_CLICK || dropdownp -> status == TT_STATUS_CLICK_FIRST_TICK) {
            /* dropdown is being held */
            dropdownp -> status = TT_STATUS_CLICK;
            dropdownp -> moveToTop = 0;
        } else {
            /* dropdown is blocked from interaction until mouse is unclicked */
            dropdownp -> status = TT_STATUS_BLOCKED;
        }
    } else {
        if (dropdownp -> status == TT_STATUS_CLICK || dropdownp -> status == TT_STATUS_CLICK_FIRST_TICK) {
            /* first tick unclicked */
            int32_t selected = round((dropdownY - turtle.mouseY) / itemHeight);
            if (directionRender == -1) {
                selected = dropdownp -> options -> length - round((turtle.mouseY - dropdownY) / itemHeight);
                if (selected == 0) {
                    selected = -1;
                }
                if (selected == dropdownp -> options -> length) {
                    selected = 0;
                }
            }
            if (turtle.mouseX > dropdownMaxXFactor[0] && turtle.mouseX < dropdownMaxXFactor[1] && selected >= 1 && selected < dropdownp -> options -> length) {
                if (dropdownp -> index >= selected) {
                    dropdownp -> index = selected - 1;
                    dropdownp -> value = selected - 1;
                } else {
                    dropdownp -> index = selected;
                    dropdownp -> value = selected;
                }
                dropdownp -> status = TT_STATUS_IDLE;
                goto LABEL_DROPDOWN_CHECK_HOVER;
            } else {
                /* first tick open */
                dropdownp -> status = TT_STATUS_OPEN_FIRST_TICK;
            }
        } else if (dropdownp -> status == TT_STATUS_OPEN || dropdownp -> status == TT_STATUS_OPEN_FIRST_TICK) {
            /* dropdown is open */
            dropdownp -> status = TT_STATUS_OPEN;
        } else if (dropdownp -> status == TT_STATUS_BLOCKED || dropdownp -> status == TT_STATUS_OPEN_CLICK || dropdownp -> status == TT_STATUS_OPEN_CLICK_FIRST_TICK) {
            dropdownp -> status = TT_STATUS_IDLE; // done to avoid a single IDLE tick if mouse is hovering over dropdown when unclicked
            goto LABEL_DROPDOWN_CHECK_HOVER;
        }
    }
    if (dropdownp -> status == TT_STATUS_OPEN || dropdownp -> status == TT_STATUS_CLICK || dropdownp -> status == TT_STATUS_OPEN_FIRST_TICK || dropdownp -> status == TT_STATUS_CLICK_FIRST_TICK) {
        int32_t selected = round((dropdownY - turtle.mouseY) / itemHeight);
        if (directionRender == -1) {
            selected = dropdownp -> options -> length - round((turtle.mouseY - dropdownY) / itemHeight);
            if (selected == 0) {
                selected = -1;
            }
            if (selected == dropdownp -> options -> length) {
                selected = 0;
            }
        }
        if (turtle.mouseX > dropdownMaxXFactor[0] && turtle.mouseX < dropdownMaxXFactor[1] && selected >= 0 && selected < dropdownp -> options -> length) {
            turtle_tools_set_color(dropdownp -> color[TT_COLOR_SLOT_DROPDOWN_HOVER]);
            if (directionRender == -1 && selected == 0) {
                turtle_rectangle(dropdownMaxXFactor[0], dropdownY - dropdownp -> size * 0.9 - (directionRender - 1) / 2.0 * dropdownp -> options -> length * itemHeight - dropdownp -> options -> length * itemHeight, dropdownMaxXFactor[1], dropdownY + dropdownp -> size * 0.9 - (directionRender - 1) / 2.0 * dropdownp -> options -> length * itemHeight - dropdownp -> options -> length * itemHeight);
            } else {
                turtle_rectangle(dropdownMaxXFactor[0], dropdownY - dropdownp -> size * 0.9 - (directionRender - 1) / 2.0 * dropdownp -> options -> length * itemHeight - selected * itemHeight, dropdownMaxXFactor[1], dropdownY + dropdownp -> size * 0.9 - (directionRender - 1) / 2.0 * dropdownp -> options -> length * itemHeight - selected * itemHeight);
            }
        }
        turtle_tools_set_color(dropdownp -> color[TT_COLOR_SLOT_DROPDOWN_TEXT]);
        int32_t renderIndex = 1;
        for (int32_t i = 0; i < dropdownp -> options -> length; i++) {
            if (i != dropdownp -> index) {
                if (dropdownp -> align == TT_DROPDOWN_ALIGN_LEFT) {
                    turtle_text_write_unicode(dropdownp -> options -> data[i].s, dropdownMaxXFactor[0] + dropdownp -> size / 2, dropdownY - (directionRender - 1) / 2.0 * dropdownp -> options -> length * itemHeight - renderIndex * itemHeight, dropdownp -> size - 1, dropdownAlignFactor);
                } else if (dropdownp -> align == TT_DROPDOWN_ALIGN_CENTER) {
                    turtle_text_write_unicode(dropdownp -> options -> data[i].s, (dropdownMaxXFactor[0] + dropdownMaxXFactor[1]) / 2, dropdownY - (directionRender - 1) / 2.0 * dropdownp -> options -> length * itemHeight - renderIndex * itemHeight, dropdownp -> size - 1, dropdownAlignFactor);
                } else if (dropdownp -> align == TT_DROPDOWN_ALIGN_RIGHT) {
                    turtle_text_write_unicode(dropdownp -> options -> data[i].s, dropdownMaxXFactor[1] - dropdownp -> size * 1.58, dropdownY - (directionRender - 1) / 2.0 * dropdownp -> options -> length * itemHeight - renderIndex * itemHeight, dropdownp -> size - 1, dropdownAlignFactor);
                }
                renderIndex++;
            }
        }
    }
    if (dropdownp -> status == TT_STATUS_OPEN || dropdownp -> status == TT_STATUS_OPEN_CLICK || dropdownp -> status == TT_STATUS_HOVER || dropdownp -> status == TT_STATUS_CLICK || dropdownp -> status == TT_STATUS_OPEN_FIRST_TICK || dropdownp -> status == TT_STATUS_OPEN_CLICK_FIRST_TICK || dropdownp -> status == TT_STATUS_HOVER_FIRST_TICK) {
        turtleToolsGlobals.elementLogicType = dropdownp -> priority;
        turtleToolsGlobals.elementLogicIndex = turtleToolsGlobals.elementLogicTemp;
    }
    LABEL_DROPDOWN_END:
    turtle_tools_set_color(dropdownp -> color[TT_COLOR_SLOT_DROPDOWN_TEXT]);
    if (dropdownp -> align == TT_DROPDOWN_ALIGN_LEFT) {
        turtle_text_write_unicode(dropdownp -> options -> data[dropdownp -> index].s, dropdownXFactor[0] + dropdownp -> size / 2, dropdownY, dropdownp -> size - 1, dropdownAlignFactor);
    } else if (dropdownp -> align == TT_DROPDOWN_ALIGN_CENTER) {
        turtle_text_write_unicode(dropdownp -> options -> data[dropdownp -> index].s, (dropdownXFactor[0] + dropdownXFactor[1]) / 2, dropdownY, dropdownp -> size - 1, dropdownAlignFactor);
    } else if (dropdownp -> align == TT_DROPDOWN_ALIGN_RIGHT) {
        turtle_text_write_unicode(dropdownp -> options -> data[dropdownp -> index].s, dropdownXFactor[1] - dropdownp -> size * 0.55, dropdownY, dropdownp -> size - 1, dropdownAlignFactor);
    }
    turtle_tools_set_color(dropdownp -> color[TT_COLOR_SLOT_DROPDOWN_TRIANGLE]);
    if (dropdownp -> status == TT_STATUS_OPEN || dropdownp -> status == TT_STATUS_CLICK || dropdownp -> status == TT_STATUS_OPEN_FIRST_TICK || dropdownp -> status == TT_STATUS_CLICK_FIRST_TICK) {
        turtle_triangle(dropdownXFactor[1] + dropdownp -> size * 0.4, dropdownY + dropdownp -> size * 0.4, dropdownXFactor[1] + dropdownp -> size * 0.4, dropdownY - dropdownp -> size * 0.4, dropdownXFactor[1] - dropdownp -> size * 0.2, dropdownY);
    } else {
        turtle_triangle(dropdownXFactor[1] + dropdownp -> size * 0.6, dropdownY + dropdownp -> size * 0.3, dropdownXFactor[1] - dropdownp -> size * 0.2, dropdownY + dropdownp -> size * 0.3, dropdownXFactor[1] + dropdownp -> size * 0.2, dropdownY - dropdownp -> size * 0.3);
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
        if (scrollbarp -> status != TT_STATUS_BLOCKED) {
            scrollbarp -> status = TT_STATUS_IDLE;
        }
        return;
    }
    double simulateMouseX = turtle.mouseX;
    double simulateMouseY = turtle.mouseY;
    double scrollbarLeft = scrollbarp -> x - scrollbarp -> length / 2;
    double scrollbarRight = scrollbarp -> x + scrollbarp -> length / 2;
    double scrollbarY = scrollbarp -> y;
    double dragLeft = scrollbarLeft + scrollbarp -> value / 100 * (scrollbarp -> length * (1 - scrollbarp -> barPercentage / 100));
    double dragRight = dragLeft + (scrollbarp -> length * scrollbarp -> barPercentage / 100);
    if (scrollbarp -> type == TT_SCROLLBAR_TYPE_HORIZONTAL) {
        turtle_pen_size(scrollbarp -> size * 1);
        turtle_tools_set_color(scrollbarp -> color[TT_COLOR_SLOT_SCROLLBAR_BASE]);
        turtle_goto(scrollbarLeft, scrollbarp -> y);
        turtle_pen_down();
        turtle_goto(scrollbarRight, scrollbarp -> y);
        turtle_pen_up();
        turtle_pen_size(scrollbarp -> size * 0.8);
        if ((scrollbarp -> status == TT_STATUS_HOVER || scrollbarp -> status == TT_STATUS_HOVER_FIRST_TICK) && turtle.mouseX > dragLeft - scrollbarp -> size * 0.4 && turtle.mouseX < dragRight + scrollbarp -> size * 0.4) {
            turtle_tools_set_color(scrollbarp -> color[TT_COLOR_SLOT_SCROLLBAR_HOVER]);
        } else if (scrollbarp -> status == TT_STATUS_CLICK || scrollbarp -> status == TT_STATUS_CLICK_FIRST_TICK) {
            turtle_tools_set_color(scrollbarp -> color[TT_COLOR_SLOT_SCROLLBAR_CLICKED]);
        } else {
            turtle_tools_set_color(scrollbarp -> color[TT_COLOR_SLOT_SCROLLBAR_BAR]);
        }
        turtle_goto(dragLeft, scrollbarp -> y);
        turtle_pen_down();
        turtle_goto(dragRight, scrollbarp -> y);
        turtle_pen_up();
    } else if (scrollbarp -> type == TT_SCROLLBAR_TYPE_VERTICAL) {
        simulateMouseX = turtle.mouseY;
        simulateMouseY = turtle.mouseX;
        scrollbarLeft = scrollbarp -> y - scrollbarp -> length / 2;
        scrollbarRight = scrollbarp -> y + scrollbarp -> length / 2;
        scrollbarY = scrollbarp -> x;
        dragRight = scrollbarRight - scrollbarp -> value / 100 * (scrollbarp -> length * (1 - scrollbarp -> barPercentage / 100));
        dragLeft = dragRight - (scrollbarp -> length * scrollbarp -> barPercentage / 100);
        turtle_pen_size(scrollbarp -> size * 1);
        turtle_tools_set_color(scrollbarp -> color[TT_COLOR_SLOT_SCROLLBAR_BASE]);
        turtle_goto(scrollbarp -> x, scrollbarLeft);
        turtle_pen_down();
        turtle_goto(scrollbarp -> x, scrollbarRight);
        turtle_pen_up();
        turtle_pen_size(scrollbarp -> size * 0.8);
        if ((scrollbarp -> status == TT_STATUS_HOVER || scrollbarp -> status == TT_STATUS_HOVER_FIRST_TICK) && turtle.mouseY > dragLeft - scrollbarp -> size * 0.4 && turtle.mouseY < dragRight + scrollbarp -> size * 0.4) {
            turtle_tools_set_color(scrollbarp -> color[TT_COLOR_SLOT_SCROLLBAR_HOVER]);
        } else if (scrollbarp -> status == TT_STATUS_CLICK || scrollbarp -> status == TT_STATUS_CLICK_FIRST_TICK) {
            turtle_tools_set_color(scrollbarp -> color[TT_COLOR_SLOT_SCROLLBAR_CLICKED]);
        } else {
            turtle_tools_set_color(scrollbarp -> color[TT_COLOR_SLOT_SCROLLBAR_BAR]);
        }
        turtle_goto(scrollbarp -> x, dragLeft);
        turtle_pen_down();
        turtle_goto(scrollbarp -> x, dragRight);
        turtle_pen_up();
    }
    /* mouse */
    if (scrollbarp -> enabled != TT_ELEMENT_ENABLED || turtleToolsGlobals.elementLogicTypeOld > scrollbarp -> priority || (turtleToolsGlobals.elementLogicTypeOld == scrollbarp -> priority && turtleToolsGlobals.elementLogicIndexOld > turtleToolsGlobals.elementLogicTemp)) {
        /* slider not enabled or higher priority element is being interacted with */
        if (scrollbarp -> status != TT_STATUS_BLOCKED) {
            scrollbarp -> status = TT_STATUS_IDLE;
        }
        goto LABEL_SCROLLBAR_END;
    }
    LABEL_SCROLLBAR_CHECK_HOVER:
    if (scrollbarp -> status != TT_STATUS_CLICK && scrollbarp -> status != TT_STATUS_BLOCKED && scrollbarp -> status != TT_STATUS_CLICK_FIRST_TICK) {
        if (simulateMouseY > scrollbarY - scrollbarp -> size * 0.5 && simulateMouseY < scrollbarY + scrollbarp -> size * 0.5 && simulateMouseX < scrollbarRight + scrollbarp -> size * 0.5 && simulateMouseX > scrollbarLeft - scrollbarp -> size * 0.5) {
            if (scrollbarp -> status == TT_STATUS_HOVER || scrollbarp -> status == TT_STATUS_HOVER_FIRST_TICK) {
                /* hovering slider */
                scrollbarp -> status = TT_STATUS_HOVER;
            } else {
                /* first tick hover */
                scrollbarp -> status = TT_STATUS_HOVER_FIRST_TICK;
            }
        } else {
            scrollbarp -> status = TT_STATUS_IDLE;
        }
    }
    if (turtle_mouse_down()) {
        if (scrollbarp -> status == TT_STATUS_HOVER || scrollbarp -> status == TT_STATUS_HOVER_FIRST_TICK) {
            /* first tick clicked */
            if (simulateMouseX > dragLeft - scrollbarp -> size * 0.4 && simulateMouseX < dragRight + scrollbarp -> size * 0.4) {
                if (scrollbarp -> type == TT_SCROLLBAR_TYPE_HORIZONTAL) {
                    turtleToolsGlobals.barAnchor = simulateMouseX - dragLeft;
                } else if (scrollbarp -> type == TT_SCROLLBAR_TYPE_VERTICAL) {
                    turtleToolsGlobals.barAnchor = dragRight - simulateMouseX;
                }
            } else {
                turtleToolsGlobals.barAnchor = (scrollbarp -> length * scrollbarp -> barPercentage / 100) / 2;
            }
            scrollbarp -> status = TT_STATUS_CLICK_FIRST_TICK;
        } else if (scrollbarp -> status == TT_STATUS_CLICK || scrollbarp -> status == TT_STATUS_CLICK_FIRST_TICK) {
            /* slider is being held */
            scrollbarp -> status = TT_STATUS_CLICK;
        } else {
            /* slider is blocked from interaction until mouse is unclicked */
            scrollbarp -> status = TT_STATUS_BLOCKED;
        }
    } else {
        if (scrollbarp -> status == TT_STATUS_CLICK || scrollbarp -> status == TT_STATUS_BLOCKED || scrollbarp -> status == TT_STATUS_CLICK_FIRST_TICK) {
            /* first tick unclicked */
            scrollbarp -> status = TT_STATUS_IDLE;
            goto LABEL_SCROLLBAR_CHECK_HOVER; // done to avoid a single IDLE tick if mouse is hovering over slider when unclicked
        }
    }
    if (scrollbarp -> status == TT_STATUS_CLICK || scrollbarp -> status == TT_STATUS_CLICK_FIRST_TICK) {
        if (scrollbarp -> type == TT_SCROLLBAR_TYPE_HORIZONTAL) {
            scrollbarp -> value = (simulateMouseX - scrollbarLeft - turtleToolsGlobals.barAnchor) / (scrollbarp -> length * (1 - scrollbarp -> barPercentage / 100)) * 100;
        } else if (scrollbarp -> type == TT_SCROLLBAR_TYPE_VERTICAL) {
            scrollbarp -> value = (scrollbarRight - simulateMouseX - turtleToolsGlobals.barAnchor) / (scrollbarp -> length * (1 - scrollbarp -> barPercentage / 100)) * 100;
        }
        if (scrollbarp -> value < 0) {
            scrollbarp -> value = 0;
        }
        if (scrollbarp -> value > 100) {
            scrollbarp -> value = 100;
        }
    }
    if (scrollbarp -> status == TT_STATUS_HOVER || scrollbarp -> status == TT_STATUS_CLICK || scrollbarp -> status == TT_STATUS_HOVER_FIRST_TICK || scrollbarp -> status == TT_STATUS_CLICK_FIRST_TICK) {
        turtleToolsGlobals.elementLogicType = scrollbarp -> priority;
        turtleToolsGlobals.elementLogicIndex = turtleToolsGlobals.elementLogicTemp;
    }
    LABEL_SCROLLBAR_END:
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
        if (contextp -> status == TT_STATUS_OPEN_CLICK) {
            turtleToolsGlobals.elementLogicType = contextp -> priority;
            turtleToolsGlobals.elementLogicIndex = turtleToolsGlobals.elementLogicTemp;
            if (!turtle_mouse_down()) {
                contextp -> status = TT_STATUS_IDLE;
            }
        }
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
    turtle_tools_set_color(contextp -> color[TT_COLOR_SLOT_CONTEXT_BASE]);
    turtle_rectangle(contextTextX, contextTextY - contextp -> size * 0.9 - (contextp -> options -> length - 1) * itemHeight - 2, contextTextX + contextp -> maxXfactor + contextp -> size / 1.25, contextTextY + contextp -> size * 0.9 + 2);
    turtle_tools_set_color(contextp -> color[TT_COLOR_SLOT_CONTEXT_TEXT]);
    contextp -> index = -1;
    contextp -> value = -1;
    for (int32_t i = 0; i < contextp -> options -> length; i++) {
        if (turtle.mouseX > contextTextX && turtle.mouseX < contextTextX + contextp -> maxXfactor + contextp -> size / 1.25 && turtle.mouseY >= contextTextY - i * itemHeight - contextp -> size * 0.9 && turtle.mouseY < contextTextY - i * itemHeight + contextp -> size * 0.9) {
            turtle_tools_set_color(contextp -> color[TT_COLOR_SLOT_CONTEXT_SELECT]);
            turtle_rectangle(contextTextX, contextTextY - i * itemHeight - contextp -> size * 0.9, contextTextX + contextp -> maxXfactor + contextp -> size / 1.25, contextTextY - i * itemHeight + contextp -> size * 0.9);
            turtle_tools_set_color(contextp -> color[TT_COLOR_SLOT_CONTEXT_TEXT]);
            contextp -> index = i;
            contextp -> value = i;
        }
        turtle_text_write_unicode(contextp -> options -> data[i].s, contextTextX + contextp -> size / 2.5, contextTextY - i * itemHeight, contextp -> size - 1, 0);
    }
    if (contextp -> enabled == TT_ELEMENT_ENABLED && (turtleToolsGlobals.elementLogicTypeOld < contextp -> priority || (turtleToolsGlobals.elementLogicTypeOld == contextp -> priority && turtleToolsGlobals.elementLogicIndexOld <= (int32_t) turtleToolsGlobals.elementLogicTemp))) {
        turtleToolsGlobals.elementLogicType = contextp -> priority;
        turtleToolsGlobals.elementLogicIndex = turtleToolsGlobals.elementLogicTemp;
        if (turtle_mouse_down()) {
            if (contextp -> index != -1) {
                contextp -> status = TT_STATUS_OPEN_CLICK;
            }
            contextp -> enabled = TT_ELEMENT_HIDE;
        }
    }
    if (contextp -> variable != NULL) {
        *contextp -> variable = contextp -> index;
    }
}

static char readerString[4096]; // global memory for efficiency

void tt_readerUpdate(tt_reader_t *readerp) {
    if (readerp -> element == TT_ELEMENT_LIST_READER) {
        /* render rectangle */
        double readerLeftX = readerp -> x;
        double readerRightX = readerp -> x + readerp -> width;
        double readerY = readerp -> y + readerp -> size * 0.8;
        double readerTopHeight = readerp -> size * 2;
        turtle_tools_set_color(readerp -> color[TT_COLOR_SLOT_LIST_READER_BASE]);
        turtle_pen_size(readerp -> size);
        turtle_goto(readerLeftX + readerp -> size / 2, readerY - readerp -> height + readerp -> size / 2);
        turtle_pen_down();
        turtle_goto(readerRightX - readerp -> size / 2, readerY - readerp -> height + readerp -> size / 2);
        turtle_goto(readerRightX - readerp -> size / 2, readerY - readerp -> size / 2);
        turtle_goto(readerLeftX + readerp -> size / 2, readerY - readerp -> size / 2);
        turtle_goto(readerLeftX + readerp -> size / 2, readerY - readerp -> height + readerp -> size / 2);
        turtle_pen_up();
        turtle_rectangle(readerLeftX + readerp -> size / 2, readerY - readerp -> height + readerp -> size / 2, readerRightX - readerp -> size / 2, readerY - readerp -> size / 2);
        /* render items */
        list_t *list = (*(readerp -> variable)).r;
        int32_t numItems = list -> length;
        int32_t maxItems = (int) ((readerp -> height - readerp -> size * 4) / (readerp -> size * 2.2) + 2);
        if (numItems > maxItems) {
            numItems = maxItems;
        }
        double itemsInWindow = (readerp -> height - readerp -> size * 4) / (readerp -> size * 2.2);
        double ypos = readerY + readerp -> size / 2 - 1.6 * readerp -> size * 2.2 + readerp -> scrollbarp -> value / 100.0 * readerp -> size * 2.2 * (list -> length - itemsInWindow);
        int32_t startingItem = (ypos + readerp -> size - (readerY - readerp -> size * 2)) / (readerp -> size * 2.2);
        ypos -= startingItem * readerp -> size * 2.2;
        if (numItems + startingItem > list -> length) {
            numItems = list -> length - startingItem;
        }
        for (int32_t i = startingItem; i < numItems + startingItem; i++) {
            turtle_tools_set_color(readerp -> color[TT_COLOR_SLOT_LIST_READER_TEXT]);
            char numberLabel[32];
            sprintf(numberLabel, "%d", i);
            double edgeX = readerLeftX + readerp -> size + strlen(numberLabel) * (readerp -> size - 1) * 0.75;
            if (i == startingItem && ypos > readerY - readerp -> size * 1.5) {
                /* exception - don't draw top of box */
                turtle_tools_set_color(readerp -> color[TT_COLOR_SLOT_LIST_READER_ITEM]);
                turtle_rectangle(edgeX, ypos, readerRightX - readerp -> size, ypos - readerp -> size);
            } else if (ypos < readerY - readerp -> height + readerp -> size) {
                /* exception - box is entirely below reader */
                break;
            } else if (ypos < readerY - readerp -> height + readerp -> size * 1.5) {
                /* exception - don't draw bottom of box */
                turtle_tools_set_color(readerp -> color[TT_COLOR_SLOT_LIST_READER_ITEM]);
                turtle_rectangle(edgeX, ypos + readerp -> size, readerRightX - readerp -> size, ypos);
                break;
            } else {
                turtle_text_write_string(numberLabel, (readerLeftX + edgeX) / 2, ypos, readerp -> size - 1, 50);
                turtle_tools_set_color(readerp -> color[TT_COLOR_SLOT_LIST_READER_ITEM]);
                turtle_rectangle(edgeX, ypos + readerp -> size, readerRightX - readerp -> size, ypos - readerp -> size);
                unitype_sprint(readerString, list -> data[i], list -> type[i]);
                turtle_tools_set_color(readerp -> color[TT_COLOR_SLOT_LIST_READER_TEXT_ITEM]);
                if (turtle_text_get_unicode_length(readerString, readerp -> size - 1) > (readerRightX - readerp -> size) - edgeX - (readerp -> size - 1)) {
                    /* text is too long to fit */
                    turtle_text_truncate_string(readerString, readerp -> size - 1, (readerRightX - readerp -> size) - edgeX - (readerp -> size - 1) * 1.5, 1);
                    strcat(readerString, "...");
                }
                turtle_text_write_unicode(readerString, edgeX + (readerp -> size - 1) / 2, ypos, readerp -> size - 1, 0);
            }
            ypos -= readerp -> size * 2.2;
        }
        /* draw top and bottom boxes */
        turtle_tools_set_color(readerp -> color[TT_COLOR_SLOT_LIST_READER_BASE]); // TT_COLOR_SLOT_LIST_READER_BASE, TT_COLOR_SLOT_LIST_READER_SCROLLBAR_BASE
        turtle_rectangle(readerLeftX + readerp -> size / 4, readerY - readerp -> size / 4, readerRightX - readerp -> size / 4, readerY - readerp -> size * 2);
        turtle_rectangle(readerLeftX + readerp -> size / 4, readerY + readerp -> size / 4 - readerp -> height, readerRightX - readerp -> size / 4, readerY + readerp -> size * 2 - readerp -> height);
        /* render label */
        turtle_tools_set_color(readerp -> color[TT_COLOR_SLOT_LIST_READER_TEXT]);
        turtle_text_write_unicode(readerp -> label, (readerLeftX + readerRightX) / 2, readerY - readerp -> size, readerp -> size - 1, 50);
        turtle_text_write_stringf((readerLeftX + readerRightX) / 2, readerY - readerp -> height + readerp -> size, readerp -> size - 1, 50, "Items: %d", list -> length);
        turtle_pen_size(readerp -> size / 12);
        turtle_tools_set_color(readerp -> color[TT_COLOR_SLOT_LIST_READER_SCROLLBAR_BAR]);
        turtle_goto(readerRightX - readerp -> size * 0.5, readerY - readerp -> height);
        turtle_pen_down();
        turtle_goto(readerRightX, readerY - readerp -> height + readerp -> size * 0.5);
        turtle_pen_up();
        turtle_goto(readerRightX - readerp -> size * 0.75, readerY - readerp -> height);
        turtle_pen_down();
        turtle_goto(readerRightX, readerY - readerp -> height + readerp -> size * 0.75);
        turtle_pen_up();
        turtle_goto(readerRightX - readerp -> size * 1.0, readerY - readerp -> height);
        turtle_pen_down();
        turtle_goto(readerRightX, readerY - readerp -> height + readerp -> size * 1.0);
        turtle_pen_up();
        /* scrollbar */
        if (list -> length >= maxItems - 1) {
            readerp -> scrollbarp -> x = readerp -> x + readerp -> width - readerp -> size / 2;
            readerp -> scrollbarp -> y = readerY - readerp -> height / 2;
            readerp -> scrollbarp -> color[TT_COLOR_SLOT_SCROLLBAR_BASE] = readerp -> color[TT_COLOR_SLOT_LIST_READER_SCROLLBAR_BASE];
            readerp -> scrollbarp -> color[TT_COLOR_SLOT_SCROLLBAR_BAR] = readerp -> color[TT_COLOR_SLOT_LIST_READER_SCROLLBAR_BAR];
            readerp -> scrollbarp -> color[TT_COLOR_SLOT_SCROLLBAR_HOVER] = readerp -> color[TT_COLOR_SLOT_LIST_READER_SCROLLBAR_HOVER];
            readerp -> scrollbarp -> color[TT_COLOR_SLOT_SCROLLBAR_CLICKED] = readerp -> color[TT_COLOR_SLOT_LIST_READER_SCROLLBAR_CLICKED];
            readerp -> scrollbarp -> barPercentage = 100.0 / (log((list -> length - itemsInWindow) / 2.0 + 1) + 1);
            readerp -> scrollbarp -> length = readerp -> height - readerp -> size * 4.75;
            turtleToolsGlobals.elementLogicTemp++;
            tt_scrollbarUpdate(readerp -> scrollbarp);
        } else {
            readerp -> scrollbarp -> value = 0;
        }
        /* mouse */
        if (readerp -> enabled != TT_ELEMENT_ENABLED || turtleToolsGlobals.elementLogicTypeOld > readerp -> priority || (turtleToolsGlobals.elementLogicTypeOld == readerp -> priority && turtleToolsGlobals.elementLogicIndexOld > turtleToolsGlobals.elementLogicTemp)) {
            /* reader not enabled or higher priority element is being interacted with */
            if (readerp -> status == TT_STATUS_CLICK || readerp -> status == TT_STATUS_CLICK_FIRST_TICK) {
                goto LABEL_LIST_READER_CHECK_HOVER;
            }
            if (readerp -> status != TT_STATUS_BLOCKED) {
                readerp -> status = TT_STATUS_IDLE;
            }
            return;
        }
        LABEL_LIST_READER_CHECK_HOVER:
        if (readerp -> status != TT_STATUS_CLICK && readerp -> status != TT_STATUS_BLOCKED && readerp -> status != TT_STATUS_CLICK_FIRST_TICK) {
            if (turtle.mouseX > readerLeftX && turtle.mouseX < readerRightX && turtle.mouseY > readerY - readerTopHeight && turtle.mouseY < readerY) {
                if (readerp -> status == TT_STATUS_HOVER || readerp -> status == TT_STATUS_HOVER_FIRST_TICK) {
                    /* hovering reader */
                    readerp -> status = TT_STATUS_HOVER;
                } else {
                    /* first tick hover */
                    readerp -> status = TT_STATUS_HOVER_FIRST_TICK;
                    readerp -> resizing = 0;
                }
            } else if (turtle.mouseX > readerRightX - readerp -> size && turtle.mouseX < readerRightX && turtle.mouseY > readerY - readerp -> height && turtle.mouseY < readerY - readerp -> height + readerp -> size && (turtle.mouseY - (readerY - readerp -> height)) + (readerRightX - turtle.mouseX) <= readerp -> size * 1.2) {
                if (readerp -> status == TT_STATUS_HOVER || readerp -> status == TT_STATUS_HOVER_FIRST_TICK) {
                    /* hovering reader */
                    readerp -> status = TT_STATUS_HOVER;
                } else {
                    /* first tick hover */
                    readerp -> status = TT_STATUS_HOVER_FIRST_TICK;
                    readerp -> resizing = 1;
                }
            } else {
                readerp -> status = TT_STATUS_IDLE;
            }
        }
        if (turtle_mouse_down()) {
            if (readerp -> status == TT_STATUS_HOVER || readerp -> status == TT_STATUS_HOVER_FIRST_TICK) {
                /* first tick clicked */
                if (readerp -> resizing == 0) {
                    readerp -> anchorX = readerp -> x;
                    readerp -> anchorY = readerp -> y;
                } else {
                    readerp -> anchorX = readerp -> width;
                    readerp -> anchorY = readerp -> height;
                }
                readerp -> mouseAnchorX = turtle.mouseX;
                readerp -> mouseAnchorY = turtle.mouseY;
                int32_t index = list_find(turtleToolsElements.readers, (unitype) (void *) readerp, 'p');
                if (index != -1 && index != turtleToolsElements.readers -> length - 1) {
                    turtleToolsElements.readers -> type[index] = 'l'; // switch to l to avoid free
                    list_delete(turtleToolsElements.readers, index);
                    list_append(turtleToolsElements.readers, (unitype) (void *) readerp, 'p');
                    readerp -> moveToTop = 1;
                }
                readerp -> status = TT_STATUS_CLICK_FIRST_TICK;
            } else if (readerp -> status == TT_STATUS_CLICK || readerp -> status == TT_STATUS_CLICK_FIRST_TICK) {
                /* reader is being held */
                readerp -> status = TT_STATUS_CLICK;
                readerp -> moveToTop = 0;
            } else {
                /* reader is blocked from interaction until mouse is unclicked */
                readerp -> status = TT_STATUS_BLOCKED;
            }
        } else {
            if (readerp -> status == TT_STATUS_CLICK || readerp -> status == TT_STATUS_BLOCKED || readerp -> status == TT_STATUS_CLICK_FIRST_TICK) {
                /* first tick unclicked */
                readerp -> status = TT_STATUS_IDLE;
                goto LABEL_LIST_READER_CHECK_HOVER; // done to avoid a single IDLE tick if mouse is hovering over reader when unclicked
            }
        }
        if (readerp -> status == TT_STATUS_CLICK || readerp -> status == TT_STATUS_CLICK_FIRST_TICK) {
            if (readerp -> resizing == 0) {
                readerp -> x = readerp -> anchorX + turtle.mouseX - readerp -> mouseAnchorX;
                readerp -> y = readerp -> anchorY + turtle.mouseY - readerp -> mouseAnchorY;
            } else {
                readerp -> width = readerp -> anchorX + turtle.mouseX - readerp -> mouseAnchorX;
                readerp -> height = readerp -> anchorY + readerp -> mouseAnchorY - turtle.mouseY;
                if (readerp -> width < readerp -> size * 8) {
                    readerp -> width = readerp -> size * 8;
                }
                if (readerp -> height < readerp -> size * 8) {
                    readerp -> height = readerp -> size * 8;
                }
            }
        }
        if (readerp -> status == TT_STATUS_HOVER || readerp -> status == TT_STATUS_CLICK || readerp -> status == TT_STATUS_HOVER_FIRST_TICK || readerp -> status == TT_STATUS_CLICK_FIRST_TICK) {
            turtleToolsGlobals.elementLogicType = readerp -> priority;
            turtleToolsGlobals.elementLogicIndex = turtleToolsGlobals.elementLogicTemp;
        }
    } else if (readerp -> element == TT_ELEMENT_VARIABLE_READER) {
        unitype variable = *(readerp -> variable);
        unitype_sprint(readerString, variable, readerp -> type);
        double innerWidth = turtle_text_get_unicode_length(readerString, readerp -> size - 1) + readerp -> size;
        if (innerWidth < readerp -> size * 4) {
            innerWidth = readerp -> size * 4;
        }
        double readerWidth = turtle_text_get_unicode_length(readerp -> label, readerp -> size - 1) + innerWidth + readerp -> size * 1.8;
        double readerLeftX = readerp -> x;
        double readerRightX = readerp -> x + readerWidth;
        double readerY = readerp -> y;
        double readerHeight = readerp -> size * 1.75;
        /* rounded rectangle (base) */
        turtle_tools_set_color(readerp -> color[TT_COLOR_SLOT_VARIABLE_READER_BASE]);
        turtle_pen_size(readerp -> size);
        turtle_goto(readerLeftX + readerp -> size / 2, readerY - readerHeight / 2 + readerp -> size / 2);
        turtle_pen_down();
        turtle_goto(readerRightX - readerp -> size / 2, readerY - readerHeight / 2 + readerp -> size / 2);
        turtle_goto(readerRightX - readerp -> size / 2, readerY + readerHeight / 2 - readerp -> size / 2);
        turtle_goto(readerLeftX + readerp -> size / 2, readerY + readerHeight / 2 - readerp -> size / 2);
        turtle_goto(readerLeftX + readerp -> size / 2, readerY - readerHeight / 2 + readerp -> size / 2);
        turtle_pen_up();
        turtle_rectangle(readerLeftX + readerp -> size / 2, readerY - readerHeight / 2 + readerp -> size / 2, readerRightX - readerp -> size / 2, readerY + readerHeight / 2 - readerp -> size / 2);
        /* rounded rectangle (item) */
        double readerInnerRightX = readerRightX - readerp -> size * 0.6;
        double readerInnerLeftX = readerInnerRightX - innerWidth;
        double readerInnerHeight = readerHeight * 0.8;
        turtle_tools_set_color(readerp -> color[TT_COLOR_SLOT_VARIABLE_READER_ITEM]);
        turtle_pen_size(readerp -> size * 0.5);
        turtle_goto(readerInnerLeftX + readerp -> size / 4, readerY - readerInnerHeight / 2 + readerp -> size / 4);
        turtle_pen_down();
        turtle_goto(readerInnerRightX - readerp -> size / 4, readerY - readerInnerHeight / 2 + readerp -> size / 4);
        turtle_goto(readerInnerRightX - readerp -> size / 4, readerY + readerInnerHeight / 2 - readerp -> size / 4);
        turtle_goto(readerInnerLeftX + readerp -> size / 4, readerY + readerInnerHeight / 2 - readerp -> size / 4);
        turtle_goto(readerInnerLeftX + readerp -> size / 4, readerY - readerInnerHeight / 2 + readerp -> size / 4);
        turtle_pen_up();
        turtle_rectangle(readerInnerLeftX + readerp -> size / 4, readerY - readerInnerHeight / 2 + readerp -> size / 4, readerInnerRightX - readerp -> size / 4, readerY + readerInnerHeight / 2 - readerp -> size / 4);
        /* render text */
        turtle_tools_set_color(readerp -> color[TT_COLOR_SLOT_VARIABLE_READER_TEXT]);
        turtle_text_write_unicode(readerp -> label, readerp -> x + readerp -> size * 0.6, readerp -> y, readerp -> size - 1, 0);
        turtle_tools_set_color(readerp -> color[TT_COLOR_SLOT_VARIABLE_READER_TEXT_ITEM]);
        turtle_text_write_unicode(readerString, (readerInnerLeftX + readerInnerRightX) / 2, readerp -> y, readerp -> size - 1, 50);
        /* mouse */
        if (readerp -> enabled != TT_ELEMENT_ENABLED || turtleToolsGlobals.elementLogicTypeOld > readerp -> priority || (turtleToolsGlobals.elementLogicTypeOld == readerp -> priority && turtleToolsGlobals.elementLogicIndexOld > turtleToolsGlobals.elementLogicTemp)) {
            /* reader not enabled or higher priority element is being interacted with */
            if (readerp -> status == TT_STATUS_CLICK || readerp -> status == TT_STATUS_CLICK_FIRST_TICK) {
                goto LABEL_VARIABLE_READER_CHECK_HOVER;
            }
            if (readerp -> status != TT_STATUS_BLOCKED) {
                readerp -> status = TT_STATUS_IDLE;
            }
            return;
        }
        LABEL_VARIABLE_READER_CHECK_HOVER:
        if (readerp -> status != TT_STATUS_CLICK && readerp -> status != TT_STATUS_BLOCKED && readerp -> status != TT_STATUS_CLICK_FIRST_TICK) {
            if (turtle.mouseX > readerLeftX && turtle.mouseX < readerRightX && turtle.mouseY > readerY - readerHeight / 2 && turtle.mouseY < readerY + readerHeight / 2) {
                if (readerp -> status == TT_STATUS_HOVER || readerp -> status == TT_STATUS_HOVER_FIRST_TICK) {
                    /* hovering reader */
                    readerp -> status = TT_STATUS_HOVER;
                } else {
                    /* first tick hover */
                    readerp -> status = TT_STATUS_HOVER_FIRST_TICK;
                }
            } else {
                readerp -> status = TT_STATUS_IDLE;
            }
        }
        if (turtle_mouse_down()) {
            if (readerp -> status == TT_STATUS_HOVER || readerp -> status == TT_STATUS_HOVER_FIRST_TICK) {
                /* first tick clicked */
                readerp -> anchorX = readerp -> x;
                readerp -> anchorY = readerp -> y;
                readerp -> mouseAnchorX = turtle.mouseX;
                readerp -> mouseAnchorY = turtle.mouseY;
                int32_t index = list_find(turtleToolsElements.readers, (unitype) (void *) readerp, 'p');
                if (index != -1 && index != turtleToolsElements.readers -> length - 1) {
                    turtleToolsElements.readers -> type[index] = 'l'; // switch to l to avoid free
                    list_delete(turtleToolsElements.readers, index);
                    list_append(turtleToolsElements.readers, (unitype) (void *) readerp, 'p');
                    readerp -> moveToTop = 1;
                }
                readerp -> status = TT_STATUS_CLICK_FIRST_TICK;
            } else if (readerp -> status == TT_STATUS_CLICK || readerp -> status == TT_STATUS_CLICK_FIRST_TICK) {
                /* reader is being held */
                readerp -> status = TT_STATUS_CLICK;
                readerp -> moveToTop = 0;
            } else {
                /* reader is blocked from interaction until mouse is unclicked */
                readerp -> status = TT_STATUS_BLOCKED;
            }
        } else {
            if (readerp -> status == TT_STATUS_CLICK || readerp -> status == TT_STATUS_BLOCKED || readerp -> status == TT_STATUS_CLICK_FIRST_TICK) {
                /* first tick unclicked */
                readerp -> status = TT_STATUS_IDLE;
                goto LABEL_VARIABLE_READER_CHECK_HOVER; // done to avoid a single IDLE tick if mouse is hovering over reader when unclicked
            }
        }
        if (readerp -> status == TT_STATUS_CLICK || readerp -> status == TT_STATUS_CLICK_FIRST_TICK) {
            readerp -> x = readerp -> anchorX + turtle.mouseX - readerp -> mouseAnchorX;
            readerp -> y = readerp -> anchorY + turtle.mouseY - readerp -> mouseAnchorY;
        }
        if (readerp -> status == TT_STATUS_HOVER || readerp -> status == TT_STATUS_CLICK || readerp -> status == TT_STATUS_HOVER_FIRST_TICK || readerp -> status == TT_STATUS_CLICK_FIRST_TICK) {
            turtleToolsGlobals.elementLogicType = readerp -> priority;
            turtleToolsGlobals.elementLogicIndex = turtleToolsGlobals.elementLogicTemp;
        }
    }
}

void turtleToolsUpdate() {
    turtleToolsUpdateUI();
    turtleToolsUpdateRibbonPopup();
}

void turtleToolsUpdateUI() {
    turtleToolsGlobals.elementLogicType = TT_ELEMENT_NONE;
    turtleToolsGlobals.elementLogicIndex = -1;
    turtleToolsGlobals.elementLogicTemp = -1;
    uint16_t shapeSave = turtle.penshape;
    turtle_pen_shape(TURTLE_PEN_SHAPE_CIRCLE);
    if (turtleToolsEnabled.buttonEnabled) {
        turtleToolsGlobals.elementLogicTemp = 0;
        for (int32_t i = 0; i < turtleToolsElements.buttons -> length; i++) {
            if (((turtle_tools_button_t *) (turtleToolsElements.buttons -> data[i].p)) -> ignored == TT_ELEMENT_IGNORED) {
                continue;
            }
            tt_buttonUpdate((turtle_tools_button_t *) (turtleToolsElements.buttons -> data[i].p));
            turtleToolsGlobals.elementLogicTemp++;
        }
    }
    if (turtleToolsEnabled.switchEnabled) {
        turtleToolsGlobals.elementLogicTemp = 0;
        for (int32_t i = 0; i < turtleToolsElements.switches -> length; i++) {
            if (((tt_switch_t *) (turtleToolsElements.switches -> data[i].p)) -> ignored == TT_ELEMENT_IGNORED) {
                continue;
            }
            tt_switchUpdate((tt_switch_t *) (turtleToolsElements.switches -> data[i].p));
            turtleToolsGlobals.elementLogicTemp++;
        }
    }
    if (turtleToolsEnabled.dialEnabled) {
        turtleToolsGlobals.elementLogicTemp = 0;
        for (int32_t i = 0; i < turtleToolsElements.dials -> length; i++) {
            if (((tt_dial_t *) (turtleToolsElements.dials -> data[i].p)) -> ignored == TT_ELEMENT_IGNORED) {
                continue;
            }
            tt_dialUpdate((tt_dial_t *) (turtleToolsElements.dials -> data[i].p));
            turtleToolsGlobals.elementLogicTemp++;
        }
    }
    if (turtleToolsEnabled.sliderEnabled) {
        turtleToolsGlobals.elementLogicTemp = 0;
        for (int32_t i = 0; i < turtleToolsElements.sliders -> length; i++) {
            if (((tt_slider_t *) (turtleToolsElements.sliders -> data[i].p)) -> ignored == TT_ELEMENT_IGNORED) {
                continue;
            }
            tt_sliderUpdate((tt_slider_t *) (turtleToolsElements.sliders -> data[i].p));
            turtleToolsGlobals.elementLogicTemp++;
        }
    }
    if (turtleToolsEnabled.textboxEnabled) {
        turtleToolsGlobals.elementLogicTemp = 0;
        for (int32_t i = 0; i < turtleToolsElements.textboxes -> length; i++) {
            if (((tt_textbox_t *) (turtleToolsElements.textboxes -> data[i].p)) -> ignored == TT_ELEMENT_IGNORED) {
                continue;
            }
            tt_textbox_t *textboxp = (tt_textbox_t *) (turtleToolsElements.textboxes -> data[i].p);
            tt_textboxUpdate(textboxp);
            if (textboxp -> moveToTop) {
                i--;
            } else {
                turtleToolsGlobals.elementLogicTemp++;
            }
        }
    }
    if (turtleToolsEnabled.dropdownEnabled) {
        turtleToolsGlobals.elementLogicTemp = 0;
        for (int32_t i = 0; i < turtleToolsElements.dropdowns -> length; i++) {
            if (((tt_dropdown_t *) (turtleToolsElements.dropdowns -> data[i].p)) -> ignored == TT_ELEMENT_IGNORED) {
                continue;
            }
            tt_dropdown_t *dropdownp = (tt_dropdown_t *) (turtleToolsElements.dropdowns -> data[i].p);
            tt_dropdownUpdate(dropdownp);
            if (dropdownp -> moveToTop) {
                i--;
            } else {
                turtleToolsGlobals.elementLogicTemp++;
            }
        }
    }
    if (turtleToolsEnabled.scrollbarEnabled) {
        turtleToolsGlobals.elementLogicTemp = 0;
        for (int32_t i = 0; i < turtleToolsElements.scrollbars -> length; i++) {
            if (((tt_scrollbar_t *) (turtleToolsElements.scrollbars -> data[i].p)) -> ignored == TT_ELEMENT_IGNORED) {
                continue;
            }
            tt_scrollbarUpdate((tt_scrollbar_t *) (turtleToolsElements.scrollbars -> data[i].p));
            turtleToolsGlobals.elementLogicTemp++;
        }
    }
    if (turtleToolsEnabled.contextEnabled) {
        turtleToolsGlobals.elementLogicTemp = 0;
        for (int32_t i = 0; i < turtleToolsElements.contexts -> length; i++) {
            if (((tt_context_t *) (turtleToolsElements.contexts -> data[i].p)) -> ignored == TT_ELEMENT_IGNORED) {
                continue;
            }
            tt_contextUpdate((tt_context_t *) (turtleToolsElements.contexts -> data[i].p));
            turtleToolsGlobals.elementLogicTemp++;
        }
    }
    if (turtleToolsEnabled.readerEnabled) {
        turtleToolsGlobals.elementLogicTemp = 0;
        for (int32_t i = 0; i < turtleToolsElements.readers -> length; i++) {
            if (((tt_reader_t *) (turtleToolsElements.readers -> data[i].p)) -> ignored == TT_ELEMENT_IGNORED) {
                continue;
            }
            tt_reader_t *readerp = (tt_reader_t *) (turtleToolsElements.readers -> data[i].p);
            tt_readerUpdate(readerp);
            if (readerp -> moveToTop) {
                i--;
            } else {
                turtleToolsGlobals.elementLogicTemp += 2;
            }
        }
    }
    turtle.penshape = shapeSave;
    turtleToolsGlobals.elementLogicTypeOld = turtleToolsGlobals.elementLogicType;
    turtleToolsGlobals.elementLogicIndexOld = turtleToolsGlobals.elementLogicIndex;
}

void turtleToolsUpdateRibbonPopup() {
    uint16_t shapeSave = turtle.penshape;
    turtle_pen_shape(TURTLE_PEN_SHAPE_CIRCLE);
    if (turtleToolsEnabled.ribbonEnabled) {
        turtle_tools_ribbon_update();
    }
    if (turtleToolsEnabled.popupEnabled) {
        turtle_tools_popup_update();
    }
    turtle.penshape = shapeSave;
    turtleToolsGlobals.elementLogicTypeOld = turtleToolsGlobals.elementLogicType;
    turtleToolsGlobals.elementLogicIndexOld = turtleToolsGlobals.elementLogicIndex;
}
