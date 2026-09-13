#include "codexion.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


int main(int ac, char **av)
{
	int i = 1;
	if (ac < 2)
	{
		printf("Error check your arguments");
		return 0;
	}
	else 
	{
		while(i < ac)
		{
			printf("%s\n", av[i]);
			i++;
		}
	}
}