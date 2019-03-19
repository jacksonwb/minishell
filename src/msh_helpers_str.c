/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_helpers1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbeall <jbeall@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/18 10:04:24 by jbeall            #+#    #+#             */
/*   Updated: 2019/03/18 10:06:15 by jbeall           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_str_split(char **str_arr)
{
	int i;

	i = 0;
	while (str_arr[i])
		free(str_arr[i++]);
	free(str_arr);
}

char	**dup_str_arr(char **ar)
{
	char	**new;
	int		count;

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

void	print_command(t_command *cmd)
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

int		has_non_trailing_slash(char *str)
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

void	free_cmd(t_command *cmd)
{
	free_str_split(cmd->argv);
	free(cmd);
}
