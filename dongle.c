#include "codexion.h"

static int	is_my_turn(t_dongle *d, t_coder *c)
{
	t_wait_node	top;

	if (!heap_peek(&d->waiters, &top))
		return (0);
	return (top.coder == c && !d->in_use && get_abs_ms() >= d->available_at);
}

void	dongle_acquire(t_dongle *d, t_coder *c)
{
	t_wait_node		node;
	t_wait_node		popped;
	struct timespec	ts;

	node.coder = c;
	node.arrival_time = get_abs_ms();
	node.deadline = c->last_compile_time + c->sim->time_to_burnout;
	pthread_mutex_lock(&d->lock);
	heap_push(&d->waiters, node);
	while (!is_my_turn(d, c) && !sim_should_stop(c->sim))
	{
		ms_to_timespec(get_abs_ms() + 5, &ts);
		pthread_cond_timedwait(&d->cond, &d->lock, &ts);
	}
	if (is_my_turn(d, c))
	{
		heap_pop(&d->waiters, &popped);
		d->in_use = 1;
	}
	pthread_mutex_unlock(&d->lock);
	if (!sim_should_stop(c->sim))
		log_state(c->sim, c->id, S_TAKEN_DONGLE);
}

void	dongle_release(t_dongle *d, t_time cooldown)
{
	pthread_mutex_lock(&d->lock);
	d->in_use = 0;
	d->available_at = get_abs_ms() + cooldown;
	pthread_cond_broadcast(&d->cond);
	pthread_mutex_unlock(&d->lock);
}
