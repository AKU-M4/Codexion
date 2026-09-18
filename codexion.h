/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adkaid-s <adkaid-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 18:05:36 by adkaid-s          #+#    #+#             */
/*   Updated: 2026/09/18 18:05:36 by adkaid-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>

typedef long long	t_time;

typedef enum e_scheduler
{
	SCHED_TYPE_FIFO,
	SCHED_TYPE_EDF
}	t_scheduler;

typedef enum e_state
{
	S_TAKEN_DONGLE,
	S_COMPILING,
	S_DEBUGING,
	S_REFACTORING,
	S_BURNED_OUT
}	t_state;

typedef struct s_sim		t_sim;
typedef struct s_coder		t_coder;
typedef struct s_dongle		t_dongle;

typedef struct s_wait_node
{
	t_coder	*coder;
	t_time	arrival_time;
	t_time	deadline;
}	t_wait_node;

typedef int	(*t_heap_cmp)(const t_wait_node *a, const t_wait_node *b);

typedef struct s_heap
{
	t_wait_node	*nodes;
	size_t		size;
	size_t		capacity;
	t_heap_cmp	cmp;
}	t_heap;

struct s_coder
{
	int				id;
	pthread_t		thread;
	t_dongle		*left;
	t_dongle		*right;
	t_sim			*sim;
	t_time			last_compile_time;
	int				compiles_done;
	pthread_mutex_t	state_lock;
};

struct s_dongle
{
	int				id;
	pthread_mutex_t	lock;
	pthread_cond_t	cond;
	int				in_use;
	t_time			available_at;
	t_heap			waiters;
};

struct s_sim
{
	int				nb_of_coders;
	int				nb_of_compiles;
	t_time			time_to_burnout;
	t_time			time_to_compile;
	t_time			time_to_debug;
	t_time			time_to_refactor;
	t_time			dongle_cd;
	t_scheduler		scheduler;
	t_time			start_time;
	t_coder			*coders;
	t_dongle		*dongles;
	pthread_t		monitor;
	volatile int	stop;
	pthread_mutex_t	stop_lock;
	pthread_mutex_t	log_lock;
	pthread_mutex_t	start_lock;
};

/* parsing.c / arg_errors.c / codexion.c */
t_sim		*parse_args(int *arr, char *scheduler);
t_scheduler	pick_scheduler(char *scheduler);
t_dongle	*build_dongles(int nb_dongles, t_scheduler sched);
t_coder		*build_coders(int nb_coders);
int			arg_errors(int ac, char **av);
char		*turn_lower(char *str);
int			*turn_int_arr(char **av);

/* heap.c */
int			cmp_fifo(const t_wait_node *a, const t_wait_node *b);
int			cmp_edf(const t_wait_node *a, const t_wait_node *b);
int			heap_push(t_heap *h, t_wait_node node);
int			heap_pop(t_heap *h, t_wait_node *out);
int			heap_peek(t_heap *h, t_wait_node *out);
void		heap_remove_coder(t_heap *h, t_coder *c);

/* time_utils.c */
t_time		get_abs_ms(void);
void		ms_to_timespec(t_time ms, struct timespec *ts);
void		smart_sleep(t_sim *sim, t_time duration_ms);

/* sim_state.c */
int			sim_should_stop(t_sim *sim);
void		set_stop(t_sim *sim);
int			check_all_done(t_sim *sim);

/* logger.c */
void		log_state(t_sim *sim, int coder_id, t_state state);

/* dongle.c / dongle_utils.c */
int			can_take(t_dongle *d, t_coder *c, t_time now);
void		wait_both(t_dongle *f, t_dongle *s);
void		push_waiter(t_dongle *d, t_coder *c, t_time now);
int			acquire_both(t_coder *c);
void		dongle_release(t_dongle *d, t_time cooldown);

/* coder.c */
void		*coder_routine(void *arg);

/* monitor.c */
void		*monitor_routine(void *arg);

/* cleanup.c */
void		cleanup_sim(t_sim *sim);

#endif
