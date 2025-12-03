//
// Created by Ajeszny on 3.12.2025.
//

#ifndef SUS_RESPONSE_H
#define SUS_RESPONSE_H

#include <stdint.h>

struct header {
    char* name;
    char* body;
};
struct http_response {
    int code;
    char* message;
    struct header* headers;
    int headers_num;
    uint8_t* body;
    int body_size;
};
struct byte_array {
    uint8_t* arr;
    size_t size;
};
struct byte_array formulate_response(struct http_response);
void add_header(struct http_response* response, const char* header_name, const char* header_value);
void free_response(struct http_response r);
#endif //SUS_RESPONSE_H
