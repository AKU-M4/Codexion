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

int *turn_int_arr(char **av)
{
	int *arr;
	int	i;

	i = 1;
	while (i <= 7)
	{
		arr[i] = atoi(av[i]) 
		i++;
	}
	return arr
}

int main(int ac, char **av)
{
	int i;
	t_sim *sim;

	if (arg_errors(ac, av) == 1)
	{
		print("Missing or Invalid Arguemtns!");
		return (1);
	}
	arg_arr = turn_int_arr(av)
	sim = parse_args(arg_arr, av[8])
	i = 1;
	while (i <= ac)
	{
		print("")
		i++;
	}
}
