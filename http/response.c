//
// Created by Ajeszny on 3.12.2025.
//

#include <heapapi.h>
#include "response.h"

void add_header(struct http_response* response, const char* header_name, const char* header_value) {
    response->headers[response->headers_num-1].name = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (strlen(header_name)+1)*sizeof(char));
    strcpy(response->headers[response->headers_num-1].name, header_name);
    response->headers[response->headers_num-1].body = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (strlen(header_value)+1)*sizeof(char));
    strcpy(response->headers[response->headers_num-1].body, header_value);
    response->headers_num += 1;
}

void free_response(struct http_response r) {
    for (int i = 0; i < r.headers_num; ++i) {
        HeapFree(GetProcessHeap(), HEAP_ZERO_MEMORY, r.headers[i].body);
        HeapFree(GetProcessHeap(), HEAP_ZERO_MEMORY, r.headers[i].name);
    }
    r.headers = 0;
    HeapFree(GetProcessHeap(), HEAP_ZERO_MEMORY, r.headers);
    r.body_size = 0;
    HeapFree(GetProcessHeap(), HEAP_ZERO_MEMORY, r.body);
    HeapFree(GetProcessHeap(), HEAP_ZERO_MEMORY, r.message);
}

struct byte_array formulate_response(struct http_response r) {
    size_t size = 0;
    char digits[10] = {0};
    itoa(r.code, digits, 10);
    size += strlen("HTTP/1.1 ")
            + strlen(digits)
            + 1//space
            + strlen(r.message);
    for (int i = 0; i < r.headers_num; ++i) {
        size += strlen(r.headers[i].name) + 1;
        size += strlen(r.headers[i].body);
    }
    size += 1;//\n
    struct byte_array result = {0};
    result.arr = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size*sizeof(char)+r.body_size);
    result.size = size+r.body_size;
    strcat(result.arr, "HTTP/1/1 ");
    strcat(result.arr, digits);
    strcat(result.arr, " ");
    strcat(result.arr, r.message);
    strcat(result.arr, "\n");
    for (int i = 0; i < r.headers_num; ++i) {
        strcat(result.arr, r.headers[i].name);
        strcat(result.arr, " ");
        strcat(result.arr, r.headers[i].body);
        strcat(result.arr, "\n");
    }
    strcat(result.arr, "\n");
    memcpy(result.arr + size, r.body, r.body_size);
    return result;
}