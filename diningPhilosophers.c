/* Name:Arko Chatterjee
   ID:1610110075
*/
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h> 
#include <string.h>
#include <pthread.h>
#include <time.h>

int n_phils;     								//number of philosophers

enum { 
	THINKING, 
	HUNGRY, 
	EATING 
}state[100];


pthread_mutex_t mutex;
pthread_cond_t cond_var;
double exec_time=0;								//execution time of complete program
double *execs;							        	//execution time of individual threads                           					

void *runner(int philNo){

	clock_t begin = clock();
	state[philNo]=THINKING;
	pthread_mutex_lock(&mutex);
	printf("Philosopher %d is thinking\n",philNo+1);
	pthread_mutex_unlock(&mutex);
	int randNum = (rand() % 3) + 1;
	sleep(randNum);                                              		//think for random duration between 1-3 seconds	
	printf("Philosopher %d is hungry\n",philNo+1);
	pickup_forks(philNo);							//pickup forks to eat
	clock_t end = clock()-begin;
	exec_time+=((double)(end)/CLOCKS_PER_SEC+(double)randNum);
	execs[philNo]+=(double)(end)/CLOCKS_PER_SEC+(double)randNum;
	return NULL;

}


void pickup_forks(int philNo){

	pthread_mutex_lock(&mutex);						// to protect the conditional variable
	state[philNo]=HUNGRY;
	//printf("Philosopher %d is hungry\n",philNo);
	check_forks(philNo);         						//check if forks are available
	while(state[philNo]!=EATING)
		pthread_cond_wait(&mutex, &cond_var);				//wait if neighbours are eating

	printf("Philosopher %d is eating\n",philNo+1);	
	int randNum = (rand() % 3) + 1;
	sleep(randNum);                                              		//eat for random duration between 1-3 seconds
	exec_time+=randNum;							//add sleep time to total execution time
	execs[philNo]+=randNum;
	return_forks(philNo);                                                   //put down the forks
	printf("Philosopher %d put down chopsticks %d and %d\n",philNo+1,((philNo+1)%n_phils)+1,((philNo+(n_phils-1))%n_phils)+1);
	pthread_mutex_unlock(&mutex);
		
	
}

void check_forks(int philNo){


	if(state[(philNo+(n_phils-1))%n_phils]!=EATING && (state[philNo]==HUNGRY) && (state[(philNo+1)%n_phils]!=EATING)){ 
										//check if neighbours are eating
		state[philNo]=EATING;
		pthread_cond_signal(&cond_var);
		
	}
	
}

void return_forks(int philNo){

	state[philNo]=THINKING;
	check_forks((philNo+1)%n_phils);
	check_forks((philNo+(n_phils-1))%n_phils);

}


void main(int argc,char** args){            					//number of philosophers is passed as a parameter to main


if(argc<2){

 	printf("Incorrect format\n");
 	printf("Syntax:  ./philosophers < N > \n");
 	exit(0);
}

pthread_mutex_init(&mutex,NULL);
pthread_cond_init(&cond_var,NULL);
int n=atoi(args[1]);

n_phils=n;

pthread_t philosophers[n];
pthread_attr_t attr[n];
execs=(double*)malloc(n*sizeof(double));
 
int i;
char ch='y';

while(ch=='y' || ch=='Y'){

	for(i=0;i<n;i++){
		pthread_attr_init(&attr[i]);
	}

	for(i=0;i<n;i++){

		pthread_create(&philosophers[i],&attr[i],(void *)runner,i);                		
	}

	for(i=0;i<n;i++){

		pthread_join(philosophers[i],NULL);                	
	}
	
	printf("Do you wish to continue?(Y/N)\n");
	scanf(" %c",&ch);
	
}

printf("Number of philosophers: %d \n",n_phils);
printf("Execution time of complete program: %f seconds\n",exec_time);
for(i=0;i<n;i++){
	
	printf("Execution time of thread %d: %f seconds\n",i+1,execs[i]);
		              	
}

}
