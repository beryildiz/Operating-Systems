#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#define QUEUESIZE 10

typedef struct {
	char buf[QUEUESIZE][256];
	long head, tail;
	int full, empty, finished, count;
	pthread_mutex_t *mut;
	pthread_cond_t *notFull, *notEmpty;
} queue;

queue *queueInit (void);
void queueDelete (queue *q);
void queueAdd (queue *q, char* in);
//void queueDel (queue *q, char* out);
char* queueDel (queue *q);


