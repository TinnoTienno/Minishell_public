/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_signal_trap.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 14:14:37 by albokanc          #+#    #+#             */
/*   Updated: 2024/06/04 16:14:54 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int g_signal;

void	ms_sig_handler_fork(int signal)
{
	if (signal == SIGQUIT)
	{
		ft_putstr_fd("Quit (core dumped)\n", 1);
		g_signal = SIGQUIT;
	}
	if (signal == SIGINT)
	{
		write(1, "\n", 1);
		g_signal = SIGINT;
	}
}

void	ms_sig_handler_hd(int signal)
{
	if (signal == SIGINT)
	{
		ft_putchar_fd('\n', 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_signal = SIGINT;
		//printf("\n");
	}
	// if (signal == SIGQUIT)
	// {
	// 	ft_putstr_fd("warning: here-document at line 1 delimited by end-of-file\n", 1);
	// 	g_signal = SIGQUIT;
	// }
	// // exit de fork
}


void	ms_sig_handler(int signal)
{
	if (signal == SIGINT)
	{
		ft_putchar_fd('\n', 1);
		rl_on_new_line();
		rl_replace_line("", 1);
		rl_redisplay();
		g_signal = SIGINT;
	}
}