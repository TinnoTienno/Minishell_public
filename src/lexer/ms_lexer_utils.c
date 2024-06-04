/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_lexer_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albokanc <albokanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 18:30:42 by albokanc          #+#    #+#             */
/*   Updated: 2024/05/21 17:13:19 by albokanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_piplex *ms_new_piplex(t_main *main, int index)
{
	t_piplex *new;
	
	new = ft_calloc(sizeof(t_piplex), 1);
	if (!new)
		ms_exit(main, ms_printerror("Malloc error\n", strerror(errno), "", MS_ERROR_CODE));
	new->index = index;
	main->lexer.size++;
	return (new);
}
t_nodlex	*ms_new_nodlex(t_main *main, t_nodlex *prev, t_piplex *current)
{
	t_nodlex *new;
	
	new = ft_calloc(sizeof(t_nodlex), 1);
	if (!new)
		ms_exit(main, ms_printerror("Malloc error\n", strerror(errno), "", MS_ERROR_CODE));
	if (!current->head)
		new->index = 0;
	else
		new->index = prev->index + 1;
	current->size++;
	return (new);
}

int	ms_stop_str(char c)
{
	if (!c)
		return (1);
	if (c == ' ')
		return (1);
	if (c == '|')
		return (1);
	if (c == '<')
		return (1);
	if (c == '>')
		return (1);
	if (c == '\t')
		return (1);
	if (c == '\n')
		return (1);
	if (c == '$')
		return (1);
	if (c == 34)
		return (1);
	if (c == 39)
		return (1);
	return (0);
}

int	ms_str_size(char *str)
{
	int i;

	i = 0;
	if (str[i] == '$')
	{
		i++;
		while (str[i] && (ft_isalnum(str[i])) && !ft_isdigit(str[1]))
			i++;
		if (ft_isdigit(str[1]) || str[1] == '?')
			return (i + 1);
		return (i);
	}
	if (str[i] == 34)
	{
		i++;
		while (str[i] && str[i] != 34)
			i++;
		return (i + 1);
	}
	if (str[i] == 39)
	{
		i++;
		while (str[i] && str[i] != 39)
			i++;
		return (i + 1);
	}
	else
	{
		while (str[i] && !ms_stop_str(str[i]))
			i++;
		return (i);
	}
}
