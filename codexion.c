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


int main(int ac, char **av)
{
	int i = 1;
	if (ac < 2)
	{
		printf("Error check your arguments");
		return 0;
	}
	else 
	{
		while(i < ac)
		{
			printf("%s\n", av[i]);
			i++;
		}
	}
}
