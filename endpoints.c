//
// Created by Ajeszny on 3.12.2025.
//

#include <heapapi.h>
#include <time.h>
#include <fileapi.h>
#include <handleapi.h>
#include <winsock2.h>
#include "endpoints.h"
#include "mime/types.h"


HANDLE mutex_notfound;
HANDLE mutex_static;

void init_default_endpoints() {
    mutex_notfound = CreateMutex(
            NULL,              // default security attributes
            FALSE,             // initially not owned
            NULL);             // unnamed mutex
    mutex_static = CreateMutex(
            NULL,              // default security attributes
            FALSE,             // initially not owned
            NULL);             // unnamed mutex
}

struct http_response populate_response() {
    struct http_response response = {0};
    response.body = NULL;
    response.body_size = 0;
    response.headers = NULL;
    const char* header_names[] = {"Server:", "Mime-Version:", "Content-Type:", "Expires:", "Date:", "Connection:"};
    time_t rawtime, newtime;
    struct tm * timeinfo, *newtimeinfo;
    time (&rawtime);
    newtime = rawtime + 10000;//10000 seconds from now
    timeinfo = localtime (&rawtime);
    newtimeinfo = localtime (&newtime);
    char datetime_when[255];
    char datetime_to[255];
    strftime(datetime_when, 255, "%c", timeinfo);
    strftime(datetime_to, 255, "%c", newtimeinfo);
    const char* header_values[] = {"SUS", "1.0", "text/html", datetime_when, datetime_to, "close"};
    for (int i = 0; i < 6; ++i) {
        add_header(&response, header_names[i], header_values[i]);
    }
    return response;
}

struct http_response not_found_default(struct http_request request) {
    struct http_response response = populate_response();
    response.code = 404;
    response.message = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(char)*(strlen("Not found")+1));
    return response;
}
struct http_response serve_static_file_default(struct http_request request) {
    char* truepath = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(char)*(1+strlen(request.path)+strlen("static/")));
    strcat(truepath, "static");
    strcat(truepath, request.path);
    HANDLE f = CreateFileA(truepath,
                           GENERIC_READ,
                           FILE_SHARE_READ,
                           NULL,
                           OPEN_EXISTING,
                           FILE_ATTRIBUTE_NORMAL,
                           NULL);
    if (f == INVALID_HANDLE_VALUE) {
        return not_found(request);
    }
    DWORD fsize = GetFileSize(f, NULL);
    LPVOID buffer = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, fsize+1);
    BOOL result = ReadFile(f, buffer, fsize, NULL, NULL);
    CloseHandle(f);
    if (!result) {
        HeapFree(GetProcessHeap(), HEAP_ZERO_MEMORY, buffer);
        return not_found(request);
    }
    struct http_response response = {0};
    response.code = 200;
    response.body = buffer;
    response.body_size = fsize;
    response.message = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(char)*(strlen("Not found")+1));
    response.headers = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(struct header)*7);
    const char* header_names[] = {"Server:", "Mime-Version:", "Content-Type:", "Content-Length:", "Expires:", "Date:", "Connection:"};
    char content_length[16];
    itoa(response.body_size, content_length, 10);
    time_t rawtime, newtime;
    struct tm * timeinfo, *newtimeinfo;
    time (&rawtime);
    newtime = rawtime + 10000;//10000 seconds from now
    timeinfo = localtime (&rawtime);
    newtimeinfo = localtime (&newtime);
    char datetime_when[255];
    char datetime_to[255];
    strftime(datetime_when, 255, "%c", timeinfo);
    strftime(datetime_to, 255, "%c", newtimeinfo);
    const char* header_values[] = {"SUS", "1.0", determine_mime_type(request.path), content_length, datetime_when, datetime_to, "close"};//Create an extension/MM determining function later
    for (int i = 0; i < 7; ++i) {
        add_header(&response, header_names[i], header_values[i]);
    }
    return response;
}

struct http_response (*not_found_ptr)(struct http_request request) = not_found_default;
struct http_response (*serve_static_file_ptr)(struct http_request request) = serve_static_file_default;

struct http_response not_found(struct http_request request) {
    DWORD wait_result = WaitForSingleObject(mutex_notfound, INFINITE);
    if (wait_result == WAIT_OBJECT_0) {
        struct http_response (*hihi)(struct http_request request) = not_found_ptr;
        ReleaseMutex(mutex_notfound);
        return hihi(request);
    }
}

struct http_response serve_static_file(struct http_request request) {
    DWORD wait_result = WaitForSingleObject(mutex_static, INFINITE);

    if (wait_result == WAIT_OBJECT_0) {
        struct http_response (*hihi)(struct http_request request) = serve_static_file_ptr;
        ReleaseMutex(mutex_static);
        return hihi(request);
    }
}

void set_notfound(struct http_response (*handler)(struct http_request)) {
    DWORD wait_result = WaitForSingleObject(mutex_notfound, INFINITE);
    if (wait_result == WAIT_OBJECT_0) {
        not_found_ptr = handler;
        ReleaseMutex(mutex_notfound);
    }
}

void override_static(struct http_response (*handler)(struct http_request)) {
    DWORD wait_result = WaitForSingleObject(mutex_static, INFINITE);
    if (wait_result == WAIT_OBJECT_0) {
        serve_static_file_ptr = handler;
        ReleaseMutex(mutex_static);
    }
}
