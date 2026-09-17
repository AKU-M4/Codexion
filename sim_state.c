#include "codexion.h"

int	sim_should_stop(t_sim *sim)
{
	int	val;

	pthread_mutex_lock(&sim->stop_lock);
	val = sim->stop;
	pthread_mutex_unlock(&sim->stop_lock);
	return (val);
}

void	set_stop(t_sim *sim)
{
	pthread_mutex_lock(&sim->stop_lock);
	sim->stop = 1;
	pthread_mutex_unlock(&sim->stop_lock);
}

int	check_all_done(t_sim *sim)
{
	int	i;
	int	done;

	i = 0;
	while (i < sim->nb_of_coders)
	{
		pthread_mutex_lock(&sim->coders[i].state_lock);
		done = sim->coders[i].compiles_done >= sim->nb_of_compiles;
		pthread_mutex_unlock(&sim->coders[i].state_lock);
		if (!done)
			return (0);
		i++;
	}
	return (1);
}
