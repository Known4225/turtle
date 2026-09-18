// #define TURTLE_IMPLEMENTATION
#include "turtle.h"
#include <time.h>

void parseRibbonOutput() {
    if (turtleToolsRibbon.output[0] == 0) {
        return;
    }
    turtleToolsRibbon.output[0] = 0;
    if (turtleToolsRibbon.output[1] == 0) { // File
        if (turtleToolsRibbon.output[2] == 1) { // New
            list_clear(osToolsFileDialog.selectedFilenames);
            printf("New\n");
        }
        if (turtleToolsRibbon.output[2] == 2) { // Save
            if (osToolsFileDialog.selectedFilenames -> length == 0) {
                if (os_tools_file_dialog_save(OSTOOLS_FILE_DIALOG_FILE, "Save.txt", NULL) != -1) {
                    printf("Saved to: %s\n", osToolsFileDialog.selectedFilenames -> data[0].s);
                }
            } else {
                printf("Saved to: %s\n", osToolsFileDialog.selectedFilenames -> data[0].s);
            }
        }
        if (turtleToolsRibbon.output[2] == 3) { // Save As...
            list_clear(osToolsFileDialog.selectedFilenames);
            if (os_tools_file_dialog_save(OSTOOLS_FILE_DIALOG_FILE, "Save.txt", NULL) != -1) {
                printf("Saved to: %s\n", osToolsFileDialog.selectedFilenames -> data[0].s);
            }
        }
        if (turtleToolsRibbon.output[2] == 4) { // Open
            list_clear(osToolsFileDialog.selectedFilenames);
            if (os_tools_file_dialog_open(OSTOOLS_FILE_DIALOG_MULTIPLE_SELECT, OSTOOLS_FILE_DIALOG_FILE, "", NULL) != -1) {
                printf("Loaded data from: ");
                list_print(osToolsFileDialog.selectedFilenames);
            }
        }
    }
    if (turtleToolsRibbon.output[1] == 1) { // Edit
        if (turtleToolsRibbon.output[2] == 1) { // Undo
            printf("Undo\n");
        }
        if (turtleToolsRibbon.output[2] == 2) { // Redo
            printf("Redo\n");
        }
        if (turtleToolsRibbon.output[2] == 3) { // Cut
            os_tools_clipboard_set_text("test123");
            printf("Cut \"test123\" to clipboard!\n");
        }
        if (turtleToolsRibbon.output[2] == 4) { // Copy
            os_tools_clipboard_set_text("test345");
            printf("Copied \"test345\" to clipboard!\n");
        }
        if (turtleToolsRibbon.output[2] == 5) { // Paste
            os_tools_clipboard_get_text();
            printf("Pasted \"%s\" from clipboard!\n", osToolsClipboard.text);
        }
    }
    if (turtleToolsRibbon.output[1] == 2) { // View
        if (turtleToolsRibbon.output[2] == 1) { // Change theme
            printf("Change theme\n");
            if (turtleToolsTheme == TT_THEME_DARK) {
                turtle_background_color(36, 30, 32);
                turtle_tools_set_theme(TT_THEME_COLT);
            } else if (turtleToolsTheme == TT_THEME_COLT) {
                turtle_background_color(212, 201, 190);
                turtle_tools_set_theme(TT_THEME_NAVY);
            } else if (turtleToolsTheme == TT_THEME_NAVY) {
                turtle_background_color(255, 255, 255);
                turtle_tools_set_theme(TT_THEME_LIGHT);
            } else if (turtleToolsTheme == TT_THEME_LIGHT) {
                turtle_background_color(30, 30, 30);
                turtle_tools_set_theme(TT_THEME_DARK);
            }
        }
        if (turtleToolsRibbon.output[2] == 2) { // GLFW
            printf("GLFW settings\n");
        }
    }
}

void parsePopupOutput(GLFWwindow *window) {
    if (turtleToolsPopup.output[0] == 0) {
        return;
    }
    turtleToolsPopup.output[0] = 0; // untoggle
    if (turtleToolsPopup.output[1] == 0) { // cancel
        turtle.close = 0;
        glfwSetWindowShouldClose(window, 0);
    }
    if (turtleToolsPopup.output[1] == 1) { // close
        turtle.popupClose = 1;
    }
}

int main(int argc, char *argv[]) {
    /* create window */
    GLFWwindow *window = turtle_create_window_icon(TURTLE_WINDOW_DEFAULT_WIDTH, TURTLE_WINDOW_DEFAULT_HEIGHT, "turtle demo", "images/thumbnail.png");
    if (window == NULL) {
        return -1; // failed to create window
    }

    /* initialise turtle */
    turtle_set_resize_mode(TURTLE_RESIZE_MODE_PAD); // change to TURTLE_RESIZE_MODE_STRETCH to have content stretch when resized
    turtle_init(window, -320, -180, 320, 180);

    /* initialise osTools */
    os_tools_init(argv[0], window); // must include argv[0] to get executableFilepath, must include GLFW window for copy paste and cursor functionality
    os_tools_file_dialog_add_global_extension("txt"); // add txt to extension restrictions
    os_tools_file_dialog_add_global_extension("csv"); // add csv to extension restrictions

    /* initialise turtle_text */
    char constructedFilepath[5120];
    strcpy(constructedFilepath, osToolsFileDialog.executableFilepath);
    strcat(constructedFilepath, "config/roberto.tgl");
    turtle_text_init(constructedFilepath);

    /* initialise turtleTools ribbon */
    turtle_tools_set_theme(TT_THEME_DARK); // dark theme preset
    strcpy(constructedFilepath, osToolsFileDialog.executableFilepath);
    strcat(constructedFilepath, "config/ribbonConfig.txt");
    turtle_tools_ribbon_init(constructedFilepath);

    // list_t *ribbonConfig = list_init();
    // list_append(ribbonConfig, (unitype) "File, 📄 New, 📄 Save, 📄 Save As..., 📄 Open", 's');
    // list_append(ribbonConfig, (unitype) "Edit, Undo, Redo, Cut, Copy, Paste", 's');
    // list_append(ribbonConfig, (unitype) "View, Change Theme, GLFW", 's');
    // turtle_tools_ribbon_init_list(ribbonConfig);

    /* initialise turtleTools popup */
    strcpy(constructedFilepath, osToolsFileDialog.executableFilepath);
    strcat(constructedFilepath, "config/popupConfig.txt");
    turtle_tools_popup_init(constructedFilepath);
    // list_t *popupConfig = list_init();
    // list_append(popupConfig, (unitype) "Are you sure you want to close?", 's');
    // list_append(popupConfig, (unitype) "Cancel", 's');
    // list_append(popupConfig, (unitype) "Close", 's');
    // turtle_tools_popup_init_list(popupConfig);
    strcpy(constructedFilepath, osToolsFileDialog.executableFilepath);
    strcat(constructedFilepath, "config/test.csv");
    list_t *rowLike = os_tools_load_csv_string(constructedFilepath, OSTOOLS_CSV_ROW);
    list_t *columnLike = os_tools_load_csv_string(constructedFilepath, OSTOOLS_CSV_COLUMN);
    if (rowLike != NULL) {
        list_print(rowLike);
    }
    if (columnLike != NULL) {
        list_print(columnLike);
    }

    /* textures */
    turtle_texture_t empvImage = turtle_texture_load("images/EMPV.png");
    // uint8_t array[16] = {
    //     100, 100, 100, 100,
    //     100, 100, 100, 100,
    //     100, 100, 100, 100,
    //     100, 100, 100, 100,
    // };
    // turtle_texture_t empvImage = turtle_texture_load_array(array, 4, 4, GL_GREEN);
    turtle_texture_print(empvImage);
    list_t *folders = os_tools_folder_list(".");
    list_t *files = os_tools_file_list(".");
    list_t *filesAndFolders = os_tools_file_and_folder_list(".");
    list_print(folders);
    list_print(files);
    list_print(filesAndFolders);
    list_free(folders);
    list_free(files);
    list_free(filesAndFolders);
    list_t *serialPorts = os_tools_serial_list();
    printf("Serial Ports: ");
    list_print(serialPorts);
    for (int32_t i = 0; i < serialPorts -> length; i++) {
        os_tools_serial_open(serialPorts -> data[i].s, OSTOOLS_BAUD_115200);
        os_tools_serial_send(serialPorts -> data[i].s, (uint8_t *) "Hello World\r\n", strlen("Hello World\r\n"));
        os_tools_serial_close(serialPorts -> data[i].s);
    }
    list_free(serialPorts);

    /* Server testing */
    // os_tools_server_socket_create("Server1", OSTOOLS_PROTOCOL_TCP, "6000");
    // os_tools_server_socket_listen("Server1", "Client1");
    // os_tools_socket_send("Client1", (uint8_t *) "Hello World\r\n", strlen("Hello World\r\n"));
    // uint8_t *buffer = calloc(128, 1);
    // os_tools_socket_receive("Client1", buffer, 128, 10000);
    // printf("Received: %s\n", buffer);
    // free(buffer);
    // os_tools_socket_delete("Client1");
    /* Client testing */
    // os_tools_client_socket_create("Client1", OSTOOLS_PROTOCOL_TCP, "127.0.0.1", "6000", 10000);
    // os_tools_socket_send("Client1", (uint8_t *) "Hello World\r\n", strlen("Hello World\r\n"));
    // uint8_t *buffer = calloc(128, 1);
    // os_tools_socket_receive("Client1", buffer, 128, 10000);
    // printf("Received: %s\n", buffer);
    // free(buffer);
    // os_tools_socket_delete("Client1");

    /* test list saving and loading */
    // list_t *listWrite = list_init();
    // list_append(listWrite, (unitype) 'A', 'c');
    // list_append(listWrite, (unitype) 1.0, 'd');
    // list_append(listWrite, (unitype) 500.1, 'd');
    // list_append(listWrite, (unitype) 2938274, 'i');
    // list_append(listWrite, (unitype) 6552, 'h');
    // list_t *listEmbed = list_init();
    // list_append(listEmbed, (unitype) "C:\\Information\\Programming\\C\\openGL\\turtle-development", 's');
    // list_append(listWrite, (unitype) listEmbed, 'r');
    // list_append(listWrite, (unitype) "SimpleString", 's');
    // list_append(listWrite, (unitype) "Hello World illegal ,,[],\\\\akdja", 's');
    // list_append(listWrite, (unitype) 'Z', 'c');
    // FILE *listWriteFile = fopen("listWriteFile.txt", "w");
    // list_write(listWriteFile, listWrite);
    // fclose(listWriteFile);
    // FILE *listReadFile = fopen("listWriteFile.txt", "r");
    // list_t *listRead = list_read(listReadFile);
    // fclose(listReadFile);
    // list_print(listWrite);
    // list_print(listRead);

    list_t *cameras = os_tools_camera_list();
    printf("Cameras: ");
    list_print(cameras);
    char *cameraName = NULL;
    uint8_t *cameraFrame = NULL;
    list_t *imageDropdownOptions = list_init();
    list_append(imageDropdownOptions, (unitype) "Image", 's');
    for (int32_t i = 0; i < cameras -> length; i += 4) {
        list_append(imageDropdownOptions, cameras -> data[i], 's');
    }
    turtle_tools_dropdown_t *imageDropdown = turtle_tools_dropdown_init("Source", imageDropdownOptions, NULL, TT_DROPDOWN_ALIGN_RIGHT, 700, 36, 8);
    int32_t oldImageDropdown = imageDropdown -> value;

    int32_t sliderVar = 0, dialVar = 0;
    turtle_tools_button_t *button = turtle_tools_button_init("Button", NULL, 150, 20, 10);
    button -> shape = TT_BUTTON_SHAPE_ROUNDED_RECTANGLE;
    turtle_tools_switch_init("Switch", NULL, 150, -20, 10);
    turtle_tools_dial_init("Exp", &dialVar, TT_DIAL_SCALE_EXP, -150, 20, 10, 0, 1000, NULL, 1);
    turtle_tools_dial_init("Linear", &dialVar, TT_DIAL_SCALE_LINEAR, -150, -20, 10, 0, 1000, NULL, 1);
    turtle_tools_dial_init("Log", &dialVar, TT_DIAL_SCALE_LOG, -150, -60, 10, 0, 1000, NULL, 1);
    turtle_tools_slider_init("Slider", NULL, TT_SLIDER_TYPE_HORIZONTAL, TT_SLIDER_ALIGN_LEFT, -100, 35, 10, 50, 0, 255, NULL, 1);
    turtle_tools_slider_init("Slider", NULL, TT_SLIDER_TYPE_HORIZONTAL, TT_SLIDER_ALIGN_CENTER, 0, 35, 10, 50, 0, 255, NULL, 1);
    turtle_tools_slider_init("Slider", NULL, TT_SLIDER_TYPE_HORIZONTAL, TT_SLIDER_ALIGN_RIGHT, 100, 35, 10, 50, 0, 255, NULL, 1);
    turtle_tools_slider_init("Log", &sliderVar, TT_SLIDER_TYPE_VERTICAL, TT_SLIDER_ALIGN_LEFT, -100, -35, 10, 50, 0, 255, NULL, 1) -> scale = TT_SLIDER_SCALE_LOG;
    turtle_tools_slider_init("Linear", &sliderVar, TT_SLIDER_TYPE_VERTICAL, TT_SLIDER_ALIGN_CENTER, 0, -35, 10, 50, 0, 255, NULL, 1) -> scale = TT_SLIDER_SCALE_LINEAR;
    turtle_tools_slider_init("Exp", &sliderVar, TT_SLIDER_TYPE_VERTICAL, TT_SLIDER_ALIGN_RIGHT, 100, -35, 10, 50, 0, 255, NULL, 1) -> scale = TT_SLIDER_SCALE_EXP;
    turtle_tools_scrollbar_t *scrollbarX = turtle_tools_scrollbar_init(NULL, TT_SCROLLBAR_TYPE_HORIZONTAL, 20, -170, 10, 550, 50);
    turtle_tools_scrollbar_t *scrollbarY = turtle_tools_scrollbar_init(NULL, TT_SCROLLBAR_TYPE_VERTICAL, 310, 0, 10, 320, 33);
    list_t *dropdownOptions = list_init();
    list_append(dropdownOptions, (unitype) "Indicator", 's');
    list_append(dropdownOptions, (unitype) "Register", 's');
    list_append(dropdownOptions, (unitype) "P15 Pin", 's');
    list_append(dropdownOptions, (unitype) "K50 Touch", 's');
    turtle_tools_dropdown_init("Dropdown", dropdownOptions, NULL, TT_DROPDOWN_ALIGN_CENTER, 0, 70, 10);
    turtle_tools_textbox_t *username = turtle_tools_textbox_init("Username", NULL, 128, -50, -110, 10, 100);
    turtle_tools_textbox_t *password = turtle_tools_textbox_init("Password", NULL, 128, -50, -135, 10, 100);
    list_t *contextOptions = list_init();
    list_append(contextOptions, (unitype) "Button", 's');
    list_append(contextOptions, (unitype) "Switch", 's');
    list_append(contextOptions, (unitype) "Dial", 's');
    list_append(contextOptions, (unitype) "Slider", 's');
    list_append(contextOptions, (unitype) "Textbox", 's');
    list_append(contextOptions, (unitype) "Dropdown", 's');
    list_append(contextOptions, (unitype) "Scrollbar", 's');
    list_append(contextOptions, (unitype) "Context", 's');
    turtle_tools_context_t *context = turtle_tools_context_init(contextOptions, NULL, 0, 0, 10);
    context -> enabled = TT_ELEMENT_HIDE;

    int32_t x = 103, y = 95, z = 215;
    list_t *sources = list_init();
    list_append(sources, (unitype) "None", 's');
    list_append(sources, (unitype) "SP932", 's');
    list_append(sources, (unitype) "SP932U", 's');
    list_append(sources, (unitype) "SP928", 's');
    list_append(sources, (unitype) "SP1203", 's');
    list_append(sources, (unitype) "SP-1550M", 's');
    turtle_tools_dial_init("Power", NULL, TT_DIAL_SCALE_LINEAR, -150, -210, 10, 0, 100, NULL, 1) -> style = TT_DIAL_STYLE_SPEEDOMETER;
    turtle_tools_dial_init("Speed", NULL, TT_DIAL_SCALE_LINEAR, -100, -210, 10, 0, 1000, NULL, 1) -> style = TT_DIAL_STYLE_SPEEDOMETER;
    turtle_tools_dial_init("Shutter", NULL, TT_DIAL_SCALE_EXP, -50, -210, 10, 0, 1000, NULL, 1) -> style = TT_DIAL_STYLE_SPEEDOMETER;
    turtle_tools_dropdown_init("Source", sources, NULL, TT_DROPDOWN_ALIGN_LEFT, -10, -211.2, 10);
    turtle_tools_slider_t *xSlider = turtle_tools_slider_init("X", &x, TT_SLIDER_TYPE_HORIZONTAL, TT_SLIDER_ALIGN_CENTER, -100, -240, 10, 100, -300, 300, "%.1lfmm", 0.1);
    turtle_tools_slider_t *ySlider = turtle_tools_slider_init("Y", &y, TT_SLIDER_TYPE_HORIZONTAL, TT_SLIDER_ALIGN_CENTER, -100, -260, 10, 100, -300, 300, "%.1lfmm", 0.1);
    turtle_tools_slider_t *zSlider = turtle_tools_slider_init("Z", &z, TT_SLIDER_TYPE_HORIZONTAL, TT_SLIDER_ALIGN_CENTER, -100, -280, 10, 100, -300, 300, "%.1lfmm", 0.1);
    xSlider -> style = TT_SLIDER_STYLE_SIDESWIPE;
    ySlider -> style = TT_SLIDER_STYLE_SIDESWIPE;
    zSlider -> style = TT_SLIDER_STYLE_SIDESWIPE;
    turtle_tools_switch_init("", NULL, 3, -240, 10);
    turtle_tools_switch_init("", NULL, 3, -260, 10);
    turtle_tools_switch_init("", NULL, 3, -280, 10);

    turtle_tools_switch_t *sideswipe = turtle_tools_switch_init("Side Swipe", NULL, 305, 15, 10);
    turtle_tools_switch_t *checkbox = turtle_tools_switch_init("Checkbox", NULL, 300, 0, 10);
    turtle_tools_switch_t *xbox = turtle_tools_switch_init("Xbox", NULL, 300, -15, 10);
    sideswipe -> style = TT_SWITCH_STYLE_SIDESWIPE;
    checkbox -> value = 1;
    checkbox -> style = TT_SWITCH_STYLE_CHECKBOX;
    xbox -> value = 1;
    xbox -> style = TT_SWITCH_STYLE_XBOX;
    turtle_tools_button_t *textButton = turtle_tools_button_init("Text Button", NULL, 330, -30, 10);
    turtle_tools_button_t *circleButton = turtle_tools_button_init("Circle Button", NULL, 338, -100, 10);
    textButton -> shape = TT_BUTTON_SHAPE_TEXT;
    circleButton -> shape = TT_BUTTON_SHAPE_CIRCLE;

    uint64_t tick = 0; // count number of ticks since application started
    turtle_tools_reader_init("tick", (unitype *) &tick, 'l', -315, 155, 10);
    turtle_tools_reader_init("elementLogicTypeOld", (unitype *) &turtleToolsGlobals.elementLogicTypeOld, 'i', -315, 135, 10);
    turtle_tools_reader_init("elementLogicIndexOld", (unitype *) &turtleToolsGlobals.elementLogicIndexOld, 'i', -315, 115, 10);
    turtle_tools_reader_t *listReader = turtle_tools_reader_init("Sources", (unitype *) &sources, 'r', -315, 95, 10);
    listReader -> height = 175;
    listReader -> width = 100;

    list_t *xPositions = list_init();
    list_t *yPositions = list_init();
    for (int32_t i = 0; i < turtleToolsElements.all -> length; i++) {
        list_append(xPositions, (unitype) ((turtle_tools_button_t *) turtleToolsElements.all -> data[i].p) -> x, 'd');
        list_append(yPositions, (unitype) ((turtle_tools_button_t *) turtleToolsElements.all -> data[i].p) -> y, 'd');
    }

    double scroll = 0.0;
    double scrollFactor = 15;
    char keys[8] = {0};

    uint32_t tps = 120; // ticks per second (locked to fps in this case)
    clock_t start, end;

    // turtle_background_color(13, 17, 23);

    while (turtle.popupClose == 0) {
        start = clock();
        turtle_get_mouse_coordinates();
        turtle_clear();
        /* update element positions (scrollbar) */
        for (int32_t i = 0; i < turtleToolsElements.all -> length; i++) {
            if (((turtle_tools_button_t *) turtleToolsElements.all -> data[i].p) -> element != TT_ELEMENT_SCROLLBAR && ((turtle_tools_button_t *) turtleToolsElements.all -> data[i].p) -> element != TT_ELEMENT_CONTEXT) {
                if ((((turtle_tools_button_t *) turtleToolsElements.all -> data[i].p) -> element == TT_ELEMENT_VARIABLE_READER || ((turtle_tools_button_t *) turtleToolsElements.all -> data[i].p) -> element == TT_ELEMENT_LIST_READER) && (((turtle_tools_reader_t *) turtleToolsElements.all -> data[i].p) -> status == TT_STATUS_CLICK || ((turtle_tools_reader_t *) turtleToolsElements.all -> data[i].p) -> status == TT_STATUS_CLICK_FIRST_TICK)) {
                    xPositions -> data[i].d = ((turtle_tools_button_t *) turtleToolsElements.all -> data[i].p) -> x + scrollbarX -> value * 5;
                    yPositions -> data[i].d = ((turtle_tools_button_t *) turtleToolsElements.all -> data[i].p) -> y - scrollbarY -> value * 3.3;
                    continue;
                }
                ((turtle_tools_button_t *) turtleToolsElements.all -> data[i].p) -> x = xPositions -> data[i].d - scrollbarX -> value * 5;
                ((turtle_tools_button_t *) turtleToolsElements.all -> data[i].p) -> y = yPositions -> data[i].d + scrollbarY -> value * 3.3;
            }
        }

        if (username -> mouseOver || password -> mouseOver) {
            os_tools_set_cursor(GLFW_IBEAM_CURSOR);
        } else {
            os_tools_set_cursor(GLFW_ARROW_CURSOR);
        }

        /* write all characters supported */
        turtle_tools_set_color(TT_COLOR_TEXT);
        turtle_text_write_unicode("AÀÁĂÄÃÅĀĄÆBCĆČĊÇDĎĐÐEÈÉĚÊËĒĖĘƏFGĞĠHĦ", scrollbarX -> value * -5 + 260, scrollbarY -> value * 3.3 - 180, 10, 0);
        turtle_text_write_unicode("IÌÍÎÏĪİĮJKĶLĹĽĻŁĿMNŃŇÑŅOÒÓÔÖÕŐØŒPQRŔ", scrollbarX -> value * -5 + 260, scrollbarY -> value * 3.3 - 195, 10, 0);
        turtle_text_write_unicode("ŘSŚŠŞȘẞTŤȚÞUÙÚÛÜŮŰŪŲVWXYÝZŹŽŻaàáâăäã", scrollbarX -> value * -5 + 260, scrollbarY -> value * 3.3 - 210, 10, 0);
        turtle_text_write_unicode("åāąæbcćčċçdďđðeèéěêëēėęəfgğġhħiìíîïī", scrollbarX -> value * -5 + 260, scrollbarY -> value * 3.3 - 225, 10, 0);
        turtle_text_write_unicode("ıįjkķlĺľļłŀmnńňñņoòóôöõőøœpqrŕřsśšşș", scrollbarX -> value * -5 + 260, scrollbarY -> value * 3.3 - 240, 10, 0);
        turtle_text_write_unicode("ßtťțþuùúûüůűūųvwxyýzźžżАБВГҐҒДЂЕЁЄӘЖ", scrollbarX -> value * -5 + 260, scrollbarY -> value * 3.3 - 255, 10, 0);
        turtle_text_write_unicode("ӁЗИӢЙІЇЈКҚҜЛЉМНҢЊОӨПРСТЋУӮҮҰЎФХҲҺЦЧҶ", scrollbarX -> value * -5 + 260, scrollbarY -> value * 3.3 - 270, 10, 0);
        turtle_text_write_unicode("ҸЏШЩЪЫЬЭЮЯабвгґғдђеёєәжӂзиӣйіїјкқҝлљ", scrollbarX -> value * -5 + 260, scrollbarY -> value * 3.3 - 285, 10, 0);
        turtle_text_write_unicode("мнңњоөпрстћуӯүұўфхҳһцчҷҹџшщъыьэюя", scrollbarX -> value * -5 + 260, scrollbarY -> value * 3.3 - 300, 10, 0);
        turtle_text_write_unicode("ΑΒΓΔΕΖΗΘΙΚΛΜΝΞΟΠΡΣΤΥΦΧΨΩ", scrollbarX -> value * -5 + 260, scrollbarY -> value * 3.3 - 315, 10, 0);
        turtle_text_write_unicode("αβγδεζηθικλμνξοπρσςτυφχψω", scrollbarX -> value * -5 + 260, scrollbarY -> value * 3.3 - 330, 10, 0);
        turtle_text_write_unicode("1234567890!@#$£€₺₽¥₩₹₣฿%^&*()`~-_=+[", scrollbarX -> value * -5 + 260, scrollbarY -> value * 3.3 - 345, 10, 0);
        turtle_text_write_unicode("{]}\\|;:‘'’“\"”,<.>/?½¨°︘📷📄📁😊♖🔄", scrollbarX -> value * -5 + 260, scrollbarY -> value * 3.3 - 360, 10, 0);

        turtle_text_write_string_rotated("Rotated Text", scrollbarX -> value * -5 - 100, scrollbarY -> value * 3.3 + 75, 9, 50, -15);

        /* draw texture */
        if (oldImageDropdown != imageDropdown -> value) {
            if (cameraName) {
                os_tools_camera_close(cameraName);
            }
            oldImageDropdown = imageDropdown -> value;
            if (imageDropdown -> value == 0) {
                cameraName = NULL;
                if (cameraFrame) {
                    free(cameraFrame);
                    cameraFrame = NULL;
                }
                turtle_texture_unload(empvImage);
                empvImage = turtle_texture_load("images/EMPV.png");
            } else {
                cameraName = cameras -> data[(imageDropdown -> value - 1) * 4].s;
                os_tools_camera_open(cameraName);
                if (cameraFrame) {
                    free(cameraFrame);
                }
                cameraFrame = malloc(cameras -> data[(imageDropdown -> value - 1) * 4 + 1].i * cameras -> data[(imageDropdown -> value - 1) * 4 + 2].i * 3);
            }
        }
        if (cameraName) {
            os_tools_camera_receive(cameraName, cameraFrame);
            turtle_texture_replace_array(empvImage, cameraFrame, cameras -> data[(imageDropdown -> value - 1) * 4 + 1].i, cameras -> data[(imageDropdown -> value - 1) * 4 + 2].i, GL_RGB);
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
            turtle_texture(empvImage, scrollbarX -> value * -5 + textureCenterX - textureWidth / 2, scrollbarY -> value * 3.3 + textureCenterY - textureHeight / 2, scrollbarX -> value * -5 + textureCenterX + textureWidth / 2, scrollbarY -> value * 3.3 + textureCenterY + textureHeight / 2, 0);
        } else {
            turtle_texture(empvImage, scrollbarX -> value * -5 + 400, scrollbarY -> value * 3.3 - 145, scrollbarX -> value * -5 + 700, scrollbarY -> value * 3.3 + 24, 0);
        }

        // turtle_pen_color(0, 0, 0);
        // turtle_3D_Triangle(-5, 0, 10, 5, 0, 10, 0, 5, 10);

        scroll = turtle_mouse_wheel();
        if (scroll != 0) {
            if (turtle_key_pressed(GLFW_KEY_LEFT_SHIFT)) {
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
        if (turtle_mouse_right()) {
            if (keys[1] == 0) {
                keys[1] = 1;
                if (turtleToolsGlobals.elementLogicType == TT_ELEMENT_CONTEXT || turtleToolsGlobals.elementLogicType == TT_ELEMENT_NONE) {
                    context -> enabled = TT_ELEMENT_ENABLED;
                    context -> x = turtle.mouseX;
                    context -> y = turtle.mouseY;
                }
            }
        } else {
            keys[1] = 0;
        }
        turtle_tools_update(); // update turtleTools
        turtle_tools_set_color(TT_COLOR_TEXT);
        turtle_text_write_stringf(-310, -170, 5, 0, "%.2lf, %.2lf", turtle.mouseX, turtle.mouseY);
        parseRibbonOutput(); // user defined function to use ribbon
        parsePopupOutput(window); // user defined function to use popup
        turtle_update(); // update the screen
        end = clock();
        while ((double) (end - start) / CLOCKS_PER_SEC < (1.0 / tps)) {
            end = clock();
        }
        tick++;
    }
    if (cameraName) {
        os_tools_camera_close(cameraName);
    }
    turtle_free();
    return 0;
}
