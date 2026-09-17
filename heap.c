#include "codexion.h"

int	cmp_fifo(const t_wait_node *a, const t_wait_node *b)
{
	return (a->arrival_time < b->arrival_time);
}

int	cmp_edf(const t_wait_node *a, const t_wait_node *b)
{
	return (a->deadline < b->deadline);
}

static void	swap_nodes(t_wait_node *a, t_wait_node *b)
{
	t_wait_node	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static int	heap_grow(t_heap *h)
{
	t_wait_node	*bigger;
	size_t		new_cap;
	size_t		i;

	new_cap = h->capacity == 0 ? 4 : h->capacity * 2;
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
	size_t	parent;

	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (h->cmp(&h->nodes[i], &h->nodes[parent]))
		{
			swap_nodes(&h->nodes[i], &h->nodes[parent]);
			i = parent;
		}
		else
			break ;
	}
}

static void	sift_down(t_heap *h, size_t i)
{
	size_t	left;
	size_t	right;
	size_t	best;

	while (1)
	{
		left = i * 2 + 1;
		right = i * 2 + 2;
		best = i;
		if (left < h->size && h->cmp(&h->nodes[left], &h->nodes[best]))
			best = left;
		if (right < h->size && h->cmp(&h->nodes[right], &h->nodes[best]))
			best = right;
		if (best == i)
			break ;
		swap_nodes(&h->nodes[i], &h->nodes[best]);
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

int	heap_peek(t_heap *h, t_wait_node *out)
{
	if (h->size == 0)
		return (0);
	*out = h->nodes[0];
	return (1);
}