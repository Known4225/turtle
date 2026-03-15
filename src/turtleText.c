/* 
████████╗██╗   ██╗██████╗ ████████╗██╗     ███████╗████████╗███████╗██╗  ██╗████████╗ ██████╗
╚══██╔══╝██║   ██║██╔══██╗╚══██╔══╝██║     ██╔════╝╚══██╔══╝██╔════╝╚██╗██╔╝╚══██╔══╝██╔════╝
   ██║   ██║   ██║██████╔╝   ██║   ██║     █████╗     ██║   █████╗   ╚███╔╝    ██║   ██║     
   ██║   ██║   ██║██╔══██╗   ██║   ██║     ██╔══╝     ██║   ██╔══╝   ██╔██╗    ██║   ██║     
   ██║   ╚██████╔╝██║  ██║   ██║   ███████╗███████╗   ██║   ███████╗██╔╝ ██╗   ██║██╗╚██████╗
   ╚═╝    ╚═════╝ ╚═╝  ╚═╝   ╚═╝   ╚══════╝╚══════╝   ╚═╝   ╚══════╝╚═╝  ╚═╝   ╚═╝╚═╝ ╚═════╝
https://patorjk.com/software/taag/#p=display&f=ANSI%20Shadow

turtleText uses openGL and the turtle library to render text on the screen
*/

#include "../include/turtleText.h"

turtleText_t turtleText;

/* initialise turtleText, must supply a font file (tgl) - if font file is not found then a default font will be substituted */
int32_t turtleTextInit(const char *filename) {
    turtlePenColor(0, 0, 0);
    turtleText.bezierPrez = 10;

    /* load file */
    char fileExists = 1;
    list_t *generatedFont = list_init();
    FILE *tgl = fopen(filename, "r");
    if (tgl == NULL) {
        printf("Error: could not open file %s, using default\n", filename);
        generateDefaultFont(generatedFont);
        fileExists = 0;
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
    turtleText.charCount = 0;
    while ((fileExists == 0 && turtleText.charCount < generatedFont -> length) || (fileExists == 1 && fgets(line, 2048, tgl) != NULL)) { // fgets to prevent overflows
        if (fileExists == 0) {
            memcpy(line, generatedFont -> data[turtleText.charCount].s, strlen(generatedFont -> data[turtleText.charCount].s) + 1);
        }
        if (line[2047] != 0) {
            printf("Error: line %d in file %s exceeded appropriate length\n", 0, filename);
            return -1;
        }

        oldptr = 0;
        strptr = 0;
        int32_t loops = 0;
        int32_t firstIndex = 0;
        int8_t widthProceed = 0;
        int32_t savedWidth = -2147483640;
        
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
                    strptr += 1;
                }
                list_append(supportedCharReferenceInit, (unitype) fontPar, 'i'); // adds as an int but will typecast back to unsigned later, this might not work correctly but it also doesn't really matter
                list_append(fontPointerInit, (unitype) fontDataInit -> length, 'i');
                firstIndex = fontDataInit -> length;
                list_append(fontDataInit, (unitype) 1, 'i');
            } else {
                sscanf((char *) parsedInt, "%u", &fontPar); // parse as integer
                if (strcmp((char *) parsedInt, "w") == 0) { // w is a width specifier
                    widthProceed = 1;
                    loops -= 1; // counter loops, this field does not count
                } else if (widthProceed) {
                    savedWidth = fontPar;
                    widthProceed = 0;
                    loops -= 1; // counter loops, this field does not count
                } else if (strcmp((char *) parsedInt, "b") == 0) {
                    list_append(fontDataInit, (unitype) 140894115, 'i'); // all b's get converted to the integer 140894115 (chosen semi-randomly)
                } else {
                    list_append(fontDataInit, (unitype) fontPar, 'i'); // fontPar will double count when it encounters a b (idk why) so if there's a b we ignore the second fontPar (which is a duplicate of the previous one)
                }
                loops += 1;
            }
            if (line[strptr] != '\n' && line[strptr] != '\0') {
                strptr += 2;
            }
            oldptr = strptr;
        }
        fontDataInit -> data[firstIndex] = (unitype) loops;
        firstIndex += 1; // using firstIndex as iteration variable
        int32_t len1 = fontDataInit -> data[firstIndex].i;
        int32_t maximums[4] = {-2147483648, -2147483648, 2147483647, 2147483647}; // for describing bounding box of a character (positive X, positive Y, negative X, negative Y)
        
        /* good programmng alert */
        #define CHECKS_EMB(ind) \
            if (fontDataInit -> data[ind].i > maximums[0]) {\
                maximums[0] = fontDataInit -> data[ind].i;\
            }\
            if (fontDataInit -> data[ind].i < maximums[3]) {\
                maximums[2] = fontDataInit -> data[ind].i;\
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
        /* maxX */
        if (savedWidth != -2147483640) {
            list_append(fontDataInit, (unitype) savedWidth, 'i');
        } else {
            if (maximums[0] < 0) {
                list_append(fontDataInit, (unitype) 45, 'i'); // default width of character (space)
            } else {
                list_append(fontDataInit, (unitype) maximums[0], 'i');
            }
        }
        /* maxY */
        if (maximums[1] < 0) {
            if (turtleText.charCount == 0) {
                list_append(fontDataInit, (unitype) 0, 'i');
            } else {
                list_append(fontDataInit, (unitype) 120, 'i');
            }
        } else {
            list_append(fontDataInit, (unitype) maximums[1], 'i');
        }
        /* minX */
        if (maximums[2] > 0) {
            list_append(fontDataInit, (unitype) 0, 'i');
        } else {
            list_append(fontDataInit, (unitype) maximums[2], 'i');
        }
        /* minY */
        if (maximums[3] > 0) {
            list_append(fontDataInit, (unitype) 0, 'i');
        } else {
            list_append(fontDataInit, (unitype) maximums[3], 'i');
        }
        turtleText.charCount += 1;
        // printf("maxX: %d, maxY: %d, minX: %d, minY: %d\n", fontDataInit -> data[fontDataInit -> length - 4].i, fontDataInit -> data[fontDataInit -> length - 3].i, fontDataInit -> data[fontDataInit -> length - 2].i, fontDataInit -> data[fontDataInit -> length - 1].i);
    }
    list_append(fontPointerInit, (unitype) (int32_t) (fontDataInit -> length), 'i'); // last pointer
    // list_print(fontDataInit);
    turtleText.fontData = malloc(sizeof(int32_t) * fontDataInit -> length); // convert lists to arrays (could be optimised cuz we already have the -> data arrays but who really cares this runs once)
    for (int32_t i = 0; i < fontDataInit -> length; i++) {
        turtleText.fontData[i] = fontDataInit -> data[i].i;
    }
    turtleText.fontPointer = malloc(sizeof(int32_t) * fontPointerInit -> length);
    for (int32_t i = 0; i < fontPointerInit -> length; i++) {
        turtleText.fontPointer[i] = fontPointerInit -> data[i].i;
    }
    turtleText.supportedCharReference = malloc(sizeof(int32_t) * supportedCharReferenceInit -> length);
    for (int32_t i = 0; i < supportedCharReferenceInit -> length; i++) {
        turtleText.supportedCharReference[i] = supportedCharReferenceInit -> data[i].i;
    }

    if (fileExists) {
        printf("%d characters loaded from %s\n", turtleText.charCount, filename);
    } else {
        printf("%d characters loaded from default font\n", turtleText.charCount);
    }

    list_free(fontDataInit);
    list_free(fontPointerInit);
    list_free(supportedCharReferenceInit);
    list_free(generatedFont);
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

/* renders a single character - INTERNAL */
void renderChar(int32_t index, double x, double y, double size) {
    double sizeSave = turtle.pensize;
    index += 1;
    int32_t len1 = turtleText.fontData[index];
    for (int32_t i = 0; i < len1; i++) {
        index += 1;
        if (turtle.pen == 1) {
            turtlePenUp();
        }
        int32_t len2 = turtleText.fontData[index];
        if (len2 == 1) {
            turtle.pensize = sizeSave * 1.5; // dots on is are a bit bigger
        } else {
            turtle.pensize = sizeSave;
        }
        for (int32_t j = 0; j < len2; j++) {
            index += 1;
            if (turtleText.fontData[index] == 140894115) { // 140894115 is the b value (reserved)
                index += 4;
                if (turtleText.fontData[index + 1] != 140894115) {
                    renderBezier(x + turtleText.fontData[index - 3] * size, y + turtleText.fontData[index - 2] * size, x + turtleText.fontData[index - 1] * size, y + turtleText.fontData[index] * size, x + turtleText.fontData[index + 1] * size, y + turtleText.fontData[index + 2] * size, turtleText.bezierPrezCurrent);
                    index += 2;
                } else {
                    renderBezier(x + turtleText.fontData[index - 3] * size, y + turtleText.fontData[index - 2] * size, x + turtleText.fontData[index - 1] * size, y + turtleText.fontData[index] * size, x + turtleText.fontData[index + 2] * size, y + turtleText.fontData[index + 3] * size, turtleText.bezierPrezCurrent);
                }
            } else {
                index += 1;
                turtleGoto(x + turtleText.fontData[index - 1] * size, y + turtleText.fontData[index] * size);
            }
            turtlePenDown();
        }
    }
    turtlePenUp();
    turtle.pensize = sizeSave;
}

/* special version of renderChar with rotation - INTERNAL */
void renderCharRotated(int32_t index, double x, double y, double size, double sinR, double cosR) {
    index += 1;
    int32_t len1 = turtleText.fontData[index];
    for (int32_t i = 0; i < len1; i++) {
        index += 1;
        if (turtle.pen == 1) {
            turtlePenUp();
        }
        int32_t len2 = turtleText.fontData[index];
        for (int32_t j = 0; j < len2; j++) {
            index += 1;
            if (turtleText.fontData[index] == 140894115) { // 140894115 is the b value (reserved)
                index += 4;
                if (turtleText.fontData[index + 1] != 140894115) {
                    renderBezier(x + turtleText.fontData[index - 3] * size * cosR + turtleText.fontData[index - 2] * size * sinR, y - turtleText.fontData[index - 3] * size * sinR + turtleText.fontData[index - 2] * size * cosR,
                                 x + turtleText.fontData[index - 1] * size * cosR + turtleText.fontData[index] * size * sinR, y - turtleText.fontData[index - 1] * size * sinR + turtleText.fontData[index] * size * cosR,
                                 x + turtleText.fontData[index + 1] * size * cosR + turtleText.fontData[index + 2] * size * sinR, y - turtleText.fontData[index + 1] * size * sinR + turtleText.fontData[index + 2] * size * cosR, turtleText.bezierPrezCurrent);
                    index += 2;
                } else {
                    renderBezier(x + turtleText.fontData[index - 3] * size * cosR + turtleText.fontData[index - 2] * size * sinR, y - turtleText.fontData[index - 3] * size * sinR + turtleText.fontData[index - 2] * size * cosR,
                                 x + turtleText.fontData[index - 1] * size * cosR + turtleText.fontData[index] * size * sinR, y - turtleText.fontData[index - 1] * size * sinR + turtleText.fontData[index] * size * cosR,
                                 x + turtleText.fontData[index + 2] * size * cosR + turtleText.fontData[index + 3] * size * sinR, y - turtleText.fontData[index + 2] * size * sinR + turtleText.fontData[index + 3] * size * cosR, turtleText.bezierPrezCurrent);
                }
            } else {
                index += 1;
                turtleGoto(x + turtleText.fontData[index - 1] * size * cosR + turtleText.fontData[index] * size * sinR, y - turtleText.fontData[index - 1] * size * sinR + turtleText.fontData[index] * size * cosR);
            }
            turtlePenDown();
        }
    }
    turtlePenUp();
}

/* gets the length of a string in coordinates on the screen */
double turtleTextGetLength(const uint32_t *text, int32_t textLength, double size) {
    if (textLength == 0) {
        return 0;
    }
    size /= 175;
    double xTrack = 0;
    for (int32_t i = 0; i < textLength; i++) {
        int32_t currentDataAddress = 0;
        for (int32_t j = 0; j < turtleText.charCount; j++) { // change to hashmap later
            if (turtleText.supportedCharReference[j] == text[i]) {
                currentDataAddress = j;
                break;
            }
        }
        xTrack += (turtleText.fontData[turtleText.fontPointer[currentDataAddress + 1] - 4] + 40) * size;
    }
    xTrack -= 40 * size;
    return xTrack;
}

/* gets the length of a formatted string in coordinates on the screen */
double turtleTextGetStringLength(const char *str, double size) {
    int32_t len = strlen(str);
    uint32_t converted[len];
    for (int32_t i = 0; i < len; i++) {
        converted[i] = (uint32_t) str[i];
    }
    return turtleTextGetLength(converted, len, size);
}

/* gets the length of a string in coordinates on the screen */
double turtleTextGetStringLengthf(double size, const char *str, ...) {
    char buffer[2048];
    va_list args;
    va_start(args, str);
    vsnprintf(buffer, 2048, str, args);
    double out = turtleTextGetStringLength(buffer, size);
    va_end(args);
    return out;
}

/* gets the length of a formatted utf8-string in coordinates on the screen */
double turtleTextGetUnicodeLength(const char *str, double size) {
    int32_t len = strlen(str);
    uint32_t converted[len];
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
            converted[next] = (uint32_t) (uint8_t) str[i];
            byteLength = 1;
        } else { // case: multi-byte character
            uint32_t convert = 0;
            for (int32_t k = 0; k < byteLength; k++) {
                convert = convert << 8;
                convert += (uint32_t) (uint8_t) str[i + k];
            }
            converted[next] = convert;
        }
        i += byteLength;
        next += 1;
    }
    return turtleTextGetLength(converted, next, size);
}

/* gets the length of a utf8-string in coordinates on the screen */
double turtleTextGetUnicodeLengthf(double size, const char *str, ...) {
    char buffer[2048];
    va_list args;
    va_start(args, str);
    vsnprintf(buffer, 2048, str, args);
    double out = turtleTextGetUnicodeLength(buffer, size);
    va_end(args);
    return out;
}

/* cut the text of a string such that it will fit in a coordinate size width (0 - left truncate, 1 - right truncate) */
void turtleTextTruncateString(char *str, double size, double width, int8_t leftRight) {
    int32_t length = strlen(str);
    size /= 175;
    double xTrack = 0;
    if (leftRight) {
        /* right */
        for (int32_t i = 0; i < length; i++) {
            int32_t currentDataAddress = 0;
            for (int32_t j = 0; j < turtleText.charCount; j++) { // change to hashmap later
                if (turtleText.supportedCharReference[j] == str[i]) {
                    currentDataAddress = j;
                    break;
                }
            }
            xTrack += (turtleText.fontData[turtleText.fontPointer[currentDataAddress + 1] - 4] + 40) * size;
            if (xTrack - 40 * size > width) {
                str[i] = '\0';
                return;
            }
        }
    } else {
        /* left */
        for (int32_t i = length - 1; i > -1; i--) {
            int32_t currentDataAddress = 0;
            for (int32_t j = 0; j < turtleText.charCount; j++) { // change to hashmap later
                if (turtleText.supportedCharReference[j] == str[i]) {
                    currentDataAddress = j;
                    break;
                }
            }
            xTrack += (turtleText.fontData[turtleText.fontPointer[currentDataAddress + 1] - 4] + 40) * size;
            if (xTrack - 40 * size > width) {
                memmove(str, str + i + 1, length - i);
                return;
            }
        }
    }
}

/* Writes to the screen - INTERNAL */
void turtleTextWrite(const uint32_t *text, int32_t textLength, double x, double y, double size, double align) {
    uint16_t saveShape = turtle.penshape;
    double saveSize = turtle.pensize;
    turtleText.bezierPrezCurrent = (int32_t) ceil(sqrt(size * turtleText.bezierPrez / 10));
    double xTrack = x;
    #ifdef TURTLE_TEXT_DO_DYNAMIC_Y_CENTERING
    double maxY = 0;
    double minY = 0;
    #endif
    size /= 175;
    turtlePenSize(20 * size);
    #if defined(TURTLE_TEXT_FAST_PEN) && !defined(TURTLE_TEXT_PRETTY_PEN)
    turtlePenShape("connected"); // fast
    #else
    #if !defined(TURTLE_TEXT_FAST_PEN) && defined(TURTLE_TEXT_PRETTY_PEN)
    turtlePenShape("circle"); // pretty
    #else
    turtlePenShape("text"); // dedicated setting that blends circle and connected
    #endif
    #endif
    list_t *xvals = list_init();
    list_t *dataIndStored = list_init();
    for (int32_t i = 0; i < textLength; i++) {
        int32_t currentDataAddress = 0;
        for (int32_t j = 0; j < turtleText.charCount; j++) { // change to hashmap later
            if (turtleText.supportedCharReference[j] == text[i]) {
                currentDataAddress = j;
                break;
            }
        }
        list_append(xvals, (unitype) xTrack, 'd');
        list_append(dataIndStored, (unitype) currentDataAddress, 'i');
        xTrack += (turtleText.fontData[turtleText.fontPointer[currentDataAddress + 1] - 4] + 40) * size;
        #ifdef TURTLE_TEXT_DO_DYNAMIC_Y_CENTERING
        if (maxY < turtleText.fontData[turtleText.fontPointer[currentDataAddress + 1] - 3]) {
            maxY = turtleText.fontData[turtleText.fontPointer[currentDataAddress + 1] - 3];
        }
        if (minY > turtleText.fontData[turtleText.fontPointer[currentDataAddress + 1] - 1]) {
            minY = turtleText.fontData[turtleText.fontPointer[currentDataAddress + 1] - 1];
        }
        #endif
    }
    xTrack -= 40 * size;
    #ifdef TURTLE_TEXT_DO_DYNAMIC_Y_CENTERING
    y -= (maxY + minY) / 2 * size;
    #else
    y -= 80 * size;
    #endif
    for (int32_t i = 0; i < textLength; i++) {
        renderChar(turtleText.fontPointer[dataIndStored -> data[i].i], xvals -> data[i].d - ((xTrack - x) * (align / 100)), y, size);
    }
    list_free(dataIndStored);
    list_free(xvals);
    turtle.penshape = saveShape; // restore the shape and size before the write
    turtle.pensize = saveSize;
}

/* Special form of write function which supports rotated text - INTERNAL */
void turtleTextWriteRotated(const uint32_t *text, int32_t textLength, double x, double y, double size, double align, double rotate) {
    uint16_t saveShape = turtle.penshape;
    double saveSize = turtle.pensize;
    turtleText.bezierPrezCurrent = (int32_t) ceil(sqrt(size * turtleText.bezierPrez / 10));
    double xTrack = 0;
    #ifdef TURTLE_TEXT_DO_DYNAMIC_Y_CENTERING
    double maxY = 0;
    double minY = 0;
    #endif
    size /= 175;
    turtlePenSize(20 * size);
    #if defined(TURTLE_TEXT_FAST_PEN) && !defined(TURTLE_TEXT_PRETTY_PEN)
    turtlePenShape("connected"); // fast
    #else
    #if !defined(TURTLE_TEXT_FAST_PEN) && defined(TURTLE_TEXT_PRETTY_PEN)
    turtlePenShape("circle"); // pretty
    #else
    turtlePenShape("text"); // dedicated setting that blends circle and connected
    #endif
    #endif
    list_t *xvals = list_init();
    list_t *dataIndStored = list_init();
    for (int32_t i = 0; i < textLength; i++) {
        int32_t currentDataAddress = 0;
        for (int32_t j = 0; j < turtleText.charCount; j++) { // change to hashmap later
            if (turtleText.supportedCharReference[j] == text[i]) {
                currentDataAddress = j;
                break;
            }
        }
        list_append(xvals, (unitype) xTrack, 'd');
        list_append(dataIndStored, (unitype) currentDataAddress, 'i');
        xTrack += (turtleText.fontData[turtleText.fontPointer[currentDataAddress + 1] - 4] + 40) * size;
        #ifdef TURTLE_TEXT_DO_DYNAMIC_Y_CENTERING
        if (maxY < turtleText.fontData[turtleText.fontPointer[currentDataAddress + 1] - 3]) {
            maxY = turtleText.fontData[turtleText.fontPointer[currentDataAddress + 1] - 3];
        }
        if (minY > turtleText.fontData[turtleText.fontPointer[currentDataAddress + 1] - 1]) {
            minY = turtleText.fontData[turtleText.fontPointer[currentDataAddress + 1] - 1];
        }
        #endif
    }
    xTrack -= 40 * size;
    #ifdef TURTLE_TEXT_DO_DYNAMIC_Y_CENTERING
    y -= (maxY + minY) / 2 * size;
    #else
    y -= 80 * size;
    #endif
    double cosR = cos(rotate / 57.2958);
    double sinR = sin(rotate / 57.2958);
    for (int32_t i = 0; i < textLength; i++) {
        renderCharRotated(turtleText.fontPointer[dataIndStored -> data[i].i], x + (xvals -> data[i].d - (xTrack * (align / 100))) * cosR, y - (xvals -> data[i].d - (xTrack * (align / 100))) * sinR, size, sinR, cosR);
    }
    list_free(dataIndStored);
    list_free(xvals);
    turtle.penshape = saveShape; // restore the shape and size before the write
    turtle.pensize = saveSize;
}

/* Write a string to the screen */
void turtleTextWriteString(const char *str, double x, double y, double size, double align) {
    int32_t len = strlen(str);
    uint32_t converted[len];
    for (int32_t i = 0; i < len; i++) {
        converted[i] = (uint32_t) str[i];
    }
    turtleTextWrite(converted, len, x, y, size, align);
}

/* Write a formatted string to the screen */
void turtleTextWriteStringf(double x, double y, double size, double align, const char *str, ...) {
    char buffer[2048];
    va_list args;
    va_start(args, str);
    vsnprintf(buffer, 2048, str, args);
    turtleTextWriteString(buffer, x, y, size, align);
    va_end(args);
}

/* Write a string to the screen (with rotation) */
void turtleTextWriteStringRotated(const char *str, double x, double y, double size, double align, double rotate) {
    int32_t len = strlen(str);
    uint32_t converted[len];
    for (int32_t i = 0; i < len; i++) {
        converted[i] = (uint32_t) str[i];
    }
    turtleTextWriteRotated(converted, len, x, y, size, align, rotate);
}

/* Write a formatted string to the screen (with rotation) */
void turtleTextWriteStringfRotated(double x, double y, double size, double align, double rotate, const char *str, ...) {
    char buffer[2048];
    va_list args;
    va_start(args, str);
    vsnprintf(buffer, 2048, str, args);
    turtleTextWriteStringRotated(buffer, x, y, size, align, rotate);
    va_end(args);
}

/* Write a utf8-string to the screen */
void turtleTextWriteUnicode(const char *str, double x, double y, double size, double align) {
    int32_t len = strlen(str);
    uint32_t converted[len];
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
            converted[next] = (uint32_t) (uint8_t) str[i];
            byteLength = 1;
        } else { // case: multi-byte character
            uint32_t convert = 0;
            for (int32_t k = 0; k < byteLength; k++) {
                convert = convert << 8;
                convert += (uint32_t) (uint8_t) str[i + k];
            }
            converted[next] = convert;
        }
        i += byteLength;
        next += 1;
    }
    turtleTextWrite(converted, next, x, y, size, align);
}

/* Write a formatted utf8-string to the screen */
void turtleTextWriteUnicodef(double x, double y, double size, double align, const char *str, ...) {
    char buffer[2048];
    va_list args;
    va_start(args, str);
    vsnprintf(buffer, 2048, str, args);
    turtleTextWriteUnicode(buffer, x, y, size, align);
    va_end(args);
}

/* Write a utf8-string to the screen (with rotation) */
void turtleTextWriteUnicodeRotated(const char *str, double x, double y, double size, double align, double rotate) {
    int32_t len = strlen(str);
    uint32_t converted[len];
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
            converted[next] = (uint32_t) (uint8_t) str[i];
            byteLength = 1;
        } else { // case: multi-byte character
            uint32_t convert = 0;
            for (int32_t k = 0; k < byteLength; k++) {
                convert = convert << 8;
                convert += (uint32_t) (uint8_t) str[i + k];
            }
            converted[next] = convert;
        }
        i += byteLength;
        next += 1;
    }
    turtleTextWriteRotated(converted, next, x, y, size, align, rotate);
}

/* Write a formatted utf8-string to the screen (with rotation) */
void turtleTextWriteUnicodefRotated(double x, double y, double size, double align, double rotate, const char *str, ...) {
    char buffer[2048];
    va_list args;
    va_start(args, str);
    vsnprintf(buffer, 2048, str, args);
    turtleTextWriteUnicodeRotated(buffer, x, y, size, align, rotate);
    va_end(args);
}

/* internal function for converting utf8 to uint32_t characters */
int32_t turtleTextConvertUnicode(const char *str, uint32_t *converted) {
    int32_t len = strlen(str);
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
            converted[next] = (uint32_t) (uint8_t) str[i];
            byteLength = 1;
        } else { // case: multi-byte character
            uint32_t convert = 0;
            for (int32_t k = 0; k < byteLength; k++) {
                convert = convert << 8;
                convert += (uint32_t) (uint8_t) str[i + k];
            }
            converted[next] = convert;
        }
        i += byteLength;
        next += 1;
    }
    return next;
}

/* if the font file is not found, use the default font (kept here) */
void generateDefaultFont(list_t *generatedFont) {
    list_append(generatedFont, (unitype) " , 0", 's');
    list_append(generatedFont, (unitype) "A, 2, 3, -160, -100, -100, 60, -40, -100, 2, -137, -40, -63, -40", 's');
    list_append(generatedFont, (unitype) "À, 3, 3, -160, -100, -100, 60, -40, -100, 2, -137, -40, -63, -40, 2, -100, 85, -120, 105", 's');
    list_append(generatedFont, (unitype) "Á, 3, 3, -160, -100, -100, 60, -40, -100, 2, -137, -40, -63, -40, 2, -100, 85, -80, 105", 's');
    list_append(generatedFont, (unitype) "Â, 3, 3, -160, -100, -100, 60, -40, -100, 2, -137, -40, -63, -40, 3, -130, 85, -100, 105, -70, 85", 's');
    list_append(generatedFont, (unitype) "Ă, 3, 3, -160, -100, -100, 60, -40, -100, 2, -137, -40, -63, -40, 2, b, -130, 105, -130, 85, b, -100, 85, -70, 85, -70, 105", 's');
    list_append(generatedFont, (unitype) "Ä, 4, 3, -160, -100, -100, 60, -40, -100, 2, -137, -40, -63, -40, 1, -120, 95, 1, -80, 95", 's');
    list_append(generatedFont, (unitype) "Ã, 3, 3, -160, -100, -100, 60, -40, -100, 2, -137, -40, -63, -40, 4, b, -135, 85, -135, 105, b, -120, 105, -110, 105, b, -100, 95, -90, 85, b, -80, 85, -65, 85, -65, 105", 's');
    list_append(generatedFont, (unitype) "Å, 3, 3, -160, -100, -100, 60, -40, -100, 2, -137, -40, -63, -40, 4, b, -100, 85, -80, 85, b, -80, 105, -80, 125, b, -100, 125, -120, 125, b, -120, 105, -120, 85, -100, 85", 's');
    list_append(generatedFont, (unitype) "Ā, 3, 3, -160, -100, -100, 60, -40, -100, 2, -137, -40, -63, -40, 2, -145, 95, -55, 95", 's');
    list_append(generatedFont, (unitype) "Ą, 2, 2, -137, -40, -63, -40, 4, -160, -100, -100, 60, b, -40, -100, -65, -105, b, -65, -125, -65, -150, -40, -145", 's');
    list_append(generatedFont, (unitype) "Æ, 4, 2, -118, -40, -50, -40, 4, -160, -100, -50, 60, -50, -100, 50, -100, 2, -50, -20, 40, -20, 2, -50, 60, 50, 60", 's');
    list_append(generatedFont, (unitype) "B, 2, 5, -160, -100, -160, 60, b, -110, 60, -60, 60, b, -60, 20, -60, -20, -110, -20, -160, -20, 3, b, -110, -20, -60, -20, b, -60, -60, -60, -100, -110, -100, -160, -100", 's');
    list_append(generatedFont, (unitype) "C, 1, 4, b, -50, 20, -60, 60, b, -100, 60, -160, 60, b, -160, -20, -160, -100, b, -100, -100, -60, -100, -50, -60", 's');
    list_append(generatedFont, (unitype) "Ć, 2, 4, b, -50, 20, -60, 60, b, -100, 60, -160, 60, b, -160, -20, -160, -100, b, -100, -100, -60, -100, -50, -60, 2, -100, 85, -80, 105", 's');
    list_append(generatedFont, (unitype) "Č, 2, 4, b, -50, 20, -60, 60, b, -100, 60, -160, 60, b, -160, -20, -160, -100, b, -100, -100, -60, -100, -50, -60, 3, -130, 105, -100, 85, -70, 105", 's');
    list_append(generatedFont, (unitype) "Ċ, 2, 4, b, -50, 20, -60, 60, b, -100, 60, -160, 60, b, -160, -20, -160, -100, b, -100, -100, -60, -100, -50, -60, 1, -100, 95", 's');
    list_append(generatedFont, (unitype) "Ç, 2, 4, b, -50, 20, -60, 60, b, -100, 60, -160, 60, b, -160, -20, -160, -100, b, -100, -100, -60, -100, -50, -60, 3, -100, -100, b, -103, -115, -80, -120, b, -80, -135, -80, -160, -105, -155", 's');
    list_append(generatedFont, (unitype) "D, 1, 5, -160, -100, -160, 60, b, -110, 60, -50, 60, b, -50, -20, -50, -100, -110, -100, -160, -100", 's');
    list_append(generatedFont, (unitype) "Ď, 2, 5, -160, -100, -160, 60, b, -110, 60, -50, 60, b, -50, -20, -50, -100, -110, -100, -160, -100, 3, -145, 105, -115, 85, -85, 105", 's');
    list_append(generatedFont, (unitype) "Đ, 2, 5, -160, -100, -160, 60, b, -110, 60, -50, 60, b, -50, -20, -50, -100, -110, -100, -160, -100, 2, -180, -20, -130, -20", 's');
    list_append(generatedFont, (unitype) "Ð, 2, 5, -160, -100, -160, 60, b, -110, 60, -50, 60, b, -50, -20, -50, -100, -110, -100, -160, -100, 2, -180, -20, -130, -20", 's');
    list_append(generatedFont, (unitype) "E, 2, 4, -70, -100, -160, -100, -160, 60, -70, 60, 2, -160, -20, -80, -20", 's');
    list_append(generatedFont, (unitype) "È, 3, 4, -70, -100, -160, -100, -160, 60, -70, 60, 2, -160, -20, -80, -20, 2, -115, 85, -135, 105", 's');
    list_append(generatedFont, (unitype) "É, 3, 4, -70, -100, -160, -100, -160, 60, -70, 60, 2, -160, -20, -80, -20, 2, -115, 85, -95, 105", 's');
    list_append(generatedFont, (unitype) "Ě, 3, 4, -70, -100, -160, -100, -160, 60, -70, 60, 2, -160, -20, -80, -20, 3, -145, 105, -115, 85, -85, 105", 's');
    list_append(generatedFont, (unitype) "Ê, 3, 4, -70, -100, -160, -100, -160, 60, -70, 60, 2, -160, -20, -80, -20, 3, -145, 85, -115, 105, -85, 85", 's');
    list_append(generatedFont, (unitype) "Ë, 4, 4, -70, -100, -160, -100, -160, 60, -70, 60, 2, -160, -20, -80, -20, 1, -135, 95, 1, -95, 95", 's');
    list_append(generatedFont, (unitype) "Ē, 3, 4, -70, -100, -160, -100, -160, 60, -70, 60, 2, -160, -20, -80, -20, 2, -145, 95, -85, 95", 's');
    list_append(generatedFont, (unitype) "Ė, 3, 4, -70, -100, -160, -100, -160, 60, -70, 60, 2, -160, -20, -80, -20, 1, -115, 95", 's');
    list_append(generatedFont, (unitype) "Ę, 2, 2, -160, -20, -80, -20, 5, -70, 60, -160, 60, -160, -100, b, -70, -100, -95, -105, b, -95, -125, -95, -150, -70, -145", 's');
    list_append(generatedFont, (unitype) "Ə, 1, 5, -30, -20, b, -160, -20, -160, -100, b, -95, -100, -30, -100, b, -30, -20, -30, 60, b, -95, 60, -125, 60, -140, 45", 's');
    list_append(generatedFont, (unitype) "F, 2, 3, -160, -100, -160, 60, -70, 60, 2, -160, -20, -80, -20", 's');
    list_append(generatedFont, (unitype) "G, 1, 6, b, -50, 20, -60, 60, b, -100, 60, -160, 60, b, -160, -20, -160, -100, b, -100, -100, -60, -100, -50, -60, -50, -20, -100, -20", 's');
    list_append(generatedFont, (unitype) "Ğ, 2, 6, b, -50, 20, -60, 60, b, -100, 60, -160, 60, b, -160, -20, -160, -100, b, -100, -100, -60, -100, -50, -60, -50, -20, -100, -20, 2, b, -135, 105, -135, 85, b, -105, 85, -75, 85, -75, 105", 's');
    list_append(generatedFont, (unitype) "Ġ, 2, 6, b, -50, 20, -60, 60, b, -100, 60, -160, 60, b, -160, -20, -160, -100, b, -100, -100, -60, -100, -50, -60, -50, -20, -100, -20, 1, -105, 95", 's');
    list_append(generatedFont, (unitype) "H, 3, 2, -160, -100, -160, 60, 2, -50, -100, -50, 60, 2, -160, -20, -50, -20", 's');
    list_append(generatedFont, (unitype) "Ħ, 4, 2, -160, -100, -160, 60, 2, -50, -100, -50, 60, 2, -160, -20, -50, -20, 2, -170, 20, -40, 20", 's');
    list_append(generatedFont, (unitype) "I, 1, 2, -160, -100, -160, 60", 's');
    list_append(generatedFont, (unitype) "Ì, 2, 2, -160, -100, -160, 60, 2, -160, 90, -170, 110", 's');
    list_append(generatedFont, (unitype) "Í, 2, 2, -160, -100, -160, 60, 2, -160, 90, -150, 110", 's');
    list_append(generatedFont, (unitype) "Î, 2, 2, -160, -100, -160, 60, 3, -180, 90, -160, 110, -140, 90", 's');
    list_append(generatedFont, (unitype) "Ï, 3, 2, -160, -100, -160, 60, 1, -180, 100, 1, -140, 100", 's');
    list_append(generatedFont, (unitype) "Ī, 2, 2, -160, -100, -160, 60, 2, -180, 100, -140, 100", 's');
    list_append(generatedFont, (unitype) "İ, 2, 2, -160, -100, -160, 60, 1, -160, 100", 's');
    list_append(generatedFont, (unitype) "Į, 1, 3, -160, 60, b, -160, -100, -170, -105, b, -170, -115, -170, -125, -160, -125", 's');
    list_append(generatedFont, (unitype) "J, 1, 3, -70, 60, b, -70, -50, -70, -100, b, -120, -100, -150, -100, -160, -70", 's');
    list_append(generatedFont, (unitype) "K, 3, 2, -160, -100, -160, 60, 2, -160, -30, -60, 60, 2, -60, -100, -140, -12", 's');
    list_append(generatedFont, (unitype) "Ķ, 4, 2, -160, -100, -160, 60, 2, -160, -30, -60, 60, 2, -60, -100, -140, -12, 1, b, -115, -105, -115, -120, -125, -125", 's');
    list_append(generatedFont, (unitype) "L, 1, 3, -160, 60, -160, -100, -70, -100", 's');
    list_append(generatedFont, (unitype) "Ĺ, 2, 3, -160, 60, -160, -100, -70, -100, 2, -160, 90, -140, 110", 's');
    list_append(generatedFont, (unitype) "Ľ, 2, 3, -160, 60, -160, -100, -70, -100, 1, b, -105, 40, -95, 55, -95, 70", 's');
    list_append(generatedFont, (unitype) "Ļ, 2, 3, -160, 60, -160, -100, -70, -100, 1, b, -115, -125, -115, -140, -125, -145", 's');
    list_append(generatedFont, (unitype) "Ł, 2, 3, -160, 60, -160, -100, -70, -100, 2, -180, -30, -135, -15", 's');
    list_append(generatedFont, (unitype) "Ŀ, 2, 3, -160, 60, -160, -100, -70, -100, 1, -120, -30", 's');
    list_append(generatedFont, (unitype) "M, 1, 5, -160, -100, -160, 60, -90, -100, -20, 60, -20, -100", 's');
    list_append(generatedFont, (unitype) "N, 1, 4, -160, -100, -160, 60, -50, -100, -50, 60", 's');
    list_append(generatedFont, (unitype) "Ń, 2, 4, -160, -100, -160, 60, -50, -100, -50, 60, 2, -105, 85, -85, 105", 's');
    list_append(generatedFont, (unitype) "Ň, 2, 4, -160, -100, -160, 60, -50, -100, -50, 60, 3, -135, 105, -105, 85, -75, 105", 's');
    list_append(generatedFont, (unitype) "Ñ, 2, 4, -160, -100, -160, 60, -50, -100, -50, 60, 4, b, -140, 85, -140, 105, b, -125, 105, -115, 105, b, -105, 95, -95, 85, b, -85, 85, -70, 85, -70, 105", 's');
    list_append(generatedFont, (unitype) "Ņ, 2, 4, -160, -100, -160, 60, -50, -100, -50, 60, 1, b, -105, -125, -105, -140, -115, -145", 's');
    list_append(generatedFont, (unitype) "O, 1, 4, b, -40, -20, -40, 60, b, -100, 60, -160, 60, b, -160, -20, -160, -100, b, -100, -100, -40, -100, -40, -20", 's');
    list_append(generatedFont, (unitype) "Ò, 2, 4, b, -40, -20, -40, 60, b, -100, 60, -160, 60, b, -160, -20, -160, -100, b, -100, -100, -40, -100, -40, -20, 2, -100, 85, -120, 105", 's');
    list_append(generatedFont, (unitype) "Ó, 2, 4, b, -40, -20, -40, 60, b, -100, 60, -160, 60, b, -160, -20, -160, -100, b, -100, -100, -40, -100, -40, -20, 2, -100, 85, -80, 105", 's');
    list_append(generatedFont, (unitype) "Ô, 2, 4, b, -40, -20, -40, 60, b, -100, 60, -160, 60, b, -160, -20, -160, -100, b, -100, -100, -40, -100, -40, -20, 3, -130, 85, -100, 105, -70, 85", 's');
    list_append(generatedFont, (unitype) "Ö, 3, 4, b, -40, -20, -40, 60, b, -100, 60, -160, 60, b, -160, -20, -160, -100, b, -100, -100, -40, -100, -40, -20, 1, -120, 95, 1, -80, 95", 's');
    list_append(generatedFont, (unitype) "Õ, 2, 4, b, -40, -20, -40, 60, b, -100, 60, -160, 60, b, -160, -20, -160, -100, b, -100, -100, -40, -100, -40, -20, 4, b, -135, 85, -135, 105, b, -120, 105, -110, 105, b, -100, 95, -90, 85, b, -80, 85, -65, 85, -65, 105", 's');
    list_append(generatedFont, (unitype) "Ő, 3, 4, b, -40, -20, -40, 60, b, -100, 60, -160, 60, b, -160, -20, -160, -100, b, -100, -100, -40, -100, -40, -20, 2, -130, 85, -115, 105, 2, -80, 85, -65, 105", 's');
    list_append(generatedFont, (unitype) "Ø, 2, 4, b, -40, -20, -40, 60, b, -100, 60, -160, 60, b, -160, -20, -160, -100, b, -100, -100, -40, -100, -40, -20, 2, -40, 60, -160, -100", 's');
    list_append(generatedFont, (unitype) "Œ, 2, 6, b, -50, -20, -50, 60, b, -105, 60, -160, 60, b, -160, -20, -160, -100, b, -105, -100, -50, -100, -50, -20, -50, 60, 30, 60, 4, 30, -100, -50, -100, -50, -20, 20, -20", 's');
    list_append(generatedFont, (unitype) "P, 1, 5, -160, -100, -160, 60, b, -100, 60, -60, 60, b, -60, 20, -60, -20, -100, -20, -160, -20", 's');
    list_append(generatedFont, (unitype) "Q, 2, 4, b, -40, -20, -40, 60, b, -100, 60, -160, 60, b, -160, -20, -160, -100, b, -100, -100, -40, -100, -40, -20, 2, -65, -90, -40, -110", 's');
    list_append(generatedFont, (unitype) "R, 2, 5, -160, -100, -160, 60, b, -100, 60, -60, 60, b, -60, 20, -60, -20, -100, -20, -160, -20, 2, -100, -20, -60, -100", 's');
    list_append(generatedFont, (unitype) "Ŕ, 3, 5, -160, -100, -160, 60, b, -100, 60, -60, 60, b, -60, 20, -60, -20, -100, -20, -160, -20, 2, -100, -20, -60, -100, 2, -125, 85, -105, 105", 's');
    list_append(generatedFont, (unitype) "Ř, 3, 5, -160, -100, -160, 60, b, -100, 60, -60, 60, b, -60, 20, -60, -20, -100, -20, -160, -20, 2, -100, -20, -60, -100, 3, -150, 105, -120, 85, -90, 105", 's');
    list_append(generatedFont, (unitype) "S, 1, 6, b, -60, 30, -70, 60, b, -110, 60, -160, 60, b, -160, 20, -160, -20, b, -110, -20, -60, -20, b, -60, -60, -60, -100, b, -110, -100, -150, -100, -160, -70", 's');
    list_append(generatedFont, (unitype) "Ś, 2, 6, b, -60, 30, -70, 60, b, -110, 60, -160, 60, b, -160, 20, -160, -20, b, -110, -20, -60, -20, b, -60, -60, -60, -100, b, -110, -100, -150, -100, -160, -70, 2, -110, 85, -90, 105", 's');
    list_append(generatedFont, (unitype) "Š, 2, 6, b, -60, 30, -70, 60, b, -110, 60, -160, 60, b, -160, 20, -160, -20, b, -110, -20, -60, -20, b, -60, -60, -60, -100, b, -110, -100, -150, -100, -160, -70, 3, -140, 105, -110, 85, -80, 105", 's');
    list_append(generatedFont, (unitype) "Ş, 2, 6, b, -60, 30, -70, 60, b, -110, 60, -160, 60, b, -160, 20, -160, -20, b, -110, -20, -60, -20, b, -60, -60, -60, -100, b, -110, -100, -150, -100, -160, -70, 3, -110, -100, b, -113, -115, -90, -120, b, -90, -135, -90, -160, -115, -150", 's');
    list_append(generatedFont, (unitype) "Ș, 2, 6, b, -60, 30, -70, 60, b, -110, 60, -160, 60, b, -160, 20, -160, -20, b, -110, -20, -60, -20, b, -60, -60, -60, -100, b, -110, -100, -150, -100, -160, -70, 1, b, -110, -125, -110, -140, -120, -145", 's');
    list_append(generatedFont, (unitype) "ẞ, 1, 5, -160, -100, b, -160, 15, -160, 60, b, -110, 60, -60, 60, -55, 30, b, -105, -20, -40, -15, b, -50, -70, -65, -110, -120, -95", 's');
    list_append(generatedFont, (unitype) "T, 2, 2, -160, 60, -40, 60, 2, -100, 60, -100, -100", 's');
    list_append(generatedFont, (unitype) "Ť, 3, 2, -160, 60, -40, 60, 2, -100, 60, -100, -100, 3, -130, 105, -100, 85, -70, 105", 's');
    list_append(generatedFont, (unitype) "Ț, 3, 2, -160, 60, -40, 60, 2, -100, 60, -100, -100, 1, b, -100, -125, -100, -140, -110, -145", 's');
    list_append(generatedFont, (unitype) "Þ, 2, 2, -160, -100, -160, 60, 4, -160, 20, b, -105, 20, -60, 20, b, -60, -20, -60, -60, -105, -60, -160, -60", 's');
    list_append(generatedFont, (unitype) "U, 1, 4, -160, 60, b, -160, -40, -160, -100, b, -105, -100, -50, -100, -50, -40, -50, 60", 's');
    list_append(generatedFont, (unitype) "Ù, 2, 4, -160, 60, b, -160, -40, -160, -100, b, -105, -100, -50, -100, -50, -40, -50, 60, 2, -105, 85, -125, 105", 's');
    list_append(generatedFont, (unitype) "Ú, 2, 4, -160, 60, b, -160, -40, -160, -100, b, -105, -100, -50, -100, -50, -40, -50, 60, 2, -105, 85, -85, 105", 's');
    list_append(generatedFont, (unitype) "Û, 2, 4, -160, 60, b, -160, -40, -160, -100, b, -105, -100, -50, -100, -50, -40, -50, 60, 3, -135, 105, -105, 85, -75, 105", 's');
    list_append(generatedFont, (unitype) "Ü, 3, 4, -160, 60, b, -160, -40, -160, -100, b, -105, -100, -50, -100, -50, -40, -50, 60, 1, -125, 95, 1, -85, 95", 's');
    list_append(generatedFont, (unitype) "Ů, 2, 4, -160, 60, b, -160, -40, -160, -100, b, -105, -100, -50, -100, -50, -40, -50, 60, 4, b, -105, 85, -85, 85, b, -85, 105, -85, 125, b, -105, 125, -125, 125, b, -125, 105, -125, 85, -105, 85", 's');
    list_append(generatedFont, (unitype) "Ű, 3, 4, -160, 60, b, -160, -40, -160, -100, b, -105, -100, -50, -100, -50, -40, -50, 60, 2, -135, 85, -120, 105, 2, -85, 85, -70, 105", 's');
    list_append(generatedFont, (unitype) "Ū, 2, 4, -160, 60, b, -160, -40, -160, -100, b, -105, -100, -50, -100, -50, -40, -50, 60, 2, -65, 95, -145, 95", 's');
    list_append(generatedFont, (unitype) "Ų, 2, 4, -160, 60, b, -160, -40, -160, -100, b, -105, -100, -50, -100, -50, -40, -50, 60, 2, b, -95, -100, -115, -105, b, -115, -125, -115, -155, -85, -145", 's');
    list_append(generatedFont, (unitype) "V, 1, 3, -160, 60, -100, -100, -40, 60", 's');
    list_append(generatedFont, (unitype) "W, 1, 5, -160, 60, -110, -100, -60, 60, -10, -100, 40, 60", 's');
    list_append(generatedFont, (unitype) "X, 2, 2, -160, -100, -50, 60, 2, -160, 60, -50, -100", 's');
    list_append(generatedFont, (unitype) "Y, 2, 3, -160, 60, -100, -30, -40, 60, 2, -100, -30, -100, -100", 's');
    list_append(generatedFont, (unitype) "Ý, 3, 3, -160, 60, -100, -30, -40, 60, 2, -100, -30, -100, -100, 2, -100, 85, -80, 105", 's');
    list_append(generatedFont, (unitype) "Z, 1, 4, -50, -100, -160, -100, -50, 60, -160, 60", 's');
    list_append(generatedFont, (unitype) "Ź, 2, 4, -50, -100, -160, -100, -50, 60, -160, 60, 2, -105, 85, -85, 105", 's');
    list_append(generatedFont, (unitype) "Ž, 2, 4, -50, -100, -160, -100, -50, 60, -160, 60, 3, -130, 105, -100, 85, -70, 105", 's');
    list_append(generatedFont, (unitype) "Ż, 2, 4, -50, -100, -160, -100, -50, 60, -160, 60, 1, -100, 95", 's');
    list_append(generatedFont, (unitype) "А, 2, 3, -160, -100, -100, 60, -40, -100, 2, -137, -40, -63, -40", 's');
    list_append(generatedFont, (unitype) "Б, 1, 6, -70, 60, -160, 60, -160, -100, b, -110, -100, -60, -100, b, -60, -60, -60, -20, -110, -20, -160, -20", 's');
    list_append(generatedFont, (unitype) "В, 2, 5, -160, -100, -160, 60, b, -110, 60, -60, 60, b, -60, 20, -60, -20, -110, -20, -160, -20, 3, b, -110, -20, -60, -20, b, -60, -60, -60, -100, -110, -100, -160, -100", 's');
    list_append(generatedFont, (unitype) "Г, 1, 3, -160, -100, -160, 60, -70, 60", 's');
    list_append(generatedFont, (unitype) "Ґ, 1, 4, -160, -100, -160, 60, -70, 60, -70, 80", 's');
    list_append(generatedFont, (unitype) "Ғ, 2, 3, -160, -100, -160, 60, -70, 60, 2, -180, -20, -120, -20", 's');
    list_append(generatedFont, (unitype) "Д, 2, 4, -160, -140, -160, -100, -20, -100, -20, -140, 3, -40, -100, -40, 60, b, -120, 60, -120, -75, -150, -100", 's');
    list_append(generatedFont, (unitype) "Ђ, 3, 2, -160, 60, -40, 60, 2, -105, -100, -105, 60, 3, b, -105, -30, -80, -20, b, -55, -20, -25, -20, b, -25, -55, -25, -100, -70, -100", 's');
    list_append(generatedFont, (unitype) "Е, 2, 4, -70, -100, -160, -100, -160, 60, -70, 60, 2, -160, -20, -80, -20", 's');
    list_append(generatedFont, (unitype) "Ё, 4, 4, -70, -100, -160, -100, -160, 60, -70, 60, 2, -160, -20, -80, -20, 1, -135, 90, 1, -95, 90", 's');
    list_append(generatedFont, (unitype) "Є, 2, 4, b, -50, 20, -60, 60, b, -100, 60, -160, 60, b, -160, -20, -160, -100, b, -100, -100, -60, -100, -50, -60, 2, -160, -20, -100, -20", 's');
    list_append(generatedFont, (unitype) "Ә, 1, 5, -30, -20, b, -160, -20, -160, -100, b, -95, -100, -30, -100, b, -30, -20, -30, 60, b, -95, 60, -125, 60, -140, 45", 's');
    list_append(generatedFont, (unitype) "Ж, 4, 4, -160, 60, -100, -20, -50, -20, 10, 60, 2, -160, -100, -100, -20, 2, -50, -20, 10, -100, 2, -75, -100, -75, 60", 's');
    list_append(generatedFont, (unitype) "Ӂ, 5, 4, -160, 60, -100, -20, -50, -20, 10, 60, 2, -160, -100, -100, -20, 2, -50, -20, 10, -100, 2, -75, -100, -75, 60, 2, b, -105, 105, -105, 85, b, -75, 85, -45, 85, -45, 105", 's');
    list_append(generatedFont, (unitype) "З, 1, 6, b, -160, 30, -145, 60, b, -110, 60, -55, 60, b, -55, 20, -55, -20, b, -115, -20, -55, -20, b, -55, -60, -55, -100, b, -110, -100, -145, -100, -160, -70", 's');
    list_append(generatedFont, (unitype) "И, 1, 4, -160, 60, -160, -100, -50, 60, -50, -100", 's');
    list_append(generatedFont, (unitype) "Ӣ, 2, 4, -160, 60, -160, -100, -50, 60, -50, -100, 2, -145, 85, -65, 85", 's');
    list_append(generatedFont, (unitype) "Й, 2, 4, -160, 60, -160, -100, -50, 60, -50, -100, 2, b, -135, 105, -135, 85, b, -105, 85, -75, 85, -75, 105", 's');
    list_append(generatedFont, (unitype) "І, 1, 2, -160, -100, -160, 60", 's');
    list_append(generatedFont, (unitype) "Ї, 3, 2, -160, -100, -160, 60, 1, -170, 100, 1, -150, 100", 's');
    list_append(generatedFont, (unitype) "Ј, 1, 3, -70, 60, b, -70, -50, -70, -100, b, -120, -100, -150, -100, -160, -70", 's');
    list_append(generatedFont, (unitype) "К, 3, 2, -160, -100, -160, 60, 3, -160, -20, -135, -20, -60, 60, 2, -134, -20, -60, -100", 's');
    list_append(generatedFont, (unitype) "Қ, 3, 2, -160, -100, -160, 60, 3, -160, -20, -135, -20, -60, 60, 4, -134, -20, -60, -100, -55, -100, -55, -125", 's');
    list_append(generatedFont, (unitype) "Ҝ, 4, 2, -160, -100, -160, 60, 3, -160, -20, -115, -20, -60, 60, 2, -115, -20, -60, -100, 2, -135, 15, -135, -55", 's');
    list_append(generatedFont, (unitype) "Л, 1, 4, b, -160, -100, -145, -100, -140, -75, -130, 60, -50, 60, -50, -100", 's');
    list_append(generatedFont, (unitype) "Љ, 1, 7, b, -160, -100, -145, -100, -140, -75, -130, 60, -50, 60, -50, -100, b, -10, -100, 40, -100, b, 40, -60, 40, -20, -10, -20, -50, -20", 's');
    list_append(generatedFont, (unitype) "М, 1, 5, -160, -100, -160, 60, -90, -100, -20, 60, -20, -100", 's');
    list_append(generatedFont, (unitype) "Н, 3, 2, -160, -100, -160, 60, 2, -160, -20, -50, -20, 2, -50, -100, -50, 60", 's');
    list_append(generatedFont, (unitype) "Ң, 3, 2, -160, -100, -160, 60, 2, -160, -20, -50, -20, 4, -50, 60, -50, -100, -45, -100, -45, -125", 's');
    list_append(generatedFont, (unitype) "Њ, 2, 2, -160, -100, -160, 60, 5, -160, -20, b, -10, -20, 40, -20, b, 40, -60, 40, -100, -10, -100, -50, -100, -50, 60", 's');
    list_append(generatedFont, (unitype) "О, 1, 4, b, -100, -100, -40, -100, b, -40, -20, -40, 60, b, -100, 60, -160, 60, b, -160, -20, -160, -100, -100, -100", 's');
    list_append(generatedFont, (unitype) "Ө, 1, 5, b, -40, -20, -40, 60, b, -100, 60, -160, 60, b, -160, -20, -160, -100, b, -100, -100, -40, -100, -40, -20, -160, -20", 's');
    list_append(generatedFont, (unitype) "П, 1, 4, -160, -100, -160, 60, -50, 60, -50, -100", 's');
    list_append(generatedFont, (unitype) "Р, 1, 5, -160, -100, -160, 60, b, -100, 60, -60, 60, b, -60, 20, -60, -20, -100, -20, -160, -20", 's');
    list_append(generatedFont, (unitype) "С, 1, 4, b, -50, 20, -60, 60, b, -100, 60, -160, 60, b, -160, -20, -160, -100, b, -100, -100, -60, -100, -50, -60", 's');
    list_append(generatedFont, (unitype) "Т, 2, 2, -100, -100, -100, 60, 2, -160, 60, -40, 60", 's');
    list_append(generatedFont, (unitype) "Ћ, 3, 2, -160, 60, -40, 60, 2, -105, 60, -105, -100, 3, b, -105, -30, -85, -20, b, -55, -20, -25, -20, -25, -55, -25, -100", 's');
    list_append(generatedFont, (unitype) "У, 2, 2, b, -145, -105, -130, -110, -120, -90, -50, 60, 2, -102, -50, -160, 60", 's');
    list_append(generatedFont, (unitype) "Ӯ, 3, 2, b, -145, -105, -130, -110, -120, -90, -50, 60, 2, -102, -50, -160, 60, 2, -145, 85, -65, 85", 's');
    list_append(generatedFont, (unitype) "Ү, 2, 3, -160, 60, -100, -30, -40, 60, 2, -100, -30, -100, -100", 's');
    list_append(generatedFont, (unitype) "Ұ, 3, 3, -160, 60, -100, -30, -40, 60, 2, -100, -30, -100, -100, 2, -150, -30, -50, -30", 's');
    list_append(generatedFont, (unitype) "Ў, 3, 2, b, -145, -105, -130, -110, -120, -90, -50, 60, 2, -102, -50, -160, 60, 2, b, -130, 105, -130, 85, b, -100, 85, -70, 85, -70, 105", 's');
    list_append(generatedFont, (unitype) "Ф, 2, 4, b, -90, 40, -160, 45, b, -160, -20, -160, -85, b, -90, -80, -20, -85, b, -20, -20, -20, 45, -90, 40, 2, -90, 60, -90, -100", 's');
    list_append(generatedFont, (unitype) "Х, 2, 2, -160, -100, -50, 60, 2, -160, 60, -50, -100", 's');
    list_append(generatedFont, (unitype) "Ҳ, 2, 2, -160, -100, -50, 60, 4, -160, 60, -50, -100, -45, -100, -45, -125", 's');
    list_append(generatedFont, (unitype) "Һ, 2, 2, -160, -100, -160, 40, 3, b, -160, -30, -140, -10, b, -110, -10, -60, -10, -60, -50, -60, -100", 's');
    list_append(generatedFont, (unitype) "Ц, 2, 4, -160, 60, -160, -100, -50, -100, -50, 60, 3, -50, -100, -45, -100, -45, -125", 's');
    list_append(generatedFont, (unitype) "Ч, 2, 2, -50, -100, -50, 60, 3, b, -50, -10, -70, -30, b, -110, -30, -160, -30, -160, 10, -160, 60", 's');
    list_append(generatedFont, (unitype) "Ҷ, 2, 3, -160, 60, b, -160, 10, -160, -30, b, -110, -30, -70, -30, -50, -10, 4, -50, 60, -50, -100, -45, -100, -45, -125", 's');
    list_append(generatedFont, (unitype) "Ҹ, 3, 2, -50, -100, -50, 60, 3, b, -50, -10, -70, -30, b, -110, -30, -160, -30, -160, 10, -160, 60, 2, -110, 0, -110, -60", 's');
    list_append(generatedFont, (unitype) "Џ, 2, 4, -160, 60, -160, -100, -50, -100, -50, 60, 2, -105, -100, -105, -125", 's');
    list_append(generatedFont, (unitype) "Ш, 2, 4, -160, 60, -160, -100, -85, -100, -85, 60, 3, -85, -100, -10, -100, -10, 60", 's');
    list_append(generatedFont, (unitype) "Щ, 3, 3, -160, 60, -160, -100, -10, -100, 2, -85, 60, -85, -100, 4, -10, 60, -10, -100, -5, -100, -5, -125", 's');
    list_append(generatedFont, (unitype) "Ъ, 1, 6, -160, 60, -135, 60, -135, -100, b, -85, -100, -35, -100, b, -35, -60, -35, -20, -85, -20, -135, -20", 's');
    list_append(generatedFont, (unitype) "Ы, 2, 5, -160, 60, -160, -100, b, -110, -100, -60, -100, b, -60, -60, -60, -20, -110, -20, -160, -20, 2, -30, -100, -30, 60", 's');
    list_append(generatedFont, (unitype) "Ь, 1, 5, -160, 60, -160, -100, b, -110, -100, -60, -100, b, -60, -60, -60, -20, -110, -20, -160, -20", 's');
    list_append(generatedFont, (unitype) "Э, 2, 4, b, -160, 20, -150, 60, b, -110, 60, -50, 60, b, -50, -20, -50, -100, b, -110, -100, -150, -100, -160, -60, 2, -50, -20, -110, -20", 's');
    list_append(generatedFont, (unitype) "Ю, 2, 2, -160, -100, -160, 60, 5, -160, -20, b, -120, -20, -120, -100, b, -60, -100, 0, -100, b, 0, -20, 0, 60, b, -60, 60, -120, 60, -120, -20", 's');
    list_append(generatedFont, (unitype) "Я, 2, 5, -60, -100, -60, 60, b, -120, 60, -160, 60, b, -160, 20, -160, -20, -120, -20, -60, -20, 2, -120, -20, -160, -100", 's');
    list_append(generatedFont, (unitype) "Α, 2, 3, -160, -100, -100, 60, -40, -100, 2, -137, -40, -63, -40", 's');
    list_append(generatedFont, (unitype) "Β, 2, 7, -160, -100, -160, 60, b, -110, 60, -60, 60, b, -60, 20, -60, -20, b, -110, -20, -60, -20, b, -60, -60, -60, -100, -110, -100, -160, -100, 2, -110, -20, -160, -20", 's');
    list_append(generatedFont, (unitype) "Γ, 1, 3, -160, -100, -160, 60, -70, 60", 's');
    list_append(generatedFont, (unitype) "Δ, 1, 4, -160, -100, -100, 60, -40, -100, -160, -100", 's');
    list_append(generatedFont, (unitype) "Ε, 2, 4, -70, -100, -160, -100, -160, 60, -70, 60, 2, -160, -20, -80, -20", 's');
    list_append(generatedFont, (unitype) "Ζ, 1, 4, -50, -100, -160, -100, -50, 60, -160, 60", 's');
    list_append(generatedFont, (unitype) "Η, 3, 2, -160, -100, -160, 60, 2, -160, -20, -50, -20, 2, -50, -100, -50, 60", 's');
    list_append(generatedFont, (unitype) "Θ, 2, 4, b, -100, -100, -40, -100, b, -40, -20, -40, 60, b, -100, 60, -160, 60, b, -160, -20, -160, -100, -100, -100, 2, -140, -20, -60, -20", 's');
    list_append(generatedFont, (unitype) "Ι, 1, 2, -160, -100, -160, 60", 's');
    list_append(generatedFont, (unitype) "Κ, 3, 2, -160, -100, -160, 60, 2, -160, -30, -60, 60, 2, -140, -12, -60, -100", 's');
    list_append(generatedFont, (unitype) "Λ, 1, 3, -160, -100, -100, 60, -40, -100", 's');
    list_append(generatedFont, (unitype) "Μ, 1, 5, -160, -100, -160, 60, -90, -100, -20, 60, -20, -100", 's');
    list_append(generatedFont, (unitype) "Ν, 1, 4, -160, -100, -160, 60, -50, -100, -50, 60", 's');
    list_append(generatedFont, (unitype) "Ξ, 3, 2, -60, -100, -160, -100, 2, -70, -20, -150, -20, 2, -60, 60, -160, 60", 's');
    list_append(generatedFont, (unitype) "Ο, 1, 4, b, -100, -100, -40, -100, b, -40, -20, -40, 60, b, -100, 60, -160, 60, b, -160, -20, -160, -100, -100, -100", 's');
    list_append(generatedFont, (unitype) "Π, 1, 4, -160, -100, -160, 60, -50, 60, -50, -100", 's');
    list_append(generatedFont, (unitype) "Ρ, 1, 5, -160, -100, -160, 60, b, -100, 60, -60, 60, b, -60, 20, -60, -20, -100, -20, -160, -20", 's');
    list_append(generatedFont, (unitype) "Σ, 1, 5, -60, 60, -160, 60, -100, -20, -160, -100, -60, -100", 's');
    list_append(generatedFont, (unitype) "Τ, 2, 2, -160, 60, -40, 60, 2, -100, 60, -100, -100", 's');
    list_append(generatedFont, (unitype) "Υ, 2, 3, -160, 60, -100, -30, -40, 60, 2, -100, -30, -100, -100", 's');
    list_append(generatedFont, (unitype) "Φ, 2, 2, -95, -100, -95, 60, 4, b, -95, -80, -30, -80, b, -30, -20, -30, 40, b, -95, 40, -160, 40, b, -160, -20, -160, -80, -95, -80", 's');
    list_append(generatedFont, (unitype) "Χ, 2, 2, -160, -100, -50, 60, 2, -160, 60, -50, -100", 's');
    list_append(generatedFont, (unitype) "Ψ, 2, 2, -100, -100, -100, 60, 2, b, -160, 60, -160, -30, b, -100, -30, -40, -30, -40, 60", 's');
    list_append(generatedFont, (unitype) "Ω, 1, 6, -160, -100, b, -115, -100, -160, -70, b, -160, -20, -160, 60, b, -100, 60, -40, 60, b, -40, -20, -40, -70, -85, -100, -40, -100", 's');
    list_append(generatedFont, (unitype) "a, 2, 3, b, -160, 0, -140, 20, b, -110, 20, -70, 20, -70, -10, -70, -100, 4, b, -70, -50, -90, -40, b, -120, -40, -160, -40, b, -160, -70, -160, -100, b, -120, -100, -80, -100, -70, -80", 's');
    list_append(generatedFont, (unitype) "à, 3, 3, b, -160, 0, -140, 20, b, -110, 20, -70, 20, -70, -10, -70, -100, 4, b, -70, -50, -90, -40, b, -120, -40, -160, -40, b, -160, -70, -160, -100, b, -120, -100, -80, -100, -70, -80, 2, -110, 45, -130, 65", 's');
    list_append(generatedFont, (unitype) "á, 3, 3, b, -160, 0, -140, 20, b, -110, 20, -70, 20, -70, -10, -70, -100, 4, b, -70, -50, -90, -40, b, -120, -40, -160, -40, b, -160, -70, -160, -100, b, -120, -100, -80, -100, -70, -80, 2, -110, 45, -90, 65", 's');
    list_append(generatedFont, (unitype) "â, 3, 3, b, -160, 0, -140, 20, b, -110, 20, -70, 20, -70, -10, -70, -100, 4, b, -70, -50, -90, -40, b, -120, -40, -160, -40, b, -160, -70, -160, -100, b, -120, -100, -80, -100, -70, -80, 3, -145, 45, -115, 65, -85, 45", 's');
    list_append(generatedFont, (unitype) "ă, 3, 3, b, -160, 0, -140, 20, b, -110, 20, -70, 20, -70, -10, -70, -100, 4, b, -70, -50, -90, -40, b, -120, -40, -160, -40, b, -160, -70, -160, -100, b, -120, -100, -80, -100, -70, -80, 2, b, -145, 65, -145, 45, b, -115, 45, -85, 45, -85, 65", 's');
    list_append(generatedFont, (unitype) "ä, 4, 3, b, -160, 0, -140, 20, b, -110, 20, -70, 20, -70, -10, -70, -100, 4, b, -70, -50, -90, -40, b, -120, -40, -160, -40, b, -160, -70, -160, -100, b, -120, -100, -80, -100, -70, -80, 1, -135, 55, 1, -95, 55", 's');
    list_append(generatedFont, (unitype) "ã, 3, 3, b, -160, 0, -140, 20, b, -110, 20, -70, 20, -70, -10, -70, -100, 4, b, -70, -50, -90, -40, b, -120, -40, -160, -40, b, -160, -70, -160, -100, b, -120, -100, -80, -100, -70, -80, 4, b, -150, 45, -150, 65, b, -135, 65, -125, 65, b, -115, 55, -105, 45, b, -95, 45, -80, 45, -80, 65", 's');
    list_append(generatedFont, (unitype) "å, 3, 3, b, -160, 0, -140, 20, b, -110, 20, -70, 20, -70, -10, -70, -100, 4, b, -70, -50, -90, -40, b, -120, -40, -160, -40, b, -160, -70, -160, -100, b, -120, -100, -80, -100, -70, -80, 4, b, -115, 45, -95, 45, b, -95, 65, -95, 85, b, -115, 85, -135, 85, b, -135, 65, -135, 45, -115, 45", 's');
    list_append(generatedFont, (unitype) "ā, 3, 3, b, -160, 0, -140, 20, b, -110, 20, -70, 20, -70, -10, -70, -100, 4, b, -70, -50, -90, -40, b, -120, -40, -160, -40, b, -160, -70, -160, -100, b, -120, -100, -80, -100, -70, -80, 2, -145, 55, -85, 55", 's');
    list_append(generatedFont, (unitype) "ą, 3, 3, b, -160, 0, -140, 20, b, -110, 20, -70, 20, -70, -10, -70, -100, 4, b, -70, -50, -90, -40, b, -120, -40, -160, -40, b, -160, -70, -160, -100, b, -120, -100, -80, -100, -70, -80, 2, b, -70, -100, -95, -105, b, -95, -125, -95, -150, -70, -145", 's');
    list_append(generatedFont, (unitype) "æ, 4, 3, b, -160, 0, -140, 20, b, -115, 20, -85, 20, -80, -10, -80, -80, 4, b, -80, -50, -90, -40, b, -125, -40, -160, -40, b, -160, -70, -160, -100, b, -125, -100, -90, -100, -80, -80, 3, -80, -40, b, 0, -40, 0, 20, b, -40, 20, -70, 20, -80, -10, 2, b, -80, -40, -85, -100, b, -35, -100, -20, -100, 0, -85", 's');
    list_append(generatedFont, (unitype) "b, 2, 2, -160, -100, -160, 60, 4, b, -160, -80, -150, -100, b, -115, -100, -70, -100, b, -70, -40, -70, 20, b, -115, 20, -150, 20, -160, 0", 's');
    list_append(generatedFont, (unitype) "c, 1, 4, b, -70, -10, -80, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -80, -100, -70, -70", 's');
    list_append(generatedFont, (unitype) "ć, 2, 4, b, -70, -10, -80, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -80, -100, -70, -70, 2, -110, 45, -90, 65", 's');
    list_append(generatedFont, (unitype) "č, 2, 4, b, -70, -10, -80, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -80, -100, -70, -70, 3, -145, 65, -115, 45, -85, 65", 's');
    list_append(generatedFont, (unitype) "ċ, 2, 4, b, -70, -10, -80, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -80, -100, -70, -70, 1, -115, 55", 's');
    list_append(generatedFont, (unitype) "ç, 2, 4, b, -70, -10, -80, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -80, -100, -70, -70, 3, -115, -100, b, -117, -115, -95, -120, b, -95, -135, -95, -160, -120, -155", 's');
    list_append(generatedFont, (unitype) "d, 2, 2, -70, -100, -70, 60, 4, b, -70, 0, -80, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -80, -100, -70, -80", 's');
    list_append(generatedFont, (unitype) "ď, 3, 2, -70, -100, -70, 60, 4, b, -70, 0, -80, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -80, -100, -70, -80, 1, b, -45, 30, -40, 40, -40, 60", 's');
    list_append(generatedFont, (unitype) "đ, 3, 2, -70, -100, -70, 60, 4, b, -70, 0, -80, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -80, -100, -70, -80, 2, -60, 50, -105, 50", 's');
    list_append(generatedFont, (unitype) "ð, 2, 5, b, -70, -45, -70, 5, b, -115, 5, -160, 5, b, -160, -45, -160, -100, b, -115, -100, -70, -100, b, -70, -45, -70, 30, -120, 60, 2, -80, 60, -120, 30", 's');
    list_append(generatedFont, (unitype) "e, 1, 5, -160, -40, b, -70, -40, -70, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -90, -100, -70, -85", 's');
    list_append(generatedFont, (unitype) "è, 2, 5, -160, -40, b, -70, -40, -70, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -90, -100, -70, -85, 2, -115, 45, -135, 65", 's');
    list_append(generatedFont, (unitype) "é, 2, 5, -160, -40, b, -70, -40, -70, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -90, -100, -70, -85, 2, -115, 45, -95, 65", 's');
    list_append(generatedFont, (unitype) "ě, 2, 5, -160, -40, b, -70, -40, -70, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -90, -100, -70, -85, 3, -145, 65, -115, 45, -85, 65", 's');
    list_append(generatedFont, (unitype) "ê, 2, 5, -160, -40, b, -70, -40, -70, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -90, -100, -70, -85, 3, -145, 45, -115, 65, -85, 45", 's');
    list_append(generatedFont, (unitype) "ë, 3, 5, -160, -40, b, -70, -40, -70, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -90, -100, -70, -85, 1, -135, 55, 1, -95, 55", 's');
    list_append(generatedFont, (unitype) "ē, 2, 5, -160, -40, b, -70, -40, -70, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -90, -100, -70, -85, 2, -145, 55, -85, 55", 's');
    list_append(generatedFont, (unitype) "ė, 2, 5, -160, -40, b, -70, -40, -70, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -90, -100, -70, -85, 1, -115, 55", 's');
    list_append(generatedFont, (unitype) "ę, 2, 5, -160, -40, b, -70, -40, -70, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -90, -100, -70, -85, 2, b, -90, -95, -120, -105, b, -120, -130, -120, -155, -90, -145", 's');
    list_append(generatedFont, (unitype) "ə, 1, 5, -70, -40, b, -160, -40, -160, -100, b, -115, -100, -70, -100, b, -70, -40, -70, 20, b, -115, 20, -140, 20, -160, 5", 's');
    list_append(generatedFont, (unitype) "f, 2, 2, -140, -100, b, -140, 40, -140, 60, -110, 55, 2, -160, 0, -115, 0", 's');
    list_append(generatedFont, (unitype) "g, 2, 3, -70, 20, b, -70, -120, -70, -160, b, -110, -160, -140, -160, -160, -140, 4, b, -70, 0, -80, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -80, -100, -70, -80", 's');
    list_append(generatedFont, (unitype) "ğ, 3, 3, -70, 20, b, -70, -120, -70, -160, b, -110, -160, -140, -160, -160, -140, 4, b, -70, 0, -80, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -80, -100, -70, -80, 2, b, -145, 65, -145, 45, b, -115, 45, -85, 45, -85, 65", 's');
    list_append(generatedFont, (unitype) "ġ, 3, 3, -70, 20, b, -70, -120, -70, -160, b, -110, -160, -140, -160, -160, -140, 4, b, -70, 0, -80, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -80, -100, -70, -80, 1, -115, 55", 's');
    list_append(generatedFont, (unitype) "h, 2, 2, -160, 60, -160, -100, 3, b, -160, -10, -150, 20, b, -115, 20, -70, 20, -70, -30, -70, -100", 's');
    list_append(generatedFont, (unitype) "ħ, 3, 2, -160, 60, -160, -100, 3, b, -160, -10, -150, 20, b, -115, 20, -70, 20, -70, -30, -70, -100, 2, -170, 45, -125, 45", 's');
    list_append(generatedFont, (unitype) "i, 2, 2, -160, -100, -160, 10, 1, -160, 50", 's');
    list_append(generatedFont, (unitype) "ì, 2, 2, -160, -100, -160, 10, 2, -160, 50, -170, 70", 's');
    list_append(generatedFont, (unitype) "í, 2, 2, -160, -100, -160, 10, 2, -160, 50, -150, 70", 's');
    list_append(generatedFont, (unitype) "î, 2, 2, -160, -100, -160, 10, 3, -180, 50, -160, 70, -140, 50", 's');
    list_append(generatedFont, (unitype) "ï, 3, 2, -160, -100, -160, 10, 1, -175, 50, 1, -145, 50", 's');
    list_append(generatedFont, (unitype) "ī, 2, 2, -160, -100, -160, 10, 2, -180, 50, -140, 50", 's');
    list_append(generatedFont, (unitype) "ı, 1, 2, -160, -100, -160, 10", 's');
    list_append(generatedFont, (unitype) "į, 2, 1, -160, 50, 3, -160, 10, b, -160, -100, -170, -105, b, -170, -115, -170, -125, -160, -125", 's');
    list_append(generatedFont, (unitype) "j, 2, 3, b, -180, -140, -170, -160, b, -150, -160, -120, -160, -120, -120, -120, 10, 1, -120, 50", 's');
    list_append(generatedFont, (unitype) "k, 3, 2, -160, -100, -160, 60, 2, -160, -60, -90, 10, 2, -140, -40, -90, -100", 's');
    list_append(generatedFont, (unitype) "ķ, 4, 2, -160, -100, -160, 60, 2, -160, -60, -90, 10, 2, -140, -40, -90, -100, 1, b, -130, -100, -130, -115, -140, -120", 's');
    list_append(generatedFont, (unitype) "l, 1, 2, -160, -100, -160, 60", 's');
    list_append(generatedFont, (unitype) "ĺ, 2, 2, -160, -100, -160, 60, 2, -160, 90, -150, 110", 's');
    list_append(generatedFont, (unitype) "ľ, 2, 2, -160, -100, -160, 60, 1, b, -135, 50, -130, 60, -130, 80", 's');
    list_append(generatedFont, (unitype) "ļ, 2, 2, -160, -100, -160, 60, 1, b, -160, -120, -160, -140, -165, -150", 's');
    list_append(generatedFont, (unitype) "ł, 2, 2, -160, -100, -160, 60, 2, -180, -25, -140, -5", 's');
    list_append(generatedFont, (unitype) "ŀ, 2, 2, -160, -100, -160, 60, 1, -135, -20", 's');
    list_append(generatedFont, (unitype) "m, 3, 2, -160, -100, -160, 20, 3, b, -160, 0, -150, 20, b, -120, 20, -80, 20, -80, -20, -80, -100, 3, b, -80, -20, -80, 20, b, -40, 20, 0, 20, 0, -20, 0, -100", 's');
    list_append(generatedFont, (unitype) "n, 2, 2, -160, -100, -160, 20, 3, b, -160, 0, -150, 20, b, -115, 20, -70, 20, -70, -20, -70, -100", 's');
    list_append(generatedFont, (unitype) "ń, 3, 2, -160, -100, -160, 20, 3, b, -160, 0, -150, 20, b, -115, 20, -70, 20, -70, -20, -70, -100, 2, -115, 45, -95, 65", 's');
    list_append(generatedFont, (unitype) "ň, 3, 2, -160, -100, -160, 20, 3, b, -160, 0, -150, 20, b, -115, 20, -70, 20, -70, -20, -70, -100, 3, -145, 65, -115, 45, -85, 65", 's');
    list_append(generatedFont, (unitype) "ñ, 3, 2, -160, -100, -160, 20, 3, b, -160, 0, -150, 20, b, -115, 20, -70, 20, -70, -20, -70, -100, 4, b, -150, 45, -150, 65, b, -135, 65, -125, 65, b, -115, 55, -105, 45, b, -95, 45, -80, 45, -80, 65", 's');
    list_append(generatedFont, (unitype) "ņ, 3, 2, -160, -100, -160, 20, 3, b, -160, 0, -150, 20, b, -115, 20, -70, 20, -70, -20, -70, -100, 1, b, -115, -100, -115, -120, -120, -130", 's');
    list_append(generatedFont, (unitype) "o, 1, 4, b, -115, -100, -70, -100, b, -70, -40, -70, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, -115, -100", 's');
    list_append(generatedFont, (unitype) "ò, 2, 4, b, -115, -100, -70, -100, b, -70, -40, -70, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, -115, -100, 2, -115, 45, -135, 65", 's');
    list_append(generatedFont, (unitype) "ó, 2, 4, b, -115, -100, -70, -100, b, -70, -40, -70, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, -115, -100, 2, -115, 45, -95, 65", 's');
    list_append(generatedFont, (unitype) "ô, 2, 4, b, -115, -100, -70, -100, b, -70, -40, -70, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, -115, -100, 3, -145, 45, -115, 65, -85, 45", 's');
    list_append(generatedFont, (unitype) "ö, 3, 4, b, -115, -100, -70, -100, b, -70, -40, -70, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, -115, -100, 1, -135, 55, 1, -95, 55", 's');
    list_append(generatedFont, (unitype) "õ, 2, 4, b, -115, -100, -70, -100, b, -70, -40, -70, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, -115, -100, 4, b, -150, 45, -150, 65, b, -135, 65, -125, 65, b, -115, 55, -105, 45, b, -95, 45, -80, 45, -80, 65", 's');
    list_append(generatedFont, (unitype) "ő, 3, 4, b, -115, -100, -70, -100, b, -70, -40, -70, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, -115, -100, 2, -140, 45, -120, 65, 2, -100, 45, -80, 65", 's');
    list_append(generatedFont, (unitype) "ø, 2, 4, b, -115, -100, -70, -100, b, -70, -40, -70, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, -115, -100, 2, -160, -100, -70, 20", 's');
    list_append(generatedFont, (unitype) "œ, 1, 8, b, -70, -40, -70, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -70, -100, -70, -40, b, 10, -40, 10, 20, b, -30, 20, -60, 20, -70, -10, b, -70, -40, -75, -100, b, -30, -100, -10, -100, 10, -85", 's');
    list_append(generatedFont, (unitype) "p, 2, 2, -160, -160, -160, 20, 4, b, -160, 0, -150, 20, b, -115, 20, -70, 20, b, -70, -40, -70, -100, b, -115, -100, -150, -100, -160, -80", 's');
    list_append(generatedFont, (unitype) "q, 2, 2, -70, 20, -70, -160, 4, b, -70, 0, -80, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -80, -100, -70, -80", 's');
    list_append(generatedFont, (unitype) "r, 2, 2, -160, -100, -160, 20, 2, b, -160, 0, -150, 20, b, -110, 20, -90, 20, -80, 10", 's');
    list_append(generatedFont, (unitype) "ŕ, 3, 2, -160, -100, -160, 20, 2, b, -160, 0, -150, 20, b, -110, 20, -90, 20, -80, 10, 2, -120, 45, -100, 65", 's');
    list_append(generatedFont, (unitype) "ř, 3, 2, -160, -100, -160, 20, 2, b, -160, 0, -150, 20, b, -110, 20, -90, 20, -80, 10, 3, -145, 65, -115, 45, -85, 65", 's');
    list_append(generatedFont, (unitype) "s, 1, 6, b, -70, 0, -80, 20, b, -115, 20, -160, 20, b, -160, -10, -160, -40, b, -115, -40, -70, -40, b, -70, -70, -70, -100, b, -115, -100, -150, -100, -160, -80", 's');
    list_append(generatedFont, (unitype) "ś, 2, 6, b, -70, 0, -80, 20, b, -115, 20, -160, 20, b, -160, -10, -160, -40, b, -115, -40, -70, -40, b, -70, -70, -70, -100, b, -115, -100, -150, -100, -160, -80, 2, -115, 45, -95, 65", 's');
    list_append(generatedFont, (unitype) "š, 2, 6, b, -70, 0, -80, 20, b, -115, 20, -160, 20, b, -160, -10, -160, -40, b, -115, -40, -70, -40, b, -70, -70, -70, -100, b, -115, -100, -150, -100, -160, -80, 3, -145, 65, -115, 45, -85, 65", 's');
    list_append(generatedFont, (unitype) "ş, 2, 6, b, -70, 0, -80, 20, b, -115, 20, -160, 20, b, -160, -10, -160, -40, b, -115, -40, -70, -40, b, -70, -70, -70, -100, b, -115, -100, -150, -100, -160, -80, 3, -115, -100, b, -117, -115, -95, -120, b, -95, -135, -95, -160, -120, -155", 's');
    list_append(generatedFont, (unitype) "ș, 2, 6, b, -70, 0, -80, 20, b, -115, 20, -160, 20, b, -160, -10, -160, -40, b, -115, -40, -70, -40, b, -70, -70, -70, -100, b, -115, -100, -150, -100, -160, -80, 1, b, -115, -110, -115, -125, -120, -140", 's');
    list_append(generatedFont, (unitype) "ß, 1, 7, -160, -100, b, -160, 15, -160, 60, b, -125, 60, -90, 60, b, -90, 25, -90, 5, b, -110, -10, -130, -25, b, -90, -45, -65, -55, b, -75, -85, -90, -110, -135, -95", 's');
    list_append(generatedFont, (unitype) "t, 2, 2, -140, 50, b, -140, -70, -140, -100, -110, -100, 2, -160, 20, -120, 20", 's');
    list_append(generatedFont, (unitype) "ť, 3, 2, -140, 50, b, -140, -70, -140, -100, -110, -100, 2, -160, 20, -120, 20, 1, b, -95, 20, -90, 30, -90, 50", 's');
    list_append(generatedFont, (unitype) "ț, 3, 2, -140, 50, b, -140, -70, -140, -100, -110, -100, 2, -160, 20, -120, 20, 1, b, -130, -110, -130, -125, -135, -140", 's');
    list_append(generatedFont, (unitype) "þ, 2, 2, -160, -140, -160, 60, 4, b, -160, 0, -150, 20, b, -115, 20, -70, 20, b, -70, -40, -70, -100, b, -115, -100, -150, -100, -160, -80", 's');
    list_append(generatedFont, (unitype) "u, 2, 2, -70, 20, -70, -100, 3, b, -70, -80, -80, -100, b, -115, -100, -160, -100, -160, -60, -160, 20", 's');
    list_append(generatedFont, (unitype) "ù, 3, 2, -70, 20, -70, -100, 3, b, -70, -80, -80, -100, b, -115, -100, -160, -100, -160, -60, -160, 20, 2, -115, 45, -135, 65", 's');
    list_append(generatedFont, (unitype) "ú, 3, 2, -70, 20, -70, -100, 3, b, -70, -80, -80, -100, b, -115, -100, -160, -100, -160, -60, -160, 20, 2, -115, 45, -95, 65", 's');
    list_append(generatedFont, (unitype) "û, 3, 2, -70, 20, -70, -100, 3, b, -70, -80, -80, -100, b, -115, -100, -160, -100, -160, -60, -160, 20, 3, -145, 45, -115, 65, -85, 45", 's');
    list_append(generatedFont, (unitype) "ü, 4, 2, -70, 20, -70, -100, 3, b, -70, -80, -80, -100, b, -115, -100, -160, -100, -160, -60, -160, 20, 1, -135, 55, 1, -95, 55", 's');
    list_append(generatedFont, (unitype) "ů, 3, 2, -70, 20, -70, -100, 3, b, -70, -80, -80, -100, b, -115, -100, -160, -100, -160, -60, -160, 20, 4, b, -115, 45, -95, 45, b, -95, 65, -95, 85, b, -115, 85, -135, 85, b, -135, 65, -135, 45, -115, 45", 's');
    list_append(generatedFont, (unitype) "ű, 4, 2, -70, 20, -70, -100, 3, b, -70, -80, -80, -100, b, -115, -100, -160, -100, -160, -60, -160, 20, 2, -100, 45, -80, 65, 2, -140, 45, -120, 65", 's');
    list_append(generatedFont, (unitype) "ū, 3, 2, -70, 20, -70, -100, 3, b, -70, -80, -80, -100, b, -115, -100, -160, -100, -160, -60, -160, 20, 2, -145, 55, -85, 55", 's');
    list_append(generatedFont, (unitype) "ų, 1, 6, -160, 20, b, -160, -60, -160, -100, b, -115, -100, -80, -100, -70, -80, -70, 20, b, -70, -100, -95, -105, b, -95, -125, -95, -150, -70, -145", 's');
    list_append(generatedFont, (unitype) "v, 1, 3, -160, 20, -115, -100, -70, 20", 's');
    list_append(generatedFont, (unitype) "w, 1, 5, -160, 20, -120, -100, -80, 20, -40, -100, 0, 20", 's');
    list_append(generatedFont, (unitype) "x, 2, 2, -160, 20, -70, -100, 2, -160, -100, -70, 20", 's');
    list_append(generatedFont, (unitype) "y, 2, 2, -70, 20, b, -130, -140, -140, -160, -160, -150, 2, -115, -99, -160, 20", 's');
    list_append(generatedFont, (unitype) "ý, 3, 2, -70, 20, b, -130, -140, -140, -160, -160, -150, 2, -115, -99, -160, 20, 2, -115, 45, -95, 65", 's');
    list_append(generatedFont, (unitype) "z, 1, 4, -70, -100, -160, -100, -70, 20, -160, 20", 's');
    list_append(generatedFont, (unitype) "ź, 2, 4, -70, -100, -160, -100, -70, 20, -160, 20, 2, -115, 45, -95, 65", 's');
    list_append(generatedFont, (unitype) "ž, 2, 4, -70, -100, -160, -100, -70, 20, -160, 20, 3, -145, 65, -115, 45, -85, 65", 's');
    list_append(generatedFont, (unitype) "ż, 2, 4, -70, -100, -160, -100, -70, 20, -160, 20, 1, -115, 55", 's');
    list_append(generatedFont, (unitype) "а, 2, 3, b, -160, 0, -140, 20, b, -110, 20, -70, 20, -70, -10, -70, -100, 4, b, -70, -50, -90, -40, b, -120, -40, -160, -40, b, -160, -70, -160, -100, b, -120, -100, -80, -100, -70, -80", 's');
    list_append(generatedFont, (unitype) "б, 1, 6, b, -160, -45, -160, 5, b, -115, 5, -70, 5, b, -70, -45, -70, -100, b, -115, -100, -160, -100, b, -160, -45, -160, 45, b, -105, 45, -85, 45, -80, 60", 's');
    list_append(generatedFont, (unitype) "в, 2, 5, -160, -100, b, -160, 20, -130, 20, b, -110, 20, -75, 20, b, -75, -10, -75, -40, -120, -40, -160, -40, 3, b, -120, -40, -75, -40, b, -75, -70, -75, -100, -110, -100, -160, -100", 's');
    list_append(generatedFont, (unitype) "г, 1, 3, -160, -100, -160, 20, -95, 20", 's');
    list_append(generatedFont, (unitype) "ґ, 1, 4, -160, -100, -160, 20, -95, 20, -95, 40", 's');
    list_append(generatedFont, (unitype) "ғ, 2, 3, -150, -100, -150, 20, -85, 20, 2, -160, -30, -130, -30", 's');
    list_append(generatedFont, (unitype) "д, 2, 4, -160, -135, -160, -100, -50, -100, -50, -135, 3, -65, -100, -65, 20, b, -135, 20, -135, -50, -145, -100", 's');
    list_append(generatedFont, (unitype) "ђ, 3, 2, -160, -100, -160, 60, 2, -180, 40, -120, 40, 3, b, -160, -10, -150, 20, b, -115, 20, -70, 20, -70, -30, b, -70, -100, -70, -130, -95, -125", 's');
    list_append(generatedFont, (unitype) "е, 1, 5, -160, -40, b, -70, -40, -70, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -90, -100, -70, -85", 's');
    list_append(generatedFont, (unitype) "ё, 3, 5, -160, -40, b, -70, -40, -70, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -90, -100, -70, -85, 1, -135, 55, 1, -95, 55", 's');
    list_append(generatedFont, (unitype) "є, 2, 4, b, -70, -10, -80, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -80, -100, -70, -70, 2, -160, -40, -110, -40", 's');
    list_append(generatedFont, (unitype) "ә, 1, 5, -70, -40, b, -160, -40, -160, -100, b, -115, -100, -70, -100, b, -70, -40, -70, 20, b, -115, 20, -140, 20, -160, 5", 's');
    list_append(generatedFont, (unitype) "ж, 4, 4, -160, -100, -115, -40, -75, -40, -30, -100, 2, -160, 20, -115, -40, 2, -75, -40, -30, 20, 2, -95, 20, -95, -100", 's');
    list_append(generatedFont, (unitype) "ӂ, 5, 4, -160, -100, -115, -40, -75, -40, -30, -100, 2, -160, 20, -115, -40, 2, -75, -40, -30, 20, 2, -95, 20, -95, -100, 2, b, -125, 65, -125, 45, b, -95, 45, -65, 45, -65, 65", 's');
    list_append(generatedFont, (unitype) "з, 1, 6, b, -160, -5, -155, 20, b, -125, 20, -85, 20, b, -85, -10, -85, -40, b, -120, -40, -85, -40, b, -85, -70, -85, -100, b, -125, -100, -155, -100, -160, -75", 's');
    list_append(generatedFont, (unitype) "и, 1, 4, -160, 20, -160, -100, -70, 20, -70, -100", 's');
    list_append(generatedFont, (unitype) "ӣ, 2, 4, -160, 20, -160, -100, -70, 20, -70, -100, 2, -145, 55, -85, 55", 's');
    list_append(generatedFont, (unitype) "й, 2, 4, -160, 20, -160, -100, -70, 20, -70, -100, 2, b, -145, 65, -145, 45, b, -115, 45, -85, 45, -85, 65", 's');
    list_append(generatedFont, (unitype) "і, 2, 2, -160, -100, -160, 10, 1, -160, 50", 's');
    list_append(generatedFont, (unitype) "ї, 3, 2, -160, -100, -160, 10, 1, -175, 50, 1, -145, 50", 's');
    list_append(generatedFont, (unitype) "ј, 2, 1, -120, 50, 3, -120, 10, b, -120, -120, -120, -160, b, -150, -160, -170, -160, -180, -140", 's');
    list_append(generatedFont, (unitype) "к, 3, 2, -160, -100, -160, 20, 3, -160, -40, -140, -40, -85, 20, 2, -140, -40, -85, -100", 's');
    list_append(generatedFont, (unitype) "қ, 3, 2, -160, -100, -160, 20, 3, -160, -40, -140, -40, -85, 20, 4, -140, -40, -85, -100, -80, -100, -80, -125", 's');
    list_append(generatedFont, (unitype) "ҝ, 4, 2, -160, -100, -160, 20, 3, -160, -40, -125, -40, -85, 20, 2, -125, -40, -85, -100, 2, -140, -15, -140, -65", 's');
    list_append(generatedFont, (unitype) "л, 1, 4, b, -160, -100, -145, -100, -140, -80, -130, 20, -70, 20, -70, -100", 's');
    list_append(generatedFont, (unitype) "љ, 1, 7, b, -160, -100, -145, -100, -140, -80, -130, 20, -70, 20, -70, -100, b, -30, -100, 5, -100, b, 5, -70, 5, -40, -30, -40, -70, -40", 's');
    list_append(generatedFont, (unitype) "м, 1, 5, -160, -100, -160, 20, -100, -100, -40, 20, -40, -100", 's');
    list_append(generatedFont, (unitype) "н, 3, 2, -160, -100, -160, 20, 2, -160, -40, -70, -40, 2, -70, -100, -70, 20", 's');
    list_append(generatedFont, (unitype) "ң, 3, 2, -160, -100, -160, 20, 2, -160, -40, -70, -40, 4, -70, 20, -70, -100, -65, -100, -65, -125", 's');
    list_append(generatedFont, (unitype) "њ, 2, 2, -160, -100, -160, 20, 5, -160, -40, b, -30, -40, 5, -40, b, 5, -70, 5, -100, -30, -100, -70, -100, -70, 20", 's');
    list_append(generatedFont, (unitype) "о, 1, 4, b, -115, -100, -70, -100, b, -70, -40, -70, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, -115, -100", 's');
    list_append(generatedFont, (unitype) "ө, 1, 5, b, -70, -40, -70, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -70, -100, -70, -40, -160, -40", 's');
    list_append(generatedFont, (unitype) "п, 1, 4, -160, -100, -160, 20, -70, 20, -70, -100", 's');
    list_append(generatedFont, (unitype) "р, 2, 2, -160, -160, -160, 20, 4, b, -160, 0, -150, 20, b, -115, 20, -70, 20, b, -70, -40, -70, -100, b, -115, -100, -150, -100, -160, -80", 's');
    list_append(generatedFont, (unitype) "с, 1, 4, b, -70, -10, -80, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -80, -100, -70, -70", 's');
    list_append(generatedFont, (unitype) "т, 2, 2, -160, 20, -70, 20, 2, -115, 20, -115, -100", 's');
    list_append(generatedFont, (unitype) "ћ, 3, 2, -160, 60, -160, -100, 3, b, -160, -10, -150, 20, b, -115, 20, -70, 20, -70, -30, -70, -100, 2, -170, 45, -125, 45", 's');
    list_append(generatedFont, (unitype) "у, 2, 2, -160, 20, -115, -100, 2, -70, 20, b, -130, -140, -140, -160, -160, -150", 's');
    list_append(generatedFont, (unitype) "ӯ, 3, 2, -160, 20, -115, -100, 2, -70, 20, b, -130, -140, -140, -160, -160, -150, 2, -145, 55, -85, 55", 's');
    list_append(generatedFont, (unitype) "ү, 2, 3, -160, 0, -115, -100, -115, -155, 2, -115, -100, -70, 0", 's');
    list_append(generatedFont, (unitype) "ұ, 3, 3, -160, 0, -115, -100, -115, -155, 2, -115, -100, -70, 0, 2, -150, -100, -80, -100", 's');
    list_append(generatedFont, (unitype) "ў, 3, 2, -160, 20, -115, -100, 2, -70, 20, b, -130, -140, -140, -160, -160, -150, 2, b, -145, 65, -145, 45, b, -115, 45, -85, 45, -85, 65", 's');
    list_append(generatedFont, (unitype) "ф, 2, 2, -100, -140, -100, 60, 4, b, -100, 10, -40, 40, b, -40, -40, -40, -120, b, -100, -90, -160, -120, b, -160, -40, -160, 40, -100, 10", 's');
    list_append(generatedFont, (unitype) "х, 2, 2, -160, -100, -70, 20, 2, -160, 20, -70, -100", 's');
    list_append(generatedFont, (unitype) "ҳ, 2, 2, -160, -100, -70, 20, 4, -160, 20, -70, -100, -65, -100, -65, -", 's');
    list_append(generatedFont, (unitype) "һ, 2, 2, -160, 60, -160, -100, 3, b, -160, -10, -150, 20, b, -115, 20, -70, 20, -70, -30, -70, -100", 's');
    list_append(generatedFont, (unitype) "ц, 2, 4, -160, 20, -160, -100, -70, -100, -70, 20, 3, -70, -100, -65, -100, -65, -125", 's');
    list_append(generatedFont, (unitype) "ч, 2, 2, -80, -100, -80, 20, 3, b, -80, -40, -95, -50, b, -120, -50, -160, -50, -160, -15, -160, 20", 's');
    list_append(generatedFont, (unitype) "ҷ, 2, 3, -160, 20, b, -160, -15, -160, -50, b, -120, -50, -95, -50, -80, -40, 4, -80, 20, -80, -100, -75, -100, -75, -125", 's');
    list_append(generatedFont, (unitype) "ҹ, 3, 2, -80, -100, -80, 20, 3, b, -80, -40, -95, -50, b, -120, -50, -160, -50, -160, -15, -160, 20, 2, -120, -20, -120, -80", 's');
    list_append(generatedFont, (unitype) "џ, 2, 4, -160, 20, -160, -100, -70, -100, -70, 20, 2, -115, -100, -115, -125", 's');
    list_append(generatedFont, (unitype) "ш, 3, 3, -160, 20, -160, -100, -20, -100, 2, -90, 20, -90, -100, 2, -20, 20, -20, -100", 's');
    list_append(generatedFont, (unitype) "щ, 3, 3, -160, 20, -160, -100, -20, -100, 2, -90, 20, -90, -100, 4, -20, 20, -20, -100, -15, -100, -15, -125", 's');
    list_append(generatedFont, (unitype) "ъ, 1, 6, -160, 20, -140, 20, -140, -100, b, -100, -100, -60, -100, b, -60, -70, -60, -40, -100, -40, -140, -40", 's');
    list_append(generatedFont, (unitype) "ы, 2, 5, -160, 20, -160, -100, b, -120, -100, -80, -100, b, -80, -70, -80, -40, -120, -40, -160, -40, 2, -55, -100, -55, 20", 's');
    list_append(generatedFont, (unitype) "ь, 1, 5, -160, 20, -160, -100, b, -120, -100, -80, -100, b, -80, -70, -80, -40, -120, -40, -160, -40", 's');
    list_append(generatedFont, (unitype) "э, 2, 4, b, -160, -10, -150, 20, b, -115, 20, -70, 20, b, -70, -40, -70, -100, b, -115, -100, -150, -100, -160, -70, 2, -70, -40, -120, -40", 's');
    list_append(generatedFont, (unitype) "ю, 2, 2, -160, 20, -160, -100, 5, -160, -40, b, -120, -40, -120, -100, b, -75, -100, -30, -100, b, -30, -40, -30, 20, b, -75, 20, -120, 20, -120, -40", 's');
    list_append(generatedFont, (unitype) "я, 2, 5, -70, -100, -70, 20, b, -120, 20, -160, 20, b, -160, -10, -160, -40, -130, -40, -70, -40, 2, -125, -40, -160, -100", 's');
    list_append(generatedFont, (unitype) "α, 2, 2, -70, 20, b, -70, -80, -70, -100, -50, -100, 4, b, -70, -10, -80, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -70, -100, -70, -70", 's');
    list_append(generatedFont, (unitype) "β, 1, 7, -160, -135, b, -160, 0, -160, 40, b, -120, 40, -85, 40, b, -85, 10, -85, -30, b, -130, -30, -75, -30, b, -75, -60, -75, -100, b, -115, -100, -140, -100, -160, -80", 's');
    list_append(generatedFont, (unitype) "γ, 2, 3, -160, 0, -115, -100, -70, 0, 2, -115, -100, -115, -155", 's');
    list_append(generatedFont, (unitype) "δ, 2, 4, b, -70, -50, -70, 0, b, -115, 0, -160, 0, b, -160, -50, -160, -100, b, -115, -100, -70, -100, -70, -50, 2, b, -92, -4, -145, 10, b, -145, 30, -150, 70, -95, 55", 's');
    list_append(generatedFont, (unitype) "ε, 1, 6, b, -70, -5, -75, 20, b, -115, 20, -160, 20, b, -160, -15, -160, -40, b, -110, -40, -160, -40, b, -160, -75, -160, -100, b, -115, -100, -75, -100, -70, -75", 's');
    list_append(generatedFont, (unitype) "ζ, 1, 4, -160, 40, b, -80, 40, -140, -20, b, -155, -55, -170, -100, b, -110, -100, -70, -100, -100, -135", 's');
    list_append(generatedFont, (unitype) "η, 2, 2, -160, -100, -160, 20, 3, b, -160, 0, -150, 20, b, -115, 20, -70, 20, -70, -20, -70, -155", 's');
    list_append(generatedFont, (unitype) "θ, 1, 5, b, -160, -25, -160, -100, b, -115, -100, -70, -100, b, -70, -25, -70, 50, b, -115, 50, -160, 50, -160, -25, -70, -25", 's');
    list_append(generatedFont, (unitype) "ι, 1, 2, -160, 20, b, -160, -70, -160, -105, -130, -100", 's');
    list_append(generatedFont, (unitype) "κ, 3, 2, -160, -100, -160, 20, 3, -160, -40, -140, -40, -85, 20, 2, -140, -40, -85, -100", 's');
    list_append(generatedFont, (unitype) "λ, 2, 2, b, -150, 50, -140, 50, -135, 40, b, -85, -90, -80, -100, -70, -100, 2, -120, 0, -160, -100", 's');
    list_append(generatedFont, (unitype) "μ, 3, 2, -70, 20, -70, -100, 3, b, -70, -80, -80, -100, b, -115, -100, -160, -100, -160, -60, -160, 20, 2, -160, -155, -160, 20", 's');
    list_append(generatedFont, (unitype) "µ, 3, 2, -70, 20, -70, -100, 3, b, -70, -80, -80, -100, b, -115, -100, -160, -100, -160, -60, -160, 20, 2, -160, -155, -160, 20", 's');
    list_append(generatedFont, (unitype) "ν, 1, 3, -160, 20, -115, -100, -70, 20", 's');
    list_append(generatedFont, (unitype) "ξ, 1, 6, b, -85, 30, -100, 40, b, -120, 40, -160, 40, b, -160, 5, -160, -30, b, -110, -30, -160, -30, b, -160, -65, -160, -100, b, -110, -100, -70, -100, -100, -135", 's');
    list_append(generatedFont, (unitype) "ο, 1, 4, b, -115, -100, -70, -100, b, -70, -40, -70, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, -115, -100", 's');
    list_append(generatedFont, (unitype) "π, 3, 2, -145, -100, -145, 20, 2, -160, 20, -60, 20, 2, -75, 20, b, -75, -80, -75, -105, -60, -98", 's');
    list_append(generatedFont, (unitype) "ρ, 1, 5, -160, -160, b, -160, 0, -150, 20, b, -115, 20, -70, 20, b, -70, -40, -70, -100, b, -115, -100, -150, -100, -160, -80", 's');
    list_append(generatedFont, (unitype) "σ, 1, 5, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -115, -100, -70, -100, b, -70, -40, -70, 20, -115, 20, -65, 20", 's');
    list_append(generatedFont, (unitype) "τ, 2, 2, -160, 20, -70, 20, 2, -115, 20, b, -115, -80, -115, -110, -85, -95", 's');
    list_append(generatedFont, (unitype) "υ, 1, 4, -160, 20, b, -160, -65, -160, -100, b, -120, -100, -80, -100, b, -80, -50, -80, 0, -100, 20", 's');
    list_append(generatedFont, (unitype) "φ, 1, 6, -100, -160, b, -100, -10, -100, 25, b, -70, 10, -40, -10, b, -40, -45, -40, -100, b, -100, -100, -160, -100, b, -160, -45, -160, 5, -130, 15", 's');
    list_append(generatedFont, (unitype) "χ, 2, 2, b, -160, 10, -150, 10, -145, 0, b, -80, -150, -75, -160, -65, -160, 2, -160, -160, -75, 10", 's');
    list_append(generatedFont, (unitype) "ψ, 2, 2, -100, -160, -100, 20, 4, -160, 20, b, -160, -40, -160, -100, b, -100, -100, -40, -100, b, -40, -40, -40, 0, -50, 20", 's');
    list_append(generatedFont, (unitype) "ω, 1, 6, b, -145, 20, -160, -5, b, -160, -40, -160, -100, b, -120, -100, -80, -100, b, -80, -30, -80, -100, b, -40, -100, 0, -100, b, 0, -40, 0, -5, -15, 20", 's');
    list_append(generatedFont, (unitype) "ς, 1, 4, b, -70, -10, -80, 20, b, -115, 20, -160, 20, b, -160, -40, -160, -100, b, -110, -100, -70, -100, -100, -135", 's');
    list_append(generatedFont, (unitype) "1, 1, 3, -160, 35, -120, 60, -120, -100, w, 55", 's');
    list_append(generatedFont, (unitype) "2, 1, 4, -65, -100, b, -160, -100, -70, -15, b, -70, 25, -70, 60, b, -110, 60, -150, 60, -160, 25", 's');
    list_append(generatedFont, (unitype) "3, 1, 6, b, -160, 30, -150, 60, b, -115, 60, -70, 60, b, -70, 20, -70, -20, b, -120, -20, -70, -20, b, -70, -60, -70, -100, b, -115, -100, -150, -100, -160, -70", 's');
    list_append(generatedFont, (unitype) "4, 1, 4, -85, -100, -85, 60, -160, -60, -65, -60", 's');
    list_append(generatedFont, (unitype) "5, 1, 6, -75, 60, -150, 60, b, -160, -30, -140, -5, b, -115, -5, -70, -5, b, -70, -55, -70, -100, b, -115, -100, -150, -100, -160, -75", 's');
    list_append(generatedFont, (unitype) "6, 1, 5, b, -100, 60, -160, 40, b, -160, -55, -160, -100, b, -118, -100, -75, -100, b, -75, -55, -75, -10, b, -118, -10, -160, -10, -160, -55", 's');
    list_append(generatedFont, (unitype) "7, 1, 3, -160, 60, -70, 60, -130, -100", 's');
    list_append(generatedFont, (unitype) "8, 1, 8, b, -118, -15, -75, -15, b, -75, -60, -75, -100, b, -117, -100, -160, -100, b, -160, -60, -160, -15, b, -118, -15, -80, -15, b, -80, 20, -80, 60, b, -117, 60, -155, 60, b, -155, 20, -155, -15, -118, -15", 's');
    list_append(generatedFont, (unitype) "9, 1, 5, b, -140, -100, -75, -90, b, -75, 15, -75, 60, b, -118, 60, -160, 60, b, -160, 15, -160, -30, b, -118, -30, -75, -30, -75, 15", 's');
    list_append(generatedFont, (unitype) "0, 1, 4, b, -112, 60, -160, 60, b, -160, -20, -160, -100, b, -112, -100, -65, -100, b, -65, -20, -65, 60, -112, 60", 's');
    list_append(generatedFont, (unitype) "!, 2, 2, -160, 60, -160, -50, 1, -160, -100", 's');
    list_append(generatedFont, (unitype) "@, 1, 10, b, -35, -140, -55, -150, b, -80, -150, -160, -150, b, -160, -55, -160, 50, b, -80, 50, 0, 50, b, 0, -55, 0, -100, b, -30, -100, -50, -100, -55, -80, b, -50, -10, -75, 15, b, -100, -5, -120, -25, b, -120, -60, -120, -100, b, -90, -100, -70, -100, -55, -80", 's');
    list_append(generatedFont, (unitype) "#, 4, 2, -130, -100, -100, 50, 2, -90, -100, -60, 50, 2, -140, 0, -40, 0, 2, -150, -50, -50, -50", 's');
    list_append(generatedFont, (unitype) "$, 3, 6, b, -80, 35, -90, 60, b, -120, 60, -160, 60, b, -160, 20, -160, -20, b, -120, -20, -80, -20, b, -80, -60, -80, -100, b, -120, -100, -150, -100, -160, -75, 2, -120, 80, -120, 60, 2, -120, -120, -120, -100", 's');
    list_append(generatedFont, (unitype) "£, 2, 4, b, -55, 30, -60, 60, b, -100, 60, -145, 60, -145, 5, b, -140, -75, -140, -100, -160, -100, -50, -100, 2, -160, -30, -100, -30", 's');
    list_append(generatedFont, (unitype) "€, 3, 4, b, -60, 50, -70, 60, b, -90, 60, -140, 60, b, -140, -20, -140, -100, b, -90, -100, -70, -100, -60, -90, 2, -160, -5, -80, -5, 2, -160, -35, -80, -35", 's');
    list_append(generatedFont, (unitype) "₺, 3, 2, -130, 70, b, -130, -100, -60, -100, -60, -10, 2, -160, 10, -90, 50, 2, -160, -30, -90, 10", 's');
    list_append(generatedFont, (unitype) "₽, 2, 5, -140, -100, -140, 60, b, -80, 60, -40, 60, b, -40, 20, -40, -20, -80, -20, -160, -20, 2, -160, -60, -90, -60", 's');
    list_append(generatedFont, (unitype) "¥, 4, 3, -160, 60, -110, -30, -60, 60, 2, -110, -30, -110, -100, 2, -150, -30, -70, -30, 2, -150, -60, -70, -60", 's');
    list_append(generatedFont, (unitype) "₩, 3, 5, -160, 60, -120, -100, -80, 60, -40, -100, 0, 60, 2, -160, 0, 0, 0, 2, -160, -40, 0, -40", 's');
    list_append(generatedFont, (unitype) "₹, 2, 6, -60, 60, -160, 60, b, -120, 60, -80, 60, b, -80, 20, -80, -20, -120, -20, -160, -20, -80, -100, 2, -160, 20, -60, 20", 's');
    list_append(generatedFont, (unitype) "₣, 3, 3, -160, -100, -160, 60, -70, 60, 2, -160, -20, -80, -20, 2, -180, -60, -120, -60", 's');
    list_append(generatedFont, (unitype) "฿, 3, 7, -160, -100, -160, 60, b, -120, 60, -70, 60, b, -70, 20, -70, -20, b, -120, -20, -70, -20, b, -70, -60, -70, -100, -120, -100, -160, -100, 2, -120, -20, -160, -20, 2, -120, 80, -120, -120", 's');
    list_append(generatedFont, (unitype) "%, 3, 2, -140, -85, -60, 40, 4, b, -135, -10, -110, -10, b, -110, 20, -110, 50, b, -135, 50, -160, 50, b, -160, 20, -160, -10, -135, -10, 4, b, -65, -40, -90, -40, b, -90, -70, -90, -100, b, -65, -100, -40, -100, b, -40, -70, -40, -40, -65, -40", 's');
    list_append(generatedFont, (unitype) "^, 1, 3, -160, 30, -140, 60, -120, 30", 's');
    list_append(generatedFont, (unitype) "&, 1, 8, -40, -100, b, -140, 10, -160, 30, b, -160, 45, -160, 80, b, -120, 80, -85, 80, b, -85, 45, -85, 20, b, -110, 5, -160, -25, b, -160, -55, -160, -100, b, -110, -100, -50, -100, -50, -40", 's');
    list_append(generatedFont, (unitype) "*, 3, 3, -120, 60, -120, 25, -89, 37, 3, -100, -5, -120, 25, -140, -5, 2, -151, 37, -120, 25", 's');
    list_append(generatedFont, (unitype) "(, 1, 2, b, -120, 70, -160, 50, b, -160, -25, -160, -100, -120, -120", 's');
    list_append(generatedFont, (unitype) "), 1, 2, b, -160, 70, -120, 50, b, -120, -25, -120, -100, -160, -120", 's');
    list_append(generatedFont, (unitype) "`, 1, 2, -140, 30, -160, 60", 's');
    list_append(generatedFont, (unitype) "~, 1, 4, b, -160, -35, -160, -5, b, -132, -5, -115, -5, b, -105, -20, -95, -35, b, -78, -35, -50, -35, -50, -5", 's');
    list_append(generatedFont, (unitype) "-, 1, 2, -160, -20, -115, -20", 's');
    list_append(generatedFont, (unitype) "_, 1, 2, -160, -100, -70, -100", 's');
    list_append(generatedFont, (unitype) "=, 2, 2, -160, -40, -80, -40, 2, -160, 0, -80, 0", 's');
    list_append(generatedFont, (unitype) "+, 2, 2, -160, -20, -60, -20, 2, -110, -70, -110, 30", 's');
    list_append(generatedFont, (unitype) "[, 1, 4, -120, 70, -160, 70, -160, -120, -120, -120", 's');
    list_append(generatedFont, (unitype) "{, 1, 4, b, -120, 70, -150, 70, b, -140, 10, -135, -10, b, -160, -25, -135, -40, b, -140, -60, -150, -120, -120, -120", 's');
    list_append(generatedFont, (unitype) "], 1, 4, -160, 70, -120, 70, -120, -120, -160, -120", 's');
    list_append(generatedFont, (unitype) "}, 1, 4, b, -160, 70, -130, 70, b, -140, 10, -145, -10, b, -120, -25, -145, -40, b, -140, -60, -130, -120, -160, -120", 's');
    list_append(generatedFont, (unitype) "\\, 1, 2, -80, -110, -160, 60", 's');
    list_append(generatedFont, (unitype) "|, 1, 2, -160, 50, -160, -120", 's');
    list_append(generatedFont, (unitype) ";, 2, 1, -160, 10, 1, b, -160, -90, -160, -110, -170, -115", 's');
    list_append(generatedFont, (unitype) ":, 2, 1, -160, 10, 1, -160, -90", 's');
    list_append(generatedFont, (unitype) "‘, 1, 1, b, -150, 30, -160, 40, -160, 60", 's');
    list_append(generatedFont, (unitype) "', 1, 2, -160, 60, -160, 30", 's');
    list_append(generatedFont, (unitype) "’, 1, 1, b, -160, 30, -150, 40, -150, 60", 's');
    list_append(generatedFont, (unitype) "“, 2, 1, b, -150, 30, -160, 40, -160, 60, 1, b, -125, 30, -135, 40, -135, 60", 's');
    list_append(generatedFont, (unitype) "\", 2, 2, -160, 60, -160, 30, 2, -135, 60, -135, 30", 's');
    list_append(generatedFont, (unitype) "”, 2, 1, b, -160, 30, -150, 40, -150, 60, 1, b, -135, 30, -125, 40, -125, 60", 's');
    list_append(generatedFont, (unitype) ",, 1, 1, b, -160, -90, -160, -110, -170, -115", 's');
    list_append(generatedFont, (unitype) "<, 1, 3, -80, 25, -160, -20, -80, -65", 's');
    list_append(generatedFont, (unitype) "., 1, 1, -160, -100", 's');
    list_append(generatedFont, (unitype) ">, 1, 3, -160, 25, -80, -20, -160, -65", 's');
    list_append(generatedFont, (unitype) "/, 1, 2, -160, -110, -80, 60", 's');
    list_append(generatedFont, (unitype) "?, 2, 4, b, -160, 25, -155, 60, b, -120, 60, -80, 60, b, -80, 25, -80, 0, b, -100, -10, -120, -20, -120, -50, 1, -120, -100", 's');
    list_append(generatedFont, (unitype) "½, 3, 2, -140, -85, -60, 40, 3, -160, 35, -135, 50, -135, -10, 4, b, -80, -55, -75, -35, b, -60, -35, -40, -35, b, -40, -55, -40, -65, -80, -100, -35, -100", 's');
    list_append(generatedFont, (unitype) "¨, 2, 1, -160, 50, 1, -130, 50", 's');
}
