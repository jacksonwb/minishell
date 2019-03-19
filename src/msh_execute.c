/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_execute.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbeall <jbeall@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/18 10:10:37 by jbeall            #+#    #+#             */
/*   Updated: 2019/03/18 19:52:24 by jbeall           ###   ########.fr       */
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

int		msh_execute(t_command *cmd)
{
	int built_in;

	if (!cmd)
		return (0);
	if ((built_in = get_built_in(cmd->name)) >= 0)
		return (execute_builtin(built_in, cmd->argv + 1));
	return (execute_external(cmd));
	return (0);
}

int		execute_external(t_command *cmd)
{
	char	*path;
	int		status;
	pid_t	pid;

	if (!(path = get_command_path(cmd->argv[0])) && access(path, F_OK) < 0)
	{
		ft_printf("msh: command not found: %s\n", cmd->argv[0]);
		return (-1);
	}
	pid = fork();
	if (!pid)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGTSTP, SIG_DFL);
		if (execve(path, cmd->argv, g_environ) < 0)
			print_access_error(path);
		exit(0);
	}
	else if (pid < 0)
		ft_printf("msh: execution error\n");
	else
		g_pid_child = wait(&status);
	free(path);
	return (0);
}

char	*get_command_path(char *cmd)
{
	char	*path;
	char	**all_paths;
	char	*tmp;
	int		i;

	if (cmd[0] == '/' || cmd[0] == '.' || has_non_trailing_slash(cmd))
		return (ft_strdup(cmd));
	tmp = get_env_value(g_environ, "PATH");
	all_paths = ft_strsplit(tmp, ':');
	free(tmp);
	i = -1;
	while (all_paths && all_paths[++i])
	{
		path = implode(all_paths[i], cmd, '/');
		if (!access(path, F_OK))
		{
			free_str_split(all_paths);
			return (path);
		}
		free(path);
	}
	all_paths ? free_str_split(all_paths) : 0;
	return (NULL);
}

int		get_built_in(char *cmd)
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

int		execute_builtin(int cmd, char **args)
{
	static int (*built_ins[])(char **) = {
		&msh_cd,
		&msh_echo,
		&msh_env,
		&msh_exit,
		&msh_setenv,
		&msh_unsetenv
	};

	return (built_ins[cmd](args));
}
