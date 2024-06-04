/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_parser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 13:55:17 by eschussl          #+#    #+#             */

/*   Updated: 2024/05/27 12:33:09 by albokanc         ###   ########.fr       */
/*   Updated: 2024/05/24 15:58:08 by eschussl         ###   ########.fr       */
/*   Updated: 2024/05/26 17:26:45 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// la norme 

int	ms_parser(t_main *main)
{
	t_nodpars	*new;
	t_nodpars	*tmp;
	t_piplex	*one;
	int			res;
	
	new = NULL;
	one = main->lexer.head;
	if (!main->lexer.head->head && !main->lexer.head->next)
		return (1);
	while (one)
	{
		if ((!one->size || (one->size == 1 && one->head->type == space)) && one->next)
			return (ms_printerror("syntax error near unexpected token `|'", "", "", MS_SYNTAX_ERROR));
		else if (!one->size || (one->size == 1 && one->head->type == space))
			return (ms_printerror("syntax error near unexpected token `newline'", "", "", MS_SYNTAX_ERROR));
		new = ft_calloc(sizeof(t_nodpars), 1);
		if (!new)
			ms_exit(main, ms_printerror("Malloc error\n", strerror(errno), "", MS_ERROR_CODE));
		if (!main->parser.head)
			main->parser.head = new;
		else
			tmp->next = new;
		main->parser.size++;
		res = ms_parsing_quotes(main, one);
		ms_parsingdquotes(main, one);
		ms_parsing_env(main, one);
		ms_parsing_spaces(main, one);
		res = ms_parsing_words(main, one, new);
		if (res)
			return (res);
		one = one->next;
		tmp = new;
	}
	//ms_printp(main);
	ms_free_lexer(main);
	return (0);
}
