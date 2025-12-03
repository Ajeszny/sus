//
// Created by Ajeszny on 2.12.2025.
//
#include "http/requests.h"
#include <stdio.h>

void parser_test() {
    const char request[] = "GET / HTTP/1.1\r\nHost: localhost:3000\r\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:145.0) Gecko/20100101 Firefox/145.0\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\nAccept-Language: en-US,en;q=0.5\r\nAccept-Encoding: gzip, deflate, br, zstd\r\nConnection: keep-alive\r\nCookie: username-localhost-8888=2|1:0|10:1764580039|23:username-localhost-8888|196:eyJ1c2VybmFtZSI6ICI4YzYwMzFhYjUzMDQ0ZmU3YTNjMTNiNzE1MGJiY2FjYiIsICJuYW1lIjogIkFub255bW91cyBFcmlub21lIiwgImRpc3BsYXlfbmFtZSI6ICJBbm9ueW1vdXMgRXJpbm9tZSIsICJpbml0aWFscyI6ICJBRSIsICJjb2xvciI6IG51bGx9|46780c36517af2ea09a96512843c59fbdcfd4e21238e5d6c959d3254dd579284; _xsrf=2|2cad1e70|164acc50b0892cfe8408ba83d24d4171|1764584311\r\nUpgrade-Insecure-Requests: 1\r\nSec-Fetch-Dest: document\r\nSec-Fetch-Mode: navigate\r\nSec-Fetch-Site: none\r\nSec-Fetch-User: ?1\r\nPriority: u=0, i\r\n\r\n";
    struct http_request result = parse_http(request, sizeof(request));
    printf("%s", result.host);
    
}

int main() {
    parser_test();
}