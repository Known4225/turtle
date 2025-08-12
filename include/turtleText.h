/*
████████╗██╗   ██╗██████╗ ████████╗██╗     ███████╗████████╗███████╗██╗  ██╗████████╗██╗  ██╗
╚══██╔══╝██║   ██║██╔══██╗╚══██╔══╝██║     ██╔════╝╚══██╔══╝██╔════╝╚██╗██╔╝╚══██╔══╝██║  ██║
   ██║   ██║   ██║██████╔╝   ██║   ██║     █████╗     ██║   █████╗   ╚███╔╝    ██║   ███████║
   ██║   ██║   ██║██╔══██╗   ██║   ██║     ██╔══╝     ██║   ██╔══╝   ██╔██╗    ██║   ██╔══██║
   ██║   ╚██████╔╝██║  ██║   ██║   ███████╗███████╗   ██║   ███████╗██╔╝ ██╗   ██║██╗██║  ██║
   ╚═╝    ╚═════╝ ╚═╝  ╚═╝   ╚═╝   ╚══════╝╚══════╝   ╚═╝   ╚══════╝╚═╝  ╚═╝   ╚═╝╚═╝╚═╝  ╚═╝
https://patorjk.com/software/taag/#p=display&f=ANSI%20Shadow
*/

#ifndef TURTLE_TEXT_H
#define TURTLE_TEXT_H

#include "turtle.h"
#include <stdarg.h>

/* turtleText variables */
typedef struct {
    int32_t bezierPrez; // precision for bezier curves
    int32_t bezierPrezCurrent;
    int32_t charCount; // number of supported characters
    uint32_t *supportedCharReference; // array containing links from (int) unicode values of characters to an index from 0 to (charCount - 1)
    int32_t *fontPointer; // array containing links from char indices (0 to (charCount - 1)) to their corresponding data position in fontData
    int32_t *fontData; // array containing packaged instructions on how to draw each character in the character set
} turtleText_t;

extern turtleText_t turtleText;

/* initialise values, must supply a font file (tgl) */
int32_t turtleTextInit(const char *filename);

/* render functions */

/* renders a quadratic bezier curve on the screen */
void renderBezier(double x1, double y1, double x2, double y2, double x3, double y3, int32_t prez);

/* renders a single character */
void renderChar(int32_t index, double x, double y, double size);

/* gets the length of a string in pixels on the screen */
double turtleTextGetLength(const uint32_t *text, int32_t textLength, double size);

/* gets the length of a string in pixels on the screen */
double turtleTextGetStringLength(const char *str, double size);

/* gets the length of a string in pixels on the screen */
double turtleTextGetStringLengthf(double size, const char *str, ...);

/* gets the length of a u-string in pixels on the screen */
double turtleTextGetUnicodeLength(const unsigned char *str, double size);

/* writes to the screen */
void turtleTextWrite(const uint32_t *text, int32_t textLength, double x, double y, double size, double align);

/* wrapper function for writing strings easier */
void turtleTextWriteString(const char *str, double x, double y, double size, double align);

/* formatted string */
void turtleTextWriteStringf(double x, double y, double size, double align, const char *str, ...);

/* wrapper function for unicode strings (UTF-8, u8"Hello World") */
void turtleTextWriteUnicode(const unsigned char *str, double x, double y, double size, double align);

int32_t turtleTextConvertUnicode(const unsigned char *str, uint32_t *converted);

/* if the font file is not found, use the default font (kept here) */
void generateDefaultFont(list_t *generatedFont);

#endif /* TURTLE_TEXT_H */
