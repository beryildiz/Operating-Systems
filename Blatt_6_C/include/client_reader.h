#include "queue.h"
#include "web_request.h"

typedef struct {
    queue* fifo;
    int id;
} client_args;

typedef struct {
    queue* fifo;
    char* file;
} reader_args;

void *reader (void *args);
void *client (void *args);


