/*
Created by Ryan Srichai

a note on COM objects:
COM objects are C++ classes/structs
This means that it has methods
This is simulated in C via lpVtbl (vtable) which is an array of function pointers
Use the lpVtbl member of a struct to call methods via STRUCTNAME -> lpVtbl -> METHODNAME(args)
https://www.codeproject.com/Articles/13601/COM-in-plain-C

Under the hood, the pointer to the struct is also a pointer to an array of function pointers (lbVtbl)
The struct is therefore the size of the number of elements of the lbVtbl array * 8 plus the data in the struct which succeeds it

Whenever we call one of these methods, we have to pass in the object (which implicitly happens in OOP languages)
Actually, we pass in a pointer to the object, obviously i'm quite familiar with this

One more nuance is that whenever we pass a COM object in a function as an argument, it must always be &object
This is because in order to call methods we use object -> lpVtbl -> method, I mean we could use object.lpVtbl -> method but it's easier and allocates less stack memory to just use pointers

So whenever you take C++ COM object sample code, just follow this process:
change all the methods to -> lpVtbl -> methods
Add &obj as the first argument of every method
Change obj to &obj for all objects passed as arguments to functions or methods

That's it! (probably)

IFileDialog: https://learn.microsoft.com/en-us/windows/win32/api/shobjidl_core/nn-shobjidl_core-ifiledialog
Clipboard: https://learn.microsoft.com/en-us/windows/win32/dataxchg/clipboard
Cursor: https://learn.microsoft.com/en-us/windows/win32/learnwin32/setting-the-cursor-image
Sockets: https://learn.microsoft.com/en-us/windows/win32/winsock/tcp-ip-raw-sockets-2

idea: try glfwGetClipboardString and glfwSetClipboardString
*/

#ifndef OSTOOLS
#define OSTOOLS

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "glfw3.h"
#include "list.h"

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
osToolsGLFWObject osToolsGLFW;
osToolsClipboardObject osToolsClipboard;
osToolsFileDialogObject osToolsFileDialog;
osToolsMemmapObject osToolsMemmap;

/* forward declarations */
uint8_t *osToolsMapFile(char *filename, uint32_t *sizeOutput);
int32_t osToolsUnmapFile(uint8_t *data);

/* OS independent functions */
void osToolsIndependentInit(GLFWwindow *window) {
    /* initialise glfw cursors */
    osToolsGLFW.osToolsWindow = window;
    osToolsGLFW.standardCursors[0] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    osToolsGLFW.standardCursors[1] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
    osToolsGLFW.standardCursors[2] = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
    osToolsGLFW.standardCursors[3] = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
    osToolsGLFW.standardCursors[4] = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
    osToolsGLFW.standardCursors[5] = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);

    /* initialise memmap module */
    osToolsMemmap.mappedFiles = list_init();
}

/* returns clipboard text */
const char *osToolsClipboardGetText() {
    osToolsClipboard.text = glfwGetClipboardString(osToolsGLFW.osToolsWindow);
    return osToolsClipboard.text;
}

/* takes null terminated strings */
int32_t osToolsClipboardSetText(const char *input) {
    glfwSetClipboardString(osToolsGLFW.osToolsWindow, input);
    return 0;
}

/*
GLFW_ARROW_CURSOR
GLFW_IBEAM_CURSOR
GLFW_CROSSHAIR_CURSOR
GLFW_HAND_CURSOR
GLFW_HRESIZE_CURSOR
GLFW_VRESIZE_CURSOR
*/
void osToolsSetCursor(uint32_t cursor) {
    switch (cursor) {
    case GLFW_ARROW_CURSOR:
        glfwSetCursor(osToolsGLFW.osToolsWindow, osToolsGLFW.standardCursors[0]);
    break;
    case GLFW_IBEAM_CURSOR:
        glfwSetCursor(osToolsGLFW.osToolsWindow, osToolsGLFW.standardCursors[1]);
    break;
    case GLFW_CROSSHAIR_CURSOR:
        glfwSetCursor(osToolsGLFW.osToolsWindow, osToolsGLFW.standardCursors[2]);
    break;
    case GLFW_HAND_CURSOR:
        glfwSetCursor(osToolsGLFW.osToolsWindow, osToolsGLFW.standardCursors[3]);
    break;
    case GLFW_HRESIZE_CURSOR:
        glfwSetCursor(osToolsGLFW.osToolsWindow, osToolsGLFW.standardCursors[4]);
    break;
    case GLFW_VRESIZE_CURSOR:
        glfwSetCursor(osToolsGLFW.osToolsWindow, osToolsGLFW.standardCursors[5]);
    break;
    default:
        glfwSetCursor(osToolsGLFW.osToolsWindow, osToolsGLFW.standardCursors[0]);
    break;
    }
}

void osToolsHideAndLockCursor() {
    glfwSetInputMode(osToolsGLFW.osToolsWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void osToolsShowCursor() {
    glfwSetInputMode(osToolsGLFW.osToolsWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

typedef enum {
    OSTOOLS_CSV_ROW = 0,
    OSTOOLS_CSV_COLUMN = 1,
    OSTOOLS_CSV = 2,
    OSTOOLS_TSV = 3,
    OSTOOLS_CSV_FIELD_DOUBLE = 4,
    OSTOOLS_CSV_FIELD_INT = 5,
    OSTOOLS_CSV_FIELD_STRING = 6,
} osToolsCSV;

list_t *osToolsLoadInternal(char *filename, osToolsCSV rowOrColumn, osToolsCSV csvOrTsv, osToolsCSV fieldType) {
    uint32_t fileSize;
    uint8_t *mappedFile = osToolsMapFile(filename, &fileSize);
    if (mappedFile == NULL) {
        return NULL;
    }
    list_t *outputList = list_init();
    /* process headers */
    list_append(outputList, (unitype) list_init(), 'r');
    uint32_t rightIndex = 0;
    uint32_t leftIndex = 0;
    if (fileSize > 3 && mappedFile[0] == 0xEF && mappedFile[1] == 0xBB && mappedFile[2] == 0xBF) {
        /* UTF8 with BOM */
        rightIndex = 3;
        leftIndex = 3;
    }
    while (rightIndex < fileSize) {
        /* case: comma */
        if (mappedFile[rightIndex] == ',') {
            mappedFile[rightIndex] = '\0';
            if (rowOrColumn == OSTOOLS_CSV_ROW) {
                list_append(outputList -> data[0].r, (unitype) (char *) (mappedFile + leftIndex), 's');
            } else {
                list_append(outputList -> data[outputList -> length - 1].r, (unitype) (char *) (mappedFile + leftIndex), 's');
                list_append(outputList, (unitype) list_init(), 'r');
            }
            mappedFile[rightIndex] = ',';
            leftIndex = rightIndex + 1;
            while (mappedFile[leftIndex] == ' ') {
                rightIndex++;
                leftIndex++;
            }
        }
        if (mappedFile[rightIndex] == '\n' || mappedFile[rightIndex] == '\r') {
            /* case: end of line */
            if (rightIndex != leftIndex) {
                char tempHold = mappedFile[rightIndex];
                mappedFile[rightIndex] = '\0';
                if (rowOrColumn == OSTOOLS_CSV_ROW) {
                    list_append(outputList -> data[0].r, (unitype) (char *) (mappedFile + leftIndex), 's');
                } else {
                    list_append(outputList -> data[outputList -> length - 1].r, (unitype) (char *) (mappedFile + leftIndex), 's');
                }
                mappedFile[rightIndex] = tempHold;
            } else {
                if (rowOrColumn == OSTOOLS_CSV_COLUMN) {
                    list_pop(outputList);
                }
            }
            break;
        }
        rightIndex++;
    }
    while (mappedFile[rightIndex] == '\r' || mappedFile[rightIndex] == '\n') {
        rightIndex++;
    }
    leftIndex = rightIndex;
    /* process data (all doubles) */
    char listType = 'd';
    switch (fieldType) {
    case OSTOOLS_CSV_FIELD_DOUBLE:
        listType = 'd';
    break;
    case OSTOOLS_CSV_FIELD_INT:
        listType = 'i';
    break;
    case OSTOOLS_CSV_FIELD_STRING:
        listType = 'z'; // zstrings avoid double malloc
    break;
    default:
        listType = 'd';
    break;
    }
    int32_t column = 0; // columns are 0-indexed for the convenience of this parser
    int32_t row = 2; // start at row 2 (rows are 1-indexed for printouts)
    if (rowOrColumn == OSTOOLS_CSV_ROW) {
        list_append(outputList, (unitype) list_init(), 'r');
    }
    while (rightIndex < fileSize) {
        if (mappedFile[rightIndex] == ',') {
            /* case: comma */
            mappedFile[rightIndex] = '\0';
            unitype field;
            if (fieldType == OSTOOLS_CSV_FIELD_DOUBLE) {
                sscanf((char *) (mappedFile + leftIndex), "%lf", (double *) &field);
            } else if (fieldType == OSTOOLS_CSV_FIELD_INT) {
                sscanf((char *) (mappedFile + leftIndex), "%d", (int *) &field);
            } else if (fieldType == OSTOOLS_CSV_FIELD_STRING) {
                field.s = malloc(rightIndex - leftIndex + 1);
                strcpy(field.s, (char *) (mappedFile + leftIndex));
            }
            if (rowOrColumn == OSTOOLS_CSV_ROW) {
                list_append(outputList -> data[outputList -> length - 1].r, field, listType);
            } else if (rowOrColumn == OSTOOLS_CSV_COLUMN) {
                if (column < outputList -> length) {
                    list_append(outputList -> data[column].r, field, listType);
                } else {
                    printf("osToolsLoadInternal - more data columns than headers at row %d\n", row);
                }
            }
            mappedFile[rightIndex] = ',';
            leftIndex = rightIndex + 1;
            while (mappedFile[leftIndex] == ' ') {
                rightIndex++;
                leftIndex++;
            }
            column++;
        }
        if (mappedFile[rightIndex] == '\n' || mappedFile[rightIndex] == '\r') {
            /* case: end of line */
            if (rightIndex != leftIndex) {
                char tempHold = mappedFile[rightIndex];
                mappedFile[rightIndex] = '\0';
                unitype field;
                if (fieldType == OSTOOLS_CSV_FIELD_DOUBLE) {
                    sscanf((char *) (mappedFile + leftIndex), "%lf", (double *) &field);
                } else if (fieldType == OSTOOLS_CSV_FIELD_INT) {
                    sscanf((char *) (mappedFile + leftIndex), "%d", (int *) &field);
                } else if (fieldType == OSTOOLS_CSV_FIELD_STRING) {
                    field.s = malloc(rightIndex - leftIndex + 1);
                    strcpy(field.s, (char *) (mappedFile + leftIndex));
                }
                if (rowOrColumn == OSTOOLS_CSV_ROW) {
                    list_append(outputList -> data[outputList -> length - 1].r, field, listType);
                } else if (rowOrColumn == OSTOOLS_CSV_COLUMN) {
                    if (column < outputList -> length) {
                        list_append(outputList -> data[column].r, field, listType);
                    } else {
                        printf("osToolsLoadInternal - more data columns than headers at row %d\n", row);
                    }
                }
                mappedFile[rightIndex] = tempHold;
            }
            while (mappedFile[rightIndex] == '\r' || mappedFile[rightIndex] == '\n') {
                rightIndex++;
            }
            leftIndex = rightIndex;
            if (rowOrColumn == OSTOOLS_CSV_ROW) {
                list_append(outputList, (unitype) list_init(), 'r');
            }
            column = 0;
            row++;
        }
        rightIndex++;
    }
    /* catch: if the file doesn't end with a newline */
    if (leftIndex == fileSize) {
        if (rowOrColumn == OSTOOLS_CSV_ROW && mappedFile[fileSize - 1] != ',' && mappedFile[fileSize - 1] != ' ') {
            list_pop(outputList);
        }
    } else {
        unitype field;
        if (fieldType == OSTOOLS_CSV_FIELD_DOUBLE) {
            sscanf((char *) (mappedFile + leftIndex), "%lf", (double *) &field);
        } else if (fieldType == OSTOOLS_CSV_FIELD_INT) {
            sscanf((char *) (mappedFile + leftIndex), "%d", (int *) &field);
        } else if (fieldType == OSTOOLS_CSV_FIELD_STRING) {
            field.s = malloc(rightIndex - leftIndex + 1);
            sscanf((char *) (mappedFile + leftIndex), "%s", field.s);
        }
        list_append(outputList -> data[outputList -> length - 1].r, field, listType);
    }
    osToolsUnmapFile(mappedFile);
    return outputList;
}

/* packages a CSV file into a list (headers are strings, all fields are doubles) - use OSTOOLS_CSV_ROW to put it in a list of lists where each list is a row of the CSV and use OSTOOLS_CSV_COLUMN to output a list of lists where each list is a column of the CSV */
list_t *osToolsLoadCSV(char *filename, osToolsCSV rowOrColumn) {
    return osToolsLoadInternal(filename, rowOrColumn, OSTOOLS_CSV, OSTOOLS_CSV_FIELD_DOUBLE);
}

/* packages a TSV file into a list (headers are strings, all fields are doubles) - use OSTOOLS_CSV_ROW to put it in a list of lists where each list is a row of the TSV and use OSTOOLS_CSV_COLUMN to output a list of lists where each list is a column of the TSV */
list_t *osToolsLoadTSV(char *filename, osToolsCSV rowOrColumn) {
    return osToolsLoadInternal(filename, rowOrColumn, OSTOOLS_TSV, OSTOOLS_CSV_FIELD_DOUBLE);
}

/* packages a CSV file into a list (headers are strings, all fields are doubles) - use OSTOOLS_CSV_ROW to put it in a list of lists where each list is a row of the CSV and use OSTOOLS_CSV_COLUMN to output a list of lists where each list is a column of the CSV */
list_t *osToolsLoadCSVDouble(char *filename, osToolsCSV rowOrColumn) {
    return osToolsLoadInternal(filename, rowOrColumn, OSTOOLS_CSV, OSTOOLS_CSV_FIELD_DOUBLE);
}

/* packages a TSV file into a list (headers are strings, all fields are doubles) - use OSTOOLS_CSV_ROW to put it in a list of lists where each list is a row of the TSV and use OSTOOLS_CSV_COLUMN to output a list of lists where each list is a column of the TSV */
list_t *osToolsLoadTSVDouble(char *filename, osToolsCSV rowOrColumn) {
    return osToolsLoadInternal(filename, rowOrColumn, OSTOOLS_TSV, OSTOOLS_CSV_FIELD_DOUBLE);
}

/* packages a CSV file into a list (headers are strings, all fields are ints) - use OSTOOLS_CSV_ROW to put it in a list of lists where each list is a row of the CSV and use OSTOOLS_CSV_COLUMN to output a list of lists where each list is a column of the CSV */
list_t *osToolsLoadCSVInt(char *filename, osToolsCSV rowOrColumn) {
    return osToolsLoadInternal(filename, rowOrColumn, OSTOOLS_CSV, OSTOOLS_CSV_FIELD_INT);
}

/* packages a TSV file into a list (headers are strings, all fields are ints) - use OSTOOLS_CSV_ROW to put it in a list of lists where each list is a row of the TSV and use OSTOOLS_CSV_COLUMN to output a list of lists where each list is a column of the TSV */
list_t *osToolsLoadTSVInt(char *filename, osToolsCSV rowOrColumn) {
    return osToolsLoadInternal(filename, rowOrColumn, OSTOOLS_TSV, OSTOOLS_CSV_FIELD_INT);
}

/* packages a CSV file into a list (headers are strings, all fields are strings) - use OSTOOLS_CSV_ROW to put it in a list of lists where each list is a row of the CSV and use OSTOOLS_CSV_COLUMN to output a list of lists where each list is a column of the CSV */
list_t *osToolsLoadCSVString(char *filename, osToolsCSV rowOrColumn) {
    return osToolsLoadInternal(filename, rowOrColumn, OSTOOLS_CSV, OSTOOLS_CSV_FIELD_STRING);
}

/* packages a TSV file into a list (headers are strings, all fields are strings) - use OSTOOLS_CSV_ROW to put it in a list of lists where each list is a row of the TSV and use OSTOOLS_CSV_COLUMN to output a list of lists where each list is a column of the TSV */
list_t *osToolsLoadTSVString(char *filename, osToolsCSV rowOrColumn) {
    return osToolsLoadInternal(filename, rowOrColumn, OSTOOLS_TSV, OSTOOLS_CSV_FIELD_STRING);
}

#ifdef OS_WINDOWS
#include <windows.h>
#include <shobjidl.h>

int32_t osToolsInit(char argv0[], GLFWwindow *window) {
    osToolsIndependentInit(window);
    /* get executable filepath */
    GetModuleFileNameA(NULL, osToolsFileDialog.executableFilepath, MAX_PATH);
    if (GetLastError() != ERROR_SUCCESS) {
        strcpy(osToolsFileDialog.executableFilepath, "null");
        printf("Error: Could not retrieve executable filepath\n");
    }
    int32_t index = strlen(osToolsFileDialog.executableFilepath) - 1;
    while (index > -1 && osToolsFileDialog.executableFilepath[index] != '\\' && osToolsFileDialog.executableFilepath[index] != '/') {
        index--;
    }
    osToolsFileDialog.executableFilepath[index + 1] = '\0';
    /* initialise file dialog */
    strcpy(osToolsFileDialog.selectedFilename, "null");
    osToolsFileDialog.openOrSave = 0; // open by default
    osToolsFileDialog.numExtensions = 0; // 0 means all extensions
    osToolsFileDialog.extensions = malloc(1 * sizeof(char *)); // malloc list

    /* initialise clipboard */
    osToolsClipboard.text = glfwGetClipboardString(osToolsGLFW.osToolsWindow);
    return 0;
}

void osToolsFileDialogAddExtension(char *extension) {
    if (strlen(extension) <= 4) {
        osToolsFileDialog.numExtensions += 1;
        osToolsFileDialog.extensions = realloc(osToolsFileDialog.extensions, osToolsFileDialog.numExtensions * sizeof(char *));
        osToolsFileDialog.extensions[osToolsFileDialog.numExtensions - 1] = strdup(extension);
    } else {
        printf("extension name: %s too long\n", extension);
    }
}

int32_t osToolsFileDialogPrompt(char openOrSave, char *filename) { // 0 - open, 1 - save, filename refers to autofill filename ("null" or empty string for no autofill)
    osToolsFileDialog.openOrSave = openOrSave;
    HRESULT hr = CoInitializeEx(NULL, 0); // https://learn.microsoft.com/en-us/windows/win32/api/objbase/ne-objbase-coinit
    if (SUCCEEDED(hr)) {
        IFileDialog *fileDialog;
        IShellItem *psiResult;
        PWSTR pszFilePath = NULL;
        hr = CoCreateInstance(&CLSID_FileOpenDialog, NULL, CLSCTX_ALL, &IID_IFileOpenDialog, (void**) &fileDialog);
        if (SUCCEEDED(hr)) {
            fileDialog -> lpVtbl -> SetOptions(fileDialog, 0); // https://learn.microsoft.com/en-us/windows/win32/api/shobjidl_core/ne-shobjidl_core-_fileopendialogoptions from my tests these don't seem to do anything

            /* configure autofill filename */
            if (openOrSave == 1 && strcmp(filename, "null") != 0) {
                int32_t i = 0;
                unsigned short prename[MAX_PATH + 1];
                while (filename[i] != '\0' && i < MAX_PATH + 1) {
                    prename[i] = filename[i]; // convert from char to WCHAR
                    i++;
                }
                prename[i] = '\0';
                fileDialog -> lpVtbl -> SetFileName(fileDialog, prename);
            }

            /* load file restrictions
            Info: each COMDLG creates one more entry to the dropdown to the right of the text box in the file dialog window
            You can only see files that are specified in the types on the current COMDLG_FILTERSPEC selected in the dropdown
            Thats why I shove all the types into one COMDLG_FILTERSPEC, because I want the user to be able to see all compatible files at once
            */
            if (osToolsFileDialog.numExtensions > 0) {
                COMDLG_FILTERSPEC *fileExtensions = malloc(sizeof(COMDLG_FILTERSPEC)); // just one filter
                WCHAR *buildFilter = malloc(10 * osToolsFileDialog.numExtensions * sizeof(WCHAR));
                int32_t j = 0;
                for (int32_t i = 0; i < osToolsFileDialog.numExtensions; i++) {
                    buildFilter[j] = (unsigned short) '*';
                    buildFilter[j + 1] = (unsigned short) '.';
                    j += 2;
                    for (uint32_t k = 0; k < strlen(osToolsFileDialog.extensions[i]) && k < 8; k++) {
                        buildFilter[j] = osToolsFileDialog.extensions[i][k];
                        j += 1;
                    }
                    buildFilter[j] = (unsigned short) ';';
                    j += 1;
                }
                buildFilter[j] = (unsigned short) '\0';
                (*fileExtensions).pszName = L"Specified Types";
                (*fileExtensions).pszSpec = buildFilter;
                fileDialog -> lpVtbl -> SetFileTypes(fileDialog, 1, fileExtensions);
                free(buildFilter);
                free(fileExtensions);
            }

            /* configure title and button text */
            if (openOrSave == 0) {
                /* open */
                fileDialog -> lpVtbl -> SetOkButtonLabel(fileDialog, L"Open");
                fileDialog -> lpVtbl -> SetTitle(fileDialog, L"Open");
            } else {
                /* save */
                fileDialog -> lpVtbl -> SetOkButtonLabel(fileDialog, L"Save");
                fileDialog -> lpVtbl -> SetTitle(fileDialog, L"Save");
            }

            /* execute */
            fileDialog -> lpVtbl -> Show(fileDialog, NULL); // opens window
            hr = fileDialog -> lpVtbl -> GetResult(fileDialog, &psiResult); // succeeds if a file is selected
            if (SUCCEEDED(hr)){
                hr = psiResult -> lpVtbl -> GetDisplayName(psiResult, SIGDN_FILESYSPATH, &pszFilePath); // extracts path name
                if (SUCCEEDED(hr)) {
                    int32_t i = 0;
                    /* convert from WCHAR to char */
                    while (pszFilePath[i] != '\0' && i < MAX_PATH + 1) {
                        osToolsFileDialog.selectedFilename[i] = pszFilePath[i];
                        i++;
                    }
                    osToolsFileDialog.selectedFilename[i] = '\0';
                    CoTaskMemFree(pszFilePath);
                    return 0;
                }
                psiResult -> lpVtbl -> Release(psiResult);
            }
            fileDialog -> lpVtbl -> Release(fileDialog);
        } else {
            printf("ERROR - HRESULT: %lx\n", hr);
        }
        CoUninitialize();
    }
    return -1;
}

uint8_t *osToolsMapFile(char *filename, uint32_t *sizeOutput) {
    HANDLE fileHandle = CreateFileA(filename, FILE_GENERIC_READ | FILE_GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (fileHandle == INVALID_HANDLE_VALUE) {
        printf("Could not open file %ld\n", GetLastError());
        *sizeOutput = 0;
        return NULL;
    }
    if (!GetFileSizeEx(fileHandle, (PLARGE_INTEGER) sizeOutput)) {
        printf("Failed to get size of file\n");
        CloseHandle(fileHandle);
        *sizeOutput = 0;
        return NULL;
    }
    HANDLE mappingHandle = CreateFileMappingA(fileHandle, NULL, PAGE_READWRITE, 0, 0, NULL);
    if (mappingHandle == NULL) {
        printf("Could not memory map file %ld\n", GetLastError());
        CloseHandle(fileHandle);
        *sizeOutput = 0;
        return NULL;
    }
    LPVOID address = MapViewOfFile(mappingHandle, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    if (address == NULL) {
        printf("Could not create map view of file %ld\n", GetLastError());
        CloseHandle(fileHandle);
        CloseHandle(mappingHandle);
        *sizeOutput = 0;
        return NULL;
    }
    list_append(osToolsMemmap.mappedFiles, (unitype) filename, 's'); // filename
    list_append(osToolsMemmap.mappedFiles, (unitype) fileHandle, 'l'); // file handle (uint64_t so no free on list_delete)
    list_append(osToolsMemmap.mappedFiles, (unitype) mappingHandle, 'l'); // mapping handle (uint64_t so no free on list_delete)
    list_append(osToolsMemmap.mappedFiles, (unitype) address, 'l'); // file data (uint64_t so no free on list_delete)
    return address;
}

int32_t osToolsUnmapFile(uint8_t *data) {
    UnmapViewOfFile(data);
    int32_t index = -1;
    for (uint32_t i = 0; i < osToolsMemmap.mappedFiles -> length; i += 4) {
        if (osToolsMemmap.mappedFiles -> data[i + 3].p == data) {
            index = i;
            break;
        }
    }
    if (index >= 0) {
        CloseHandle(osToolsMemmap.mappedFiles -> data[index + 1].p);
        CloseHandle(osToolsMemmap.mappedFiles -> data[index + 2].p);
        list_delete(osToolsMemmap.mappedFiles, index);
        list_delete(osToolsMemmap.mappedFiles, index);
        list_delete(osToolsMemmap.mappedFiles, index);
        list_delete(osToolsMemmap.mappedFiles, index);
        return 0;
    } else {
        printf("Could not find %p in memory mapped index\n", data);
        return -1;
    }
}

#endif
#ifdef OS_LINUX
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

/* This is the zenity version of osToolsFileDialog.h, it's for linux */

/* 
a note on zenity:
FILE* filenameStream = popen("zenity --file-selection", "r"); (popen runs the first argument in the shell as a bash script)
this function returns a pointer to a stream of data that contains only the filepath
popen with (zenity --file-selection) will not return a FILE* to the location of the file. You cannot read the file from this FILE*, you must call fopen on the filename

additionally, filters can be added with
FILE* filenameStream = popen("zenity --file-selection --file-filter='Name | *.ext *.ext2 *.ext3'", "r");
This is similar to COMDLG_FILTERSPEC struct's pszName and pszSpec, so you can add more filter "profiles" by using multiple --file-filter tags in the command
*/

void osToolsInit(char argv0[], GLFWwindow *window) {
    osToolsIndependentInit(window);
    /* get executable filepath */
    FILE *exStringFile = popen("pwd", "r");
    fscanf(exStringFile, "%s", osToolsFileDialog.executableFilepath);
    strcat(osToolsFileDialog.executableFilepath, "/");
    strcat(osToolsFileDialog.executableFilepath, argv0);
    
    int32_t index = strlen(osToolsFileDialog.executableFilepath) - 1;
    while (index > -1 && osToolsFileDialog.executableFilepath[index] != '/') {
        index--;
    }
    osToolsFileDialog.executableFilepath[index + 1] = '\0';

    /* initialise file dialog */
    strcpy(osToolsFileDialog.selectedFilename, "null");
    osToolsFileDialog.openOrSave = 0; // open by default
    osToolsFileDialog.numExtensions = 0; // 0 means all extensions
    osToolsFileDialog.extensions = malloc(1 * sizeof(char *)); // malloc list
}

void osToolsFileDialogAddExtension(char *extension) {
    if (strlen(extension) <= 4) {
        osToolsFileDialog.numExtensions += 1;
        osToolsFileDialog.extensions = realloc(osToolsFileDialog.extensions, osToolsFileDialog.numExtensions * 8);
        osToolsFileDialog.extensions[osToolsFileDialog.numExtensions - 1] = strdup(extension);
    } else {
        printf("extension name: %s too long\n", extension);
    }
}

int32_t osToolsFileDialogPrompt(char openOrSave, char *prename) { // 0 - open, 1 - save, prename refers to autofill filename ("null" or empty string for no autofill)
    char fullCommand[23 + 13 + 256 + 15 + 34 + 7 * osToolsFileDialog.numExtensions + 14 + 1]; // 23 for zenity --file-selection, 13 for --filename=', 256 for prename, 15 for --title='Open', 34 for --file-filter='Specified Types | , 7 for each extension, 14 for title, 1 for \0
    strcpy(fullCommand, "zenity --file-selection");
    /* configure autofill filename */
    if (openOrSave == 1 && strcmp(prename, "null") != 0) {
        strcat(fullCommand, " --filename='");
        strcat(fullCommand, prename);
        strcat(fullCommand, "'");
    }

    /* configure title */
    char title[16] = " --title='Open'";
    if (openOrSave == 1) {
        strcpy(title, " --title='Save'");
    }
    strcat(fullCommand, title);

    /* configure extensions */
    if (osToolsFileDialog.numExtensions > 0) {
        char buildFilter[7 * osToolsFileDialog.numExtensions + 1]; // last space is replaced with ' and followed by \0
        int32_t j = 0;
        for (int32_t i = 0; i < osToolsFileDialog.numExtensions; i++) {
            buildFilter[j] = '*';
            buildFilter[j + 1] = '.';
            j += 2;
            for (uint32_t k = 0; k < strlen(osToolsFileDialog.extensions[i]) && k < 8; k++) {
                buildFilter[j] = osToolsFileDialog.extensions[i][k];
                j += 1;
            }
            if (i != osToolsFileDialog.numExtensions - 1) { // dont add space if it's the last element
                buildFilter[j] = ' ';
                j += 1;
            }
        }
        buildFilter[j] = '\'';
        buildFilter[j + 1] = '\0';
        char filterName[35] = " --file-filter='Specified Types | ";
        strcat(fullCommand, filterName);
        strcat(fullCommand, buildFilter);
    }

    /* execute */
    FILE* filenameStream = popen(fullCommand, "r");
    if (fgets(osToolsFileDialog.selectedFilename, 4097, filenameStream) == NULL) { // adds a \n before \0 (?)
        strcpy(osToolsFileDialog.selectedFilename, "null");
        return -1;
    }
    for (uint32_t i = 0; i < 4096; i++) {
        if (osToolsFileDialog.selectedFilename[i] == '\n') {
            osToolsFileDialog.selectedFilename[i] = '\0'; // replace all newlines with null characters
        }
    }
    pclose(filenameStream);
    return 0;
}

uint8_t *osToolsMapFile(char *filename, uint32_t *sizeOutput) {
    int32_t fd = open(filename, O_RDWR);
    struct stat stats;
    if (fstat(fd, &stats) == -1) {
        printf("Could not get stats of file %s\n", filename);
        *sizeOutput = 0;
        return NULL;
    }
    *sizeOutput = stats.st_size;
    void *out = mmap(NULL, *sizeOutput, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (out == MAP_FAILED) {
        printf("Could not memory map file %s\n", filename);
        *sizeOutput = 0;
        return NULL;
    }
    list_append(osToolsMemmap.mappedFiles, (unitype) out, 'l');
    list_append(osToolsMemmap.mappedFiles, (unitype) *sizeOutput, 'i');
    return (uint8_t *) out;
}

int32_t osToolsUnmapFile(uint8_t *data) {
    int32_t index = -1;
    for (uint32_t i = 0; i < osToolsMemmap.mappedFiles -> length; i += 2) {
        if (osToolsMemmap.mappedFiles -> data[i].p == data) {
            index = i;
            break;
        }
    }
    if (index >= 0) {
        munmap(data, osToolsMemmap.mappedFiles -> data[index + 1].i);
        return 0;
    } else {
        printf("Could not find %p in memory mapped index\n", data);
        return -1;
    }
}

#endif
#endif