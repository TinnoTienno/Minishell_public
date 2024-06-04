/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_printp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 13:28:50 by eschussl          #+#    #+#             */
/*   Updated: 2024/05/31 18:49:40 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_print_in_out(t_nodred *head)
{
	t_nodred *tmp;
	int			i = 1;

	tmp = head;
	while (tmp)
	{
		printf("%d - word : %s | mod : %d\n", i++, tmp->word, tmp->mod);
		tmp = tmp->next;
	}
}
void	ms_print_var(t_nodargs *head)
{
	t_nodargs	*tmp;

	tmp = head;
	while (tmp)
	{
		printf("var : |%s|\n", tmp->var);
		tmp = tmp->next;
	}
}

void	ms_printp(t_main *main)
{
	t_nodpars	*tmp;

	tmp = main->parser.head;
	printf("nb of nodes : %d\n\n", main->parser.size);
	while (tmp)
	{
		printf("\n");
		printf("|new node| \n-----------------\n");
		if (tmp->cmd)
			printf("cmd : %s\n", tmp->cmd);
		if (tmp->builtin)
			printf("builtin : %s\n", tmp->builtin);
		if (tmp->flags)
		{
			printf("flags :\n");
			ms_print_var(tmp->flags);
		}
		if (tmp->input)
		{
			printf("input :\n");
			ms_print_in_out(tmp->input);
		}
		if (tmp->output)
		{
			printf("output :\n");
			ms_print_in_out(tmp->output);
		}
		if (tmp->args)
		{
			printf("args :\n");
			ms_print_var(tmp->args);
		}
		printf("\n-------------------------------------------------------\n");
		tmp = tmp->next;
	}
}