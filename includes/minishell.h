/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbeall <jbeall@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 17:21:52 by jbeall            #+#    #+#             */
/*   Updated: 2019/03/19 11:27:41 by jbeall           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <errno.h>
# include "libft.h"
# include "ft_printf.h"
# include "get_next_line.h"

# define PURPLE "\x1b[38;5;135m"
# define RESET "\x1b[0m"
# define ANSI_COLOR_RED "\x1b[31m"
# define ANSI_LEFT "\033[1D"

char			**g_environ;
pid_t			g_pid_child;
int				g_prompt_flag;

typedef struct	s_command
{
	char		*name;
	char		**argv;
}				t_command;

typedef struct	s_command_list
{
	int			num_commands;
	t_command	*commands;
}				t_command_list;

typedef struct	s_replace
{
	int			offset;
	int			seg_len;
	char		*var;
}				t_replace;

/*
** BUILTINS
*/
int				msh_cd(char **args);
int				msh_echo(char **args);
int				msh_env(char **args);
int				msh_exit(char **args);
int				msh_setenv(char **args);
int				msh_unsetenv(char **args);

/*
** EXECUTE
*/
int				msh_execute(t_command *cmd);
int				execute_external(t_command *cmd);
char			*get_command_path(char *cmd);
int				get_built_in(char *cmd);
int				execute_builtin(int cmd, char **args);

/*
** PARSE
*/
t_command		*msh_parse(char *input);
char			*str_replace(char *big, char *small, int offset, int seg_len);
int				is_valid_tilde_start(char *str, int i);
int				is_valid_tilde_middle(char *str, int i);
char			*implode(char *s1, char *s2, char c);
void			print_access_error(char *path);

/*
** HELPERS
*/
void			free_str_split(char **str_arr);
char			**dup_str_arr(char **ar);
int				has_non_trailing_slash(char *str);
void			free_cmd(t_command *cmd);
char			*get_env_value(char **env, char *key);
int				get_env_key_index(char **env, char *key);
void			change_env_at(char *var, int index);
char			**add_env_var(char **env, char *key, char *value);
char			**remove_env_var_at(char **env, int index);
void			set_env_safe(char **env, char *key, char *val);
char			*str_char_join(char *str1, char *str2, char join);
int				check_var_chars(char *var);
void			print_split_str(char **ar);
int				has_non_whitespace_chars(char *str);
char			**remove_invalid_args(char **ar);
char			*trim_whitespace(char *str);
char			*trim_whitespace(char *str);
void			trim_args(char **ar);

#endif
