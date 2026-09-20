/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adkaid-s <adkaid-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 18:03:12 by adkaid-s          #+#    #+#             */
/*   Updated: 2026/09/18 18:05:23 by adkaid-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	order_dongles(t_coder *c, t_dongle **f, t_dongle **s)
{
	*f = c->left;
	*s = c->right;
	if ((*f)->id > (*s)->id)
	{
		*f = c->right;
		*s = c->left;
	}
}

int	can_take(t_dongle *d, t_coder *c, t_time now)
{
	t_wait_node	top;

	if (d->in_use || now < d->available_at)
		return (0);
	if (heap_peek(&d->waiters, &top) && top.coder != c)
		return (0);
	return (1);
}

static void	do_wait(t_dongle *target, t_time wake)
{
	struct timespec	ts;

	if (wake > 0)
	{
		ms_to_timespec(wake, &ts);
		pthread_cond_timedwait(&target->cond, &target->lock, &ts);
	}
	else
		pthread_cond_wait(&target->cond, &target->lock);
	pthread_mutex_unlock(&target->lock);
}

void	wait_both(t_dongle *f, t_dongle *s)
{
	t_dongle		*target;
	t_dongle		*other;
	t_time			now;
	t_time			wake;

	now = get_abs_ms();
	target = f;
	other = s;
	if (!f->in_use && now >= f->available_at
		&& (s->in_use || now < s->available_at))
	{
		target = s;
		other = f;
	}
	wake = 0;
	if (!target->in_use && target->available_at > now)
		wake = target->available_at;
	else if (!target->in_use)
		wake = now + 5;
	pthread_mutex_unlock(&other->lock);
	do_wait(target, wake);
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
