/*
Written by Ryan Srichai 11.08.25

This application bundles all turtle libraries into a single portable header file, singlefile/turtle.h
*/

/* define output file */
const char outputFile[] = "singlefile/turtle.h";

/* define header files in order from top to bottom of output file */
const char headerFiles[][128] = {
    "include/list.h",
    "include/turtle.h",
    "include/turtleText.h",
    ""
};

/* define source files in order from top to bottom of output file */
const char sourceFiles[][128] = {
    "list.c",
    "turtle.c",
    ""
};

/* these macros correspond to the source files */
const char implementationMacros[][128] = {
    "UNITYPE_LIST_IMPLEMENTATION",
    "TURTLE_INTERNAL_IMPLEMENTATION",
    "TURTLE_TEXT_IMPLEMENTATION",
    "TURTLE_TOOLS_IMPLEMENTATION",
    "OS_TOOLS_IMPLEMENTATION"
};