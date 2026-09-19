#include <stdio.h>
#include <heapapi.h>
#include <time.h>
#include <fileapi.h>
#include <handleapi.h>
#include "server.h"
#include "endpoints.h"

struct http_response my_not_found(struct http_request request) {
    struct http_response response = populate_response();
    response.code = 200;
    HANDLE f = CreateFileA("notfound.html",
                           GENERIC_READ,
                           FILE_SHARE_READ,
                           NULL,
                           OPEN_EXISTING,
                           FILE_ATTRIBUTE_NORMAL,
                           NULL);
    if (f == INVALID_HANDLE_VALUE) {
        return response;
    }

    DWORD fsize = GetFileSize(f, NULL);
    LPVOID buffer = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, fsize+1);
    BOOL result = ReadFile(f, buffer, fsize, NULL, NULL);
    CloseHandle(f);
    if (!result) {
        HeapFree(GetProcessHeap(), HEAP_ZERO_MEMORY, buffer);
        return response;
    }
    response.body = buffer;
    response.body_size = fsize;
    response.message = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(char)*(strlen("Not found")+1));
    char content_length[16];
    itoa(response.body_size, content_length, 10);
    add_header(&response, "Content-Length", content_length);
    return response;
}

int main() {
    init_server(3000);
    printf("Running on port 3000");
    set_notfound(my_not_found);
    start_listening();
    stop_server();
    return 0;
}
