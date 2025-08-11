/*
windows COM port support
https://learn.microsoft.com/en-us/windows/win32/devio/configuring-a-communications-resource
*/

#ifndef WIN32COM
#define WIN32COM 1 // include guard

#include <windows.h>
#include <shobjidl.h>
#include <string.h>

typedef struct {
    char name[32];
    HANDLE comHandle;
} win32ComPortObject;

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

#endif