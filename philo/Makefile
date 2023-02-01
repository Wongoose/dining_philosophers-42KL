NAME	= philo
CC		= gcc
CFLAGS	= -Wall -Wextra -Werror -pthread

SRCS	= error.c main.c philo.c utils.c
OBJS	= ${SRCS:.c=.o}

all:	${NAME}

${NAME}:	${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o ${NAME}

clean:
	rm -rf ${OBJS}

fclean:	clean
	rm -rf ${NAME}

re: fclean all

.PHONY: clean fclean re