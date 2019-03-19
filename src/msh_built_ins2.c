/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_built_ins2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbeall <jbeall@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/18 10:17:46 by jbeall            #+#    #+#             */
/*   Updated: 2019/03/18 19:56:24 by jbeall           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*str_char_join(char *str1, char *str2, char join)
{
	char *new;

	new = ft_strnew(ft_strlen(str1) + ft_strlen(str2) + 2);
	ft_strncpy(new, str1, ft_strlen(str1));
	new[ft_strlen(str1)] = join;
	ft_strncat(new, str2, ft_strlen(str2));
	return (new);
}

int		check_var_chars(char *var)
{
	while (*var)
	{
		if (*var == '=')
			return (0);
		var++;
	}
	return (1);
}

int		change_dir_to(char *path)
{
	char *old_pwd;
	char *pwd;

	old_pwd = getcwd(NULL, 0);
	if (chdir(path) >= 0)
	{
		set_env_safe(g_environ, "OLDPWD", old_pwd);
		pwd = getcwd(NULL, 0);
		set_env_safe(g_environ, "PWD", pwd);
		free(pwd);
		free(old_pwd);
		return (0);
	}
	free(old_pwd);
	return (-1);
}

void	change_dir_to_env(char *var)
{
	char *val;

	val = get_env_value(g_environ, var);
	if (val)
	{
		change_dir_to(val);
		free(val);
	}
}

int		msh_cd(char **args)
{
	if (!*args)
		change_dir_to_env("HOME");
	else if (!ft_strcmp(args[0], "-"))
		change_dir_to_env("OLDPWD");
	else
	{
		if (change_dir_to(args[0]) < 0)
		{
			if (access(args[0], F_OK) < 0)
				ft_printf("msh: error : directory does not exist\n");
			else if (access(args[0], X_OK) < 0)
				ft_printf("msh: error : permission denied\n");
			else
				ft_printf("msh: error : directory not accessible\n");
		}
	}
	return (0);
}
