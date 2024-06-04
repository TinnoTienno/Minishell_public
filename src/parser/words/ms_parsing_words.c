/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_parsing_words.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:54:55 by albokanc          #+#    #+#             */
/*   Updated: 2024/06/01 15:34:41 by noda             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int	ms_error_syntax(t_piplex *curlex)
{
	t_nodlex	*tmp;
	
	tmp = curlex->head;
	while (tmp)
	{
		if (tmp->type == input || tmp->type == output || tmp->type == append || tmp->type == here_doc)
		{
			if (!(tmp->next && ((tmp->next->type == word || tmp->next->type == dquote || tmp->next->type == squote) || (tmp->next->type == space && (tmp->next->next->type == dquote || tmp->next->next->type == word || tmp->next->next->type == squote)))))
			{
				if (tmp->next && (tmp->next->type == input || (tmp->next->type == space && tmp->next->next && tmp->next->next->type == input)))
					return (ms_printerror("syntax error near unexpected token `<'", "", "",MS_SYNTAX_ERROR));
				else if (tmp->next && (tmp->next->type == here_doc || (tmp->next->type == space && tmp->next->next && tmp->next->next->type == here_doc)))
					return (ms_printerror("syntax error near unexpected token `<<'", "", "",MS_SYNTAX_ERROR));
				else if (tmp->next && (tmp->next->type == output || (tmp->next->type == space && tmp->next->next && tmp->next->next->type == output)))
					return (ms_printerror("syntax error near unexpected token `>'", "", "",MS_SYNTAX_ERROR));
				else if (tmp->next && (tmp->next->type == append || (tmp->next->type == space && tmp->next->next && tmp->next->next->type == append)))
					return (ms_printerror("syntax error near unexpected token `>>'", "", "",MS_SYNTAX_ERROR));
				else if (curlex->next)
					return (ms_printerror("syntax error near unexpected token `|'", "", "",MS_SYNTAX_ERROR));
				else
					return (ms_printerror("syntax error near unexpected token `newline'", "", "",MS_SYNTAX_ERROR));
			}
		}
		tmp = tmp->next;
	}
	return (0);
}

// curlex = current lexing node ; curpars = current parsing node
// fonction qui definit les mots en tokens

int	ms_parsing_words(t_main *main, t_piplex *curlex, t_nodpars *curpars)
{
	//t_nodlex	*tmp;
	int			res;
	
	res = 0;
	res = ms_error_syntax(curlex);
	res = ms_input_set(main, curlex, curpars);
	if (res)
		return (res);
	res = ms_output_set(main, curlex, curpars);
	if (res)
		return (res);
	ms_cmd_set(main, curlex, curpars);
	return (res);
}