# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ajeannin <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/19 18:37:40 by ajeannin          #+#    #+#              #
#    Updated: 2023/06/27 18:21:22 by ajeannin         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

########## V A R I A B L E S ###########

NAME			= philo
INCLUDES		= -Iincludes
SRC_DIR			= srcs/
OBJ_DIR			= obj/
CC				= gcc
CFLAGS			= -Wall -Werror -Wextra



############# S O U R C E S ############

SRC_FILES		= clean_exit	\
				  death			\
				  forks			\
				  handle_error	\
				  init			\
				  life			\
				  philo			\
				  utils			\
				  
SRC				= $(addprefix $(SRC_DIR), $(addsuffix .c, $(SRC_FILES)))
OBJ				= $(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRC_FILES)))



################ R U L E S #############

all				:		$(NAME)

$(NAME)			:		$(OBJ)
					@$(CC) $(FLAGS) $(OBJ) $(INCLUDES) -o $(NAME)
					@echo "Compilation Philosophers: done"
					@cd srcs && ctags -R
					@echo "tags are available"

$(OBJ_DIR)%.o 	:		$(SRC_DIR)%.c
					@mkdir -p $(OBJ_DIR)
					@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean			:
					@rm -rf $(OBJ_DIR)
					@echo "Clean obj_dir: done"

fclean			:		clean
					@rm -rf $(NAME)
					@echo "Fclean Philosohers: done"

re 				: 		fclean all

norme			:
					@echo "\n\nNorme Includes\n"
					@norminette includes
					@echo "\n\nNorme Sources\n"
					@norminette srcs

