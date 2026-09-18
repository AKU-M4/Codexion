/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adkaid-s <adkaid-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 18:02:55 by adkaid-s          #+#    #+#             */
/*   Updated: 2026/09/18 18:05:23 by adkaid-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	init_links(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->nb_of_coders)
	{
		sim->coders[i].sim = sim;
		sim->coders[i].right = &sim->dongles[i];
		sim->coders[i].left = &sim->dongles[(i + 1) % sim->nb_of_coders];
		i++;
	}
}

static void	init_sim_params(t_sim *sim, int *arr, char *scheduler)
{
	sim->nb_of_coders = arr[0];
	sim->time_to_burnout = arr[1];
	sim->time_to_compile = arr[2];
	sim->time_to_debug = arr[3];
	sim->time_to_refactor = arr[4];
	sim->nb_of_compiles = arr[5];
	sim->dongle_cd = arr[6];
	sim->scheduler = pick_scheduler(scheduler);
	sim->start_time = 0;
	sim->stop = 0;
}

t_sim	*parse_args(int *arr, char *scheduler)
{
	t_sim	*sim;

	sim = malloc(sizeof(t_sim));
	if (sim == NULL)
		return (NULL);
	init_sim_params(sim, arr, scheduler);
	sim->coders = build_coders(sim->nb_of_coders);
	sim->dongles = build_dongles(sim->nb_of_coders, sim->scheduler);
	if (sim->coders == NULL || sim->dongles == NULL)
	{
		cleanup_sim(sim);
		return (NULL);
	}
	init_links(sim);
	pthread_mutex_init(&sim->stop_lock, NULL);
	pthread_mutex_init(&sim->log_lock, NULL);
	pthread_mutex_init(&sim->start_lock, NULL);
	return (sim);
}
