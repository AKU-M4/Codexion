/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adkaid-s <adkaid-s@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 18:03:04 by adkaid-s          #+#    #+#             */
/*   Updated: 2026/09/19 00:31:28 by adkaid-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	heap_grow(t_heap *h)
{
	t_wait_node	*bigger;
	size_t		new_cap;
	size_t		i;

	if (h->capacity == 0)
		new_cap = 4;
	else
		new_cap = h->capacity * 2;
	bigger = malloc(sizeof(t_wait_node) * new_cap);
	if (bigger == NULL)
		return (0);
	i = 0;
	while (i < h->size)
	{
		bigger[i] = h->nodes[i];
		i++;
	}
	free(h->nodes);
	h->nodes = bigger;
	h->capacity = new_cap;
	return (1);
}

static void	sift_up(t_heap *h, size_t i)
{
	size_t		parent;
	t_wait_node	tmp;

	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (h->cmp(&h->nodes[i], &h->nodes[parent]))
		{
			tmp = h->nodes[i];
			h->nodes[i] = h->nodes[parent];
			h->nodes[parent] = tmp;
			i = parent;
		}
		else
			break ;
	}
}

static void	sift_down(t_heap *h, size_t i)
{
	size_t		best;
	t_wait_node	tmp;

	while (1)
	{
		best = i;
		if (i * 2 + 1 < h->size
			&& h->cmp(&h->nodes[i * 2 + 1], &h->nodes[best]))
			best = i * 2 + 1;
		if (i * 2 + 2 < h->size
			&& h->cmp(&h->nodes[i * 2 + 2], &h->nodes[best]))
			best = i * 2 + 2;
		if (best == i)
			break ;
		tmp = h->nodes[i];
		h->nodes[i] = h->nodes[best];
		h->nodes[best] = tmp;
		i = best;
	}
}

int	heap_push(t_heap *h, t_wait_node node)
{
	if (h->size == h->capacity)
	{
		if (!heap_grow(h))
			return (0);
	}
	h->nodes[h->size] = node;
	h->size++;
	sift_up(h, h->size - 1);
	return (1);
}

int	heap_pop(t_heap *h, t_wait_node *out)
{
	if (h->size == 0)
		return (0);
	*out = h->nodes[0];
	h->size--;
	h->nodes[0] = h->nodes[h->size];
	if (h->size > 0)
		sift_down(h, 0);
	return (1);
}