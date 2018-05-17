/* Name:Arko Chatterjee
   ID:1610110075
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include<stdlib.h> 
#include<string.h>
#include <pthread.h>

/* structure for passing data to threads */
typedef struct
{
int start;
int end;
int threadNo;                              //for identifying the thread currently running
} parameters;


int* sortArray;                             //global array
int* end;
pthread_mutex_t lock;                       //mutex lock

int getArray(char* fname){

char* line;
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
sortArray=(int*)malloc(strlen(line)*sizeof(int));
//int* t;
end=sortArray;

char* temp = strtok(line, ",");
while (temp!= NULL) {
 *end= atoi(temp);               //convert string to integer
 temp= strtok(NULL, " ,");
 end++;
 size++;
}
return size;
}


int partition (int arr[], int low, int high){                            //partition function for quicksort
   
 int pivot = arr[high]; 
 int i = (low - 1); 							 // Index of smaller element
 for (int j = low; j <= high- 1; j++)
   {
        if (arr[j]<=pivot)
        {
            i++;    
            int temp=arr[i]; 
            arr[i]=arr[j];
            arr[j]=temp;
        }
    }
 int t=arr[i+1]; 
 arr[i+1]=arr[high];
 arr[high]=t;
 return (i + 1);
}
 
void quickSort(int arr[], int low, int high)
{
    if (low < high)
    {
        int pindex = partition(arr, low, high);
        quickSort(arr, low, pindex - 1);
        quickSort(arr, pindex + 1, high);
    }
}


void *runner(parameters *param){
 
 int i;
 pthread_mutex_lock(&lock);
 printf("\nThread %d contains:\n",param->threadNo);
 for(i=param->start;i<param->end;i++){
 printf("%d ",sortArray[i]);
 }
 printf("\n");
 pthread_mutex_unlock(&lock);

 quickSort(sortArray,param->start,param->end-1);

 pthread_mutex_lock(&lock);
 printf("\nSorted array for thread: %d\n",param->threadNo);
 for(i=param->start;i<param->end;i++){
 printf("%d ",sortArray[i]);
 }
 printf("\n\n");
 pthread_mutex_unlock(&lock);

 //pthread_exit(0);
}

void *merge(parameters *param){

int* temp=(int*)malloc(sizeof(sortArray));
int *head=temp;
int *ptr1,*ptr2;
ptr1=&sortArray[param->start];
ptr2=&sortArray[param->end];    //end here is the start of second partition 


while(*ptr1!=sortArray[param->end] && ptr2<=end){

 if(*ptr1<*ptr2){
	
        *temp=*ptr1;
	ptr1++;    
	temp++;

 }
 else{

 	*temp=*ptr2;
	ptr2++;
	temp++;
 }

 if(*ptr1==sortArray[param->end]){
	while(ptr2!=end){
		*temp=*ptr2;
		temp++;
		ptr2++;
	}
 }
 
 else if(ptr2==end){
	while(*ptr1!=sortArray[param->end]){
		*temp=*ptr1;
		temp++;
		ptr1++;
	}
 }
}

sortArray=head;
//pthread_exit(0);

}


void main(int argc,char** args){            //name of file containing the array is passes as parameter to main

if(argc<2){
 printf("Incorrect format\n");
 printf("Syntax:  ./sorting <filename> \n");
 exit(0);
}
pthread_mutex_init(&lock, NULL);            //initialize mutex lock
int size=getArray(args[1]);
printf("\nThe size of array is %d\n",size);
printf("The array elements are:\n");
int i;
for(i=0;i<size;i++){
printf("%d ",sortArray[i]);
}
printf("\n");


pthread_t tid1,tid2,mergeThread;
pthread_attr_t attr1,attr2,mattr;

pthread_attr_init(&attr1);
pthread_attr_init(&attr2);
pthread_attr_init(&mattr);


parameters *param1=(parameters*)malloc(sizeof(parameters));
parameters *param2=(parameters*)malloc(sizeof(parameters));

param1->start=0;
param1->end=size/2;
param1->threadNo=1;
param2->start=size/2;
param2->end=size;
param2->threadNo=2;

pthread_create(&tid1,&attr1,runner,param1);
pthread_create(&tid2,&attr2,runner,param2);

pthread_join(tid1,NULL);
pthread_join(tid2,NULL);


parameters *mergeParam=(parameters*)malloc(sizeof(parameters));
mergeParam->start=0;
mergeParam->end=size/2;                                                //end is start of next thread

pthread_create(&mergeThread,&mattr,merge,mergeParam);
pthread_join(mergeThread,NULL);

int n=0;
printf("The sorted array elements are:\n");
for(i=0;i<size;i++){
n++;
printf("%d ",sortArray[i]);
}
printf("\n");

}
