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
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int is_valid_digit(char *str)
{
	int	i;
	i = 0;

	while (str[i])
	{
		if (str[i] >= "0" && str[i] <= "9")
			i++;
		else
			return (1);
	}
	return (0);
}

int arg_errors(int ac, char **av)
{
	int	i;

	i = 0;
	while (i < ac)
	{
		
		i++;
	}
}

int main(int ac, char **av)
{
	int i;

	i = 0;
	while (i < ac)
	{

		i++;
	}
}
