/*
 ██████╗ ███████╗████████╗ ██████╗  ██████╗ ██╗     ███████╗   ██╗  ██╗
██╔═══██╗██╔════╝╚══██╔══╝██╔═══██╗██╔═══██╗██║     ██╔════╝   ██║  ██║
██║   ██║███████╗   ██║   ██║   ██║██║   ██║██║     ███████╗   ███████║
██║   ██║╚════██║   ██║   ██║   ██║██║   ██║██║     ╚════██║   ██╔══██║
╚██████╔╝███████║   ██║   ╚██████╔╝╚██████╔╝███████╗███████║██╗██║  ██║
 ╚═════╝ ╚══════╝   ╚═╝    ╚═════╝  ╚═════╝ ╚══════╝╚══════╝╚═╝╚═╝  ╚═╝
https://patorjk.com/software/taag/#p=display&f=ANSI%20Shadow
*/

#ifndef OS_TOOLS_H
#define OS_TOOLS_H

#include "list.h"
#include "glad.h"
#include "glfw3.h"

/* required forward declarations (for packaging) */
typedef struct GLFWcursor GLFWcursor;
extern const uint32_t moveCursorData[1024];
extern const uint32_t leftDiagonalCursorData[1024];
extern const uint32_t rightDiagonalCursorData[1024];
#define GLFW_DLESIZE_CURSOR     0x00036007
#define GLFW_DRESIZE_CURSOR     0x00036008
#define GLFW_MOVE_CURSOR        0x00036009

typedef struct {
    GLFWwindow *osToolsWindow;
    GLFWcursor *standardCursors[10];
} ost_glfw_t;

typedef struct {
    const char *text; // clipboard text data (heap allocated)
} ost_clipboard_t;

typedef struct {
    char executableFilepath[4096 + 1]; // filepath of executable
    list_t *selectedFilenames; // output filenames from osToolsFileDialogPrompt (erased with every call, only has multiple items when multiselect is used)
    list_t *globalExtensions; // list of global extensions accepted by file dialog
} ost_file_dialog_t;

typedef struct {
    list_t *mappedFiles; // Format Windows: name, handle, mapping handle, address. Format Linux: name, size
} ost_memmap_t;

/* global objects */
extern ost_glfw_t osToolsGLFW;
extern ost_clipboard_t osToolsClipboard;
extern ost_file_dialog_t osToolsFileDialog;
extern ost_memmap_t osToolsMemmap;

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
GLFW_DLESIZE_CURSOR
GLFW_DRESIZE_CURSOR
GLFW_MOVE_CURSOR
*/
void osToolsSetCursor(uint32_t cursor);

void osToolsHideAndLockCursor();

void osToolsShowCursor();

typedef enum {
    OSTOOLS_CSV_ROW = 0,
    OSTOOLS_CSV_COLUMN = 1,
} ost_csv_t;

typedef enum {
    OSTOOLS_CSV_FIELD_DOUBLE = 0,
    OSTOOLS_CSV_FIELD_INT = 1,
    OSTOOLS_CSV_FIELD_STRING = 2,
} ost_csv_field_t;

list_t *osToolsLoadInternal(char *filename, ost_csv_t rowOrColumn, char delimeter, ost_csv_field_t fieldType);

/* packages a CSV file into a list (headers are strings, all fields are doubles) - use OSTOOLS_CSV_ROW to put it in a list of lists where each list is a row of the CSV and use OSTOOLS_CSV_COLUMN to output a list of lists where each list is a column of the CSV */
list_t *osToolsLoadCSV(char *filename, ost_csv_t rowOrColumn);

/* packages a CSV file into a list (headers are strings, all fields are doubles) - use OSTOOLS_CSV_ROW to put it in a list of lists where each list is a row of the CSV and use OSTOOLS_CSV_COLUMN to output a list of lists where each list is a column of the CSV */
list_t *osToolsLoadCSVDouble(char *filename, ost_csv_t rowOrColumn);

/* packages a CSV file into a list (headers are strings, all fields are ints) - use OSTOOLS_CSV_ROW to put it in a list of lists where each list is a row of the CSV and use OSTOOLS_CSV_COLUMN to output a list of lists where each list is a column of the CSV */
list_t *osToolsLoadCSVInt(char *filename, ost_csv_t rowOrColumn);

/* packages a CSV file into a list (headers are strings, all fields are strings) - use OSTOOLS_CSV_ROW to put it in a list of lists where each list is a row of the CSV and use OSTOOLS_CSV_COLUMN to output a list of lists where each list is a column of the CSV */
list_t *osToolsLoadCSVString(char *filename, ost_csv_t rowOrColumn);

/* untether the program from the console that spawned it - will close a console if the program is run independently */
void osToolsCloseConsole();

/* COM support */
typedef enum {
    OSTOOLS_BAUD_110 = 110,
    OSTOOLS_BAUD_300 = 300,
    OSTOOLS_BAUD_600 = 600,
    OSTOOLS_BAUD_1200 = 1200,
    OSTOOLS_BAUD_2400 = 2400,
    OSTOOLS_BAUD_4800 = 4800,
    OSTOOLS_BAUD_9600 = 9600, // common low baud rate
    OSTOOLS_BAUD_14400 = 14400,
    OSTOOLS_BAUD_19200 = 19200,
    OSTOOLS_BAUD_38400 = 38400,
    OSTOOLS_BAUD_56000 = 56000, // common medium baud rate
    OSTOOLS_BAUD_57600 = 57600,
    OSTOOLS_BAUD_115200 = 115200, // common high speed baud rate
    OSTOOLS_BAUD_128000 = 128000,
    OSTOOLS_BAUD_256000 = 256000,
} osToolsComBaud_t;

typedef struct {
    list_t *com; // Format Windows: name, HANDLE
} ost_com_t;

extern ost_com_t osToolsCom;

/* get a list of all com ports (strings) */
list_t *osToolsComList();

/* opens a com port */
int32_t osToolsComOpen(char *name, osToolsComBaud_t baudRate);

/* returns number of bytes sent. This function blocks until all data has been sent (or error) */
int32_t osToolsComSend(char *name, uint8_t *data, int32_t length);

/* returns number of bytes received. This function blocks until length bytes are received or timeoutMilliseconds is exceeded */
int32_t osToolsComReceive(char *name, uint8_t *buffer, int32_t length, int32_t timeoutMilliseconds);

/* closes a com port */
int32_t osToolsComClose(char *name);

/* Socket (IPv4) support */
typedef enum {
    OSTOOLS_PROTOCOL_TCP = 0,
    OSTOOLS_PROTOCOL_UDP = 1,
} osToolsSocketProtocol_t;

typedef enum {
    OSI_NAME = 0,
    OSI_SOCKET = 1,
    OSI_TYPE = 2,
    OSI_PROTOCOL = 4,
    OSI_ADDRESS = 5,
    OSI_PORT = 21,
    OSI_NUMBER_OF_FIELDS = 32,
} os_socket_index_t;

typedef struct {
    list_t *socket; // Format: name, SOCKET (Windows) sockfd (Linux), client/server, reserved, protocol, address[0], address[1], address[2], address[4], reserved * 12, port (int), reserved * 10
    int8_t win32wsaActive;
} ost_socket_t;

extern ost_socket_t osToolsSocket;

/* gets the address of a socket as a string (populated in address argument) */
int32_t osToolsGetSocketAddress(char *socketName, char *address, int32_t length);

/* gets the port of a socket as a string (populated in port argument) */
int32_t osToolsGetPort(char *socketName, char *port, int32_t length);

/* create the name for the server (used to access it), as well as a protocol (either OSTOOLS_PROTOCOL_TCP or OSTOOLS_PROTOCOL_UDP) and a binding address */
int32_t osToolsServerSocketCreate(char *serverName, osToolsSocketProtocol_t protocol, char *serverAddress);

/* listens for connections on a server socket, when a connection comes in it will create a new socket and assign it the name clientName. This function blocks until a connection is received then it will return a list [address (string), port (string)] of the incoming connection */
int32_t osToolsServerSocketListen(char *serverName, char *clientName);

/* create the name for the client (used to access it), as well as a protocol (either OSTOOLS_PROTOCOL_TCP or OSTOOLS_PROTOCOL_UDP) and specify the server's address, and port (the server that this client socket will connect to) */
int32_t osToolsClientSocketCreate(char *clientName, osToolsSocketProtocol_t protocol, char *serverAddress, char *serverPort, int32_t timeoutMilliseconds);

/* sends data over a socket. This function blocks until all data has been sent (or error) */
int32_t osToolsSocketSend(char *socketName, uint8_t *data, int32_t length);

/* receives up to length bytes from a socket - returns number of bytes received. This function blocks until length bytes are received or timeoutMilliseconds is exceeded */
int32_t osToolsSocketReceive(char *socketName, uint8_t *data, int32_t length, int32_t timeoutMilliseconds);

/* close and destroy a socket */
int32_t osToolsSocketDestroy(char *socketName);

/* Camera support */
typedef struct {
    list_t *camera; // Format Windows: camera name, width, height, framerate, pointer to source reader, pointer to IMFTransform decoder, pointer to buffer, pointer to IMFTransform decoder, pointer to buffer, pointer to savedMediaType
} ost_camera_t;

extern ost_camera_t osToolsCamera;

/* gets a list [camera name (string), width (int), height (int), framerate (double), ...] of camera devices on the system */
list_t *osToolsCameraList();

/* opens a camera given camera name from osToolsCameraList */
int32_t osToolsCameraOpen(char *name);

/* gets an RGB buffer from the camera - buffer must be at least width * height * 3 bytes */
int32_t osToolsCameraReceive(char *name, uint8_t *data);

/* closes a camera */
int32_t osToolsCameraClose(char *name);

#ifdef OS_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shobjidl.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mfidl.h>
#include <mfapi.h>
#include <mfreadwrite.h>
#include <wmcodecdsp.h>
#endif

#ifdef OS_LINUX
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#endif

/* initialise osTools, pass in argv[0] from main function as well as GLFW window object */
int32_t osToolsInit(char argv0[], GLFWwindow *window);

/* clear the list of global extensions */
void osToolsFileDialogClearGlobalExtensions();

/* add a single extension to the global file extensions */
void osToolsFileDialogAddGlobalExtension(char *extension);

/* copies the data from the list to the global extensions (you can free the list passed in immediately after calling this) */
void osToolsFileDialogSetGlobalExtensions(list_t *extensions);

typedef enum {
    OSTOOLS_FILE_DIALOG_OPEN = 0,
    OSTOOLS_FILE_DIALOG_SAVE = 1,
} ost_file_dialog_save_t;

typedef enum {
    OSTOOLS_FILE_DIALOG_SINGLE_SELECT = 0,
    OSTOOLS_FILE_DIALOG_MULTISELECT = 1,
    OSTOOLS_FILE_DIALOG_MULTI_SELECT = 1,
    OSTOOLS_FILE_DIALOG_MULTIPLE_SELECT = 1,
} ost_file_dialog_multiselect_t;

typedef enum {
    OSTOOLS_FILE_DIALOG_FILE = 0,
    OSTOOLS_FILE_DIALOG_FOLDER = 1,
} ost_file_dialog_folder_t;

/*
openOrSave: 0 - open, 1 - save
multiselect: 0 - single file select, 1 - multiselect, can only use when opening (cannot save to multiple files)
folder: 0 - file dialog, 1 - folder dialog
prename: refers to autofill filename ("null" or empty string for no autofill)
extensions: pass in a list of accepted file extensions or pass in NULL to use global list of extensions
*/
int32_t osToolsFileDialogPrompt(ost_file_dialog_save_t openOrSave, ost_file_dialog_multiselect_t multiselect, ost_file_dialog_folder_t folder, char *prename, list_t *extensions);

/* save dialog - set prename to NULL for no prename, set extensions to NULL to use global extensions */
int32_t osToolsFileDialogSave(ost_file_dialog_folder_t folder, char *prename, list_t *extensions);

/* open dialog - set prename to NULL for no prename, set extensions to NULL to use global extensions */
int32_t osToolsFileDialogOpen(ost_file_dialog_multiselect_t multiselect, ost_file_dialog_folder_t folder, char *prename, list_t *extensions);

uint8_t *osToolsFileMap(char *filename, uint32_t *sizeOutput);

int32_t osToolsFileUnmap(uint8_t *data);

/* lists files in a directory (does NOT list folders), format [name, size, name, size, ...] */
list_t *osToolsFileList(char *directory);

/* non-recursive, lists folders in a directory, format [name, name, ...] */
list_t *osToolsFolderList(char *directory);

/* lists files and folders in a directory, format [name, size, name, size, ...] (size is -1 for folders) */
list_t *osToolsFileAndFolderList(char *directory);

/* create a folder */
int32_t osToolsFolderCreate(char *folder);

/* delete a folder (and all files and subfolders) */
int32_t osToolsFolderDestroy(char *folder);

#endif /* OS_TOOLS_H */
