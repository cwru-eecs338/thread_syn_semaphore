#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "sem_functions.h"
#include <pthread.h>


#define DELAY_TIME 3

void *do_work(void *);

int main(){
	pid_t result;
	int sem_id;
	pthread_t worker;
	sem_id = semget(ftok(".",'a'),1,0666|IPC_CREAT);
	init_sem(sem_id,0);
	pthread_create(&worker, NULL, do_work,(void *)&sem_id);

	sem_p(sem_id);  //wait
	printf("A thread %u is created by the initial thread(PID=%d, thread ID is %u)\n",(unsigned)worker, getpid(),(unsigned)pthread_self());
	sem_v(sem_id); //signal
	printf("The returned value of semaphore in initial thread is %d\n",semctl(sem_id,0,GETVAL,0));
	del_sem(sem_id);
	exit(0);
}

void * do_work(void *para){
	int sem_id;
	sem_id=(int)*(int *)para;
	printf("Child process will wait for some seconds...\n");
	printf("the thread ID for the new thread is %u, its process ID is %d\n",(unsigned)pthread_self(),getpid());
	sleep(DELAY_TIME);

	printf("the value of semaphore now in new thread is %d\n",semctl(sem_id,0,GETVAL,0));
	sem_v(sem_id);  //signal 

}

