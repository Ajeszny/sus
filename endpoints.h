//
// Created by Ajeszny on 3.12.2025.
//

#ifndef SUS_ENDPOINTS_H
#define SUS_ENDPOINTS_H
#include "http/response.h"
#include "http/requests.h"
struct http_response not_found(struct http_request request);
struct http_response serve_static_file(struct http_request request);
void set_notfound(struct http_response (*handler)(struct http_request));
void override_static(struct http_response (*handler)(struct http_request));
void init_default_endpoints();
#endif //SUS_ENDPOINTS_H
