#include <pthread.h>
#include <stdio.h>

pthread_mutex_t	lock;
int				x = 0;

void	*add(void *x)
{
	int	*n;
	int	i;

	n = (int *)x;
	i = 0;
	while (i < 100)
	{
		pthread_mutex_lock(&lock);
		*n += i++;
		pthread_mutex_unlock(&lock);
	}
	printf("%i\n", *n);
}

int	main(void) // thread!!
{
	pthread_t id[10];

	pthread_mutex_init(&lock, NULL);

	int i = 0;
	while (i < 10)
	{
		pthread_create(&id[i], NULL, add, &x);
		i++;
	}

	i = 0;
	while (i < 10)
		pthread_join(id[i++], NULL);

	printf("\noutput:   %i\n", x);
	return (0);
}