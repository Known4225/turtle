// #define TURTLE_IMPLEMENTATION
#include "turtle.h"
#include <time.h>

void parseRibbonOutput() {
    if (tt_ribbon.output[0] == 0) {
        return;
    }
    tt_ribbon.output[0] = 0;
    if (tt_ribbon.output[1] == 0) { // File
        if (tt_ribbon.output[2] == 1) { // New
            list_clear(osToolsFileDialog.selectedFilenames);
            printf("New\n");
        }
        if (tt_ribbon.output[2] == 2) { // Save
            if (osToolsFileDialog.selectedFilenames -> length == 0) {
                if (osToolsFileDialogSave(OSTOOLS_FILE_DIALOG_FILE, "Save.txt", NULL) != -1) {
                    printf("Saved to: %s\n", osToolsFileDialog.selectedFilenames -> data[0].s);
                }
            } else {
                printf("Saved to: %s\n", osToolsFileDialog.selectedFilenames -> data[0].s);
            }
        }
        if (tt_ribbon.output[2] == 3) { // Save As...
            list_clear(osToolsFileDialog.selectedFilenames);
            if (osToolsFileDialogSave(OSTOOLS_FILE_DIALOG_FILE, "Save.txt", NULL) != -1) {
                printf("Saved to: %s\n", osToolsFileDialog.selectedFilenames -> data[0].s);
            }
        }
        if (tt_ribbon.output[2] == 4) { // Open
            list_clear(osToolsFileDialog.selectedFilenames);
            if (osToolsFileDialogOpen(OSTOOLS_FILE_DIALOG_MULTIPLE_SELECT, OSTOOLS_FILE_DIALOG_FILE, "", NULL) != -1) {
                printf("Loaded data from: ");
                list_print(osToolsFileDialog.selectedFilenames);
            }
        }
    }
    if (tt_ribbon.output[1] == 1) { // Edit
        if (tt_ribbon.output[2] == 1) { // Undo
            printf("Undo\n");
        }
        if (tt_ribbon.output[2] == 2) { // Redo
            printf("Redo\n");
        }
        if (tt_ribbon.output[2] == 3) { // Cut
            osToolsClipboardSetText("test123");
            printf("Cut \"test123\" to clipboard!\n");
        }
        if (tt_ribbon.output[2] == 4) { // Copy
            osToolsClipboardSetText("test345");
            printf("Copied \"test345\" to clipboard!\n");
        }
        if (tt_ribbon.output[2] == 5) { // Paste
            osToolsClipboardGetText();
            printf("Pasted \"%s\" from clipboard!\n", osToolsClipboard.text);
        }
    }
    if (tt_ribbon.output[1] == 2) { // View
        if (tt_ribbon.output[2] == 1) { // Change theme
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
        if (tt_ribbon.output[2] == 2) { // GLFW
            printf("GLFW settings\n");
        } 
    }
}

void parsePopupOutput(GLFWwindow *window) {
    if (tt_popup.output[0] == 0) {
        return;
    }
    tt_popup.output[0] = 0; // untoggle
    if (tt_popup.output[1] == 0) { // cancel
        turtle.close = 0;
        glfwSetWindowShouldClose(window, 0);
    }
    if (tt_popup.output[1] == 1) { // close
        turtle.popupClose = 1;
    }
}

int main(int argc, char *argv[]) {
    /* Initialise glfw */
    if (!glfwInit()) {
        return -1;
    }
    glfwWindowHint(GLFW_SAMPLES, 4); // MSAA (Anti-Aliasing) with 4 samples (must be done before window is created (?))

    /* Create a windowed mode window and its OpenGL context */
    const GLFWvidmode *monitorSize = glfwGetVideoMode(glfwGetPrimaryMonitor());
    int32_t windowHeight = monitorSize -> height;
    double optimizedScalingFactor = 0.8; // Set this number to 1 on windows and 0.8 on Ubuntu for maximum compatibility (fixes issue with incorrect stretching)
    #ifdef OS_WINDOWS
    optimizedScalingFactor = 1;
    #endif
    #ifdef OS_LINUX
    optimizedScalingFactor = 0.8;
    #endif
    GLFWwindow *window = glfwCreateWindow(windowHeight * 16 / 9 * optimizedScalingFactor, windowHeight * optimizedScalingFactor, "turtle demo", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowSizeLimits(window, windowHeight * 16 / 9 * 0.4, windowHeight * 0.4, windowHeight * 16 / 9 * optimizedScalingFactor, windowHeight * optimizedScalingFactor);
    /* initialise logo */
    GLFWimage icon;
    int32_t iconChannels;
    char constructedFilepath[5120];
    strcpy(constructedFilepath, osToolsFileDialog.executableFilepath);
    strcat(constructedFilepath, "images/thumbnail.png");
    uint8_t *iconPixels = stbi_load(constructedFilepath, &icon.width, &icon.height, &iconChannels, 4); // 4 color channels for RGBA
    if (iconPixels != NULL) {
        icon.pixels = iconPixels;
        glfwSetWindowIcon(window, 1, &icon);
        glfwPollEvents(); // update taskbar icon correctly on windows - https://github.com/glfw/glfw/issues/2753
        free(iconPixels);
    } else {
        printf("Could not load thumbnail %s\n", constructedFilepath);
    }

    /* initialise turtle */
    turtleInit(window, -320, -180, 320, 180);
    #ifdef OS_LINUX
    glfwSetWindowPos(window, 0, 36);
    #endif
    if (optimizedScalingFactor > 0.85) {
        glfwSetWindowSize(window, windowHeight * 16 / 9 * 0.865, windowHeight * 0.85); // doing it this way ensures the window spawns in the top left of the monitor and fixes resizing limits
    } else {
        glfwSetWindowSize(window, windowHeight * 16 / 9 * optimizedScalingFactor, windowHeight * optimizedScalingFactor);
    }
    /* initialise osTools */
    osToolsInit(argv[0], window); // must include argv[0] to get executableFilepath, must include GLFW window for copy paste and cursor functionality
    osToolsFileDialogAddGlobalExtension("txt"); // add txt to extension restrictions
    osToolsFileDialogAddGlobalExtension("csv"); // add csv to extension restrictions
    /* initialise turtleText */
    strcpy(constructedFilepath, osToolsFileDialog.executableFilepath);
    strcat(constructedFilepath, "config/roberto.tgl");
    turtleTextInit(constructedFilepath);
    /* initialise turtleTools ribbon */
    turtleToolsSetTheme(TT_THEME_DARK); // dark theme preset
    strcpy(constructedFilepath, osToolsFileDialog.executableFilepath);
    strcat(constructedFilepath, "config/ribbonConfig.txt");
    ribbonInit(constructedFilepath);
    // list_t *ribbonConfig = list_init();
    // list_append(ribbonConfig, (unitype) "File, New, Save, Save As..., Open", 's');
    // list_append(ribbonConfig, (unitype) "Edit, Undo, Redo, Cut, Copy, Paste", 's');
    // list_append(ribbonConfig, (unitype) "View, Change Theme, GLFW", 's');
    // ribbonInitList(ribbonConfig);
    /* initialise turtleTools popup */
    strcpy(constructedFilepath, osToolsFileDialog.executableFilepath);
    strcat(constructedFilepath, "config/popupConfig.txt");
    popupInit(constructedFilepath);
    // list_t *popupConfig = list_init();
    // list_append(popupConfig, (unitype) "Are you sure you want to close?", 's');
    // list_append(popupConfig, (unitype) "Cancel", 's');
    // list_append(popupConfig, (unitype) "Close", 's');
    // popupInitList(popupConfig);
    strcpy(constructedFilepath, osToolsFileDialog.executableFilepath);
    strcat(constructedFilepath, "config/test.csv");
    list_t *rowLike = osToolsLoadCSVString(constructedFilepath, OSTOOLS_CSV_ROW);
    list_t *columnLike = osToolsLoadCSVString(constructedFilepath, OSTOOLS_CSV_COLUMN);
    if (rowLike != NULL) {
        list_print(rowLike);
    }
    if (columnLike != NULL) {
        list_print(columnLike);
    }
    /* textures */
    turtle_texture_t empvImage = turtleTextureLoad("images/EMPV.png");
    // uint8_t array[16] = {
    //     100, 100, 100, 100,
    //     100, 100, 100, 100,
    //     100, 100, 100, 100,
    //     100, 100, 100, 100,
    // };
    // turtle_texture_t empvImage = turtleTextureLoadArray(array, 4, 4, GL_GREEN);
    turtleTexturePrint(empvImage);
    list_t *folders = osToolsFolderList(".");
    list_t *files = osToolsFileList(".");
    list_t *filesAndFolders = osToolsFileAndFolderList(".");
    list_print(folders);
    list_print(files);
    list_print(filesAndFolders);
    list_free(folders);
    list_free(files);
    list_free(filesAndFolders);
    list_t *serialPorts = osToolsSerialList();
    printf("Serial Ports: ");
    list_print(serialPorts);
    for (int32_t i = 0; i < serialPorts -> length; i++) {
        osToolsSerialOpen(serialPorts -> data[i].s, OSTOOLS_BAUD_115200);
        osToolsSerialSend(serialPorts -> data[i].s, (uint8_t *) "Hello World\r\n", strlen("Hello World\r\n"));
        osToolsSerialClose(serialPorts -> data[i].s);
    }
    list_free(serialPorts);
    /* Server testing */
    // osToolsServerSocketCreate("Server1", OSTOOLS_PROTOCOL_TCP, "6000");
    // osToolsServerSocketListen("Server1", "Client1");
    // osToolsSocketSend("Client1", (uint8_t *) "Hello World\r\n", strlen("Hello World\r\n"));
    // uint8_t *buffer = calloc(128, 1);
    // osToolsSocketReceive("Client1", buffer, 128, 10000);
    // printf("Received: %s\n", buffer);
    // free(buffer);
    // osToolsSocketDestroy("Client1");
    /* Client testing */
    // osToolsClientSocketCreate("Client1", OSTOOLS_PROTOCOL_TCP, "127.0.0.1", "6000", 10000);
    // osToolsSocketSend("Client1", (uint8_t *) "Hello World\r\n", strlen("Hello World\r\n"));
    // uint8_t *buffer = calloc(128, 1);
    // osToolsSocketReceive("Client1", buffer, 128, 10000);
    // printf("Received: %s\n", buffer);
    // free(buffer);
    // osToolsSocketDestroy("Client1");

    list_t *cameras = osToolsCameraList();
    printf("Cameras: ");
    list_print(cameras);
    char *cameraName = NULL;
    uint8_t *cameraFrame = NULL;
    list_t *imageDropdownOptions = list_init();
    list_append(imageDropdownOptions, (unitype) "Image", 's');
    for (int32_t i = 0; i < cameras -> length; i += 4) {
        list_append(imageDropdownOptions, cameras -> data[i], 's');
    }
    tt_dropdown_t *imageDropdown = dropdownInit("Source", imageDropdownOptions, NULL, TT_DROPDOWN_ALIGN_RIGHT, 700, 36, 8);
    int32_t oldImageDropdown = imageDropdown -> value;

    uint32_t tps = 120; // ticks per second (locked to fps in this case)
    uint64_t tick = 0; // count number of ticks since application started
    clock_t start, end;

    double sliderVar, dialVar;
    tt_button_t *button = buttonInit("Button", NULL, 150, 20, 10);
    button -> shape = TT_BUTTON_SHAPE_ROUNDED_RECTANGLE;
    switchInit("Switch", NULL, 150, -20, 10);
    dialInit("Exp", &dialVar, TT_DIAL_SCALE_EXP, -150, 20, 10, 0, 1000, 1);
    dialInit("Linear", &dialVar, TT_DIAL_SCALE_LINEAR, -150, -20, 10, 0, 1000, 1);
    dialInit("Log", &dialVar, TT_DIAL_SCALE_LOG, -150, -60, 10, 0, 1000, 1);
    sliderInit("Slider", NULL, TT_SLIDER_TYPE_HORIZONTAL, TT_SLIDER_ALIGN_LEFT, -100, 35, 10, 50, 0, 255, 1);
    sliderInit("Slider", NULL, TT_SLIDER_TYPE_HORIZONTAL, TT_SLIDER_ALIGN_CENTER, 0, 35, 10, 50, 0, 255, 1);
    sliderInit("Slider", NULL, TT_SLIDER_TYPE_HORIZONTAL, TT_SLIDER_ALIGN_RIGHT, 100, 35, 10, 50, 0, 255, 1);
    sliderInit("Log", &sliderVar, TT_SLIDER_TYPE_VERTICAL, TT_SLIDER_ALIGN_LEFT, -100, -35, 10, 50, 0, 255, 1) -> scale = TT_SLIDER_SCALE_LOG;
    sliderInit("Linear", &sliderVar, TT_SLIDER_TYPE_VERTICAL, TT_SLIDER_ALIGN_CENTER, 0, -35, 10, 50, 0, 255, 1) -> scale = TT_SLIDER_SCALE_LINEAR;
    sliderInit("Exp", &sliderVar, TT_SLIDER_TYPE_VERTICAL, TT_SLIDER_ALIGN_RIGHT, 100, -35, 10, 50, 0, 255, 1) -> scale = TT_SLIDER_SCALE_EXP;
    tt_scrollbar_t *scrollbarX = scrollbarInit(NULL, TT_SCROLLBAR_HORIZONTAL, 20, -170, 10, 550, 50);
    tt_scrollbar_t *scrollbarY = scrollbarInit(NULL, TT_SCROLLBAR_VERTICAL, 310, 0, 10, 320, 33);
    list_t *dropdownOptions = list_init();
    list_append(dropdownOptions, (unitype) "A", 's');
    list_append(dropdownOptions, (unitype) "Long Item", 's');
    list_append(dropdownOptions, (unitype) "Very Long Item Name", 's');
    list_append(dropdownOptions, (unitype) "B", 's');
    list_append(dropdownOptions, (unitype) "C", 's');
    list_append(dropdownOptions, (unitype) "D", 's');
    list_append(dropdownOptions, (unitype) "E", 's');
    tt_dropdown_t *dropdown = dropdownInit("Dropdown", dropdownOptions, NULL, TT_DROPDOWN_ALIGN_CENTER, 0, 70, 10);
    tt_textbox_t *textbox = textboxInit("Textbox", NULL, 128, -50, -110, 10, 100);
    list_t *contextOptions = list_init();
    list_append(contextOptions, (unitype) "Button", 's');
    list_append(contextOptions, (unitype) "Switch", 's');
    list_append(contextOptions, (unitype) "Dial", 's');
    list_append(contextOptions, (unitype) "Slider", 's');
    list_append(contextOptions, (unitype) "Scrollbar", 's');
    list_append(contextOptions, (unitype) "Context", 's');
    list_append(contextOptions, (unitype) "Dropdown", 's');
    list_append(contextOptions, (unitype) "Textbox", 's');
    tt_context_t *context = contextInit(contextOptions, NULL, 0, 0, 10);
    context -> enabled = TT_ELEMENT_HIDE;

    double x = 103, y = 95, z = 215;
    list_t *sources = list_init();
    list_append(sources, (unitype) "None", 's');
    list_append(sources, (unitype) "SP932", 's');
    list_append(sources, (unitype) "SP932U", 's');
    list_append(sources, (unitype) "SP928", 's');
    list_append(sources, (unitype) "SP1203", 's');
    list_append(sources, (unitype) "SP-1550M", 's');
    dialInit("Power", NULL, TT_DIAL_SCALE_LINEAR, -150, -210, 10, 0, 100, 1);
    dialInit("Speed", NULL, TT_DIAL_SCALE_LINEAR, -100, -210, 10, 0, 1000, 1);
    dialInit("Exposure", NULL, TT_DIAL_SCALE_EXP, -50, -210, 10, 0, 1000, 1);
    dropdownInit("Source", sources, NULL, TT_DROPDOWN_ALIGN_LEFT, -10, -207, 10);
    tt_slider_t *xSlider = sliderInit("", &x, TT_SLIDER_TYPE_HORIZONTAL, TT_SLIDER_ALIGN_CENTER, -100, -240, 10, 100, -300, 300, 0);
    tt_slider_t *ySlider = sliderInit("", &y, TT_SLIDER_TYPE_HORIZONTAL, TT_SLIDER_ALIGN_CENTER, -100, -260, 10, 100, -300, 300, 0);
    tt_slider_t *zSlider = sliderInit("", &z, TT_SLIDER_TYPE_HORIZONTAL, TT_SLIDER_ALIGN_CENTER, -100, -280, 10, 100, -300, 300, 0);
    switchInit("", NULL, -10, -240, 10);
    switchInit("", NULL, -10, -260, 10);
    switchInit("", NULL, -10, -280, 10);

    tt_switch_t *sideswipe = switchInit("Side Swipe", NULL, 305, 15, 10);
    tt_switch_t *checkbox = switchInit("Checkbox", NULL, 300, 0, 10);
    tt_switch_t *xbox = switchInit("Xbox", NULL, 300, -15, 10);
    sideswipe -> style = TT_SWITCH_STYLE_SIDESWIPE_LEFT;
    checkbox -> value = 1;
    checkbox -> style = TT_SWITCH_STYLE_CHECKBOX;
    xbox -> value = 1;
    xbox -> style = TT_SWITCH_STYLE_XBOX;
    tt_button_t *textButton = buttonInit("Text Button", NULL, 330, -30, 10);
    tt_button_t *circleButton = buttonInit("Circle Button", NULL, 338, -100, 10);
    textButton -> shape = TT_BUTTON_SHAPE_TEXT;
    circleButton -> shape = TT_BUTTON_SHAPE_CIRCLE;

    list_t *xPositions = list_init();
    list_t *yPositions = list_init();
    for (uint32_t i = 0; i < tt_elements.all -> length; i++) {
        list_append(xPositions, (unitype) ((tt_button_t *) tt_elements.all -> data[i].p) -> x, 'd');
        list_append(yPositions, (unitype) ((tt_button_t *) tt_elements.all -> data[i].p) -> y, 'd');
    }

    double scroll = 0.0;
    double scrollFactor = 15;
    char keys[8] = {0};
    while (turtle.popupClose == 0) {
        start = clock();
        turtleGetMouseCoords();
        turtleClear();
        /* update element positions */
        for (uint32_t i = 0; i < tt_elements.all -> length; i++) {
            if (((tt_button_t *) tt_elements.all -> data[i].p) -> element != TT_ELEMENT_SCROLLBAR && ((tt_button_t *) tt_elements.all -> data[i].p) -> element != TT_ELEMENT_CONTEXT) {
                ((tt_button_t *) tt_elements.all -> data[i].p) -> x = xPositions -> data[i].d - scrollbarX -> value * 5;
                ((tt_button_t *) tt_elements.all -> data[i].p) -> y = yPositions -> data[i].d + scrollbarY -> value * 3.3;
            }
        }

        /* write element annotations */
        tt_setColor(TT_COLOR_TEXT);
        turtleTextWriteString("X", xSlider -> x - xSlider -> length / 2 - xSlider -> size, xSlider -> y, xSlider -> size - 1, 100);
        turtleTextWriteStringf(ySlider -> x + xSlider -> length / 2 + xSlider -> size, xSlider -> y, 4, 0, "%.01lf", round(x) / 10);
        turtleTextWriteString("Y", xSlider -> x - ySlider -> length / 2 - xSlider -> size, ySlider -> y, xSlider -> size - 1, 100);
        turtleTextWriteStringf(ySlider -> x + ySlider -> length / 2 + xSlider -> size, ySlider -> y, 4, 0, "%.01lf", round(y) / 10);
        turtleTextWriteString("Z", zSlider -> x - zSlider -> length / 2 - xSlider -> size, zSlider -> y, xSlider -> size - 1, 100);
        turtleTextWriteStringf(zSlider -> x + zSlider -> length / 2 + xSlider -> size, zSlider -> y, 4, 0, "%.01lf", round(z) / 10);
        if (textbox -> mouseOver) {
            osToolsSetCursor(GLFW_IBEAM_CURSOR);
        } else {
            osToolsSetCursor(GLFW_ARROW_CURSOR);
        }

        /* write all characters supported */
        turtleTextWriteUnicode("AÀÁĂÄÃÅĀĄÆBCĆČĊÇDĎĐÐEÈÉĚÊËĒĖĘƏFGĞĠHĦ", scrollbarX -> value * -5 + 260, scrollbarY -> value * 3.3 - 180, 10, 0);
        turtleTextWriteUnicode("IÌÍÎÏĪİĮJKĶLĹĽĻŁĿMNŃŇÑŅOÒÓÔÖÕŐØŒPQRŔ", scrollbarX -> value * -5 + 260, scrollbarY -> value * 3.3 - 195, 10, 0);
        turtleTextWriteUnicode("ŘSŚŠŞȘẞTŤȚÞUÙÚÛÜŮŰŪŲVWXYÝZŹŽŻaàáâăäã", scrollbarX -> value * -5 + 260, scrollbarY -> value * 3.3 - 210, 10, 0);
        turtleTextWriteUnicode("åāąæbcćčċçdďđðeèéěêëēėęəfgğġhħiìíîïī", scrollbarX -> value * -5 + 260, scrollbarY -> value * 3.3 - 225, 10, 0);
        turtleTextWriteUnicode("ıįjkķlĺľļłŀmnńňñņoòóôöõőøœpqrŕřsśšşș", scrollbarX -> value * -5 + 260, scrollbarY -> value * 3.3 - 240, 10, 0);
        turtleTextWriteUnicode("ßtťțþuùúûüůűūųvwxyýzźžżАБВГҐҒДЂЕЁЄӘЖ", scrollbarX -> value * -5 + 260, scrollbarY -> value * 3.3 - 255, 10, 0);
        turtleTextWriteUnicode("ӁЗИӢЙІЇЈКҚҜЛЉМНҢЊОӨПРСТЋУӮҮҰЎФХҲҺЦЧҶ", scrollbarX -> value * -5 + 260, scrollbarY -> value * 3.3 - 270, 10, 0);
        turtleTextWriteUnicode("ҸЏШЩЪЫЬЭЮЯабвгґғдђеёєәжӂзиӣйіїјкқҝлљ", scrollbarX -> value * -5 + 260, scrollbarY -> value * 3.3 - 285, 10, 0);
        turtleTextWriteUnicode("мнңњоөпрстћуӯүұўфхҳһцчҷҹџшщъыьэюя", scrollbarX -> value * -5 + 260, scrollbarY -> value * 3.3 - 300, 10, 0);
        turtleTextWriteUnicode("ΑΒΓΔΕΖΗΘΙΚΛΜΝΞΟΠΡΣΤΥΦΧΨΩ", scrollbarX -> value * -5 + 260, scrollbarY -> value * 3.3 - 315, 10, 0);
        turtleTextWriteUnicode("αβγδεζηθικλμνξοπρσςτυφχψω", scrollbarX -> value * -5 + 260, scrollbarY -> value * 3.3 - 330, 10, 0);
        turtleTextWriteUnicode("1234567890!@#$£€₺₽¥₩₹₣฿%^&*()`~-_=+[", scrollbarX -> value * -5 + 260, scrollbarY -> value * 3.3 - 345, 10, 0);
        turtleTextWriteUnicode("{]}\\|;:‘'’“\"”,<.>/?½¨", scrollbarX -> value * -5 + 260, scrollbarY -> value * 3.3 - 360, 10, 0);
        
        turtleTextWriteStringRotated("Rotated Text", scrollbarX -> value * -5 - 100, scrollbarY -> value * 3.3 + 75, 9, 50, -15);
        
        /* draw texture */
        if (oldImageDropdown != imageDropdown -> value) {
            if (cameraName) {
                osToolsCameraClose(cameraName);
            }
            oldImageDropdown = imageDropdown -> value;
            if (imageDropdown -> value == 0) {
                cameraName = NULL;
                if (cameraFrame) {
                    free(cameraFrame);
                    cameraFrame = NULL;
                }
                turtleTextureUnload(empvImage);
                empvImage = turtleTextureLoad("images/EMPV.png");
            } else {
                cameraName = cameras -> data[(imageDropdown -> value - 1) * 4].s;
                osToolsCameraOpen(cameraName);
                if (cameraFrame) {
                    free(cameraFrame);
                }
                cameraFrame = malloc(cameras -> data[(imageDropdown -> value - 1) * 4 + 1].i * cameras -> data[(imageDropdown -> value - 1) * 4 + 2].i * 3);
            }
        }
        if (cameraName) {
            osToolsCameraReceive(cameraName, cameraFrame);
            turtleTextureUnload(empvImage);
            empvImage = turtleTextureLoadArray(cameraFrame, cameras -> data[(imageDropdown -> value - 1) * 4 + 1].i, cameras -> data[(imageDropdown -> value - 1) * 4 + 2].i, GL_RGB);
            double textureCenterX = 550;
            double textureCenterY = -60.5;
            double textureWidth = 300.0 / ((16.0 / 9) * ((double) cameras -> data[(imageDropdown -> value - 1) * 4 + 2].i / cameras -> data[(imageDropdown -> value - 1) * 4 + 1].i));
            if (textureWidth > 300) {
                textureWidth = 300;
            }
            double textureHeight = (16.0 / 9) * ((double) cameras -> data[(imageDropdown -> value - 1) * 4 + 2].i / cameras -> data[(imageDropdown -> value - 1) * 4 + 1].i) * 169.0;
            if (textureHeight > 169) {
                textureHeight = 169;
            }
            turtleTexture(empvImage, scrollbarX -> value * -5 + textureCenterX - textureWidth / 2, scrollbarY -> value * 3.3 + textureCenterY - textureHeight / 2, scrollbarX -> value * -5 + textureCenterX + textureWidth / 2, scrollbarY -> value * 3.3 + textureCenterY + textureHeight / 2, 0, 255, 255, 255);
        } else {
            turtleTexture(empvImage, scrollbarX -> value * -5 + 400, scrollbarY -> value * 3.3 - 145, scrollbarX -> value * -5 + 700, scrollbarY -> value * 3.3 + 24, 0, 255, 255, 255);
        }

        // turtlePenColor(0, 0, 0);
        // turtle3DTriangle(-5, 0, 10, 5, 0, 10, 0, 5, 10);

        scroll = turtleMouseWheel();
        if (scroll != 0) {
            if (turtleKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
                scrollbarX -> value -= scroll * scrollFactor;
                if (scrollbarX -> value < 0) {
                    scrollbarX -> value = 0;
                }
                if (scrollbarX -> value > 100) {
                    scrollbarX -> value = 100;
                }
            } else {
                scrollbarY -> value -= scroll * scrollFactor;
                if (scrollbarY -> value < 0) {
                    scrollbarY -> value = 0;
                }
                if (scrollbarY -> value > 100) {
                    scrollbarY -> value = 100;
                }
            }
        }
        if (button -> value) {
            button -> value = 0;
            printf("button clicked\n");
        }
        if (circleButton -> value) {
            circleButton -> value = 0;
            printf("circle button clicked\n");
        }
        if (textButton -> value) {
            textButton -> value = 0;
            printf("text button clicked\n");
        }
        if (turtleMouseRight()) {
            if (keys[1] == 0) {
                keys[1] = 1;
                context -> enabled = TT_ELEMENT_ENABLED;
                context -> x = turtle.mouseX;
                context -> y = turtle.mouseY;
            }
        } else {
            keys[1] = 0;
        }
        turtleToolsUpdate(); // update turtleTools
        tt_setColor(TT_COLOR_TEXT);
        turtleTextWriteStringf(-310, -170, 5, 0, "%.2lf, %.2lf", turtle.mouseX, turtle.mouseY);
        parseRibbonOutput(); // user defined function to use ribbon
        parsePopupOutput(window); // user defined function to use popup
        turtleUpdate(); // update the screen
        end = clock();
        while ((double) (end - start) / CLOCKS_PER_SEC < (1.0 / tps)) {
            end = clock();
        }
        tick++;
    }
    if (cameraName) {
        osToolsCameraClose(cameraName);
    }
    turtleFree();
    glfwTerminate();
    return 0;
}
