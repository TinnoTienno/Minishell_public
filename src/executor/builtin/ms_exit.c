/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albokanc <albokanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 18:51:26 by albokanc          #+#    #+#             */
/*   Updated: 2024/05/28 13:26:50 by albokanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// If n is specified, but its value is not between 0 
// and  255  inclusively, the exit status is undefined.

void ms_clean_and_exit(t_main *main, int status)
{ 
	ms_free_exec(main);
	ms_free_parser(main);
	ms_here_doc_del();
	ms_free_envp(main);
	printf("exit\n");
	exit(status);
}

int ms_exit_args(t_main *main, t_nodpars *current)
{
	unsigned char status;
	int i;

	i = 0;
	status = 0;
	if (!current->args)
		return (0);
	if (current->args->var[i] == '+')
		i++;
	while (current->args->var[i] && (ft_isdigit(current->args->var[i])))
		i++;
	if (current->args->var[i])
		return (printf("exit: %s: numeric argument required\n", current->args->var), 1);
	if (current->args->next)
		return (printf("exit : too many arguments\n"), 1);
	status = ft_atoi(current->args->var);
	ms_clean_and_exit(main, status);
	return (0);
}
int ms_exit_flags(t_main *main, t_nodpars *current)
{
	unsigned char status;
	int i;

	i = 1;
	status = 0;
	if (!current->flags)
		return (0);
	while (current->flags->var[i] && ft_isdigit(current->flags->var[i]))
		i++;
	if (current->flags->var[i])
		return (printf("exit: %s: numeric argument required\n", current->flags->var), 1);
	if (current->flags->next)
		return (printf("exit : too many arguments\n"), 1);
	status = ft_atoi(current->flags->var);
	ms_clean_and_exit(main, status);
	return (0);
}
// The behavior of exit when given an invalid argument or unknown option
// is unspecified, because of differing practices in the various 
// historical implementations.

int ms_exit_b(t_main *main, t_nodpars *current)
{
	if (current->args || current->flags)
	{
		if (current->args && current->flags)
		{
			printf("too many arguments.\n");
			return (1);
		}
		if (ms_exit_args(main, current))
			return (1);
		if (ms_exit_flags(main, current))
			return (1);
	}
	else if (!current->flags && !current->args)
		ms_clean_and_exit(main, main->exitcode);
	return (0);
}