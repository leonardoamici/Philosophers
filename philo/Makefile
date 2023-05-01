NOCOL = \e[0m
GREEN = \e[1;92m
RED = \e[1;31m
CYAN = \e[1;36m
TCOL = $(CYAN)
RMD = $(GREEN)successfully $(RED)removed$(NOCOL)
CMP = $(GREEN)successfully compiled$(NOCOL)
SCMP = $(GREEN)successfully sanitized and compiled$(NOCOL)
#----------------------------------------------

CC = cc

NAME = philo

SRC = philo.c \
	err_check.c \
	philo_utils.c \
	philo_utils2.c \
	utils.c \
	action_utils.c

FLAGS = -Wall -Werror -Wextra -g

SFLAGS = -fsanitize=address

TFLAG = -lpthread

all: $(NAME)

$(NAME): fclean
	$(CC) $(FLAGS) $(SRC) $(TFLAG) -o $(NAME)
	echo "$(TCOL)$(NAME) $(CMP)"

sanitize: 
	$(CC) $(SFLAGS) $(FLAGS) $(SRC) $(TFLAG) -o $(NAME)
	echo "$(TCOL)$(NAME) $(SCMP)"

clean:

fclean: clean
		if [ -f $(NAME) ]; then\
			rm -rf $(NAME);\
			echo "$(TCOL)$(NAME) $(RMD)";\
		fi

re: fclean all

.PHONY: all re clean fclean

.SILENT:
