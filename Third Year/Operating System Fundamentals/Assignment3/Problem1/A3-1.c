#include <pthread.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int circle_count;
int total_points = 10000000;

double random_double();
void * start_routine();

int main( int argc, char *argv[]){
	double est_pi;

	pthread_t tid;			//the thread identifier
	pthread_attr_t attr; 		//set of thread attributes

	pthread_attr_init(&attr);	//get the default attributes
	pthread_create(&tid, &attr, start_routine, NULL);//create thread
	pthread_join(tid, NULL);	//waot for thread

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
	circle_count = hit_count;

return;
}


double random_double(){
	return (random() / ((double)RAND_MAX +1));
}

