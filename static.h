//
// Created by Ajeszny on 3.12.2025.
//

#ifndef SUS_STATIC_H
#define SUS_STATIC_H
#include "http/response.h"
#include "http/requests.h"
struct http_response not_found(struct http_request request);
struct http_response serve_static_file(struct http_request request);
#endif //SUS_STATIC_H
