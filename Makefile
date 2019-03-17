# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jbeall <jbeall@student.42.us.org>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/03/03 12:06:08 by jbeall            #+#    #+#              #
#    Updated: 2019/03/15 19:18:55 by jbeall           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#==================================== GENERAL =================================#

NAME = minishell

FLAGS = -Wall -Wextra -Werror #-Ofast #-fsanitize=address

SRC_DIR = ./src/

#=================================== SOURCES ==================================#

SRC = minishell.c built_ins.c msh_parse.c

#=================================== OBJECTS ==================================#

OBJ_DIR = .obj/

OBJ = $(addprefix $(OBJ_DIR), $(SRC:.c=.o))

#================================== LIBRARIES =================================#

LIBFT = -L ./libft/ -lft

#=================================== HEADERS ==================================#

INC_DIR = ./includes/

LIB_INC = libft/includes/

INC = -I $(LIB_INC) -I $(INC_DIR)

#==================================== COLORS ==================================#

#COLORS
COM_COLOR   = \033[92m
NO_COLOR    = \033[m

#TEXT
COM_STRING  = "$(NAME) Compilation Successful"
CLEAN_OBJ	= "Cleaned $(NAME) Objects"
CLEAN_NAME	= "Cleaned $(NAME) Binary"

#===================================== RULES ==================================#

all: lib obj $(NAME)

$(NAME): $(OBJ)
	@gcc $(FLAGS) $(LIBFT) -g $^ -o $@
	@echo "$(COM_COLOR)$(COM_STRING)$(NO_COLOR)"

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@gcc $(FLAGS) $(INC) -g -c $< -o $@

obj:
	@mkdir -p $(OBJ_DIR)

clean: libclean
	@rm -rf $(OBJ_DIR)
	@echo "$(COM_COLOR)$(CLEAN_OBJ)$(NO_COLOR)"

fclean: clean libfclean
	@rm -f $(NAME)
	@echo "$(COM_COLOR)$(CLEAN_NAME)$(NO_COLOR)"

#LIBFT

lib:
	@make -C ./libft all

libfclean:
	@make -C libft fclean

libclean:
	@make -C libft clean

re: fclean all
