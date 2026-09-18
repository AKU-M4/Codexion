/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adkaid-s <adkaid-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 18:03:26 by adkaid-s          #+#    #+#             */
/*   Updated: 2026/09/18 18:05:23 by adkaid-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	coder_compile(t_coder *c)
{
	pthread_mutex_lock(&c->state_lock);
	c->last_compile_time = get_abs_ms();
	pthread_mutex_unlock(&c->state_lock);
	log_state(c->sim, c->id, S_COMPILING);
	smart_sleep(c->sim, c->sim->time_to_compile);
	pthread_mutex_lock(&c->state_lock);
	c->compiles_done++;
	pthread_mutex_unlock(&c->state_lock);
}

static void	coder_debug_refactor(t_coder *c)
{
	if (sim_should_stop(c->sim))
		return ;
	log_state(c->sim, c->id, S_DEBUGING);
	smart_sleep(c->sim, c->sim->time_to_debug);
	if (sim_should_stop(c->sim))
		return ;
	log_state(c->sim, c->id, S_REFACTORING);
	smart_sleep(c->sim, c->sim->time_to_refactor);
}

static void	*single_coder(t_coder *c)
{
	pthread_mutex_lock(&c->right->lock);
	c->right->in_use = 1;
	pthread_mutex_unlock(&c->right->lock);
	log_state(c->sim, c->id, S_TAKEN_DONGLE);
	while (!sim_should_stop(c->sim))
		usleep(1000);
	return (NULL);
}

static int	coder_step(t_coder *c)
{
	if (!acquire_both(c))
		return (0);
	coder_compile(c);
	dongle_release(c->left, c->sim->dongle_cd);
	dongle_release(c->right, c->sim->dongle_cd);
	pthread_mutex_lock(&c->state_lock);
	if (c->compiles_done >= c->sim->nb_of_compiles)
	{
		pthread_mutex_unlock(&c->state_lock);
		return (0);
	}
	pthread_mutex_unlock(&c->state_lock);
	coder_debug_refactor(c);
	return (1);
}

void	*coder_routine(void *arg)
{
	t_coder	*c;

	c = (t_coder *)arg;
	pthread_mutex_lock(&c->sim->start_lock);
	pthread_mutex_unlock(&c->sim->start_lock);
	if (c->sim->nb_of_coders == 1)
		return (single_coder(c));
	if (c->id % 2 == 0)
		smart_sleep(c->sim, c->sim->time_to_compile / 2);
	while (!sim_should_stop(c->sim))
	{
		if (!coder_step(c))
			break ;
	}
	return (NULL);
}
