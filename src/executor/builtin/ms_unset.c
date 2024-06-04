/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albokanc <albokanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 13:34:19 by albokanc          #+#    #+#             */
/*   Updated: 2024/05/28 17:24:03 by albokanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ms_unset_name(t_main *main, char *name)
{
	t_nodenv	*tmp;
	void		*tmp2;

	if (!ft_strncmp(main->envp.head->name, name, ft_strlen(name) + 1))
	{
		free(main->envp.head->name);
		free(main->envp.head->value);
		tmp2 = main->envp.head->next;
		free (main->envp.head);
		main->envp.head = tmp2;
		return ;
	}
	tmp = main->envp.head;
	while (tmp && tmp->next)
	{
		if (!ft_strncmp(tmp->next->name, name, ft_strlen(name) + 1))
		{
			free(tmp->next->name);
			free(tmp->next->value);
			tmp2 = tmp->next->next;
			free (tmp->next);
			tmp->next = tmp2;
			break;
		}
		tmp = tmp->next;
	}
}

int ms_unset_env_var(t_main *main, t_nodargs *args)
{
	t_nodargs *tmp;

	tmp = args;
	while (tmp)
	{
		ms_unset_name(main, tmp->var);	
		tmp = tmp->next;
	}
	return (0);
}

int ms_unset(t_main *main, t_nodpars *current)
{
	if (current->flags)
	{
		printf("No options allowed for this project.\n");
		return (1);
	}
	if (current->args)
	{
		if (ms_unset_env_var(main, current->args))
			return (1);
	}
	return (0);
}