#include "codexion.h"

static void	acquire_both(t_coder *c)
{
	t_dongle	*first;
	t_dongle	*second;

	if (c->left->id < c->right->id)
	{
		first = c->left;
		second = c->right;
	}
	else
	{
		first = c->right;
		second = c->left;
	}
	dongle_acquire(first, c);
	dongle_acquire(second, c);
}

static void	coder_compile(t_coder *c)
{
	pthread_mutex_lock(&c->state_lock);
	c->last_compile_time = get_abs_ms();
	pthread_mutex_unlock(&c->state_lock);
	log_state(c->sim, c->id, S_COMPILING);
	usleep(c->sim->time_to_compile * 1000);
	pthread_mutex_lock(&c->state_lock);
	c->compiles_done++;
	pthread_mutex_unlock(&c->state_lock);
}

static void	coder_debug_refactor(t_coder *c)
{
	log_state(c->sim, c->id, S_DEBUGING);
	usleep(c->sim->time_to_debug * 1000);
	log_state(c->sim, c->id, S_REFACTORING);
	usleep(c->sim->time_to_refactor * 1000);
}

void	*coder_routine(void *arg)
{
	t_coder	*c;

	c = (t_coder *)arg;
	if (c->sim->nb_of_coders == 1)
	{
		/* Only one dongle exists in total, and compiling needs two.
		** A lone coder can never compile; they will burn out and the
		** monitor thread will stop the simulation. We just wait. */
		while (!sim_should_stop(c->sim))
			usleep(1000);
		return (NULL);
	}
	while (!sim_should_stop(c->sim))
	{
		acquire_both(c);
		coder_compile(c);
		dongle_release(c->left, c->sim->dongle_cd);
		dongle_release(c->right, c->sim->dongle_cd);
		if (sim_should_stop(c->sim))
			break ;
		coder_debug_refactor(c);
	}
	return (NULL);
}
