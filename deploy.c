/*
Written by Ryan Srichai 11.08.25

This application bundles all turtle libraries into a single portable header file, singlefile_output/turtle.h
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>

/* define output file */
const char outputFile[] = "singlefile_output/turtle.h";

const char wrappingMacro[] = "TURTLE_H"; // this macro guards the entire file
const char sourceMacro[] = "TURTLE_IMPLEMENTATION"; // this macro enables all source implementation macros

/* define header files in order from top to bottom of output file */
const char headerFiles[][128] = {
    "include/list.h",
    "include/turtle.h",
    "include/turtleText.h",
    "include/turtleTools.h",
    "include/osTools.h",
    "include/khrplatform.h",
    "include/glad.h",
    "include/glfw3.h",
};

/* define source files in order from top to bottom of output file */
const char sourceFiles[][128] = {
    "src/list.c",
    "src/turtle.c",
    "src/turtleText.c",
    "src/turtleTools.c",
    "src/osTools.c",
};

/* these macros correspond to the source files */
const char implementationMacros[][128] = {
    "UNITYPE_LIST_IMPLEMENTATION",
    "TURTLE_INTERNAL_IMPLEMENTATION",
    "TURTLE_TEXT_IMPLEMENTATION",
    "TURTLE_TOOLS_IMPLEMENTATION",
    "OS_TOOLS_IMPLEMENTATION"
};

int32_t headerLength = sizeof(headerFiles) / 128;
int32_t sourceLength = sizeof(sourceFiles) / 128;
int32_t macrosLength = sizeof(implementationMacros) / 128;

const int32_t dependencyTree[] = {
    0, 0, 0, 0, 0, // list has no dependencies
    1, 0, 0, 0, 0, // turtle internal requires on list
    0, 1, 0, 0, 0, // turtle text requires on turtle internal
    0, 0, 1, 0, 0, // turtle tools requires on turtle text
    0, 1, 0, 0, 0, // os tools requires turtle internal
};

/* check if this line of code should not be included in the final output, return 1 if the line is blacklisted */
int32_t lineBlacklist(char *line) {
    /* blacklist any #includes to my own files (as they won't exist in the singlefile version) */
    if (line[0] != '#') {
        return 0;
    }
    char blacklisted[headerLength * 2][128];
    for (int32_t i = 0; i < headerLength; i++) {
        sprintf(blacklisted[i], "#include \"%s\"\n", headerFiles[i]);
    }
    for (int32_t i = 0; i < headerLength; i++) {
        sprintf(blacklisted[i + headerLength], "#include \"%s\"\n", headerFiles[i] + 8);
    }
    for (int32_t i = 0; i < headerLength * 2; i++) {
        // printf("%s%s\n", blacklisted[i], line);
        if (strcmp(blacklisted[i], line) == 0) {
            return 1;
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    // printf("%d %d %d\n", sizeof(headerFiles), sizeof(sourceFiles), sizeof(implementationMacros));
    /* check for file existence */
    for (int32_t i = 0; i < headerLength; i++) {
        FILE *fileExists = fopen(headerFiles[i], "r");
        if (!fileExists) {
            printf("ERROR: Header file %s does not exist, exiting\n", sourceFiles[i]);
            return -1;
        }
        fclose(fileExists);
    }
    for (int32_t i = 0; i < sourceLength; i++) {
        FILE *fileExists = fopen(sourceFiles[i], "r");
        if (!fileExists) {
            printf("ERROR: Source file %s does not exist, exiting\n", sourceFiles[i]);
            return -1;
        }
        fclose(fileExists);
    }

    /* construct final file */
    FILE *outputfp = fopen(outputFile, "w");
    /* add wrapping macro */
    fprintf(outputfp, "#ifndef %s\n#define %s\n\n", wrappingMacro, wrappingMacro);
    /* add headers */
    for (int32_t i = 0; i < headerLength; i++) {
        FILE *headerfp = fopen(headerFiles[i], "r");
        uint8_t buffer[4096]; // line cannot exceed 4096 characters, my longest line is 446 characters so I think we're safe
        while (fgets(buffer, 4096, headerfp) != NULL) {
            if (!lineBlacklist(buffer)) {
                fprintf(outputfp, "%s", buffer);
            }
        }
        fwrite("\n", 1, 1, outputfp);
        fclose(headerfp);
    }
    /* add dependency checker */
    fprintf(outputfp, "\n\n#ifdef %s\n", sourceMacro);
    for (int32_t i = 0; i < macrosLength; i++) {
        fprintf(outputfp, "#define %s\n", implementationMacros[i]);
    }
    fprintf(outputfp, "#endif /* %s */\n", sourceMacro);
    for (int32_t j = 0; j < macrosLength; j++) {
        int32_t neverSeen = 1;
        for (int32_t i = 0; i < sizeof(dependencyTree) / sizeof(int32_t) / macrosLength; i++) {
            // printf("%d\n", dependencyTree[j * macrosLength + i]);
            if (dependencyTree[j * macrosLength + i]) {
                if (neverSeen) {
                    fprintf(outputfp, "\n#ifdef %s\n", implementationMacros[j]);
                    neverSeen = 0;
                }
                fprintf(outputfp, "#ifndef %s\n", implementationMacros[i]);
                fprintf(outputfp, "#error %s not defined\n", implementationMacros[i]);
                fprintf(outputfp, "#endif /* %s */\n", implementationMacros[i]);
            }
        }
        if (!neverSeen) {
            fprintf(outputfp, "#endif /* %s */\n\n", implementationMacros[j]);
        }
    }
    /* add source files */
    fprintf(outputfp, "/*\n\
███████╗ ██████╗ ██╗   ██╗██████╗  ██████╗███████╗\n\
██╔════╝██╔═══██╗██║   ██║██╔══██╗██╔════╝██╔════╝\n\
███████╗██║   ██║██║   ██║██████╔╝██║     █████╗  \n\
╚════██║██║   ██║██║   ██║██╔══██╗██║     ██╔══╝  \n\
███████║╚██████╔╝╚██████╔╝██║  ██║╚██████╗███████╗\n\
╚══════╝ ╚═════╝  ╚═════╝ ╚═╝  ╚═╝ ╚═════╝╚══════╝\n\
https://patorjk.com/software/taag/#p=display&f=ANSI%%20Shadow\n\
*/\n");
    for (int32_t i = 0; i < sourceLength; i++) {
        fprintf(outputfp, "#ifdef %s\n", implementationMacros[i]);
        FILE *sourcefp = fopen(sourceFiles[i], "r");
        uint8_t buffer[4096];
        while (fgets(buffer, 4096, sourcefp) != NULL) {
            if (!lineBlacklist(buffer)) {
                fprintf(outputfp, "%s", buffer);
            }
        }
        fwrite("\n", 1, 1, outputfp);
        fclose(sourcefp);
        fprintf(outputfp, "#endif /* %s */\n", implementationMacros[i]);
    }

    /* add wrapping macro endif */
    fprintf(outputfp, "\n\n#endif /* %s */\n", wrappingMacro);
    fclose(outputfp);

    printf("Successfully exported singlefile header to %s\n", outputFile);
}