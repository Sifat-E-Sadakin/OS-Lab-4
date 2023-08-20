#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
/*
This program provides a possible solution using mutex and semaphore.
use 5 Farmers and 5 ShopOwners to demonstrate the solution.
*/
#define MaxCrops 5		// Maximum crops a Farmer can produce or a Shpoowner can take
#define warehouseSize 5 // Size of the warehouse
sem_t empty;
sem_t full;
int in = 0;
int out = 0;
char crops[warehouseSize] = {'R', 'W', 'P', 'S', 'M'};	   // indicating room for different crops
char warehouse[warehouseSize] = {'N', 'N', 'N', 'N', 'N'}; // initially all the room is empty
pthread_mutex_t mutex;
void *Farmer(void *far)
{
	for (int i = 0; i < MaxCrops; i++)
	{
		sem_wait(&empty);
		pthread_mutex_lock(&mutex);
		warehouse[in] = crops[in];

		printf("Farmer %d: Insert crops %c at %d\n", *((int *)far), warehouse[in], in);
		in = (in + 1) % warehouseSize;
		pthread_mutex_unlock(&mutex);

		sem_post(&full);
	}
	printf("Farmer%d:", *((int *)far));
	for (int g = 0; g < warehouseSize; g++)
	{

		printf(" %c", warehouse[g]);
	}
	printf("\n");
}
void *ShopOwner(void *sho)
{
	for (int i = 0; i < MaxCrops; i++)
	{
		sem_wait(&full);
		pthread_mutex_lock(&mutex);
		char item = warehouse[out];

		printf("Shop Owner %d: Remove crops %c from %d\n", *((int *)sho), item, out);
		warehouse[out] = 'N';
		out = (out + 1) % warehouseSize;
		pthread_mutex_unlock(&mutex);

		sem_post(&empty);
	}
	printf("ShopOwner%d:", *((int *)sho));
	for (int w = 0; w < warehouseSize; w++)
	{
		printf(" %c", warehouse[w]);
	}

	printf("\n");
}
int main()
{
	pthread_t farm[5], shop[5];
	pthread_mutex_init(&mutex, NULL);
	sem_init(&empty, 0, warehouseSize);
	sem_init(&full, 0, 0);
	int a[5] = {1, 2, 3, 4, 5};
	for (int i = 0; i < 5; i++)
	{
		pthread_create(&farm[i], NULL, (void *)Farmer, (void *)&a[i]);
	}
	for (int i = 0; i < 5; i++)
	{
		pthread_create(&shop[i], NULL, (void *)ShopOwner, (void *)&a[i]);
	}
	for (int i = 0; i < 5; i++)
	{
		pthread_join(farm[i], NULL);
	}
	for (int i = 0; i < 5; i++)
	{
		pthread_join(shop[i], NULL);
	}
	pthread_mutex_destroy(&mutex);
	sem_destroy(&empty);
	sem_destroy(&full);
	return 0;
}
