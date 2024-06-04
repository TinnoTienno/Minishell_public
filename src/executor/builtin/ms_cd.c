/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albokanc <albokanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 16:19:35 by albokanc          #+#    #+#             */
/*   Updated: 2024/06/03 14:16:49 by albokanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *ms_env_elem(t_main *main, char *elem)
{
	t_nodenv *tmp;

	tmp = main->envp.head;
	while (tmp)
	{
		if (!ft_strncmp(tmp->name, elem, ft_strlen(elem)))
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

void ms_set_env(t_main *main)
{
	t_nodenv *tmp;
	char *old_pwd;
	char *new_pwd;

	old_pwd = ms_env_elem(main, "PWD");
	new_pwd = getcwd(NULL, 0);
	// besoin de protection if new_pwd == NULL
	tmp = main->envp.head;
	while (tmp)
	{
		if (!ft_strncmp(tmp->name, "OLDPWD", 7))
		{
			free(tmp->value);
			tmp->value = old_pwd;
		}
		else if (!ft_strncmp(tmp->name, "PWD", 4))
		{
			tmp->value = ft_strdup(new_pwd);
			if (!tmp->value)
				ms_printerror("Malloc error\n", strerror(errno), "", MS_ERROR_CODE);
		}
		tmp = tmp->next; 
	}
	// free(new_pwd)
}

// cd with only relative or absolute path
// chdir(char *path) change the current working directory

int ms_cd(t_main *main, t_nodpars *current)
{
	if (!current->args || (current->args && current->args->var[0] == '~'
		&& !current->args->var[1]))
	{
		if (chdir(ms_env_elem(main, "HOME")) == -1)
		{
			printf("%s: HOME not set\n", current->builtin);
			return (1);
		}
	}
	else if (current->args && !current->args->next)	
	{
		if (current->args->var[0] == '-' && !current->args->var[1])
			//ms_cd_minus(main);
		if (access(current->args->var, F_OK) == 0 && !opendir(current->args->var))
			return (ms_printerror("cd: ", current->args->var, ": Not a directory", 1));
		if (chdir(current->args->var) == -1)
		{
			printf("cd: %s: %s\n", current->args->var, strerror(errno));
			return (1);
		}
	}
	else
	{
		printf("cd: too many arguments\n");
		return (1);
	}
	ms_set_env(main);
	return (0);
}
