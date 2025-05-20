/* turtleText uses openGL and the turtle library to render text on the screen */

#ifndef TURTLETEXT
#define TURTLETEXT
#include "turtle.h"

/* turtleText variables */
typedef struct {
    int32_t bezierPrez; // precision for bezier curves
    int32_t charCount; // number of supported characters
    uint32_t *supportedCharReference; // array containing links from (int) unicode values of characters to an index from 0 to (charCount - 1)
    int32_t *fontPointer; // array containing links from char indices (0 to (charCount - 1)) to their corresponding data position in fontData
    int32_t *fontData; // array containing packaged instructions on how to draw each character in the character set
} turtleText;

turtleText turtleTextRender;

/* initialise values, must supply a font file (tgl) */
int32_t turtleTextInit(const char *filename) {
    turtlePenColor(0, 0, 0);
    turtleTextRender.bezierPrez = 10;

    /* load file */
    FILE *tgl = fopen(filename, "r");
    if (tgl == NULL) {
        printf("Error: could not open file %s\n", filename);
        return -1;
    }

    list_t *fontDataInit = list_init(); // these start as lists and become int arrays
    list_t *fontPointerInit = list_init();
    list_t *supportedCharReferenceInit = list_init();
    uint8_t parsedInt[12]; // max possible length of an int as a string is 11
    uint32_t fontPar;
    int32_t oldptr;
    int32_t strptr;

    char line[2048]; // maximum line length
    line[2047] = 0; // canary value
    turtleTextRender.charCount = 0;
    while (fgets(line, 2048, tgl) != NULL) { // fgets to prevent overflows
        if (line[2047] != 0) {
            printf("Error: line %d in file %s exceeded appropriate length\n", 0, filename);
            return -1;
        }

        oldptr = 0;
        strptr = 0;
        int32_t loops = 0;
        int32_t firstIndex = 0;
        
        while (line[strptr] != '\n' && line[strptr] != '\0') {
            while (line[strptr] != ',' && line[strptr] != '\n' && line[strptr] != '\0') {
                parsedInt[strptr - oldptr] = line[strptr];
                strptr += 1;
            }
            parsedInt[strptr - oldptr] = '\0';
            if (oldptr == 0) {
                fontPar = 0; // parse as unicode char (basically just take the multibyte utf-8 encoding of the character and literal cast it to an uint32_t (maximum of 4 bytes per character in utf-8 (?)))
                if (strlen((char *) parsedInt) > 4) {
                    printf("Error: character at line %d too long for uint32_t\n", supportedCharReferenceInit -> length + 1);
                }
                for (int32_t i = strlen((char *) parsedInt); i > 0; i--) {
                    int32_t abri = (strlen((char *) parsedInt) - i);
                    fontPar += (uint32_t) parsedInt[abri] << ((i - 1) * 8);
                }
                if (fontPar == 0) { // exception for the comma character
                    fontPar = 44;
                }
                list_append(supportedCharReferenceInit, (unitype) fontPar, 'i'); // adds as an int but will typecast back to unsigned later, this might not work correctly but it also doesn't really matter
                list_append(fontPointerInit, (unitype) fontDataInit -> length, 'i');
                firstIndex = fontDataInit -> length;
                list_append(fontDataInit, (unitype) 1, 'i');
            } else {
                sscanf((char *) parsedInt, "%u", &fontPar); // parse as integer
                if (strcmp((char *) parsedInt, "b") == 0) {
                    list_append(fontDataInit, (unitype) 140894115, 'i'); // all b's get converted to the integer 140894115 (chosen semi-randomly)
                } else {
                    list_append(fontDataInit, (unitype) fontPar, 'i'); // fontPar will double count when it encounters a b (idk why) so if there's a b we ignore the second fontPar (which is a duplicate of the previous one)
                }
                loops += 1;
            }
            if (line[strptr] != '\n' && line[strptr] != '\0')
                strptr += 2;
            oldptr = strptr;
        }
        fontDataInit -> data[firstIndex] = (unitype) loops;
        firstIndex += 1; // using firstIndex as iteration variable
        int32_t len1 = fontDataInit -> data[firstIndex].i;
        int32_t maximums[4] = {-2147483648, -2147483648, 2147483647, 2147483647}; // for describing bounding box of a character
        
        /* good programmng alert */
        #define CHECKS_EMB(ind) \
            if (fontDataInit -> data[ind].i > maximums[0]) {\
                maximums[0] = fontDataInit -> data[ind].i;\
            }\
            if (fontDataInit -> data[ind].i < maximums[3]) {\
                maximums[3] = fontDataInit -> data[ind].i;\
            }\
            if (fontDataInit -> data[ind + 1].i > maximums[1]) {\
                maximums[1] = fontDataInit -> data[ind + 1].i;\
            }\
            if (fontDataInit -> data[ind + 1].i < maximums[3]) {\
                maximums[3] = fontDataInit -> data[ind + 1].i;\
            }
        for (int32_t i = 0; i < len1; i++) {
            firstIndex += 1;
            int32_t len2 = fontDataInit -> data[firstIndex].i;
            for (int32_t j = 0; j < len2; j++) {
                firstIndex += 1;
                if (fontDataInit -> data[firstIndex].i == 140894115) {
                    firstIndex += 1;
                    fontDataInit -> data[firstIndex] = (unitype) (fontDataInit -> data[firstIndex].i + 160);
                    fontDataInit -> data[firstIndex + 1] = (unitype) (fontDataInit -> data[firstIndex + 1].i + 100);
                    CHECKS_EMB(firstIndex);
                    firstIndex += 2;
                    fontDataInit -> data[firstIndex] = (unitype) (fontDataInit -> data[firstIndex].i + 160);
                    fontDataInit -> data[firstIndex + 1] = (unitype) (fontDataInit -> data[firstIndex + 1].i + 100);
                    CHECKS_EMB(firstIndex);
                    firstIndex += 1;
                    if (fontDataInit -> data[firstIndex + 1].i != 140894115) {
                        firstIndex += 1;
                        fontDataInit -> data[firstIndex] = (unitype) (fontDataInit -> data[firstIndex].i + 160);
                        fontDataInit -> data[firstIndex + 1] = (unitype) (fontDataInit -> data[firstIndex + 1].i + 100);
                        CHECKS_EMB(firstIndex);
                        firstIndex += 1;
                    }
                } else {
                    fontDataInit -> data[firstIndex] = (unitype) (fontDataInit -> data[firstIndex].i + 160);
                    fontDataInit -> data[firstIndex + 1] = (unitype) (fontDataInit -> data[firstIndex + 1].i + 100);
                    CHECKS_EMB(firstIndex);
                    firstIndex += 1;
                }
            }
        }
        if (maximums[0] < 0) {
            list_append(fontDataInit, (unitype) 90, 'i');
        } else {
            list_append(fontDataInit, (unitype) maximums[0], 'i');
        }
        if (maximums[3] > 0) {
            list_append(fontDataInit, (unitype) 0, 'i');
        } else {
            list_append(fontDataInit, (unitype) maximums[3], 'i');
        }
        if (maximums[1] < 0) {
            if (turtleTextRender.charCount == 0)
                list_append(fontDataInit, (unitype) 0, 'i');
            else
                list_append(fontDataInit, (unitype) 120, 'i');
        } else {
            list_append(fontDataInit, (unitype) maximums[1], 'i');
        }
        if (maximums[2] > 0) {
            list_append(fontDataInit, (unitype) 0, 'i');
        } else {
            list_append(fontDataInit, (unitype) maximums[2], 'i');
        }
        turtleTextRender.charCount += 1;
    }
    list_append(fontPointerInit, (unitype) (int32_t) (fontDataInit -> length), 'i'); // last pointer
    turtleTextRender.fontData = malloc(sizeof(int32_t) * fontDataInit -> length); // convert lists to arrays (could be optimised cuz we already have the -> data arrays but who really cares this runs once)
    for (uint32_t i = 0; i < fontDataInit -> length; i++) {
        turtleTextRender.fontData[i] = fontDataInit -> data[i].i;
    }
    turtleTextRender.fontPointer = malloc(sizeof(int32_t) * fontPointerInit -> length);
    for (uint32_t i = 0; i < fontPointerInit -> length; i++) {
        turtleTextRender.fontPointer[i] = fontPointerInit -> data[i].i;
    }
    turtleTextRender.supportedCharReference = malloc(sizeof(int32_t) * supportedCharReferenceInit -> length);
    for (uint32_t i = 0; i < supportedCharReferenceInit -> length; i++) {
        turtleTextRender.supportedCharReference[i] = supportedCharReferenceInit -> data[i].i;
    }

    printf("%d characters loaded from %s\n", turtleTextRender.charCount, filename);

    list_free(fontDataInit);
    list_free(fontPointerInit);
    list_free(supportedCharReferenceInit);
    return 0;
}

/* render functions */

/* renders a quadratic bezier curve on the screen */
void renderBezier(double x1, double y1, double x2, double y2, double x3, double y3, int32_t prez) {
    turtleGoto(x1, y1);
    turtlePenDown();
    double iter1 = 1;
    double iter2 = 0;
    for (int32_t i = 0; i < prez; i++) {
        iter1 -= (double) 1 / prez;
        iter2 += (double) 1 / prez;
        double t1 = iter1 * iter1;
        double t2 = iter2 * iter2;
        double t3 = 2 * iter1 * iter2;
        turtleGoto(t1 * x1 + t3 * x2 + t2 * x3, t1 * y1 + t3 * y2 + t2 * y3);
    }
    turtleGoto(x3, y3);
}

/* renders a single character */
void renderChar(int32_t index, double x, double y, double size) {
    index += 1;
    int32_t len1 = turtleTextRender.fontData[index];
    for (int32_t i = 0; i < len1; i++) {
        index += 1;
        if (turtle.pen == 1)
            turtlePenUp();
        int32_t len2 = turtleTextRender.fontData[index];
        for (int32_t j = 0; j < len2; j++) {
            index += 1;
            if (turtleTextRender.fontData[index] == 140894115) { // 140894115 is the b value (reserved)
                index += 4;
                if (turtleTextRender.fontData[index + 1] != 140894115) {
                    renderBezier(x + turtleTextRender.fontData[index - 3] * size, y + turtleTextRender.fontData[index - 2] * size, x + turtleTextRender.fontData[index - 1] * size, y + turtleTextRender.fontData[index] * size, x + turtleTextRender.fontData[index + 1] * size, y + turtleTextRender.fontData[index + 2] * size, turtleTextRender.bezierPrez);
                    index += 2;
                } else {
                    renderBezier(x + turtleTextRender.fontData[index - 3] * size, y + turtleTextRender.fontData[index - 2] * size, x + turtleTextRender.fontData[index - 1] * size, y + turtleTextRender.fontData[index] * size, x + turtleTextRender.fontData[index + 2] * size, y + turtleTextRender.fontData[index + 3] * size, turtleTextRender.bezierPrez);
                }
            } else {
                index += 1;
                turtleGoto(x + turtleTextRender.fontData[index - 1] * size, y + turtleTextRender.fontData[index] * size);
            }
            turtlePenDown();
        }
    }
    turtlePenUp();
}

/* gets the length of a string in pixels on the screen */
double turtleTextGetLength(const uint32_t *text, int32_t textLength, double size) {
    size /= 175;
    double xTrack = 0;
    for (int32_t i = 0; i < textLength; i++) {
        int32_t currentDataAddress = 0;
        for (int32_t j = 0; j < turtleTextRender.charCount; j++) { // change to hashmap later
            if (turtleTextRender.supportedCharReference[j] == text[i]) {
                currentDataAddress = j;
                break;
            }
        }
        xTrack += (turtleTextRender.fontData[turtleTextRender.fontPointer[currentDataAddress + 1] - 4] + 40) * size;
    }
    xTrack -= 40 * size;
    return xTrack;
}

/* gets the length of a string in pixels on the screen */
double turtleTextGetStringLength(const char *str, double size) {
    uint32_t len = strlen(str);
    uint32_t converted[len];
    for (uint32_t i = 0; i < len; i++) {
        converted[i] = (uint32_t) str[i];
    }
    return turtleTextGetLength(converted, len, size);
}

/* gets the length of a u-string in pixels on the screen */
double turtleTextGetUnicodeLength(const unsigned char *str, double size) {
    int32_t len = strlen((char *) str);
    uint32_t converted[len]; // max number of characters in a utf-8 string of length len
    int32_t byteLength;
    int32_t i = 0;
    int32_t next = 0;
    while (i < len) {
        byteLength = 0;
        for (uint8_t j = 0; j < 8; j++) {
            unsigned char mask = 128 >> j;
            if (str[i] & mask) {
                byteLength += 1;
            } else {
                j = 8; // end loop
            }
        }
        if (byteLength == 0) { // case: ASCII character
            converted[next] = (uint32_t) str[i];
            byteLength = 1;
        } else { // case: multi-byte character
            uint32_t convert = 0;
            for (int32_t k = 0; k < byteLength; k++) {
                convert = convert << 8;
                convert += (uint32_t) str[i + k];
            }
            converted[next] = convert;
        }
        i += byteLength;
        next += 1;
    }
    return turtleTextGetLength(converted, len, size);
}

/* writes to the screen */
void turtleTextWrite(const uint32_t *text, int32_t textLength, double x, double y, double size, double align) {
    char saveShape = turtle.penshape;
    double saveSize = turtle.pensize;
    turtleTextRender.bezierPrez = (int32_t) ceil(sqrt(size * 1)); // change the 1 for higher or lower bezier precision
    double xTrack = x;
    size /= 175;
    y -= size * 70;
    turtlePenSize(20 * size);
    // turtlePenShape("connected"); // fast
    // turtlePenShape("circle"); // pretty
    turtlePenShape("text"); // dedicated setting that blends circle and connected
    list_t *xvals = list_init();
    list_t *dataIndStored = list_init();
    for (int32_t i = 0; i < textLength; i++) {
        int32_t currentDataAddress = 0;
        for (int32_t j = 0; j < turtleTextRender.charCount; j++) { // change to hashmap later
            if (turtleTextRender.supportedCharReference[j] == text[i]) {
                currentDataAddress = j;
                break;
            }
        }
        list_append(xvals, (unitype) xTrack, 'd');
        list_append(dataIndStored, (unitype) currentDataAddress, 'i');
        xTrack += (turtleTextRender.fontData[turtleTextRender.fontPointer[currentDataAddress + 1] - 4] + 40) * size;
    }
    xTrack -= 40 * size;
    for (int32_t i = 0; i < textLength; i++) {
        renderChar((double) turtleTextRender.fontPointer[dataIndStored -> data[i].i], xvals -> data[i].d - ((xTrack - x) * (align / 100)), y, size);
    }
    list_free(dataIndStored);
    list_free(xvals);
    turtle.penshape = saveShape; // restore the shape and size before the write
    turtle.pensize = saveSize;
    // no variables in turtleTextRender are changed
}

/* wrapper function for writing strings easier */
void turtleTextWriteString(const char *str, double x, double y, double size, double align) {
    int32_t len = strlen(str);
    uint32_t converted[len];
    for (int32_t i = 0; i < len; i++) {
        converted[i] = (uint32_t) str[i];
    }
    turtleTextWrite(converted, len, x, y, size, align);
}

/* wrapper function for unicode strings (UTF-8, u8"Hello World") */
void turtleTextWriteUnicode(const unsigned char *str, double x, double y, double size, double align) {
    int32_t len = strlen((char *) str);
    uint32_t converted[len]; // max number of characters in a utf-8 string of length len
    int32_t byteLength;
    int32_t i = 0;
    int32_t next = 0;
    while (i < len) {
        byteLength = 0;
        for (uint8_t j = 0; j < 8; j++) {
            uint8_t mask = 128 >> j;
            if (str[i] & mask) {
                byteLength += 1;
            } else {
                j = 8; // end loop
            }
        }
        if (byteLength == 0) { // case: ASCII character
            converted[next] = (uint32_t) str[i];
            byteLength = 1;
        } else { // case: multi-byte character
            uint32_t convert = 0;
            for (int32_t k = 0; k < byteLength; k++) {
                convert = convert << 8;
                convert += (uint32_t) str[i + k];
            }
            converted[next] = convert;
        }
        i += byteLength;
        next += 1;
    }
    turtleTextWrite(converted, next, x, y, size, align);
}
#endif