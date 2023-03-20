#include <stdio.h>
#include <string.h>

#include "queue.h"

int main(){

    /* Fragen ob die delete Methode irgendwas zurueckgeben darf oder ist das nicht Threadsicher? */
    char url[256];
    
    // Queue erzeugen
    queue* fifo = queueInit();

    FILE* fd_in;
    if (!(fd_in = fopen("testSites_10_2020.txt", "r"))) { // r bedeuted read modus
        perror("failed to open input file");
        exit(EXIT_FAILURE);
    }

    // Queue mit Elementen fuettern
    while (fgets(url, sizeof (url), fd_in)) {
		if (strtok(url, "\r\n") == NULL)
            continue;

		// while -> spinlock
		if (fifo->full) {
			printf ("reader: queue FULL.\n");       
            break;
		}
		queueAdd (fifo, strdup(url));
	}

    fclose(fd_in);

    // Elemente ausgeben
    char* del;

    while (fifo->empty != 1) {
        del = queueDel(fifo);
        printf("deleted: %s\n", del);
    }

    // Elemente nach dem Loeschen wieder ausgeben
    for (int i = fifo->head; i < fifo->tail; i++) {
        printf("%s, ", fifo->buf[i]);
    }
    printf("\n");

    return 0;
}