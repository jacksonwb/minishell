/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbeall <jbeall@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/18 20:06:24 by jbeall            #+#    #+#             */
/*   Updated: 2019/03/18 20:12:03 by jbeall           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			has_non_whitespace_chars(char *str)
{
	int i;

	i = -1;
	while (str[++i])
	{
		if (!ft_isspace(str[i]))
			return (1);
	}
	return (0);
}

char		**remove_invalid_args(char **ar)
{
	int		i;
	int		j;
	int		count;
	char	**new;

	i = -1;
	count = 0;
	while (ar[++i])
	{
		if (has_non_whitespace_chars(ar[i]))
			++count;
	}
	if (count != i)
	{
		new = (char**)ft_memalloc(sizeof(char*) * (count + 1));
		i = -1;
		j = 0;
		while (ar[++i])
			if (has_non_whitespace_chars(ar[i]))
				new[j++] = ft_strdup(ar[i]);
		free_str_split(ar);
		return (new);
	}
	return (ar);
}

char		*trim_whitespace(char *str)
{
	int i;
	int j;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (ft_isspace(str[i]))
			i++;
		else
			str[j++] = str[i++];
	}
	str[j] = '\0';
	return (str);
}

void		trim_args(char **ar)
{
	int i;

	i = 0;
	while (ar[i])
	{
		ar[i] = trim_whitespace(ar[i]);
		i++;
	}
}

void		set_env_safe(char **env, char *key, char *val)
{
	if (get_env_key_index(env, key) < 0)
		g_environ = add_env_var(env, key, val);
	else
		change_env_at(val, get_env_key_index(env, key));
}
