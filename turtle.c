#include "include/turtleTools.h"
#include "include/osTools.h"
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
                    turtleBgColor(255, 255, 255);
                    turtleToolsLightTheme();
                } else {
                    turtleBgColor(30, 30, 30);
                    turtleToolsDarkTheme();
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
            turtle.shouldClose = 1;
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
    int32_t windowHeight = monitorSize -> height * 0.85;
    GLFWwindow *window = glfwCreateWindow(windowHeight * 16 / 9, windowHeight, "turtle", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowSizeLimits(window, windowHeight * 16 / 9, windowHeight, windowHeight * 16 / 9, windowHeight);

    /* initialize turtle */
    turtleInit(window, -320, -180, 320, 180);
    /* initialise turtleText */
    turtleTextInit("include/roberto.tgl");
    /* initialise turtleTools ribbon */
    turtleToolsDarkTheme(); // dark theme preset
    ribbonInit("include/ribbonConfig.txt");
    /* initialise turtleTools popup */
    popupInit("include/popupConfig.txt", -60, -20, 60, 20);
    /* initialise osTools */
    osToolsInit(argv[0], window); // must include argv[0] to get executableFilepath, must include GLFW window
    osToolsFileDialogAddExtension("txt"); // add txt to extension restrictions

    uint32_t tps = 120; // ticks per second (locked to fps in this case)
    uint64_t tick = 0; // count number of ticks since application started
    clock_t start, end;

    turtleBgColor(30, 30, 30);
    int32_t buttonVar, switchVar = 0, dropdownVar = 0;
    double dialVar = 0.0, sliderVar = 0.0, scrollbarVar = 0.0;
    list_t *dropdownOptions = list_init();
    list_append(dropdownOptions, (unitype) "a", 's');
    list_append(dropdownOptions, (unitype) "long item", 's');
    list_append(dropdownOptions, (unitype) "very long item name", 's');
    list_append(dropdownOptions, (unitype) "b", 's');
    list_append(dropdownOptions, (unitype) "c", 's');
    list_append(dropdownOptions, (unitype) "d", 's');
    list_append(dropdownOptions, (unitype) "e", 's');
    buttonInit("button", &buttonVar, TT_BUTTON_SHAPE_RECTANGLE, 150, 20, 10);
    switchInit("switch", &switchVar, 150, -20, 10);
    dialInit("dial", &dialVar, TT_DIAL_EXP, -150, 20, 10, 0, 1000, 1);
    dialInit("dial", &dialVar, TT_DIAL_LINEAR, -150, -20, 10, 0, 1000, 1);
    sliderInit("slider", &sliderVar, TT_SLIDER_HORIZONTAL, TT_SLIDER_ALIGN_LEFT, -100, 35, 10, 50, 0, 255, 1);
    sliderInit("slider", &sliderVar, TT_SLIDER_HORIZONTAL, TT_SLIDER_ALIGN_CENTER, 0, 35, 10, 50, 0, 255, 1);
    sliderInit("slider", &sliderVar, TT_SLIDER_HORIZONTAL, TT_SLIDER_ALIGN_RIGHT, 100, 35, 10, 50, 0, 255, 1);
    sliderInit("slider", &sliderVar, TT_SLIDER_VERTICAL, TT_SLIDER_ALIGN_LEFT, -100, -35, 10, 50, 0, 255, 1);
    sliderInit("slider", &sliderVar, TT_SLIDER_VERTICAL, TT_SLIDER_ALIGN_CENTER, 0, -35, 10, 50, 0, 255, 1);
    sliderInit("slider", &sliderVar, TT_SLIDER_VERTICAL, TT_SLIDER_ALIGN_RIGHT, 100, -35, 10, 50, 0, 255, 1);
    scrollbarInit(&scrollbarVar, TT_SCROLLBAR_VERTICAL, 300, 0, 10, 300, 33);
    dropdownInit("dropdown", dropdownOptions, &dropdownVar, TT_DROPDOWN_ALIGN_CENTER, 0, 70, 10);

    list_t *yPositions = list_init();
    for (uint32_t i = 0; i < tt_elements.all -> length; i++) {
        list_append(yPositions, (unitype) ((tt_button_t *) tt_elements.all -> data[i].p) -> y, 'd');
    }

    while (turtle.shouldClose == 0) {
        start = clock();
        turtleGetMouseCoords();
        turtleClear();
        char coordsStr[16];
        sprintf(coordsStr, "%.2lf, %.2lf", turtle.mouseX, turtle.mouseY);
        tt_setColor(TT_COLOR_TEXT);
        turtleTextWriteString(coordsStr, -310, -170, 5, 0);
        for (uint32_t i = 0; i < tt_elements.all -> length; i++) {
            if (((tt_button_t *) tt_elements.all -> data[i].p) -> element != TT_ELEMENT_SCROLLBAR) {
                ((tt_button_t *) tt_elements.all -> data[i].p) -> y = yPositions -> data[i].d + scrollbarVar * 3;
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