/* Name:ARKO CHATTERJEE
   ID:1610110075
*/

#include <stdio.h>
#include <unistd.h>
#include<stdlib.h>
#include<string.h>
#define MAX_LINE 80 /* The maximum length command */
#define MAX_HISTORY 100
#define MAX_COMMANDS 100

char history[MAX_HISTORY+MAX_COMMANDS][MAX_LINE];
int history_size=0,ind=0;
int newIndex=0;                                                            //new history commands

int parse(char line[],int size,char** args){
	
int i,j=0,k,count=0;
char c;

 for(i=0;i<size;i++){

  if((c=line[i])==' ' || (c=line[i])=='\t' ||(c=line[i])=='\n' || *(line+i+1)=='\0')
  {                                                                                         // copy from j to i+1 into args[0,1,2...]
      if(*(line+i+1)=='\0')
      i=i+1;                                                                                // to read the last character of line

      args[count]= (char*) malloc((i-j+1)*sizeof(char));
      int x=0;
      for(k=j;k<i;k++){
        *(args[count]+x)=*(line+k);
        x++;
      }

     *(args[count]+x+1)='\0';
     count++;
     j=i+1;
  }

  else{
     continue;
  }

 }
args[count]=NULL;

return count;                                                    // return number of commands

}


void writeHistory(char* line){

   FILE *fptr=fopen("history.txt","a+");
   fprintf(fptr,"%s\n",line);
   //fclose(fptr);

}


void initHistory(){                                              //reads history.txt file and store contents to history array

char line[MAX_LINE];
FILE *file;
int n=0;
             					               
file = fopen("history.txt", "r");

while(fgets(line,sizeof(line),file)){                            // get the size of history
history_size++;
}

ind=0;

if(history_size>MAX_HISTORY){
   n=history_size-MAX_HISTORY;                                  //get the starting position to read 100 commands
   ind=n;                                                       //ind shows how much greater history_size is from 100
}

fseek(file,0,SEEK_SET);                                        //seek to start of file

int i=0;
while(fgets(history[i],sizeof(history[i]),file)){
   if(n>0){
          n--;                                                //discard starting n commands
   }
   else{
          i++;                                                                 
   }
}

history_size=i;
//newIndex=history_size;                                        //set newIndex to end
fclose(file);
}

void readHistory(){

int num=10,i;
i=history_size+newIndex;
i--;
  while(num>0 && i>=0){
      printf("%d. %s",i+1+ind,history[i]);                    //ind (index) is used when the history_size is greater than MAX_HISTORY
      i--;
      num--;
  }

}



int main(void){

char *args[MAX_LINE/2 + 1]; /* command line arguments */
int shouldRun = 1; /* flag to determine when to exit program */
char line[MAX_LINE];
initHistory();                                                                 //store the records into history array
printf("\n\n***********************************************************\n\n");
printf("\t\tWELCOME TO OSH SHELL:\n\n");
printf("***********************************************************\n\n");

while (shouldRun) {

printf("osh>");
gets(line);

if(strcmp(line,"history")==0){

    readHistory();
    continue;

}
//printf("%s",line);
if(strlen(line)>MAX_LINE){
  printf("Command size cannot be greater than %s",MAX_LINE);
  continue;
}
//writeHistory(line);
if((*line)!='!' && strcmp(line,"history")!=0 && strcmp(line,"quit")!=0){
   int i=0,x=0;
   writeHistory(line);
   for(i=0;i<strlen(line);i++){                                     //to store the new commands into buffer array 
   	history[newIndex+history_size][x]=line[i];
        x++;
   }
   history[newIndex+history_size][x]='\n';
   history[newIndex+history_size][x+1]='\0';
   newIndex++;	
   
}


int n;
n=parse(line,strlen(line),args);                                     //returns the number of arguments in args

if(strcmp(line,"quit")==0){
  
    shouldRun=0;
    continue;
}else{
 
if(line[0]=='!'){
      
     if(line[1]=='!'){                                                //for !! case
        char *temp;
	temp=(char*)malloc(sizeof(history[history_size+newIndex-1]));
        temp=history[history_size+newIndex-1];
        int x=strlen(temp);
        temp[x-1]='\0';
        // printf("%s",temp);
	int n=parse(temp,strlen(temp),args);
        //printf("%s",args[0]);
     }else{								//for !num case
       	char*temp;
	int i;
	temp=(char*)malloc(sizeof(line));
	int size=strlen(line);
	for(i=1;i<size;i++){
		temp[i-1]=line[i];
	}
	temp[i-1]='\0';
	//printf("%s\n",temp);
        int num=atoi(temp);
	if(num-ind<0){							
		printf("Invalid index! History currently stores only 100 records.\n");
		continue;
	}else if(num>(MAX_HISTORY+newIndex+ind+1)){
		printf("Invalid index! History not present.\n");
		continue;	
	}
	temp=(char*)malloc(sizeof(history[num-ind-1]));               //ind is subtracted to get correct array element
	temp=history[num-ind-1];
        int x=strlen(temp);
        temp[x-1]='\0';
	int n=parse(temp,strlen(temp),args);
	 
     }
}

 pid_t pid;
 pid= fork();                                                         //create a child process.
 
 if(pid<0){
   printf("Error! new process not created.\n");   

 }
 else if(pid==0){                                                     //child process

  printf("\n");
 

     int len=strlen(args[n-1]);
     if(*(args[n-1]+len-1)=='&')  
     	args[n-1]=NULL; 

     int x=execvp(args[0],args);
	printf("\n");
	if(x=-1){
	   printf("INVALID COMMAND!\n");
        }
     
  
 
 exit(1);
 }
 
 else{                                                            //parent process
   int len=strlen(args[n-1]);	
   if(*(args[n-1]+len-1)!='&')                                    
     wait(NULL);                                                  //if the command does not include & then wait for child process to be completed.
   else{

	continue;  
   }                                              
  }

}
fflush(stdout);
/**
* After reading user input, the steps are:
* (1) fork a child process using fork()* (2) the child process will invoke execvp()
* (3) if command included &, parent will not invoke wait()
* (4) if command is quit, the shell should exit
* Explain your steps as comments in the code itself.
*/
}
return 0;
}
