#inluce "codexion.h"

t_time get_ms(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);

    return ((t_time)tv.tv_sec * 1000 + tv.tv_usec / 100 )
}