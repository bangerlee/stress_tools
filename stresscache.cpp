#include <sys/syscall.h>  
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <sched.h>
#include <math.h>

#define LOOP 10000
#define MATSIZE 256

// g++ -pthread stresscache.cpp -o stresscache

pthread_t tid[32];
int mat[MATSIZE][MATSIZE];

void * thread1(void *arg)
{
        int k = 0;
        for (; k < LOOP; k++) {
            for ( int i = 0; i < MATSIZE; i++ ) {
                for ( int j = 0; j < i; j++ ) {
                    int aux = mat[i][j];
                    mat[i][j] = mat[j][i];
                    mat[j][i] = aux;
                }
            } 
        }
	return NULL;
}

int main(int argc, const char *argv[])
{

	if(argc < 1) {
	    return -1;
	}

	int cnt = atoi(argv[1]);

        for ( int i = 0; i < MATSIZE; i++ ) {
            for ( int j = 0; j < MATSIZE; j++ ) {
                mat[i][j] = i+j;
            } 
        }  

	for(int i = 0; i < cnt; i++) {
	    pthread_create ( &(tid[i]), NULL, thread1, NULL ) ;
	}

	for(int i = 0; i < cnt; i++) {
	    pthread_join(tid[i], NULL);
	}
	return 0;
}


