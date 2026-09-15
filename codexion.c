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
	printf("%i\n", sim->nb_of_coders);
	printf("%lli\n", sim->time_to_compile);
	return (0);
}