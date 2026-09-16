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

/* turtle_text variables */
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
} turtle_text_t;

extern turtle_text_t turtleText;

/* initialise turtle_text, must supply a font file (tgl) - if font file is not found then a default font will be substituted */
int32_t turtle_text_init(const char *filename);

/* render functions */

/* renders a quadratic bezier curve on the screen */
void turtle_text_render_bezier(double x1, double y1, double x2, double y2, double x3, double y3, int32_t prez);

/* renders a single character - INTERNAL */
void turtle_text_render_char(int32_t index, double x, double y, double size);

/* special version of turtle_text_render_char with rotation - INTERNAL */
void turtle_text_render_char_rotated(int32_t index, double x, double y, double size, double sinR, double cosR);

/* gets the length of a string in coordinates on the screen */
double turtle_text_get_length(const uint32_t *text, int32_t textLength, double size);

/* gets the length of a formatted string in coordinates on the screen */
double turtle_text_get_string_length(const char *str, double size);

/* gets the length of a string in coordinates on the screen (max of 2048 characters) */
double turtle_text_get_string_lengthf(double size, const char *str, ...);

/* gets the length of a formatted utf8-string in coordinates on the screen */
double turtle_text_get_unicode_length(const char *str, double size);

/* gets the length of a utf8-string in coordinates on the screen (max of 2048 characters) */
double turtle_text_get_unicode_lengthf(double size, const char *str, ...);

/* cut the text of a string such that it will fit in a coordinate size width (0 - left truncate, 1 - right truncate) */
void turtle_text_truncate_string(char *str, double size, double width, int8_t leftRight);

/* Writes to the screen - INTERNAL */
void turtle_text_write(const uint32_t *text, int32_t textLength, double x, double y, double size, double align);

/* Special form of write function which supports rotated text - INTERNAL */
void turtle_text_write_rotated(const uint32_t *text, int32_t textLength, double x, double y, double size, double align, double rotate);

/* Write a string to the screen */
void turtle_text_write_string(const char *str, double x, double y, double size, double align);

/* Write a formatted string to the screen (max of 2048 characters) */
void turtle_text_write_stringf(double x, double y, double size, double align, const char *str, ...);

/* Write a string to the screen (with rotation) */
void turtle_text_write_string_rotated(const char *str, double x, double y, double size, double align, double rotate);

/* Write a formatted string to the screen (with rotation) (max of 2048 characters) */
void turtle_text_write_stringf_rotated(double x, double y, double size, double align, double rotate, const char *str, ...);

/* Write a utf8-string to the screen */
void turtle_text_write_unicode(const char *str, double x, double y, double size, double align);

/* Write a formatted utf8-string to the screen (max of 2048 characters) */
void turtle_text_write_unicodef(double x, double y, double size, double align, const char *str, ...);

/* Write a utf8-string to the screen (with rotation) */
void turtle_text_write_unicode_rotated(const char *str, double x, double y, double size, double align, double rotate);

/* Write a formatted utf8-string to the screen (with rotation) (max of 2048 characters) */
void turtle_text_write_unicodef_rotated(double x, double y, double size, double align, double rotate, const char *str, ...);

/* internal function for converting utf8 to uint32_t characters */
int32_t turtle_text_convert_unicode(const char *str, uint32_t *converted);

/* if the font file is not found, use the default font (kept here) */
void turtle_text_generate_default_font(list_t *generatedFont);

#endif /* TURTLE_TEXT_H */
