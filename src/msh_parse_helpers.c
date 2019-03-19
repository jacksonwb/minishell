/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_parse_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbeall <jbeall@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/18 10:08:11 by jbeall            #+#    #+#             */
/*   Updated: 2019/03/18 10:23:52 by jbeall           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*str_replace(char *big, char *small, int offset, int seg_len)
{
	char	*new;
	int		len;

	len = (small ? ft_strlen(small) : 0) + ft_strlen(big) - seg_len;
	new = ft_strnew(len);
	new = ft_strncpy(new, big, offset);
	if (small)
		ft_strlcat(new, small, len + 1);
	ft_strlcat(new, big + offset + seg_len, len + 1);
	return (new);
}

int		is_valid_tilde_start(char *str, int i)
{
	return (str[i] == '~' && (str[i + 1] == '/' || !str[i + 1]
				|| str[i + 1] == ' '));
}

int		is_valid_tilde_middle(char *str, int i)
{
	return (str[i] == '~' && (str[i + 1] == '/' || !str[i + 1]
			|| str[i + 1] == ' ') && str[i - 1] == ' ');
}

char	*implode(char *s1, char *s2, char c)
{
	char *new;

	new = ft_strnew(ft_strlen(s1) + ft_strlen(s2) + 1);
	ft_strcpy(new, s1);
	new[ft_strlen(s1)] = c;
	ft_strcat(new, s2);
	return (new);
}

void	print_access_error(char *path)
{
	struct stat stats;
	int			status;

	if ((status = stat(path, &stats)) < 0)
		ft_printf("msh: command not found: %s\n", path);
	else
		ft_printf("msh: permission denied: %s\n", path);
}
