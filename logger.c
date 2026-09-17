#include "codexion.h"

static const char	*state_to_str(t_state state)
{
	if (state == S_TAKEN_DONGLE)
		return ("has taken a dongle");
	if (state == S_COMPILING)
		return ("is compiling");
	if (state == S_DEBUGING)
		return ("is debugging");
	if (state == S_REFACTORING)
		return ("is refactoring");
	return ("burned out");
}

void	log_state(t_sim *sim, int coder_id, t_state state)
{
	t_time	elapsed;

	pthread_mutex_lock(&sim->log_lock);
	elapsed = get_abs_ms() - sim->start_time;
	printf("%lld %d %s\n", elapsed, coder_id, state_to_str(state));
	pthread_mutex_unlock(&sim->log_lock);
}
