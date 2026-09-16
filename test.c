#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t fuel_mutex;
pthread_cond_t fuel_cond;
int fuel = 0;

void *fueling()
{
	int i = 0;
	while (i < 10)
	{
		pthread_mutex_lock(&fuel_mutex);
		fuel += 70;
		printf("Fuel was Filled %d...\n", fuel);
		pthread_mutex_unlock(&fuel_mutex);
		pthread_cond_broadcast(&fuel_cond);
		sleep(1);
		i++;
	}
}

void *car()
{
	pthread_mutex_lock(&fuel_mutex);
	while (fuel < 40)
	{
		printf("Not enough Fuel yet\n");
		pthread_cond_wait(&fuel_cond, &fuel_mutex);
	}
	fuel -= 40;
	printf("=== CAR FUELED UP ===");
	printf("Fuel left after filling %d\n", fuel);
	pthread_mutex_unlock(&fuel_mutex);
}

int main()
{
	int i = 0;
	pthread_t t[6];

	pthread_mutex_init(&fuel_mutex, NULL);
	pthread_cond_init(&fuel_cond, NULL);
	while (i < 6)
	{
		if (i == 4)
			pthread_create(&t[i], NULL, &fueling, NULL);
		else
			pthread_create(&t[i], NULL, &car, NULL);
			printf("=== CAR NUMBER %d ===", i + 1);
		i++;
	}
	i = 0;
	while ( i < 5)
	{
		pthread_join(t[i], NULL);
		i++;
	}
	pthread_mutex_destroy(&fuel_mutex);
	pthread_cond_destroy(&fuel_cond);
}