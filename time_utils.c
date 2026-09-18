/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adkaid-s <adkaid-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 18:02:41 by adkaid-s          #+#    #+#             */
/*   Updated: 2026/09/18 18:05:23 by adkaid-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_time	get_abs_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((t_time)tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	ms_to_timespec(t_time ms, struct timespec *ts)
{
	ts->tv_sec = ms / 1000;
	ts->tv_nsec = (ms % 1000) * 1000000;
}

void	smart_sleep(t_sim *sim, t_time duration_ms)
{
	t_time	start;

	start = get_abs_ms();
	while (!sim_should_stop(sim))
	{
		if (get_abs_ms() - start >= duration_ms)
			break ;
		usleep(500);
	}
}
