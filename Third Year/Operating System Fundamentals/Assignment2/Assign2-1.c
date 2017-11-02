#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main (int argc, char *argv[]){


int status;
pid_t pid;

if( (pid = fork()) ){ // parent
	sleep(20);	//child becomes zombie during this sleep call
	wait(&status);
}else{ // child
	exit(20);
}

return 0;
}

