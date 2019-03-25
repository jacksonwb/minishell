# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jackson <jbeall@student.42.us.org>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/03/03 12:06:08 by jbeall            #+#    #+#              #
#    Updated: 2019/03/24 20:39:53 by jackson          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#==================================== GENERAL =================================#

NAME = minishell

FLAGS = -Wall -Wextra -Werror #-Ofast #-fsanitize=address

SRC_DIR = ./src/

#=================================== SOURCES ==================================#

SRC = minishell.c built_ins.c msh_parse.c msh_helpers_str.c msh_built_ins2.c \
msh_parse_helpers.c msh_env_helpers.c msh_execute.c msh_helpers.c

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
COM_STRING  = "$(NAME) compilation successful"
CLEAN_OBJ	= "cleaned $(NAME) objects"
CLEAN_NAME	= "cleaned $(NAME) binary"

#===================================== RULES ==================================#

all: lib obj $(NAME)

$(NAME): $(OBJ)
	@echo "Building Objects..."
	@clang $(FLAGS) $^ $(LIBFT) -o $@
	@echo "$(COM_COLOR) $(COM_STRING) $(NO_COLOR)"

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@clang $(FLAGS) $(INC) -g -c $< -o $@
	

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
	@make -C ./libft all --no-print-directory

libfclean:
	@make -C libft fclean --no-print-directory

libclean:
	@make -C libft clean --no-print-directory

re: fclean all
