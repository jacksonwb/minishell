/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbeall <jbeall@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 17:57:22 by jbeall            #+#    #+#             */
/*   Updated: 2019/03/15 20:01:32 by jbeall           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*g_built_in_str[] = {
		"cd",
		"echo",
		"env",
		"exit",
		"setenv",
		"unsetenv",
		NULL
};

void free_str_split(char** str_arr)
{
	int i;

	i = 0;
	while (str_arr[i])
		free(str_arr[i++]);
	free(str_arr);
}

int get_env_key_index(char **env, char *key)
{
	int i;
	char **env_var;
	int index;

	i = 0;
	index = -1;
	while (env[i])
	{
		env_var = ft_strsplit(env[i], '=');
		if (env_var && env_var[0] && !ft_strcmp(key, env_var[0]))
			index = i;
		free_str_split(env_var);
		if (index > -1)
			return (index);
		i++;
	}
	return (index);
}

char **dup_str_arr(char** ar)
{
	char **new;
	int count;

	count = 0;
	while (ar[count])
		count++;
	new = (char**)ft_memalloc(sizeof(char*) * (count + 1));
	while (count)
	{
		new[count - 1] = ft_strdup(ar[count - 1]);
		count--;
	}
	return (new);
}

char *get_env_value(char **env, char *key)
{
	int i;
	char **env_var;
	char *val;

	i = 0;
	val = NULL;
	while (env[i])
	{
		env_var = ft_strsplit(env[i], '=');
		if (env_var && env_var[0] && env_var[1] && !ft_strcmp(key, env_var[0]))
			val = ft_strdup(env_var[1]);
		free_str_split(env_var);
		if (val)
			return (val);
			i++;
	}
	return (val);
}

int prompt(void)
{
	ft_printf(PURPLE "(minishell)" RESET " λ ");
	return (1);
}

void print_command(t_command *cmd)
{
	int i;

	ft_printf("CMD: %s\n", cmd->name);
	i = 1;
	ft_printf("ARGS:\n");
	while (cmd->argv[i])
	{
		ft_printf("%s\n", cmd->argv[i]);
		i++;
	}
}

int	get_built_in(char *cmd)
{
	int i;

	i = 0;
	while (g_built_in_str[i])
	{
		if (!ft_strcmp(cmd, g_built_in_str[i]))
			return (i);
		i++;
	}
	return (-1);
}

int execute_builtin(int cmd, char **args)
{
	int (*built_ins[])(char **) =
	{
		&msh_cd,
		&msh_echo,
		&msh_env,
		&msh_exit,
		&msh_setenv,
		&msh_unsetenv
	};

	return(built_ins[cmd](args));
}

char *implode(char *s1, char *s2, char c)
{
	char *new;

	new = ft_strnew(ft_strlen(s1) + ft_strlen(s2) + 1);
	ft_strcpy(new, s1);
	new[ft_strlen(s1)] = c;
	ft_strcat(new, s2);
	return (new);
}

int has_non_trailing_slash(char *str)
{
	int i;
	int count;

	i = 0;
	count = ft_strlen(str);
	while (i < count - 1)
	{
		if (str[i] == '/')
			return (1);
		i++;
	}
	return (0);
}

char *get_command_path(char *cmd)
{
	char *path;
	char **all_paths;
	char *tmp;
	int i;

	if (cmd[0] == '/' || cmd[0] == '.' || has_non_trailing_slash(cmd))
		return (ft_strdup(cmd));
	else
	{
		all_paths = ft_strsplit((tmp = get_env_value(g_environ, "PATH")), ':');
		free(tmp);
		i = 0;
		while (all_paths[i])
		{
			path = implode(all_paths[i], cmd, '/');
			if (!access(path, F_OK))
			{
				free_str_split(all_paths);
				return (path);
			}
			free(path);
			i++;
		}
	}
	free_str_split(all_paths);
	return (NULL);
}

void print_access_error(char *path)
{
	struct stat stats;
	int status;

	if ((status = stat(path, &stats)) < 0)
		ft_printf("msh: command not found: %s\n", path);
	else
		ft_printf("msh: permission denied: %s\n", path);
}

int execute_external(t_command *cmd)
{
	char *path;
	int status;
	pid_t child;
	pid_t pid;

	if (!(path = get_command_path(cmd->argv[0])) && access(path, F_OK) < 0)
	{
		ft_printf("msh: command not found: %s\n", cmd->argv[0]);
		return(-1);
	}
	pid = fork();
	if (!pid)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGTSTP, SIG_DFL);
		if(execve(path, cmd->argv, g_environ) < 0)
			print_access_error(path);
		exit(0);
	}
	else if (pid < 0)
		ft_printf("msh: execution error\n");
	else
		child = wait(&status);
	free(path);
	return (0);
}

void sig_handler(int sig)
{
	if (sig == SIGINT)
	{
		ft_printf("\n");
		prompt();
	}
}

int msh_execute(t_command *cmd)
{
	int built_in;

	if (!cmd)
		return(0);

	if ((built_in = get_built_in(cmd->name)) >= 0)
		return (execute_builtin(built_in, cmd->argv + 1));
	return (execute_external(cmd));
	return(0);
}

void free_cmd(t_command *cmd)
{
	free_str_split(cmd->argv);
	free(cmd);
}

int main(void)
{
	extern char **environ;
	char *input;
	char **tokens;
	t_command *cmd;
	int i;

	g_environ = dup_str_arr(environ);
	signal(SIGTSTP, SIG_IGN);
	signal(SIGINT, &sig_handler);
	while(prompt() && get_next_line(1, &input))
	{
		i = -1;
		if (*input)
		{
			tokens = ft_strsplit(input, ';');
			while (tokens[++i])
			{
				cmd = msh_parse(tokens[i]);
				msh_execute(cmd);
				free_cmd(cmd);
			}
			free_str_split(tokens);
		}
		free(input);
	}
	return (0);
}