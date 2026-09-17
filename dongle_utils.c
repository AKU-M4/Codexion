#include "codexion.h"

int	can_take(t_dongle *d, t_coder *c, t_time now)
{
	t_wait_node	top;

	if (d->in_use || now < d->available_at)
		return (0);
	if (heap_peek(&d->waiters, &top) && top.coder != c)
		return (0);
	return (1);
}

void	wait_both(t_dongle *f, t_dongle *s)
{
	struct timespec	ts;
	t_time			now;
	t_time			wake;

	now = get_abs_ms();
	wake = now + 5;
	if (!f->in_use && f->available_at > now && f->available_at < wake)
		wake = f->available_at;
	if (!s->in_use && s->available_at > now && s->available_at < wake)
		wake = s->available_at;
	ms_to_timespec(wake, &ts);
	pthread_cond_timedwait(&f->cond, &f->lock, &ts);
}

void	push_waiter(t_dongle *d, t_coder *c, t_time now)
{
	t_wait_node	node;

	node.coder = c;
	node.arrival_time = now;
	pthread_mutex_lock(&c->state_lock);
	node.deadline = c->last_compile_time + c->sim->time_to_burnout;
	pthread_mutex_unlock(&c->state_lock);
	pthread_mutex_lock(&d->lock);
	heap_push(&d->waiters, node);
	pthread_mutex_unlock(&d->lock);
}
