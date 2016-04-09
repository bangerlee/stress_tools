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

#define MWC_SEED_W          (521288629UL)
#define MWC_SEED_Z          (362436069UL)
#define LOOP 10000000

pthread_t tid[32];

// g++ -pthread stresscpu.cpp -o stresscpu

void * thread1(void *arg)
{
        int i = 0;
        uint32_t z = MWC_SEED_Z;
        uint32_t w = MWC_SEED_W;

        for (; i < LOOP; i++) {
	    z = 36969 * (z & 65535) + (z >> 16);
	    w = 18000 * (w & 65535) + (w >> 16);
            uint64_t rnd = z << 16 + w; 
	    double r = sqrt((double)rnd) * sqrt((double)rnd);
        }
	return NULL;
}

int main(int argc, const char *argv[])
{

	if(argc < 1) {
		return -1;
	}

	int cnt = atoi(argv[1]);

	for(int i = 0; i < cnt; i++) {
		pthread_create ( &(tid[i]), NULL, thread1, NULL ) ;
	}

	for(int i = 0; i < cnt; i++) {
		pthread_join(tid[i], NULL);
	}
	return 0;
}


