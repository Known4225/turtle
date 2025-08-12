/*
https://gist.github.com/mmozeiko/c0dfcc8fec527a90a02145d2cc0bfb6d
https://learn.microsoft.com/en-us/windows/win32/winsock/complete-server-code
https://learn.microsoft.com/en-us/windows/win32/winsock/complete-client-code
*/

#ifndef WIN32TCP
#define WIN32TCP 1 // include guard

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <shobjidl.h>
#include <string.h>

#define WIN32TCP_NUM_SOCKETS 32

typedef struct {
    char *address;
    char *port;
    SOCKET connectSocket[WIN32TCP_NUM_SOCKETS];
    char socketOpen[WIN32TCP_NUM_SOCKETS];
} win32SocketObject;

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