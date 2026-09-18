/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adkaid-s <adkaid-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 18:02:58 by adkaid-s          #+#    #+#             */
/*   Updated: 2026/09/18 18:05:23 by adkaid-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	check_one_coder(t_sim *sim, t_coder *coder)
{
	t_time	last;
	int		done;

	pthread_mutex_lock(&coder->state_lock);
	last = coder->last_compile_time;
	done = coder->compiles_done;
	pthread_mutex_unlock(&coder->state_lock);
	if (done >= sim->nb_of_compiles)
		return (0);
	if (get_abs_ms() - last > sim->time_to_burnout)
	{
		log_state(sim, coder->id, S_BURNED_OUT);
		return (1);
	}
	return (0);
}

static int	check_burnout(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->nb_of_coders)
	{
		if (check_one_coder(sim, &sim->coders[i]))
			return (1);
		i++;
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;

	sim = (t_sim *)arg;
	pthread_mutex_lock(&sim->start_lock);
	pthread_mutex_unlock(&sim->start_lock);
	while (!sim_should_stop(sim))
	{
		if (check_all_done(sim))
		{
			set_stop(sim);
			return (NULL);
		}
		if (check_burnout(sim))
			return (NULL);
		usleep(500);
	}
	return (NULL);
}