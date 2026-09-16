/*
████████╗██╗   ██╗██████╗ ████████╗██╗     ███████╗    ██████╗
╚══██╔══╝██║   ██║██╔══██╗╚══██╔══╝██║     ██╔════╝   ██╔════╝
   ██║   ██║   ██║██████╔╝   ██║   ██║     █████╗     ██║     
   ██║   ██║   ██║██╔══██╗   ██║   ██║     ██╔══╝     ██║     
   ██║   ╚██████╔╝██║  ██║   ██║   ███████╗███████╗██╗╚██████╗
   ╚═╝    ╚═════╝ ╚═╝  ╚═╝   ╚═╝   ╚══════╝╚══════╝╚═╝ ╚═════╝
https://patorjk.com/software/taag/#p=display&f=ANSI%20Shadow

openGL implementation of turtle and the turtletools module
features:
adjustable pen (size and colour)
resizable window support
keyboard and mouse presses
*/

#include "../include/turtle.h"

turtle_t turtle;

#ifdef TURTLE_ENABLE_TEXTURES
#define STB_IMAGE_IMPLEMENTATION
/* shader code */
const char *turtleVertexShaderSource = 
#ifdef OS_BROWSER
"#version 300 es\n"
"precision mediump float;\n"
#else
"#version 330 core\n"
#endif /* OS_BROWSER */
"layout(location = 0) in vec2 vPosition;\n"
"layout(location = 1) in vec4 vColor;\n"
"layout(location = 2) in vec3 textureCoordVert;\n"
"out vec4 shadeColor;\n"
"out vec2 textureCoordFrag;\n"
"out float textureIndex;\n"
"void main() {\n"
"    gl_Position = vec4(vPosition, 0.0, 1.0);\n"
"    shadeColor = vColor;\n"
"    textureCoordFrag = textureCoordVert.xy;\n"
"    textureIndex = textureCoordVert.z;\n"
"}\0";

const char *turtleFragmentShaderSource = 
#ifdef OS_BROWSER
"#version 300 es\n"
"uniform mediump sampler2DArray textureImages;\n"
"precision mediump float;\n"
#else
"#version 330 core\n"
"uniform sampler2DArray textureImages;\n"
#endif /* OS_BROWSER */
"in vec4 shadeColor;\n"
"in vec2 textureCoordFrag;\n"
"in float textureIndex;\n"
"out vec4 fragColor;\n"
"void main() {\n"
"    if (textureIndex > 0.5) {\n"
"        fragColor = texture(textureImages, vec3(textureCoordFrag, textureIndex)) * shadeColor;\n"
"    } else {\n"
"        fragColor = shadeColor;\n"
"    }\n"
"}\0";
#endif /* TURTLE_ENABLE_TEXTURES */

#ifdef OS_BROWSER
/* https://github.com/emscripten-core/emscripten/pull/20831#discussion_r1415646154 */
static EM_BOOL turtleBrowserWindowResize(int eventType, const EmscriptenUiEvent *uiEvent, void *userData) {
    double canvasWidth, canvasHeight;
    emscripten_get_element_css_size("#size-watcher", &canvasWidth, &canvasHeight);
    glfwSetWindowSize(glfwGetCurrentContext(), (int32_t) canvasWidth, (int32_t) canvasHeight);
    return true;
}
#endif /* OS_BROWSER */

/* special function that can be called prior to turtle_init - this function condenses the window creation code boilerplate */
GLFWwindow *turtle_create_window(int32_t windowWidth, int32_t windowHeight, char *windowName) {
    /* Initialise glfw */
    if (!glfwInit()) {
        return NULL;
    }
    glfwWindowHint(GLFW_SAMPLES, 4); // MSAA (Anti-Aliasing) with 4 samples (must be done before window is created (?))

    #ifndef OS_BROWSER
    /* Create a windowed mode window and its OpenGL context */
    const GLFWvidmode *monitorSize = glfwGetVideoMode(glfwGetPrimaryMonitor());
    int32_t totalHeight = monitorSize -> height;
    double optimizedScalingFactor = 1; // Set this number to 1 on windows and 0.8 on Ubuntu for maximum compatibility (fixes issue with incorrect stretching)
    turtle.windowSpecial = 0;
    #ifdef OS_WINDOWS
    optimizedScalingFactor = 1;
    #endif
    #ifdef OS_LINUX
    optimizedScalingFactor = 0.9;
    #endif
    if (windowWidth == TURTLE_WINDOW_DEFAULT_WIDTH) {
        windowWidth = totalHeight * 16.0 / 9.0 * optimizedScalingFactor;
        turtle.windowSpecial |= 1;
    }
    if (windowWidth == TURTLE_WINDOW_MONITOR_WIDTH) {
        windowWidth = totalHeight * 16.0 / 9.0 * optimizedScalingFactor;
        turtle.windowSpecial |= 4;
    }
    if (windowHeight == TURTLE_WINDOW_DEFAULT_HEIGHT) {
        windowHeight = totalHeight * optimizedScalingFactor;
        turtle.windowSpecial |= 2;
    }
    if (windowHeight == TURTLE_WINDOW_MONITOR_HEIGHT) {
        windowHeight = totalHeight * optimizedScalingFactor;
        turtle.windowSpecial |= 8;
    }
    #else
    if (windowWidth == TURTLE_WINDOW_DEFAULT_WIDTH) {
        windowWidth = 3840;
    }
    if (windowWidth == TURTLE_WINDOW_MONITOR_WIDTH) {
        windowWidth = 3840;
    }
    if (windowHeight == TURTLE_WINDOW_DEFAULT_HEIGHT) {
        windowHeight = 2160;
    }
    if (windowHeight == TURTLE_WINDOW_MONITOR_HEIGHT) {
        windowHeight = 2160;
    }
    #endif /* OS_BROWSER */
    GLFWwindow *window = glfwCreateWindow(windowWidth, windowHeight, windowName, NULL, NULL);
    if (!window) {
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);
    #ifdef OS_BROWSER
    emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, glfwGetCurrentContext(), false, turtleBrowserWindowResize);
    #else
    glfwSetWindowSizeLimits(window, totalHeight * 16 / 9 * 0.4, totalHeight * 0.4, totalHeight * 16 / 9 * optimizedScalingFactor, totalHeight * optimizedScalingFactor);
    #endif /* OS_BROWSER */
    return window;
}

#ifdef TURTLE_ENABLE_TEXTURES
/* special function that can be called prior to turtle_init - this function condenses the window creation code with icon boilerplate */
GLFWwindow *turtle_create_window_icon(int32_t windowWidth, int32_t windowHeight, char *windowName, char *filename) {
    GLFWwindow *window = turtle_create_window(windowWidth, windowHeight, windowName);
    /* initialise logo */
    GLFWimage icon;
    int32_t iconChannels;
    uint8_t *iconPixels = stbi_load(filename, &icon.width, &icon.height, &iconChannels, 4); // 4 color channels for RGBA
    if (iconPixels != NULL) {
        icon.pixels = iconPixels;
        glfwSetWindowIcon(window, 1, &icon);
        glfwPollEvents(); // update taskbar icon correctly on windows - https://github.com/glfw/glfw/issues/2753
        free(iconPixels);
    } else {
        printf("Could not load thumbnail %s\n", filename);
    }
    return window;
}
#endif

/* initializes the turtletools module */
void turtle_init(GLFWwindow *window, double leftX, double bottomY, double rightX, double topY) {
    #ifndef TURTLE_ENABLE_TEXTURES
    /* fixed pipeline */
    gladLoadGL();
    #endif /* TURTLE_ENABLE_TEXTURES */
    #ifdef TURTLE_ENABLE_TEXTURES
    #ifndef OS_BROWSER
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("could not initialise glad\n");
    }
    if (glGenVertexArrays == NULL) {
        printf("couldn't load openGL\n");
    }
    #endif /* OS_BROWSER */

    /* set up shaders */
    uint32_t VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    uint32_t VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * BUFFER_OBJECT_SIZE, (void *) 0); // position attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * BUFFER_OBJECT_SIZE, (void *) (2 * sizeof(float))); // color attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * BUFFER_OBJECT_SIZE, (void *) (6 * sizeof(float))); // texture coordinate attribute

    int32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &turtleVertexShaderSource, NULL);
    glCompileShader(vertexShader);
    char errorMessage[512];
    int32_t success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, errorMessage);
        printf("Error compiling vertex shader\n");
        printf("%s\n", errorMessage);
        return;
    }
    int32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &turtleFragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, errorMessage);
        printf("Error compiling fragment shader\n");
        printf("%s\n", errorMessage);
        return;
    }
    int32_t shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shaderProgram, 512, NULL, errorMessage);
        printf("Error linking shaders\n");
        printf("%s\n", errorMessage);
        return;
    }
    
    glUseProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(shaderProgram);
    turtle.bufferList = floatList_init();
    turtle.textureList = list_init();
    list_append(turtle.textureList, (unitype) "null", 's'); // cannot have texture code of 0 because of shader using 0 as the non-texture code
    list_append(turtle.textureList, (unitype) 0, 'i');
    list_append(turtle.textureList, (unitype) 0, 'i');
    list_append(turtle.textureList, (unitype) 0, 'i');
    /* setup texture parameters */
    glGenTextures(1, &turtle.textureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, turtle.textureID);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    if (turtle.textureWidth == 0) {
        turtle.textureWidth = 1024; // default
    }
    if (turtle.textureHeight == 0) {
        turtle.textureHeight = 1024; // default
    }
    if (turtle.maxTextures == 0) {
        turtle.maxTextures = 32; // default
    }
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, turtle.textureWidth, turtle.textureHeight, turtle.maxTextures, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL); // textures at 1024x1024
    #endif /* TURTLE_ENABLE_TEXTURES */
    glfwMakeContextCurrent(window); // various glfw things
    glEnable(GL_ALPHA);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(1.0, 1.0, 1.0, 0.0); // white background by default
    turtle.window = window;
    turtle.close = 0;
    turtle.popupClose = 0;
    turtle.keyPressed = list_init();
    turtle.lastscreenbounds[0] = 0;
    turtle.lastscreenbounds[1] = 0;
    turtle.penPos = list_init();
    turtle.penHash = 0;
    turtle.lastLength = 0;
    turtle.x = 0;
    turtle.y = 0;
    turtle.pensize = 1;
    turtle.penshape = TURTLE_PEN_SHAPE_CIRCLE;
    turtle.circleprez = 9; // default circleprez value
    turtle.pen = 0;
    turtle.red = 0;
    turtle.green = 0;
    turtle.blue = 0;
    turtle.alpha = 0;
    for (uint8_t i = 0; i < 4; i++) {
        turtle.currentColor[i] = -1.0;
    }
    /* 3D variables */
    turtle.cameraX = 0;
    turtle.cameraY = 0;
    turtle.cameraZ = 0;
    turtle.cameraFOV = 90;
    turtle.cameraDirectionLeftRight = 0;
    turtle.cameraDirectionUpDown = 0;

    turtle_set_world_coordinates(leftX, bottomY, rightX, topY);
    turtle.keyCallback = NULL;
    turtle.unicodeCallback = NULL;
    glfwSetCharCallback(window, turtle_unicode_sense);
    glfwSetKeyCallback(window, turtle_key_sense); // initiate mouse and keyboard detection
    glfwSetMouseButtonCallback(window, turtle_mouse_sense);
    glfwSetScrollCallback(window, turtle_scroll_sense);

    /* adjust window position and size */
    #ifdef OS_LINUX
    glfwSetWindowPos(window, 0, 36);
    #endif
    #ifdef OS_WINDOWS
    glfwSetWindowPos(window, 0, 31);
    #endif
    double width = turtle.screenbounds[1] * turtle.aspect;
    double height = turtle.screenbounds[1];
    if (turtle.windowSpecial & 1) {
        width *= 0.85;
    }
    if (turtle.windowSpecial & 2) {
        height *= 0.85;
    }
    if (turtle.windowSpecial & 4) {
        width *= 0.92;
    }
    if (turtle.windowSpecial & 8) {
        height *= 0.92;
    }
    glfwSetWindowSize(window, width, height); // doing it this way ensures the window spawns in the top left of the monitor and fixes resizing limits
    #ifdef OS_BROWSER
    turtleBrowserWindowResize(0, NULL, NULL);
    #endif
}

/* run this to set the bounds of the window in coordinates */
void turtle_set_world_coordinates(double leftX, double bottomY, double rightX, double topY) {
    glfwGetWindowSize(turtle.window, &turtle.screenbounds[0], &turtle.screenbounds[1]);
    turtle.centerAndScale[0] = (rightX + leftX) / 2;
    turtle.centerAndScale[1] = (topY + bottomY) / 2;
    turtle.centerAndScale[2] = (rightX - leftX) / 2 * turtle.screenbounds[0];
    turtle.centerAndScale[3] = (topY - bottomY) / 2 * turtle.screenbounds[1];
    turtle.initscreenbounds[0] = turtle.screenbounds[0];
    turtle.initscreenbounds[1] = turtle.screenbounds[1];
    turtle.initbounds[0] = leftX;
    turtle.initbounds[1] = bottomY;
    turtle.initbounds[2] = rightX;
    turtle.initbounds[3] = topY;
    turtle.aspect = (double) turtle.initscreenbounds[0] / turtle.initscreenbounds[1];
    memcpy(turtle.bounds, turtle.initbounds, 4 * sizeof(double));
}

/* detect character */
void turtle_unicode_sense(GLFWwindow *window, uint32_t codepoint) {
    if (turtle.unicodeCallback != NULL) {
        turtle.unicodeCallback(codepoint);
    }
}

/* detect key presses */
void turtle_key_sense(GLFWwindow *window, int32_t key, int32_t scancode, int32_t action, int32_t mods) {
    if (turtle.keyCallback != NULL) {
        turtle.keyCallback(key, scancode, action);
    }
    if (action == GLFW_PRESS) {
        list_append(turtle.keyPressed, (unitype) key, 'i');
    }
    if (action == GLFW_RELEASE) {
        list_remove(turtle.keyPressed, (unitype) key, 'i');
    }
}

/* detect mouse clicks */
void turtle_mouse_sense(GLFWwindow *window, int32_t button, int32_t action, int32_t mods) {
    if (action == GLFW_PRESS) {
        switch(button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            list_append(turtle.keyPressed, (unitype) "m1", 's');
            if ((turtle.mouseX > turtle.initbounds[0] && turtle.mouseX < turtle.initbounds[2] && turtle.mouseY > turtle.initbounds[1] && turtle.mouseY < turtle.initbounds[3]) || turtle.resizeMode == TURTLE_RESIZE_MODE_PAD_NO_BARS) {
                turtle.mousePressed[0] = 1;
            }
        break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            list_append(turtle.keyPressed, (unitype) "m2", 's');
            if ((turtle.mouseX > turtle.initbounds[0] && turtle.mouseX < turtle.initbounds[2] && turtle.mouseY > turtle.initbounds[1] && turtle.mouseY < turtle.initbounds[3]) || turtle.resizeMode == TURTLE_RESIZE_MODE_PAD_NO_BARS) {
                turtle.mousePressed[1] = 1;
            }
        break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            list_append(turtle.keyPressed, (unitype) "m3", 's');
            if ((turtle.mouseX > turtle.initbounds[0] && turtle.mouseX < turtle.initbounds[2] && turtle.mouseY > turtle.initbounds[1] && turtle.mouseY < turtle.initbounds[3]) || turtle.resizeMode == TURTLE_RESIZE_MODE_PAD_NO_BARS) {
                turtle.mousePressed[2] = 1;
            }
        break;
        default:
        break;
        }
    }
    if (action == GLFW_RELEASE) {
        switch(button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            list_remove(turtle.keyPressed, (unitype) "m1", 's');
            turtle.mousePressed[0] = 0;
        break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            list_remove(turtle.keyPressed, (unitype) "m2", 's');
            turtle.mousePressed[1] = 0;
        break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            list_remove(turtle.keyPressed, (unitype) "m3", 's');
            turtle.mousePressed[2] = 0;
        break;
        default:
        break;
        }
    }
}

/* detect scroll wheel */
void turtle_scroll_sense(GLFWwindow *window, double xoffset, double yoffset) {
    turtle.scrollY = yoffset;
}

/* the behavior with the mouse wheel is different since it can't be "on" or "off" */
double turtle_mouse_wheel() {
    double temp = turtle.scrollY;
    turtle.scrollY = 0;
    return temp;
}

/* top level boolean output call to check if the key with code [key] is currently being held down. Uses the GLFW_KEY_X macros */
int8_t turtle_key_pressed(int32_t key) {
    return list_count(turtle.keyPressed, (unitype) key, 'c');
}

/* top level boolean output call to check if the left click button is currently being held down */
int8_t turtle_mouse_down() {
    return turtle.mousePressed[0];
}

/* alternate duplicate of turtle_mouse_down() */
int8_t turtle_mouse_left() {
    return turtle.mousePressed[0];
}

/* top level boolean output call to check if the right click button is currently being held down */
int8_t turtle_mouse_right() {
    return turtle.mousePressed[1];
}

/* top level boolean output call to check if the middle mouse button is currently being held down */
int8_t turtle_mouse_middle() {
    return turtle.mousePressed[2];
}

/* alternate duplicate of turtle_mouse_middle() */
int8_t turtle_mouse_mid() {
    return turtle.mousePressed[2];
}

/* puts the mouse coordinates in turtle.mouseX and turtle.mouseY */
void turtle_get_mouse_coordinates() {
    glfwGetCursorPos(turtle.window, &turtle.mouseAbsX, &turtle.mouseAbsY); // get mouse positions (absolute)
    if (turtle.resizeMode == TURTLE_RESIZE_MODE_STRETCH) {
        turtle.mouseX = (turtle.mouseAbsX - turtle.screenbounds[0] / 2) / turtle.screenbounds[0] * (turtle.initbounds[2] - turtle.initbounds[0]) + (turtle.bounds[0] + turtle.bounds[2]) / 2;
        turtle.mouseY = (turtle.mouseAbsY - turtle.screenbounds[1] / 2) / turtle.screenbounds[1] * (turtle.initbounds[1] - turtle.initbounds[3]) + (turtle.bounds[1] + turtle.bounds[3]) / 2;
    } else if (turtle.resizeMode == TURTLE_RESIZE_MODE_PAD || turtle.resizeMode == TURTLE_RESIZE_MODE_PAD_NO_BARS) {
        double originalAspect = (double) turtle.initscreenbounds[0] / turtle.initscreenbounds[1];
        double currentAspect = (double) turtle.screenbounds[0] / turtle.screenbounds[1];
        if (currentAspect > originalAspect) {
            turtle.mouseX = ((turtle.mouseAbsX - turtle.screenbounds[0] / 2) / turtle.screenbounds[0] * (turtle.initbounds[2] - turtle.initbounds[0]) + (turtle.bounds[0] + turtle.bounds[2]) / 2) * currentAspect / originalAspect;
            turtle.mouseY = (turtle.mouseAbsY - turtle.screenbounds[1] / 2) / turtle.screenbounds[1] * (turtle.initbounds[1] - turtle.initbounds[3]) + (turtle.bounds[1] + turtle.bounds[3]) / 2;
        } else {
            turtle.mouseX = (turtle.mouseAbsX - turtle.screenbounds[0] / 2) / turtle.screenbounds[0] * (turtle.initbounds[2] - turtle.initbounds[0]) + (turtle.bounds[0] + turtle.bounds[2]) / 2;
            turtle.mouseY = ((turtle.mouseAbsY - turtle.screenbounds[1] / 2) / turtle.screenbounds[1] * (turtle.initbounds[1] - turtle.initbounds[3]) + (turtle.bounds[1] + turtle.bounds[3]) / 2) * originalAspect / currentAspect;
        }
    }
}

/* set the background color */
void turtle_background_color(uint8_t r, uint8_t g, uint8_t b) {
    glClearColor(r / 255.0, g / 255.0, b / 255.0, 1.0);
}

/* set the pen color */
void turtle_pen_color(uint8_t r, uint8_t g, uint8_t b) {
    turtle.red = r;
    turtle.green = g;
    turtle.blue = b;
    turtle.alpha = 255;
}

/* set the pen color (with transparency) */
void turtle_pen_color_alpha(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    turtle.red = r;
    turtle.green = g;
    turtle.blue = b;
    turtle.alpha = 255 - a;
}

/* set the pen size */
void turtle_pen_size(double size) {
    turtle.pensize = size * 0.5; // ensures pensize matches pixel size (a pen size of 240 will be 240 coordinates long)
}

/* clears all the pen drawings */
void turtle_clear() {
    // list_free(turtle.penPos);
    // turtle.penPos = list_init();
    turtle.penPos -> length = 0; // could be dangerous
}

/* pen down */
void turtle_pen_down() {
    if (turtle.pen == 0) {
        turtle.pen = 1;
        int8_t changed = 0;
        int32_t len = turtle.penPos -> length;
        if (len > 0) {
            unitype *ren = turtle.penPos -> data;
            if (ren[len - 9].d != turtle.x) {changed = 1;}
            if (ren[len - 8].d != turtle.y) {changed = 1;}
            if (ren[len - 7].d != turtle.pensize) {changed = 1;}
            if (ren[len - 6].b != turtle.red) {changed = 1;}
            if (ren[len - 5].b != turtle.green) {changed = 1;}
            if (ren[len - 4].b != turtle.blue) {changed = 1;}
            if (ren[len - 3].b != turtle.alpha) {changed = 1;}
            if (ren[len - 2].hu != turtle.penshape) {changed = 1;}
            if (ren[len - 1].d != turtle.circleprez) {changed = 1;}
        } else {
            changed = 1;
        }
        if (changed == 1) {
            list_append(turtle.penPos, (unitype) turtle.x, 'd');
            list_append(turtle.penPos, (unitype) turtle.y, 'd');
            list_append(turtle.penPos, (unitype) turtle.pensize, 'd');
            list_append(turtle.penPos, (unitype) turtle.red, 'b');
            list_append(turtle.penPos, (unitype) turtle.green, 'b');
            list_append(turtle.penPos, (unitype) turtle.blue, 'b');
            list_append(turtle.penPos, (unitype) turtle.alpha, 'b');
            list_append(turtle.penPos, (unitype) turtle.penshape, 'h');
            list_append(turtle.penPos, (unitype) turtle.circleprez, 'd');
        }
    }
}

/* lift the pen */
void turtle_pen_up() {
    if (turtle.pen == 1) {
        turtle.pen = 0;
        if (turtle.penPos -> length > 0 && turtle.penPos -> type[turtle.penPos -> length - 1] != 'c') {
            list_append(turtle.penPos, (unitype) 0, 'c');
            list_append(turtle.penPos, (unitype) 0, 'c');
            list_append(turtle.penPos, (unitype) 0, 'c');
            list_append(turtle.penPos, (unitype) 0, 'c');
            list_append(turtle.penPos, (unitype) 0, 'c');
            list_append(turtle.penPos, (unitype) 0, 'c');
            list_append(turtle.penPos, (unitype) 0, 'c');
            list_append(turtle.penPos, (unitype) 0, 'c');
            list_append(turtle.penPos, (unitype) 0, 'c');
        }
    }
}

/* set the pen shape ("circle", "square", "triangle", "none", or "connected") */
void turtle_pen_shape(turtle_pen_shape_t shape) {
    turtle.penshape = shape;
}

/* set the circle precision (default 9) */
void turtle_pen_prez(double prez) {
    turtle.circleprez = prez;
}

/* moves the turtle to a coordinate */
void turtle_goto(double x, double y) {
    if (fabs(turtle.x - x) > 0.01 || fabs(turtle.y - y) > 0.01) {
        turtle.x = x;
        turtle.y = y;
        if (turtle.pen == 1) {
            int8_t changed = 0;
            int32_t len = turtle.penPos -> length;
            if (len > 0) {
                unitype *ren = turtle.penPos -> data;
                if (ren[len - 9].d != turtle.x) {changed = 1;}
                if (ren[len - 8].d != turtle.y) {changed = 1;}
                if (ren[len - 7].d != turtle.pensize) {changed = 1;}
                if (ren[len - 6].d != turtle.red) {changed = 1;}
                if (ren[len - 5].d != turtle.green) {changed = 1;}
                if (ren[len - 4].d != turtle.blue) {changed = 1;}
                if (ren[len - 3].d != turtle.alpha) {changed = 1;}
                if (ren[len - 2].hu != turtle.penshape) {changed = 1;}
                if (ren[len - 1].d != turtle.circleprez) {changed = 1;}
            } else {
                changed = 1;
            }
            if (changed == 1) {
                list_append(turtle.penPos, (unitype) x, 'd');
                list_append(turtle.penPos, (unitype) y, 'd');
                list_append(turtle.penPos, (unitype) turtle.pensize, 'd');
                list_append(turtle.penPos, (unitype) turtle.red, 'b');
                list_append(turtle.penPos, (unitype) turtle.green, 'b');
                list_append(turtle.penPos, (unitype) turtle.blue, 'b');
                list_append(turtle.penPos, (unitype) turtle.alpha, 'b');
                list_append(turtle.penPos, (unitype) turtle.penshape, 'h');
                list_append(turtle.penPos, (unitype) turtle.circleprez, 'd');
            }
        }
    }
}

#ifndef TURTLE_ENABLE_TEXTURES
/* draws a circle at the specified x and y (coordinates) */
void turtle_circle_render_internal(double x, double y, double rad, double r, double g, double b, double a, double xcenter, double ycenter, double xfact, double yfact, double prez) {
    int8_t colorChange = 0;
    if (r != turtle.currentColor[0]) {colorChange = 1;}
    if (g != turtle.currentColor[1]) {colorChange = 1;}
    if (b != turtle.currentColor[2]) {colorChange = 1;}
    if (a != turtle.currentColor[3]) {colorChange = 1;}
    if (colorChange == 1) {
        glColor4d(r, g, b, a);
        turtle.currentColor[0] = r;
        turtle.currentColor[1] = g;
        turtle.currentColor[2] = b;
        turtle.currentColor[3] = a;
    }
    glBegin(GL_TRIANGLE_FAN);
    for (double i = 0; i < prez; i++) {
        glVertex2d((x + rad * sin(2 * i * M_PI / prez)) * xfact + xcenter, (y + rad * cos(2 * i * M_PI / prez)) * yfact + ycenter);
    }
    glEnd();
}

/* draws a rectangle */
void turtle_rectangle_render_internal(double x1, double y1, double x2, double y2, double r, double g, double b, double a, double xcenter, double ycenter, double xfact, double yfact) {
    int8_t colorChange = 0;
    if (r != turtle.currentColor[0]) {colorChange = 1;}
    if (g != turtle.currentColor[1]) {colorChange = 1;}
    if (b != turtle.currentColor[2]) {colorChange = 1;}
    if (a != turtle.currentColor[3]) {colorChange = 1;}
    if (colorChange == 1) {
        glColor4d(r, g, b, a);
        turtle.currentColor[0] = r;
        turtle.currentColor[1] = g;
        turtle.currentColor[2] = b;
        turtle.currentColor[3] = a;
    }
    glBegin(GL_TRIANGLE_FAN);
    glVertex2d(x1 * xfact + xcenter, y1 * yfact + ycenter);
    glVertex2d(x2 * xfact + xcenter, y1 * yfact + ycenter);
    glVertex2d(x2 * xfact + xcenter, y2 * yfact + ycenter);
    glVertex2d(x1 * xfact + xcenter, y2 * yfact + ycenter);
    glEnd();
}

/* draws a triangle */
void turtle_triangle_render_internal(double x1, double y1, double x2, double y2, double x3, double y3, double r, double g, double b, double a, double xcenter, double ycenter, double xfact, double yfact) {
    int8_t colorChange = 0;
    if (r != turtle.currentColor[0]) {colorChange = 1;}
    if (g != turtle.currentColor[1]) {colorChange = 1;}
    if (b != turtle.currentColor[2]) {colorChange = 1;}
    if (a != turtle.currentColor[3]) {colorChange = 1;}
    if (colorChange == 1) {
        glColor4d(r, g, b, a);
        turtle.currentColor[0] = r;
        turtle.currentColor[1] = g;
        turtle.currentColor[2] = b;
        turtle.currentColor[3] = a;
    }
    glBegin(GL_TRIANGLES);
    glVertex2d(x1 * xfact + xcenter, y1 * yfact + ycenter);
    glVertex2d(x2 * xfact + xcenter, y2 * yfact + ycenter);
    glVertex2d(x3 * xfact + xcenter, y3 * yfact + ycenter);
    glEnd();
}

/* draws a quadrilateral */
void turtle_quad_render_internal(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double r, double g, double b, double a, double xcenter, double ycenter, double xfact, double yfact) {
    int8_t colorChange = 0;
    if (r != turtle.currentColor[0]) {colorChange = 1;}
    if (g != turtle.currentColor[1]) {colorChange = 1;}
    if (b != turtle.currentColor[2]) {colorChange = 1;}
    if (a != turtle.currentColor[3]) {colorChange = 1;}
    if (colorChange == 1) {
        glColor4d(r, g, b, a);
        turtle.currentColor[0] = r;
        turtle.currentColor[1] = g;
        turtle.currentColor[2] = b;
        turtle.currentColor[3] = a;
    }
    glBegin(GL_TRIANGLE_FAN);
    glVertex2d(x1 * xfact + xcenter, y1 * yfact + ycenter);
    glVertex2d(x2 * xfact + xcenter, y2 * yfact + ycenter);
    glVertex2d(x3 * xfact + xcenter, y3 * yfact + ycenter);
    glVertex2d(x4 * xfact + xcenter, y4 * yfact + ycenter);
    glEnd();
}
#endif /* TURTLE_ENABLE_TEXTURES */

#ifdef TURTLE_ENABLE_TEXTURES
/* function to add a vertex to the turtle.bufferList */
void turtle_add_vertex(double x, double y, double r, double g, double b, double a, double tx, double ty, double useTexture) {
    floatList_append(turtle.bufferList, x);
    floatList_append(turtle.bufferList, y);
    floatList_append(turtle.bufferList, r);
    floatList_append(turtle.bufferList, g);
    floatList_append(turtle.bufferList, b);
    floatList_append(turtle.bufferList, a);
    floatList_append(turtle.bufferList, tx);
    floatList_append(turtle.bufferList, ty);
    floatList_append(turtle.bufferList, useTexture);
}

/* draws a circle at the specified x and y (coordinates) */
void turtle_circle_render_internal(double x, double y, double rad, double r, double g, double b, double a, double xcenter, double ycenter, double xfact, double yfact, double prez) {
    int32_t p = (int32_t) prez;
    if (p > 0) {
        // p--;
        float originX = x * xfact + xcenter;
        float originY = (y + rad) * yfact + ycenter;
        turtle_add_vertex(originX, originY, r, g, b, a, 0, 0, 0);
        turtle_add_vertex((x + rad * sin(2 * 1 * M_PI / prez)) * xfact + xcenter, (y + rad * cos(2 * 1 * M_PI / prez)) * yfact + ycenter, r, g, b, a, 0, 0, 0);
        int32_t i = 0;
        for (; i < p; i++) {
            turtle_add_vertex((x + rad * sin(2 * i * M_PI / prez)) * xfact + xcenter, (y + rad * cos(2 * i * M_PI / prez)) * yfact + ycenter, r, g, b, a, 0, 0, 0);
            turtle_add_vertex(originX, originY, r, g, b, a, 0, 0, 0);
            turtle_add_vertex(turtle.bufferList -> data[turtle.bufferList -> length - BUFFER_OBJECT_SIZE * 2], turtle.bufferList -> data[turtle.bufferList -> length - BUFFER_OBJECT_SIZE * 2 + 1], r, g, b, a, 0, 0, 0);
        }
        turtle_add_vertex((x + rad * sin(2 * i * M_PI / prez)) * xfact + xcenter, (y + rad * cos(2 * i * M_PI / prez)) * yfact + ycenter, r, g, b, a, 0, 0, 0);
    }
}

/* draws a rectangle */
void turtle_rectangle_render_internal(double x1, double y1, double x2, double y2, double r, double g, double b, double a, double xcenter, double ycenter, double xfact, double yfact) {
    turtle_add_vertex(x1 * xfact + xcenter, y1 * yfact + ycenter, r, g, b, a, 0, 0, 0);
    turtle_add_vertex(x2 * xfact + xcenter, y1 * yfact + ycenter, r, g, b, a, 0, 0, 0);
    turtle_add_vertex(x2 * xfact + xcenter, y2 * yfact + ycenter, r, g, b, a, 0, 0, 0);
    turtle_add_vertex(x1 * xfact + xcenter, y1 * yfact + ycenter, r, g, b, a, 0, 0, 0);
    turtle_add_vertex(x2 * xfact + xcenter, y2 * yfact + ycenter, r, g, b, a, 0, 0, 0);
    turtle_add_vertex(x1 * xfact + xcenter, y2 * yfact + ycenter, r, g, b, a, 0, 0, 0);
}

/* draws a triangle */
void turtle_triangle_render_internal(double x1, double y1, double x2, double y2, double x3, double y3, double r, double g, double b, double a, double xcenter, double ycenter, double xfact, double yfact) {
    turtle_add_vertex(x1 * xfact + xcenter, y1 * yfact + ycenter, r, g, b, a, 0, 0, 0);
    turtle_add_vertex(x2 * xfact + xcenter, y2 * yfact + ycenter, r, g, b, a, 0, 0, 0);
    turtle_add_vertex(x3 * xfact + xcenter, y3 * yfact + ycenter, r, g, b, a, 0, 0, 0);
}

/* draws a quadrilateral */
void turtle_quad_render_internal(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double r, double g, double b, double a, double xcenter, double ycenter, double xfact, double yfact) {
    turtle_add_vertex(x1 * xfact + xcenter, y1 * yfact + ycenter, r, g, b, a, 0, 0, 0);
    turtle_add_vertex(x2 * xfact + xcenter, y2 * yfact + ycenter, r, g, b, a, 0, 0, 0);
    turtle_add_vertex(x3 * xfact + xcenter, y3 * yfact + ycenter, r, g, b, a, 0, 0, 0);
    turtle_add_vertex(x1 * xfact + xcenter, y1 * yfact + ycenter, r, g, b, a, 0, 0, 0);
    turtle_add_vertex(x3 * xfact + xcenter, y3 * yfact + ycenter, r, g, b, a, 0, 0, 0);
    turtle_add_vertex(x4 * xfact + xcenter, y4 * yfact + ycenter, r, g, b, a, 0, 0, 0);
}

void turtle_texture_render_internal(int32_t textureCode, double x1, double y1, double x2, double y2, double r, double g, double b, double rot, double xcenter, double ycenter, double xfact, double yfact) {
    /* do the rotation math here - rotate on center */
    double avgX = (x1 + x2) / 2;
    double avgY = (y1 + y2) / 2;
    double sinRot = sin(rot);
    double cosRot = cos(rot);
    double x1Displace = x1 - avgX;
    double y1Displace = y1 - avgY;
    double x2Displace = x2 - avgX;
    double y2Displace = y2 - avgY;
    x1 = avgX + x1Displace * cosRot - y1Displace * sinRot;
    y1 = avgY + x1Displace * sinRot + y1Displace * cosRot;
    x2 = avgX + x2Displace * cosRot - y2Displace * sinRot;
    y2 = avgY + x2Displace * sinRot + y2Displace * cosRot;
    double x3 = avgX + x2Displace * cosRot - y1Displace * sinRot;
    double y3 = avgY + x2Displace * sinRot + y1Displace * cosRot;
    double x4 = avgX + x1Displace * cosRot - y2Displace * sinRot;
    double y4 = avgY + x1Displace * sinRot + y2Displace * cosRot;
    turtle_add_vertex(x4 * xfact + xcenter, y4 * yfact + ycenter, r, g, b, 1.0, 0, 0, textureCode);
    turtle_add_vertex(x1 * xfact + xcenter, y1 * yfact + ycenter, r, g, b, 1.0, 0, 1, textureCode);
    turtle_add_vertex(x3 * xfact + xcenter, y3 * yfact + ycenter, r, g, b, 1.0, 1, 1, textureCode);
    turtle_add_vertex(x4 * xfact + xcenter, y4 * yfact + ycenter, r, g, b, 1.0, 0, 0, textureCode);
    turtle_add_vertex(x2 * xfact + xcenter, y2 * yfact + ycenter, r, g, b, 1.0, 1, 0, textureCode);
    turtle_add_vertex(x3 * xfact + xcenter, y3 * yfact + ycenter, r, g, b, 1.0, 1, 1, textureCode);
}

void printList(double *list, int32_t len) {
    printf("[");
    for (int32_t i = 0; i < len; i++) {
        if (i == len - 1) {
            printf("%lf]\n", list[i]);
        } else {
            printf("%lf ", list[i]);
        }
    }
}

void printListInt(int32_t *list, int32_t len) {
    printf("[");
    for (int32_t i = 0; i < len; i++) {
        if (i == len - 1) {
            printf("%d]\n", list[i]);
        } else {
            printf("%d ", list[i]);
        }
    }
}

uint8_t *turtle_image_resize(uint8_t *dest, uint32_t destWidth, uint32_t destHeight, uint32_t destEncoding, uint8_t *src, uint32_t srcWidth, uint32_t srcHeight, uint32_t srcEncoding, turtle_image_resize_t method) {
    /* determine encoding */
    uint8_t destChannels = 0;
    if (destEncoding == GL_RGB) {
        destChannels = 3;
    } else if (destEncoding == GL_BGR) {
        destChannels = 3;
    } else if (destEncoding == GL_RGBA) {
        destChannels = 4;
    } else if (destEncoding == GL_BGRA) {
        destChannels = 4;
    } else if (destEncoding == GL_RED || destEncoding == GL_GREEN || destEncoding == GL_BLUE || destEncoding == GL_ALPHA) {
        destChannels = 1;
    } else {
        printf("turtle_image_resize: Unsupported destination encoding %d\n", destEncoding);
        return NULL;
    }
    uint8_t srcChannels = 0;
    if (srcEncoding == GL_RGB) {
        srcChannels = 3;
    } else if (srcEncoding == GL_BGR) {
        srcChannels = 3;
    } else if (srcEncoding == GL_RGBA) {
        srcChannels = 4;
    } else if (srcEncoding == GL_BGRA) {
        srcChannels = 4;
    } else if (srcEncoding == GL_RED || srcEncoding == GL_GREEN || srcEncoding == GL_BLUE || srcEncoding == GL_ALPHA) {
        srcChannels = 1;
    } else {
        printf("turtle_image_resize: Unsupported source encoding %d\n", srcEncoding);
        return NULL;
    }
    if (srcEncoding != destEncoding) {
        printf("turtle_image_resize: Source encoding must match destination encoding (TODO)\n");
        return NULL;
    }
    int8_t freeDest = 0;
    if (dest == NULL) {
        freeDest = 1;
        dest = malloc(destWidth * destHeight * destChannels);
    }
    uint8_t *destAlt = dest;
    if (method == TURTLE_IMAGE_RESIZE_SRGB || method == TURTLE_IMAGE_RESIZE_LINEAR) {
        /* TODO - make TURTLE_IMAGE_RESIZE_SRGB distinct from linear */
        /* TODO - validate that linear resize is consistent and doesn't crash */
        double strideWidth = (double) srcWidth / destWidth;
        double strideHeight = (double) srcHeight / destHeight;
        // printf("w: %lf h: %lf\n", strideWidth, strideHeight);
        double runningWidth = 0;
        double runningHeight = 0;
        double totalArea = strideWidth * strideHeight;
        int32_t widthOne = floor(strideWidth + 1) + 1;
        double *widths = malloc(widthOne * destWidth * sizeof(double));
        int32_t *extraWidths = malloc(destWidth * 2 * sizeof(int32_t)); // extraWidth, widthLen
        double *heights = malloc((floor(strideHeight + 1) + 1) * sizeof(double));
        int32_t *heightsPrecalc = malloc((floor(strideHeight + 1) + 1) * sizeof(int32_t));
        double pixels[destChannels];
        int32_t widthsLen = 0;
        for (int32_t j = 0; j < destWidth; j++) {
            double acruW = floor(runningWidth + 1) - runningWidth;
            if (strideWidth < acruW) {
                acruW = strideWidth;
                widths[j * widthOne] = acruW;
                widthsLen = j * widthOne + 1;
            } else {
                widths[j * widthOne] = acruW;
                widthsLen = j * widthOne + 1;
                while (strideWidth - acruW >= 1) {
                    widths[widthsLen] = 1;
                    acruW += 1;
                    widthsLen++;
                }
                if (strideWidth - acruW > 0.00001) { // TODO remove if statement
                    widths[widthsLen] = strideWidth - acruW;
                    widthsLen++;
                }
            }
            extraWidths[j * 2] = (int32_t) floor(runningWidth) * srcChannels;
            extraWidths[j * 2 + 1] = widthsLen - j * widthOne;
            runningWidth += strideWidth;
            // printList(widths, widthsLen);
        }
        // printListInt(extraWidths, destWidth * 2);
        int32_t heightsLen = 0;
        for (int32_t i = 0; i < destHeight; i++) {
            double acruH = floor(runningHeight + 1) - runningHeight;
            if (strideHeight < acruH) {
                acruH = strideHeight;
                heights[0] = acruH;
                heightsLen = 1;
            } else {
                heights[0] = acruH;
                heightsLen = 1;
                while (strideHeight - acruH > 1) {
                    heights[heightsLen] = 1;
                    acruH += 1;
                    heightsLen++;
                }
                if (strideHeight - acruH > 0.00001) { // TODO remove if statement
                    heights[heightsLen] = strideHeight - acruH;
                    heightsLen++;
                }
            }
            heightsPrecalc[0] = (int32_t) floor(runningHeight + 0.00001) * srcWidth * srcChannels;
            double heightSum = 0;
            for (int32_t j = 0; j < heightsLen - 1; j++) {
                heightSum += heights[j];
                heightsPrecalc[j + 1] = (int32_t) floor(runningHeight + heightSum + 0.00001) * srcWidth * srcChannels;
            }
            // printf("%d %d heights: ", i, destHeight);
            // printList(heights, heightsLen);
            runningWidth = 0;
            for (int32_t j = 0; j < destWidth; j++) {
                for (int32_t k = 0; k < destChannels; k++) {
                    pixels[k] = 0;
                }
                for (int32_t h = 0; h < heightsLen; h++) {
                    uint8_t *srcAlt = src + heightsPrecalc[h] + extraWidths[j * 2];
                    for (int32_t w = 0; w < extraWidths[j * 2 + 1]; w++) {
                        for (int32_t k = 0; k < destChannels; k++) {
                            pixels[k] += heights[h] * widths[j * widthOne + w] * (*srcAlt++);
                        }
                    }
                }
                for (int32_t k = 0; k < destChannels; k++) {
                    *destAlt++ = pixels[k] / totalArea;
                }
                runningWidth += strideWidth;
            }
            runningHeight += strideHeight;
        }
        free(widths);
        free(extraWidths);
        free(heights);
        free(heightsPrecalc);
    } else if (method == TURTLE_IMAGE_RESIZE_NEAREST) {
        int32_t precalculatedWidths[destWidth];
        int32_t precalculatedHeights[destHeight];
        for (int32_t i = 0; i < destHeight; i++) {
            precalculatedHeights[i] = (int32_t) ((double) (i + 0.5) / destHeight * srcHeight) * srcWidth * srcChannels;
            if (precalculatedHeights[i] >= srcHeight * srcWidth * srcChannels) {
                printf("ruh roh raggy\n");
                precalculatedHeights[i] = srcHeight * srcWidth * srcChannels - srcWidth * srcChannels;
            }
        }
        for (int32_t j = 0; j < destWidth; j++) {
            precalculatedWidths[j] = (int32_t) ((double) (j + 0.5) / destWidth * srcWidth) * srcChannels;
            if (precalculatedWidths[j] >= srcWidth * srcChannels) {
                printf("reykjavik\n");
                precalculatedWidths[j] = srcWidth * srcChannels - srcChannels;
            }
        }
        for (int32_t i = 0; i < destHeight; i++) {
            for (int32_t j = 0; j < destWidth; j++) {
                uint8_t *srcAlt = src + precalculatedHeights[i] + precalculatedWidths[j];
                for (int32_t k = 0; k < destChannels; k++) {
                    *destAlt++ = *srcAlt++;
                }
            }
        }
    } else {
        if (freeDest) {
            free(dest);
        }
        printf("turtle_image_resize: Unsupported method %d\n", method);
        return NULL;
    }
    return dest;
}

turtle_texture_t turtle_texture_load(char *filename) {
    /* load image */
    int width;
    int height;
    int channels;
    uint8_t *image = stbi_load(filename, &width, &height, &channels, 0);
    if (image == NULL) {
        printf("turtle_texture_load: Could not load image %s\n", filename);
        return -1;
    }
    uint32_t encoding = GL_RGB;
    if (channels == 4) {
        encoding = GL_RGBA;
    }
    if (channels == 1) {
        encoding = GL_RED;
    }
    uint8_t *resized = malloc(channels * turtle.textureWidth * turtle.textureHeight);
    turtle_image_resize(resized, turtle.textureWidth, turtle.textureHeight, encoding, image, width, height, encoding, TURTLE_IMAGE_RESIZE_LINEAR);
    free(image);
    if (resized == NULL) {
        printf("turtle_texture_load: Could not resize image %s\n", filename);
        return -1;
    }
    /* find first available texture */
    turtle_texture_t texture = -1;
    for (int32_t i = 4; i < turtle.textureList -> length; i += 4) {
        if (strcmp(turtle.textureList -> data[i].s, "") == 0) {
            free(turtle.textureList -> data[i].s);
            turtle.textureList -> data[i].s = strdup(filename);
            turtle.textureList -> data[i + 1].i = width;
            turtle.textureList -> data[i + 2].i = height;
            turtle.textureList -> data[i + 3].i = channels;
            texture = i;
            break;
        }
    }
    if (texture == -1) {
        texture = turtle.textureList -> length;
        list_append(turtle.textureList, (unitype) filename, 's');
        list_append(turtle.textureList, (unitype) width, 'i');
        list_append(turtle.textureList, (unitype) height, 'i');
        list_append(turtle.textureList, (unitype) channels, 'i');
    }
    /* load to GPU */
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, texture / 4, turtle.textureWidth, turtle.textureHeight, 1, encoding, GL_UNSIGNED_BYTE, resized);
    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
    free(resized);
    turtle.forceUpdate = 1;
    return texture;
}

turtle_texture_t turtle_texture_load_array(uint8_t *array, uint32_t width, uint32_t height, uint32_t encoding) {
    return turtle_texture_load_list_array_internal(NULL, array, width, height, encoding);
}

turtle_texture_t turtle_texture_load_list(list_t *list, uint32_t width, uint32_t height, uint32_t encoding) {
    return turtle_texture_load_list_array_internal(list, NULL, width, height, encoding);
}

turtle_texture_t turtle_texture_load_list_array_internal(list_t *list, uint8_t *array, uint32_t width, uint32_t height, uint32_t encoding) {
    /* determine encoding */
    uint8_t channels = 0;
    if (encoding == GL_RGB) {
        channels = 3;
    } else if (encoding == GL_BGR) {
        channels = 3;
    } else if (encoding == GL_RGBA) {
        channels = 4;
    } else if (encoding == GL_BGRA) {
        channels = 4;
    } else if (encoding == GL_RED || encoding == GL_GREEN || encoding == GL_BLUE || encoding == GL_ALPHA) {
        channels = 1;
    } else {
        printf("turtle_texture_load_list_array_internal: Unsupported encoding %d\n", encoding);
        return -1;
    }
    uint8_t freeArrayFlag = 0;
    if (array == NULL) {
        if (list == NULL) {
            printf("turtle_texture_load_list_array_internal: both list and array are NULL\n");
            return -1;
        }
        array = malloc(list -> length);
        for (int32_t i = 0; i < list -> length; i++) {
            array[i] = list -> data[i].b;
        }
        freeArrayFlag = 1;
    }
    uint8_t *resized = malloc(channels * turtle.textureWidth * turtle.textureHeight);
    turtle_image_resize(resized, turtle.textureWidth, turtle.textureHeight, encoding, array, width, height, encoding, TURTLE_IMAGE_RESIZE_LINEAR);
    if (freeArrayFlag) {
        free(array);
    }
    if (resized == NULL) {
        printf("turtle_texture_load_list_array_internal: Could not resize image\n");
        return -1;
    }
    /* find first available texture */
    turtle_texture_t texture = -1;
    char pointerValue[20];
    if (freeArrayFlag) {
        sprintf(pointerValue, "0x%llX", (uint64_t) list);
    } else {
        sprintf(pointerValue, "0x%llX", (uint64_t) array);
    }
    for (int32_t i = 4; i < turtle.textureList -> length; i += 4) {
        if (strcmp(turtle.textureList -> data[i].s, "") == 0) {
            free(turtle.textureList -> data[i].s);
            turtle.textureList -> data[i].s = strdup(pointerValue);
            turtle.textureList -> data[i + 1].i = width;
            turtle.textureList -> data[i + 2].i = height;
            turtle.textureList -> data[i + 3].i = channels;
            texture = i;
            break;
        }
    }
    if (texture == -1) {
        texture = turtle.textureList -> length;
        list_append(turtle.textureList, (unitype) pointerValue, 's');
        list_append(turtle.textureList, (unitype) width, 'i');
        list_append(turtle.textureList, (unitype) height, 'i');
        list_append(turtle.textureList, (unitype) channels, 'i');
    }
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, texture / 4, turtle.textureWidth, turtle.textureHeight, 1, encoding, GL_UNSIGNED_BYTE, resized);
    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
    free(resized);
    turtle.forceUpdate = 1;
    return texture;
}

int32_t turtle_texture_get_width(turtle_texture_t texture) {
    if (texture < 0 || texture >= turtle.textureList -> length) {
        return -1;
    }
    return turtle.textureList -> data[texture + 1].i;
}

int32_t turtle_texture_get_height(turtle_texture_t texture) {
    if (texture < 0 || texture >= turtle.textureList -> length) {
        return -1;
    }
    return turtle.textureList -> data[texture + 2].i;
}

void turtle_texture_print(turtle_texture_t texture) {
    printf("Texture ID: %d\n", texture);
    if (texture >= 0 && texture < turtle.textureList -> length) {
        printf("- Texture Name: %s\n", turtle.textureList -> data[texture].s);
        printf("- Texture Width: %d\n", turtle.textureList -> data[texture + 1].i);
        printf("- Texture Height: %d\n", turtle.textureList -> data[texture + 2].i);
        printf("- Texture Channel: %d\n", turtle.textureList -> data[texture + 3].i);
    } else {
        printf("- Texture Name: NULL\n");
        printf("- Texture Width: NULL\n");
        printf("- Texture Height: NULL\n");
        printf("- Texture Channel: NULL\n");
    }
}

/* replace a texture with new data from a png, jpg, or bmp */
int32_t turtle_texture_replace(turtle_texture_t texture, char *filename) {
    /* load image */
    int width;
    int height;
    int channels;
    uint8_t *image = stbi_load(filename, &width, &height, &channels, 0);
    if (image == NULL) {
        printf("turtle_texture_replace: Could not load image %s\n", filename);
        return -1;
    }
    uint32_t encoding = GL_RGB;
    if (channels == 4) {
        encoding = GL_RGBA;
    }
    if (channels == 1) {
        encoding = GL_RED;
    }
    uint8_t *resized = malloc(channels * turtle.textureWidth * turtle.textureHeight);
    turtle_image_resize(resized, turtle.textureWidth, turtle.textureHeight, encoding, image, width, height, encoding, TURTLE_IMAGE_RESIZE_LINEAR);
    free(image);
    if (resized == NULL) {
        printf("turtle_texture_replace: Could not resize image %s\n", filename);
        return -1;
    }
    /* load to GPU */
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, texture / 4, turtle.textureWidth, turtle.textureHeight, 1, encoding, GL_UNSIGNED_BYTE, resized);
    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
    free(resized);
    turtle.forceUpdate = 1;
    return texture;
}

/* replace a texture with new data from an array - supported encodings: GL_RGB, GL_RGBA, GL_BGR, GL_BGRA, GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA */
int32_t turtle_texture_replace_array(turtle_texture_t texture, uint8_t *array, uint32_t width, uint32_t height, uint32_t encoding) {
    return turtle_texture_replace_list_array_internal(texture, NULL, array, width, height, encoding);
}

/* replace a texture with new data from an list - supported encodings: GL_RGB, GL_RGBA, GL_BGR, GL_BGRA, GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA */
int32_t turtle_texture_replace_list(turtle_texture_t texture, list_t *list, uint32_t width, uint32_t height, uint32_t encoding) {
    return turtle_texture_replace_list_array_internal(texture, list, NULL, width, height, encoding);
}

int32_t turtle_texture_replace_list_array_internal(turtle_texture_t texture, list_t *list, uint8_t *array, uint32_t width, uint32_t height, uint32_t encoding) {
    /* determine encoding */
    uint8_t channels = 0;
    if (encoding == GL_RGB) {
        channels = 3;
    } else if (encoding == GL_BGR) {
        channels = 3;
    } else if (encoding == GL_RGBA) {
        channels = 4;
    } else if (encoding == GL_BGRA) {
        channels = 4;
    } else if (encoding == GL_RED || encoding == GL_GREEN || encoding == GL_BLUE || encoding == GL_ALPHA) {
        channels = 1;
    } else {
        printf("turtle_texture_replace_list_array_internal: Unsupported encoding %d\n", encoding);
        return -1;
    }
    uint8_t freeArrayFlag = 0;
    if (array == NULL) {
        if (list == NULL) {
            printf("turtle_texture_replace_list_array_internal: both list and array are NULL\n");
            return -1;
        }
        array = malloc(list -> length);
        for (int32_t i = 0; i < list -> length; i++) {
            array[i] = list -> data[i].b;
        }
        freeArrayFlag = 1;
    }
    uint8_t *resized = malloc(channels * turtle.textureWidth * turtle.textureHeight);
    turtle_image_resize(resized, turtle.textureWidth, turtle.textureHeight, encoding, array, width, height, encoding, TURTLE_IMAGE_RESIZE_LINEAR);
    if (freeArrayFlag) {
        free(array);
    }
    if (resized == NULL) {
        printf("turtle_texture_replace_list_array_internal: Could not resize image\n");
        return -1;
    }
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, texture / 4, turtle.textureWidth, turtle.textureHeight, 1, encoding, GL_UNSIGNED_BYTE, resized);
    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
    free(resized);
    turtle.forceUpdate = 1;
    return texture;
}

int32_t turtle_texture_unload(turtle_texture_t texture) {
    /* update list */
    if (texture >= turtle.textureList -> length || texture < 1) {
        return -1;
    }
    free(turtle.textureList -> data[texture].s);
    turtle.textureList -> data[texture].s = strdup("");
    turtle.forceUpdate = 1;
    return 0;
}

int32_t turtle_texture_unload_all() {
    list_free(turtle.textureList);
    list_append(turtle.textureList, (unitype) "null", 's'); // filename, cannot have texture code of 0 because of shader using 0 as the non-texture code
    list_append(turtle.textureList, (unitype) 0, 'i'); // width
    list_append(turtle.textureList, (unitype) 0, 'i'); // height
    list_append(turtle.textureList, (unitype) 0, 'i'); // channels
    turtle.forceUpdate = 1;
    return 0;
}

void turtle_texture(turtle_texture_t texture, double x1, double y1, double x2, double y2, double rot) {
    list_append(turtle.penPos, (unitype) x1, 'd');
    list_append(turtle.penPos, (unitype) y1, 'd');
    list_append(turtle.penPos, (unitype) x2, 'd');
    list_append(turtle.penPos, (unitype) y2, 'd');
    list_append(turtle.penPos, (unitype) rot, 'd'); // rotation (degrees, bearing)
    list_append(turtle.penPos, (unitype) 255, 'b');
    list_append(turtle.penPos, (unitype) 255, 'b');
    list_append(turtle.penPos, (unitype) (128 + texture / 4), 'h'); // blit texture signifier + texture code - limited to 32639 textures
    list_append(turtle.penPos, (unitype) 255, 'b');
}

void turtle_texture_color(turtle_texture_t texture, double x1, double y1, double x2, double y2, double rot, uint8_t r, uint8_t g, uint8_t b) {
    list_append(turtle.penPos, (unitype) x1, 'd');
    list_append(turtle.penPos, (unitype) y1, 'd');
    list_append(turtle.penPos, (unitype) x2, 'd');
    list_append(turtle.penPos, (unitype) y2, 'd');
    list_append(turtle.penPos, (unitype) rot, 'd'); // rotation (degrees, bearing)
    list_append(turtle.penPos, (unitype) r, 'b');
    list_append(turtle.penPos, (unitype) g, 'b');
    list_append(turtle.penPos, (unitype) (128 + texture / 4), 'h'); // blit texture signifier + texture code - limited to 32639 textures
    list_append(turtle.penPos, (unitype) b, 'b');
}

void turtle_set_texture_size(int32_t width, int32_t height) {
    turtle.textureWidth = width;
    turtle.textureHeight = height;
}

void turtle_set_max_textures(int32_t maxTextures) {
    turtle.maxTextures = maxTextures;
}
#endif /* TURTLE_ENABLE_TEXTURES */

void turtle_set_resize_mode(turtle_resize_mode_t resizeMode) {
    turtle.resizeMode = resizeMode;
}

/* adds a (blit) triangle to the pipeline (for better speed) */
void turtle_triangle(double x1, double y1, double x2, double y2, double x3, double y3) {
    list_append(turtle.penPos, (unitype) x1, 'd');
    list_append(turtle.penPos, (unitype) y1, 'd');
    list_append(turtle.penPos, (unitype) x2, 'd');
    list_append(turtle.penPos, (unitype) turtle.red, 'b');
    list_append(turtle.penPos, (unitype) turtle.green, 'b');
    list_append(turtle.penPos, (unitype) turtle.blue, 'b');
    list_append(turtle.penPos, (unitype) turtle.alpha, 'b');
    list_append(turtle.penPos, (unitype) 66, 'h'); // blit triangle signifier
    list_append(turtle.penPos, (unitype) y2, 'd'); // some unconventional formatting but it works

    list_append(turtle.penPos, (unitype) x3, 'd');
    list_append(turtle.penPos, (unitype) y3, 'd');
    list_append(turtle.penPos, (unitype) 0, 'd'); // zero'd out (wasted space)
    list_append(turtle.penPos, (unitype) turtle.red, 'b'); // duplicate colour data (wasted space)
    list_append(turtle.penPos, (unitype) turtle.green, 'b');
    list_append(turtle.penPos, (unitype) turtle.blue, 'b');
    list_append(turtle.penPos, (unitype) turtle.alpha, 'b');
    list_append(turtle.penPos, (unitype) 66, 'h'); // blit triangle signifier
    list_append(turtle.penPos, (unitype) 0, 'd'); // zero'd out (wasted space)
}

void turtle_triangle_color(double x1, double y1, double x2, double y2, double x3, double y3, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    list_append(turtle.penPos, (unitype) x1, 'd');
    list_append(turtle.penPos, (unitype) y1, 'd');
    list_append(turtle.penPos, (unitype) x2, 'd');
    list_append(turtle.penPos, (unitype) r, 'b');
    list_append(turtle.penPos, (unitype) g, 'b');
    list_append(turtle.penPos, (unitype) b, 'b');
    list_append(turtle.penPos, (unitype) (255 - a), 'b');
    list_append(turtle.penPos, (unitype) 66, 'h'); // blit triangle signifier
    list_append(turtle.penPos, (unitype) y2, 'd'); // some unconventional formatting but it works

    list_append(turtle.penPos, (unitype) x3, 'd');
    list_append(turtle.penPos, (unitype) y3, 'd');
    list_append(turtle.penPos, (unitype) 0, 'd'); // zero'd out (wasted space)
    list_append(turtle.penPos, (unitype) r, 'b'); // duplicate colour data (wasted space)
    list_append(turtle.penPos, (unitype) g, 'b');
    list_append(turtle.penPos, (unitype) b, 'b');
    list_append(turtle.penPos, (unitype) (255 - a), 'b');
    list_append(turtle.penPos, (unitype) 66, 'h'); // blit triangle signifier
    list_append(turtle.penPos, (unitype) 0, 'd'); // zero'd out (wasted space)
}

/* adds a (blit) quad to the pipeline (for better speed) */
void turtle_quad(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4) {
    list_append(turtle.penPos, (unitype) x1, 'd');
    list_append(turtle.penPos, (unitype) y1, 'd');
    list_append(turtle.penPos, (unitype) x2, 'd');
    list_append(turtle.penPos, (unitype) turtle.red, 'b');
    list_append(turtle.penPos, (unitype) turtle.green, 'b');
    list_append(turtle.penPos, (unitype) turtle.blue, 'b');
    list_append(turtle.penPos, (unitype) turtle.alpha, 'b');
    list_append(turtle.penPos, (unitype) 67, 'h'); // blit quad signifier
    list_append(turtle.penPos, (unitype) y2, 'd'); // some unconventional formatting but it works

    list_append(turtle.penPos, (unitype) x3, 'd');
    list_append(turtle.penPos, (unitype) y3, 'd');
    list_append(turtle.penPos, (unitype) x4, 'd');
    list_append(turtle.penPos, (unitype) turtle.red, 'b'); // duplicate colour data (wasted space)
    list_append(turtle.penPos, (unitype) turtle.green, 'b');
    list_append(turtle.penPos, (unitype) turtle.blue, 'b');
    list_append(turtle.penPos, (unitype) turtle.alpha, 'b');
    list_append(turtle.penPos, (unitype) 67, 'h'); // blit quad signifier
    list_append(turtle.penPos, (unitype) y4, 'd');
}

void turtle_quad_color(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    list_append(turtle.penPos, (unitype) x1, 'd');
    list_append(turtle.penPos, (unitype) y1, 'd');
    list_append(turtle.penPos, (unitype) x2, 'd');
    list_append(turtle.penPos, (unitype) r, 'b');
    list_append(turtle.penPos, (unitype) g, 'b');
    list_append(turtle.penPos, (unitype) b, 'b');
    list_append(turtle.penPos, (unitype) (255 - a), 'b');
    list_append(turtle.penPos, (unitype) 67, 'h'); // blit quad signifier
    list_append(turtle.penPos, (unitype) y2, 'd'); // some unconventional formatting but it works

    list_append(turtle.penPos, (unitype) x3, 'd');
    list_append(turtle.penPos, (unitype) y3, 'd');
    list_append(turtle.penPos, (unitype) x4, 'd');
    list_append(turtle.penPos, (unitype) r, 'b'); // duplicate colour data (wasted space)
    list_append(turtle.penPos, (unitype) g, 'b');
    list_append(turtle.penPos, (unitype) b, 'b');
    list_append(turtle.penPos, (unitype) (255 - a), 'b');
    list_append(turtle.penPos, (unitype) 67, 'h'); // blit quad signifier
    list_append(turtle.penPos, (unitype) y4, 'd');
}

/* adds a (blit) rectangle to the pipeline (uses quad interface) */
void turtle_rectangle(double x1, double y1, double x2, double y2) {
    list_append(turtle.penPos, (unitype) x1, 'd');
    list_append(turtle.penPos, (unitype) y1, 'd');
    list_append(turtle.penPos, (unitype) x2, 'd');
    list_append(turtle.penPos, (unitype) turtle.red, 'b');
    list_append(turtle.penPos, (unitype) turtle.green, 'b');
    list_append(turtle.penPos, (unitype) turtle.blue, 'b');
    list_append(turtle.penPos, (unitype) turtle.alpha, 'b');
    list_append(turtle.penPos, (unitype) 67, 'h'); // blit quad signifier
    list_append(turtle.penPos, (unitype) y1, 'd'); // some unconventional formatting but it works

    list_append(turtle.penPos, (unitype) x2, 'd');
    list_append(turtle.penPos, (unitype) y2, 'd');
    list_append(turtle.penPos, (unitype) x1, 'd');
    list_append(turtle.penPos, (unitype) turtle.red, 'b'); // duplicate colour data (wasted space)
    list_append(turtle.penPos, (unitype) turtle.green, 'b');
    list_append(turtle.penPos, (unitype) turtle.blue, 'b');
    list_append(turtle.penPos, (unitype) turtle.alpha, 'b');
    list_append(turtle.penPos, (unitype) 67, 'h'); // blit quad signifier
    list_append(turtle.penPos, (unitype) y2, 'd');
}

void turtle_rectangle_color(double x1, double y1, double x2, double y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    list_append(turtle.penPos, (unitype) x1, 'd');
    list_append(turtle.penPos, (unitype) y1, 'd');
    list_append(turtle.penPos, (unitype) x2, 'd');
    list_append(turtle.penPos, (unitype) r, 'b');
    list_append(turtle.penPos, (unitype) g, 'b');
    list_append(turtle.penPos, (unitype) b, 'b');
    list_append(turtle.penPos, (unitype) (255 - a), 'b');
    list_append(turtle.penPos, (unitype) 67, 'h'); // blit quad signifier
    list_append(turtle.penPos, (unitype) y1, 'd'); // some unconventional formatting but it works

    list_append(turtle.penPos, (unitype) x2, 'd');
    list_append(turtle.penPos, (unitype) y2, 'd');
    list_append(turtle.penPos, (unitype) x1, 'd');
    list_append(turtle.penPos, (unitype) r, 'b'); // duplicate colour data (wasted space)
    list_append(turtle.penPos, (unitype) g, 'b');
    list_append(turtle.penPos, (unitype) b, 'b');
    list_append(turtle.penPos, (unitype) (255 - a), 'b');
    list_append(turtle.penPos, (unitype) 67, 'h'); // blit quad signifier
    list_append(turtle.penPos, (unitype) y2, 'd');
}

/* adds a (blit) circle to the pipeline */
void turtle_circle(double x, double y, double radius) {
    list_append(turtle.penPos, (unitype) x, 'd');
    list_append(turtle.penPos, (unitype) y, 'd');
    list_append(turtle.penPos, (unitype) radius, 'd');
    list_append(turtle.penPos, (unitype) turtle.red, 'b');
    list_append(turtle.penPos, (unitype) turtle.green, 'b');
    list_append(turtle.penPos, (unitype) turtle.blue, 'b');
    list_append(turtle.penPos, (unitype) turtle.alpha, 'b');
    list_append(turtle.penPos, (unitype) 64, 'h'); // blit circle signifier
    list_append(turtle.penPos, (unitype) turtle.circleprez, 'd');
}

void turtle_circle_color(double x, double y, double radius, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    list_append(turtle.penPos, (unitype) x, 'd');
    list_append(turtle.penPos, (unitype) y, 'd');
    list_append(turtle.penPos, (unitype) radius, 'd');
    list_append(turtle.penPos, (unitype) r, 'b');
    list_append(turtle.penPos, (unitype) g, 'b');
    list_append(turtle.penPos, (unitype) b, 'b');
    list_append(turtle.penPos, (unitype) (255 - a), 'b');
    list_append(turtle.penPos, (unitype) 64, 'h'); // blit circle signifier
    list_append(turtle.penPos, (unitype) turtle.circleprez, 'd');
}

/* create a triangle in 3D */
void turtle_3D_Triangle(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3) {
    list_append(turtle.penPos, (unitype) x1, 'd');
    list_append(turtle.penPos, (unitype) y1, 'd');
    list_append(turtle.penPos, (unitype) z1, 'd');
    list_append(turtle.penPos, (unitype) turtle.red, 'b');
    list_append(turtle.penPos, (unitype) turtle.green, 'b');
    list_append(turtle.penPos, (unitype) turtle.blue, 'b');
    list_append(turtle.penPos, (unitype) turtle.alpha, 'b');
    list_append(turtle.penPos, (unitype) 130, 'h'); // blit 3D triangle signifier
    list_append(turtle.penPos, (unitype) x2, 'd');

    list_append(turtle.penPos, (unitype) y2, 'd');
    list_append(turtle.penPos, (unitype) z2, 'd');
    list_append(turtle.penPos, (unitype) x3, 'd');
    list_append(turtle.penPos, (unitype) y3, 'd');
    list_append(turtle.penPos, (unitype) z3, 'd');
    list_append(turtle.penPos, (unitype) 0, 'd');
    list_append(turtle.penPos, (unitype) 0, 'd');
    list_append(turtle.penPos, (unitype) 130, 'h'); // blit 3D triangle signifier
    list_append(turtle.penPos, (unitype) 0, 'd');
}

/* 3D -> 2D using perspective projection matrix */
void turtle_perspective(double x, double y, double z, double *xOut, double *yOut) {
    /* https://www.scratchapixel.com/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix/building-basic-perspective-projection-matrix.html */
    // double transform[16] = {
    //     1, 0, 0, 0,
    //     0, 1, 0, turtle.cameraDirectionLeftRight,
    //     0, 0, 1, turtle.cameraDirectionUpDown,
    //     0, 0, 0, 1,
    // };
    // x = x * transform[0 ] + y * transform[1 ] + z * transform[2 ] + transform[3 ];
    // y = x * transform[4 ] + y * transform[5 ] + z * transform[6 ] + transform[7 ];
    // z = x * transform[8 ] + y * transform[9 ] + z * transform[10] + transform[11];
    // double outWTest = x * transform[12] + y * transform[13] + z * transform[14] + transform[15];
    // x /= outWTest;
    // y /= outWTest;
    // z /= outWTest;
    // double scalingFactor = 1 / (tan((turtle.cameraFOV / 2) / 57.2958));
    // double near = 0.1;
    // double hypot = (x - turtle.cameraX) * (x - turtle.cameraX) + (y - turtle.cameraY) * (y - turtle.cameraY) + (z - turtle.cameraZ) * (z - turtle.cameraZ);
    // double far = 100;
    // // double far = hypot * sin((90 - turtle.cameraDirectionUpDown) / 57.2958) * sin((90 - turtle.cameraDirectionLeftRight) / 57.2958);
    // double threeThree = -far / (far - near);
    // double perspective[16] = {
    //     scalingFactor, 0,             0,                 0,
    //     0,             scalingFactor, 0,                 0,
    //     0,             0,             threeThree,       -1,
    //     0,             0,             threeThree * near, 0,
    // };
    // double outX = x * perspective[0 ] + y * perspective[1 ] + z * perspective[2 ] + perspective[3 ];
    // double outY = x * perspective[4 ] + y * perspective[5 ] + z * perspective[6 ] + perspective[7 ];
    // // double outZ = x * perspective[8 ] + y * perspective[9 ] + z * perspective[10] + perspective[11];
    // double outW = x * perspective[12] + y * perspective[13] + z * perspective[14] + perspective[15];
    // outX /= outW;
    // outY /= outW;
    // *xOut = outX * 640;
    // *yOut = outY * 360;
    // printf("%lf %lf\n", *xOut, *yOut);
}

/* draws the turtle's path on the screen, "this could all be a shader" */
void turtle_update() {
    /* bad fix to a niche problem part 1 */
    if (turtle.pen == 1) {
        if (turtle.penPos -> length > 0 && turtle.penPos -> type[turtle.penPos -> length - 1] != 'c') {
            list_append(turtle.penPos, (unitype) 0, 'c');
            list_append(turtle.penPos, (unitype) 0, 'c');
            list_append(turtle.penPos, (unitype) 0, 'c');
            list_append(turtle.penPos, (unitype) 0, 'c');
            list_append(turtle.penPos, (unitype) 0, 'c');
            list_append(turtle.penPos, (unitype) 0, 'c');
            list_append(turtle.penPos, (unitype) 0, 'c');
            list_append(turtle.penPos, (unitype) 0, 'c');
            list_append(turtle.penPos, (unitype) 0, 'c');
        }
    }
    /* used to have a feature that only redrew the screen if there have been any changes from last frame, but it has been removed.
       opted to redraw every frame and not list_copy, an alternative is hashing the penPos list. An interesting idea for sure... for another time */
    int8_t changed = 0;
    int32_t len = turtle.penPos -> length;
    unitype *ren = turtle.penPos -> data;
    int8_t *renType = turtle.penPos -> type;
    if (turtle.forceUpdate) {
        turtle.forceUpdate = 0;
        changed = 1;
    }
    glfwGetWindowSize(turtle.window, &turtle.screenbounds[0], &turtle.screenbounds[1]);
    if (turtle.screenbounds[0] != turtle.lastscreenbounds[0] || turtle.screenbounds[1] != turtle.lastscreenbounds[1]) {
        changed = 1;
        turtle.lastscreenbounds[0] = turtle.screenbounds[0];
        turtle.lastscreenbounds[1] = turtle.screenbounds[1];
    }
    if (len != turtle.lastLength) {
        changed = 1;
        turtle.lastLength = len;
    }
    if (changed == 0) {
        uint64_t oldHash = turtle.penHash;
        turtle.penHash = 0; // I don't use this but it's an idea: https://stackoverflow.com/questions/57455444/very-low-collision-non-cryptographic-hashing-function
        for (uint32_t i = 0; i < len; i++) {
            turtle.penHash += (uint64_t) turtle.penPos -> data[i].p; // simple addition hash. I know not technically safe since i cast all sizes to 8 byte, but it should still work
        }
        if (oldHash != turtle.penHash) {
            changed = 1;
        }
    }
    if (turtle.resizeMode == TURTLE_RESIZE_MODE_STRETCH) {
        turtle.bounds[0] = turtle.centerAndScale[0] - turtle.centerAndScale[2] / turtle.screenbounds[0];
        turtle.bounds[2] = turtle.centerAndScale[0] + turtle.centerAndScale[2] / turtle.screenbounds[0];
        turtle.bounds[1] = turtle.centerAndScale[1] - turtle.centerAndScale[3] / turtle.screenbounds[1];
        turtle.bounds[3] = turtle.centerAndScale[1] + turtle.centerAndScale[3] / turtle.screenbounds[1];
    } else if (turtle.resizeMode == TURTLE_RESIZE_MODE_PAD || turtle.resizeMode == TURTLE_RESIZE_MODE_PAD_NO_BARS) {
        double originalAspect = (double) turtle.initscreenbounds[0] / turtle.initscreenbounds[1];
        double currentAspect = (double) turtle.screenbounds[0] / turtle.screenbounds[1];
        if (currentAspect > originalAspect) {
            turtle.bounds[0] = turtle.centerAndScale[0] - turtle.centerAndScale[2] / turtle.screenbounds[0] * currentAspect / originalAspect;
            turtle.bounds[2] = turtle.centerAndScale[0] + turtle.centerAndScale[2] / turtle.screenbounds[0] * currentAspect / originalAspect;
            turtle.bounds[1] = turtle.centerAndScale[1] - turtle.centerAndScale[3] / turtle.screenbounds[1];
            turtle.bounds[3] = turtle.centerAndScale[1] + turtle.centerAndScale[3] / turtle.screenbounds[1];
        } else {
            turtle.bounds[0] = turtle.centerAndScale[0] - turtle.centerAndScale[2] / turtle.screenbounds[0];
            turtle.bounds[2] = turtle.centerAndScale[0] + turtle.centerAndScale[2] / turtle.screenbounds[0];
            turtle.bounds[1] = turtle.centerAndScale[1] - turtle.centerAndScale[3] / turtle.screenbounds[1] * originalAspect / currentAspect;
            turtle.bounds[3] = turtle.centerAndScale[1] + turtle.centerAndScale[3] / turtle.screenbounds[1] * originalAspect / currentAspect;
        }
    }
    if (changed) {
        #ifdef TURTLE_ENABLE_TEXTURES
        turtle.bufferList -> length = 0; // don't bother freeing the memory
        #endif /* TURTLE_ENABLE_TEXTURES */
        double xfact = 1.0 / ((turtle.bounds[2] - turtle.bounds[0]) / 2);
        double yfact = 1.0 / ((turtle.bounds[3] - turtle.bounds[1]) / 2);
        double xcenter = (double) turtle.screenbounds[0] / turtle.initscreenbounds[0] - 1 - (turtle.bounds[0] + turtle.bounds[2]) / 2 * xfact;
        double ycenter = (double) turtle.screenbounds[1] / turtle.initscreenbounds[1] - 1 - (turtle.bounds[1] + turtle.bounds[3]) / 2 * yfact;
        double lastSize = -1;
        double lastPrez = -1;
        double precomputedLog = 5;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for (int32_t i = 0; i < len; i += 9) {
            if (renType[i] == 'd') {
                switch (ren[i + 7].hu) {
                case TURTLE_PEN_SHAPE_CIRCLE: // penshape circle
                    if (lastSize != ren[i + 2].d || lastPrez != ren[i + 8].d) {
                        precomputedLog = ren[i + 8].d * log(2.71 + ren[i + 2].d);
                    }
                    lastSize = ren[i + 2].d;
                    lastPrez = ren[i + 8].d;
                    turtle_circle_render_internal(ren[i].d, ren[i + 1].d, ren[i + 2].d, ren[i + 3].b / 255.0, ren[i + 4].b / 255.0, ren[i + 5].b / 255.0, ren[i + 6].b / 255.0, xcenter, ycenter, xfact, yfact, precomputedLog);
                break;
                case TURTLE_PEN_SHAPE_SQUARE: // penshape square
                    turtle_rectangle_render_internal(ren[i].d - ren[i + 2].d, ren[i + 1].d - ren[i + 2].d, ren[i].d + ren[i + 2].d, ren[i + 1].d + ren[i + 2].d, ren[i + 3].b / 255.0, ren[i + 4].b / 255.0, ren[i + 5].b / 255.0, ren[i + 6].b / 255.0, xcenter, ycenter, xfact, yfact);
                break;
                case TURTLE_PEN_SHAPE_TRIANGLE: // penshape triangle
                    turtle_triangle_render_internal(ren[i].d - ren[i + 2].d, ren[i + 1].d - ren[i + 2].d, ren[i].d + ren[i + 2].d, ren[i + 1].d - ren[i + 2].d, ren[i].d, ren[i + 1].d + ren[i + 2].d, ren[i + 3].b / 255.0, ren[i + 4].b / 255.0, ren[i + 5].b / 255.0, ren[i + 6].b / 255.0, xcenter, ycenter, xfact, yfact);
                break;
                case TURTLE_PEN_SHAPE_TEXT: // penshape text
                    if (i - 9 < 0 || i + 9 >= len || renType[i - 1] == 'c' || ren[i - 2].hu > 5) {
                        if (lastSize != ren[i + 2].d || lastPrez != ren[i + 8].d) {
                            precomputedLog = ren[i + 8].d * log(2.71 + ren[i + 2].d);
                        }
                        lastSize = ren[i + 2].d;
                        lastPrez = ren[i + 8].d;
                        turtle_circle_render_internal(ren[i].d, ren[i + 1].d, ren[i + 2].d, ren[i + 3].b / 255.0, ren[i + 4].b / 255.0, ren[i + 5].b / 255.0, ren[i + 6].b / 255.0, xcenter, ycenter, xfact, yfact, precomputedLog);
                    }
                break;
                default:
                break;
                }
                if (i + 18 < len && renType[i + 9] == 'd' && ren[i + 7].hu < 64 && (ren[i + 7].hu == 4 || ren[i + 7].hu == 5 || (fabs(ren[i].d - ren[i + 9].d) > ren[i + 2].d / 2 || fabs(ren[i + 1].d - ren[i + 10].d) > ren[i + 2].d / 2))) { // tests for next point continuity and also ensures that the next point is at sufficiently different coordinates
                    double dir = atan((ren[i + 9].d - ren[i].d) / (ren[i + 1].d - ren[i + 10].d));
                    double sinn = sin(dir + M_PI / 2);
                    double coss = cos(dir + M_PI / 2);
                    turtle_quad_render_internal(ren[i].d + ren[i + 2].d * sinn, ren[i + 1].d - ren[i + 2].d * coss, ren[i + 9].d + ren[i + 2].d * sinn, ren[i + 10].d - ren[i + 2].d * coss, ren[i + 9].d - ren[i + 2].d * sinn, ren[i + 10].d + ren[i + 2].d * coss, ren[i].d - ren[i + 2].d * sinn, ren[i + 1].d + ren[i + 2].d * coss, ren[i + 3].b / 255.0, ren[i + 4].b / 255.0, ren[i + 5].b / 255.0, ren[i + 6].b / 255.0, xcenter, ycenter, xfact, yfact);
                    if ((ren[i + 7].hu == 4 || ren[i + 7].hu == 5) && i + 18 < len && renType[i + 18] == 'd') {
                        double dir2 = atan((ren[i + 18].d - ren[i + 9].d) / (ren[i + 10].d - ren[i + 19].d));
                        double sinn2 = sin(dir2 + M_PI / 2);
                        double coss2 = cos(dir2 + M_PI / 2);
                        turtle_triangle_render_internal(ren[i + 9].d + ren[i + 2].d * sinn, ren[i + 10].d - ren[i + 2].d * coss, ren[i + 9].d - ren[i + 2].d * sinn, ren[i + 10].d + ren[i + 2].d * coss, ren[i + 9].d + ren[i + 11].d * sinn2, ren[i + 10].d - ren[i + 11].d * coss2, ren[i + 3].b / 255.0, ren[i + 4].b / 255.0, ren[i + 5].b / 255.0, ren[i + 6].b / 255.0, xcenter, ycenter, xfact, yfact); // in a perfect world the program would know which one of these triangles to render (to blend the segments)
                        turtle_triangle_render_internal(ren[i + 9].d + ren[i + 2].d * sinn, ren[i + 10].d - ren[i + 2].d * coss, ren[i + 9].d - ren[i + 2].d * sinn, ren[i + 10].d + ren[i + 2].d * coss, ren[i + 9].d - ren[i + 11].d * sinn2, ren[i + 10].d + ren[i + 11].d * coss2, ren[i + 3].b / 255.0, ren[i + 4].b / 255.0, ren[i + 5].b / 255.0, ren[i + 6].b / 255.0, xcenter, ycenter, xfact, yfact); // however we live in a world where i am bad at math, so it just renders both no matter what (one has no effect)
                    }
                } else {
                    if (ren[i + 7].hu == TURTLE_PEN_SHAPE_CONNECTED && i > 8 && renType[i - 8] == 'c') {
                        if (!(lastSize == ren[i + 2].d) || !(lastPrez != ren[i + 8].d)) {
                            precomputedLog = ren[i + 8].d * log(2.71 + ren[i + 2].d);
                        }
                        lastSize = ren[i + 2].d;
                        lastPrez = ren[i + 8].d;
                        turtle_circle_render_internal(ren[i].d, ren[i + 1].d, ren[i + 2].d, ren[i + 3].b / 255.0, ren[i + 4].b / 255.0, ren[i + 5].b / 255.0, ren[i + 6].b / 255.0, xcenter, ycenter, xfact, yfact, precomputedLog);
                    }
                    if (ren[i + 7].hu == TURTLE_PEN_SHAPE_TEXT && i > 8) {
                        if (!(lastSize == ren[i + 2].d) || !(lastPrez != ren[i + 8].d)) {
                            precomputedLog = ren[i + 8].d * log(2.71 + ren[i + 2].d);
                        }
                        lastSize = ren[i + 2].d;
                        lastPrez = ren[i + 8].d;
                        turtle_circle_render_internal(ren[i].d, ren[i + 1].d, ren[i + 2].d, ren[i + 3].b / 255.0, ren[i + 4].b / 255.0, ren[i + 5].b / 255.0, ren[i + 6].b / 255.0, xcenter, ycenter, xfact, yfact, precomputedLog);
                    }
                }
                if (ren[i + 7].hu == 64) { // blit circle
                    if (!(lastSize == ren[i + 2].d) || !(lastPrez != ren[i + 8].d)) {
                        precomputedLog = ren[i + 8].d * log(2.71 + ren[i + 2].d);
                    }
                    lastSize = ren[i + 2].d;
                    lastPrez = ren[i + 8].d;
                    turtle_circle_render_internal(ren[i].d, ren[i + 1].d, ren[i + 2].d, ren[i + 3].b / 255.0, ren[i + 4].b / 255.0, ren[i + 5].b / 255.0, ren[i + 6].b / 255.0, xcenter, ycenter, xfact, yfact, precomputedLog);
                }
                if (ren[i + 7].hu == 66) { // blit triangle
                    turtle_triangle_render_internal(ren[i].d, ren[i + 1].d, ren[i + 2].d, ren[i + 8].d, ren[i + 9].d, ren[i + 10].d, ren[i + 3].b / 255.0, ren[i + 4].b / 255.0, ren[i + 5].b / 255.0, ren[i + 6].b / 255.0, xcenter, ycenter, xfact, yfact);
                    i += 9;
                }
                if (ren[i + 7].hu == 67) { // blit quad
                    turtle_quad_render_internal(ren[i].d, ren[i + 1].d, ren[i + 2].d, ren[i + 8].d, ren[i + 9].d, ren[i + 10].d, ren[i + 11].d, ren[i + 17].d, ren[i + 3].b / 255.0, ren[i + 4].b / 255.0, ren[i + 5].b / 255.0, ren[i + 6].b / 255.0, xcenter, ycenter, xfact, yfact);
                    i += 9;
                }
                #ifdef TURTLE_ENABLE_TEXTURES
                if (ren[i + 7].hu >= 128) { // blit texture (rectangle)
                    turtle_texture_render_internal(ren[i + 7].hu - 128, ren[i].d, ren[i + 1].d, ren[i + 2].d, ren[i + 3].d, ren[i + 5].b / 255.0, ren[i + 6].b / 255.0, ren[i + 8].b / 255.0, ren[i + 4].d / 57.2958, xcenter, ycenter, xfact, yfact);
                }
                #endif /* TURTLE_ENABLE_TEXTURES */
                // if (ren[i + 7].hu == 256) { // blit 3D sphere

                // }
                // if (ren[i + 7].hu == 257) { // blit 3D circle

                // }
                // if (ren[i + 7].hu == 258) { // blit 3D triangle
                //     turtle_perspective(ren[i].d, ren[i + 1].d, ren[i + 2].d, &ren[i].d, &ren[i + 1].d);
                //     turtle_perspective(ren[i + 8].d, ren[i + 9].d, ren[i + 10].d, &ren[i + 8].d, &ren[i + 9].d);
                //     turtle_perspective(ren[i + 11].d, ren[i + 12].d, ren[i + 13].d, &ren[i + 11].d, &ren[i + 12].d);
                //     turtle_triangle_render_internal(ren[i].d, ren[i + 1].d, ren[i + 8].d, ren[i + 9].d, ren[i + 11].d, ren[i + 12].d, ren[i + 3].b / 255.0, ren[i + 4].b / 255.0, ren[i + 5].b / 255.0, ren[i + 6].b / 255.0, xcenter, ycenter, xfact, yfact);
                //     i += 9;
                // }
                // if (ren[i + 7].hu == 259) { // blit 3D quad

                // }
            }
        }
        if (turtle.resizeMode == TURTLE_RESIZE_MODE_PAD) {
            /* pad sides of window with black bars */
            double originalAspect = (double) turtle.initscreenbounds[0] / turtle.initscreenbounds[1];
            double currentAspect = (double) turtle.screenbounds[0] / turtle.screenbounds[1];
            if (currentAspect > originalAspect) {
                turtle_rectangle_render_internal(turtle.initbounds[0], turtle.initbounds[1], turtle.bounds[0], turtle.initbounds[3], 0, 0, 0, 1.0, xcenter, ycenter, xfact, yfact);
                turtle_rectangle_render_internal(turtle.initbounds[2], turtle.initbounds[1], turtle.bounds[2], turtle.initbounds[3], 0, 0, 0, 1.0, xcenter, ycenter, xfact, yfact);
            } else {
                turtle_rectangle_render_internal(turtle.initbounds[0], turtle.initbounds[1], turtle.initbounds[2], turtle.bounds[1], 0, 0, 0, 1.0, xcenter, ycenter, xfact, yfact);
                turtle_rectangle_render_internal(turtle.initbounds[0], turtle.initbounds[3], turtle.initbounds[2], turtle.bounds[3], 0, 0, 0, 1.0, xcenter, ycenter, xfact, yfact);
            }
        }
        #ifdef TURTLE_ENABLE_TEXTURES
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * turtle.bufferList -> length, turtle.bufferList -> data, GL_STATIC_DRAW);
        glDrawArrays(GL_TRIANGLES, 0, turtle.bufferList -> length / BUFFER_OBJECT_SIZE);
        #endif /* TURTLE_ENABLE_TEXTURES */
        glfwSwapBuffers(turtle.window);
    }
    /* bad fix to niche problem part 2 */
    if (turtle.pen == 1) {
        for (int8_t i = 0; i < 9; i++) {
            list_pop(turtle.penPos);
        }
    }
    glfwPollEvents();
    if (glfwWindowShouldClose(turtle.window)) {
        turtle.close = 1;
        if (turtle.popupClose) {
            glfwTerminate();
        }
    }
    #ifdef OS_BROWSER
    emscripten_sleep(0);
    #endif /* OS_BROWSER */
}

/* keeps the window open while doing nothing else (from python turtle.mainloop()) */
void turtle_main_loop() {
    while (turtle.close == 0) {
        turtle_update();
    }
}

/* free turtle memory */
void turtle_free() {
    list_free(turtle.keyPressed);
    list_free(turtle.penPos);
    #ifdef TURTLE_ENABLE_TEXTURES
    floatList_free(turtle.bufferList);
    #endif /* TURTLE_ENABLE_TEXTURES */
    glfwTerminate();
}
