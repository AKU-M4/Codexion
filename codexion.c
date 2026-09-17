/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adkaid-s <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 16:19:26 by adkaid-s          #+#    #+#             */
/*   Updated: 2026/09/13 16:19:28 by adkaid-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	*turn_int_arr(char **av)
{
	int	*arr;
	int	i;

	arr = malloc(sizeof(int) * 7);
	if (arr == NULL)
		return (NULL);
	i = 0;
	while (i < 7)
	{
		arr[i] = atoi(av[i + 1]);
		i++;
	}
	return (arr);
}

static void	spawn_threads(t_sim *sim)
{
	int	i;

	sim->start_time = get_abs_ms();
	i = 0;
	while (i < sim->nb_of_coders)
	{
		sim->coders[i].last_compile_time = sim->start_time;
		pthread_create(&sim->coders[i].thread, NULL, coder_routine,
			&sim->coders[i]);
		i++;
	}
	pthread_create(&sim->monitor, NULL, monitor_routine, sim);
}

static void	join_threads(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->nb_of_coders)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
	pthread_join(sim->monitor, NULL);
}

int	main(int ac, char **av)
{
	t_sim	*sim;
	int		*arg_arr;

	if (arg_errors(ac, av) == 1)
	{
		printf("Missing or Invalid Arguments!\n");
		return (1);
	}
	arg_arr = turn_int_arr(av);
	if (arg_arr == NULL)
		return (1);
	sim = parse_args(arg_arr, av[ac - 1]);
	free(arg_arr);
	if (sim == NULL)
		return (1);
	spawn_threads(sim);
	join_threads(sim);
	cleanup_sim(sim);
	return (0);
}