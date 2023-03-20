/*
* Wunderschoene implementierung von Threads
* von Berkan Yildiz, Johannes Belaschow und Weewick
*/

#include <malloc.h>
#include <stdlib.h> 
#include <string.h>
#include <sys/time.h>

#include "client_reader.h"


#define USE_PROXY 0

/* 
 * Main
 */
int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr,
                "Usage: %s <inputfile> [options]\n\n"
                "Options:\n"
                "\t--webreq-delay <uint>\n"
                "\t--webreq-delay-seed <int>\n"
                "\t--webreq-path <path>\n"
                "\t--thread-count <uint>\n",
                argv[0]);
        return EXIT_FAILURE;
    }
    
    // generate time stamp
    struct timeval  tv;

    gettimeofday(&tv, NULL);
    double start = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;

    //-------------------------------------------------------------
    
    reader_args* r_args = malloc(sizeof(reader_args*));

    int thread_num = 1;
    if (argc > 2) {
        for (int i = 2; i < argc; i++) {
            if ( (strcmp(argv[i],"--thread-count") == 0) && (i < argc-1) ) {
                thread_num = atoi(argv[i+1]);  // atoi = string to integer -> wir lesen das darauf folgende Argument nach dem thread-count Flag 
                                                //und schreiben die gelesene Zahl auf die thread_num variable
            }
        }
    }

    client_args* c_args[thread_num];

    queue* fifo = queueInit ();
    if (fifo == NULL) {
		fprintf (stderr, "main: Queue Init failed.\n");
		exit (EXIT_FAILURE);
	}

	pthread_t readerT;
    pthread_t clientT[thread_num];

	r_args->file = argv[1];
    r_args->fifo = fifo; // fifo argument zeigt auf fifo welche wir in der main erstellt haben

    pthread_create (&readerT, NULL, reader, r_args); 

    webreq_init(argc, argv);

    for (int i=0; i<thread_num; i++) {
        c_args[i] = malloc(sizeof(client_args*));
        c_args[i]->fifo = fifo; // fifo argument zeigt auf fifo welche wir in der main erstellt haben
        c_args[i]->id = i;      // id fuer die einzelnen threads wird uebergeben
        pthread_create (&clientT[i], NULL, client, c_args[i]);
    }

    pthread_join (readerT, NULL);   

    for (int i=0; i<thread_num; i++) {
        pthread_join (clientT[i], NULL); // main thread wartet bis die client- und reader-threads fertig sind
    }

    webreq_cleanup();   // speicher vom webrequest aufraeumen

	queueDelete (fifo); // speicher der fifo aufraeumen

    free(r_args); // alloziierter speicherplatz fuer argumente wird freigegeben
    for (int i=0; i<thread_num; i++) {
        free(c_args[i]); // alloziierter speicherplatz fuer argumente wird freigegeben
    }

    // measure runtime
    gettimeofday(&tv, NULL);
    double end = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
    double diff = (end - start) / 1000;

    printf("Duration: %5.2f s\n", diff);

    return EXIT_SUCCESS;
}

