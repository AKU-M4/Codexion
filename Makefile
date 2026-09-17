NAME		= codexion
LIB			= libcodexion.a

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -pthread
AR			= ar
ARFLAGS		= rcs

SRC_DIR		= .
OBJ_DIR		= obj

SRCS		= codexion.c \
			  arg_errors.c \
			  parsing.c \
			  heap.c \
			  time_utils.c \
			  sim_state.c \
			  logger.c \
			  dongle.c \
			  coder.c \
			  monitor.c \
			  cleanup.c

OBJS		= $(SRCS:%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(LIB)
	$(CC) $(CFLAGS) $(LIB) -o $(NAME)

$(LIB): $(OBJS)
	$(AR) $(ARFLAGS) $(LIB) $(OBJS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c codexion.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME) $(LIB)

re: fclean all

.PHONY: all clean fclean re
