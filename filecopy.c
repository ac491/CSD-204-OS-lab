/* Name:Arko Chatterjee
   ID:1610110075
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include<stdlib.h>
#include <fcntl.h>                                               // for file mode values
#include<string.h>
#define BUFFERSIZE 100

long file_size;                                                 // the size of source file is copied to both parent and child processes

void getSize(FILE* source,char* fname){                         //get the size of source file

source=fopen(fname,"r");    
fseek(source, 0, SEEK_END);
file_size = ftell(source);
fseek(source, 0, SEEK_SET); 
close(source);
}



int main(int argc,char** args){

if(argc<2){
printf("INCORRECT SYNTAX!");
}


FILE *source, *destination;
char readBuffer[BUFFERSIZE];                               //shared buffer to write to and read from pipe


if((source=open(args[1],O_RDONLY))==-1){                   //check if source file exists
 
 printf("Source Does not exist!\n");
 exit(0);
}

getSize(source,args[1]);                                   //get the size of source file



if(strchr(args[2],'.')==NULL){
  
  if(strcmp(strchr(args[1],'.'),".txt")==0){
  
 	strcat(args[2],"/untitled.txt");
  }
  else  if(strcmp(strchr(args[1],'.'),".pdf")==0){
  
 	strcat(args[2],"/untitled.pdf");
  }
  else  if(strcmp(strchr(args[1],'.'),".jpg")==0){
  
 	strcat(args[2],"/untitled.jpg");
  }

}
else if((destination=open(args[2],O_RDONLY))==-1){              //check if file already exists.

 printf("Creating new file: %s\n",args[2]);

}
else{
 char ch;
 printf("\nThe file already exists. Do you wish to override the file?(Y/N)\n");
 scanf("%c",&ch);
 if(ch=='y'|| ch=='Y'){
    //do nothing
 }
 else{
    exit(0);                                               //exit if user does not want to override
 }
}
close(destination);

int fd[2];
pid_t cpid;
pipe(fd);                                                  /* create pipe */

if((cpid = fork()) == -1){
  perror("fork");
  exit(1);
}
 
if(cpid == 0){                                                            //child process                                                        

	close(fd[1]);                                                                                                                    
	destination= open(args[2], O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);                                   

        while(read(fd[0],readBuffer,sizeof(readBuffer))>0 && file_size>BUFFERSIZE){
             
            write(destination,readBuffer,sizeof(readBuffer));
            file_size-=BUFFERSIZE;
       
        }
        write(destination,readBuffer,file_size);
        close(fd[0]); 
	printf("The file is copied to %s\n",args[2]); 
                     
	close(destination);
	

}else{                                                                    //parent process
        
        /* parent only writes. Close the read end */
	close(fd[0]);

	source=open(args[1],O_RDONLY);   

	while(read(source,readBuffer,sizeof(readBuffer))>0 && file_size>BUFFERSIZE){
             
            write(fd[1],readBuffer,sizeof(readBuffer));
            file_size-=BUFFERSIZE;
       
        }
        write(fd[1],readBuffer,file_size);
	close(fd[1]);
	close(source);                                                      
}

return 0;
}
