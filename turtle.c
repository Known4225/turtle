// #define TURTLE_IMPLEMENTATION
#include "turtle.h"
#include <time.h>

void parseRibbonOutput() {
    if (ribbonRender.output[0] == 1) {
        ribbonRender.output[0] = 0;
        if (ribbonRender.output[1] == 0) { // File
            if (ribbonRender.output[2] == 1) { // New
                printf("New\n");
            }
            if (ribbonRender.output[2] == 2) { // Save
                if (strcmp(osToolsFileDialog.selectedFilename, "null") == 0) {
                    if (osToolsFileDialogPrompt(1, "") != -1) {
                        printf("Saved to: %s\n", osToolsFileDialog.selectedFilename);
                    }
                } else {
                    printf("Saved to: %s\n", osToolsFileDialog.selectedFilename);
                }
            }
            if (ribbonRender.output[2] == 3) { // Save As...
                if (osToolsFileDialogPrompt(1, "") != -1) {
                    printf("Saved to: %s\n", osToolsFileDialog.selectedFilename);
                }
            }
            if (ribbonRender.output[2] == 4) { // Open
                if (osToolsFileDialogPrompt(0, "") != -1) {
                    printf("Loaded data from: %s\n", osToolsFileDialog.selectedFilename);
                }
            }
        }
        if (ribbonRender.output[1] == 1) { // Edit
            if (ribbonRender.output[2] == 1) { // Undo
                printf("Undo\n");
            }
            if (ribbonRender.output[2] == 2) { // Redo
                printf("Redo\n");
            }
            if (ribbonRender.output[2] == 3) { // Cut
                osToolsClipboardSetText("test123");
                printf("Cut \"test123\" to clipboard!\n");
            }
            if (ribbonRender.output[2] == 4) { // Copy
                osToolsClipboardSetText("test345");
                printf("Copied \"test345\" to clipboard!\n");
            }
            if (ribbonRender.output[2] == 5) { // Paste
                osToolsClipboardGetText();
                printf("Pasted \"%s\" from clipboard!\n", osToolsClipboard.text);
            }
        }
        if (ribbonRender.output[1] == 2) { // View
            if (ribbonRender.output[2] == 1) { // Change theme
                printf("Change theme\n");
                if (tt_theme == TT_THEME_DARK) {
                    turtleBgColor(36, 30, 32);
                    turtleToolsSetTheme(TT_THEME_COLT);
                } else if (tt_theme == TT_THEME_COLT) {
                    turtleBgColor(212, 201, 190);
                    turtleToolsSetTheme(TT_THEME_NAVY);
                } else if (tt_theme == TT_THEME_NAVY) {
                    turtleBgColor(255, 255, 255);
                    turtleToolsSetTheme(TT_THEME_LIGHT);
                } else if (tt_theme == TT_THEME_LIGHT) {
                    turtleBgColor(30, 30, 30);
                    turtleToolsSetTheme(TT_THEME_DARK);
                }
            } 
            if (ribbonRender.output[2] == 2) { // GLFW
                printf("GLFW settings\n");
            } 
        }
    }
}

void parsePopupOutput(GLFWwindow *window) {
    if (popup.output[0] == 1) {
        popup.output[0] = 0; // untoggle
        if (popup.output[1] == 0) { // cancel
            turtle.close = 0;
            glfwSetWindowShouldClose(window, 0);
        }
        if (popup.output[1] == 1) { // close
            turtle.popupClose = 1;
        }
    }
}

int main(int argc, char *argv[]) {
    /* Initialize glfw */
    if (!glfwInit()) {
        return -1;
    }
    glfwWindowHint(GLFW_SAMPLES, 4); // MSAA (Anti-Aliasing) with 4 samples (must be done before window is created (?))

    /* Create a windowed mode window and its OpenGL context */
    const GLFWvidmode *monitorSize = glfwGetVideoMode(glfwGetPrimaryMonitor());
    int32_t windowHeight = monitorSize -> height;
    GLFWwindow *window = glfwCreateWindow(windowHeight * 16 / 9, windowHeight, "turtle demo", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowSizeLimits(window, windowHeight * 16 / 9 * 0.4, windowHeight * 0.4, windowHeight * 16 / 9, windowHeight);

    /* initialize turtle */
    turtleInit(window, -320, -180, 320, 180);
    glfwSetWindowSize(window, windowHeight * 16 / 9 * 0.85, monitorSize -> height * 0.85); // doing it this way ensures the window spawns in the top left of the monitor and fixes resizing limits
    /* initialise turtleText */
    turtleTextInit("config/roberto.tgl");
    /* initialise turtleTools ribbon */
    turtleToolsSetTheme(TT_THEME_DARK); // dark theme preset
    ribbonInit("config/ribbonConfig.txt");
    /* initialise turtleTools popup */
    popupInit("config/popupConfig.txt", -60, -20, 60, 20);
    /* initialise osTools */
    osToolsInit(argv[0], window); // must include argv[0] to get executableFilepath, must include GLFW window
    osToolsFileDialogAddExtension("txt"); // add txt to extension restrictions
    list_t *rowLike = osToolsLoadCSVString("config/test.csv", OSTOOLS_CSV_ROW);
    list_t *columnLike = osToolsLoadCSVString("config/test.csv", OSTOOLS_CSV_COLUMN);
    if (rowLike != NULL) {
        list_print(rowLike);
    }
    if (columnLike != NULL) {
        list_print(columnLike);
    }

    uint32_t tps = 120; // ticks per second (locked to fps in this case)
    uint64_t tick = 0; // count number of ticks since application started
    clock_t start, end;

    turtleBgColor(30, 30, 30);
    int8_t buttonVar, switchVar = 0;
    int32_t dropdownVar = 0;
    double dialVar = 0.0, sliderVar = 0.0, scrollbarVarX = 0.0, scrollbarVarY = 0.0;
    list_t *dropdownOptions = list_init();
    list_append(dropdownOptions, (unitype) "a", 's');
    list_append(dropdownOptions, (unitype) "long item", 's');
    list_append(dropdownOptions, (unitype) "very long item name", 's');
    list_append(dropdownOptions, (unitype) "b", 's');
    list_append(dropdownOptions, (unitype) "c", 's');
    list_append(dropdownOptions, (unitype) "d", 's');
    list_append(dropdownOptions, (unitype) "e", 's');
    tt_button_t *button = buttonInit("button", &buttonVar, 150, 20, 10);
    button -> shape = TT_BUTTON_SHAPE_ROUNDED_RECTANGLE;
    switchInit("switch", &switchVar, 150, -20, 10);
    dialInit("dial", &dialVar, TT_DIAL_EXP, -150, 20, 10, 0, 1000, 1);
    dialInit("dial", &dialVar, TT_DIAL_LINEAR, -150, -20, 10, 0, 1000, 1);
    sliderInit("slider", &sliderVar, TT_SLIDER_HORIZONTAL, TT_SLIDER_ALIGN_LEFT, -100, 35, 10, 50, 0, 255, 1);
    sliderInit("slider", &sliderVar, TT_SLIDER_HORIZONTAL, TT_SLIDER_ALIGN_CENTER, 0, 35, 10, 50, 0, 255, 1);
    sliderInit("slider", &sliderVar, TT_SLIDER_HORIZONTAL, TT_SLIDER_ALIGN_RIGHT, 100, 35, 10, 50, 0, 255, 1);
    sliderInit("slider", &sliderVar, TT_SLIDER_VERTICAL, TT_SLIDER_ALIGN_LEFT, -100, -35, 10, 50, 0, 255, 1);
    sliderInit("slider", &sliderVar, TT_SLIDER_VERTICAL, TT_SLIDER_ALIGN_CENTER, 0, -35, 10, 50, 0, 255, 1);
    sliderInit("slider", &sliderVar, TT_SLIDER_VERTICAL, TT_SLIDER_ALIGN_RIGHT, 100, -35, 10, 50, 0, 255, 1);
    scrollbarInit(&scrollbarVarX, TT_SCROLLBAR_HORIZONTAL, 20, -170, 10, 550, 50);
    scrollbarInit(&scrollbarVarY, TT_SCROLLBAR_VERTICAL, 310, 0, 10, 320, 33);
    dropdownInit("dropdown", dropdownOptions, &dropdownVar, TT_DROPDOWN_ALIGN_CENTER, 0, 70, 10);
    textboxInit("Textbox", 128, -50, -110, 10, 100);

    double power = 0.0, speed = 0.0, exposure = 0.0, x = 103, y = 95, z = 215;
    int8_t xEnabled = 0, yEnabled = 1, zEnabled = 0;
    list_t *sources = list_init();
    int sourceIndex = 0;
    list_append(sources, (unitype) "None", 's');
    list_append(sources, (unitype) "SP932", 's');
    list_append(sources, (unitype) "SP932U", 's');
    list_append(sources, (unitype) "SP928", 's');
    list_append(sources, (unitype) "SP1203", 's');
    list_append(sources, (unitype) "SP-1550M", 's');
    dialInit("Power", &power, TT_DIAL_LINEAR, -150, -210, 10, 0, 100, 1);
    dialInit("Speed", &speed, TT_DIAL_LINEAR, -100, -210, 10, 0, 1000, 1);
    dialInit("Exposure", &exposure, TT_DIAL_EXP, -50, -210, 10, 0, 1000, 1);
    dropdownInit("Source", sources, &sourceIndex, TT_DROPDOWN_ALIGN_LEFT, -10, -207, 10);
    tt_slider_t *xSlider = sliderInit("", &x, TT_SLIDER_HORIZONTAL, TT_SLIDER_ALIGN_CENTER, -100, -240, 10, 100, -300, 300, 0);
    tt_slider_t *ySlider = sliderInit("", &y, TT_SLIDER_HORIZONTAL, TT_SLIDER_ALIGN_CENTER, -100, -260, 10, 100, -300, 300, 0);
    tt_slider_t *zSlider = sliderInit("", &z, TT_SLIDER_HORIZONTAL, TT_SLIDER_ALIGN_CENTER, -100, -280, 10, 100, -300, 300, 0);
    switchInit("", &xEnabled, -10, -240, 10);
    switchInit("", &yEnabled, -10, -260, 10);
    switchInit("", &zEnabled, -10, -280, 10);

    int8_t sideswipe = 0, checkbox = 1, xbox = 1;
    tt_switch_t *switch_sideswipe = switchInit("Side Swipe", &sideswipe, 305, 15, 10);
    tt_switch_t *switch_checkbox = switchInit("Checkbox", &checkbox, 300, 0, 10);
    tt_switch_t *switch_xbox = switchInit("Xbox", &xbox, 300, -15, 10);
    switch_sideswipe -> style = TT_SWITCH_STYLE_SIDESWIPE_LEFT;
    switch_checkbox -> style = TT_SWITCH_STYLE_CHECKBOX;
    switch_xbox -> style = TT_SWITCH_STYLE_XBOX;
    int8_t textButton = 0, circleButton = 0;
    tt_button_t *button_textButton = buttonInit("Text Button", &textButton, 330, -30, 10);
    tt_button_t *button_circleButton = buttonInit("Circle Button", &circleButton, 338, -88, 10);
    button_textButton -> shape = TT_BUTTON_SHAPE_TEXT;
    button_circleButton -> shape = TT_BUTTON_SHAPE_CIRCLE;


    list_t *xPositions = list_init();
    list_t *yPositions = list_init();
    for (uint32_t i = 0; i < tt_elements.all -> length; i++) {
        list_append(xPositions, (unitype) ((tt_button_t *) tt_elements.all -> data[i].p) -> x, 'd');
        list_append(yPositions, (unitype) ((tt_button_t *) tt_elements.all -> data[i].p) -> y, 'd');
    }

    double scroll = 0.0;
    double scrollFactor = 15;
    while (turtle.popupClose == 0) {
        start = clock();
        turtleGetMouseCoords();
        turtleClear();
        /* update element positions */
        for (uint32_t i = 0; i < tt_elements.all -> length; i++) {
            if (((tt_button_t *) tt_elements.all -> data[i].p) -> element != TT_ELEMENT_SCROLLBAR) {
                ((tt_button_t *) tt_elements.all -> data[i].p) -> x = xPositions -> data[i].d - scrollbarVarX * 5;
                ((tt_button_t *) tt_elements.all -> data[i].p) -> y = yPositions -> data[i].d + scrollbarVarY * 3.3;
            }
        }

        /* write element annotations */
        tt_setColor(TT_COLOR_TEXT);
        turtleTextWriteStringf(-310, -170, 5, 0, "%.2lf, %.2lf", turtle.mouseX, turtle.mouseY);
        turtleTextWriteString("X", xSlider -> x - xSlider -> length / 2 - xSlider -> size, xSlider -> y, xSlider -> size - 1, 100);
        turtleTextWriteStringf(ySlider -> x + xSlider -> length / 2 + xSlider -> size, xSlider -> y, 4, 0, "%.01lf", round(x) / 10);
        turtleTextWriteString("Y", xSlider -> x - ySlider -> length / 2 - xSlider -> size, ySlider -> y, xSlider -> size - 1, 100);
        turtleTextWriteStringf(ySlider -> x + ySlider -> length / 2 + xSlider -> size, ySlider -> y, 4, 0, "%.01lf", round(y) / 10);
        turtleTextWriteString("Z", zSlider -> x - zSlider -> length / 2 - xSlider -> size, zSlider -> y, xSlider -> size - 1, 100);
        turtleTextWriteStringf(zSlider -> x + zSlider -> length / 2 + xSlider -> size, zSlider -> y, 4, 0, "%.01lf", round(z) / 10);

        /* write all characters supported */
        turtleTextWriteUnicode((uint8_t *) u8"AÀÁĂÄÃÅĀĄÆBCĆČĊÇDĎĐÐEÈÉĚÊËĒĖĘƏFGĞĠHĦ", scrollbarVarX * -5 + 320, scrollbarVarY * 3.3 - 180, 10, 0);
        turtleTextWriteUnicode((uint8_t *) u8"IÌÍÎÏĪİĮJKĶLĹĽĻŁĿMNŃŇÑŅOÒÓÔÖÕŐØŒPQRŔŘSŚŠŞȘ", scrollbarVarX * -5 + 320, scrollbarVarY * 3.3 - 195, 10, 0);
        turtleTextWriteUnicode((uint8_t *) u8"ẞTŤȚÞUÙÚÛÜŮŰŪŲVWXYÝZŹŽŻaàáâăäãåāąæbcć", scrollbarVarX * -5 + 320, scrollbarVarY * 3.3 - 210, 10, 0);
        turtleTextWriteUnicode((uint8_t *) u8"čċçdďđðeèéěêëēėęəfgğġhħiìíîïīıįjkķlĺľļłŀmnńňñņoòóôö", scrollbarVarX * -5 + 320, scrollbarVarY * 3.3 - 225, 10, 0);
        turtleTextWriteUnicode((uint8_t *) u8"õőøœpqrŕřsśšşșßtťțþuùúûüůűūųvwxyýzźžż", scrollbarVarX * -5 + 320, scrollbarVarY * 3.3 - 240, 10, 0);
        turtleTextWriteUnicode((uint8_t *) u8"АБВГҐҒДЂЕЁЄӘЖӁЗИӢЙІЇЈКҚҜЛЉМНҢЊОӨПРСТ", scrollbarVarX * -5 + 320, scrollbarVarY * 3.3 - 255, 10, 0);
        turtleTextWriteUnicode((uint8_t *) u8"ЋУӮҮҰЎФХҲҺЦЧҶҸЏШЩЪЫЬЭЮЯабвгґғдђеёєәж", scrollbarVarX * -5 + 320, scrollbarVarY * 3.3 - 270, 10, 0);
        turtleTextWriteUnicode((uint8_t *) u8"ӂзиӣйіїјкқҝлљмнңњоөпрстћуӯүұўфхҳһцчҷҹџшщъ", scrollbarVarX * -5 + 320, scrollbarVarY * 3.3 - 285, 10, 0);
        turtleTextWriteUnicode((uint8_t *) u8"ыьэюя", scrollbarVarX * -5 + 320, scrollbarVarY * 3.3 - 300, 10, 0);
        turtleTextWriteUnicode((uint8_t *) u8"ΑΒΓΔΕΖΗΘΙΚΛΜΝΞΟΠΡΣΤΥΦΧΨΩ", scrollbarVarX * -5 + 320, scrollbarVarY * 3.3 - 315, 10, 0);
        turtleTextWriteUnicode((uint8_t *) u8"αβγδεζηθικλμνξοπρσςτυφχψω", scrollbarVarX * -5 + 320, scrollbarVarY * 3.3 - 330, 10, 0);
        turtleTextWriteUnicode((uint8_t *) u8"1234567890!@#$£€₺₽¥₩₹₣฿%^&*()`~-_=+[{]}\\|;:‘'’“\"”,", scrollbarVarX * -5 + 320, scrollbarVarY * 3.3 - 345, 10, 0);
        turtleTextWriteUnicode((uint8_t *) u8"<.>/?½¨", scrollbarVarX * -5 + 320, scrollbarVarY * 3.3 - 360, 10, 0);

        // turtlePenColor(0, 0, 0);
        // turtle3DTriangle(-5, 0, 10, 5, 0, 10, 0, 5, 10);

        scroll = turtleMouseWheel();
        if (scroll != 0) {
            if (turtleKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
                scrollbarVarX -= scroll * scrollFactor;
                if (scrollbarVarX < 0) {
                    scrollbarVarX = 0;
                }
                if (scrollbarVarX > 100) {
                    scrollbarVarX = 100;
                }
            } else {
                scrollbarVarY -= scroll * scrollFactor;
                if (scrollbarVarY < 0) {
                    scrollbarVarY = 0;
                }
                if (scrollbarVarY > 100) {
                    scrollbarVarY = 100;
                }
            }
        }
        turtleToolsUpdate(); // update turtleTools
        parseRibbonOutput(); // user defined function to use ribbon
        parsePopupOutput(window); // user defined function to use popup
        turtleUpdate(); // update the screen
        end = clock();
        while ((double) (end - start) / CLOCKS_PER_SEC < (1.0 / tps)) {
            end = clock();
        }
        tick++;
    }
    turtleFree();
    glfwTerminate();
    return 0;
}