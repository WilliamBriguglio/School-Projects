#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

int main (int argc, char *argv[]){

pid_t pid;
int fp, nr, x = 8, ctrl = 1, col = 0;
int pbs = 0; //preceeded by space?
char buf[1];
char command[50];
strcpy( command, "kill -9        "); // leaves space to be overwritten by PPID

fp = open("zproc.txt", O_RDWR | O_TRUNC);
if( fp <= 0){	//file does not alread exist
	fp = open("zproc.txt", O_CREAT | O_EXCL | O_RDWR, 00777);
}

if( (pid = fork()) ){ // parent
	sleep(1); // make sure zombie is created first
	system("ps -l"); // list processes
	system("ps -l | grep ' Z ' > zproc.txt" ); //save lines from output of ps that conatin ' Z ' in a file called zproc.txt
	while(ctrl == 1){
		nr = read(fp, buf , 1);
		if( (buf[0] == ' ') && (pbs == 0) ){
			col++;
			pbs = 1;
		}
		if( buf[0] != ' ' ){
			pbs = 0; // this variable is to check if there are mutiple spaces in a row, so that we do not increment col when it shouldn't 
		}
		if( (col == 4) && (buf[0] != ' ') ){
			command[x] = buf[0]; //start overwriting the blank space with the PPID so "kill -9" will have its second argument
			x++;
		}
		if( col == 5){
			system(command); //since in next column, the full PPID has been scanned in and command can be executed
			col++; // since we are no longer worried about the column number, we can increment  it so the command isnt called more than once
		}
		if( buf[0] == '\n' ){
			col = 0; //newline means there is another zombie process on the next line
		}
		if( nr < 1){
			ctrl = 0; //number of read bytes equals zero means that we have reached EOF
		}
	}
	close(fp);
	waitpid(pid, 0, 0); // be sure child is done
	system("ps -l"); // print off new process list
}else{ // child
	execl("./prog1", "prog1", NULL);
}




return 0;
}
