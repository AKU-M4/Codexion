/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adkaid-s <adkaid-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 18:03:07 by adkaid-s          #+#    #+#             */
/*   Updated: 2026/09/18 18:05:23 by adkaid-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	cmp_fifo(const t_wait_node *a, const t_wait_node *b)
{
	if (a->arrival_time != b->arrival_time)
		return (a->arrival_time < b->arrival_time);
	return (a->coder->id < b->coder->id);
}

int	cmp_edf(const t_wait_node *a, const t_wait_node *b)
{
	if (a->deadline != b->deadline)
		return (a->deadline < b->deadline);
	if (a->arrival_time != b->arrival_time)
		return (a->arrival_time < b->arrival_time);
	return (a->coder->id < b->coder->id);
}

int	heap_peek(t_heap *h, t_wait_node *out)
{
	if (h->size == 0)
		return (0);
	*out = h->nodes[0];
	return (1);
}

void	heap_remove_coder(t_heap *h, t_coder *c)
{
	size_t	i;

	i = 0;
	while (i < h->size)
	{
		if (h->nodes[i].coder == c)
		{
			h->nodes[i] = h->nodes[h->size - 1];
			h->size--;
			return ;
		}
		i++;
	}
}
