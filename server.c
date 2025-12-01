//
// Created by Ajeszny on 1.12.2025.
//

#include "server.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

int last_error;
SOCKET listener = INVALID_SOCKET;
_Atomic int running = false;
_Atomic int file_size;
HANDLE mutex_listener;
void serve_client(SOCKET connection);
DWORD WINAPI client_serve_handler( LPVOID client ) {
    serve_client((SOCKET)client);
}

int init_server(int port) {
    char port_buf[6] = {0};
    itoa(port, port_buf, 10);
    WSADATA goof;
    last_error = WSAStartup(MAKEWORD(2,2), &goof);
    if (last_error != 0) {
        return -1;
    }
    struct addrinfo *result = NULL, hints;

    ZeroMemory(&hints, sizeof (hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    last_error = getaddrinfo(NULL, port_buf, &hints, &result);
    if (last_error != 0) {
        return -2;
    }
    mutex_listener = CreateMutex(
            NULL,              // default security attributes
            FALSE,             // initially not owned
            NULL);             // unnamed mutex
    DWORD wait_result = WaitForSingleObject(mutex_listener, INFINITE);
    if (wait_result == WAIT_OBJECT_0) {
        listener = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        last_error = bind(listener, result->ai_addr, (int)result->ai_addrlen);
        if (last_error == SOCKET_ERROR) {
            return -3;
        }
        last_error = listen(listener, SOMAXCONN);
        if (last_error == SOCKET_ERROR) {
            return -4;
        }
        ReleaseMutex(mutex_listener);
    }
    running = true;
    return 0;
}
void start_listening() {
    LPDWORD* thread_identifiers = NULL;
    HANDLE* thread_handlers = NULL;
    int handlers_num = 0;
    while (running) {
        SOCKET client = accept(listener, NULL, NULL);
        if (client == INVALID_SOCKET) {
            break;
        }
        ++handlers_num;
        thread_handlers = HeapReAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, thread_handlers, handlers_num*sizeof(LPDWORD));
        thread_identifiers = HeapReAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, thread_handlers, handlers_num*sizeof(LPDWORD));

        thread_handlers[handlers_num-1] = CreateThread(
                NULL,
                0,
                client_serve_handler,
                (LPVOID)client,
                0,
                thread_identifiers[handlers_num-1]
                );
    }
    WaitForMultipleObjects(handlers_num, thread_handlers, TRUE, INFINITE);
    for (int i = 0; i < handlers_num; ++i) {
        if (thread_handlers != NULL) {
            CloseHandle(thread_handlers[i]);
        }//Completely useless check but my IDE gives me warning and I don't like warnings
    }
    if (thread_handlers != NULL) {
         HeapFree(GetProcessHeap(), 0, thread_handlers);
    }
    if (thread_identifiers != NULL) {
        HeapFree(GetProcessHeap(), 0, thread_identifiers);
    }

}

void stop_server() {
    running = false;
    closesocket(listener);
    WSACleanup();
}

void serve_client(SOCKET connection) {

}

