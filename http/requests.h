//
// Created by Ajeszny on 2.12.2025.
//

#ifndef SUS_REQUESTS_H
#define SUS_REQUESTS_H
#include <stdint.h>
enum type {
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    CONNECT,
    OPTIONS,
    TRACE,
    PATCH
};
struct http_request {
    enum type type;
    char* path;
    char* version;
    char* host;
    char* user_agent;
    char* payload;
    char* accept;
    char* accept_language;
    char* accept_encoding;
    char* connection;
    char* cookie;
    uint8_t* body;
    size_t body_size
};

struct http_request parse_http(const char* http, size_t blocksize);
void free_request(struct http_request r);
#endif //SUS_REQUESTS_H
