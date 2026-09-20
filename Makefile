NAME	= codexion

CC		= cc
CFLAGS	= -Wall -Wextra -Werror -pthread

SRCS	= codexion.c \
		  arg_errors.c \
		  parsing.c \
		  builder.c \
		  heap.c \
		  heap_utils.c \
		  time_utils.c \
		  sim_state.c \
		  logger.c \
		  dongle.c \
		  dongle_utils.c \
		  coder.c \
		  monitor.c \
		  cleanup.c

OBJS	= $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c codexion.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
