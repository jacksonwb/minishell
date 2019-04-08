/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbeall <jbeall@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/14 20:18:11 by jbeall            #+#    #+#             */
/*   Updated: 2019/04/08 16:07:37 by jbeall           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_replace	parameter_exp_index(char *str)
{
	int			i;
	t_replace	next;

	i = -1;
	next.offset = -1;
	next.seg_len = 0;
	while (str[++i])
	{
		if (str[i] == '$' && ft_isalnum(str[i + 1]))
		{
			next.offset = i;
			next.seg_len += 1;
			break ;
		}
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
	return (next);
}

t_replace	tilde_exp_index(char *str)
{
	int			i;
	t_replace	next;

	i = -1;
	next.offset = -1;
	next.seg_len = 0;
	while (str[++i])
	{
		if (i == 0)
		{
			if (is_valid_tilde_start(str, i))
			{
				next.offset = i;
				next.seg_len = 1;
				break ;
			}
		}
		else if (is_valid_tilde_middle(str, i))
		{
			next.offset = i;
			next.seg_len = 1;
			break ;
		}
	}
	return (next);
}

void		parameter_exp(t_command *cmd)
{
	t_replace	next;
	char		*tmp;
	char		*tmp2;
	int			i;

	i = 0;
	while (cmd->argv[i])
	{
		while ((next = parameter_exp_index(cmd->argv[i])).offset > -1)
		{
			tmp = cmd->argv[i];
			tmp2 = get_env_value(g_environ, next.var);
			cmd->argv[i] = str_replace(cmd->argv[i], tmp2, next.offset,
				next.seg_len);
			free(tmp);
			free(tmp2);
			free(next.var);
		}
		i++;
	}
}

void		tilde_exp(t_command *cmd)
{
	int			i;
	char		*tmp;
	char		*tmp2;
	t_replace	next;

	i = 0;
	while (cmd->argv[i])
	{
		while ((next = tilde_exp_index(cmd->argv[i])).offset > -1)
		{
			tmp = cmd->argv[i];
			tmp2 = get_env_value(g_environ, "HOME");
			cmd->argv[i] = str_replace(cmd->argv[i], tmp2, next.offset,
				next.seg_len);
			free(tmp);
			free(tmp2);
		}
		++i;
	}
}

t_command	*msh_parse(char *input)
{
	char		**split;
	t_command	*cmd;

	if (!input)
		return (NULL);
	if (!(split = ft_strsplit(input, ' ')))
		return (NULL);
	split = remove_invalid_args(split);
	trim_args(split);
	if (!split[0])
	{
		free(split);
		return (NULL);
	}
	cmd = (t_command*)ft_memalloc(sizeof(t_command));
	cmd->name = split[0];
	cmd->argv = split;
	parameter_exp(cmd);
	tilde_exp(cmd);
	return (cmd);
}
