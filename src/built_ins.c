/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbeall <jbeall@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/13 14:40:47 by jbeall            #+#    #+#             */
/*   Updated: 2019/03/15 19:21:32 by jbeall           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int msh_cd(char **args)
{
	char *pwd;

	if (!*args)
	{
		chdir(get_env_value(g_environ, "HOME"));
		change_env_at(get_env_value(g_environ, "HOME"), get_env_key_index(g_environ, "PWD"));
	}
	else
	{
		if(chdir(args[0]) < 0)
		{
			if (access(args[0], F_OK) < 0)
				ft_printf("msh: error : directory does not exist\n");
			else if(access(args[0], X_OK) < 0)
				ft_printf("msh: error : permission denied\n");
			else
				ft_printf("msh: error : directory not accessible\n");
		}
		else
		{
			change_env_at((pwd = getcwd(NULL, 0)), get_env_key_index(g_environ, "PWD"));
			free(pwd);
		}
	}
	return (0);
}

int msh_echo(char **args)
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

int msh_env(char **args)
{
	int i;

	(void)args;
	i = 0;
	while (g_environ[i])
		ft_printf("%s\n", g_environ[i++]);
	return (0);
}

int msh_exit(char **args)
{
	(void)args;
	exit(0);
}

int	check_var_chars(char *var)
{
	while (*var)
	{
		if (*var == '=')
			return (0);
		var++;
	}
	return (1);
}

char *str_char_join(char *str1, char *str2, char join)
{
	char *new;

	new = ft_strnew(ft_strlen(str1) + ft_strlen(str2) + 2);
	ft_strncpy(new, str1, ft_strlen(str1));
	new[ft_strlen(str1)] = join;
	ft_strncat(new, str2, ft_strlen(str2));
	return (new);
}

void change_env_at(char *var, int index)
{
	char **str_ar;

	str_ar = ft_strsplit(g_environ[index], '=');
	free(g_environ[index]);
	g_environ[index] = str_char_join(str_ar[0], var, '=');
	free_str_split(str_ar);
}

/*
** Creates and returns a copy of the env with the new value added, then frees the old env.
*/
char **add_env_var(char **env, char *key, char *value)
{
	char **new_env;
	int num_vars;
	int i;

	num_vars = 0;
	i = 0;
	while (env[num_vars])
		++num_vars;
	new_env = (char**)ft_memalloc(sizeof(char*) * (num_vars + 2));
	while(i < num_vars)
	{
		new_env[i] = env[i];
		i++;
	}
	new_env[num_vars] = str_char_join(key, value, '=');
	free(g_environ);
	return (new_env);
}

char **remove_env_var_at(char **env, int index)
{
	char **new_env;
	int num_vars;
	int i;

	num_vars = 0;
	i = 0;
	while (env[num_vars])
		++num_vars;
	new_env = (char**)ft_memalloc(sizeof(char*) * num_vars);
	while(i < num_vars - 1)
	{
		new_env[i] = i < index ? env[i] : env[i + 1];
		i++;
	}
	free(g_environ[index]);
	free(g_environ);
	return (new_env);
}

int msh_setenv(char **args)
{
	int index;

	if (!args[0])
		return(msh_env(NULL));
	if (args[0] && !args[1])
	{
		if ((index = get_env_key_index(g_environ, args[0])) > -1)
			change_env_at("", index);
		else if (check_var_chars(args[0]))
			g_environ = add_env_var(g_environ, args[0], "");
	}
	if (args[0] && args[1])
	{
		if ((index = get_env_key_index(g_environ, args[0])) > -1 && check_var_chars(args[1]))
			change_env_at(args[1], index);
		else if (check_var_chars(args[0]) && check_var_chars(args[1]))
			g_environ = add_env_var(g_environ, args[0], args[1]);
	}
	return (0);
}

int msh_unsetenv(char **args)
{
	int index;

	if (args[0] && (index = get_env_key_index(g_environ, args[0])) > -1)
		g_environ = remove_env_var_at(g_environ, index);
	return (0);
}
