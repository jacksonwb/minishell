/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbeall <jbeall@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/13 14:40:47 by jbeall            #+#    #+#             */
/*   Updated: 2019/03/18 10:22:20 by jbeall           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		msh_echo(char **args)
{
	int i;

	i = 0;
	while (args[i])
	{
		i ? ft_printf(" ") : 0;
		ft_printf("%s", args[i++]);
	}
	ft_printf("\n");
	return (0);
}

int		msh_env(char **args)
{
	int i;

	(void)args;
	i = 0;
	while (g_environ[i])
		ft_printf("%s\n", g_environ[i++]);
	return (0);
}

int		msh_exit(char **args)
{
	(void)args;
	exit(0);
}

int		msh_setenv(char **args)
{
	int index;

	if (!args[0])
		return (msh_env(NULL));
	if (args[0] && !args[1])
	{
		if ((index = get_env_key_index(g_environ, args[0])) > -1)
			change_env_at("", index);
		else if (check_var_chars(args[0]))
			g_environ = add_env_var(g_environ, args[0], "");
	}
	if (args[0] && args[1])
	{
		if ((index = get_env_key_index(g_environ, args[0])) > -1 &&
			check_var_chars(args[1]))
			change_env_at(args[1], index);
		else if (check_var_chars(args[0]) && check_var_chars(args[1]))
			g_environ = add_env_var(g_environ, args[0], args[1]);
	}
	return (0);
}

int		msh_unsetenv(char **args)
{
	int index;

	if (args[0] && (index = get_env_key_index(g_environ, args[0])) > -1)
		g_environ = remove_env_var_at(g_environ, index);
	return (0);
}
