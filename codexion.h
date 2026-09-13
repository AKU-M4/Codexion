/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adkaid-s <adkaid-s@1337.ma>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 16:17:30 by adkaid-s          #+#    #+#             */
/*   Updated: 2026/09/13 16:17:36 by adkaid-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
#define CODEXION_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

t_sim 		*parse_args(int *arr, char *scheduler);
t_scheduler pick_scheduler(char *scheduler);
t_dongle	*build_dongles(int nb_dongles);
t_coder		*build_coders(int nb_coders);


typedef long long t_time;

typedef enum e_scheduler{
	SCHED_FIFO,
	SCHED_EDF
}	t_scheduler;


typedef enum e_state{
	S_TAKEN_DONGLE,
	S_COMPILING,
	S_DEBUGING,
	S_REFACTORING
}	t_state;

typedef struct s_wait_node{
	t_coder *coder;
	t_time arrival_time;
	t_time deadline;
}	t_wait_node;

typedef int	(*t_heap_cmp)(const t_wait_node *a, const t_wait_node *b);

typedef struct t_heap{
	t_wait_node	*nodes;
	size_t	size;
	size_t	capacity;
	t_heap_cmp cmp;
}	t_heap;

typedef struct s_sim t_sim;
typedef	struct s_coder	t_coder;
typedef	struct s_dongle t_dongle;

struct s_coder 
{
	int id;
	pthread_t thread;
	t_dongle *left;
	t_dongle *right;
	t_sim *sim;

	t_time last_compile_time;
	int compiles_done;

	pthread_mutex_t state_lock;
};

typedef struct s_dongle{
	int id;
	pthread_mutex_t lock;
	pthread_cond_t cond;
	int in_use;
	t_time available_at;
	t_heap	waiters;
}	t_dongle;

struct	s_sim{
	int			nb_of_coders;
	int			nb_of_compiles;
	t_time		time_to_burnout;
	t_time		time_to_compile;
	t_time		time_to_debug;
	t_time		time_to_refactor;
	t_time		dongle_cd;
	t_scheduler	scheduler;
	
	t_time 		start_time;
	t_coder		*coders;
	t_dongle	*dongles;
	
	volatile	int	stop;
	pthread_mutex_t	stop_lock;

	pthread_mutex_t log_lock;
};

#endif
