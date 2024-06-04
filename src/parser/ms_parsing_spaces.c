/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_parsing_spaces.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albokanc <albokanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:54:14 by albokanc          #+#    #+#             */
/*   Updated: 2024/05/21 17:17:05 by albokanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// supprime les "Non-espaces" entre des mots, en fusionnant les mots ensemble.

static void	ms_join_words(t_main *main, t_nodlex *first, t_piplex *current)
{
	char		*strtmp;
	t_nodlex	*tmp;
	void		*ptmp;
	
	strtmp = ft_strjoin(first->word, first->next->word);
	if (!strtmp)
		ms_exit(main, ms_printerror("Malloc error\n", strerror(errno), "", MS_ERROR_CODE));
	free (first->word);
	first->word = strtmp;
	first->type = word;
	ptmp = first->next->next;
	if (first->next->word)
		free (first->next->word);
	free (first->next);
	first->next = ptmp;
	tmp = first->next;
	while (tmp)
	{
		tmp->index--;
		tmp = tmp->next;
	}
	current->size--;
}
void	ms_parsing_spaces(t_main *main, t_piplex *current)
{
	t_nodlex	*tmp;

	tmp = current->head;
	while (tmp->next)
	{
		if ((tmp->type == word || tmp->type == squote || tmp->type == dquote) && (tmp->next->type == word || tmp->next->type == squote || tmp->next->type == dquote))
			ms_join_words(main, tmp, current);
		else
			tmp = tmp->next;		
	}
}
