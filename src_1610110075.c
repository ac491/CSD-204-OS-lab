/* Name:Arko Chatterjee
   ID:1610110075
*/


#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include<stdlib.h> 
#include<string.h>
#include "pagealgos.h"

int* a;                            		 			//global array



int minPriority(int priority[], int n){
    int i, minimum = priority[0], pos = 0;				//find the page with minimum priority
 
    for(i = 1; i < n; ++i){
        if(priority[i] < minimum){
            minimum = priority[i];
            pos = i;
        }
    }
    
    return pos;
}


int LRU(int *pageString,int size,int frameSize){
		
int frames[frameSize],priority[frameSize],i,j,pos,faults = 0;
int flag1=0, flag2=0;								//flag1 is when frame is not empty and flag2 for replacements
int counter = 0;								//variable to give max priority to recently inserted element

for(i = 0; i < frameSize; i++){
        frames[i] = -1;
    }
    
    for(i=0;i<size;i++){
        
	flag1 = flag2 = 0;
        
        for(j=0;j<frameSize;j++){
            if(frames[j] == pageString[i]){
                counter++;
                priority[j]=counter;
                   flag1=flag2=1;
                   break;
               }
        }
        
        if(flag1==0){
            for(j=0;j<frameSize;j++){
                if(frames[j]==-1){
                    counter++;
                    faults++;
                    frames[j]=pageString[i];
                    priority[j]=counter;
                    flag2=1;
                    break;
                }
            }    
        }
        
        if(flag2==0){
            pos=minPriority(priority,frameSize);
            counter++;
            faults++;
            frames[pos]=pageString[i];
            priority[pos]=counter;
        }
        
    }
    
    return faults;
}

int FIFO(int *pageString,int size,int frameSize){
	
	int i,j,k=0,hit=0,fault=0;
	int frame[frameSize];
	
	for(i=0;i<frameSize;i++)
		frame[i]=-1;

	for(i=0;i<size;i++){

		hit=0;
		for(j=0;j<frameSize;j++)
			if(frame[j]==pageString[i])
				hit=1;								//check if element is present in frame

		if(hit==0){

			frame[k]=pageString[i];
			k=(k+1)%frameSize;
			fault++;

		}
		
	
	}
	
	return fault;
}

int OPT(int *pageString,int size,int frameSize){

    int frames[frameSize];
    int temp[frameSize];
    int flag1,flag2,flag3,i,j,k,pos,max,faults = 0;

    for(i=0;i<frameSize;i++){
        frames[i] = -1;
    }
    
    for(i=0;i<size;i++){
        
       		flag1=flag2=0;
        
       		for(j=0;j<frameSize;j++){
            		if(frames[j] == pageString[i]){
                   		flag1=flag2=1;					//if page hit
                   		break;
               		}
        	}
        
        	if(flag1==0){    						//if some frames are empty
            		for(j=0;j<frameSize;j++){
                		if(frames[j]==-1){
                    			faults++;
                    			frames[j]=pageString[i];
                    			flag2=1;
                    			break;
                		}
            		}    
        	}
        
        	if(flag2==0){							//to replace pages in frame

            		flag3=0;						//to check if some element does not occur again
            
            		for(j=0;j<frameSize;j++){
                		temp[j] = -1;
                
                		for(k=i+1;k<size;k++){
                    			if(frames[j]==pageString[k]){
                        			temp[j]=k;
                        			break;
                    			}
                		}
            		}
            
            		for(j=0;j<frameSize;j++){
                		if(temp[j]==-1){
                    			pos=j;
                    			flag3=1;
                    			break;
                		}
            		}
            
            		if(flag3==0){						//if flag3==0 then we search for the page which will be required last
                		max=temp[0];
                		pos=0;
                
                		for(j=1;j<frameSize;j++){
                    			if(temp[j]>max){
                        			max=temp[j];
                        			pos=j;
                    			}
                		}                
            		}
            
            		frames[pos]=pageString[i];
            		faults++;
        	}
        
    }
    
  
    
    return faults;


}

int getArray(char* fname){			//function to parse the array elements

char* line;
int* end; 
FILE* source;
source=fopen(fname,"r");    
fseek(source, 0, SEEK_END);
long file_size = ftell(source);
fseek(source, 0, SEEK_SET); 
line=(char*)malloc(file_size);
fread(line,file_size,1,source);
fclose(source);

int i;
int size=0;
a=(int*)malloc(strlen(line)*sizeof(int));
end=a;

char* temp = strtok(line, ",");
while (temp!= NULL) {
 *end= atoi(temp);               //convert string to integer
 temp= strtok(NULL, " ,");
 end++;
 size++;
}
return size;
}




void main(int argc,char** args){            //name of file containing the array is passes as parameter to main

	if(argc<2){
 		printf("Incorrect format\n");
 		printf("Syntax:  ./pagerep <filename> \n");
 		exit(0);
	}

	int size=getArray(args[1]);
	
	int faults[4];
	int s[]={1,4,6,10};
	int i;
	
	printf("\nFAULTS TABLE:\n");
	printf("\tAlgorithm\tsize:\t1\t4\t6\t10\t\n\n");
	printf("\tFIFO\t\t\t");
	for(i=0;i<4;i++){
		faults[i]=FIFO(a,size,s[i]);
		printf("%d\t",faults[i]);
	}
	printf("\n");
	printf("\tLRU\t\t\t");
	for(i=0;i<4;i++){
		faults[i]=LRU(a,size,s[i]);
		printf("%d\t",faults[i]);
	}
	printf("\n");
	printf("\tOPT\t\t\t");
	for(i=0;i<4;i++){
		faults[i]=OPT(a,size,s[i]);
		printf("%d\t",faults[i]);
	}
	printf("\n");


}



