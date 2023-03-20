#include <stdio.h> 
#include <sys/file.h>
#include <stdio.h>
#include <unistd.h> 

#include "client_reader.h"


void *reader (void *args)
{
    reader_args *arguments = (reader_args *)args; // uebergebener Parameter in reader_args zurueckcasten


    char url[256];
    int i;
    int pos = 0;

    // open file containing the pages to GET
    int fd_in;
    if (!(fd_in = open(arguments->file, O_RDONLY))) { // Ready-Only flag nutzen!
        perror("failed to open input file");
        exit(EXIT_FAILURE);
    }

	while (pread(fd_in, url, 256, pos) > 0) { // fgets ist laut doku nicht threadsafe, deshalb pread benutzt  
        if((url[0]=='\n') || (url[0]=='\r')) {
            pos++;
            continue;
        }
        i = strcspn(url, "\n"); // Index vom Zeilenumbruch des Chararrays wird gesucht und auf i geschrieben
        pos += i+1;
        url[i] = '\0';  // setze den gefundenen Zeilenumbruch auf ein \0 -> String-Ende setzen

        pthread_mutex_lock (arguments->fifo->mut);

		// while -> spinlock
		if (arguments->fifo->full == 1) {
			printf ("reader: queue FULL.\n");        
			pthread_cond_wait (arguments->fifo->notFull, arguments->fifo->mut);
		}
        queueAdd (arguments->fifo, url);
        printf("reader: added %s to queue.\n", url); 
		pthread_mutex_unlock (arguments->fifo->mut);
		pthread_cond_signal (arguments->fifo->notEmpty);
        //usleep(100000); // Sleep wurde beim debuggen benoetigt da Raceconditions auftraten
	}

    // close file
    close(fd_in);

    printf ("reader exited.\n");
    arguments->fifo->finished = 1;

	return (NULL);
}

void *client (void *args) {
    client_args *arguments = (client_args *)args;

    char url[256];
    char tmp[256];
    char* domain;
    char* rest;
    int count;

	while(1) {
		pthread_mutex_lock (arguments->fifo->mut);

        // while -> spinlock
		if (arguments->fifo->empty == 1) {
			printf ("client_%d: queue EMPTY.\n", arguments->id+1);
            if (arguments->fifo->finished == 1) {
                printf ("client_%d exited.\n", arguments->id+1);
                pthread_mutex_unlock (arguments->fifo->mut);
                return (NULL);
            }
			pthread_cond_wait (arguments->fifo->notEmpty, arguments->fifo->mut);
		}

        // Wert von QueueDelete auf die URL geschrieben. 
        // Fuer IO, strcopy ist noetig, zerstoerendes lesen!

        strcpy(url, queueDel (arguments->fifo));
        count = arguments->fifo->count;
        printf("client_%d: read %s from queue.\n", arguments->id+1, url);
		pthread_mutex_unlock (arguments->fifo->mut);
		pthread_cond_signal (arguments->fifo->notFull);

        strcpy(tmp, url);
        rest = tmp;
        strtok_r(tmp, "/", &rest);
        domain = strtok_r(NULL, "/", &rest); // domain aus der url entnehmen

        if (domain == NULL) {
            fprintf(stderr, "[ERROR] domain could not be extracted from URL [%s], skipping\n", url);
        } else {
            char filename[64];
            snprintf(filename, sizeof (filename), "%d_%d_%s.html", arguments->id+1, count, domain);

            printf("[START] Downloading URL: %s ->> File: %s\n", url, filename);

            int res;
#if USE_PROXY
            res = webreq_download_via_proxy(url, filename);
#else
            res = webreq_download(url, filename);
#endif
            if (res < 0)
                fprintf(stderr, "[ERROR] URL: %s, Message: %s\n", url, webreq_error(res));
            else if (res != WEBREQ_HTTP_OK)
                fprintf(stderr, "[ERROR] HTTP Status %d returned for URL: %s\n", res, url);
            else
                printf("[DONE ] URL: %s ->> File: %s\n", url, filename);
        }

        //usleep(5000); // Sleep wurde beim debuggen benoetigt da Raceconditions auftraten
	}
	return (NULL);
}

