/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_envp.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 14:32:47 by eschussl          #+#    #+#             */

/*   Updated: 2024/05/24 14:27:41 by albokanc         ###   ########.fr       */

/*   Updated: 2024/05/22 15:42:50 by eschussl         ###   ########.fr       */

/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// tranforme le char **envp en liste chainee, afin de la manipuler avec export

char *ms_envp_name(char *str, int *i)
{
	char *res;
	
	while (str[*i] && str[*i] != '=')
		*i += 1;
	*i += 1;
	res = ft_calloc(sizeof(char), *i);
	if (!res)
		return (NULL);
	*i = 0;
	while (str[*i] && str[*i] != '=')
	{
		res[*i] = str[*i];
		*i += 1;
	}
	res[*i] = 0;
	*i += 1;
	return (res);
}
void	ms_envp(t_main *main, char **envp)
{
	t_nodenv	*new;
	t_nodenv	*tmp;
	int			i;
	int			j;
	
	i = -1;
	j = 0;
	while (envp[++i])
	{
		new = ft_calloc(sizeof(t_nodenv), 1);
		if (!new)
			ms_exit(main, ms_printerror("Malloc error\n", strerror(errno), "", MS_ERROR_CODE));
		main->envp.size++;
		if (!main->envp.head)
			main->envp.head = new;
		else
			tmp->next = new;
		new->name = ms_envp_name(envp[i], &j);
		if (!new->name)
			ms_exit(main, ms_printerror("Malloc error\n", strerror(errno), "", MS_ERROR_CODE));
		if (envp[i][j])
		{
			new->value = ft_strdup(&envp[i][j]);
			if (!new->value)
				ms_exit(main, ms_printerror("Malloc error\n", strerror(errno), "", MS_ERROR_CODE));
		}
		else
			new->exp = 1;
		tmp = new;
	}
}

void	ms_fill_envp(char *res, char *first, char *last)
{
	int i;
	int	j;

	i = 0;
	j = 0;
	while (first[i])
	{
		res[i] = first[i];
		i++;
	}
	res[i++] = '=';
	while (last[j])
	{
		res[i + j] = last[j];
		j++;
	}
	res[i + j] = 0;
}

void	ms_envp_tab(t_main *main)
{
	t_nodenv	*tmp;
	int			i;

	tmp = main->envp.head;
	main->exec.envptab = ft_calloc(main->envp.size + 1, sizeof(char *));
	if (!main->exec.envptab)
		ms_exit(main, ms_printerror("Malloc error\n", strerror(errno), "", MS_ERROR_CODE));
	i = 0;
	while (tmp)
	{
		if (!tmp->exp)
		{
			main->exec.envptab[i] = ft_calloc(ft_strlen(tmp->name) + ft_strlen(tmp->value) + 2, sizeof(char));
			if (!main->exec.envptab[i])
				ms_exit(main, ms_printerror("Malloc error\n", strerror(errno), "", MS_ERROR_CODE));
			ms_fill_envp(main->exec.envptab[i], tmp->name, tmp->value);
			i++;
		}
		tmp = tmp->next;
	}
}
