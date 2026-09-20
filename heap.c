/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adkaid-s <adkaid-s@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 18:03:04 by adkaid-s          #+#    #+#             */
/*   Updated: 2026/09/19 20:23:29 by adkaid-s         ###   ########.fr       */
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

int	heap_push(t_heap *h, t_wait_node node)
{
	size_t	i;

	if (h->size == h->capacity)
	{
		if (!heap_grow(h))
			return (0);
	}
	i = h->size;
	while (i > 0 && h->cmp(&node, &h->nodes[i - 1]))
	{
		h->nodes[i] = h->nodes[i - 1];
		i--;
	}
	h->nodes[i] = node;
	h->size++;
	return (1);
}

int	heap_pop(t_heap *h, t_wait_node *out)
{
	size_t	i;

	if (h->size == 0)
		return (0);
	*out = h->nodes[0];
	i = 0;
	while (i + 1 < h->size)
	{
		h->nodes[i] = h->nodes[i + 1];
		i++;
	}
	h->size--;
	return (1);
}