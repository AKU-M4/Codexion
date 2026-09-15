/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adkaid-s <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 16:18:46 by adkaid-s          #+#    #+#             */
/*   Updated: 2026/09/13 16:18:48 by adkaid-s         ###   ########.fr       */
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

t_dongle	*build_dongles(int nb_dongles)
{
	int			i;
	t_dongle	*dongles;

	dongles = malloc(sizeof(t_dongle) * nb_dongles);
	if (dongles == NULL)
		return (NULL);
	i = 0;
	while (i < nb_dongles)
	{
		dongles[i].id = i;
		dongles[i].in_use = 0;
		dongles[i].available_at = 0;
		dongles[i].waiters.size = 0;
		dongles[i].waiters.capacity = 0;
		dongles[i].waiters.cmp = NULL;
		dongles[i].waiters.nodes = NULL;
		pthread_mutex_init(&dongles[i].lock, NULL);
		pthread_cond_init(&dongles[i].cond, NULL);
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

t_sim	*parse_args(int *arr, char *scheduler)
{
	int		i;
	t_sim	*sim;

	sim = malloc(sizeof(t_sim));
	if (sim == NULL)
		return (NULL);
	sim->nb_of_coders = arr[0];
	sim->time_to_burnout = arr[1];
	sim->time_to_compile = arr[2];
	sim->time_to_debug = arr[3];
	sim->time_to_refactor = arr[4];
	sim->nb_of_compiles = arr[5];
	sim->dongle_cd = arr[6];
	sim->scheduler = pick_scheduler(scheduler);
	sim->coders = build_coders(sim->nb_of_coders);
	sim->dongles = build_dongles(sim->nb_of_coders);
	if (sim->coders == NULL || sim->dongles == NULL)
	{
		free(sim);
		return (NULL);
	}
	i = 0;
	while (i < sim->nb_of_coders)
	{
		sim->coders[i].sim = sim;
		sim->coders[i].right = &sim->dongles[i];
		sim->coders[i].left = &sim->dongles[(i + 1) % sim->nb_of_coders];
		i++;
	}
	sim->start_time = 0;
	sim->stop = 0;
	pthread_mutex_init(&sim->stop_lock, NULL);
	pthread_mutex_init(&sim->log_lock, NULL);
	return (sim);
}