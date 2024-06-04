/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exec_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 15:52:36 by eschussl          #+#    #+#             */
/*   Updated: 2024/05/26 15:51:07 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ms_check_mod(t_nodred *first)
{
	t_nodred	*tmp;
	
	tmp = first;
	while (tmp)
	{
		if (tmp->mod == 1)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}
int		ms_tabsize(t_nodpars *current)
{
	t_nodargs	*tmp;
	int			tabsize;

	tmp = current->flags;
	tabsize = 0;
	while (tmp)
	{
		tabsize++;
		tmp = tmp->next;
	}
	tmp = current->args;
	while (tmp)
	{
		tabsize++;
		tmp = tmp->next;
	}
	return (tabsize);
}
char	**ms_cmdtab_init(t_main *main, t_nodpars *current, char *cmd)
{
	char **cmdtab;
	
	cmdtab = ft_calloc(ms_tabsize(current) + 2, sizeof(char *));
	if (!cmdtab)
		ms_exit(main, ms_printerror("Malloc error\n", strerror(errno), "", MS_ERROR_CODE));
	cmdtab[0] = ft_strdup(cmd);
	if (!cmdtab[0])
		ms_exit(main, ms_printerror("Malloc error\n", strerror(errno), "", MS_ERROR_CODE));
	return (cmdtab);
}

char	**ms_cmdtab(char *cmd, t_nodpars *current, t_main *main)
{
	t_nodargs	*tmp;
	char		**cmdtab;
	int			i;

	cmdtab = ms_cmdtab_init(main, current, cmd);
	i = 1;
	tmp = current->flags;
	while (tmp)
	{
		cmdtab[i] = ft_strdup(tmp->var);
		if (!cmdtab[i])
			ms_exit(main, ms_printerror("Malloc error\n", strerror(errno), "", MS_ERROR_CODE));
		tmp = tmp->next;
		i++;
	}
	tmp = current->args;
	while (tmp)
	{
		cmdtab[i] = ft_strdup(tmp->var);
		if (!cmdtab[i++])
			ms_exit(main, ms_printerror("Malloc error\n", strerror(errno), "", MS_ERROR_CODE));
		tmp = tmp->next;
	}
	return (cmdtab);
}

char	*ms_cmdpath(char *path, char *cmd)
{
	char *res;
	int	i;
	int	j;

	res = ft_calloc(ft_strlen(path) + ft_strlen(cmd) + 2, sizeof(char));
	if (!res)
		return (NULL);
	i = -1;
	while (path[++i])
		res[i] = path[i];
	res[i++] = '/';
	j = -1;
	while (cmd[++j])
		res[i + j] = cmd[j];
	res[i + j] = 0;
	return (res);
}
