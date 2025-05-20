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
#include "glfw3.h"

GLFWwindow *osToolsWindow;

#ifdef OS_WINDOWS
#include <windows.h>
#include <shobjidl.h>

typedef struct {
    char executableFilepath[MAX_PATH + 1]; // filepath of executable
    char selectedFilename[MAX_PATH + 1]; // output filename - maximum filepath is 260 characters (?)
    char openOrSave; // 0 - open, 1 - save
    int32_t numExtensions; // number of extensions
    char **extensions; // array of allowed extensions (7 characters long max (cuz *.json;))
} win32FileDialogObject; // almost as bad of naming as the windows API

typedef struct {
    char *text; // clipboard text data (heap allocated)
} win32ClipboardObject;

win32FileDialogObject osFileDialog;
win32ClipboardObject osClipboard;

int32_t osToolsInit(char argv0[], GLFWwindow *window) {
    osToolsWindow = window;
    /* get executable filepath */
    GetModuleFileNameA(NULL, osFileDialog.executableFilepath, MAX_PATH);
    if (GetLastError() != ERROR_SUCCESS) {
        strcpy(osFileDialog.executableFilepath, "null");
        printf("error: could not retrieve executable filepath\n");
    }
    int32_t index = strlen(osFileDialog.executableFilepath) - 1;
    while (index > -1 && osFileDialog.executableFilepath[index] != '\\' && osFileDialog.executableFilepath[index] != '/') {
        index--;
    }
    osFileDialog.executableFilepath[index + 1] = '\0';
    /* initialise file dialog */
    strcpy(osFileDialog.selectedFilename, "null");
    osFileDialog.openOrSave = 0; // open by default
    osFileDialog.numExtensions = 0; // 0 means all extensions
    osFileDialog.extensions = malloc(1 * sizeof(char *)); // malloc list

    /* initialise clipboard */
    if (!OpenClipboard(NULL)) { // initialises win32Clipboard.text as clipboard text data
        printf("error: could not open clipboard (windows)\n");
        return -1;
    }
    HANDLE clipboardHandle = GetClipboardData(CF_TEXT);
    LPTSTR wstrData;
    if (clipboardHandle != NULL) {
        wstrData = GlobalLock(clipboardHandle);
        if (wstrData != NULL) {
            uint32_t i = 0;
            uint32_t dynMem = 8; // start with 7 characters
            osClipboard.text = malloc(dynMem);
            while (wstrData[i] != '\0' && i < 4294967295) {
                osClipboard.text[i] = wstrData[i]; // convert from WCHAR to char
                i++;
                if (i >= dynMem) { // if i is eight we need to realloc to at least 9
                    dynMem *= 2;
                    osClipboard.text = realloc(osClipboard.text, dynMem);
                }
            }
            osClipboard.text[i] = '\0';
            GlobalUnlock(clipboardHandle);
        } else {
            printf("error: could not lock clipboard\n");
            CloseClipboard();
            return -1;
        }
    } else {
        printf("error: could not read from clipboard\n");
        CloseClipboard();
        return -1;
    }
    CloseClipboard();
    return 0;
}

int32_t osClipboardGetText() { // gets the text from win32Clipboard
    free(osClipboard.text);
    if (!OpenClipboard(NULL)) { // https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-openclipboard
        printf("error: could not open clipboard\n");
        return -1;
    }
    HANDLE clipboardHandle = GetClipboardData(CF_TEXT); // https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getclipboarddata
    LPTSTR wstrData; // WCHAR string
    if (clipboardHandle != NULL) {
        wstrData = GlobalLock(clipboardHandle); // https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-globallock
        if (wstrData != NULL) {
            uint32_t i = 0;
            uint32_t dynMem = 8; // start with 7 characters
            osClipboard.text = malloc(dynMem);
            while (wstrData[i] != '\0' && i < 4294967295) {
                osClipboard.text[i] = wstrData[i]; // convert from WCHAR to char
                i++;
                if (i >= dynMem) { // if i is eight we need to realloc to at least 9
                    dynMem *= 2;
                    osClipboard.text = realloc(osClipboard.text, dynMem);
                }
            }
            osClipboard.text[i] = '\0';
            GlobalUnlock(clipboardHandle);
        } else {
            printf("error: could not lock clipboard\n");
            CloseClipboard();
            return -1;
        }
    } else {
        printf("error: could not read from clipboard\n");
        CloseClipboard();
        return -1;
    }
    CloseClipboard();
    return 0;
}

int32_t osClipboardSetText(const char *input) { // takes null terminated strings
    if (!OpenClipboard(NULL)) { // technically (according to windows documentation) I should get the HWND (window handle) for the GLFW window, but that requires using the glfw3native.h header which would require lots of rewrites and endanger cross-platform compatibility
        printf("error: could not open clipboard\n");
        return -1;
    }
    uint32_t dynMem = strlen(input) + 1; // +1 for the null character
    /* GlobalAlloc is like malloc but windows */
    /* "Handles" are like pointers to data but not directly, you have to "GlobalLock" to actually access the data */
    /* GlobalAlloc allows you to alloc memory at the place that the Handle points to */
    HANDLE clipboardBufferHandle = GlobalAlloc(GMEM_MOVEABLE, dynMem); // https://learn.microsoft.com/en-us/windows/win32/sysinfo/handles-and-objects
    LPTSTR clipboardBufferObject = GlobalLock(clipboardBufferHandle); // WCHAR string
    for (uint32_t i = 0; i < dynMem; i++) {
        clipboardBufferObject[i] = input[i]; // convert from char to WCHAR
    }
    GlobalUnlock(clipboardBufferObject);
    /* Empty clipboard: Empties the clipboard and frees handles to data in the clipboard. The function then assigns ownership of the clipboard to the window that currently has the clipboard open. 
    This is a problem because our openClipboard window handle (HWND) is NULL, so the ownership doesn't get transferred, but it still works on my machine */
    if (!EmptyClipboard()) { // https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-emptyclipboard
        printf("error: could not empty clipboard\n");
        CloseClipboard();
        return -1;
    }
    if (SetClipboardData(CF_TEXT, clipboardBufferHandle) == NULL) { // https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setclipboarddata
        printf("error: could not set cliboard data\n");
        CloseClipboard();
        return -1;
    }
    CloseClipboard();
    return 0;
}

void osFileDialogAddExtension(char *extension) {
    if (strlen(extension) <= 4) {
        osFileDialog.numExtensions += 1;
        osFileDialog.extensions = realloc(osFileDialog.extensions, osFileDialog.numExtensions * sizeof(char *));
        osFileDialog.extensions[osFileDialog.numExtensions - 1] = strdup(extension);
    } else {
        printf("extension name: %s too long\n", extension);
    }
}

int32_t osFileDialogPrompt(char openOrSave, char *filename) { // 0 - open, 1 - save, filename refers to autofill filename ("null" or empty string for no autofill)
    osFileDialog.openOrSave = openOrSave;
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
            if (osFileDialog.numExtensions > 0) {
                COMDLG_FILTERSPEC *fileExtensions = malloc(sizeof(COMDLG_FILTERSPEC)); // just one filter
                WCHAR *buildFilter = malloc(10 * osFileDialog.numExtensions * sizeof(WCHAR));
                int32_t j = 0;
                for (int32_t i = 0; i < osFileDialog.numExtensions; i++) {
                    buildFilter[j] = (unsigned short) '*';
                    buildFilter[j + 1] = (unsigned short) '.';
                    j += 2;
                    for (uint32_t k = 0; k < strlen(osFileDialog.extensions[i]) && k < 8; k++) {
                        buildFilter[j] = osFileDialog.extensions[i][k];
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
            if (openOrSave == 0) { // open
                fileDialog -> lpVtbl -> SetOkButtonLabel(fileDialog, L"Open");
                fileDialog -> lpVtbl -> SetTitle(fileDialog, L"Open");
            } else { // save
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
                    while (pszFilePath[i] != '\0' && i < MAX_PATH + 1) {
                        osFileDialog.selectedFilename[i] = pszFilePath[i]; // convert from WCHAR to char
                        i++;
                    }
                    osFileDialog.selectedFilename[i] = '\0';
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

#define CURSOR_POINTER IDC_ARROW
#define CURSOR_LOADING IDC_WAIT
#define CURSOR_HAND IDC_HAND
#define CURSOR_MOVING IDC_SIZEALL
#define CURSOR_CROSS IDC_CROSS
#define CURSOR_UPDOWN IDC_SIZENS
#define CURSOR_SIDESIDE IDC_SIZEWE
#define CURSOR_DIAGONALLEFT IDC_SIZENWSE
#define CURSOR_DIAGONALRIGHT IDC_SIZENESW

void osSetCursor(LPCTSTR cursor) {
    HCURSOR hCursor = LoadCursor(NULL, cursor);
    SetCursor(hCursor);
}

void osHideAndLockCursor() {
    ShowCursor(0);
}

void osShowCursor() {
    ShowCursor(1);
}
#endif
#ifdef OS_LINUX

/* This is the zenity version of osFileDialog.h, it's for linux */

/* 
a note on zenity:
FILE* filenameStream = popen("zenity --file-selection", "r"); (popen runs the first argument in the shell as a bash script)
this function returns a pointer to a stream of data that contains only the filepath
popen with (zenity --file-selection) will not return a FILE* to the location of the file. You cannot read the file from this FILE*, you must call fopen on the filename

additionally, filters can be added with
FILE* filenameStream = popen("zenity --file-selection --file-filter='Name | *.ext *.ext2 *.ext3'", "r");
This is similar to COMDLG_FILTERSPEC struct's pszName and pszSpec, so you can add more filter "profiles" by using multiple --file-filter tags in the command
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char executableFilepath[4097]; // filepath of executable
    char selectedFilename[4097]; // output filename - maximum filepath is 4096 characters (?)
    char openOrSave; // 0 - open, 1 - save
    int32_t numExtensions; // number of extensions
    char **extensions; // array of allowed extensions (7 characters long max (cuz *.json ))
} zenityFileDialogObject;

typedef struct {
    char *text; // clipboard text data (heap allocated)
} xclipClipboardObject;

zenityFileDialogObject osFileDialog;
xclipClipboardObject osClipboard;

void osToolsInit(char argv0[], GLFWwindow *window) {
    osToolsWindow = window;
    /* get executable filepath */
    FILE *exStringFile = popen("pwd", "r");
    fscanf(exStringFile, "%s", osFileDialog.executableFilepath);
    strcat(osFileDialog.executableFilepath, "/");
    strcat(osFileDialog.executableFilepath, argv0);
    
    int32_t index = strlen(osFileDialog.executableFilepath) - 1;
    while (index > -1 && osFileDialog.executableFilepath[index] != '/') {
        index--;
    }
    osFileDialog.executableFilepath[index + 1] = '\0';

    /* initialise file dialog */
    strcpy(osFileDialog.selectedFilename, "null");
    osFileDialog.openOrSave = 0; // open by default
    osFileDialog.numExtensions = 0; // 0 means all extensions
    osFileDialog.extensions = malloc(1 * sizeof(char *)); // malloc list
}

/* gets the text */
int32_t osClipboardGetText() {
    osClipboard.text = strdup(glfwGetClipboardString(osToolsWindow));
    return 0;
}

/* takes null terminated strings */
int32_t osClipboardSetText(const char *input) {
    glfwSetClipboardString(osToolsWindow, input);
    return 0;
}

void osFileDialogAddExtension(char *extension) {
    if (strlen(extension) <= 4) {
        osFileDialog.numExtensions += 1;
        osFileDialog.extensions = realloc(osFileDialog.extensions, osFileDialog.numExtensions * 8);
        osFileDialog.extensions[osFileDialog.numExtensions - 1] = strdup(extension);
    } else {
        printf("extension name: %s too long\n", extension);
    }
}

int32_t osFileDialogPrompt(char openOrSave, char *prename) { // 0 - open, 1 - save, prename refers to autofill filename ("null" or empty string for no autofill)
    char fullCommand[23 + 13 + 256 + 15 + 34 + 7 * osFileDialog.numExtensions + 14 + 1]; // 23 for zenity --file-selection, 13 for --filename=', 256 for prename, 15 for --title='Open', 34 for --file-filter='Specified Types | , 7 for each extension, 14 for title, 1 for \0
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
    if (osFileDialog.numExtensions > 0) {
        char buildFilter[7 * osFileDialog.numExtensions + 1]; // last space is replaced with ' and followed by \0
        int32_t j = 0;
        for (int32_t i = 0; i < osFileDialog.numExtensions; i++) {
            buildFilter[j] = '*';
            buildFilter[j + 1] = '.';
            j += 2;
            for (uint32_t k = 0; k < strlen(osFileDialog.extensions[i]) && k < 8; k++) {
                buildFilter[j] = osFileDialog.extensions[i][k];
                j += 1;
            }
            if (i != osFileDialog.numExtensions - 1) { // dont add space if it's the last element
                buildFilter[j] = ' ';
                j += 1;
            }
        }
        buildFilter[j] = '\'';
        buildFilter[j + 1] = '\0';
        char filterName[35] = " --file-filter='Specified Types | ";
        strcat(fullCommand, filterName);
        strcat(fullCommand, buildFilter); // really glad that C is such a good language for string manipulation /s
    }

    /* execute */
    // printf("%s\n", fullCommand);
    FILE* filenameStream = popen(fullCommand, "r");
    if (fgets(osFileDialog.selectedFilename, 4097, filenameStream) == NULL) { // adds a \n before \0 (?)
        // printf("Error: fgets\n");
        strcpy(osFileDialog.selectedFilename, "null");
        return -1;
    }
    for (uint32_t i = 0; i < 4096; i++) {
        if (osFileDialog.selectedFilename[i] == '\n') {
            osFileDialog.selectedFilename[i] = '\0'; // replace all newlines with null characters
        }
    }
    // printf("Success, filename: %s\n", osFileDialog.filename);
    pclose(filenameStream);
    return 0;
}
#endif
#endif