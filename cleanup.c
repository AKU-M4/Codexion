#include "codexion.h"

void	cleanup_sim(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->nb_of_coders)
	{
		pthread_mutex_destroy(&sim->coders[i].state_lock);
		i++;
	}
	i = 0;
	while (i < sim->nb_of_coders)
	{
		pthread_mutex_destroy(&sim->dongles[i].lock);
		pthread_cond_destroy(&sim->dongles[i].cond);
		free(sim->dongles[i].waiters.nodes);
		i++;
	}
	pthread_mutex_destroy(&sim->stop_lock);
	pthread_mutex_destroy(&sim->log_lock);
	free(sim->coders);
	free(sim->dongles);
	free(sim);
}