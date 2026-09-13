#include "codexion.h"

t_scheduler pick_scheduler(char *scheduler)
{
	t_scheduler sched;
	if (strcmp(scheduler, "edf") == 0)
		return SCHED_EDF;
	if (strcmp(scheduler, "fifo") == 0)
		return SCHED_EDF;
}	

t_sim *parse_args(int *arr, char *scheduler)
{
	t_sim *sim;
	int	i;
	int	arr_len;
		
	i = 0;
	sim = malloc(sizeof(t_sim));

	sim->nb_of_coders = arr[0];
	sim->time_to_burnout = arr[1];
	sim->time_to_compile = arr[2];
	sim->time_to_debug = arr[3];
	sim->time_to_refactor = arr[4];
	sim->nb_of_compiles = arr[5];
	sim->dongle_cd = arr[6];
	sim->scheduler = pick_scheduler(scheduler);
	sim->coders = 

}
