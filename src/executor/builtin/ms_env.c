/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 16:21:27 by albokanc          #+#    #+#             */
/*   Updated: 2024/06/01 11:46:23 by noda             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ms_env(t_main *main, t_nodpars *current)
{
	t_nodenv *tmp;

	if (current->flags || current->args)
	{
		if (current->flags)
		{
			printf("%s: %s: invalid option \n", current->builtin, current->flags->var);
			return (125);
		}
		else if (current->args->var)
		{
			printf("No arguments allowed for env. Look at mini-shell subject :)\n");
			return (1);
		}
	}
	tmp = main->envp.head;
	while (tmp)
	{
		if (!tmp->exp)
			printf("%s=%s \n", tmp->name, tmp->value);
		tmp = tmp->next;
	}
	return (0);
}