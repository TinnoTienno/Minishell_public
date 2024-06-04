/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_printl.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 14:29:10 by eschussl          #+#    #+#             */
/*   Updated: 2024/05/26 18:11:17 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_printl(t_lexer lexer)
{
	t_piplex *tmp;
	t_nodlex *tmp2;

	tmp = lexer.head;
	printf("nombre de process : %d\n", lexer.size);
	while (tmp)
	{
		tmp2 = tmp->head;
		printf("nombre de mots : %d\n", tmp->size);
		while (tmp2)
		{
			printf("word : %s\n", tmp2->word);
			printf("type : %i\n", tmp2->type);
			printf("next : %p\n", tmp2->next);
			printf("-----\n");
			tmp2 = tmp2->next;
		}
		printf("|\n");
		tmp = tmp->next;
	}
	printf("\n");
}