#ifndef TURTLETEXT
#include "turtleText.h"
#endif

typedef struct { // slider "class"
    double *dbind; // for double binds
    int *ibind; // for int binds
    float *fbind; // for float binds
    short *hbind; // for short binds
    char *cbind; // for char binds
    const char* displayName; // name
    double x; // position
    double y;
    double width;
    double length;
    double min;
    double max;
    char style; // 0 = horizontal, 1 = vertical, 2 = dial
    char bindType;
    char hover; // is the user's mouse hovering over the slider?
    char mouseDown; // is the user dragging the slider?
} slider_t;

void sliderInit(slider_t *selfp, unitype *bind, char bindType, const char *name, const char* style, double x, double y, double width, double length, double min, double max) {
    slider_t self = *selfp;
    self.bindType = bindType;
    switch(self.bindType) {
        case 'i':
        self.ibind = (int*) bind;
        break;
        case 'h':
        self.hbind = (short*) bind;
        break;
        case 'c':
        self.cbind = (char*) bind;
        break;
        case 'f':
        self.fbind = (float*) bind;
        break;
        case 'd':
        self.dbind = (double*) bind;
        break;
        default:
        printf("sliderInit: unrecognised bindType\n");
        return;
    }
    self.displayName = strdup(name);
    self.x = x;
    self.y = y;
    self.width = width;
    self.length = length;
    self.min = min;
    self.max = max;
    self.hover = 0;
    self.mouseDown = 0;
    self.style = 2; // default (dial)
    if (strcmp(style, "h") == 0 || strcmp(style, "horizontal") == 0) {
        self.style = 0;
    }
    if (strcmp(style, "v") == 0 || strcmp(style, "vertical") == 0) {
        self.style = 1;
    }
    if (strcmp(style, "d") == 0 || strcmp(style, "dial") == 0 || strcmp(style, "c") == 0 || strcmp(style, "circle") == 0) {
        self.style = 2;
    }
    *selfp = self;
}

void sliderControl(slider_t *selfp) { // assumes turtle.mouseX and turtle.mouseY have been set
    slider_t self = *selfp;
    if (self.hover == 1 && turtleMouseDown()) {
        self.mouseDown = 1;
    } else {
        if (!turtleMouseDown()) {
            self.mouseDown = 0;
        }
    }
    if (turtle.mouseX > self.x - 5 && turtle.mouseX < self.x + 5 && turtle.mouseY > self.y - self.length / 2 - 5 && turtle.mouseY < self.y + self.length / 2 - self.length * 0.1 + 5 && !turtleMouseDown()) {
        self.hover = 1;
    } else {
        self.hover = 0;
    }
    if (self.mouseDown == 1) {
        double perc = (turtle.mouseY - (self.y - self.length / 2)) / ((self.y + self.length / 2 - self.length * 0.1) - (self.y - self.length / 2));
        if (perc > 1) {
            perc = 1;
        }
        if (perc < 0) {
            perc = 0;
        }
        if (self.bindType == 'i') {
            *self.ibind = ((int) (self.min + perc * (self.max - self.min)));
        }
        if (self.bindType == 'd') {
            *self.dbind = (self.min + perc * (self.max - self.min));
        }
        
    }
    *selfp = self;
}

void sliderRender(slider_t *selfp) {
    slider_t self = *selfp;
    double bound;
    switch(self.bindType) {
        case 'i':
        bound = (double) *self.ibind;
        break;
        case 'h':
        bound = (double) *self.hbind;
        break;
        case 'c':
        bound = (double) *self.cbind;
        break;
        case 'f':
        bound = (double) *self.fbind;
        break;
        case 'd':
        bound = *self.dbind;
        break;
        default:
        bound = 0;
        break;
    }
    turtlePenColor(0, 0, 0);
    char text[128];
    char value[128];
    sprintf(value, "%.0lf", bound);
    strcpy(text, self.displayName);
    strcat(text, ": ");
    strcat(text, value);
    turtleTextWriteString(text, self.x, self.y + self.length / 2 - self.length * 0.04, 10, 50);
    turtlePenShape("circle");

    turtleGoto(self.x + self.width / 2, self.y + self.length / 2);
    // turtlePenDown();
    turtleGoto(self.x - self.width / 2, self.y + self.length / 2);
    turtleGoto(self.x - self.width / 2, self.y - self.length / 2);
    turtleGoto(self.x + self.width / 2, self.y - self.length / 2);
    turtleGoto(self.x + self.width / 2, self.y + self.length / 2);
    turtlePenUp();

    turtleGoto(self.x, self.y + self.length / 2 - self.length * 0.1);
    turtlePenSize(5);
    turtlePenColor(160, 160, 160);
    turtlePenDown();
    turtleGoto(self.x, self.y - self.length / 2);
    turtlePenUp();
    turtlePenColor(0, 0, 0);
    turtlePenSize(8);
    turtleGoto(self.x, self.y + self.length / 2 - self.length * 0.1 - self.length * 0.9 * (self.max - bound) / (self.max - self.min));
    turtlePenDown();
    turtlePenUp();
    sliderControl(&self);
    *selfp = self;
}