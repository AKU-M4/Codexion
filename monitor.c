#include "codexion.h"

static int	check_burnout(t_sim *sim)
{
	int		i;
	t_time	last;

	i = 0;
	while (i < sim->nb_of_coders)
	{
		pthread_mutex_lock(&sim->coders[i].state_lock);
		last = sim->coders[i].last_compile_time;
		pthread_mutex_unlock(&sim->coders[i].state_lock);
		if (get_abs_ms() - last > sim->time_to_burnout)
		{
			log_state(sim, sim->coders[i].id, S_BURNED_OUT);
			return (1);
		}
		i++;
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;

	sim = (t_sim *)arg;
	while (1)
	{
		if (check_burnout(sim))
		{
			set_stop(sim);
			return (NULL);
		}
		if (check_all_done(sim))
		{
			set_stop(sim);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}