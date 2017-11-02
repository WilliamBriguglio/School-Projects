#include <pthread.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int circle_count;
int total_points = 10000000;

#define NUM_THREADS 4
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

double random_double();
void * start_routine();

int main(void){
	double est_pi;

	pthread_t workers[NUM_THREADS];		//array of threads
	pthread_attr_t attr; 		//set of thread attributes
	pthread_attr_init(&attr);	//get the default attributes


	for(int i = 0; i < NUM_THREADS; i++){
		pthread_create(&workers[i], &attr, start_routine, NULL);//create thread
	}
	for(int i = 0; i < NUM_THREADS; i++){
		pthread_join(workers[i], NULL);	//wait for threads
	}

	total_points = total_points * NUM_THREADS;
	est_pi = (4 * ((double)circle_count/(double)total_points));//estimate pi
	printf("%f\n", est_pi);

return 0;
}

void * start_routine(){

	int hit_count = 0;
	double x, y;

	srandom((unsigned)time(NULL)); //seed random number generator

	//obtain a  random (x,y) point  between (-1,-1) and (1,1) and test for hit
	for(int i = 0; i < total_points; i++){
		x = random_double() * 2.0 -1.0;
		y = random_double() * 2.0 -1.0;
		if( sqrt(x*x + y*y) < 1.0){
			++hit_count;
		}
	}

	pthread_mutex_lock(&mutex); //acquire lock
	circle_count += hit_count;
	pthread_mutex_unlock(&mutex); //release lock

return;
}

double random_double(){
	return (random() / ((double)RAND_MAX +1));
}

