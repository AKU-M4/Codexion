/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adkaid-s <adkaid-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 18:03:10 by adkaid-s          #+#    #+#             */
/*   Updated: 2026/09/18 18:05:23 by adkaid-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	try_claim_both(t_dongle *f, t_dongle *s, t_coder *c, t_time now)
{
	t_wait_node	p;

	if (!can_take(f, c, now) || !can_take(s, c, now))
		return (0);
	heap_pop(&f->waiters, &p);
	heap_pop(&s->waiters, &p);
	f->in_use = 1;
	s->in_use = 1;
	pthread_mutex_unlock(&s->lock);
	pthread_mutex_unlock(&f->lock);
	log_state(c->sim, c->id, S_TAKEN_DONGLE);
	log_state(c->sim, c->id, S_TAKEN_DONGLE);
	return (1);
}

int	acquire_both(t_coder *c)
{
	t_dongle	*f;
	t_dongle	*s;
	t_time		now;

	order_dongles(c, &f, &s);
	now = get_abs_ms();
	push_waiter(f, c, now);
	push_waiter(s, c, now);
	while (!sim_should_stop(c->sim))
	{
		pthread_mutex_lock(&f->lock);
		pthread_mutex_lock(&s->lock);
		if (try_claim_both(f, s, c, get_abs_ms()))
			return (1);
		wait_both(f, s);
		pthread_mutex_unlock(&f->lock);
	}
	return (0);
}

void	dongle_release(t_dongle *d, t_time cooldown)
{
	pthread_mutex_lock(&d->lock);
	d->in_use = 0;
	d->available_at = get_abs_ms() + cooldown;
	pthread_cond_broadcast(&d->cond);
	pthread_mutex_unlock(&d->lock);
}
