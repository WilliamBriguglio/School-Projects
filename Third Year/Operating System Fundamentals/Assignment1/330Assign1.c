#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[]){

int ip, op, bytesread, byteswritten;
char ipfn[50];                          //input file name
char opfn[50];                          //output file name
char buffer[10];

printf("Enter source file name\n");
scanf("%s", ipfn);                          //get source file name

printf("Enter destination file name\n");
scanf("%s", opfn);                          //get destination file name

ip = open(ipfn, O_RDONLY);
if(ip <= 0){                                //if open returns non positive int then error, assume file does not exist
    printf("FILE DOES NOT EXIST!\n");
    abort();
}

op = open(opfn, O_CREAT | O_EXCL | O_WRONLY , 00777 );
if(op <= 0){                                //if open returns non positive int then error, assume file already exists
    printf("FILE ALREADY EXISTS!\n");
    abort();
}

while(1){
    bytesread = read(ip, buffer, sizeof(buffer));
    if( bytesread < 1){                             //read from source and copy to destination in a infinite loop
        break;                                      //break when read stops scanning any bytes, EOF
    }
    write(op, buffer, bytesread);
}

close(op);
close(ip);

printf("Write from %s to %s has been completed", ipfn, opfn);

return 0;
}
