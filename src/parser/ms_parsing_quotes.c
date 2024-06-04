/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_parsing_quotes.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albokanc <albokanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:52:39 by albokanc          #+#    #+#             */
/*   Updated: 2024/05/21 17:16:32 by albokanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static enum s_tokens	get_token(char c)
{
	if (c == '\'')
		return (squote);
	if (c == '"')
		return (dquote);
	return (-1);
}

static int	ms_check_quotes(t_nodlex *current)
{
	if (current->word[0] == '"' && current->word[ft_strlen(current->word) - 1] != '"')
		return (MS_ERROR_DQUOTES);
	else if (current->word[0] == '\'' && current->word[ft_strlen(current->word) - 1] != '\'')
		return (MS_ERROR_SQUOTES);
	return (0);
}
// enleve les quotes, si '', alors on passe en un autre type histoire de ne pas manipuler le texte si une seule ' ou " dans une string, alors renvoie une erreur"

int	ms_parsing_quotes(t_main *main, t_piplex *current)
{
	t_nodlex	*tmp;
	char		*strtmp;
	int			res;
	
	tmp = current->head;
	while (tmp)
	{
		if (tmp->type == word && (tmp->word[0] == '\'' || tmp->word[0] == 34))
		{
			res = ms_check_quotes(tmp);
			if (res)
				return (res);
			tmp->type = get_token(tmp->word[0]);
			strtmp = ft_substr(tmp->word, 1, ft_strlen(tmp->word) - 2);
			if (!strtmp)
				ms_exit(main, ms_printerror("Malloc error\n", strerror(errno), "", MS_ERROR_CODE));
			free (tmp->word);
			tmp->word = strtmp;
		}
		tmp = tmp->next;
	}
	return (0);
}
