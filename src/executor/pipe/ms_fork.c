/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_fork.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 15:05:58 by eschussl          #+#    #+#             */
/*   Updated: 2024/06/03 18:18:14 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_fork_middle(t_main *main, t_nodpars *tmp, int *i)
{
	while (tmp->next)
	{
		main->exec.pid[*i] = fork();
		if (main->exec.pid[*i] == -1)
			ms_exit(main, ms_printerror("Fork 3 error\n", strerror(errno), "", MS_FORK_ERROR));
		if (main->exec.pid[*i] == 0)
			ms_child(main, main->exec.pipefd[*i - 1], tmp, main->exec.pipefd[*i]);
		*i += 1;
		tmp = tmp->next;
	}
}
void	ms_waiting(t_main *main)
{
	int 	j;
	int		i;
	pid_t	pid;
	int status;

	pid = waitpid(-1, &status, 0);
	j = 0;
	while (pid != -1 && j < main->parser.size)
	{
		i = 0;
		while (i < main->parser.size)
		{
			if (main->exec.pid[i] == pid)
			{
				if (i == main->parser.size -1 && status)
					main->exitcode = WEXITSTATUS(status);
				else if (i == main->parser.size)
					main->exitcode = 0;
				j++;
				break ;
			}
			i++;
		}
		pid = waitpid(-1, &status, 0);
	}
}
void	ms_fork(t_main	*main)
{
	int			i;
	int			fake[2];
	t_nodpars	*tmp;
	
	i = 1;
	fake[0] = -1;
	fake[1] = -1;
	ms_pip_init(main);
	signal(SIGQUIT, ms_sig_handler_fork);
	signal(SIGINT, ms_sig_handler_fork);
	main->exec.pid[0] = fork();
	if (main->exec.pid[0] == -1)
		ms_exit(main, ms_printerror("Fork 1 error\n", strerror(errno), "", MS_FORK_ERROR));
	if (main->exec.pid[0] == 0)
		ms_child(main, fake, main->parser.head, main->exec.pipefd[0]);
	ms_fork_middle(main, main->parser.head->next, &i);
	main->exec.pid[i] = fork();
	if (main->exec.pid[i] == -1)
		ms_exit(main, ms_printerror("Fork 2 error\n", strerror(errno), "", MS_FORK_ERROR));
	tmp = main->parser.head;
	while (tmp->next)
		tmp = tmp->next;
	if (main->exec.pid[i] == 0)
		ms_child(main, main->exec.pipefd[i - 1], tmp, fake);
	ms_pip_close(main);
	ms_waiting(main);
}
