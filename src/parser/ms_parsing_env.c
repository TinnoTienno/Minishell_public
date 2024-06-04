/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_parsing_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albokanc <albokanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:50:10 by albokanc          #+#    #+#             */
/*   Updated: 2024/06/03 14:31:44 by albokanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// check la taille de value en enlevant les espace avant et après

int ms_var_size(char *str)
{
	int	i;
	int	space;
	int	size;

	i = 0;
	space = 0;
	while (str[i] && str[i] == ' ')
	{
		space++;
		i++;
	}
	while (str[i])
		i++;
	size = i;
	i--;
	while(i >= 0 && str[i] == ' ')
	{
		i--;
		space++;
	}
	if (size > space)
		return (size - space);
	else
		return (0);
}

// Imprime la variable sans les espaces autour
char *ms_var_dup(char *str)
{
	char	*res;
	int		size;
	int		i;
	int		j;

	size = ms_var_size(str);
	res = ft_calloc(size + 1, sizeof(char));
	if (!res)
		return (NULL);
	if (!size)
		return (res);
	i = 0;
	j = 0;
	while (str[i] == ' ')
		i++;
	while (j < size)
		res[j++] = str[i++];
	res[size] = 0;
	return (res);
}

// check pour une variable d'environnement derriere le '$'

static char	*ms_env_variable(t_main *main, char *str, t_tokens type)
{
	char		*res;
	t_nodenv	*tmp;
	
	tmp = main->envp.head;
	while (tmp)
	{
		if (ft_strncmp(str, tmp->name, ft_strlen(str)) == 0 && ft_strlen(str) == ft_strlen(tmp->name))
		{
			//printf("str: %s| value : %s | type : %d\n", str, tmp->value, type);
			if (type != dquote)
				res = ms_var_dup(tmp->value);
			else
				res = ft_strdup(tmp->value);
			if (!res)
				ms_exit(main, ms_printerror("Malloc error\n", strerror(errno), "", MS_ERROR_CODE));
			return (res);
		}
		tmp = tmp->next;
	}
	res = ft_strdup("");
	if (!res)
		ms_exit(main, ms_printerror("Malloc error\n", strerror(errno), "", MS_ERROR_CODE));
	return (res);
}
// observe les $ dans des word et les tranforme en variable

void	ms_parsing_env(t_main *main, t_piplex *current)
{
	t_nodlex	*tmp;
	void		*void_tmp;
	t_nodlex	*tmp2;
	char		*strtmp;
	
	tmp = current->head;
	while (tmp)
	{
		if (tmp->type == word && tmp->word[0] == '$' && (!tmp->word[1] || ft_isdigit(tmp->word[1]))
			&& tmp->next && (tmp->next->type == dquote || tmp->next->type == squote))
		{
			free(tmp->word);
			tmp->word = tmp->next->word;
			tmp->type = word;
			void_tmp = tmp->next->next;
			free(tmp->next);
			tmp->next = void_tmp;
			tmp2 = tmp;
			while (tmp2)
			{
				tmp2->index--;
				tmp2 = tmp2->next;
			}
			current->size--;
		}
		else if ((tmp->type == word || tmp->type == dquote || tmp->type == squote) && tmp->word[0] == '$' && !tmp->word[1])
		{
			free(tmp->word);
			tmp->word = ft_strdup("$");
			if (!tmp->word)
				ms_exit(main, ms_printerror("Malloc error\n", strerror(errno), "", MS_ERROR_CODE));
		}
		else if (tmp->type == word && tmp->word[0] == '$' && tmp->word[1] == '?')
		{
			free (tmp->word);
			tmp->word = ft_itoa(main->exitcode);
			if (!tmp->word)
				ms_exit(main, ms_printerror("Malloc error\n", strerror(errno), "", MS_ERROR_CODE));
		}
		else if ((tmp->type == word || tmp->type == dquote) && tmp->word[0] == '$')
		{
			strtmp = ms_env_variable(main, &tmp->word[1], tmp->type);
			free (tmp->word);
			tmp->word = strtmp;
		}
		tmp = tmp->next;
	}
}

