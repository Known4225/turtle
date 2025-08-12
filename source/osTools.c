#include "include/osTools.h"

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

/* global objects */
osToolsGLFWObject osToolsGLFW;
osToolsClipboardObject osToolsClipboard;
osToolsFileDialogObject osToolsFileDialog;
osToolsMemmapObject osToolsMemmap;

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

list_t *osToolsLoadInternal(char *filename, osToolsCSV rowOrColumn, char delimeter, osToolsCSV fieldType) {
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
list_t *osToolsLoadCSV(char *filename, osToolsCSV rowOrColumn) {
    return osToolsLoadInternal(filename, rowOrColumn, ',', OSTOOLS_CSV_FIELD_DOUBLE);
}

/* packages a CSV file into a list (headers are strings, all fields are doubles) - use OSTOOLS_CSV_ROW to put it in a list of lists where each list is a row of the CSV and use OSTOOLS_CSV_COLUMN to output a list of lists where each list is a column of the CSV */
list_t *osToolsLoadCSVDouble(char *filename, osToolsCSV rowOrColumn) {
    return osToolsLoadInternal(filename, rowOrColumn, ',', OSTOOLS_CSV_FIELD_DOUBLE);
}

/* packages a CSV file into a list (headers are strings, all fields are ints) - use OSTOOLS_CSV_ROW to put it in a list of lists where each list is a row of the CSV and use OSTOOLS_CSV_COLUMN to output a list of lists where each list is a column of the CSV */
list_t *osToolsLoadCSVInt(char *filename, osToolsCSV rowOrColumn) {
    return osToolsLoadInternal(filename, rowOrColumn, ',', OSTOOLS_CSV_FIELD_INT);
}

/* packages a CSV file into a list (headers are strings, all fields are strings) - use OSTOOLS_CSV_ROW to put it in a list of lists where each list is a row of the CSV and use OSTOOLS_CSV_COLUMN to output a list of lists where each list is a column of the CSV */
list_t *osToolsLoadCSVString(char *filename, osToolsCSV rowOrColumn) {
    return osToolsLoadInternal(filename, rowOrColumn, ',', OSTOOLS_CSV_FIELD_STRING);
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

/*
windows COM port support
https://learn.microsoft.com/en-us/windows/win32/devio/configuring-a-communications-resource
*/

win32ComPortObject win32com;

/* opens a com port */
int win32comInit(win32ComPortObject *com, char *name) {
    strcpy(com -> name, name);
    DCB dcb;
    BOOL fSuccess;
    /* https://support.microsoft.com/en-us/topic/howto-specify-serial-ports-larger-than-com9-db9078a5-b7b6-bf00-240f-f749ebfd913e */
    char comName[24] = "\\\\.\\";
    if (strlen(name) > 4) {
        strcpy(comName + strlen(comName), name);
    } else {
        strcpy(comName, name);
    }
    com -> comHandle = CreateFileA(comName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (com -> comHandle == INVALID_HANDLE_VALUE) {
        printf("Could not open com port %s, error %d\n", name, GetLastError());
        return -1;
    } else {
        printf("Successfully opened port %s\n", name);
    }
    /* Initialize the DCB structure. */
    SecureZeroMemory(&dcb, sizeof(DCB));
    dcb.DCBlength = sizeof(DCB);
    /* Build on the current configuration by first retrieving all current */
    /* settings. */
    fSuccess = GetCommState(com -> comHandle, &dcb);
    if (!fSuccess) {
        /* Handle the error. */
        printf("GetCommState failed with error %d.\n", GetLastError());
        return -1;
    }
    /* Fill in some DCB values and set the com state: 
       115200 bps, 8 data bits, no parity, and 1 stop bit. */
    dcb.BaudRate = CBR_115200;    // baud rate
    dcb.ByteSize = 8;             // data size, xmit and rcv
    dcb.Parity   = NOPARITY;      // parity bit
    dcb.StopBits = ONESTOPBIT;    // stop bit
    fSuccess = SetCommState(com -> comHandle, &dcb);
    if (!fSuccess) {
        /* Handle the error. */
        printf("SetCommState failed with error %d.\n", GetLastError());
        return -1;
    }
    /* Get the comm config again. */
    fSuccess = GetCommState(com -> comHandle, &dcb);
    if (!fSuccess) {
        /* Handle the error. */
        printf("GetCommState failed with error %d.\n", GetLastError());
        return -1;
    }
    return 0;
}

/* returns number of bytes sent */
int win32comSend(win32ComPortObject *com, unsigned char *data, int length) {
    /* https://www.codeproject.com/Articles/3061/Creating-a-Serial-communication-on-Win32#sending */
    DWORD bytes;
    if (WriteFile(com -> comHandle, data, length, &bytes, NULL) == 0) {
        printf("win32comSend failed with error %d\n", GetLastError());
        return -1;
    }
    return bytes;
}

/* returns number of bytes received */
int win32comReceive(win32ComPortObject *com, unsigned char *buffer, int length) {
    DWORD bytes;
    if (ReadFile(com -> comHandle, buffer, length, &bytes, NULL) == 0) {
        printf("win32comReceive failed with error %d\n", GetLastError());
        return -1;
    }
    return bytes;
}

/* closes a com port */
int win32comClose(win32ComPortObject *com) {
    if (CloseHandle(com -> comHandle) == 0) {
        printf("win32comClosefailed with error %d\n", GetLastError());
        return -1;
    }
    return 0;
}

/*
https://gist.github.com/mmozeiko/c0dfcc8fec527a90a02145d2cc0bfb6d
https://learn.microsoft.com/en-us/windows/win32/winsock/complete-server-code
https://learn.microsoft.com/en-us/windows/win32/winsock/complete-client-code
*/

win32SocketObject win32Socket;

int win32tcpInit(char *address, char *port) {
    for (int i = 0; i < WIN32TCP_NUM_SOCKETS; i++) {
        win32Socket.connectSocket[i] = 0;
        win32Socket.socketOpen[i] = 0;
    }
    win32Socket.address = address;
    win32Socket.port = port;
    char modifiable[strlen(address) + 1];
    strcpy(modifiable, address);
    char *check = strtok(modifiable, ".");
    int segments = 0;
    unsigned char ipAddress[4] = {0};
    while (check != NULL) {
        if (segments > 3) {
            printf("Could not initialise win32tcp - invalid ip address\n");
            return 1;
        }
        int segmentValue = atoi(check);
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
    int status;
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
        win32Socket.connectSocket[0] = socket(ptr -> ai_family, ptr -> ai_socktype, ptr -> ai_protocol);
        if (win32Socket.connectSocket[0] == INVALID_SOCKET) {
            WSACleanup();
            return 1;
        }

        /* Connect to server */
        status = connect(win32Socket.connectSocket[0], ptr -> ai_addr, (int) ptr -> ai_addrlen);
        if (status == SOCKET_ERROR) {
            closesocket(win32Socket.connectSocket[0]);
            win32Socket.connectSocket[0] = INVALID_SOCKET;
            continue;
        }
        break;
    }

    // /* Send an initial buffer */
    // char sendbuf[2] = {12, 34};
    // status = send(win32Socket.connectSocket[0], sendbuf, 2, 0);
    // if (status == SOCKET_ERROR) {
    //     closesocket(win32Socket.connectSocket[0]);
    //     WSACleanup();
    //     return 1;
    // }

    // printf("Bytes Sent: %ld\n", status);

    /* shutdown the connection since no more data will be sent */
    status = shutdown(win32Socket.connectSocket[0], SD_SEND);
    if (status == SOCKET_ERROR) {
        closesocket(win32Socket.connectSocket[0]);
        WSACleanup();
        return 1;
    }

    /* Receive until the peer closes the connection */
    int recvbuflen = 512;
    char recvbuf[recvbuflen];
    do {

        status = recv(win32Socket.connectSocket[0], recvbuf, recvbuflen, 0);
        if (status > 0) {
            // printf("Bytes received: %d\n", status);
        } else if (status == 0) {
            // printf("Connection closed\n");
        } else {
            // printf("recv failed with error: %d\n", WSAGetLastError());
        }

    } while (status > 0);

    /* cleanup */
    closesocket(win32Socket.connectSocket[0]);
    printf("Successfully connected to %d.%d.%d.%d\n", ipAddress[0], ipAddress[1], ipAddress[2], ipAddress[3]);
    return 0;
}

int win32newSocket() {

}

SOCKET *win32tcpCreateSocket() {
    /* define socket index */
    int socketIndex = 0;
    for (int i = 0; i < WIN32TCP_NUM_SOCKETS; i++) {
        if (win32Socket.socketOpen[i] == 0) {
            win32Socket.socketOpen[i] = 1;
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
    int status = getaddrinfo(win32Socket.address, win32Socket.port, &hints, &result);
    if (status != 0) {
        WSACleanup();
        return NULL;
    }

    /* Attempt to connect to an address until one succeeds */
    for (ptr = result; ptr != NULL; ptr = ptr -> ai_next) {

        /* Create a SOCKET for connecting to server */
        win32Socket.connectSocket[socketIndex] = socket(ptr -> ai_family, ptr -> ai_socktype, ptr -> ai_protocol);
        if (win32Socket.connectSocket[socketIndex] == INVALID_SOCKET) {
            WSACleanup();
            return NULL;
        }

        /* Connect to server */
        status = connect(win32Socket.connectSocket[socketIndex], ptr -> ai_addr, (int) ptr -> ai_addrlen);
        if (status == SOCKET_ERROR) {
            closesocket(win32Socket.connectSocket[socketIndex]);
            win32Socket.connectSocket[socketIndex] = INVALID_SOCKET;
            continue;
        }
        break;
    }
    // unsigned long mode;
    // printf("ioctlsocket %d\n", ioctlsocket(win32Socket.connectSocket[socketIndex], FIONBIO, &mode));
    return &win32Socket.connectSocket[socketIndex];
}

int win32tcpSend(SOCKET *socket, unsigned char *data, int length) {
    int status = send(*socket, data, length, 0 );
    if (status == SOCKET_ERROR) {
        closesocket(*socket);
        return 1;
    }
    return 0;
}

int win32tcpReceive(SOCKET *socket, unsigned char *buffer, int length) {
    int status = 1;
    int bytes = 0;
    while (status > 0) {
        status = recv(*socket, buffer, length, 0);
        if (status > 0) {
            // printf("Bytes received: %d\n", status);
        } else if (status == 0) {
            // printf("Connection closed\n");
        } else {
            // printf("recv failed with error: %d\n", WSAGetLastError());
        }
        bytes += status;
        if (bytes >= length) {
            return bytes;
        }
    }
    return bytes;
}

int win32tcpReceive2(SOCKET *socket, unsigned char *buffer, int length) {
    int status = 1;
    status = recv(*socket, buffer, length, 0);
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
    return 0;
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
