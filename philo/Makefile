NOCOL = \e[0m
GREEN = \e[1;92m
RED = \e[1;31m
CYAN = \e[1;36m
TCOL = $(CYAN)
RMD = $(GREEN)successfully $(RED)removed$(NOCOL)
CMP = $(GREEN)successfully compiled$(NOCOL)

#----------------------------------------------

CC = cc

NAME = philo

SRC = philo.c \
	err_check.c \
	philo_utils.c

FLAGS = -Wall -Werror -Wextra -g

TFLAG = -lpthread

all: $(NAME)

$(NAME): comp

comp:
	$(CC) $(FLAGS) $(SRC) $(TFLAG) -o $(NAME)
	echo "$(TCOL)$(NAME) $(CMP)"

clean:

fclean: clean
		if [ -f $(NAME) ]; then\
			rm -rf $(NAME);\
			echo "$(TCOL)$(NAME) $(RMD)";\
		fi

re: fclean all

.PHONY: all re clean fclean

.SILENT:
