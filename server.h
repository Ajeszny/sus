//
// Created by Ajeszny on 1.12.2025.
//

#ifndef SUS_SERVER_H
#define SUS_SERVER_H
#include "http/requests.h"
#include "http/response.h"
int init_server(int port);
void start_listening();
void stop_server();
int add_endpoint(const char* route, struct http_response (*handler)(struct http_request));
#endif //SUS_SERVER_H
