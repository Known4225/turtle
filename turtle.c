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
                if (strcmp(osFileDialog.selectedFilename, "null") == 0) {
                    if (osFileDialogPrompt(1, "") != -1) {
                        printf("Saved to: %s\n", osFileDialog.selectedFilename);
                    }
                } else {
                    printf("Saved to: %s\n", osFileDialog.selectedFilename);
                }
            }
            if (ribbonRender.output[2] == 3) { // Save As...
                if (osFileDialogPrompt(1, "") != -1) {
                    printf("Saved to: %s\n", osFileDialog.selectedFilename);
                }
            }
            if (ribbonRender.output[2] == 4) { // Open
                if (osFileDialogPrompt(0, "") != -1) {
                    printf("Loaded data from: %s\n", osFileDialog.selectedFilename);
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
                osClipboardSetText("test123");
                printf("Cut \"test123\" to clipboard!\n");
            }
            if (ribbonRender.output[2] == 4) { // Copy
                osClipboardSetText("test345");
                printf("Copied \"test345\" to clipboard!\n");
            }
            if (ribbonRender.output[2] == 5) { // Paste
                osClipboardGetText();
                printf("Pasted \"%s\" from clipboard!\n", osClipboard.text);
            }
        }
        if (ribbonRender.output[1] == 2) { // View
            if (ribbonRender.output[2] == 1) { // Change theme
                printf("Change theme\n");
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
    int windowHeight = monitorSize -> height * 0.85;
    GLFWwindow *window = glfwCreateWindow(windowHeight * 16 / 9, windowHeight, "turtle", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowSizeLimits(window, 128, 72, windowHeight * 16 / 9, windowHeight);

    /* initialize turtle */
    turtleInit(window, -320, -180, 320, 180);
    /* initialise turtleText */
    turtleTextInit(window, "include/fontBez.tgl");
    /* initialise turtleTools ribbon */
    ribbonInit(window, "include/ribbonConfig.txt");
    ribbonDarkTheme(); // dark theme preset
    /* initialise popup */
    char constructedPath[4097 + 32];
    strcpy(constructedPath, osFileDialog.executableFilepath);
    strcat(constructedPath, "include/popupConfig.txt");
    popupInit(constructedPath, -60, -20, 60, 20);
    popupDarkTheme(); // dark theme preset
    /* initialise osTools */
    osToolsInit(argv[0], window); // must include argv[0] to get executableFilepath, must include GLFW window
    osFileDialogAddExtension("txt"); // add txt to extension restrictions

    uint32_t tps = 120; // ticks per second (locked to fps in this case)
    uint64_t tick = 0; // count number of ticks since application started
    clock_t start, end;

    turtleBgColor(30, 30, 30);

    while (turtle.shouldClose == 0) {
        start = clock();
        turtleGetMouseCoords();
        turtleClear();
        ribbonUpdate();
        parseRibbonOutput();
        popupUpdate();
        parsePopupOutput(window);
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