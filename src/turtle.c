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
"#version 330 core\n"
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
"#version 330 core\n"
"uniform sampler2DArray textureImages;\n"
"in vec4 shadeColor;\n;"
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

/* initializes the turtletools module */
void turtleInit(GLFWwindow* window, int32_t minX, int32_t minY, int32_t maxX, int32_t maxY) {
    #ifndef TURTLE_ENABLE_TEXTURES
    /* fixed pipeline */
    gladLoadGL();
    #endif /* TURTLE_ENABLE_TEXTURES */
    #ifdef TURTLE_ENABLE_TEXTURES
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("could not initialise glad\n");
    }
    if (glGenVertexArrays == NULL) {
        printf("couldn't load openGL\n");
    }

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
    }
    int32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &turtleFragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, errorMessage);
        printf("Error compiling fragment shader\n");
        printf("%s\n", errorMessage);
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
    }
    
    glUseProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(shaderProgram);
    turtle.bufferList = bufferList_init();
    turtle.textureList = list_init();
    list_append(turtle.textureList, (unitype) "null", 's'); // cannot have texture code of 0 because of shader using 0 as the non-texture code
    /* setup texture parameters */
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    turtle.textureWidth = 2048;
    turtle.textureHeight = 2048;
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGB, turtle.textureWidth, turtle.textureHeight, 100, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); // 100 textures at 2048x2048
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
    turtle.penshape = 0;
    turtle.circleprez = 9; // default circleprez value
    turtle.pen = 0;
    turtle.penr = 0.0;
    turtle.peng = 0.0;
    turtle.penb = 0.0;
    turtle.pena = 0.0;
    for (uint8_t i = 0; i < 3; i++) {
        turtle.currentColor[i] = 0.0;
    }
    turtle.currentColor[3] = 1.0;
    /* 3D variables */
    turtle.cameraX = 0;
    turtle.cameraY = 0;
    turtle.cameraZ = 0;
    turtle.cameraFOV = 90;
    turtle.cameraDirectionLeftRight = 0;
    turtle.cameraDirectionUpDown = 0;

    turtleSetWorldCoordinates(minX, minY, maxX, maxY);
    turtle.keyCallback = NULL;
    turtle.unicodeCallback = NULL;
    glfwSetCharCallback(window, unicodeSense);
    glfwSetKeyCallback(window, keySense); // initiate mouse and keyboard detection
    glfwSetMouseButtonCallback(window, mouseSense);
    glfwSetScrollCallback(window, scrollSense);
}

/* run this to set the bounds of the window in coordinates */
void turtleSetWorldCoordinates(int32_t minX, int32_t minY, int32_t maxX, int32_t maxY) {
    glfwGetWindowSize(turtle.window, &turtle.screenbounds[0], &turtle.screenbounds[1]);
    turtle.centerAndScale[0] = (double) (maxX + minX) / 2;
    turtle.centerAndScale[1] = (double) (maxY + minY) / 2;
    turtle.centerAndScale[2] = (double) (maxX - minX) / 2 * turtle.screenbounds[0];
    turtle.centerAndScale[3] = (double) (maxY - minY) / 2 * turtle.screenbounds[1];
    turtle.initscreenbounds[0] = turtle.screenbounds[0];
    turtle.initscreenbounds[1] = turtle.screenbounds[1];
    turtle.initbounds[0] = minX;
    turtle.initbounds[1] = minY;
    turtle.initbounds[2] = maxX;
    turtle.initbounds[3] = maxY;
    memcpy(turtle.bounds, turtle.initbounds, 4 * sizeof(int32_t));
}

/* detect character */
void unicodeSense(GLFWwindow *window, uint32_t codepoint) {
    if (turtle.unicodeCallback != NULL) {
        turtle.unicodeCallback(codepoint);
    }
}

/* detect key presses */
void keySense(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods) {
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
void mouseSense(GLFWwindow *window, int32_t button, int32_t action, int32_t mods) {
    if (action == GLFW_PRESS) {
        switch(button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            list_append(turtle.keyPressed, (unitype) "m1", 's');
            turtle.mousePressed[0] = 1;
        break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            list_append(turtle.keyPressed, (unitype) "m2", 's');
            turtle.mousePressed[1] = 1;
        break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            list_append(turtle.keyPressed, (unitype) "m3", 's');
            turtle.mousePressed[2] = 1;
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
void scrollSense(GLFWwindow* window, double xoffset, double yoffset) {
    turtle.scrollY = yoffset;
}

/* the behavior with the mouse wheel is different since it can't be "on" or "off" */
double turtleMouseWheel() {
    double temp = turtle.scrollY;
    turtle.scrollY = 0;
    return temp;
}

/* top level boolean output call to check if the key with code [key] is currently being held down. Uses the GLFW_KEY_X macros */
int8_t turtleKeyPressed(int32_t key) {
    return list_count(turtle.keyPressed, (unitype) key, 'c');
}

/* top level boolean output call to check if the left click button is currently being held down */
int8_t turtleMouseDown() {
    return turtle.mousePressed[0];
}

/* top level boolean output call to check if the right click button is currently being held down */
int8_t turtleMouseRight() {
    return turtle.mousePressed[1];
}

/* top level boolean output call to check if the middle mouse button is currently being held down */
int8_t turtleMouseMiddle() {
    return turtle.mousePressed[2];
}

/* alternate duplicate of top level boolean output call to check if the middle mouse button is currently being held down */
int8_t turtleMouseMid() {
    return turtle.mousePressed[2];
}

/* gets the mouse coordinates */
void turtleGetMouseCoords() {
    glfwGetCursorPos(turtle.window, &turtle.mouseAbsX, &turtle.mouseAbsY); // get mouse positions (absolute)
    turtle.mouseX = turtle.mouseAbsX;
    turtle.mouseY = turtle.mouseAbsY;
    turtle.mouseX = (turtle.mouseAbsX - turtle.screenbounds[0] / 2) / turtle.screenbounds[0] * (turtle.initbounds[2] - turtle.initbounds[0]);
    turtle.mouseY = (turtle.mouseAbsY - turtle.screenbounds[1] / 2) / turtle.screenbounds[1] * (turtle.initbounds[1] - turtle.initbounds[3]);
}

/* set the background color */
void turtleBgColor(double r, double g, double b) {
    glClearColor(r / 255, g / 255, b / 255, 255.0);
}

/* set the pen color */
void turtlePenColor(double r, double g, double b) {
    turtle.penr = r / 255;
    turtle.peng = g / 255;
    turtle.penb = b / 255;
    turtle.pena = 255.0;
}

/* set the pen color (with transparency) */
void turtlePenColorAlpha(double r, double g, double b, double a) {
    turtle.penr = r / 255;
    turtle.peng = g / 255;
    turtle.penb = b / 255;
    turtle.pena = 1.0 - a / 255;
}

/* set the pen size */
void turtlePenSize(double size) {
    turtle.pensize = size * 0.5; // ensures pensize matches pixel size (a pen size of 240 will be 240 coordinates long)
}

/* clears all the pen drawings */
void turtleClear() {
    // list_free(turtle.penPos);
    // turtle.penPos = list_init();
    turtle.penPos -> length = 0; // could be dangerous
}

/* pen down */
void turtlePenDown() {
    if (turtle.pen == 0) {
        turtle.pen = 1;
        int8_t changed = 0;
        int32_t len = turtle.penPos -> length;
        if (len > 0) {
            unitype *ren = turtle.penPos -> data;
            if (ren[len - 9].d != turtle.x) {changed = 1;}
            if (ren[len - 8].d != turtle.y) {changed = 1;}
            if (ren[len - 7].d != turtle.pensize) {changed = 1;}
            if (ren[len - 6].d != turtle.penr) {changed = 1;}
            if (ren[len - 5].d != turtle.peng) {changed = 1;}
            if (ren[len - 4].d != turtle.penb) {changed = 1;}
            if (ren[len - 3].d != turtle.pena) {changed = 1;}
            if (ren[len - 2].h != turtle.penshape) {changed = 1;}
            if (ren[len - 1].d != turtle.circleprez) {changed = 1;}
        } else {
            changed = 1;
        }
        if (changed == 1) {
            list_append(turtle.penPos, (unitype) turtle.x, 'd');
            list_append(turtle.penPos, (unitype) turtle.y, 'd');
            list_append(turtle.penPos, (unitype) turtle.pensize, 'd');
            list_append(turtle.penPos, (unitype) turtle.penr, 'd');
            list_append(turtle.penPos, (unitype) turtle.peng, 'd');
            list_append(turtle.penPos, (unitype) turtle.penb, 'd');
            list_append(turtle.penPos, (unitype) turtle.pena, 'd');
            list_append(turtle.penPos, (unitype) turtle.penshape, 'h');
            list_append(turtle.penPos, (unitype) turtle.circleprez, 'd');
        }
    }
}

/* lift the pen */
void turtlePenUp() {
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
void turtlePenShape(char *selection) {
    if (strcmp(selection, "circle") == 0 || strcmp(selection, "Circle") == 0) {
        turtle.penshape = 0;
    }
    if (strcmp(selection, "square") == 0 || strcmp(selection, "Square") == 0) {
        turtle.penshape = 1;
    }
    if (strcmp(selection, "triangle") == 0 || strcmp(selection, "Triangle") == 0) {
        turtle.penshape = 2;
    }
    if (strcmp(selection, "none") == 0 || strcmp(selection, "None") == 0) {
        turtle.penshape = 3;
    }
    if (strcmp(selection, "connected") == 0 || strcmp(selection, "Connected") == 0) {
        turtle.penshape = 4;
    }
    if (strcmp(selection, "text") == 0 || strcmp(selection, "Text") == 0) {
        turtle.penshape = 5;
    }
}

/* set the circle precision */
void turtlePenPrez(double prez) {
    turtle.circleprez = prez;
}

/* moves the turtle to a coordinate */
void turtleGoto(double x, double y) {
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
                if (ren[len - 6].d != turtle.penr) {changed = 1;}
                if (ren[len - 5].d != turtle.peng) {changed = 1;}
                if (ren[len - 4].d != turtle.penb) {changed = 1;}
                if (ren[len - 3].d != turtle.pena) {changed = 1;}
                if (ren[len - 2].h != turtle.penshape) {changed = 1;}
                if (ren[len - 1].d != turtle.circleprez) {changed = 1;}
            } else {
                changed = 1;
            }
            if (changed == 1) {
                list_append(turtle.penPos, (unitype) x, 'd');
                list_append(turtle.penPos, (unitype) y, 'd');
                list_append(turtle.penPos, (unitype) turtle.pensize, 'd');
                list_append(turtle.penPos, (unitype) turtle.penr, 'd');
                list_append(turtle.penPos, (unitype) turtle.peng, 'd');
                list_append(turtle.penPos, (unitype) turtle.penb, 'd');
                list_append(turtle.penPos, (unitype) turtle.pena, 'd');
                list_append(turtle.penPos, (unitype) turtle.penshape, 'h');
                list_append(turtle.penPos, (unitype) turtle.circleprez, 'd');
            }
        }
    }
}

#ifndef TURTLE_ENABLE_TEXTURES
/* draws a circle at the specified x and y (coordinates) */
void turtleCircleRenderInternal(double x, double y, double rad, double r, double g, double b, double a, double xcenter, double ycenter, double xfact, double yfact, double prez) {
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

/* draws a square */
void turtleSquareRenderInternal(double x1, double y1, double x2, double y2, double r, double g, double b, double a, double xcenter, double ycenter, double xfact, double yfact) {
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
void turtleTriangleRenderInternal(double x1, double y1, double x2, double y2, double x3, double y3, double r, double g, double b, double a, double xcenter, double ycenter, double xfact, double yfact) {
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
void turtleQuadRenderInternal(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double r, double g, double b, double a, double xcenter, double ycenter, double xfact, double yfact) {
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

turtle_texture_t turtleTextureLoad(char *filename) {
    printf("turtleTextureLoad: TURTLE_ENABLE_TEXTURES not enabled - textures will not render\n");
    turtle_texture_t output;
    output.id = -1;
    return output;
}

int32_t turtleTextureUnload(turtle_texture_t texture) {
    return -1;
}

void turtleTexture(turtle_texture_t texture, double x1, double y1, double x2, double y2, double rot, double r, double g, double b) {
    return;
}

void turtlePrintTexture(turtle_texture_t texture) {
    printf("turtlePrintTexture: TURTLE_ENABLE_TEXTURES not enabled\n");
}
#endif /* TURTLE_ENABLE_TEXTURES */

#ifdef TURTLE_ENABLE_TEXTURES
/* function to add a vertex to the turtle.bufferList */
void addVertex(double x, double y, double r, double g, double b, double a, double tx, double ty, double useTexture) {
    bufferList_append(turtle.bufferList, x);
    bufferList_append(turtle.bufferList, y);
    bufferList_append(turtle.bufferList, r);
    bufferList_append(turtle.bufferList, g);
    bufferList_append(turtle.bufferList, b);
    bufferList_append(turtle.bufferList, a);
    bufferList_append(turtle.bufferList, tx);
    bufferList_append(turtle.bufferList, ty);
    bufferList_append(turtle.bufferList, useTexture);
}

/* draws a circle at the specified x and y (coordinates) */
void turtleCircleRenderInternal(double x, double y, double rad, double r, double g, double b, double a, double xcenter, double ycenter, double xfact, double yfact, double prez) {
    int32_t p = (int32_t) prez;
    if (p > 0) {
        // p--;
        float originX = x * xfact + xcenter;
        float originY = (y + rad) * yfact + ycenter;
        addVertex(originX, originY, r, g, b, a, 0, 0, 0);
        addVertex((x + rad * sin(2 * 1 * M_PI / prez)) * xfact + xcenter, (y + rad * cos(2 * 1 * M_PI / prez)) * yfact + ycenter, r, g, b, a, 0, 0, 0);
        int32_t i = 0;
        for (; i < p; i++) {
            addVertex((x + rad * sin(2 * i * M_PI / prez)) * xfact + xcenter, (y + rad * cos(2 * i * M_PI / prez)) * yfact + ycenter, r, g, b, a, 0, 0, 0);
            addVertex(originX, originY, r, g, b, a, 0, 0, 0);
            addVertex(turtle.bufferList -> data[turtle.bufferList -> length - BUFFER_OBJECT_SIZE * 2], turtle.bufferList -> data[turtle.bufferList -> length - BUFFER_OBJECT_SIZE * 2 + 1], r, g, b, a, 0, 0, 0);
        }
        addVertex((x + rad * sin(2 * i * M_PI / prez)) * xfact + xcenter, (y + rad * cos(2 * i * M_PI / prez)) * yfact + ycenter, r, g, b, a, 0, 0, 0);
    }
}

/* draws a square */
void turtleSquareRenderInternal(double x1, double y1, double x2, double y2, double r, double g, double b, double a, double xcenter, double ycenter, double xfact, double yfact) {
    // float square[24] = {x1 * xfact, y1 * yfact, r, g, b, a, x2 * xfact, y1 * yfact, r, g, b, a, x2 * xfact, y2 * yfact, r, g, b, a, x1 * xfact, y2 * yfact, r, g, b, a};
    addVertex(x1 * xfact + xcenter, y1 * yfact + ycenter, r, g, b, a, 0, 0, 0);
    addVertex(x2 * xfact + xcenter, y1 * yfact + ycenter, r, g, b, a, 0, 0, 0);
    addVertex(x2 * xfact + xcenter, y2 * yfact + ycenter, r, g, b, a, 0, 0, 0);
    addVertex(x1 * xfact + xcenter, y1 * yfact + ycenter, r, g, b, a, 0, 0, 0);
    addVertex(x2 * xfact + xcenter, y2 * yfact + ycenter, r, g, b, a, 0, 0, 0);
    addVertex(x1 * xfact + xcenter, y2 * yfact + ycenter, r, g, b, a, 0, 0, 0);
}

/* draws a triangle */
void turtleTriangleRenderInternal(double x1, double y1, double x2, double y2, double x3, double y3, double r, double g, double b, double a, double xcenter, double ycenter, double xfact, double yfact) {
    // float triangle[18] = {x1 * xfact, y1 * yfact, r, g, b, a, x2 * xfact, y2 * yfact, r, g, b, a, x3 * xfact, y3 * yfact, r, g, b, a};
    addVertex(x1 * xfact + xcenter, y1 * yfact + ycenter, r, g, b, a, 0, 0, 0);
    addVertex(x2 * xfact + xcenter, y2 * yfact + ycenter, r, g, b, a, 0, 0, 0);
    addVertex(x3 * xfact + xcenter, y3 * yfact + ycenter, r, g, b, a, 0, 0, 0);
}

/* draws a quadrilateral */
void turtleQuadRenderInternal(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double r, double g, double b, double a, double xcenter, double ycenter, double xfact, double yfact) {
    addVertex(x1 * xfact + xcenter, y1 * yfact + ycenter, r, g, b, a, 0, 0, 0);
    addVertex(x2 * xfact + xcenter, y2 * yfact + ycenter, r, g, b, a, 0, 0, 0);
    addVertex(x3 * xfact + xcenter, y3 * yfact + ycenter, r, g, b, a, 0, 0, 0);
    addVertex(x1 * xfact + xcenter, y1 * yfact + ycenter, r, g, b, a, 0, 0, 0);
    addVertex(x3 * xfact + xcenter, y3 * yfact + ycenter, r, g, b, a, 0, 0, 0);
    addVertex(x4 * xfact + xcenter, y4 * yfact + ycenter, r, g, b, a, 0, 0, 0);
}

void turtleTextureRenderInternal(int32_t textureCode, double x1, double y1, double x2, double y2, double r, double g, double b, double rot, double xcenter, double ycenter, double xfact, double yfact) {
    rot += M_PI; // messed up somewhere and this is the simplest way to fix it (rotate 180)
    /* do the rotation math here - rotate on center */
    double avgX = (x1 + x2) / 2;
    double avgY = (y1 + y2) / 2;
    double sinRot = sin(rot);
    double cosRot = cos(rot);
    double x1Displace = x1 - avgX;
    double y1Displace = y1 - avgY;
    double x2Displace = x2 - avgX;
    double y2Displace = y2 - avgY;
    x1 = avgX + x1Displace * sinRot - y1Displace * cosRot;
    y1 = avgY + x1Displace * cosRot + y1Displace * sinRot;
    x2 = avgX + x2Displace * sinRot - y2Displace * cosRot;
    y2 = avgY + x2Displace * cosRot + y2Displace * sinRot;
    double x3 = avgX + x2Displace * sinRot - y1Displace * cosRot;
    double y3 = avgY + x2Displace * cosRot + y1Displace * sinRot;
    double x4 = avgX + x1Displace * sinRot - y2Displace * cosRot;
    double y4 = avgY + x1Displace * cosRot + y2Displace * sinRot;
    addVertex(x1 * xfact + xcenter, y1 * yfact + ycenter, r, g, b, 1.0, 0, 0, textureCode);
    addVertex(x3 * xfact + xcenter, y3 * yfact + ycenter, r, g, b, 1.0, 0, 1, textureCode);
    addVertex(x2 * xfact + xcenter, y2 * yfact + ycenter, r, g, b, 1.0, 1, 1, textureCode);
    addVertex(x1 * xfact + xcenter, y1 * yfact + ycenter, r, g, b, 1.0, 0, 0, textureCode);
    addVertex(x4 * xfact + xcenter, y4 * yfact + ycenter, r, g, b, 1.0, 1, 0, textureCode);
    addVertex(x2 * xfact + xcenter, y2 * yfact + ycenter, r, g, b, 1.0, 1, 1, textureCode);
}

turtle_texture_t turtleTextureLoad(char *filename) {
    /* determine encoding */
    /* load image */
    int width;
    int height;
    int nbChannels;
    unsigned char *image = stbi_load(filename, &width, &height, &nbChannels, 0);
    if (image == NULL) {
        printf("turtleTextureLoad: Could not load image %s\n", filename);
        turtle_texture_t output;
        output.id = -1;
        return output;
    }
    uint32_t encoding = GL_RGB;
    uint32_t stb_encoding = STBIR_RGB;
    uint8_t stride = 3;
    if (nbChannels == 4) {
        encoding = GL_RGBA;
        stb_encoding = STBIR_RGBA;
        stride = 4;
    }
    unsigned char *resized = malloc(stride * turtle.textureWidth * turtle.textureHeight);
    stbir_resize_uint8_linear(image, width, height, stride * width, resized, turtle.textureWidth, turtle.textureHeight, stride * turtle.textureWidth, stb_encoding);
    free(image);
    if (resized == NULL) {
        printf("turtleTextureLoad: Could not resize image %s\n", filename);
        turtle_texture_t output;
        output.id = -1;
    }
    /* find first available texture */
    turtle_texture_t texture;
    texture.id = -1;
    for (int32_t i = 0; i < turtle.textureList -> length; i++) {
        if (strcmp(turtle.textureList -> data[i].s, "") == 0) {
            texture.id = i;
            free(turtle.textureList -> data[texture.id].s);
            turtle.textureList -> data[texture.id].s = strdup(filename);
            break;
        }
    }
    if (texture.id == -1) {
        texture.id = turtle.textureList -> length;
        list_append(turtle.textureList, (unitype) filename, 's');
    }
    texture.width = width;
    texture.height = height;
    /* load to GPU */
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, texture.id, turtle.textureWidth, turtle.textureHeight, 1, encoding, GL_UNSIGNED_BYTE, resized);
    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
    free(resized);
    return texture;
}

int32_t turtleTextureUnload(turtle_texture_t texture) {
    /* update list */
    if (texture.id >= turtle.textureList -> length || texture.id < 0) {
        return -1;
    }
    free(turtle.textureList -> data[texture.id].s);
    turtle.textureList -> data[texture.id].s = strdup("");
    /* remove from GPU */
    return 0;
}

void turtleTexture(turtle_texture_t texture, double x1, double y1, double x2, double y2, double rot, double r, double g, double b) {
    list_append(turtle.penPos, (unitype) x1, 'd');
    list_append(turtle.penPos, (unitype) y1, 'd');
    list_append(turtle.penPos, (unitype) x2, 'd');
    list_append(turtle.penPos, (unitype) y2, 'd');
    list_append(turtle.penPos, (unitype) rot, 'd'); // rotation (degrees, bearing)
    list_append(turtle.penPos, (unitype) (r / 255), 'd');
    list_append(turtle.penPos, (unitype) (g / 255), 'd');
    list_append(turtle.penPos, (unitype) (128 + texture.id), 'h'); // blit texture signifier + texture code - limited to 32639 textures
    list_append(turtle.penPos, (unitype) (b / 255), 'd');
}

void turtlePrintTexture(turtle_texture_t texture) {
    printf("Texture ID: %d\n", texture.id);
    if (texture.id >= 0 && texture.id < turtle.textureList -> length) {
        printf("- Texture Name: %s\n", turtle.textureList -> data[texture.id].s);
    } else {
        printf("- Texture Name: NULL\n");
    }
    printf("- Texture Width: %d\n", texture.width);
    printf("- Texture Height: %d\n", texture.height);
}
#endif /* TURTLE_ENABLE_TEXTURES */

/* adds a (blit) triangle to the pipeline (for better speed) */
void turtleTriangle(double x1, double y1, double x2, double y2, double x3, double y3) {
    list_append(turtle.penPos, (unitype) x1, 'd');
    list_append(turtle.penPos, (unitype) y1, 'd');
    list_append(turtle.penPos, (unitype) x2, 'd');
    list_append(turtle.penPos, (unitype) turtle.penr, 'd');
    list_append(turtle.penPos, (unitype) turtle.peng, 'd');
    list_append(turtle.penPos, (unitype) turtle.penb, 'd');
    list_append(turtle.penPos, (unitype) turtle.pena, 'd');
    list_append(turtle.penPos, (unitype) 66, 'h'); // blit triangle signifier
    list_append(turtle.penPos, (unitype) y2, 'd'); // some unconventional formatting but it works

    list_append(turtle.penPos, (unitype) x3, 'd');
    list_append(turtle.penPos, (unitype) y3, 'd');
    list_append(turtle.penPos, (unitype) 0, 'd'); // zero'd out (wasted space)
    list_append(turtle.penPos, (unitype) turtle.penr, 'd'); // duplicate colour data (wasted space)
    list_append(turtle.penPos, (unitype) turtle.peng, 'd');
    list_append(turtle.penPos, (unitype) turtle.penb, 'd');
    list_append(turtle.penPos, (unitype) turtle.pena, 'd');
    list_append(turtle.penPos, (unitype) 66, 'h'); // blit triangle signifier
    list_append(turtle.penPos, (unitype) 0, 'd'); // zero'd out (wasted space)
}

void turtleTriangleColor(double x1, double y1, double x2, double y2, double x3, double y3, double r, double g, double b, double a) {
    list_append(turtle.penPos, (unitype) x1, 'd');
    list_append(turtle.penPos, (unitype) y1, 'd');
    list_append(turtle.penPos, (unitype) x2, 'd');
    list_append(turtle.penPos, (unitype) (r / 255), 'd');
    list_append(turtle.penPos, (unitype) (g / 255), 'd');
    list_append(turtle.penPos, (unitype) (b / 255), 'd');
    list_append(turtle.penPos, (unitype) (a / 255), 'd');
    list_append(turtle.penPos, (unitype) 66, 'h'); // blit triangle signifier
    list_append(turtle.penPos, (unitype) y2, 'd'); // some unconventional formatting but it works

    list_append(turtle.penPos, (unitype) x3, 'd');
    list_append(turtle.penPos, (unitype) y3, 'd');
    list_append(turtle.penPos, (unitype) 0, 'd'); // zero'd out (wasted space)
    list_append(turtle.penPos, (unitype) r, 'd'); // duplicate colour data (wasted space)
    list_append(turtle.penPos, (unitype) g, 'd');
    list_append(turtle.penPos, (unitype) b, 'd');
    list_append(turtle.penPos, (unitype) a, 'd');
    list_append(turtle.penPos, (unitype) 66, 'h'); // blit triangle signifier
    list_append(turtle.penPos, (unitype) 0, 'd'); // zero'd out (wasted space)
}

/* adds a (blit) quad to the pipeline (for better speed) */
void turtleQuad(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4) {
    list_append(turtle.penPos, (unitype) x1, 'd');
    list_append(turtle.penPos, (unitype) y1, 'd');
    list_append(turtle.penPos, (unitype) x2, 'd');
    list_append(turtle.penPos, (unitype) turtle.penr, 'd');
    list_append(turtle.penPos, (unitype) turtle.peng, 'd');
    list_append(turtle.penPos, (unitype) turtle.penb, 'd');
    list_append(turtle.penPos, (unitype) turtle.pena, 'd');
    list_append(turtle.penPos, (unitype) 67, 'h'); // blit quad signifier
    list_append(turtle.penPos, (unitype) y2, 'd'); // some unconventional formatting but it works

    list_append(turtle.penPos, (unitype) x3, 'd');
    list_append(turtle.penPos, (unitype) y3, 'd');
    list_append(turtle.penPos, (unitype) x4, 'd');
    list_append(turtle.penPos, (unitype) turtle.penr, 'd'); // duplicate colour data (wasted space)
    list_append(turtle.penPos, (unitype) turtle.peng, 'd');
    list_append(turtle.penPos, (unitype) turtle.penb, 'd');
    list_append(turtle.penPos, (unitype) turtle.pena, 'd');
    list_append(turtle.penPos, (unitype) 67, 'h'); // blit quad signifier
    list_append(turtle.penPos, (unitype) y4, 'd');
}

void turtleQuadColor(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double r, double g, double b, double a) {
    list_append(turtle.penPos, (unitype) x1, 'd');
    list_append(turtle.penPos, (unitype) y1, 'd');
    list_append(turtle.penPos, (unitype) x2, 'd');
    list_append(turtle.penPos, (unitype) (r / 255), 'd');
    list_append(turtle.penPos, (unitype) (g / 255), 'd');
    list_append(turtle.penPos, (unitype) (b / 255), 'd');
    list_append(turtle.penPos, (unitype) (a / 255), 'd');
    list_append(turtle.penPos, (unitype) 67, 'h'); // blit quad signifier
    list_append(turtle.penPos, (unitype) y2, 'd'); // some unconventional formatting but it works

    list_append(turtle.penPos, (unitype) x3, 'd');
    list_append(turtle.penPos, (unitype) y3, 'd');
    list_append(turtle.penPos, (unitype) x4, 'd');
    list_append(turtle.penPos, (unitype) r, 'd'); // duplicate colour data (wasted space)
    list_append(turtle.penPos, (unitype) g, 'd');
    list_append(turtle.penPos, (unitype) b, 'd');
    list_append(turtle.penPos, (unitype) a, 'd');
    list_append(turtle.penPos, (unitype) 67, 'h'); // blit quad signifier
    list_append(turtle.penPos, (unitype) y4, 'd');
}

/* adds a (blit) rectangle to the pipeline (uses quad interface) */
void turtleRectangle(double x1, double y1, double x2, double y2) {
    list_append(turtle.penPos, (unitype) x1, 'd');
    list_append(turtle.penPos, (unitype) y1, 'd');
    list_append(turtle.penPos, (unitype) x2, 'd');
    list_append(turtle.penPos, (unitype) turtle.penr, 'd');
    list_append(turtle.penPos, (unitype) turtle.peng, 'd');
    list_append(turtle.penPos, (unitype) turtle.penb, 'd');
    list_append(turtle.penPos, (unitype) turtle.pena, 'd');
    list_append(turtle.penPos, (unitype) 67, 'h'); // blit quad signifier
    list_append(turtle.penPos, (unitype) y1, 'd'); // some unconventional formatting but it works

    list_append(turtle.penPos, (unitype) x2, 'd');
    list_append(turtle.penPos, (unitype) y2, 'd');
    list_append(turtle.penPos, (unitype) x1, 'd');
    list_append(turtle.penPos, (unitype) turtle.penr, 'd'); // duplicate colour data (wasted space)
    list_append(turtle.penPos, (unitype) turtle.peng, 'd');
    list_append(turtle.penPos, (unitype) turtle.penb, 'd');
    list_append(turtle.penPos, (unitype) turtle.pena, 'd');
    list_append(turtle.penPos, (unitype) 67, 'h'); // blit quad signifier
    list_append(turtle.penPos, (unitype) y2, 'd');
}

void turtleRectangleColor(double x1, double y1, double x2, double y2, double r, double g, double b, double a) {
    list_append(turtle.penPos, (unitype) x1, 'd');
    list_append(turtle.penPos, (unitype) y1, 'd');
    list_append(turtle.penPos, (unitype) x2, 'd');
    list_append(turtle.penPos, (unitype) (r / 255), 'd');
    list_append(turtle.penPos, (unitype) (g / 255), 'd');
    list_append(turtle.penPos, (unitype) (b / 255), 'd');
    list_append(turtle.penPos, (unitype) (a / 255), 'd');
    list_append(turtle.penPos, (unitype) 67, 'h'); // blit quad signifier
    list_append(turtle.penPos, (unitype) y1, 'd'); // some unconventional formatting but it works

    list_append(turtle.penPos, (unitype) x2, 'd');
    list_append(turtle.penPos, (unitype) y2, 'd');
    list_append(turtle.penPos, (unitype) x1, 'd');
    list_append(turtle.penPos, (unitype) r, 'd'); // duplicate colour data (wasted space)
    list_append(turtle.penPos, (unitype) g, 'd');
    list_append(turtle.penPos, (unitype) b, 'd');
    list_append(turtle.penPos, (unitype) a, 'd');
    list_append(turtle.penPos, (unitype) 67, 'h'); // blit quad signifier
    list_append(turtle.penPos, (unitype) y2, 'd');
}

/* adds a (blit) circle to the pipeline */
void turtleCircle(double x, double y, double radius) {
    list_append(turtle.penPos, (unitype) x, 'd');
    list_append(turtle.penPos, (unitype) y, 'd');
    list_append(turtle.penPos, (unitype) radius, 'd');
    list_append(turtle.penPos, (unitype) turtle.penr, 'd');
    list_append(turtle.penPos, (unitype) turtle.peng, 'd');
    list_append(turtle.penPos, (unitype) turtle.penb, 'd');
    list_append(turtle.penPos, (unitype) turtle.pena, 'd');
    list_append(turtle.penPos, (unitype) 64, 'h'); // blit circle signifier
    list_append(turtle.penPos, (unitype) turtle.circleprez, 'd');
}

/* create a triangle in 3D */
void turtle3DTriangle(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3) {
    list_append(turtle.penPos, (unitype) x1, 'd');
    list_append(turtle.penPos, (unitype) y1, 'd');
    list_append(turtle.penPos, (unitype) z1, 'd');
    list_append(turtle.penPos, (unitype) turtle.penr, 'd');
    list_append(turtle.penPos, (unitype) turtle.peng, 'd');
    list_append(turtle.penPos, (unitype) turtle.penb, 'd');
    list_append(turtle.penPos, (unitype) turtle.pena, 'd');
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
void turtlePerspective(double x, double y, double z, double *xOut, double *yOut) {
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
void turtleUpdate() {
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
    uint32_t len = turtle.penPos -> length;
    unitype *ren = turtle.penPos -> data;
    int8_t *renType = turtle.penPos -> type;
    uint64_t oldHash = turtle.penHash;
    turtle.penHash = 0; // I don't use this but it's an idea: https://stackoverflow.com/questions/57455444/very-low-collision-non-cryptographic-hashing-function
    for (uint32_t i = 0; i < len; i++) {
        turtle.penHash += turtle.penPos -> data[i].l; // simple addition hash. I know not technically safe since i cast all sizes to 8 byte, but it should still work
    }
    if (len != turtle.lastLength || oldHash != turtle.penHash) {
        changed = 1;
        turtle.lastLength = len;
    }
    glfwGetWindowSize(turtle.window, &turtle.screenbounds[0], &turtle.screenbounds[1]);
    if (turtle.screenbounds[0] != turtle.lastscreenbounds[0] || turtle.screenbounds[1] != turtle.lastscreenbounds[1]) {
        changed = 1;
        turtle.lastscreenbounds[0] = turtle.screenbounds[0];
        turtle.lastscreenbounds[1] = turtle.screenbounds[1];
    }
    turtle.bounds[0] = turtle.centerAndScale[0] - turtle.centerAndScale[2] / turtle.screenbounds[0];
    turtle.bounds[2] = turtle.centerAndScale[0] + turtle.centerAndScale[2] / turtle.screenbounds[0];
    turtle.bounds[1] = turtle.centerAndScale[1] - turtle.centerAndScale[3] / turtle.screenbounds[1];
    turtle.bounds[3] = turtle.centerAndScale[1] + turtle.centerAndScale[3] / turtle.screenbounds[1];
    if (changed) {
        #ifdef TURTLE_ENABLE_TEXTURES
        turtle.bufferList -> length = 0; // don't bother freeing the memory
        #endif /* TURTLE_ENABLE_TEXTURES */
        double xfact = 1.0 / ((turtle.bounds[2] - turtle.bounds[0]) / 2);
        double yfact = 1.0 / ((turtle.bounds[3] - turtle.bounds[1]) / 2);
        double xcenter = (double) turtle.screenbounds[0] / turtle.initscreenbounds[0] - 1;
        double ycenter = (double) turtle.screenbounds[1] / turtle.initscreenbounds[1] - 1;
        double lastSize = -1;
        double lastPrez = -1;
        double precomputedLog = 5;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for (int32_t i = 0; i < (int32_t) len; i += 9) {
            if (renType[i] == 'd') {
                switch (ren[i + 7].h) {
                case 0:
                    if (!(lastSize == ren[i + 2].d) || !(lastPrez != ren[i + 8].d)) {
                        precomputedLog = ren[i + 8].d * log(2.71 + ren[i + 2].d);
                    }
                    lastSize = ren[i + 2].d;
                    lastPrez = ren[i + 8].d;
                    turtleCircleRenderInternal(ren[i].d, ren[i + 1].d, ren[i + 2].d, ren[i + 3].d, ren[i + 4].d, ren[i + 5].d, ren[i + 6].d, xcenter, ycenter, xfact, yfact, precomputedLog);
                break;
                case 1:
                    turtleSquareRenderInternal(ren[i].d - ren[i + 2].d, ren[i + 1].d - ren[i + 2].d, ren[i].d + ren[i + 2].d, ren[i + 1].d + ren[i + 2].d, ren[i + 3].d, ren[i + 4].d, ren[i + 5].d, ren[i + 6].d, xcenter, ycenter, xfact, yfact);
                break;
                case 2:
                    turtleTriangleRenderInternal(ren[i].d - ren[i + 2].d, ren[i + 1].d - ren[i + 2].d, ren[i].d + ren[i + 2].d, ren[i + 1].d - ren[i + 2].d, ren[i].d, ren[i + 1].d + ren[i + 2].d, ren[i + 3].d, ren[i + 4].d, ren[i + 5].d, ren[i + 6].d, xcenter, ycenter, xfact, yfact);
                break;
                case 5:
                    if (i - 9 < 0 || renType[i - 9] == 'c') {
                        if (!(lastSize == ren[i + 2].d) || !(lastPrez != ren[i + 8].d)) {
                            precomputedLog = ren[i + 8].d * log(2.71 + ren[i + 2].d);
                        }
                        lastSize = ren[i + 2].d;
                        lastPrez = ren[i + 8].d;
                        turtleCircleRenderInternal(ren[i].d, ren[i + 1].d, ren[i + 2].d, ren[i + 3].d, ren[i + 4].d, ren[i + 5].d, ren[i + 6].d, xcenter, ycenter, xfact, yfact, precomputedLog);
                    }
                break;
                default:
                break;
                }
                if (i + 18 < (int32_t) len && renType[i + 9] == 'd' && ren[i + 7].h < 64 && (ren[i + 7].h == 4 || ren[i + 7].h == 5 || (fabs(ren[i].d - ren[i + 9].d) > ren[i + 2].d / 2 || fabs(ren[i + 1].d - ren[i + 10].d) > ren[i + 2].d / 2))) { // tests for next point continuity and also ensures that the next point is at sufficiently different coordinates
                    double dir = atan((ren[i + 9].d - ren[i].d) / (ren[i + 1].d - ren[i + 10].d));
                    double sinn = sin(dir + M_PI / 2);
                    double coss = cos(dir + M_PI / 2);
                    turtleQuadRenderInternal(ren[i].d + ren[i + 2].d * sinn, ren[i + 1].d - ren[i + 2].d * coss, ren[i + 9].d + ren[i + 2].d * sinn, ren[i + 10].d - ren[i + 2].d * coss, ren[i + 9].d - ren[i + 2].d * sinn, ren[i + 10].d + ren[i + 2].d * coss, ren[i].d - ren[i + 2].d * sinn, ren[i + 1].d + ren[i + 2].d * coss, ren[i + 3].d, ren[i + 4].d, ren[i + 5].d, ren[i + 6].d, xcenter, ycenter, xfact, yfact);
                    if ((ren[i + 7].h == 4 || ren[i + 7].h == 5) && i + 18 < (int32_t) len && renType[i + 18] == 'd') {
                        double dir2 = atan((ren[i + 18].d - ren[i + 9].d) / (ren[i + 10].d - ren[i + 19].d));
                        double sinn2 = sin(dir2 + M_PI / 2);
                        double coss2 = cos(dir2 + M_PI / 2);
                        turtleTriangleRenderInternal(ren[i + 9].d + ren[i + 2].d * sinn, ren[i + 10].d - ren[i + 2].d * coss, ren[i + 9].d - ren[i + 2].d * sinn, ren[i + 10].d + ren[i + 2].d * coss, ren[i + 9].d + ren[i + 11].d * sinn2, ren[i + 10].d - ren[i + 11].d * coss2, ren[i + 3].d, ren[i + 4].d, ren[i + 5].d, ren[i + 6].d, xcenter, ycenter, xfact, yfact); // in a perfect world the program would know which one of these triangles to render (to blend the segments)
                        turtleTriangleRenderInternal(ren[i + 9].d + ren[i + 2].d * sinn, ren[i + 10].d - ren[i + 2].d * coss, ren[i + 9].d - ren[i + 2].d * sinn, ren[i + 10].d + ren[i + 2].d * coss, ren[i + 9].d - ren[i + 11].d * sinn2, ren[i + 10].d + ren[i + 11].d * coss2, ren[i + 3].d, ren[i + 4].d, ren[i + 5].d, ren[i + 6].d, xcenter, ycenter, xfact, yfact); // however we live in a world where i am bad at math, so it just renders both no matter what (one has no effect)
                    }
                } else {
                    if (ren[i + 7].h == 4 && i > 8 && renType[i - 8] == 'c') {
                        if (!(lastSize == ren[i + 2].d) || !(lastPrez != ren[i + 8].d)) {
                            precomputedLog = ren[i + 8].d * log(2.71 + ren[i + 2].d);
                        }
                        lastSize = ren[i + 2].d;
                        lastPrez = ren[i + 8].d;
                        turtleCircleRenderInternal(ren[i].d, ren[i + 1].d, ren[i + 2].d, ren[i + 3].d, ren[i + 4].d, ren[i + 5].d, ren[i + 6].d, xcenter, ycenter, xfact, yfact, precomputedLog);
                    }
                    if (ren[i + 7].h == 5 && i > 8) {
                        if (!(lastSize == ren[i + 2].d) || !(lastPrez != ren[i + 8].d)) {
                            precomputedLog = ren[i + 8].d * log(2.71 + ren[i + 2].d);
                        }
                        lastSize = ren[i + 2].d;
                        lastPrez = ren[i + 8].d;
                        turtleCircleRenderInternal(ren[i].d, ren[i + 1].d, ren[i + 2].d, ren[i + 3].d, ren[i + 4].d, ren[i + 5].d, ren[i + 6].d, xcenter, ycenter, xfact, yfact, precomputedLog);
                    }
                }
                if (ren[i + 7].h == 64) { // blit circle
                    if (!(lastSize == ren[i + 2].d) || !(lastPrez != ren[i + 8].d)) {
                        precomputedLog = ren[i + 8].d * log(2.71 + ren[i + 2].d);
                    }
                    lastSize = ren[i + 2].d;
                    lastPrez = ren[i + 8].d;
                    turtleCircleRenderInternal(ren[i].d, ren[i + 1].d, ren[i + 2].d, ren[i + 3].d, ren[i + 4].d, ren[i + 5].d, ren[i + 6].d, xcenter, ycenter, xfact, yfact, precomputedLog);
                }
                if (ren[i + 7].h == 66) { // blit triangle
                    turtleTriangleRenderInternal(ren[i].d, ren[i + 1].d, ren[i + 2].d, ren[i + 8].d, ren[i + 9].d, ren[i + 10].d, ren[i + 3].d, ren[i + 4].d, ren[i + 5].d, ren[i + 6].d, xcenter, ycenter, xfact, yfact);
                    i += 9;
                }
                if (ren[i + 7].h == 67) { // blit quad
                    turtleQuadRenderInternal(ren[i].d, ren[i + 1].d, ren[i + 2].d, ren[i + 8].d, ren[i + 9].d, ren[i + 10].d, ren[i + 11].d, ren[i + 17].d, ren[i + 3].d, ren[i + 4].d, ren[i + 5].d, ren[i + 6].d, xcenter, ycenter, xfact, yfact);
                    i += 9;
                }
                #ifdef TURTLE_ENABLE_TEXTURES
                if (ren[i + 7].h >= 128) { // blit texture (rectangle)
                    turtleTextureRenderInternal(ren[i + 7].h - 128, ren[i].d, ren[i + 1].d, ren[i + 2].d, ren[i + 3].d, ren[i + 5].d, ren[i + 6].d, ren[i + 8].d, ren[i + 4].d / 57.2958, xcenter, ycenter, xfact, yfact);
                }
                #endif /* TURTLE_ENABLE_TEXTURES */
                // if (ren[i + 7].h == 256) { // blit 3D sphere

                // }
                // if (ren[i + 7].h == 257) { // blit 3D circle

                // }
                // if (ren[i + 7].h == 258) { // blit 3D triangle
                //     turtlePerspective(ren[i].d, ren[i + 1].d, ren[i + 2].d, &ren[i].d, &ren[i + 1].d);
                //     turtlePerspective(ren[i + 8].d, ren[i + 9].d, ren[i + 10].d, &ren[i + 8].d, &ren[i + 9].d);
                //     turtlePerspective(ren[i + 11].d, ren[i + 12].d, ren[i + 13].d, &ren[i + 11].d, &ren[i + 12].d);
                //     turtleTriangleRenderInternal(ren[i].d, ren[i + 1].d, ren[i + 8].d, ren[i + 9].d, ren[i + 11].d, ren[i + 12].d, ren[i + 3].d, ren[i + 4].d, ren[i + 5].d, ren[i + 6].d, xcenter, ycenter, xfact, yfact);
                //     i += 9;
                // }
                // if (ren[i + 7].h == 259) { // blit 3D quad

                // }
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
}

/* keeps the window open while doing nothing else (from python turtleMainLoop()) */
void turtleMainLoop() {
    while (turtle.close == 0) {
        turtleUpdate();
    }
}

/* free turtle memory */
void turtleFree() {
    list_free(turtle.keyPressed);
    list_free(turtle.penPos);
    #ifdef TURTLE_ENABLE_TEXTURES
    bufferList_free(turtle.bufferList);
    #endif /* TURTLE_ENABLE_TEXTURES */
}
