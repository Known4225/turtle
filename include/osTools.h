/*
 ██████╗ ███████╗████████╗ ██████╗  ██████╗ ██╗     ███████╗   ██╗  ██╗
██╔═══██╗██╔════╝╚══██╔══╝██╔═══██╗██╔═══██╗██║     ██╔════╝   ██║  ██║
██║   ██║███████╗   ██║   ██║   ██║██║   ██║██║     ███████╗   ███████║
██║   ██║╚════██║   ██║   ██║   ██║██║   ██║██║     ╚════██║   ██╔══██║
╚██████╔╝███████║   ██║   ╚██████╔╝╚██████╔╝███████╗███████║██╗██║  ██║
 ╚═════╝ ╚══════╝   ╚═╝    ╚═════╝  ╚═════╝ ╚══════╝╚══════╝╚═╝╚═╝  ╚═╝
https://patorjk.com/software/taag/#p=display&f=ANSI%20Shadow
*/

#include "turtle.h"

typedef struct GLFWcursor GLFWcursor;

typedef struct {
    GLFWwindow *osToolsWindow;
    GLFWcursor *standardCursors[6];
} osToolsGLFWObject;

typedef struct {
    const char *text; // clipboard text data (heap allocated)
} osToolsClipboardObject;

typedef struct {
    char executableFilepath[4096 + 1]; // filepath of executable
    char selectedFilename[4096 + 1]; // output filename - maximum filepath is 260 characters on windows and 4096 on linux
    char openOrSave; // 0 - open, 1 - save
    int32_t numExtensions; // number of extensions
    char **extensions; // array of allowed extensions (7 characters long max (cuz *.json;))
} osToolsFileDialogObject;

typedef struct {
    list_t *mappedFiles;
} osToolsMemmapObject;

/* global objects */
extern osToolsGLFWObject osToolsGLFW;
extern osToolsClipboardObject osToolsClipboard;
extern osToolsFileDialogObject osToolsFileDialog;
extern osToolsMemmapObject osToolsMemmap;

/* OS independent functions */
void osToolsIndependentInit(GLFWwindow *window);

/* returns clipboard text */
const char *osToolsClipboardGetText();

/* takes null terminated strings */
int32_t osToolsClipboardSetText(const char *input);

/*
GLFW_ARROW_CURSOR
GLFW_IBEAM_CURSOR
GLFW_CROSSHAIR_CURSOR
GLFW_HAND_CURSOR
GLFW_HRESIZE_CURSOR
GLFW_VRESIZE_CURSOR
*/
void osToolsSetCursor(uint32_t cursor);

void osToolsHideAndLockCursor();

void osToolsShowCursor();

typedef enum {
    OSTOOLS_CSV_ROW = 0,
    OSTOOLS_CSV_COLUMN = 1,
    OSTOOLS_CSV_FIELD_DOUBLE = 2,
    OSTOOLS_CSV_FIELD_INT = 3,
    OSTOOLS_CSV_FIELD_STRING = 4,
} osToolsCSV;

list_t *osToolsLoadInternal(char *filename, osToolsCSV rowOrColumn, char delimeter, osToolsCSV fieldType);

/* packages a CSV file into a list (headers are strings, all fields are doubles) - use OSTOOLS_CSV_ROW to put it in a list of lists where each list is a row of the CSV and use OSTOOLS_CSV_COLUMN to output a list of lists where each list is a column of the CSV */
list_t *osToolsLoadCSV(char *filename, osToolsCSV rowOrColumn);

/* packages a CSV file into a list (headers are strings, all fields are doubles) - use OSTOOLS_CSV_ROW to put it in a list of lists where each list is a row of the CSV and use OSTOOLS_CSV_COLUMN to output a list of lists where each list is a column of the CSV */
list_t *osToolsLoadCSVDouble(char *filename, osToolsCSV rowOrColumn);

/* packages a CSV file into a list (headers are strings, all fields are ints) - use OSTOOLS_CSV_ROW to put it in a list of lists where each list is a row of the CSV and use OSTOOLS_CSV_COLUMN to output a list of lists where each list is a column of the CSV */
list_t *osToolsLoadCSVInt(char *filename, osToolsCSV rowOrColumn);

/* packages a CSV file into a list (headers are strings, all fields are strings) - use OSTOOLS_CSV_ROW to put it in a list of lists where each list is a row of the CSV and use OSTOOLS_CSV_COLUMN to output a list of lists where each list is a column of the CSV */
list_t *osToolsLoadCSVString(char *filename, osToolsCSV rowOrColumn);

#ifdef OS_WINDOWS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <shobjidl.h>

typedef struct {
    char name[32];
    HANDLE comHandle;
} win32ComPortObject;

#define WIN32TCP_NUM_SOCKETS 32

typedef struct {
    char *address;
    char *port;
    SOCKET connectSocket[WIN32TCP_NUM_SOCKETS];
    char socketOpen[WIN32TCP_NUM_SOCKETS];
} win32SocketObject;
#endif

#ifdef OS_LINUX
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#endif

int32_t osToolsInit(char argv0[], GLFWwindow *window);

void osToolsFileDialogAddExtension(char *extension);

int32_t osToolsFileDialogPrompt(char openOrSave, char *prename);

uint8_t *osToolsMapFile(char *filename, uint32_t *sizeOutput);

int32_t osToolsUnmapFile(uint8_t *data);
