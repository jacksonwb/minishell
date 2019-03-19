/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbeall <jbeall@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 17:57:22 by jbeall            #+#    #+#             */
/*   Updated: 2019/03/19 11:32:02 by jbeall           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		prompt(void)
{
	ft_printf(PURPLE "(minishell)" RESET " λ ");
	return (1);
}

void	sig_handler(int sig)
{
	if (sig == SIGINT)
	{
		ft_printf("\n");
		if (g_pid_child)
			kill(g_pid_child, SIGINT);
		g_pid_child = 0;
		if (g_prompt_flag)
			prompt();
	}
}

void	init_msh(void)
{
	extern char			**environ;
	struct sigaction	interrupt;

	g_environ = dup_str_arr(environ);
	g_pid_child = 0;
	g_prompt_flag = 0;
	interrupt.sa_handler = &sig_handler;
	interrupt.sa_flags = SA_RESTART;
	signal(SIGTSTP, SIG_IGN);
	sigaction(SIGINT, &interrupt, NULL);
}

void	print_split_str(char **ar)
{
	int i;

	i = 0;
	while (ar[i])
	{
		ft_printf("[%s]\n", ar[i]);
		i++;
	}
}

int		main(void)
{
	char		*input;
	char		**tokens;
	t_command	*cmd;
	int			i;

	init_msh();
	while (prompt() && (g_prompt_flag = 1) && get_next_line(1, &input))
	{
		g_prompt_flag = 0;
		i = -1;
		if (input && *input)
		{
			tokens = ft_strsplit(input, ';');
			while (tokens[++i])
			{
				cmd = msh_parse(tokens[i]);
				msh_execute(cmd);
				cmd ? free_cmd(cmd) : 0;
			}
			free_str_split(tokens);
		}
		free(input);
	}
	return (0);
}
