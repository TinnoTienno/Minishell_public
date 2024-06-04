/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_parsingdquotes.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 17:39:04 by eschussl          #+#    #+#             */
/*   Updated: 2024/05/31 18:50:24 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_new2(t_main *main, t_nodlex *new1, char *rest)
{
	int			i;
	t_nodlex	*new2;
	
	new2 = ft_calloc(1, sizeof(t_nodlex));
	if (!new2)
		ms_exit(main, ms_printerror("Malloc error\n", strerror(errno), "", MS_ERROR_CODE));
	new2->next = new1->next;
	new1->next = new2;
	new2->type = dquote;
	i = 0;
	while (rest[i])
		i++;
	new2->word = ft_calloc(i + 1, sizeof(char));
	if (!new2->word)
		ms_exit(main, ms_printerror("Malloc error\n", strerror(errno), "", MS_ERROR_CODE));
	i = -1;
	while (rest[++i])
		new2->word[i] = rest[i];
	
}
void	ms_new1(t_main *main, t_nodlex *current)
{
	t_nodlex *new1;
	int	i;
	int	j;

	new1 = ft_calloc(1, sizeof(t_nodlex));
	if (!new1)
		ms_exit(main, ms_printerror("Malloc error\n", strerror(errno), "", MS_ERROR_CODE));
	new1->next = current->next;
	current->next = new1;
	new1->type = dquote;
	i = 0;
	while (current->word[i] != '$')
		i++;
	j = 0;
	while (current->word[i + j] && current->word[i + j] != ' ' && current->word[i + j] != '\'')
		j++;
	new1->word = ft_calloc(j + 1, sizeof(char));
	if (!new1->word)
		ms_exit(main, ms_printerror("Malloc error\n", strerror(errno), "", MS_ERROR_CODE));
	j = 0;
	while (current->word[i + j] && current->word[i + j] != ' ' && current->word[i + j] != '\'')
	{
		new1->word[j] = current->word[i + j];
		j++;
	}
	if (current->word[i + j])
		ms_new2(main, new1, &current->word[i + j]);
}
char *ms_new0(t_main *main, t_nodlex *current)
{
	int i;
	char *res;
	
	i = 0;
	while (current->word[i] != '$')
		i++;
	res = ft_calloc(i + 1, sizeof(char));
	if (!res)
		ms_exit(main, ms_printerror("Malloc error\n", strerror(errno), "", MS_ERROR_CODE));
	i = -1;
	while (current->word[++i] != '$')
		res[i] = current->word[i];
	free (current->word);
	return (res);
}
void	ms_build(t_main *main, t_nodlex *current)
{
	ms_new1(main, current);	
	current->word = ms_new0(main, current);
}

void	ms_parsingdquotes(t_main *main, t_piplex *current)
{
	t_nodlex *tmp;
	int			i;
	
	tmp = current->head;
	while (tmp)
	{
		if (tmp->type == dquote)
		{
			i = 0;
			while (tmp->word[i])
			{
				if (tmp->word[i] == '$')
				{
					ms_build(main, tmp);
					tmp = tmp->next;
					break;
				}
				i++;
			}
		}
		tmp = tmp->next;
	}
}