/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builder.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adkaid-s <adkaid-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 18:03:31 by adkaid-s          #+#    #+#             */
/*   Updated: 2026/09/18 18:05:23 by adkaid-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_coder	*build_coders(int nb_coders)
{
	int		i;
	t_coder	*coders;

	coders = malloc(sizeof(t_coder) * nb_coders);
	if (coders == NULL)
		return (NULL);
	i = 0;
	while (i < nb_coders)
	{
		coders[i].id = i + 1;
		coders[i].left = NULL;
		coders[i].right = NULL;
		coders[i].sim = NULL;
		coders[i].last_compile_time = 0;
		coders[i].compiles_done = 0;
		pthread_mutex_init(&coders[i].state_lock, NULL);
		i++;
	}
	return (coders);
}

static void	init_dongle(t_dongle *d, int id, t_scheduler sched)
{
	d->id = id;
	d->in_use = 0;
	d->available_at = 0;
	d->waiters.size = 0;
	d->waiters.capacity = 0;
	d->waiters.nodes = NULL;
	if (sched == SCHED_TYPE_EDF)
		d->waiters.cmp = cmp_edf;
	else
		d->waiters.cmp = cmp_fifo;
	pthread_mutex_init(&d->lock, NULL);
	pthread_cond_init(&d->cond, NULL);
}

t_dongle	*build_dongles(int nb_dongles, t_scheduler sched)
{
	int			i;
	t_dongle	*dongles;

	dongles = malloc(sizeof(t_dongle) * nb_dongles);
	if (dongles == NULL)
		return (NULL);
	i = 0;
	while (i < nb_dongles)
	{
		init_dongle(&dongles[i], i, sched);
		i++;
	}
	return (dongles);
}

t_scheduler	pick_scheduler(char *scheduler)
{
	if (strcmp(scheduler, "edf") == 0)
		return (SCHED_TYPE_EDF);
	return (SCHED_TYPE_FIFO);
}
