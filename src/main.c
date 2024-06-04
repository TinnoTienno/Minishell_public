/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 13:33:50 by eschussl          #+#    #+#             */

/*   Updated: 2024/05/23 16:08:46 by albokanc         ###   ########.fr       */

/*   Updated: 2024/05/22 15:43:39 by eschussl         ###   ########.fr       */

/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_signal;

void	ms_routine(t_main *main)
{
	char *str;
	int err;

	while (1)
	{
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, ms_sig_handler);
		str = readline("\033[31mMinishell > \e[0m");
		if (!str)
        {
            ft_putstr_fd("exit\n", 1);
            break;
        }
		if (str && *str)
			add_history (str);
		main->lexer.size = 0;
		ms_lexer(main, str);
		err = ms_parser(main);
		if (err)
			ms_error(main, err);
		else
		 	ms_executor(main);
		main->ilex = 0;
		ms_free_exec(main);
		ms_free_parser(main);
		ms_here_doc_del();
		if (g_signal == SIGINT)
		{
			main->exitcode = 130;
			g_signal = 0;
		}
	}
	rl_clear_history();
}

int main(int argc, char **argv, char **envp)
{
	static t_main main;

	(void) argv;
	if (argc != 1)
		return (printf("Too many arguments\n"), 1);
	ms_envp(&main, envp);
	ms_routine(&main);
	ms_free_envp(&main);
	if (g_signal)
		main.exitcode = g_signal;
    return (main.exitcode);
}

