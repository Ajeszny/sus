//
// Created by Ajeszny on 2.12.2025.
//

#include "requests.h"
#include <string.h>
#include <heapapi.h>
void parse_row(struct http_request* request, char* row);
struct http_request parse_http(const char* http, size_t blocksize) {
    struct http_request res = {0};
    char* requestcopy = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, blocksize*sizeof(char));
    memcpy(requestcopy, http, blocksize);
    char* cursor = requestcopy;
    char* first_row = strtok(requestcopy, "\r\n");
    cursor += strlen(first_row) + 1;
    char* type = strtok(first_row, " ");
    if (strcmp(type, "GET") == 0) {
        res.type = GET;
    } else if (strcmp(type, "POST") == 0) {
        res.type = POST;
    } else if (strcmp(type, "PUT") == 0) {
        res.type = PUT;
    }
    char* route = strtok(NULL, " ");
    res.path = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (strlen(route)+1)*sizeof(char));
    strcpy(res.path, route);
    char* version = strtok(NULL, " ");
    res.version = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (strlen(version)+1)*sizeof(char));
    strcpy(res.version, version);
    char* row = strtok(cursor, "\r\n");
    cursor += strlen(row) + 2;
    while (*row != '\0'&&*row != '\n') {
        parse_row(&res, row);
        //Block for parsing body
        char* body = strstr(cursor, "\n\r\n");
        if (body == cursor) {
            //Body section has been reached and we can stop parsing
            body += 3;
            size_t body_size = blocksize - (body - requestcopy);
            res.body_size = body_size;
            res.body = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, body_size*sizeof(char));
            memcpy(res.body, body, body_size);
            break;
        }
        char* new_row = strtok(cursor, "\r\n");
        if (!new_row) {
            break;
        }
        row = new_row;
        cursor += strlen(row) + 2;
    }
    res.payload = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (strlen(row)+1)*sizeof(char));
    strcpy(res.payload, row);
    HeapFree(GetProcessHeap(), HEAP_ZERO_MEMORY, requestcopy);
    return res;
}

void parse_row(struct http_request* request, char* row) {
    char* header = strtok(row, " ");
    char* value = row + strlen(header) + 1;
    if (strcmp(header, "Host:") == 0) {
        request->host = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (strlen(value)+1)*sizeof(char));
        strcpy(request->host, value);
    } else if (strcmp(header, "User-Agent:") == 0) {
        request->user_agent = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (strlen(value)+1)*sizeof(char));
        strcpy(request->user_agent, value);
    } else if (strcmp(header, "Accept:") == 0) {
        request->accept = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (strlen(value)+1)*sizeof(char));
        strcpy(request->accept, value);
    } else if (strcmp(header, "Accept-Language:") == 0) {
        request->accept_language = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (strlen(value)+1)*sizeof(char));
        strcpy(request->accept_language, value);
    } else if (strcmp(header, "Accept-Encoding:") == 0) {
        request->accept_encoding = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (strlen(value)+1)*sizeof(char));
        strcpy(request->accept_encoding, value);
    } else if (strcmp(header, "Connection:") == 0) {
        request->connection = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (strlen(value)+1)*sizeof(char));
        strcpy(request->connection, value);
    } else if (strcmp(header, "Cookie:") == 0) {
        request->cookie = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (strlen(value)+1)*sizeof(char));
        strcpy(request->cookie, value);
    } else {
        return;
    }
}