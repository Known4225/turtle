/* TEMP TODO
- Figure out why readyFlag never says ready
- Fix delay on H264 decoder
- Fix green bar on bottom of image from H264 camera (and redshift)
- Fix upside down image (height-wise) on H264 image

https://community.nxp.com/t5/i-MX-Processors/1080p-h264-encoding-error/m-p/931107

Perhaps because the camera has 800x600 resolution and 600 is not divisible by 16 is why this issue is caused.
*/


/*
 ██████╗ ███████╗████████╗ ██████╗  ██████╗ ██╗     ███████╗    ██████╗
██╔═══██╗██╔════╝╚══██╔══╝██╔═══██╗██╔═══██╗██║     ██╔════╝   ██╔════╝
██║   ██║███████╗   ██║   ██║   ██║██║   ██║██║     ███████╗   ██║     
██║   ██║╚════██║   ██║   ██║   ██║██║   ██║██║     ╚════██║   ██║     
╚██████╔╝███████║   ██║   ╚██████╔╝╚██████╔╝███████╗███████║██╗╚██████╗
 ╚═════╝ ╚══════╝   ╚═╝    ╚═════╝  ╚═════╝ ╚══════╝╚══════╝╚═╝ ╚═════╝
https://patorjk.com/software/taag/#p=display&f=ANSI%20Shadow

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
Actually, we pass in a pointer to the object

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

#include "../include/osTools.h"

/* global objects */
ost_glfw_t osToolsGLFW;
ost_clipboard_t osToolsClipboard;
ost_file_dialog_t osToolsFileDialog;
ost_memmap_t osToolsMemmap;
ost_com_t osToolsCom;
ost_socket_t osToolsSocket;
ost_camera_t osToolsCamera;

/* OS independent functions */
void osToolsIndependentInit(GLFWwindow *window) {
    /* initialise file dialog */
    osToolsFileDialog.selectedFilenames = list_init();
    osToolsFileDialog.globalExtensions = list_init();
    /* initialise clipboard */
    osToolsClipboard.text = glfwGetClipboardString(osToolsGLFW.osToolsWindow);
    /* initialise glfw cursors */
    osToolsGLFW.osToolsWindow = window;
    osToolsGLFW.standardCursors[0] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    osToolsGLFW.standardCursors[1] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
    osToolsGLFW.standardCursors[2] = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
    osToolsGLFW.standardCursors[3] = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
    osToolsGLFW.standardCursors[4] = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
    osToolsGLFW.standardCursors[5] = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
    /* initialise non-standard cursors */
    GLFWimage leftDiagonalCursor;
    leftDiagonalCursor.width = 32;
    leftDiagonalCursor.height = 32;
    leftDiagonalCursor.pixels = (unsigned char *) leftDiagonalCursorData;
    osToolsGLFW.standardCursors[6] = glfwCreateCursor(&leftDiagonalCursor, 8, 8);
    GLFWimage rightDiagonalCursor;
    rightDiagonalCursor.width = 32;
    rightDiagonalCursor.height = 32;
    rightDiagonalCursor.pixels = (unsigned char *) rightDiagonalCursorData;
    osToolsGLFW.standardCursors[7] = glfwCreateCursor(&rightDiagonalCursor, 8, 8);
    GLFWimage moveCursor;
    moveCursor.width = 32;
    moveCursor.height = 32;
    moveCursor.pixels = (unsigned char *) moveCursorData;
    osToolsGLFW.standardCursors[8] = glfwCreateCursor(&moveCursor, 12, 12);
    /* initialise memmap module */
    osToolsMemmap.mappedFiles = list_init();
    /* initialise COM module */
    osToolsCom.com = list_init();
    /* initialise sockets module */
    osToolsSocket.socket = list_init();
    osToolsSocket.win32wsaActive = 0;
    /* initialise camera module */
    osToolsCamera.camera = list_init();
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
GLFW_DLESIZE_CURSOR
GLFW_DRESIZE_CURSOR
GLFW_MOVE_CURSOR
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
    case GLFW_DLESIZE_CURSOR:
        glfwSetCursor(osToolsGLFW.osToolsWindow, osToolsGLFW.standardCursors[6]);
    break;
    case GLFW_DRESIZE_CURSOR:
        glfwSetCursor(osToolsGLFW.osToolsWindow, osToolsGLFW.standardCursors[7]);
    break;
    case GLFW_MOVE_CURSOR:
        glfwSetCursor(osToolsGLFW.osToolsWindow, osToolsGLFW.standardCursors[8]);
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

list_t *osToolsLoadInternal(char *filename, ost_csv_t rowOrColumn, char delimeter, ost_csv_field_t fieldType) {
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
    int8_t inQuotes = 0;
    while (rightIndex < fileSize) {
        if (mappedFile[rightIndex] & 0b10000000) {
            /* case: unicode */
            if (mappedFile[rightIndex] & 0b11100000) {
                if (mappedFile[rightIndex] & 0b11110000) {
                    rightIndex++;
                }
                rightIndex++;
            }
            rightIndex += 2; // multi-byte character cannot be a comma, quote, or newline
        }
        if (mappedFile[rightIndex] == '"') {
            /* case: quote */
            inQuotes = !inQuotes;
            if (inQuotes) {
                // leftIndex = rightIndex + 1;
            }
        }
        /* case: comma */
        if (mappedFile[rightIndex] == delimeter && !inQuotes) {
            mappedFile[rightIndex] = '\0';
            if (rowOrColumn == OSTOOLS_CSV_ROW) {
                list_append(outputList -> data[0].r, (unitype) (char *) (mappedFile + leftIndex), 's');
            } else {
                list_append(outputList -> data[outputList -> length - 1].r, (unitype) (char *) (mappedFile + leftIndex), 's');
                list_append(outputList, (unitype) list_init(), 'r');
            }
            mappedFile[rightIndex] = delimeter;
            leftIndex = rightIndex + 1;
            while (mappedFile[leftIndex] == ' ') {
                rightIndex++;
                leftIndex++;
            }
        }
        if ((mappedFile[rightIndex] == '\n' || mappedFile[rightIndex] == '\r') && !inQuotes) {
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
    inQuotes = 0;
    while (rightIndex < fileSize) {
        if (mappedFile[rightIndex] & 0b10000000) {
            /* case: unicode */
            if (mappedFile[rightIndex] & 0b11100000) {
                if (mappedFile[rightIndex] & 0b11110000) {
                    rightIndex++;
                }
                rightIndex++;
            }
            rightIndex += 2; // multi-byte character cannot be a comma, quote, or newline
        }
        if (mappedFile[rightIndex] == '"') {
            /* case: quote */
            inQuotes = !inQuotes;
            if (inQuotes) {
                // leftIndex = rightIndex + 1;
            }
        }
        if (mappedFile[rightIndex] == delimeter && !inQuotes) {
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
            mappedFile[rightIndex] = delimeter;
            leftIndex = rightIndex + 1;
            while (mappedFile[leftIndex] == ' ') {
                rightIndex++;
                leftIndex++;
            }
            column++;
        }
        if ((mappedFile[rightIndex] == '\n' || mappedFile[rightIndex] == '\r') && !inQuotes) {
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
            rightIndex--;
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
        if (rowOrColumn == OSTOOLS_CSV_ROW && mappedFile[fileSize - 1] != delimeter && mappedFile[fileSize - 1] != ' ') {
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
list_t *osToolsLoadCSV(char *filename, ost_csv_t rowOrColumn) {
    return osToolsLoadInternal(filename, rowOrColumn, ',', OSTOOLS_CSV_FIELD_DOUBLE);
}

/* packages a CSV file into a list (headers are strings, all fields are doubles) - use OSTOOLS_CSV_ROW to put it in a list of lists where each list is a row of the CSV and use OSTOOLS_CSV_COLUMN to output a list of lists where each list is a column of the CSV */
list_t *osToolsLoadCSVDouble(char *filename, ost_csv_t rowOrColumn) {
    return osToolsLoadInternal(filename, rowOrColumn, ',', OSTOOLS_CSV_FIELD_DOUBLE);
}

/* packages a CSV file into a list (headers are strings, all fields are ints) - use OSTOOLS_CSV_ROW to put it in a list of lists where each list is a row of the CSV and use OSTOOLS_CSV_COLUMN to output a list of lists where each list is a column of the CSV */
list_t *osToolsLoadCSVInt(char *filename, ost_csv_t rowOrColumn) {
    return osToolsLoadInternal(filename, rowOrColumn, ',', OSTOOLS_CSV_FIELD_INT);
}

/* packages a CSV file into a list (headers are strings, all fields are strings) - use OSTOOLS_CSV_ROW to put it in a list of lists where each list is a row of the CSV and use OSTOOLS_CSV_COLUMN to output a list of lists where each list is a column of the CSV */
list_t *osToolsLoadCSVString(char *filename, ost_csv_t rowOrColumn) {
    return osToolsLoadInternal(filename, rowOrColumn, ',', OSTOOLS_CSV_FIELD_STRING);
}

void osToolsFileDialogClearGlobalExtensions() {
    list_clear(osToolsFileDialog.globalExtensions);
}

void osToolsFileDialogAddGlobalExtension(char *extension) {
    list_append(osToolsFileDialog.globalExtensions, (unitype) extension, 's');
}

void osToolsFileDialogSetGlobalExtensions(list_t *extensions) {
    list_copy(osToolsFileDialog.globalExtensions, extensions);
}

int32_t osToolsFileDialogSave(ost_file_dialog_folder_t folder, char *prename, list_t *extensions) {
    return osToolsFileDialogPrompt(OSTOOLS_FILE_DIALOG_SAVE, OSTOOLS_FILE_DIALOG_SINGLE_SELECT, folder, prename, extensions);
}

int32_t osToolsFileDialogOpen(ost_file_dialog_multiselect_t multiselect, ost_file_dialog_folder_t folder, char *prename, list_t *extensions) {
    return osToolsFileDialogPrompt(OSTOOLS_FILE_DIALOG_OPEN, multiselect, folder, prename, extensions);
}

#ifdef OS_WINDOWS

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
    return 0;
}

int32_t osToolsFileDialogPrompt(ost_file_dialog_save_t openOrSave, ost_file_dialog_multiselect_t multiselect, ost_file_dialog_folder_t folder, char *prename, list_t *extensions) {
    HRESULT hr = CoInitializeEx(NULL, 0); // https://learn.microsoft.com/en-us/windows/win32/api/objbase/ne-objbase-coinit
    if (FAILED(hr)) {
        return -1;
    }
    /* polymorphism in C */
    IFileOpenDialog *openDialog;
    IFileSaveDialog *saveDialog;
    IFileDialog *fileDialog;
    PWSTR pszFilePath = NULL;
    if (openOrSave == OSTOOLS_FILE_DIALOG_OPEN || folder == OSTOOLS_FILE_DIALOG_FOLDER) {
        hr = CoCreateInstance(&CLSID_FileOpenDialog, NULL, CLSCTX_ALL, &IID_IFileOpenDialog, (void **) &openDialog);
        fileDialog = (IFileDialog *) openDialog;
    } else if (openOrSave == OSTOOLS_FILE_DIALOG_SAVE) {
        hr = CoCreateInstance(&CLSID_FileSaveDialog, NULL, CLSCTX_ALL, &IID_IFileSaveDialog, (void **) &saveDialog);
        fileDialog = (IFileDialog *) saveDialog;
    }
    if (FAILED(hr)) {
        CoUninitialize();
        return -1;
    }
    FILEOPENDIALOGOPTIONS options = 0;
    if (openOrSave == OSTOOLS_FILE_DIALOG_SAVE) {
        /* prompt user if trying to overwrite file when saving */
        options |= FOS_OVERWRITEPROMPT;
    }
    if (openOrSave == OSTOOLS_FILE_DIALOG_OPEN && multiselect == OSTOOLS_FILE_DIALOG_MULTI_SELECT) {
        /* enable multiselect */
        options |= FOS_ALLOWMULTISELECT;
    }
    if (folder == OSTOOLS_FILE_DIALOG_FOLDER) {
        /* switch to folder-only prompt */
        options |= FOS_PICKFOLDERS;
    }
    fileDialog -> lpVtbl -> SetOptions(fileDialog, options); // https://learn.microsoft.com/en-us/windows/win32/api/shobjidl_core/ne-shobjidl_core-_fileopendialogoptions
    /* configure autofill filename */
    if (prename != NULL && strcmp(prename, "null") != 0 && strcmp(prename, "") != 0) {
        int32_t i = 0;
        WCHAR wprename[MAX_PATH + 1];
        while (prename[i] != '\0' && i < MAX_PATH + 1) {
            wprename[i] = prename[i]; // convert from char to WCHAR
            i++;
        }
        wprename[i] = '\0';
        fileDialog -> lpVtbl -> SetFileName(fileDialog, wprename);
    }

    /* load file restrictions
    Info: each COMDLG creates one more entry to the dropdown to the right of the text box in the file dialog window
    You can only see files that are specified in the types on the current COMDLG_FILTERSPEC selected in the dropdown
    Thats why I shove all the types into one COMDLG_FILTERSPEC, because I want the user to be able to see all compatible files at once
    */
    if (extensions == NULL) {
        extensions = osToolsFileDialog.globalExtensions;
    }
    if (extensions -> length > 0 && folder == 0) {
        COMDLG_FILTERSPEC *fileExtensions = malloc(sizeof(COMDLG_FILTERSPEC)); // just one filter
        int32_t totalMemory = 1;
        for (int32_t i = 0; i < extensions -> length; i++) {
            totalMemory += strlen(extensions -> data[i].s) + 3;
        }
        WCHAR *buildFilter = malloc(totalMemory * 2);
        int32_t j = 0;
        for (int32_t i = 0; i < extensions -> length; i++) {
            buildFilter[j] = (uint16_t) '*';
            buildFilter[j + 1] = (uint16_t) '.';
            j += 2;
            for (uint32_t k = 0; k < strlen(extensions -> data[i].s) && k < 8; k++) {
                buildFilter[j] = extensions -> data[i].s[k];
                j += 1;
            }
            buildFilter[j] = (uint16_t) ';';
            j += 1;
        }
        buildFilter[j] = (uint16_t) '\0';
        // printf("malloc'd %d bytes and used %d of them\n", totalMemory, j + 1);
        (*fileExtensions).pszName = L"Specified Types";
        (*fileExtensions).pszSpec = buildFilter;
        fileDialog -> lpVtbl -> SetFileTypes(fileDialog, 1, fileExtensions);
        free(buildFilter);
        free(fileExtensions);
    }

    /* configure title and button text */
    if (openOrSave == OSTOOLS_FILE_DIALOG_OPEN) {
        /* open */
        fileDialog -> lpVtbl -> SetOkButtonLabel(fileDialog, L"Open");
        fileDialog -> lpVtbl -> SetTitle(fileDialog, L"Open");
    } else if (openOrSave == OSTOOLS_FILE_DIALOG_SAVE) {
        /* save */
        fileDialog -> lpVtbl -> SetOkButtonLabel(fileDialog, L"Save");
        fileDialog -> lpVtbl -> SetTitle(fileDialog, L"Save");
    }

    /* execute */
    IShellItemArray *psiResultArray;
    IShellItem *psiResult;
    fileDialog -> lpVtbl -> Show(fileDialog, NULL); // opens window
    if (openOrSave == OSTOOLS_FILE_DIALOG_OPEN && multiselect == OSTOOLS_FILE_DIALOG_MULTI_SELECT) {
        hr = openDialog -> lpVtbl -> GetResults(openDialog, &psiResultArray); // succeeds if a file is selected
        if (FAILED(hr)) {
            printf("failed GetResults %ld\n", hr);
            fileDialog -> lpVtbl -> Release(fileDialog);
            CoUninitialize();
            return -1;
        }
        DWORD numberOfSelectedFiles;
        psiResultArray -> lpVtbl -> GetCount(psiResultArray, &numberOfSelectedFiles);
        list_clear(osToolsFileDialog.selectedFilenames);
        for (int32_t i = 0; i < numberOfSelectedFiles; i++) {
            IShellItem *psiResult;
            hr = psiResultArray -> lpVtbl -> GetItemAt(psiResultArray, 0, &psiResult);
            hr |= psiResult -> lpVtbl -> GetDisplayName(psiResult, SIGDN_FILESYSPATH, &pszFilePath); // extracts path name
            if (FAILED(hr)) {
                psiResult -> lpVtbl -> Release(psiResult);
                fileDialog -> lpVtbl -> Release(fileDialog);
                CoUninitialize();
                return -1;
            }
            int32_t i = 0;
            /* convert from WCHAR to char */
            int32_t pathLength = wcslen(pszFilePath);
            char addToSelectedFilenames[pathLength + 2];
            while (pszFilePath[i] != '\0' && i < MAX_PATH + 1) {
                addToSelectedFilenames[i] = pszFilePath[i];
                i++;
            }
            if (folder) {
                addToSelectedFilenames[i] = '\\';
                addToSelectedFilenames[i + 1] = '\0';
            } else {
                addToSelectedFilenames[i] = '\0';
            }
            list_append(osToolsFileDialog.selectedFilenames, (unitype) addToSelectedFilenames, 's');
            psiResult -> lpVtbl -> Release(psiResult);
            CoTaskMemFree(pszFilePath);
        }
    } else {
        hr = fileDialog -> lpVtbl -> GetResult(fileDialog, &psiResult); // succeeds if a file is selected
        if (FAILED(hr)) {
            printf("failed GetResult %ld\n", hr);
            fileDialog -> lpVtbl -> Release(fileDialog);
            CoUninitialize();
            return -1;
        }
        hr = psiResult -> lpVtbl -> GetDisplayName(psiResult, SIGDN_FILESYSPATH, &pszFilePath); // extracts path name
        if (FAILED(hr)) {
            psiResult -> lpVtbl -> Release(psiResult);
        }
        int32_t i = 0;
        /* convert from WCHAR to char */
        int32_t pathLength = wcslen(pszFilePath);
        char addToSelectedFilenames[pathLength + 2];
        while (pszFilePath[i] != '\0' && i < MAX_PATH + 1) {
            addToSelectedFilenames[i] = pszFilePath[i];
            i++;
        }
        if (folder) {
            addToSelectedFilenames[i] = '\\';
            addToSelectedFilenames[i + 1] = '\0';
        } else {
            addToSelectedFilenames[i] = '\0';
        }
        list_append(osToolsFileDialog.selectedFilenames, (unitype) addToSelectedFilenames, 's');
        psiResult -> lpVtbl -> Release(psiResult);
        CoTaskMemFree(pszFilePath);
    }
    fileDialog -> lpVtbl -> Release(fileDialog);
    CoUninitialize();
    return 0;
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

list_t *osToolsListFilesAndFolders(char *directory) {
    /* https://learn.microsoft.com/en-us/windows/win32/fileio/listing-the-files-in-a-directory */
    list_t *output = list_init();
    if (strlen(directory) > MAX_PATH - 3) {
        printf("osToolsListFiles: Directory name too long\n");
        return output;
    }
    char directoryFor[MAX_PATH];
    strcpy(directoryFor, directory);
    strcat(directoryFor, "\\*");
    WIN32_FIND_DATA findData;
    HANDLE fileHandle = FindFirstFile(directoryFor, &findData);
    if (fileHandle == INVALID_HANDLE_VALUE) {
        printf("osToolsListFiles: Handle invalid error %ld\n", GetLastError());
        return output;
    }
    LARGE_INTEGER filesize;
    do {
        if (strcmp(findData.cFileName, ".") == 0 || strcmp(findData.cFileName, "..") == 0) {
            continue;
        }
        list_append(output, (unitype) findData.cFileName, 's'); // add filename
        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            /* directory */
            list_append(output, (unitype) (int64_t) -1, LIST_TYPE_INT64);
        } else {
            filesize.LowPart = findData.nFileSizeLow;
            filesize.HighPart = findData.nFileSizeHigh;
            list_append(output, (unitype) (int64_t) filesize.QuadPart, LIST_TYPE_INT64);
        }
    } while (FindNextFile(fileHandle, &findData) != 0);
    return output;
}

list_t *osToolsListFiles(char *directory) {
    /* https://learn.microsoft.com/en-us/windows/win32/fileio/listing-the-files-in-a-directory */
    list_t *output = list_init();
    if (strlen(directory) > MAX_PATH - 3) {
        printf("osToolsListFiles: Directory name too long\n");
        return output;
    }
    char directoryFor[MAX_PATH];
    strcpy(directoryFor, directory);
    strcat(directoryFor, "\\*");
    WIN32_FIND_DATA findData;
    HANDLE fileHandle = FindFirstFile(directoryFor, &findData);
    if (fileHandle == INVALID_HANDLE_VALUE) {
        printf("osToolsListFiles: Handle invalid error %ld\n", GetLastError());
        return output;
    }
    LARGE_INTEGER filesize;
    do {
        if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            filesize.LowPart = findData.nFileSizeLow;
            filesize.HighPart = findData.nFileSizeHigh;
            list_append(output, (unitype) findData.cFileName, 's'); // add filename
            list_append(output, (unitype) (int64_t) filesize.QuadPart, LIST_TYPE_INT64);
        }
    } while (FindNextFile(fileHandle, &findData) != 0);
    /* sort list https://stackoverflow.com/questions/39048747/how-in-the-world-does-windows-file-explorer-sort-by-name 
    I'm actually just not going to sort them */
    return output;
}

list_t *osToolsListFolders(char *directory) {
    /* https://learn.microsoft.com/en-us/windows/win32/fileio/listing-the-files-in-a-directory */
    list_t *output = list_init();
    if (strlen(directory) > MAX_PATH - 3) {
        printf("osToolsListFolders: Directory name too long\n");
        return output;
    }
    char directoryFor[MAX_PATH];
    strcpy(directoryFor, directory);
    strcat(directoryFor, "\\*");
    WIN32_FIND_DATA findData;
    HANDLE fileHandle = FindFirstFile(directoryFor, &findData);
    if (fileHandle == INVALID_HANDLE_VALUE) {
        printf("osToolsListFolders: Handle invalid error %ld\n", GetLastError());
        return output;
    }
    do {
        if (strcmp(findData.cFileName, ".") == 0 || strcmp(findData.cFileName, "..") == 0) {
            continue;
        }
        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            /* directory */
            list_append(output, (unitype) findData.cFileName, 's'); // add filename
        }
    } while (FindNextFile(fileHandle, &findData) != 0);
    return output;
}

int32_t osToolsCreateFolder(char *folder) {
    /* https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-createdirectory */
    return CreateDirectory(folder, NULL);
}

int32_t osToolsDeleteFolder(char *folder) {
    /* https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-removedirectorya */
    char directoryFor[MAX_PATH + 10] = "rd /s /q ";
    int32_t len = strlen(folder);
    char *holdingCell = strdup(folder);
    for (int32_t i = 0; i < len; i++) {
        if (holdingCell[i] == '/') {
            holdingCell[i] = '\\';
        }
    }
    strcat(directoryFor, holdingCell);
    free(holdingCell);
    return system(directoryFor);
    // RemoveDirectoryA(folder);
}

void osToolsCloseConsole() {
    FreeConsole();
}

/*
windows COM port support
https://learn.microsoft.com/en-us/windows/win32/devio/configuring-a-communications-resource
*/

list_t *osToolsComList() {
    list_t *output = list_init();
    char comName[8] = "COM";
    char pathInfo[128];
    for (int32_t i = 0; i < 255; i++) {
        sprintf(comName + 3, "%d", i);
        DWORD deviceExist = QueryDosDeviceA(comName, pathInfo, 128);
        if (deviceExist != 0) {
            list_append(output, (unitype) comName, 's');
        }
        if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
            printf("osToolsComList: Unusual COM device detected on %s\n", comName);
        }
    }
    return output;
}

int32_t osToolsComOpen(char *name, osToolsComBaud_t baudRate) {
    /* verify COM */
    if (strlen(name) < 3 || name[0] != 'C' || name[1] != 'O' || name[2] != 'M') {
        // printf("osToolsComOpen: name must start with \"COM\"\n");
        return -1;
    }
    /* check if this port is already open */
    int32_t index = list_find(osToolsCom.com, (unitype) name, 's');
    if (index != -1) {
        if (CloseHandle((HANDLE) (osToolsCom.com -> data[index + 1].l)) == 0) {
            printf("osToolsComClose failed with error %ld\n", GetLastError());
            return -1;
        }
        list_delete(osToolsCom.com, index);
        list_delete(osToolsCom.com, index);
    }
    /* open COM port */
    DCB dcb;
    BOOL fSuccess;
    /* https://support.microsoft.com/en-us/topic/howto-specify-serial-ports-larger-than-com9-db9078a5-b7b6-bf00-240f-f749ebfd913e */
    char comName[24] = "\\\\.\\";
    if (strlen(name) > 4) {
        strcpy(comName + strlen(comName), name);
    } else {
        strcpy(comName, name);
    }
    HANDLE comHandle = CreateFileA(comName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (comHandle == INVALID_HANDLE_VALUE) {
        printf("Could not open com port %s, error %ld\n", name, GetLastError());
        return -1;
    } else {
        list_append(osToolsCom.com, (unitype) name, 's');
        list_append(osToolsCom.com, (unitype) comHandle, 'l');
    }
    /* Initialize the DCB structure. */
    SecureZeroMemory(&dcb, sizeof(DCB));
    dcb.DCBlength = sizeof(DCB);
    /* Build on the current configuration by first retrieving all current */
    /* settings. */
    fSuccess = GetCommState(comHandle, &dcb);
    if (!fSuccess) {
        /* Handle the error. */
        printf("GetCommState failed with error %ld\n", GetLastError());
        return -1;
    }
    /* Fill in some DCB values and set the com state: 
       115200 bps, 8 data bits, no parity, and 1 stop bit. */
    dcb.BaudRate = baudRate;      // baud rate
    dcb.ByteSize = 8;             // data size, xmit and rcv
    dcb.Parity   = NOPARITY;      // parity bit
    dcb.StopBits = ONESTOPBIT;    // stop bit
    fSuccess = SetCommState(comHandle, &dcb);
    if (!fSuccess) {
        /* Handle the error. */
        printf("SetCommState failed with error %ld\n", GetLastError());
        return -1;
    }
    /* Get the comm config again. */
    fSuccess = GetCommState(comHandle, &dcb);
    if (!fSuccess) {
        /* Handle the error. */
        printf("GetCommState failed with error %ld\n", GetLastError());
        return -1;
    }
    return 0;
}

int32_t osToolsComSend(char *name, uint8_t *data, int32_t length) {
    /* check if this port is open */
    int32_t index = list_find(osToolsCom.com, (unitype) name, 's');
    if (index == -1) {
        printf("osToolsComSend: %s not open\n", name);
        return 0;
    }
    /* https://www.codeproject.com/Articles/3061/Creating-a-Serial-communication-on-Win32#sending */
    DWORD bytes;
    if (WriteFile((HANDLE) (osToolsCom.com -> data[index + 1].l), data, length, &bytes, NULL) == 0) {
        printf("osToolsComSend failed with error %ld\n", GetLastError());
        return -1;
    }
    return bytes;
}

int32_t osToolsComReceive(char *name, uint8_t *buffer, int32_t length, int32_t timeoutMilliseconds) {
    /* check if this port is open */
    int32_t index = list_find(osToolsCom.com, (unitype) name, 's');
    if (index == -1) {
        printf("osToolsComReceive: %s not open\n", name);
        return 0;
    }
    /* Set comm timeout */
    COMMTIMEOUTS timeout = {0, 0, timeoutMilliseconds, 0, 0};
    SetCommTimeouts((HANDLE) (osToolsCom.com -> data[index + 1].l), &timeout);
    /* read from COM */
    DWORD bytes;
    if (ReadFile((HANDLE) (osToolsCom.com -> data[index + 1].l), buffer, length, &bytes, NULL) == 0) {
        printf("osToolsComReceive failed with error %ld\n", GetLastError());
        return -1;
    }
    return bytes;
}

int32_t osToolsComClose(char *name) {
    /* check if this port is already open */
    int32_t index = list_find(osToolsCom.com, (unitype) name, 's');
    if (index != -1) {
        if (CloseHandle((HANDLE) (osToolsCom.com -> data[index + 1].l)) == 0) {
            printf("osToolsComClose failed with error %ld\n", GetLastError());
            return -1;
        }
        list_delete(osToolsCom.com, index);
        list_delete(osToolsCom.com, index);
    }
    return 0;
}

/*
https://gist.github.com/mmozeiko/c0dfcc8fec527a90a02145d2cc0bfb6d
https://learn.microsoft.com/en-us/windows/win32/winsock/complete-server-code
https://learn.microsoft.com/en-us/windows/win32/winsock/complete-client-code
https://csperkins.org/teaching/2007-2008/networked-systems/lecture04.pdf
https://tangentsoft.com/wskfaq/examples/ipaddr.html
*/

int32_t osToolsGetIP(char *address, uint8_t *buffer, int32_t maxSegments) {
    char modifiable[strlen(address) + 1];
    strcpy(modifiable, address);
    char *check = strtok(modifiable, ".");
    int32_t segments = 0;
    while (check != NULL) {
        if (segments > maxSegments - 1) {
            printf("osToolsGetIP ERROR: invalid ip address\n");
            return -1;
        }
        int32_t segmentValue = atoi(check);
        if (segmentValue > 255 || segmentValue < 0) {
            printf("osToolsGetIP ERROR: invalid ip address\n");
            return -1;
        }
        buffer[segments] = segmentValue;
        check = strtok(NULL, ".");
        segments++;
    }
    return segments;
}

int32_t osToolsGetSocketAddress(char *socketName, char *address, int32_t length) {
    if (!socketName) {
        printf("osToolsGetSocketAddress ERROR: socketName is NULL\n");
        return -1;
    }
    if (!address) {
        printf("osToolsGetSocketAddress ERROR: address is NULL\n");
        return -1;
    }
    int32_t socketIndex = list_find(osToolsSocket.socket, (unitype) socketName, 's');
    if (socketIndex == -1) {
        printf("osToolsGetSocketAddress ERROR: Could not find socket %s\n", socketName);
        return -1;
    }
    /* only IPv4 */
    char addressA[32];
    sprintf(addressA, "%hhu.%hhu.%hhu.%hhu", osToolsSocket.socket -> data[socketIndex + OSI_ADDRESS].i, osToolsSocket.socket -> data[socketIndex + OSI_ADDRESS + 1].i, osToolsSocket.socket -> data[socketIndex + OSI_ADDRESS + 2].i, osToolsSocket.socket -> data[socketIndex + OSI_ADDRESS + 3].i);
    strncpy(address, addressA, length);
    address[length - 1] = '\0';
    return 0;
}

int32_t osToolsGetPort(char *socketName, char *port, int32_t length) {
    if (!socketName) {
        printf("osToolsGetSocketAddress ERROR: socketName is NULL\n");
        return -1;
    }
    if (!port) {
        printf("osToolsGetSocketAddress ERROR: port is NULL\n");
        return -1;
    }
    int32_t socketIndex = list_find(osToolsSocket.socket, (unitype) socketName, 's');
    if (socketIndex == -1) {
        printf("osToolsGetPort ERROR: Could not find socket %s\n", socketName);
        return -1;
    }
    char portA[8];
    sprintf(portA, "%d", osToolsSocket.socket -> data[socketIndex + OSI_PORT].i);
    strncpy(port, portA, length);
    port[length - 1] = '\0';
    return 0;
}

int32_t osToolsServerSocketCreate(char *serverName, osToolsSocketProtocol_t protocol, char *serverPort) {
    if (!serverName) {
        printf("osToolsServerSocketCreate ERROR: serverName is NULL\n");
        return -1;
    }
    if (!serverPort) {
        printf("osToolsServerSocketCreate ERROR: port is NULL\n");
        return -1;
    }
    int32_t status;
    if (osToolsSocket.win32wsaActive == 0) {
        /* Initialize Winsock */
        WSADATA wsaData;
        status = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (status != 0) {
            printf("osToolsServerSocketCreate ERROR: Could not initialise Winsock\n");
            return -1;
        }
        osToolsSocket.win32wsaActive = 1;
    }
    struct addrinfo *result;
    struct addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM;
    if (protocol == OSTOOLS_PROTOCOL_TCP) {
        hints.ai_protocol = IPPROTO_TCP;
    } else if (protocol == OSTOOLS_PROTOCOL_UDP) {
        hints.ai_protocol = IPPROTO_UDP;
    }
    hints.ai_flags = AI_PASSIVE;

    /* get local address */
    char hostName[128];
    status = gethostname(hostName, sizeof(hostName));
    if (status == SOCKET_ERROR) {
        printf("osToolsServerSocketCreate ERROR: Could not gethostname\n");
        return -1;
    }
    printf("Host Name: %s\n", hostName);

    struct hostent *host = gethostbyname(hostName);
    if (host == NULL) {
        printf("osToolsServerSocketCreate ERROR: Could not gethostbyname\n");
        return -1;
    }

    int32_t iter = 0;
    struct in_addr address;
    while (host -> h_addr_list[iter] != NULL) {
        memcpy(&address, host -> h_addr_list[iter], sizeof(address));
        printf("- Address: %s\n", inet_ntoa(address));
        iter++;
    }
    char *serverAddress = inet_ntoa(address);
    if (serverAddress == NULL) {
        printf("osToolsServerSocketCreate ERROR: No addresses to create server on\n");
        return -1;
    }
    uint8_t ipAddress[4] = {0};
    if (osToolsGetIP(serverAddress, ipAddress, 4) != 4) {
        printf("osToolsServerSocketCreate ERROR: Invalid ip address\n");
    }

    /* Resolve the server address and port */
    status = getaddrinfo(serverAddress, serverPort, &hints, &result);
    if (status != 0) {
        printf("osToolsServerSocketCreate ERROR: Could not getaddrinfo\n");
        return -1;
    }
    struct addrinfo *resultElement = result;
    while (resultElement) {
        printf("Hosting a server on address %s:%s\n", serverAddress, serverPort);
        resultElement = resultElement -> ai_next;
    }
    if (result == NULL) {
        printf("osToolsServerSocketCreate ERROR: getaddrinfo returned NULL\n");
        return -1;
    }

    /* Create a SOCKET for the server to listen for client connections - Use the first element of linked list returned by getaddrinfo */
    SOCKET winsocket = socket(result -> ai_family, result -> ai_socktype, result -> ai_protocol);
    if (winsocket == INVALID_SOCKET) {
        freeaddrinfo(result);
        printf("osToolsServerSocketCreate ERROR: Could not create socket\n");
        return -1;
    }

    /* Bind the socket to the address */
    status = bind(winsocket, result -> ai_addr, result -> ai_addrlen);
    if (status == SOCKET_ERROR) {
        freeaddrinfo(result);
        printf("osToolsServerSocketCreate ERROR: Could not bind socket %s to address %s\n", serverName, serverAddress);
        return -1;
    }
    freeaddrinfo(result);

    list_append(osToolsSocket.socket, (unitype) serverName, 's');
    list_append(osToolsSocket.socket, (unitype) (void *) winsocket, 'l'); // socket
    list_append(osToolsSocket.socket, (unitype) 0, 'i'); // server specifier
    list_append(osToolsSocket.socket, (unitype) 0, 'i'); // reserved
    list_append(osToolsSocket.socket, (unitype) protocol, 'i'); // protocol
    list_append(osToolsSocket.socket, (unitype) ipAddress[0], 'i'); // address[0]
    list_append(osToolsSocket.socket, (unitype) ipAddress[1], 'i'); // address[1]
    list_append(osToolsSocket.socket, (unitype) ipAddress[2], 'i'); // address[2]
    list_append(osToolsSocket.socket, (unitype) ipAddress[3], 'i'); // address[3]
    for (int32_t i = 4; i < 16; i++) {
        list_append(osToolsSocket.socket, (unitype) 0, 'i'); // address padding
    }
    int32_t port;
    sscanf(serverPort, "%d", &port);
    list_append(osToolsSocket.socket, (unitype) port, 'i'); // port
    for (int32_t i = 0; i < 10; i++) {
        list_append(osToolsSocket.socket, (unitype) 0, 'i'); // reserved padding
    }
    return 0;
}

int32_t osToolsServerSocketListen(char *serverName, char *clientName) {
    if (!serverName) {
        printf("osToolsServerSocketListen ERROR: serverName is NULL\n");
        return -1;
    }
    if (!clientName) {
        printf("osToolsServerSocketListen ERROR: clientName is NULL\n");
        return -1;
    }
    int32_t socketIndex = list_find(osToolsSocket.socket, (unitype) serverName, 's');
    if (socketIndex == -1) {
        printf("osToolsServerSocketListen ERROR: Could not find socket %s\n", serverName);
        return -1;
    }
    int32_t status;
    SOCKET winsocket = (SOCKET) osToolsSocket.socket -> data[socketIndex + OSI_SOCKET].p;

    status = listen(winsocket, SOMAXCONN);
    if (status == SOCKET_ERROR) {
        printf("osToolsServerSocketListen ERROR: Listen failed\n");
        return -1;
    }
    struct sockaddr_in address;
    uint32_t addressLen = sizeof(address);
    SOCKET connection = accept(winsocket, (struct sockaddr *) &address, &addressLen);
    if (connection == INVALID_SOCKET) {
        printf("osToolsServerSocketListen ERROR: Accept failed\n");
        return -1;
    }
    uint8_t ipAddress[4] = {0};
    if (osToolsGetIP(inet_ntoa(address.sin_addr), ipAddress, 4) != 4) {
        printf("osToolsServerSocketListen ERROR: Invalid ip address\n");
    }
    printf("Incoming connection from %s:%d\n", inet_ntoa(address.sin_addr), address.sin_port);
    list_append(osToolsSocket.socket, (unitype) clientName, 's');
    list_append(osToolsSocket.socket, (unitype) (void *) connection, 'l'); // socket
    list_append(osToolsSocket.socket, (unitype) 1, 'i'); // client specifier
    list_append(osToolsSocket.socket, (unitype) 0, 'i'); // reserved
    list_append(osToolsSocket.socket, osToolsSocket.socket -> data[socketIndex + OSI_PROTOCOL], 'i'); // protocol
    list_append(osToolsSocket.socket, (unitype) ipAddress[0], 'i'); // address[0]
    list_append(osToolsSocket.socket, (unitype) ipAddress[1], 'i'); // address[1]
    list_append(osToolsSocket.socket, (unitype) ipAddress[2], 'i'); // address[2]
    list_append(osToolsSocket.socket, (unitype) ipAddress[3], 'i'); // address[3]
    for (int32_t i = 4; i < 16; i++) {
        list_append(osToolsSocket.socket, (unitype) 0, 'i'); // address padding
    }
    list_append(osToolsSocket.socket, (unitype) address.sin_port, 'i'); // port
    for (int32_t i = 0; i < 10; i++) {
        list_append(osToolsSocket.socket, (unitype) 0, 'i'); // reserved padding
    }
    return 0;
}

int32_t osToolsClientSocketCreate(char *clientName, osToolsSocketProtocol_t protocol, char *serverAddress, char *serverPort, int32_t timeoutMilliseconds) {
    if (!clientName) {
        printf("osToolsClientSocketCreate ERROR: clientName is NULL\n");
        return -1;
    }
    if (!serverAddress) {
        printf("osToolsClientSocketCreate ERROR: serverAddress is NULL\n");
        return -1;
    }
    if (!serverPort) {
        printf("osToolsClientSocketCreate ERROR: port is NULL\n");
        return -1;
    }
    uint8_t ipAddress[4] = {0};
    if (osToolsGetIP(serverAddress, ipAddress, 4) != 4) {
        printf("osToolsClientSocketCreate ERROR: Invalid ip address\n");
        return -1;
    }
    int32_t status;
    if (osToolsSocket.win32wsaActive == 0) {
        /* Initialize Winsock */
        WSADATA wsaData;
        status = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (status != 0) {
            printf("osToolsClientSocketCreate ERROR: Could not initialise Winsock\n");
            return -1;
        }
        osToolsSocket.win32wsaActive = 1;
    }
    /* Create client socket */
    struct addrinfo *result;
    struct addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM;
    if (protocol == OSTOOLS_PROTOCOL_TCP) {
        hints.ai_protocol = IPPROTO_TCP;
    } else if (protocol == OSTOOLS_PROTOCOL_UDP) {
        hints.ai_protocol = IPPROTO_UDP;
    }

    status = getaddrinfo(serverAddress, serverPort, &hints, &result);
    if (status != 0) {
        printf("osToolsClientSocketCreate ERROR: Could not getaddrinfo of %s:%s, failed with %d\n", serverAddress, serverPort, status);
        return -1;
    }

    struct addrinfo *resultElement = result;
    while (resultElement) {
        // printf("Connecting to %s:%s\n", serverAddress, serverPort);
        resultElement = resultElement -> ai_next;
    }
    if (result == NULL) {
        printf("osToolsClientSocketCreate ERROR: getaddrinfo returned NULL\n");
        return -1;
    }
    /* Use the first element of linked list returned by getaddrinfo */
    SOCKET winsocket = socket(result -> ai_family, result -> ai_socktype, result -> ai_protocol);
    if (winsocket == INVALID_SOCKET) {
        freeaddrinfo(result);
        printf("osToolsClientSocketCreate ERROR: Could not create socket\n");
        return -1;
    }
    /* set socket to non blocking mode while connecting */
    u_long nonblocking = 1;
    status = ioctlsocket(winsocket, FIONBIO, &nonblocking);
    if (status != NO_ERROR) {
        printf("osToolsClientSocketCreate ERROR: Could not set socket to non-blocking mode\n");
        return -1;
    }
    status = connect(winsocket, result -> ai_addr, (int32_t) result -> ai_addrlen);
    if (status == SOCKET_ERROR) {
        if (WSAGetLastError() != WSAEWOULDBLOCK) {
            printf("osToolsClientSocketCreate ERROR: Could not connect socket %d\n", WSAGetLastError());
            return -1;
        }
        struct timeval timeout;
        timeout.tv_sec = timeoutMilliseconds / 1000;
        timeoutMilliseconds -= (timeoutMilliseconds / 1000) * 1000;
        timeout.tv_usec = timeoutMilliseconds * 1000;
        fd_set winsockArray;
        winsockArray.fd_count = 1;
        winsockArray.fd_array[0] = winsocket;
        status = select(0, NULL, &winsockArray, NULL, &timeout);
        if (status != 1) {
            printf("osToolsClientSocketCreate ERROR: Could not connect socket (timeout)\n");
            return -1;
        }
    }
    /* set socket back to blocking mode */
    u_long blocking = 0;
    status = ioctlsocket(winsocket, FIONBIO, &blocking);
    if (status != NO_ERROR) {
        printf("osToolsClientSocketCreate ERROR: Could not set socket to blocking mode\n");
        return -1;
    }
    printf("Connected to %s:%s\n", serverAddress, serverPort);
    list_append(osToolsSocket.socket, (unitype) clientName, 's');
    list_append(osToolsSocket.socket, (unitype) (void *) winsocket, 'l'); // socket
    list_append(osToolsSocket.socket, (unitype) 1, 'i'); // client specifier
    list_append(osToolsSocket.socket, (unitype) 0, 'i'); // reserved
    list_append(osToolsSocket.socket, (unitype) protocol, 'i'); // protocol
    list_append(osToolsSocket.socket, (unitype) ipAddress[0], 'i'); // address[0]
    list_append(osToolsSocket.socket, (unitype) ipAddress[1], 'i'); // address[1]
    list_append(osToolsSocket.socket, (unitype) ipAddress[2], 'i'); // address[2]
    list_append(osToolsSocket.socket, (unitype) ipAddress[3], 'i'); // address[3]
    for (int32_t i = 4; i < 16; i++) {
        list_append(osToolsSocket.socket, (unitype) 0, 'i'); // address padding
    }
    int32_t port;
    sscanf(serverPort, "%d", &port);
    list_append(osToolsSocket.socket, (unitype) port, 'i'); // port
    for (int32_t i = 0; i < 10; i++) {
        list_append(osToolsSocket.socket, (unitype) 0, 'i'); // reserved padding
    }
    return 0;
}

int32_t osToolsSocketSend(char *socketName, uint8_t *data, int32_t length) {
    if (!socketName) {
        printf("osToolsSocketSend ERROR: socketName is NULL\n");
        return -1;
    }
    int32_t socketIndex = list_find(osToolsSocket.socket, (unitype) socketName, 's');
    if (socketIndex == -1) {
        printf("osToolsSocketSend ERROR: Could not find socket %s\n", socketName);
        return -1;
    }
    int32_t status = send((SOCKET) osToolsSocket.socket -> data[socketIndex + OSI_SOCKET].p, (char *) data, length, 0);
    if (status == SOCKET_ERROR) {
        printf("osToolsSocketSend ERROR: Failed to send\n");
        return -1;
    }
    return status;
}

int32_t osToolsSocketReceive(char *socketName, uint8_t *data, int32_t length, int32_t timeoutMilliseconds) {
    if (!socketName) {
        printf("osToolsSocketReceive ERROR: socketName is NULL\n");
        return -1;
    }
    int32_t socketIndex = list_find(osToolsSocket.socket, (unitype) socketName, 's');
    if (socketIndex == -1) {
        printf("osToolsSocketReceive ERROR: Could not find socket %s\n", socketName);
        return -1;
    }
    setsockopt((SOCKET) osToolsSocket.socket -> data[socketIndex + OSI_SOCKET].p, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeoutMilliseconds, sizeof(timeoutMilliseconds)); // https://stackoverflow.com/questions/2876024/linux-is-there-a-read-or-recv-from-socket-with-timeout
    int32_t status = recv((SOCKET) osToolsSocket.socket -> data[socketIndex + OSI_SOCKET].p, (char *) data, length, 0);
    if (status == SOCKET_ERROR) {
        if (WSAGetLastError() == WSAETIMEDOUT) {
            /* timeout */
            printf("osToolsSocketReceive ERROR: Failed to receive (timeout)\n");
            return 0;
        }
        printf("osToolsSocketReceive ERROR: Failed to receive\n");
        return -1;
    }
    return status;
}

int32_t osToolsSocketDestroy(char *socketName) {
    if (!socketName) {
        printf("osToolsSocketDestroy ERROR: socketName is NULL\n");
        return -1;
    }
    int32_t socketIndex = list_find(osToolsSocket.socket, (unitype) socketName, 's');
    if (socketIndex == -1) {
        printf("osToolsSocketDestroy ERROR: Could not find socket %s\n", socketName);
        return -1;
    }
    /* shutdown and close socket */
    int32_t status = shutdown((SOCKET) osToolsSocket.socket -> data[socketIndex + OSI_SOCKET].p, SD_SEND);
    if (status == SOCKET_ERROR) {
        printf("osToolsSocketDestroy WARN: Shutdown not successful\n");
    }
    closesocket((SOCKET) osToolsSocket.socket -> data[socketIndex + OSI_SOCKET].p);
    list_delete_range(osToolsSocket.socket, socketIndex, socketIndex + OSI_NUMBER_OF_FIELDS);
    return 0;
}

/* symbol IID_IMFMediaSource and MF_MT_FRAME_SIZE not linked despite their existence in mfapi.h and mfidl.h. Found in mfuuid.lib */
// const GUID IID_IMFMediaSource = {0x279a808d, 0xaec7, 0x40c8, {0x9c,0x6b, 0xa6, 0xb4, 0x92, 0xc7, 0x8a, 0x66}};
// const GUID MF_MT_FRAME_SIZE = {0x1652c33d, 0xd6b2, 0x4012, {0xb8, 0x34, 0x72, 0x03, 0x08, 0x49, 0xa3, 0x7d}};
// const GUID MF_MT_FRAME_RATE = {0xc459a2e8, 0x3d2c, 0x4e44, {0xb1, 0x32, 0xfe, 0xe5, 0x15, 0x6c, 0x7b, 0xb0}};
// const GUID MF_MT_MAJOR_TYPE = {0x48eba18e, 0xf8c9, 0x4687, {0xbf, 0x11, 0x0a, 0x74, 0xc9, 0xf9, 0x6a, 0x8f}};
// const GUID MF_MT_SUBTYPE = {0xf7e34c9a, 0x42e8, 0x4714, {0xb7, 0x4b, 0xcb, 0x29, 0xd7, 0x2c, 0x35, 0xe5}};
// const GUID MFMediaType_Video = {0x73646976, 0x0000, 0x0010, {0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71}};
// const GUID MFVideoFormat_MJPG = {0x47504a4d, 0x0000, 0x0010, {0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71}}; // MJPG
// const GUID MFVideoFormat_NV12 = {0x3231564e, 0x0000, 0x0010, {0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71}}; // NV12
// const GUID MFVideoFormat_NV21 = {0x3132564e, 0x0000, 0x0010, {0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71}}; // NV21
// const GUID MFVideoFormat_YV12 = {0x32315659, 0x0000, 0x0010, {0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71}}; // YV12
// const GUID MFVideoFormat_H264 = {0x34363248, 0x0000, 0x0010, {0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71}}; // H264
// const GUID MFVideoFormat_RGB8 = {41 /* D3DFMT_P8 */, 0x0000, 0x0010, {0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71}}; // See D3D definitions
// const GUID MFVideoFormat_RGB24 = {20 /* D3DFMT_R8G8B8 */, 0x0000, 0x0010, {0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71}}; // See D3D definitions
// const GUID MFImageFormat_RGB32 = {0x00000016, 0x0000, 0x0010, {0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71}};
// const GUID MFVideoFormat_RGB32 = {0x00000016, 0x0000, 0x0010, {0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71}};
const GUID CLSID_CMSH264DecoderMFT = {0x62CE7E72, 0x4C71, 0x4d20, {0xB1, 0x5D, 0x45, 0x28, 0x31, 0xA8, 0x7D, 0x9D}};
const GUID CLSID_CWMADecMediaObject = {0x2eeb4adf, 0x4578, 0x4d10, {0xbc, 0xa7, 0xbb, 0x95, 0x5f, 0x56, 0x32, 0x0a}};
const GUID CLSID_CWMAEncMediaObject = {0x70f598e9, 0xf4ab, 0x495a, {0x99, 0xe2, 0xa7, 0xc4, 0xd3, 0xd8, 0x9a, 0xbf}};

list_t *osToolsCameraList() {
    list_clear(osToolsCamera.camera);
    list_t *output = list_init();
    HRESULT hr = CoInitializeEx(NULL, 0);
    if (FAILED(hr)) {
        printf("osToolsCameraList CoInitializeEx Error: 0x%lX\n", hr);
        return output;
    }
    hr = MFStartup(MF_VERSION, MFSTARTUP_NOSOCKET);
    if (FAILED(hr)) {
        printf("osToolsCameraList MFStartup Error: 0x%lX\n", hr);
        return output;
    }
    /* https://learn.microsoft.com/en-us/windows/win32/medfound/enumerating-video-capture-devices
       https://gist.github.com/mmozeiko/a5adab1ad11ea6d0643ceb67bb8e3e19
    */

    IMFMediaSource *pSource = NULL;
    IMFAttributes *pAttributes = NULL;
    IMFActivate **ppDevices = NULL;

    /* Create an attribute store to specify the enumeration parameters. */
    hr = MFCreateAttributes(&pAttributes, 1);
    if (FAILED(hr)) {
        printf("osToolsCameraList MFCreateAttributes Error: 0x%lX\n", hr);
        goto osToolsCameraList_done;
    }

    /* Source type: video capture devices */
    hr = pAttributes -> lpVtbl -> SetGUID(pAttributes, &MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE, &MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID);
    if (FAILED(hr)) {
        printf("osToolsCameraList SetGUID Error: 0x%lX\n", hr);
        goto osToolsCameraList_done;
    }

    /* Enumerate devices. */
    UINT32 count;
    hr = MFEnumDeviceSources(pAttributes, &ppDevices, &count);
    if (FAILED(hr)) {
        printf("osToolsCameraList MFEnumDeviceSources Error: 0x%lX\n", hr);
        goto osToolsCameraList_done;
    }

    if (count == 0) {
        hr = E_FAIL;
        printf("osToolsCameraList: Error no cameras found\n");
        goto osToolsCameraList_done;
    }
    printf("osToolsCameraList: Found %d cameras\n", count);

    /* Create the media source object. */
    for (int32_t i = 0; i < count; i++) {
        hr = ppDevices[i] -> lpVtbl -> ActivateObject(ppDevices[i], &IID_IMFMediaSource, (void **) &pSource);
        if (FAILED(hr)) {
            printf("osToolsCameraList ActivateObject Error: 0x%lX\n", hr);
            goto osToolsCameraList_done;
        }
        DWORD characteristics;
        pSource -> lpVtbl -> GetCharacteristics(pSource, &characteristics);
        printf("- Source characteristics: %lX\n", characteristics);
        /* check stream type */
        IMFPresentationDescriptor *presentationDescriptor;
        pSource -> lpVtbl -> CreatePresentationDescriptor(pSource, &presentationDescriptor);
        DWORD streamCount;
        presentationDescriptor -> lpVtbl -> GetStreamDescriptorCount(presentationDescriptor, &streamCount);
        printf("- Source streams: %ld\n", streamCount);
        uint32_t setWidth = 0;
        uint32_t setHeight = 0;
        double setFramerate = 0.0;
        int32_t maxFramerate = -1;
        double minDifference = 100000000000.0;
        /* goal: Closest resolution to 1024x1024, Fastest framerate */
        if (streamCount > 0) {
            GUID savedSubtype;
            int32_t selected;
            IMFStreamDescriptor *streamDescriptor;
            hr = presentationDescriptor -> lpVtbl -> GetStreamDescriptorByIndex(presentationDescriptor, 0, &selected, &streamDescriptor);
            if (FAILED(hr)) {
                printf("osToolsCameraList GetStreamDescriptorByIndex Error: 0x%lX\n", hr);
                goto osToolsCameraList_done;
            }
            IMFMediaTypeHandler *mediaTypeHandler;
            hr = streamDescriptor -> lpVtbl -> GetMediaTypeHandler(streamDescriptor, &mediaTypeHandler);
            if (FAILED(hr)) {
                printf("osToolsCameraList GetMediaTypeHandler Error: 0x%lX\n", hr);
                goto osToolsCameraList_done;
            }
            DWORD mediaTypeCount;
            mediaTypeHandler -> lpVtbl -> GetMediaTypeCount(mediaTypeHandler, &mediaTypeCount);
            printf("- Media Type Count: %ld\n", mediaTypeCount);
            IMFTransform *h264decoder = NULL;
            IMFTransform *nv12decoder = NULL;
            for (int32_t j = 0; j < mediaTypeCount; j++) {
                // printf("- Media Type %d\n", j);
                IMFMediaType *mediaType;
                hr = mediaTypeHandler -> lpVtbl -> GetMediaTypeByIndex(mediaTypeHandler, j, &mediaType);
                if (FAILED(hr)) {
                    printf("osToolsCameraList GetMediaTypeByIndex Error: 0x%lX\n", hr);
                    goto osToolsCameraList_done;
                }
                hr = mediaType -> lpVtbl -> SetUINT32(mediaType, &MF_SOURCE_READER_ENABLE_VIDEO_PROCESSING, TRUE);
                if (FAILED(hr)) {
                    printf("osToolsCameraList SetUINT32 Error: 0x%lX\n", hr);
                    goto osToolsCameraList_done;
                }
                GUID majorType;
                hr = mediaType -> lpVtbl -> GetMajorType(mediaType, &majorType);
                if (FAILED(hr)) {
                    printf("osToolsCameraList GetMajorType Error: 0x%lX\n", hr);
                    goto osToolsCameraList_done;
                }
                /* see mfapi.h for DEFINE_GUID (MFMediaType_Video, 0x73646976, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71); */
                if (majorType.Data1 == 0x73646976 && majorType.Data2 == 0x0000 && majorType.Data3 == 0x0010 && majorType.Data4[0] == 0x80 && majorType.Data4[1] == 0x00 && majorType.Data4[2] == 0x00 && majorType.Data4[3] == 0xaa && majorType.Data4[4] == 0x00 && majorType.Data4[5] == 0x38 && majorType.Data4[6] == 0x9b && majorType.Data4[7] == 0x71) {
                    /* MFMediaType_Video */
                    // printf("  - Major type: MFMediaType_Video\n");
                    GUID subtype;
                    hr = mediaType -> lpVtbl -> GetGUID(mediaType, &MF_MT_SUBTYPE, &subtype);
                    if (FAILED(hr)) {
                        printf("osToolsCameraList GetGUID Error: 0x%lX\n", hr);
                        goto osToolsCameraList_done;
                    }
                    // printf("  - Subtype: %08lx-%02hx%02hx-%02x%02x-%02x%02x%02x%02x%02x%02x\n", subtype.Data1, subtype.Data2, subtype.Data3, // 3231564E-0010-8000-00AA00389B71
                    // subtype.Data4[0], subtype.Data4[1], subtype.Data4[2], subtype.Data4[3], subtype.Data4[4], subtype.Data4[5], subtype.Data4[6], subtype.Data4[7]);
                    uint64_t sizePacked;
                    hr = mediaType -> lpVtbl -> GetUINT64(mediaType, &MF_MT_FRAME_SIZE, &sizePacked);
                    if (FAILED(hr)) {
                        printf("osToolsCameraList GetUINT64 Error: 0x%lX\n", hr);
                        goto osToolsCameraList_done;
                    }
                    uint32_t width = (uint32_t) (sizePacked >> 32);
                    uint32_t height = (uint32_t) sizePacked;
                    uint64_t frameRatePacked;
                    mediaType -> lpVtbl -> GetUINT64(mediaType, &MF_MT_FRAME_RATE, &frameRatePacked);
                    uint32_t frameRateNum = (uint32_t) (frameRatePacked >> 32);
                    uint32_t frameRateDen = (uint32_t) frameRatePacked;
                    // printf("  - Width: %d\n", width);
                    // printf("  - Height: %d\n", height);
                    // printf("  - Frames/s: %.02lf\n", (double) frameRateNum / frameRateDen);
                    double difference = (width - 1024) * (width - 1024) + (height - 1024) * (height - 1024);
                    if ((frameRateDen != 0 && (int32_t) (frameRateNum / frameRateDen) > maxFramerate) || (frameRateDen != 0 && (int32_t) (frameRateNum / frameRateDen) == maxFramerate && difference < minDifference)) {
                        minDifference = difference;
                        maxFramerate = frameRateNum / frameRateDen;
                        setWidth = width;
                        setHeight = height;
                        setFramerate = (double) frameRateNum / frameRateDen;
                        savedSubtype = subtype;
                        if (savedSubtype.Data1 == 0x34363248) {
                            /* H264 decoder required */
                            hr = CoCreateInstance(&CLSID_CMSH264DecoderMFT, NULL, CLSCTX_ALL, &IID_IMFTransform, (void **) &h264decoder);
                            if (FAILED(hr)) {
                                printf("osToolsCameraList ERROR: CoCreateInstance failed on H264 decoder 0x%lX\n", hr);
                                goto osToolsCameraList_done;
                            }
                            DWORD numInputStreams;
                            DWORD numOutputStreams;
                            h264decoder -> lpVtbl -> GetStreamCount(h264decoder, &numInputStreams, &numOutputStreams);
                            if (numInputStreams < 1 || numOutputStreams < 1) {
                                printf("osToolsCameraList ERROR: Not enough streams on H264 decoder\n");
                                goto osToolsCameraList_done;
                            }
                            IMFMediaType *inputNativeType;
                            MFCreateMediaType(&inputNativeType);
                            mediaType -> lpVtbl -> CopyAllItems(mediaType, (IMFAttributes *) inputNativeType);
                            inputNativeType -> lpVtbl -> SetUINT32(inputNativeType, &MF_MT_INTERLACE_MODE, MFVideoInterlace_MixedInterlaceOrProgressive);
                            IMFMediaType *NV12MediaType;
                            MFCreateMediaType(&NV12MediaType);
                            inputNativeType -> lpVtbl -> CopyAllItems(inputNativeType, (IMFAttributes *) NV12MediaType);
                            hr = NV12MediaType -> lpVtbl -> SetGUID(NV12MediaType, &MF_MT_SUBTYPE, &MFVideoFormat_NV12);
                            if (FAILED(hr)) {
                                printf("osToolsCameraList SetGUID Error: 0x%lX\n", hr);
                                goto osToolsCameraList_done;
                            }
                            hr = h264decoder -> lpVtbl -> SetInputType(h264decoder, 0, inputNativeType, 0);
                            if (FAILED(hr)) {
                                printf("osToolsCameraList H264 SetInputType Error: 0x%lX\n", hr);
                                goto osToolsCameraList_done;
                            }
                            hr = h264decoder -> lpVtbl -> SetOutputType(h264decoder, 0, NV12MediaType, 0);
                            if (FAILED(hr)) {
                                printf("osToolsCameraList H264 SetOutputType Error: 0x%lX\n", hr);
                                goto osToolsCameraList_done;
                            }
                            /* NV12 to RGB32 decoder - https://learn.microsoft.com/en-us/windows/win32/medfound/registering-and-enumerating-mfts#enumerating-mfts */
                            IMFActivate **ppActivate = NULL;
                            MFT_REGISTER_TYPE_INFO inputInfo = {MFMediaType_Video, MFVideoFormat_NV12};
                            MFT_REGISTER_TYPE_INFO outputInfo = {MFMediaType_Video, MFVideoFormat_RGB32};
                            uint32_t unFlags = MFT_ENUM_FLAG_SYNCMFT  | MFT_ENUM_FLAG_LOCALMFT | MFT_ENUM_FLAG_SORTANDFILTER;
                            uint32_t codecs = 0;
                            hr = MFTEnumEx(MFT_CATEGORY_VIDEO_PROCESSOR, unFlags, &inputInfo, &outputInfo, &ppActivate, &codecs); // it's a video processor not an encoder/decoder
                            if (FAILED(hr)) {
                                printf("osToolsCameraList ERROR: MFTEnumEx failed with 0x%lX\n", hr);
                                goto osToolsCameraList_done;
                            }
                            if (SUCCEEDED(hr) && codecs == 0) {
                                printf("osToolsCameraList ERROR: No codecs for NV12 to RGB32\n");
                                goto osToolsCameraList_done;
                            }
                            hr = ppActivate[0] -> lpVtbl -> ActivateObject(ppActivate[0], &IID_IMFTransform, (void **) &nv12decoder);
                            for (int32_t i = 0; i < codecs; i++) {
                                ppActivate[i] -> lpVtbl -> Release(ppActivate[i]);
                            }
                            CoTaskMemFree(ppActivate);
                            if (FAILED(hr)) {
                                printf("osToolsCameraList ActivateObject Error: 0x%lX\n", hr);
                                goto osToolsCameraList_done;
                            }
                            IMFMediaType *RGB32MediaType;
                            MFCreateMediaType(&RGB32MediaType);
                            NV12MediaType -> lpVtbl -> CopyAllItems(NV12MediaType, (IMFAttributes *) RGB32MediaType);
                            hr = RGB32MediaType -> lpVtbl -> SetGUID(RGB32MediaType, &MF_MT_SUBTYPE, &MFVideoFormat_RGB32);
                            hr = nv12decoder -> lpVtbl -> SetInputType(nv12decoder, 0, NV12MediaType, 0);
                            if (FAILED(hr)) {
                                printf("osToolsCameraList NV12 SetInputType Error: 0x%lX\n", hr);
                                goto osToolsCameraList_done;
                            }
                            hr = nv12decoder -> lpVtbl -> SetOutputType(nv12decoder, 0, RGB32MediaType, 0);
                            if (FAILED(hr)) {
                                printf("osToolsCameraList NV12 SetOutputType Error: 0x%lX\n", hr);
                                goto osToolsCameraList_done;
                            }
                        } else {
                            hr = mediaTypeHandler -> lpVtbl -> SetCurrentMediaType(mediaTypeHandler, mediaType);
                            if (FAILED(hr)) {
                                printf("osToolsCameraList SetCurrentMediaType Error: 0x%lX\n", hr);
                                goto osToolsCameraList_done;
                            }
                        }
                    }
                } else {
                    /* Not MFMediaType_Video */
                    // printf("  - Major type: %08lx-%02hx%02hx-%02x%02x-%02x%02x%02x%02x%02x%02x\n", majorType.Data1, majorType.Data2, majorType.Data3,
                    // majorType.Data4[0], majorType.Data4[1], majorType.Data4[2], majorType.Data4[3], majorType.Data4[4], majorType.Data4[5], majorType.Data4[6], majorType.Data4[7]);
                }
            }
            printf("- Saved Subtype: %08lx-%02hx%02hx-%02x%02x-%02x%02x%02x%02x%02x%02x\n", savedSubtype.Data1, savedSubtype.Data2, savedSubtype.Data3,
            savedSubtype.Data4[0], savedSubtype.Data4[1], savedSubtype.Data4[2], savedSubtype.Data4[3], savedSubtype.Data4[4], savedSubtype.Data4[5], savedSubtype.Data4[6], savedSubtype.Data4[7]);
            char cameraString[32];
            sprintf(cameraString, "USB Camera %d", i);
            list_append(output, (unitype) cameraString, 's');
            list_append(output, (unitype) setWidth, 'i');
            list_append(output, (unitype) setHeight, 'i');
            list_append(output, (unitype) setFramerate, 'd');
            list_append(osToolsCamera.camera, (unitype) cameraString, 's');
            list_append(osToolsCamera.camera, (unitype) setWidth, 'i');
            list_append(osToolsCamera.camera, (unitype) setHeight, 'i');
            list_append(osToolsCamera.camera, (unitype) setFramerate, 'd');
            list_append(osToolsCamera.camera, (unitype) (void *) pSource, 'l');
            list_append(osToolsCamera.camera, (unitype) NULL, 'l');
            list_append(osToolsCamera.camera, (unitype) (void *) h264decoder, 'l');
            list_append(osToolsCamera.camera, (unitype) NULL, 'l');
            list_append(osToolsCamera.camera, (unitype) (void *) nv12decoder, 'l');
            list_append(osToolsCamera.camera, (unitype) NULL, 'l');
        }
    }
osToolsCameraList_done:
    if (pAttributes) {
        pAttributes -> lpVtbl -> Release(pAttributes);
        pAttributes = NULL;
    }
    for (DWORD i = 0; i < count; i++) {
        if (ppDevices[i]) {
            ppDevices[i] -> lpVtbl -> Release(ppDevices[i]);
            ppDevices[i] = NULL;
        }
    }
    CoTaskMemFree(ppDevices);
    return output;
}

int32_t osToolsCameraOpen(char *name) {
    int32_t cameraIndex = list_find(osToolsCamera.camera, (unitype) name, 's');
    if (cameraIndex == -1) {
        return -1;
    }
    IMFMediaSource *pSource = (IMFMediaSource *) osToolsCamera.camera -> data[cameraIndex + 4].p;
    IMFAttributes *pAttributes;
    /* https://stackoverflow.com/questions/43507393/media-foundation-set-video-interlacing-and-decode */
    MFCreateAttributes(&pAttributes, 1);
    pAttributes -> lpVtbl -> SetUINT32(pAttributes, &MF_READWRITE_ENABLE_HARDWARE_TRANSFORMS, TRUE);
    pAttributes -> lpVtbl -> SetUINT32(pAttributes, &MF_SOURCE_READER_ENABLE_VIDEO_PROCESSING, TRUE);
    pAttributes -> lpVtbl -> SetUINT32(pAttributes, &MF_SOURCE_READER_DISCONNECT_MEDIASOURCE_ON_SHUTDOWN, TRUE);
    IMFSourceReader *pReader;
    HRESULT hr = MFCreateSourceReaderFromMediaSource(pSource, pAttributes, &pReader);
    if (FAILED(hr)) {
        printf("osToolsCameraOpen MFCreateSourceReaderFromMediaSource Error: 0x%lX\n", hr);
        return -1;
    }
    pReader -> lpVtbl -> SetStreamSelection(pReader, MF_SOURCE_READER_FIRST_VIDEO_STREAM, TRUE);
    IMFMediaType *readerNativeType;
    int32_t dwStreamIndex = 0;
    while (pReader -> lpVtbl -> GetNativeMediaType(pReader, MF_SOURCE_READER_FIRST_VIDEO_STREAM, dwStreamIndex, &readerNativeType) == S_OK) {
        GUID nativeSubtype;
        readerNativeType -> lpVtbl -> GetGUID(readerNativeType, &MF_MT_SUBTYPE, &nativeSubtype);
        // printf("- Native Subtype %d: %08lx-%02hx%02hx-%02x%02x-%02x%02x%02x%02x%02x%02x\n", dwStreamIndex, nativeSubtype.Data1, nativeSubtype.Data2, nativeSubtype.Data3,
        // nativeSubtype.Data4[0], nativeSubtype.Data4[1], nativeSubtype.Data4[2], nativeSubtype.Data4[3], nativeSubtype.Data4[4], nativeSubtype.Data4[5], nativeSubtype.Data4[6], nativeSubtype.Data4[7]);
        dwStreamIndex++;
    }
    /*
    https://learn.microsoft.com/en-us/windows/win32/medfound/media-type-attributes
    https://learn.microsoft.com/en-us/windows/win32/medfound/video-subtype-guids#yuv-formats-8-bit-and-palettized
    https://stackoverflow.com/questions/10244657/nv12-to-rgb24-conversion-code-in-c
    https://stackoverflow.com/questions/50751767/nv12-to-rgb32-with-microsoft-media-foundation
    https://learn.microsoft.com/en-us/windows/win32/medfound/processing-media-data-with-the-source-reader#setting-output-formats
    */
    IMFMediaType *readerType;
    MFCreateMediaType(&readerType);
    readerType -> lpVtbl -> SetUINT32(readerType, &MF_SOURCE_READER_ENABLE_VIDEO_PROCESSING, TRUE);
    hr = readerType -> lpVtbl -> SetGUID(readerType, &MF_MT_MAJOR_TYPE, &MFMediaType_Video);
    if (FAILED(hr)) {
        printf("osToolsCameraOpen SetGUID Error: 0x%lX\n", hr);
        return -1;
    }
    if (osToolsCamera.camera -> data[cameraIndex + 6].p) {
        /* H264 decoder exists */
        hr = readerType -> lpVtbl -> SetGUID(readerType, &MF_MT_SUBTYPE, &MFVideoFormat_H264);
        if (FAILED(hr)) {
            printf("osToolsCameraOpen SetGUID Error: 0x%lX\n", hr);
            return -1;
        }
        /* https://stackoverflow.com/questions/30825271/how-to-create-imfsample-for-windowsmediafoundation-h-264-encoder-mft */
        MFT_OUTPUT_DATA_BUFFER *pTransformBuffer = malloc(sizeof(MFT_OUTPUT_DATA_BUFFER));
        pTransformBuffer -> dwStreamID = 0;
        pTransformBuffer -> pSample = NULL;
        MFCreateSample(&(pTransformBuffer -> pSample));
        IMFMediaBuffer *pBuffer = NULL;
        MFCreateMemoryBuffer(osToolsCamera.camera -> data[cameraIndex + 1].i * osToolsCamera.camera -> data[cameraIndex + 2].i * 2, &pBuffer);
        pTransformBuffer -> pSample -> lpVtbl -> AddBuffer(pTransformBuffer -> pSample, pBuffer);
        pTransformBuffer -> dwStatus = 0;
        pTransformBuffer -> pEvents = NULL;
        osToolsCamera.camera -> data[cameraIndex + 7].p = (void *) pTransformBuffer;
        /* NV12 decoder buffer */
        pTransformBuffer = malloc(sizeof(MFT_OUTPUT_DATA_BUFFER));
        pTransformBuffer -> dwStreamID = 0;
        pTransformBuffer -> pSample = NULL;
        MFCreateSample(&(pTransformBuffer -> pSample));
        MFCreateMemoryBuffer(osToolsCamera.camera -> data[cameraIndex + 1].i * osToolsCamera.camera -> data[cameraIndex + 2].i * 4, &pBuffer);
        pTransformBuffer -> pSample -> lpVtbl -> AddBuffer(pTransformBuffer -> pSample, pBuffer);
        pTransformBuffer -> dwStatus = 0;
        pTransformBuffer -> pEvents = NULL;
        osToolsCamera.camera -> data[cameraIndex + 9].p = (void *) pTransformBuffer;
    } else {
        /* no decoder */
        hr = readerType -> lpVtbl -> SetGUID(readerType, &MF_MT_SUBTYPE, &MFVideoFormat_RGB32);
        if (FAILED(hr)) {
            printf("osToolsCameraOpen SetGUID Error: 0x%lX\n", hr);
            return -1;
        }
    }
    hr = pReader -> lpVtbl -> SetCurrentMediaType(pReader, MF_SOURCE_READER_FIRST_VIDEO_STREAM, NULL, readerType);
    if (FAILED(hr)) {
        printf("osToolsCameraOpen SetCurrentMediaType Error: 0x%lX\n", hr); // getting 0xC00D5212 -> MF_E_TOPO_CODEC_NOT_FOUND: Could not find a decoder for the native stream type
        return -1;
    }
    osToolsCamera.camera -> data[cameraIndex + 5].p = (void *) pReader;
    return 0;
}

int32_t osToolsCameraReceive(char *name, uint8_t *data) {
    int32_t cameraIndex = list_find(osToolsCamera.camera, (unitype) name, 's');
    if (cameraIndex == -1) {
        return 0;
    }
    IMFSourceReader *pReader = (IMFSourceReader *) osToolsCamera.camera -> data[cameraIndex + 5].p;
    if (pReader == NULL) {
        return 0;
    }
    /* https://gist.github.com/mmozeiko/a5adab1ad11ea6d0643ceb67bb8e3e19 */
    IMFSample *pSample;
    DWORD stream;
    DWORD flags;
    LONGLONG timestamp;
    HRESULT hr;

    while (1) {
        /* this is reading in syncronous blocking mode, MF supports also async calls */
        hr = pReader -> lpVtbl -> ReadSample(pReader, MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, &stream, &flags, &timestamp, &pSample);
        if (FAILED(hr)) {
            printf("osToolsCameraReceive ERROR: ReadSample failed with %lX\n", hr);
            return 0;
        }
        if (flags & MF_SOURCE_READERF_STREAMTICK) {
            continue;
        }
        break;
    }

    if (osToolsCamera.camera -> data[cameraIndex + 6].p) {
        /* Phase 1: H264 to NV12 */
        IMFTransform *h264decoder = (IMFTransform *) osToolsCamera.camera -> data[cameraIndex + 6].p;
        IMFTransform *nv12decoder = (IMFTransform *) osToolsCamera.camera -> data[cameraIndex + 8].p;
        hr = h264decoder -> lpVtbl -> ProcessInput(h264decoder, 0, pSample, 0);
        if (FAILED(hr)) {
            printf("osToolsCameraReceive ERROR: H264 ProcessInput failed with 0x%lX\n", hr);
            pSample -> lpVtbl -> Release(pSample);
            return 0;
        }
        // MFT_OUTPUT_STREAM_INFO outputInfo;
        // hr = h264decoder -> lpVtbl -> GetOutputStreamInfo(h264decoder, 0, &outputInfo);
        // printf("Output info flags: %lX\n", outputInfo.dwFlags);
        /*
        Output = 7.
        MFT_OUTPUT_STREAM_WHOLE_SAMPLES = 0x1,
        MFT_OUTPUT_STREAM_SINGLE_SAMPLE_PER_BUFFER = 0x2,
        MFT_OUTPUT_STREAM_FIXED_SAMPLE_SIZE = 0x4,
        */
        while (1) {
            MFT_OUTPUT_DATA_BUFFER *pTransformBuffer = (MFT_OUTPUT_DATA_BUFFER *) osToolsCamera.camera -> data[cameraIndex + 7].p;
            if (pTransformBuffer == NULL) {
                printf("osToolsCameraReceive ERROR: pTransformBuffer is NULL\n");
                return 0;
            }
            IMFMediaBuffer *pBuffer;
            hr = pTransformBuffer -> pSample -> lpVtbl -> GetBufferByIndex(pTransformBuffer -> pSample, 0, &pBuffer);
            pBuffer -> lpVtbl -> SetCurrentLength(pBuffer, 0); // rewind buffer so it can be used again
            DWORD transformFlags = 0;
            hr = h264decoder -> lpVtbl -> ProcessOutput(h264decoder, 0, 1, pTransformBuffer, &transformFlags);
            if (FAILED(hr)) {
                if (hr == 0xC00D6D61 || (transformFlags & 0x100)) {
                    /* stream change */
                    IMFMediaType *possibleType;
                    int32_t typeIndex = 0;
                    printf("Possible Types:\n");
                    while (!FAILED(h264decoder -> lpVtbl -> GetOutputAvailableType(h264decoder, 0, typeIndex, &possibleType))) {
                        GUID possibleSubtype;
                        hr = possibleType -> lpVtbl -> GetGUID(possibleType, &MF_MT_SUBTYPE, &possibleSubtype);
                        printf("- Subtype: %08lx-%02hx%02hx-%02x%02x-%02x%02x%02x%02x%02x%02x\n", possibleSubtype.Data1, possibleSubtype.Data2, possibleSubtype.Data3,
                        possibleSubtype.Data4[0], possibleSubtype.Data4[1], possibleSubtype.Data4[2], possibleSubtype.Data4[3], possibleSubtype.Data4[4], possibleSubtype.Data4[5], possibleSubtype.Data4[6], possibleSubtype.Data4[7]);
                        if (typeIndex == 0) {
                            /* just use the first one idfk */
                            hr = h264decoder -> lpVtbl -> SetOutputType(h264decoder, 0, possibleType, 0);
                            if (FAILED(hr)) {
                                printf("osToolsCameraReceive ERROR: SetOutputType failed with 0x%lX\n", hr);
                                return 0;
                            }
                        }
                        typeIndex++;
                    }
                    continue;
                } else {
                    printf("osToolsCameraReceive ERROR: H264 ProcessOutput failed with 0x%lX with flag 0x%lX\n", hr, transformFlags);
                    pSample -> lpVtbl -> Release(pSample); // assume sample must not be released between ProcessInput and ProcessOutput
                    return 0;
                }
            }
            pSample -> lpVtbl -> Release(pSample); // assume sample must not be released between ProcessInput and ProcessOutput
            pSample = pTransformBuffer -> pSample;
            /* Phase 2: NV12 to RGB32 */
            // MFT_OUTPUT_STREAM_INFO outputInfo;
            // hr = nv12decoder -> lpVtbl -> GetOutputStreamInfo(nv12decoder, 0, &outputInfo);
            // printf("Output info flags: %lX\n", outputInfo.dwFlags);
            /* literally 0 flags, i hate you */
            pTransformBuffer = (MFT_OUTPUT_DATA_BUFFER *) osToolsCamera.camera -> data[cameraIndex + 9].p;
            hr = pTransformBuffer -> pSample -> lpVtbl -> GetBufferByIndex(pTransformBuffer -> pSample, 0, &pBuffer);
            pBuffer -> lpVtbl -> SetCurrentLength(pBuffer, 0); // rewind buffer so it can be used again
            hr = nv12decoder -> lpVtbl -> ProcessInput(nv12decoder, 0, pSample, 0);
            if (FAILED(hr)) {
                printf("osToolsCameraReceive ERROR: NV12 ProcessInput failed with 0x%lX\n", hr);
                nv12decoder -> lpVtbl -> ProcessMessage(nv12decoder, MFT_MESSAGE_COMMAND_FLUSH, 0);
                return 0;
            }
            hr = nv12decoder -> lpVtbl -> ProcessOutput(nv12decoder, 0, 1, pTransformBuffer, &transformFlags);
            if (FAILED(hr)) {
                printf("osToolsCameraReceive ERROR: NV12 ProcessOutput failed with 0x%lX with flag 0x%lX\n", hr, transformFlags);
                return 0;
            }
            nv12decoder -> lpVtbl -> ProcessMessage(nv12decoder, MFT_MESSAGE_COMMAND_FLUSH, 0);
            pSample = pTransformBuffer -> pSample;
            break;
        }
    }

    IMFMediaBuffer *pBuffer;

    hr = pSample -> lpVtbl -> ConvertToContiguousBuffer(pSample, &pBuffer);
    if (FAILED(hr)) {
        return 0;
    }
    BYTE *rawBuffer;
    DWORD size;
    hr = pBuffer -> lpVtbl -> Lock(pBuffer, &rawBuffer, NULL, &size);
    if (FAILED(hr)) {
        return 0;
    }
    /* drop the alpha character */
    int32_t iterData = 0;
    for (int32_t i = 0; i < size; i += 4) {
        data[iterData++] = rawBuffer[i + 2];
        data[iterData++] = rawBuffer[i + 1];
        data[iterData++] = rawBuffer[i];
    }

    pBuffer -> lpVtbl -> Unlock(pBuffer);
    if (osToolsCamera.camera -> data[cameraIndex + 6].p == NULL) {
        pBuffer -> lpVtbl -> Release(pBuffer);
        pSample -> lpVtbl -> Release(pSample);
    }
    return iterData;
}

int32_t osToolsCameraClose(char *name) {
    int32_t cameraIndex = list_find(osToolsCamera.camera, (unitype) name, 's');
    if (cameraIndex == -1) {
        return -1;
    }
    IMFSourceReader *pReader = (IMFSourceReader *) osToolsCamera.camera -> data[cameraIndex + 5].p;
    if (pReader) {
        pReader -> lpVtbl -> Release(pReader);
        osToolsCamera.camera -> data[cameraIndex + 5].p = NULL;
    }
    IMFTransform *h264decoder = (IMFTransform *) osToolsCamera.camera -> data[cameraIndex + 6].p;
    if (h264decoder) {
        h264decoder -> lpVtbl -> ProcessMessage(h264decoder, MFT_MESSAGE_COMMAND_FLUSH, 0);
        // h264decoder -> lpVtbl -> Release(h264decoder);
        // osToolsCamera.camera -> data[cameraIndex + 6].p = NULL;
    }
    IMFTransform *nv12decoder = (IMFTransform *) osToolsCamera.camera -> data[cameraIndex + 6].p;
    if (nv12decoder) {
        nv12decoder -> lpVtbl -> ProcessMessage(nv12decoder, MFT_MESSAGE_COMMAND_FLUSH, 0);
    }
    return 0;
}

#endif
#ifdef OS_LINUX

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

int32_t osToolsInit(char argv0[], GLFWwindow *window) {
    osToolsIndependentInit(window);
    /* get executable filepath */
    FILE *exStringFile = popen("pwd", "r");
    if (fscanf(exStringFile, "%s", osToolsFileDialog.executableFilepath) == 0) {
        return -1;
    }
    strcat(osToolsFileDialog.executableFilepath, "/");
    strcat(osToolsFileDialog.executableFilepath, argv0);
    
    int32_t index = strlen(osToolsFileDialog.executableFilepath) - 1;
    while (index > -1 && osToolsFileDialog.executableFilepath[index] != '/') {
        index--;
    }
    osToolsFileDialog.executableFilepath[index + 1] = '\0';
    return 0;
}

int32_t osToolsFileDialogPrompt(ost_file_dialog_save_t openOrSave, ost_file_dialog_multiselect_t multiselect, ost_file_dialog_folder_t folder, char *prename, list_t *extensions) {
    if (extensions == NULL) {
        extensions = osToolsFileDialog.globalExtensions;
    }
    int32_t totalMemory = 23 + 14 + strlen(prename) + 15 + 7 + 20 + 11 + 17 + 12 + 36 + 21; // 23 for zenity --file-selection, 14 for --filename=', prename, 15 for --title='Open', 7 for --save, 20 for --confirm-overwrite, 11 for --multiple, 17 for --separator="\n", 12 for --directory, 36 for --file-filter='Specified Types | , added per extension, 20 for if I got this wrong, 1 for \0
    for (int32_t i = 0; i < extensions -> length; i++) {
        totalMemory += strlen(extensions -> data[i].s) + 3;
    }
    char fullCommand[totalMemory];
    strcpy(fullCommand, "zenity --file-selection");
    /* configure autofill filename */
    if (prename != NULL && strcmp(prename, "null") != 0 && strcmp(prename, "") != 0) {
        strcat(fullCommand, " --filename=\"");
        strcat(fullCommand, prename);
        strcat(fullCommand, "\"");
    }

    /* configure title */
    if (openOrSave == OSTOOLS_FILE_DIALOG_SAVE) {
        strcat(fullCommand, " --title=\"Save\"");
        strcat(fullCommand, " --save");
        strcat(fullCommand, " --confirm-overwrite");
    } else if (openOrSave == OSTOOLS_FILE_DIALOG_OPEN) {
        strcat(fullCommand, " --title=\"Open\"");
    }

    /* configure multiselect and folder */
    if (multiselect) {
        strcat(fullCommand, " --multiple");
        strcat(fullCommand,  " --separator=\"\n\"");
    }
    if (folder) {
        strcat(fullCommand, " --directory");
    }

    /* configure extensions */
    if (extensions -> length > 0) {
        char filterName[35] = " --file-filter=\"Specified Types | ";
        strcat(fullCommand, filterName);
    }
    for (int32_t i = 0; i < extensions -> length; i++) {
        char buildFilter[strlen(extensions -> data[i].s) + 5];
        buildFilter[0] = '*';
        buildFilter[1] = '.';
        buildFilter[2] = '\0';
        strcat(buildFilter, extensions -> data[i].s);
        if (i == extensions -> length - 1) { // dont add space if it's the last element
            strcat(buildFilter, "\"");
        } else {
            strcat(buildFilter, " ");
        }
        strcat(fullCommand, buildFilter);
    }
    printf("%s\n", fullCommand); // zenity --file-selection --title="Save" --save --confirm-overwrite --file-filter="Specified Types | *.txt"

    /* execute */
    FILE* filenameStream = popen(fullCommand, "r");
    char output[4097];
    int32_t files = 0;
    while (fgets(output, 4097, filenameStream) != NULL) {
        if (files == 0) {
            list_clear(osToolsFileDialog.selectedFilenames);
        }
        output[strlen(output) - 1] = '\0';
        list_append(osToolsFileDialog.selectedFilenames, (unitype) output, 's');
        files++;
    }
    pclose(filenameStream);
    if (files == 0) {
        return -1;
    }
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

list_t *osToolsListFilesAndFolders(char *directory) {
    list_t *output = list_init();
    DIR *dir = opendir(directory);
    if (dir == NULL) {
        return output;
    }
    struct dirent *dstream = readdir(dir);
    while (dstream != NULL) {
        if (strcmp(dstream -> d_name, ".") == 0 || strcmp(dstream -> d_name, "..") == 0) {
            dstream = readdir(dir);
            continue;
        }
        list_append(output, (unitype) dstream -> d_name, 's');
        dstream = readdir(dir);
    }
    closedir(dir);
    return output;
}

list_t *osToolsListFiles(char *directory) {
    list_t *output = list_init();
    DIR *dir = opendir(directory);
    if (dir == NULL) {
        return output;
    }
    struct dirent *dstream = readdir(dir);
    while (dstream != NULL) {
        char fullpath[5000] = "";
        strcat(fullpath, directory);
        strcat(fullpath, "/");
        strcat(fullpath, dstream -> d_name);
        struct stat checkType;
        stat(fullpath, &checkType);
        if (S_ISREG(checkType.st_mode)) {
            list_append(output, (unitype) dstream -> d_name, 's');
        }
        dstream = readdir(dir);
    }
    closedir(dir);
    return output;
}

list_t *osToolsListFolders(char *directory) {
    list_t *output = list_init();
    DIR *dir = opendir(directory);
    if (dir == NULL) {
        return output;
    }
    struct dirent *dstream = readdir(dir);
    while (dstream != NULL) {
        if (strcmp(dstream -> d_name, ".") == 0 || strcmp(dstream -> d_name, "..") == 0) {
            dstream = readdir(dir);
            continue;
        }
        char fullpath[5000] = "";
        strcat(fullpath, directory);
        strcat(fullpath, "/");
        strcat(fullpath, dstream -> d_name);
        struct stat checkType;
        stat(fullpath, &checkType);
        if (S_ISDIR(checkType.st_mode)) {
            list_append(output, (unitype) dstream -> d_name, 's');
        }
        dstream = readdir(dir);
    }
    closedir(dir);
    return output;
}

int32_t osToolsCreateFolder(char *folder) {
    return mkdir(folder, 0755);
}

int32_t osToolsDeleteFolder(char *folder) {
    char command[5000] = "rm -rf ";
    strcat(command, folder);
    return system(command);
}

void osToolsCloseConsole() {
    /* don't know how to do this yet - https://unix.stackexchange.com/questions/743272/programatically-start-a-background-process-under-linux */
    return;
}

list_t *osToolsComList() {
    list_t *output = list_init();
    return output;
}

int32_t osToolsComOpen(char *name, osToolsComBaud_t baudRate) {
    return -1;
}

int32_t osToolsComSend(char *name, uint8_t *data, int32_t length) {
    return -1;
}

int32_t osToolsComReceive(char *name, uint8_t *buffer, int32_t length, int32_t timeoutMilliseconds) {
    return -1;
}

int32_t osToolsComClose(char *name) {
    return -1;
}

int32_t osToolsGetIP(char *address, uint8_t *buffer, int32_t maxSegments) {
    char modifiable[strlen(address) + 1];
    strcpy(modifiable, address);
    char *check = strtok(modifiable, ".");
    int32_t segments = 0;
    while (check != NULL) {
        if (segments > maxSegments - 1) {
            printf("osToolsGetIP ERROR: invalid ip address\n");
            return -1;
        }
        int32_t segmentValue = atoi(check);
        if (segmentValue > 255 || segmentValue < 0) {
            printf("osToolsGetIP ERROR: invalid ip address\n");
            return -1;
        }
        buffer[segments] = segmentValue;
        check = strtok(NULL, ".");
        segments++;
    }
    return segments;
}

int32_t osToolsGetSocketAddress(char *socketName, char *address, int32_t length) {
    if (!socketName) {
        printf("osToolsGetSocketAddress ERROR: socketName is NULL\n");
        return -1;
    }
    if (!address) {
        printf("osToolsGetSocketAddress ERROR: address is NULL\n");
        return -1;
    }
    int32_t socketIndex = list_find(osToolsSocket.socket, (unitype) socketName, 's');
    if (socketIndex == -1) {
        printf("osToolsGetSocketAddress ERROR: Could not find socket %s\n", socketName);
        return -1;
    }
    /* only IPv4 */
    char addressA[32];
    sprintf(addressA, "%hhu.%hhu.%hhu.%hhu", osToolsSocket.socket -> data[socketIndex + OSI_ADDRESS].i, osToolsSocket.socket -> data[socketIndex + OSI_ADDRESS + 1].i, osToolsSocket.socket -> data[socketIndex + OSI_ADDRESS + 2].i, osToolsSocket.socket -> data[socketIndex + OSI_ADDRESS + 3].i);
    strncpy(address, addressA, length);
    address[length - 1] = '\0';
    return 0;
}

int32_t osToolsGetPort(char *socketName, char *port, int32_t length) {
    if (!socketName) {
        printf("osToolsGetSocketAddress ERROR: socketName is NULL\n");
        return -1;
    }
    if (!port) {
        printf("osToolsGetSocketAddress ERROR: port is NULL\n");
        return -1;
    }
    int32_t socketIndex = list_find(osToolsSocket.socket, (unitype) socketName, 's');
    if (socketIndex == -1) {
        printf("osToolsGetPort ERROR: Could not find socket %s\n", socketName);
        return -1;
    }
    char portA[8];
    sprintf(portA, "%d", osToolsSocket.socket -> data[socketIndex + OSI_PORT].i);
    strncpy(port, portA, length);
    port[length - 1] = '\0';
    return 0;
}

int32_t osToolsServerSocketCreate(char *serverName, osToolsSocketProtocol_t protocol, char *serverPort) {
    if (!serverName) {
        printf("osToolsServerSocketCreate ERROR: serverName is NULL\n");
        return -1;
    }
    if (!serverPort) {
        printf("osToolsServerSocketCreate ERROR: port is NULL\n");
        return -1;
    }
    int32_t status;
    struct addrinfo *result;
    struct addrinfo hints;
    for (int32_t i = 0; i < sizeof(hints); i++) {
        *((((uint8_t *) &hints) + i)) = 0; // bzero
    }
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM;
    if (protocol == OSTOOLS_PROTOCOL_TCP) {
        hints.ai_protocol = IPPROTO_TCP;
    } else if (protocol == OSTOOLS_PROTOCOL_UDP) {
        hints.ai_protocol = IPPROTO_UDP;
    }
    hints.ai_flags = AI_PASSIVE;

    /* get local address */
    char hostName[128];
    status = gethostname(hostName, sizeof(hostName));
    if (status == -1) {
        printf("osToolsServerSocketCreate ERROR: Could not gethostname\n");
        return -1;
    }
    printf("Host Name: %s\n", hostName);

    struct hostent *host = gethostbyname(hostName);
    if (host == NULL) {
        printf("osToolsServerSocketCreate ERROR: Could not gethostbyname\n");
        return -1;
    }

    int32_t iter = 0;
    struct in_addr address;
    while (host -> h_addr_list[iter] != NULL) {
        memcpy(&address, host -> h_addr_list[iter], sizeof(address));
        printf("- Address: %s\n", inet_ntoa(address));
        iter++;
    }
    char *serverAddress = inet_ntoa(address);
    if (serverAddress == NULL) {
        printf("osToolsServerSocketCreate ERROR: No addresses to create server on\n");
        return -1;
    }
    uint8_t ipAddress[4] = {0};
    if (osToolsGetIP(serverAddress, ipAddress, 4) != 4) {
        printf("osToolsServerSocketCreate ERROR: Invalid ip address\n");
    }

    /* Resolve the server address and port */
    status = getaddrinfo(serverAddress, serverPort, &hints, &result);
    if (status != 0) {
        printf("osToolsServerSocketCreate ERROR: Could not getaddrinfo\n");
        return -1;
    }
    struct addrinfo *resultElement = result;
    while (resultElement) {
        printf("Hosting a server on address %s:%s\n", serverAddress, serverPort);
        resultElement = resultElement -> ai_next;
    }
    if (result == NULL) {
        printf("osToolsServerSocketCreate ERROR: getaddrinfo returned NULL\n");
        return -1;
    }

    /* Create a SOCKET for the server to listen for client connections - Use the first element of linked list returned by getaddrinfo */
    int32_t sockfd = socket(result -> ai_family, result -> ai_socktype, result -> ai_protocol);
    if (sockfd == -1) {
        freeaddrinfo(result);
        printf("osToolsServerSocketCreate ERROR: Could not create socket\n");
        return -1;
    }

    /* Bind the socket to the address */
    status = bind(sockfd, result -> ai_addr, result -> ai_addrlen);
    if (status == -1) {
        freeaddrinfo(result);
        printf("osToolsServerSocketCreate ERROR: Could not bind socket %s to address %s\n", serverName, serverAddress);
        return -1;
    }
    freeaddrinfo(result);

    list_append(osToolsSocket.socket, (unitype) serverName, 's');
    list_append(osToolsSocket.socket, (unitype) sockfd, 'i'); // socket
    list_append(osToolsSocket.socket, (unitype) 0, 'i'); // server specifier
    list_append(osToolsSocket.socket, (unitype) 0, 'i'); // reserved
    list_append(osToolsSocket.socket, (unitype) protocol, 'i'); // protocol
    list_append(osToolsSocket.socket, (unitype) ipAddress[0], 'i'); // address[0]
    list_append(osToolsSocket.socket, (unitype) ipAddress[1], 'i'); // address[1]
    list_append(osToolsSocket.socket, (unitype) ipAddress[2], 'i'); // address[2]
    list_append(osToolsSocket.socket, (unitype) ipAddress[3], 'i'); // address[3]
    for (int32_t i = 4; i < 16; i++) {
        list_append(osToolsSocket.socket, (unitype) 0, 'i'); // address padding
    }
    int32_t port;
    sscanf(serverPort, "%d", &port);
    list_append(osToolsSocket.socket, (unitype) port, 'i'); // port
    for (int32_t i = 0; i < 10; i++) {
        list_append(osToolsSocket.socket, (unitype) 0, 'i'); // reserved padding
    }
    return 0;
}

int32_t osToolsServerSocketListen(char *serverName, char *clientName) {
    if (!serverName) {
        printf("osToolsServerSocketListen ERROR: serverName is NULL\n");
        return -1;
    }
    if (!clientName) {
        printf("osToolsServerSocketListen ERROR: clientName is NULL\n");
        return -1;
    }
    int32_t socketIndex = list_find(osToolsSocket.socket, (unitype) serverName, 's');
    if (socketIndex == -1) {
        printf("osToolsServerSocketListen ERROR: Could not find socket %s\n", serverName);
        return -1;
    }
    int32_t status;
    int32_t sockfd = osToolsSocket.socket -> data[socketIndex + OSI_SOCKET].i;

    status = listen(sockfd, SOMAXCONN);
    if (status == -1) {
        printf("osToolsServerSocketListen ERROR: Listen failed\n");
        return -1;
    }
    struct sockaddr_in address;
    uint32_t addressLen = sizeof(address);
    int32_t connectionfd = accept(sockfd, (struct sockaddr *) &address, &addressLen);
    if (connectionfd == -1) {
        printf("osToolsServerSocketListen ERROR: Accept failed\n");
        return -1;
    }
    uint8_t ipAddress[4] = {0};
    if (osToolsGetIP(inet_ntoa(address.sin_addr), ipAddress, 4) != 4) {
        printf("osToolsServerSocketListen ERROR: Invalid ip address\n");
    }
    printf("Incoming connection from %s:%d\n", inet_ntoa(address.sin_addr), address.sin_port);
    list_append(osToolsSocket.socket, (unitype) clientName, 's');
    list_append(osToolsSocket.socket, (unitype) connectionfd, 'i'); // socket
    list_append(osToolsSocket.socket, (unitype) 1, 'i'); // client specifier
    list_append(osToolsSocket.socket, (unitype) 0, 'i'); // reserved
    list_append(osToolsSocket.socket, osToolsSocket.socket -> data[socketIndex + OSI_PROTOCOL], 'i'); // protocol
    list_append(osToolsSocket.socket, (unitype) ipAddress[0], 'i'); // address[0]
    list_append(osToolsSocket.socket, (unitype) ipAddress[1], 'i'); // address[1]
    list_append(osToolsSocket.socket, (unitype) ipAddress[2], 'i'); // address[2]
    list_append(osToolsSocket.socket, (unitype) ipAddress[3], 'i'); // address[3]
    for (int32_t i = 4; i < 16; i++) {
        list_append(osToolsSocket.socket, (unitype) 0, 'i'); // address padding
    }
    list_append(osToolsSocket.socket, (unitype) address.sin_port, 'i'); // port
    for (int32_t i = 0; i < 10; i++) {
        list_append(osToolsSocket.socket, (unitype) 0, 'i'); // reserved padding
    }
    return 0;
}

int32_t osToolsClientSocketCreate(char *clientName, osToolsSocketProtocol_t protocol, char *serverAddress, char *serverPort, int32_t timeoutMilliseconds) {
    if (!clientName) {
        printf("osToolsClientSocketCreate ERROR: clientName is NULL\n");
        return -1;
    }
    if (!serverAddress) {
        printf("osToolsClientSocketCreate ERROR: serverAddress is NULL\n");
        return -1;
    }
    if (!serverPort) {
        printf("osToolsClientSocketCreate ERROR: port is NULL\n");
        return -1;
    }
    uint8_t ipAddress[4] = {0};
    if (osToolsGetIP(serverAddress, ipAddress, 4) != 4) {
        printf("osToolsClientSocketCreate ERROR: Invalid ip address\n");
        return -1;
    }
    int32_t status;
    /* Create client socket */
    struct addrinfo *result;
    struct addrinfo hints;
    for (int32_t i = 0; i < sizeof(hints); i++) {
        *((((uint8_t *) &hints) + i)) = 0; // bzero
    }
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM;
    if (protocol == OSTOOLS_PROTOCOL_TCP) {
        hints.ai_protocol = IPPROTO_TCP;
    } else if (protocol == OSTOOLS_PROTOCOL_UDP) {
        hints.ai_protocol = IPPROTO_UDP;
    }

    status = getaddrinfo(serverAddress, serverPort, &hints, &result);
    if (status != 0) {
        printf("osToolsClientSocketCreate ERROR: Could not getaddrinfo of %s:%s, failed with %d\n", serverAddress, serverPort, status);
        return -1;
    }

    struct addrinfo *resultElement = result;
    while (resultElement) {
        // printf("Connecting to %s:%s\n", serverAddress, serverPort);
        resultElement = resultElement -> ai_next;
    }
    if (result == NULL) {
        printf("osToolsClientSocketCreate ERROR: getaddrinfo returned NULL\n");
        return -1;
    }
    /* Use the first element of linked list returned by getaddrinfo */
    int32_t sockfd = socket(result -> ai_family, result -> ai_socktype, result -> ai_protocol);
    if (sockfd == -1) {
        freeaddrinfo(result);
        printf("osToolsClientSocketCreate ERROR: Could not create socket\n");
        return -1;
    }
    /* set socket to non blocking mode while connecting - https://stackoverflow.com/questions/1543466/how-do-i-change-a-tcp-socket-to-be-non-blocking */
    status = fcntl(sockfd, F_GETFL, 0);
    if (status == -1) {
        printf("osToolsClientSocketCreate ERROR: Could not get fcntl flags\n");
        return -1;
    }
    status &= O_NONBLOCK;
    status = fcntl(sockfd, F_SETFL, status);
    if (status != 0) {
        printf("osToolsClientSocketCreate ERROR: Could not set socket to non-blocking mode\n");
        return -1;
    }
    status = connect(sockfd, result -> ai_addr, (int32_t) result -> ai_addrlen);
    if (status == -1) {
        if (errno != EINPROGRESS) {
            printf("osToolsClientSocketCreate ERROR: Could not connect socket %d (%s)\n", errno, strerror(errno));
            return -1;
        }
        struct timeval timeout;
        timeout.tv_sec = timeoutMilliseconds / 1000;
        timeoutMilliseconds -= (timeoutMilliseconds / 1000) * 1000;
        timeout.tv_usec = timeoutMilliseconds * 1000;
        fd_set fdsockArray;
        fdsockArray.__fds_bits[0] = sockfd;
        status = select(sockfd + 1, NULL, &fdsockArray, NULL, &timeout);
        if (status != 1) {
            printf("osToolsClientSocketCreate ERROR: Could not connect socket (timeout)\n");
            return -1;
        }
    }
    /* set socket back to blocking mode */
    status = fcntl(sockfd, F_GETFL, 0);
    if (status == -1) {
        printf("osToolsClientSocketCreate ERROR: Could not get fcntl flags\n");
        return -1;
    }
    status &= ~O_NONBLOCK;
    status = fcntl(sockfd, F_SETFL, status);
    if (status != 0) {
        printf("osToolsClientSocketCreate ERROR: Could not set socket to blocking mode\n");
        return -1;
    }
    printf("Connected to %s:%s\n", serverAddress, serverPort);
    list_append(osToolsSocket.socket, (unitype) clientName, 's');
    list_append(osToolsSocket.socket, (unitype) sockfd, 'l'); // socket
    list_append(osToolsSocket.socket, (unitype) 1, 'i'); // client specifier
    list_append(osToolsSocket.socket, (unitype) 0, 'i'); // reserved
    list_append(osToolsSocket.socket, (unitype) protocol, 'i'); // protocol
    list_append(osToolsSocket.socket, (unitype) ipAddress[0], 'i'); // address[0]
    list_append(osToolsSocket.socket, (unitype) ipAddress[1], 'i'); // address[1]
    list_append(osToolsSocket.socket, (unitype) ipAddress[2], 'i'); // address[2]
    list_append(osToolsSocket.socket, (unitype) ipAddress[3], 'i'); // address[3]
    for (int32_t i = 4; i < 16; i++) {
        list_append(osToolsSocket.socket, (unitype) 0, 'i'); // address padding
    }
    int32_t port;
    sscanf(serverPort, "%d", &port);
    list_append(osToolsSocket.socket, (unitype) port, 'i'); // port
    for (int32_t i = 0; i < 10; i++) {
        list_append(osToolsSocket.socket, (unitype) 0, 'i'); // reserved padding
    }
    return 0;
}

int32_t osToolsSocketSend(char *socketName, uint8_t *data, int32_t length) {
    if (!socketName) {
        printf("osToolsSocketSend ERROR: socketName is NULL\n");
        return -1;
    }
    int32_t socketIndex = list_find(osToolsSocket.socket, (unitype) socketName, 's');
    if (socketIndex == -1) {
        printf("osToolsSocketSend ERROR: Could not find socket %s\n", socketName);
        return -1;
    }
    int32_t status = send(osToolsSocket.socket -> data[socketIndex + OSI_SOCKET].i, (char *) data, length, 0);
    if (status == -1) {
        printf("osToolsSocketSend ERROR: Failed to send\n");
        return -1;
    }
    return status;
}

int32_t osToolsSocketReceive(char *socketName, uint8_t *data, int32_t length, int32_t timeoutMilliseconds) {
    if (!socketName) {
        printf("osToolsSocketReceive ERROR: socketName is NULL\n");
        return -1;
    }
    int32_t socketIndex = list_find(osToolsSocket.socket, (unitype) socketName, 's');
    if (socketIndex == -1) {
        printf("osToolsSocketReceive ERROR: Could not find socket %s\n", socketName);
        return -1;
    }
    setsockopt(osToolsSocket.socket -> data[socketIndex + OSI_SOCKET].i, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeoutMilliseconds, sizeof(timeoutMilliseconds)); // https://stackoverflow.com/questions/2876024/linux-is-there-a-read-or-recv-from-socket-with-timeout
    int32_t status = recv(osToolsSocket.socket -> data[socketIndex + OSI_SOCKET].i, (char *) data, length, 0);
    if (status == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            /* timeout */
            printf("osToolsSocketReceive ERROR: Failed to receive (timeout)\n");
            return 0;
        }
        printf("osToolsSocketReceive ERROR: Failed to receive\n");
        return -1;
    }
    return status;
}

int32_t osToolsSocketDestroy(char *socketName) {
    if (!socketName) {
        printf("osToolsSocketDestroy ERROR: socketName is NULL\n");
        return -1;
    }
    int32_t socketIndex = list_find(osToolsSocket.socket, (unitype) socketName, 's');
    if (socketIndex == -1) {
        printf("osToolsSocketDestroy ERROR: Could not find socket %s\n", socketName);
        return -1;
    }
    /* shutdown and close socket */
    int32_t status = shutdown(osToolsSocket.socket -> data[socketIndex + OSI_SOCKET].i, SHUT_RDWR);
    if (status == -1) {
        printf("osToolsSocketDestroy WARN: Shutdown not successful\n");
    }
    close(osToolsSocket.socket -> data[socketIndex + OSI_SOCKET].i);
    list_delete_range(osToolsSocket.socket, socketIndex, socketIndex + OSI_NUMBER_OF_FIELDS);
    return 0;
}

list_t *osToolsCameraList() {
    list_t *output = list_init();
    return output;
}

int32_t osToolsCameraOpen(char *name) {
    return -1;
}

int32_t osToolsCameraReceive(char *name, uint8_t *data) {
    return -1;
}

int32_t osToolsCameraClose(char *name) {
    return -1;
}

#endif

/* created using https://notisrac.github.io/FileToCArray (32 bit RGBA) */
const uint32_t moveCursorData[1024] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xffffffff, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0xff000000, 0xffffffff, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xffffffff, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0xff000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xffffffff, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0xff000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0xff000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xffffffff, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0xff000000, 0xffffffff, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xffffffff, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xffffffff, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
};

/* created using https://notisrac.github.io/FileToCArray (32 bit RGBA) */
const uint32_t leftDiagonalCursorData[1024] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xff000000, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xff000000, 0x00000000, 0xff000000, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0xff000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0xff000000, 0xff000000, 0x00000000, 0xff000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0xff000000, 0xffffffff, 0xff000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0xff000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0xff000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0xff000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
};

/* created using https://notisrac.github.io/FileToCArray (32 bit RGBA) */
const uint32_t rightDiagonalCursorData[1024] = {
    0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0xff000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0xff000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0xff000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0xff000000, 0xffffffff, 0xff000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0xff000000, 0xff000000, 0x00000000, 0xff000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0xff000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xff000000, 0x00000000, 0xff000000, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xff000000, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
};