#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <string.h>
#include "buffer.h"

//Sub_Routines
int insert_item(buffer_item item);
int remove_item(buffer_item *item);
void *consumer(void *param);
void *producer(void *param);

//Global_variables
buffer_item buffer[BUFFER_SIZE];
//semaphore flags
sem_t full, empty, mutex;
int counter, input, output;

//main
int main(int argc, char **argv)
{
	if (argc != 4)
	{
		printf("Error input.Please follow the usage : ./program_name <wait time> <Producer processes amout> <Consumer processes amout>.\n");
		exit(1);
	}

	//Transfer argvs from string to integer
	const long int stime = strtol(argv[1], NULL, 0);
	const long int num_producer = strtol(argv[2], NULL, 0);
	const long int num_consumer = strtol(argv[3], NULL, 0);
	pthread_t producers[num_producer];
	pthread_t consumers[num_consumer];

	int i;		//index fo iterative
	//use time as random seed
	srand(time(NULL));
	//initialize sem_empty and sem_full
	sem_init(&empty, 0, BUFFER_SIZE);
	sem_init(&full, 0, 0);
	//Use semaphore to replace mutex
	sem_init(&mutex, 0, 1);

	//Create the producer and consumer using pthreads
	for(i=0; i<num_producer;i++)
		pthread_create(&producers[i],NULL,producer,NULL);
	for(i=0; i<num_consumer;i++)
		pthread_create(&consumers[i],NULL,consumer,NULL);

	//Let Producer and Consumer runs a while then terminate the program
	sleep(stime);
	return 0;
}

//Producer thread
void *producer(void *param)
{
	int item;
	while(1)
	{
		//Sleep for 1~3 seconds to wait semaphores
		sleep(rand()%3+1); 
		//random a number between 0 and RAND_MAX
		item = rand();
		//if return 0(success)then print the number inserted to buffer
		if(!insert_item(item))
			printf("Insert %d to buffer.\n",item);
		else
			printf("Insert Error.\n");
			
	}
}

//Consumer thread
void *consumer(void *param)
{
	int item;
	while(1)
	{
		//Sleep for 1~3 seconds to wait semaphores
		sleep(rand()%3+1); 
		//if return 0(success)then print the number removed from buffer
		if(!remove_item(&item))
			printf("Remove %d from buffer.\n",item);
		else
			printf("Remove Error.\n");
	}
}

//insert_item subroutine
int insert_item(buffer_item item)
{
	int success;		//return value
	sem_wait(&empty);	//wait for empty and the mutex state
	sem_wait(&mutex);

	//insert a item to buffer
	if(counter != BUFFER_SIZE)
	{
		buffer[input] = item;
		//circular queue
		input = (input+1)%BUFFER_SIZE;
		counter++;
		success = 0;
	}
	//failed
	else
		success = -1;
	
	//Set semaphores
	sem_post(&mutex);
	sem_post(&full);
	
	return success;
}

//remove_item subroutine
int remove_item(buffer_item *item)
{
	int success;
	sem_wait(&full);
	sem_wait(&mutex);

	//Remove a item from buffer
	if(counter != 0)
	{
		*item = buffer[output];
		//circular queue
		output = (output+1)%BUFFER_SIZE;
		counter--;
		success = 0;
	}
	//failed
	else
		success = -1;
	
	//Set semaphores
	sem_post(&mutex);
	sem_post(&empty);

	return success;
}

