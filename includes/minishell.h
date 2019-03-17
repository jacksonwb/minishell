/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbeall <jbeall@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 17:21:52 by jbeall            #+#    #+#             */
/*   Updated: 2019/03/15 18:50:13 by jbeall           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "libft.h"
#include "ft_printf.h"
#include "get_next_line.h"

#define PURPLE "\x1b[38;5;135m"
#define RESET "\x1b[0m"
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_LEFT "\033[1D"

char		**g_environ;

typedef struct	s_command
{
	char		*name;
	char		**argv;
}				t_command;

typedef struct	s_command_list
{
	int			num_commands;
	t_command 	*commands;
}				t_command_list;

typedef struct	s_replace
{
	int			offset;
	int			seg_len;
	char*		var;
}				t_replace;

/*
** BUILTINS
*/
int msh_cd(char **args);
int msh_echo(char **args);
int msh_env(char **args);
int msh_exit(char **args);
int msh_setenv(char **args);
int msh_unsetenv(char **args);

/*
** PARSE
*/
t_command *msh_parse(char *input);

/*
** HELPERS
*/
char *get_env_value(char **env, char *key);
int get_env_key_index(char **env, char *key);
void free_str_split(char** str_arr);
void change_env_at(char *var, int index);
char *str_replace(char *big, char *small, int offset, int seg_len);

#endif