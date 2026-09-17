#include "codexion.h"

void	cleanup_sim(t_sim *sim)
{
	int	i;

	if (sim == NULL)
		return ;
	i = 0;
	while (sim->coders && i < sim->nb_of_coders)
		pthread_mutex_destroy(&sim->coders[i++].state_lock);
	i = 0;
	while (sim->dongles && i < sim->nb_of_coders)
	{
		pthread_mutex_destroy(&sim->dongles[i].lock);
		pthread_cond_destroy(&sim->dongles[i].cond);
		free(sim->dongles[i].waiters.nodes);
		i++;
	}
	pthread_mutex_destroy(&sim->stop_lock);
	pthread_mutex_destroy(&sim->log_lock);
	pthread_mutex_destroy(&sim->start_lock);
	free(sim->coders);
	free(sim->dongles);
	free(sim);
}