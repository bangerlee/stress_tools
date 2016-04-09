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
#define LOOP 1000

pthread_t tid[32];
int opt_cpu_load_slice = 64;
int load = 50;

// g++ -pthread stresscpuratio.cpp -o stresscpuratio 

static inline double timeval_to_double(const struct timeval *tv)
{
        return (double)tv->tv_sec + ((double)tv->tv_usec / 1000000.0);
}

double time_now(void)
{                               
        struct timeval now;

        gettimeofday(&now, NULL);
        return timeval_to_double(&now);
}

void cpu_sqrt()
{
        int i = 0;
        uint32_t z = MWC_SEED_Z;
        uint32_t w = MWC_SEED_W;

        for (; i < 16384; i++) {
	    z = 36969 * (z & 65535) + (z >> 16);
	    w = 18000 * (w & 65535) + (w >> 16);
            uint64_t rnd = z << 16 + w; 
	    double r = sqrt((double)rnd) * sqrt((double)rnd);
        }
}

void * thread1(void *arg)
{
       double bias = 0.0;
       int i = 0;
       for(; i < LOOP; i++){
           double t, delay;
           double t1, t2, t3;
           struct timeval tv;         

           t1 = time_now();
           int j = 0;
           for(; j < opt_cpu_load_slice; j++){
               cpu_sqrt();
           }
           t2 = time_now();
           t = t2 - t1;
           delay = t * (((100.0 / (double) load)) - 1.0);
           delay -= bias;

           tv.tv_sec = delay;
           tv.tv_usec = (delay - tv.tv_sec) * 1000000.0;
           select(0, NULL, NULL, NULL, &tv);

           t3 = time_now();
           bias = (t3 - t2) - delay; 
       }
       return NULL; 
}

int main(int argc, const char *argv[])
{

	if(argc < 1) {
		return -1;
	}

	int cnt = atoi(argv[1]);
        load = atoi(argv[2]);

	for(int i = 0; i < cnt; i++) {
		pthread_create ( &(tid[i]), NULL, thread1, NULL ) ;
	}

	for(int i = 0; i < cnt; i++) {
		pthread_join(tid[i], NULL);
	}
	return 0;
}
