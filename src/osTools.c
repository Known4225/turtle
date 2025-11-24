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

#include "../include/osTools.h"

/* global objects */
ost_glfw_t osToolsGLFW;
ost_clipboard_t osToolsClipboard;
ost_file_dialog_t osToolsFileDialog;
ost_memmap_t osToolsMemmap;

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
    win32com.comList = list_init();
    win32camera.cameraList = list_init();
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

win32com_t win32com;

list_t *osToolsListComPorts() {
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
            printf("osToolsListComPorts: Unusual COM device detected on %s\n", comName);
        }
    }
    return output;
}

int32_t osToolsComOpen(char *name, osToolsBaud_t baudRate, int32_t timeoutMilliseconds) {
    /* verify COM */
    if (strlen(name) < 3 || name[0] != 'C' || name[1] != 'O' || name[2] != 'M') {
        printf("osToolsComOpen: name must start with \"COM\"\n");
        return -1;
    }
    /* check if this port is already open */
    int32_t index = list_find(win32com.comList, (unitype) name, 's');
    if (index != -1) {
        if (CloseHandle((HANDLE) (win32com.comList -> data[index + 1].l)) == 0) {
            printf("osToolsComClose failed with error %ld\n", GetLastError());
            return -1;
        }
        list_delete(win32com.comList, index);
        list_delete(win32com.comList, index);
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
        list_append(win32com.comList, (unitype) name, 's');
        list_append(win32com.comList, (unitype) comHandle, 'l');
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
    dcb.BaudRate = baudRate;    // baud rate
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
    /* Set comm timeout */
    COMMTIMEOUTS timeout = {0, 0, timeoutMilliseconds, 0, 0};
    fSuccess = SetCommTimeouts(comHandle, &timeout);
    return 0;
}

int32_t osToolsComSend(char *name, uint8_t *data, int32_t length) {
    /* check if this port is open */
    int32_t index = list_find(win32com.comList, (unitype) name, 's');
    if (index == -1) {
        printf("osToolsComSend: %s not open\n", name);
        return 0;
    }
    /* https://www.codeproject.com/Articles/3061/Creating-a-Serial-communication-on-Win32#sending */
    DWORD bytes;
    if (WriteFile((HANDLE) (win32com.comList -> data[index + 1].l), data, length, &bytes, NULL) == 0) {
        printf("osToolsComSend failed with error %ld\n", GetLastError());
        return -1;
    }
    return bytes;
}

int32_t osToolsComReceive(char *name, uint8_t *buffer, int32_t length) {
    /* check if this port is open */
    int32_t index = list_find(win32com.comList, (unitype) name, 's');
    if (index == -1) {
        printf("osToolsComReceive: %s not open\n", name);
        return 0;
    }
    /* read from COM */
    DWORD bytes;
    if (ReadFile((HANDLE) (win32com.comList -> data[index + 1].l), buffer, length, &bytes, NULL) == 0) {
        printf("osToolsComReceive failed with error %ld\n", GetLastError());
        return -1;
    }
    return bytes;
}

int32_t osToolsComClose(char *name) {
    /* check if this port is already open */
    int32_t index = list_find(win32com.comList, (unitype) name, 's');
    if (index != -1) {
        if (CloseHandle((HANDLE) (win32com.comList -> data[index + 1].l)) == 0) {
            printf("osToolsComClose failed with error %ld\n", GetLastError());
            return -1;
        }
        list_delete(win32com.comList, index);
        list_delete(win32com.comList, index);
    }
    return 0;
}

/* symbol IID_IMFMediaSource and MF_MT_FRAME_SIZE not linked despite their existence in mfapi.h and mfidl.h */
const GUID DECLSPEC_SELECTANY IID_IMFMediaSource = {0x279a808d, 0xaec7, 0x40c8, {0x9c,0x6b, 0xa6, 0xb4, 0x92, 0xc7, 0x8a, 0x66}};
const GUID DECLSPEC_SELECTANY MF_MT_FRAME_SIZE = {0x1652c33d, 0xd6b2, 0x4012, {0xb8, 0x34, 0x72, 0x03, 0x08, 0x49, 0xa3, 0x7d}};
const GUID MF_MT_MAJOR_TYPE = {0x48eba18e, 0xf8c9, 0x4687, {0xbf, 0x11, 0x0a, 0x74, 0xc9, 0xf9, 0x6a, 0x8f}};
const GUID MF_MT_SUBTYPE = {0xf7e34c9a, 0x42e8, 0x4714, {0xb7, 0x4b, 0xcb, 0x29, 0xd7, 0x2c, 0x35, 0xe5}};
const GUID MFMediaType_Video = {0x73646976, 0x0000, 0x0010, {0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71}};
const GUID MFVideoFormat_MJPG = {0x47504a4d, 0x0000, 0x0010, {0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71}}; // MJPG
const GUID MFVideoFormat_NV12 = {0x3231564e, 0x0000, 0x0010, {0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71}}; // NV12
const GUID MFVideoFormat_YV12 = {0x32315659, 0x0000, 0x0010, {0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71}}; // YV12
const GUID MFVideoFormat_H264 = {0x34363248, 0x0000, 0x0010, {0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71}}; // H264
const GUID MFVideoFormat_RGB8 = {41 /* D3DFMT_P8 */, 0x0000, 0x0010, {0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71}}; // See D3D definitions
const GUID MFVideoFormat_RGB24 = {20 /* D3DFMT_R8G8B8 */, 0x0000, 0x0010, {0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71}}; // See D3D definitions
const GUID MFImageFormat_RGB32 = {0x00000016, 0x0000, 0x0010, {0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71}};
const GUID MFVideoFormat_RGB32 = {0x00000016, 0x0000, 0x0010, {0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71}};

win32camera_t win32camera;

list_t *osToolsListCameras() {
    list_t *output = list_init();
    HRESULT hr = CoInitializeEx(NULL, 0);
    if (FAILED(hr)) {
        printf("osToolsListCameras CoInitializeEx Error: 0x%lX\n", hr);
        return output;
    }
    hr = MFStartup(MF_VERSION, MFSTARTUP_NOSOCKET);
    if (FAILED(hr)) {
        printf("osToolsListCameras MFStartup Error: 0x%lX\n", hr);
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
        printf("osToolsListCameras MFCreateAttributes Error: 0x%lX\n", hr);
        goto osToolsListCameras_done;
    }

    /* Source type: video capture devices */
    hr = pAttributes -> lpVtbl -> SetGUID(pAttributes, &MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE, &MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID);
    if (FAILED(hr)) {
        printf("osToolsListCameras SetGUID Error: 0x%lX\n", hr);
        goto osToolsListCameras_done;
    }

    /* Enumerate devices. */
    UINT32 count;
    hr = MFEnumDeviceSources(pAttributes, &ppDevices, &count);
    if (FAILED(hr)) {
        printf("osToolsListCameras MFEnumDeviceSources Error: 0x%lX\n", hr);
        goto osToolsListCameras_done;
    }

    if (count == 0) {
        hr = E_FAIL;
        printf("osToolsListCameras: Error no cameras found\n");
        goto osToolsListCameras_done;
    }
    printf("osToolsListCameras: Found %d cameras\n", count);

    /* Create the media source object. */
    for (int32_t i = 0; i < count; i++) {
        hr = ppDevices[i] -> lpVtbl -> ActivateObject(ppDevices[i], &IID_IMFMediaSource, (void **) &pSource);
        if (FAILED(hr)) {
            printf("osToolsListCameras ActivateObject Error: 0x%lX\n", hr);
            goto osToolsListCameras_done;
        }
        
        // IMFMediaType *readerType;
        // hr = MFCreateMediaType(&readerType);
        // if (FAILED(hr)) {
        //     printf("osToolsListCameras MFCreateMediaType Error: 0x%lX\n", hr);
        //     goto osToolsListCameras_done;
        // }
        // readerType -> lpVtbl -> SetUINT32(readerType, &MF_SOURCE_READER_ENABLE_VIDEO_PROCESSING, 1);
        // hr = pReader -> lpVtbl -> SetCurrentMediaType(pReader, MF_SOURCE_READER_FIRST_VIDEO_STREAM, NULL, readerType);
        // if (FAILED(hr)) {
        //     printf("osToolsListCameras SetCurrentMediaType Error: 0x%lX\n", hr);
        //     goto osToolsListCameras_done;
        // }
        // hr = readerType -> lpVtbl -> SetGUID(readerType, &MF_MT_MAJOR_TYPE, &MFMediaType_Video);
        // if (FAILED(hr)) {
        //     printf("osToolsListCameras SetGUID Error: 0x%lX\n", hr);
        //     goto osToolsListCameras_done;
        // }
        // hr = readerType -> lpVtbl -> SetGUID(readerType, &MF_MT_SUBTYPE, &MFVideoFormat_MJPG);
        // if (FAILED(hr)) {
        //     printf("osToolsListCameras SetGUID Error: 0x%lX\n", hr);
        //     goto osToolsListCameras_done;
        // }
        // // you can also set desired width/height here
        // hr = pReader -> lpVtbl -> SetCurrentMediaType(pReader, MF_SOURCE_READER_FIRST_VIDEO_STREAM, NULL, readerType);
        // if (FAILED(hr)) {
        //     printf("osToolsListCameras SetCurrentMediaType Error: 0x%lX\n", hr);
        //     goto osToolsListCameras_done;
        // }
        // readerType -> lpVtbl -> Release(readerType);
        DWORD characteristics;
        pSource -> lpVtbl -> GetCharacteristics(pSource, &characteristics);
        printf("- Source characteristics: %lX\n", characteristics);
        /* check stream type */
        IMFPresentationDescriptor *presentationDescriptor;
        pSource -> lpVtbl -> CreatePresentationDescriptor(pSource, &presentationDescriptor);
        DWORD streamCount;
        presentationDescriptor -> lpVtbl -> GetStreamDescriptorCount(presentationDescriptor, &streamCount);
        printf("- Source streams: %ld\n", streamCount);
        uint32_t width = 0;
        uint32_t height = 0;
        if (streamCount > 0) {
            int32_t selected;
            IMFStreamDescriptor *streamDescriptor;
            hr = presentationDescriptor -> lpVtbl -> GetStreamDescriptorByIndex(presentationDescriptor, 0, &selected, &streamDescriptor);
            if (FAILED(hr)) {
                printf("osToolsListCameras GetStreamDescriptorByIndex Error: 0x%lX\n", hr);
                goto osToolsListCameras_done;
            }
            IMFMediaTypeHandler *mediaTypeHandler;
            hr = streamDescriptor -> lpVtbl -> GetMediaTypeHandler(streamDescriptor, &mediaTypeHandler);
            if (FAILED(hr)) {
                printf("osToolsListCameras GetMediaTypeHandler Error: 0x%lX\n", hr);
                goto osToolsListCameras_done;
            }
            DWORD mediaTypeCount;
            mediaTypeHandler -> lpVtbl -> GetMediaTypeCount(mediaTypeHandler, &mediaTypeCount);
            printf("- Media Type Count: %ld\n", mediaTypeCount);
            for (int32_t j = 0; j < mediaTypeCount; j++) {
                printf("- Media Type %d\n", j);
                IMFMediaType *mediaType;
                hr = mediaTypeHandler -> lpVtbl -> GetMediaTypeByIndex(mediaTypeHandler, j, &mediaType);
                if (FAILED(hr)) {
                    printf("osToolsListCameras GetMediaTypeByIndex Error: 0x%lX\n", hr);
                    goto osToolsListCameras_done;
                }
                hr = mediaType -> lpVtbl -> SetUINT32(mediaType, &MF_SOURCE_READER_ENABLE_VIDEO_PROCESSING, 1);
                if (FAILED(hr)) {
                    printf("osToolsListCameras SetUINT32 Error: 0x%lX\n", hr);
                    goto osToolsListCameras_done;
                }
                GUID majorType;
                hr = mediaType -> lpVtbl -> GetMajorType(mediaType, &majorType);
                if (FAILED(hr)) {
                    printf("osToolsListCameras GetMajorType Error: 0x%lX\n", hr);
                    goto osToolsListCameras_done;
                }
                /* see mfapi.h for DEFINE_GUID (MFMediaType_Video, 0x73646976, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71); */
                if (majorType.Data1 == 0x73646976 && majorType.Data2 == 0x0000 && majorType.Data3 == 0x0010 && majorType.Data4[0] == 0x80 && majorType.Data4[1] == 0x00 && majorType.Data4[2] == 0x00 && majorType.Data4[3] == 0xaa && majorType.Data4[4] == 0x00 && majorType.Data4[5] == 0x38 && majorType.Data4[6] == 0x9b && majorType.Data4[7] == 0x71) {
                    /* MFMediaType_Video */
                    printf("  - Major type: MFMediaType_Video\n");
                    GUID subtype;
                    hr = mediaType -> lpVtbl -> GetGUID(mediaType, &MF_MT_SUBTYPE, &subtype);
                    if (FAILED(hr)) {
                        printf("osToolsListCameras GetGUID Error: 0x%lX\n", hr);
                        goto osToolsListCameras_done;
                    }
                    printf("  - Subtype: %08lx-%02hx%02hx-%02x%02x-%02x%02x%02x%02x%02x%02x\n", subtype.Data1, subtype.Data2, subtype.Data3, // 3231564E-0010-8000-00AA00389B71
                    subtype.Data4[0], subtype.Data4[1], subtype.Data4[2], subtype.Data4[3], subtype.Data4[4], subtype.Data4[5], subtype.Data4[6], subtype.Data4[7]);
                    uint64_t sizePacked;
                    hr = mediaType -> lpVtbl -> GetUINT64(mediaType, &MF_MT_FRAME_SIZE, &sizePacked);
                    if (FAILED(hr)) {
                        printf("osToolsListCameras GetUINT64 Error: 0x%lX\n", hr);
                        goto osToolsListCameras_done;
                    }
                    width = (uint32_t) (sizePacked >> 32);
                    height = (uint32_t) sizePacked;
                    BOOL compressed;
                    mediaType -> lpVtbl -> IsCompressedFormat(mediaType, &compressed);
                    printf("  - Width: %d\n", width);
                    printf("  - Height: %d\n", height);
                    printf("  - Compressed: %d\n", compressed);
                    // mediaType -> lpVtbl -> SetUINT32(mediaType, &MF_SOURCE_READER_ENABLE_VIDEO_PROCESSING, 1);
                    // mediaType -> lpVtbl -> SetGUID(mediaType, &MF_MT_SUBTYPE, &MFVideoFormat_NV12);
                    hr = mediaTypeHandler -> lpVtbl -> SetCurrentMediaType(mediaTypeHandler, mediaType);
                    if (FAILED(hr)) {
                        printf("osToolsListCameras SetCurrentMediaType Error: 0x%lX\n", hr);
                        goto osToolsListCameras_done;
                    }
                } else {
                    /* Not MFMediaType_Video */
                    printf("  - Major type: %08lx-%02hx%02hx-%02x%02x-%02x%02x%02x%02x%02x%02x\n", majorType.Data1, majorType.Data2, majorType.Data3,
                    majorType.Data4[0], majorType.Data4[1], majorType.Data4[2], majorType.Data4[3], majorType.Data4[4], majorType.Data4[5], majorType.Data4[6], majorType.Data4[7]);
                    goto osToolsListCameras_done;
                }
            }
            /* https://stackoverflow.com/questions/43507393/media-foundation-set-video-interlacing-and-decode */
            MFCreateAttributes(&pAttributes, 1);
            pAttributes -> lpVtbl -> SetUINT32(pAttributes, &MF_READWRITE_ENABLE_HARDWARE_TRANSFORMS, TRUE);
            pAttributes -> lpVtbl -> SetUINT32(pAttributes, &MF_SOURCE_READER_ENABLE_VIDEO_PROCESSING, TRUE);
            IMFSourceReader *pReader;
            hr = MFCreateSourceReaderFromMediaSource(pSource, pAttributes, &pReader);
            if (FAILED(hr)) {
                printf("osToolsListCameras MFCreateSourceReaderFromMediaSource Error: 0x%lX\n", hr);
                goto osToolsListCameras_done;
            }
            IMFMediaType *readerNativeType;
            pReader -> lpVtbl -> GetNativeMediaType(pReader, MF_SOURCE_READER_FIRST_VIDEO_STREAM, MF_SOURCE_READER_CURRENT_TYPE_INDEX, &readerNativeType);
            GUID nativeSubtype;
            readerNativeType -> lpVtbl -> GetGUID(readerNativeType, &MF_MT_SUBTYPE, &nativeSubtype);
            printf("- Native Subtype: %08lx-%02hx%02hx-%02x%02x-%02x%02x%02x%02x%02x%02x\n", nativeSubtype.Data1, nativeSubtype.Data2, nativeSubtype.Data3,
            nativeSubtype.Data4[0], nativeSubtype.Data4[1], nativeSubtype.Data4[2], nativeSubtype.Data4[3], nativeSubtype.Data4[4], nativeSubtype.Data4[5], nativeSubtype.Data4[6], nativeSubtype.Data4[7]);
            // IMFMediaType *readerType;
            // pReader -> lpVtbl -> GetCurrentMediaType(pReader, MF_SOURCE_READER_FIRST_VIDEO_STREAM, &readerType);
            // uint32_t readerTypeVideoProcessing = 0;
            // readerType -> lpVtbl -> GetUINT32(readerType, &MF_SOURCE_READER_ENABLE_VIDEO_PROCESSING, &readerTypeVideoProcessing);
            // uint32_t readerTypeD3D = 0;
            // readerType -> lpVtbl -> GetUINT32(readerType, &MF_SOURCE_READER_D3D_MANAGER, &readerTypeD3D);
            // uint32_t readerTypeDisableConverters = 0;
            // readerType -> lpVtbl -> GetUINT32(readerType, &MF_READWRITE_DISABLE_CONVERTERS, &readerTypeDisableConverters);
            // printf("readerTypeVideoProcessing: %d %d %d\n", readerTypeVideoProcessing, readerTypeD3D, readerTypeDisableConverters);
            /* https://learn.microsoft.com/en-us/windows/win32/medfound/processing-media-data-with-the-source-reader#setting-output-formats */
            IMFMediaType *readerType;
            MFCreateMediaType(&readerType);
            readerType -> lpVtbl -> SetUINT32(readerType, &MF_SOURCE_READER_ENABLE_VIDEO_PROCESSING, 1);
            hr = readerType -> lpVtbl -> SetGUID(readerType, &MF_MT_MAJOR_TYPE, &MFMediaType_Video);
            if (FAILED(hr)) {
                printf("osToolsListCameras SetGUID Error: 0x%lX\n", hr);
                goto osToolsListCameras_done;
            }
            hr = readerType -> lpVtbl -> SetGUID(readerType, &MF_MT_SUBTYPE, &MFVideoFormat_YV12);
            if (FAILED(hr)) {
                printf("osToolsListCameras SetGUID Error: 0x%lX\n", hr);
                goto osToolsListCameras_done;
            }
            hr = pReader -> lpVtbl -> SetCurrentMediaType(pReader, MF_SOURCE_READER_FIRST_VIDEO_STREAM, NULL, readerType);
            if (FAILED(hr)) {
                printf("osToolsListCameras SetCurrentMediaType Error: 0x%lX\n", hr);
                // goto osToolsListCameras_done;
            }
            char cameraString[32];
            sprintf(cameraString, "camera%d", i);
            list_append(output, (unitype) cameraString, 's');
            list_append(output, (unitype) width, 'i');
            list_append(output, (unitype) height, 'i');
            list_append(output, (unitype) 3, 'i'); // TODO
            list_append(win32camera.cameraList, (unitype) cameraString, 's');
            list_append(win32camera.cameraList, (unitype) width, 'i');
            list_append(win32camera.cameraList, (unitype) height, 'i');
            list_append(win32camera.cameraList, (unitype) 3, 'i'); // TODO
            list_append(win32camera.cameraList, (unitype) (void *) pReader, 'l');
        }
    }
osToolsListCameras_done:
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
    if (pSource) {
        pSource -> lpVtbl -> Release(pSource);
        pSource = NULL;
    }
    return output;
}

int32_t osToolsCameraOpen(char *name) {
    return -1;
}

int32_t osToolsCameraReceive(char *name, uint8_t *data) {
    int32_t index = list_find(win32camera.cameraList, (unitype) name, 's');
    if (index == -1) {
        return -1;
    }
    IMFSourceReader *pReader = (IMFSourceReader *) win32camera.cameraList -> data[index + 4].p;
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
            return -1;
        }
        if (flags & MF_SOURCE_READERF_STREAMTICK) {
            continue;
        }
        break;
    }

    IMFMediaBuffer *pBuffer;

    hr = pSample -> lpVtbl -> ConvertToContiguousBuffer(pSample, &pBuffer);
    if (FAILED(hr)) {
        return -1;
    }
    BYTE *rawBuffer;
    DWORD size;
    hr = pBuffer -> lpVtbl -> Lock(pBuffer, &rawBuffer, NULL, &size);
    if (FAILED(hr)) {
        return -1;
    }
    printf("camera buffersize: %ld\n", size);
    if (size > win32camera.cameraList -> data[index + 1].i * win32camera.cameraList -> data[index + 2].i * win32camera.cameraList -> data[index + 3].i) {
        size = win32camera.cameraList -> data[index + 1].i * win32camera.cameraList -> data[index + 2].i * win32camera.cameraList -> data[index + 3].i;
    }
    memcpy(data, rawBuffer, size);

    pBuffer -> lpVtbl -> Unlock(pBuffer);
    pBuffer -> lpVtbl -> Release(pBuffer);
    pSample -> lpVtbl -> Release(pSample);
    return -1;
}

int32_t osToolsCameraClose(char *name) {
    return -1;
}

/*
https://gist.github.com/mmozeiko/c0dfcc8fec527a90a02145d2cc0bfb6d
https://learn.microsoft.com/en-us/windows/win32/winsock/complete-server-code
https://learn.microsoft.com/en-us/windows/win32/winsock/complete-client-code
*/

win32socket_t win32socket;

int32_t win32tcpInit(char *address, char *port) {
    for (int32_t i = 0; i < WIN32TCP_NUM_SOCKETS; i++) {
        win32socket.connectSocket[i] = 0;
        win32socket.socketOpen[i] = 0;
    }
    win32socket.address = address;
    win32socket.port = port;
    char modifiable[strlen(address) + 1];
    strcpy(modifiable, address);
    char *check = strtok(modifiable, ".");
    int32_t segments = 0;
    uint8_t ipAddress[4] = {0};
    while (check != NULL) {
        if (segments > 3) {
            printf("Could not initialise win32tcp - invalid ip address\n");
            return 1;
        }
        int32_t segmentValue = atoi(check);
        if (segmentValue > 255 || segmentValue < 0) {
            printf("Could not initialise win32tcp - invalid ip address\n");
            return 1;
        }
        ipAddress[segments] = segmentValue;
        check = strtok(NULL, ".");
        segments++;
    }
    if (segments != 4) {
        printf("Could not initialise win32tcp - invalid ip address\n");
        return 1;
    }
    /* Initialize Winsock */
    WSADATA wsaData;
    int32_t status;
    status = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (status != 0) {
        return 1;
    }
    
    /* hints */
    struct addrinfo hints;
    struct addrinfo *result;
    struct addrinfo *ptr;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    /* Resolve the server address and port */
    status = getaddrinfo(address, port, &hints, &result);
    if (status != 0) {
        WSACleanup();
        return 1;
    }

    /* Attempt to connect to an address until one succeeds */
    for (ptr = result; ptr != NULL; ptr = ptr -> ai_next) {
        /* Create a SOCKET for connecting to server */
        win32socket.connectSocket[0] = socket(ptr -> ai_family, ptr -> ai_socktype, ptr -> ai_protocol);
        if (win32socket.connectSocket[0] == INVALID_SOCKET) {
            WSACleanup();
            return 1;
        }

        /* Connect to server */
        status = connect(win32socket.connectSocket[0], ptr -> ai_addr, (int) ptr -> ai_addrlen);
        if (status == SOCKET_ERROR) {
            closesocket(win32socket.connectSocket[0]);
            win32socket.connectSocket[0] = INVALID_SOCKET;
            continue;
        }
        break;
    }

    // /* Send an initial buffer */
    // char sendbuf[2] = {12, 34};
    // status = send(win32socket.connectSocket[0], sendbuf, 2, 0);
    // if (status == SOCKET_ERROR) {
    //     closesocket(win32socket.connectSocket[0]);
    //     WSACleanup();
    //     return 1;
    // }

    // printf("Bytes Sent: %ld\n", status);

    /* shutdown the connection since no more data will be sent */
    status = shutdown(win32socket.connectSocket[0], SD_SEND);
    if (status == SOCKET_ERROR) {
        closesocket(win32socket.connectSocket[0]);
        WSACleanup();
        return 1;
    }

    /* Receive until the peer closes the connection */
    int32_t recvbuflen = 512;
    char recvbuf[recvbuflen];
    status = 1;
    while (status > 0) {
        status = recv(win32socket.connectSocket[0], recvbuf, recvbuflen, 0);
        if (status > 0) {
            // printf("Bytes received: %d\n", status);
        } else if (status == 0) {
            // printf("Connection closed\n");
        } else {
            // printf("recv failed with error: %d\n", WSAGetLastError());
        }
    }

    /* cleanup */
    closesocket(win32socket.connectSocket[0]);
    printf("Successfully connected to %d.%d.%d.%d\n", ipAddress[0], ipAddress[1], ipAddress[2], ipAddress[3]);
    return 0;
}

SOCKET *win32tcpCreateSocket(int32_t timeoutMilliseconds) {
    /* define socket index */
    int32_t socketIndex = 0;
    for (int32_t i = 0; i < WIN32TCP_NUM_SOCKETS; i++) {
        if (win32socket.socketOpen[i] == 0) {
            win32socket.socketOpen[i] = 1;
            socketIndex = i;
            break;
        }
    }
    if (socketIndex == WIN32TCP_NUM_SOCKETS) {
        /* no sockets left */
        return NULL;
    }
    /* hints */
    struct addrinfo hints;
    struct addrinfo *result;
    struct addrinfo *ptr;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    /* Resolve the server address and port */
    int32_t status = getaddrinfo(win32socket.address, win32socket.port, &hints, &result);
    if (status != 0) {
        WSACleanup();
        return NULL;
    }

    /* Attempt to connect to an address until one succeeds */
    for (ptr = result; ptr != NULL; ptr = ptr -> ai_next) {
        /* Create a SOCKET for connecting to server */
        win32socket.connectSocket[socketIndex] = socket(ptr -> ai_family, ptr -> ai_socktype, ptr -> ai_protocol);
        if (win32socket.connectSocket[socketIndex] == INVALID_SOCKET) {
            WSACleanup();
            return NULL;
        }
        /* Connect to server */
        setsockopt(win32socket.connectSocket[socketIndex], SOL_SOCKET, SO_RCVTIMEO, (const char *) &timeoutMilliseconds, sizeof(timeoutMilliseconds));
        status = connect(win32socket.connectSocket[socketIndex], ptr -> ai_addr, (int) ptr -> ai_addrlen);
        if (status == SOCKET_ERROR) {
            closesocket(win32socket.connectSocket[socketIndex]);
            win32socket.connectSocket[socketIndex] = INVALID_SOCKET;
            continue;
        }
        break;
    }
    // uint32_t mode;
    // printf("ioctlsocket %d\n", ioctlsocket(win32socket.connectSocket[socketIndex], FIONBIO, &mode));
    return &win32socket.connectSocket[socketIndex];
}

int32_t win32tcpSend(SOCKET *socket, uint8_t *data, int32_t length) {
    int32_t status = send(*socket, (const char *) data, length, 0 );
    if (status == SOCKET_ERROR) {
        closesocket(*socket);
        return 1;
    }
    return 0;
}

int32_t win32tcpReceive(SOCKET *socket, uint8_t *buffer, int32_t length) {
    int32_t status = 1;
    status = recv(*socket, (char *) buffer, length, 0);
    if (status > 0) {
        // printf("Bytes received: %d\n", status);
    } else if (status == 0) {
        // printf("Connection closed\n");
    } else {
        // printf("recv failed with error: %d\n", WSAGetLastError());
    }
    return status;
}

void win32tcpDeinit() {
    WSACleanup();
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

list_t *osToolsListComPorts() {
    list_t *output = list_init();
    return output;
}

int32_t osToolsComOpen(char *name, osToolsBaud_t baudRate, int32_t timeoutMilliseconds) {
    return -1;
}

int32_t osToolsComSend(char *name, uint8_t *data, int32_t length) {
    return -1;
}

int32_t osToolsComReceive(char *name, uint8_t *buffer, int32_t length) {
    return -1;
}

int32_t osToolsComClose(char *name) {
    return -1;
}

list_t *osToolsListCameras() {
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