/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_env_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbeall <jbeall@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/18 10:14:18 by jbeall            #+#    #+#             */
/*   Updated: 2019/03/18 20:07:24 by jbeall           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	change_env_at(char *var, int index)
{
	char **str_ar;

	str_ar = ft_strsplit(g_environ[index], '=');
	free(g_environ[index]);
	g_environ[index] = str_char_join(str_ar[0], var, '=');
	free_str_split(str_ar);
}

/*
** Creates and returns a copy of the env with the new value added,
** then frees the old env.
*/

char	**add_env_var(char **env, char *key, char *value)
{
	char	**new_env;
	int		num_vars;
	int		i;

	num_vars = 0;
	i = 0;
	while (env[num_vars])
		++num_vars;
	new_env = (char**)ft_memalloc(sizeof(char*) * (num_vars + 2));
	while (i < num_vars)
	{
		new_env[i] = env[i];
		i++;
	}
	new_env[num_vars] = str_char_join(key, value, '=');
	free(g_environ);
	return (new_env);
}

char	**remove_env_var_at(char **env, int index)
{
	char	**new_env;
	int		num_vars;
	int		i;

	num_vars = 0;
	i = 0;
	while (env[num_vars])
		++num_vars;
	new_env = (char**)ft_memalloc(sizeof(char*) * num_vars);
	while (i < num_vars - 1)
	{
		new_env[i] = i < index ? env[i] : env[i + 1];
		i++;
	}
	free(g_environ[index]);
	free(g_environ);
	return (new_env);
}

int		get_env_key_index(char **env, char *key)
{
	int		i;
	int		index;
	char	**env_var;

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

char	*get_env_value(char **env, char *key)
{
	int		i;
	char	**env_var;
	char	*val;

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
