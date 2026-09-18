/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_errors.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adkaid-s <adkaid-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 18:03:39 by adkaid-s          #+#    #+#             */
/*   Updated: 2026/09/18 18:05:12 by adkaid-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

char	*turn_lower(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
			str[i] += 32;
		i++;
	}
	return (str);
}

int	arg_errors(int ac, char **av)
{
	int		i;
	char	*lower_str;

	if (ac != 9)
		return (1);
	i = 1;
	while (i < 8)
	{
		if (!is_numeric(av[i]))
			return (1);
		if (i < 7 && atoi(av[i]) <= 0)
			return (1);
		if (i == 7 && atoi(av[i]) < 0)
			return (1);
		i++;
	}
	lower_str = turn_lower(av[8]);
	if (strcmp(lower_str, "edf") != 0 && strcmp(lower_str, "fifo") != 0)
		return (1);
	return (0);
}
