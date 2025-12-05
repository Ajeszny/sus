#include <stdio.h>
#include "server.h"

int main() {
    init_server(3000);
    printf("Running on port 3000");
    start_listening();
    stop_server();
    return 0;
}
