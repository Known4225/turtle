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

/* enabling this option will render the string "pppp" at a higher y than "PPPP" such that the center of their Y values will match */
// #define TURTLE_TEXT_DO_DYNAMIC_Y_CENTERING

/* enabling this option will make text rendering faster by culling the circles on ends of text - this might make the text look pretty bad */
// #define TURTLE_TEXT_FAST_PEN

/* enabling this option will add extra circles significantly decreasing the speed but increasing the render quality */
// #define TURTLE_TEXT_PRETTY_PEN

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
    /*
    Format:
    maximum x coordinate
    maximum y coordinate
    minimum x coordinate
    minimum y coordinate
    outer loops {
        inner loops {
            coordinates
            ...
        }
        inner loops {
            coordinates
            ...
        }
    }
    */
} turtleText_t;

extern turtleText_t turtleText;

/* initialise turtleText, must supply a font file (tgl) - if font file is not found then a default font will be substituted */
int32_t turtleTextInit(const char *filename);

/* render functions */

/* renders a quadratic bezier curve on the screen */
void renderBezier(double x1, double y1, double x2, double y2, double x3, double y3, int32_t prez);

/* renders a single character - INTERNAL */
void renderChar(int32_t index, double x, double y, double size);

/* special version of renderChar with rotation - INTERNAL */
void renderCharRotated(int32_t index, double x, double y, double size, double sinR, double cosR);

/* gets the length of a string in coordinates on the screen */
double turtleTextGetLength(const uint32_t *text, int32_t textLength, double size);

/* gets the length of a formatted string in coordinates on the screen */
double turtleTextGetStringLength(const char *str, double size);

/* gets the length of a string in coordinates on the screen */
double turtleTextGetStringLengthf(double size, const char *str, ...);

/* gets the length of a formatted utf8-string in coordinates on the screen */
double turtleTextGetUnicodeLength(const char *str, double size);

/* gets the length of a utf8-string in coordinates on the screen */
double turtleTextGetUnicodeLengthf(double size, const char *str, ...);

/* cut the text of a string such that it will fit in a coordinate size width (0 - left truncate, 1 - right truncate) */
void turtleTextTruncateString(char *str, double size, double width, int8_t leftRight);

/* Writes to the screen - INTERNAL */
void turtleTextWrite(const uint32_t *text, int32_t textLength, double x, double y, double size, double align);

/* Special form of write function which supports rotated text - INTERNAL */
void turtleTextWriteRotated(const uint32_t *text, int32_t textLength, double x, double y, double size, double align, double rotate);

/* Write a string to the screen */
void turtleTextWriteString(const char *str, double x, double y, double size, double align);

/* Write a formatted string to the screen */
void turtleTextWriteStringf(double x, double y, double size, double align, const char *str, ...);

/* Write a string to the screen (with rotation) */
void turtleTextWriteStringRotated(const char *str, double x, double y, double size, double align, double rotate);

/* Write a formatted string to the screen (with rotation) */
void turtleTextWriteStringfRotated(double x, double y, double size, double align, double rotate, const char *str, ...);

/* Write a utf8-string to the screen */
void turtleTextWriteUnicode(const char *str, double x, double y, double size, double align);

/* Write a formatted utf8-string to the screen */
void turtleTextWriteUnicodef(double x, double y, double size, double align, const char *str, ...);

/* Write a utf8-string to the screen (with rotation) */
void turtleTextWriteUnicodeRotated(const char *str, double x, double y, double size, double align, double rotate);

/* Write a formatted utf8-string to the screen (with rotation) */
void turtleTextWriteUnicodefRotated(double x, double y, double size, double align, double rotate, const char *str, ...);

/* internal function for converting utf8 to uint32_t characters */
int32_t turtleTextConvertUnicode(const char *str, uint32_t *converted);

/* if the font file is not found, use the default font (kept here) */
void generateDefaultFont(list_t *generatedFont);

#endif /* TURTLE_TEXT_H */
