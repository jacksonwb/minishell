/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbeall <jbeall@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/14 20:18:11 by jbeall            #+#    #+#             */
/*   Updated: 2019/03/15 19:40:30 by jbeall           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *str_replace(char *big, char *small, int offset, int seg_len)
{
	char *new;
	int len;

	len = (small ? ft_strlen(small) : 0) + ft_strlen(big) - seg_len;
	new = ft_strnew(len);
	new = ft_strncpy(new, big, offset);
	if (small)
		ft_strlcat(new, small, len + 1);
	ft_strlcat(new, big + offset + seg_len, len + 1);
	return (new);
}

t_replace parameter_exp_index(char *str)
{
	int i;
	t_replace next;

	i = 0;
	next.offset = -1;
	next.seg_len = 0;
	while (str[i])
	{
		if (str[i] == '$' && ft_isalnum(str[i + 1]))
		{
			next.offset = i;
			next.seg_len += 1;
			break;
		}
		i++;
	}
	while (next.offset > -1 && ft_isalnum(str[++i]))
	{
		next.seg_len += 1;
	}
	if (next.offset > -1)
	{
		next.var = ft_strnew(next.seg_len - 1);
		ft_strncpy(next.var, str + next.offset + 1, next.seg_len - 1);
	}
	return(next);
}

t_replace tilde_exp_index(char *str)
{
	int i;
	t_replace next;

	i = 0;
	next.offset = -1;
	next.seg_len = 0;
	while (str[i])
	{
		if (i == 0)
		{
			if (str[i] == '~' && (str[i + 1] == '/' || !str[i + 1] || str[i + 1] == ' '))
			{
				next.offset = i;
				next.seg_len = 1;
				break;
			}
		}
		else if (str[i] == '~' && (str[i + 1] == '/' || !str[i + 1] || str[i + 1] == ' ') && str[i - 1] == ' ')
		{
			next.offset = i;
			next.seg_len = 1;
			break;
		}
		++i;
	}
	return (next);
}

void msh_expand(t_command *cmd)
{
	int i;
	char *tmp;
	char *tmp2;
	t_replace next;

	i = 0;
	while (cmd->argv[i])
	{
		while ((next = parameter_exp_index(cmd->argv[i])).offset > -1)
		{
			tmp = cmd->argv[i];
			cmd->argv[i] = str_replace(cmd->argv[i], (tmp2 = get_env_value(g_environ, next.var)), next.offset, next.seg_len);
			free(tmp);
			free(tmp2);
			free(next.var);
		}
		while ((next = tilde_exp_index(cmd->argv[i])).offset > -1)
		{
			tmp = cmd->argv[i];
			cmd->argv[i] = str_replace(cmd->argv[i], (tmp2 = get_env_value(g_environ, "HOME")), next.offset, next.seg_len);
			free(tmp);
			free(tmp2);
		}
		++i;
	}
}

t_command *msh_parse(char *input)
{
	char **split;
	t_command *cmd;
	if (!input)
		return (NULL);
	if (!(split = ft_strsplit(input, ' ')))
		return (NULL);
	cmd = (t_command*)ft_memalloc(sizeof(t_command));
	cmd->name = split[0];
	cmd->argv = split;
	msh_expand(cmd);
	return (cmd);
}
