/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 16:19:41 by albokanc          #+#    #+#             */
/*   Updated: 2024/06/02 12:00:27 by noda             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_nodenv	*ms_check_node(t_main *main, char *var)
{
	t_nodenv	*tmp; // our actual envp
	t_nodenv	*new; // new node that we add at the end

	
	tmp = main->envp.head;
	while (tmp)
	{
		if (!ft_strncmp(var, tmp->name, ft_strlen(tmp->name))) // check if NAME is already in env
		{
			free(tmp->name);
			free(tmp->value);
			return (tmp);
		}
		tmp = tmp->next;
	}
	tmp = main->envp.head;
	while (tmp && tmp->next)
		tmp = tmp->next;
	new = ft_calloc(sizeof(t_nodenv), 1);
	if (!new)
		ms_exit(main, ms_printerror("Malloc error\n", strerror(errno), "", MS_ERROR_CODE));
	if (main->envp.size > 0)
		tmp->next = new;
	else
		main->envp.head = new;
	main->envp.size++;
	return (new);
}
// export name=value format
// the variable name can contain "_", "=" (not at the begining of NAME) but not other special char
// tu t'amuses trop avec tes codes ascii incompréhensibles, c'est trop chiant a debug xddd
int ms_special_c(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c<= 'Z') || c == '_')
		return (1);
	if (c >= '0' && c <= '9')
		return (2);
	return (0);
}
int	ms_name_format(char *str)
{
	int i;

	i = 1;
	if (str[0] == '=' || ms_special_c(str[0]) != 1)
		return (1);
	while (str[i] && str[i] != '=' && str[i] != ' ')
	{
		if (!ms_special_c(str[i]))
			return (1);
		i++;
	}
	return (0);
}

int	ms_no_equal(char *cmd)
{
	int i;

	i = -1;
	while (cmd[++i])
	{
		if (cmd[i] == '=')
			return (0);
	}
	return (1);
}

int	ms_fake_flag(char *var)
{
	if (var[0] == '-' && var[1] == '-')
	{
		printf("export: --: invalid option\n");
		return (1);
	}
	return (0);
}
int ms_set(char *var, t_main *main)
{
	t_nodenv *new;
	int i;
	
	if (ms_name_format(var))
		return (1);
	new = ms_check_node(main, var);
	i = 0;
	new->name = ms_envp_name(var, &i);
	if (!new->name)
		ms_exit(main, ms_printerror("Malloc error\n", strerror(errno), "", MS_ERROR_CODE));
	if (ms_no_equal(var))
		new->exp = 1;
	else
	{
		new->exp = 0;
		new->value = ft_strdup(&var[i]);
		if (!new->value)
			ms_exit(main, ms_printerror("Malloc error\n", strerror(errno), "", MS_ERROR_CODE));
	}
	return (0);
}

// function to check if name and value have both the right format 
int	ms_env_format(t_main *main, t_nodpars *current)
{
	t_nodargs	*tmp;
	int			err;

	tmp = current->args;
	
	while (tmp)
	{
		if (ms_fake_flag(tmp->var))
			return (2);
		err = ms_set(tmp->var, main);
		if (err && tmp == current->args)
		{
			printf("%s: '%s': not a valid identifier\n", main->parser.head->builtin, tmp->var); 
			if (!tmp->next)
				return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

// When no arguments are given, the results are unspecified
void	ms_export_print(t_main *main)
{
	t_nodenv *tmp;

	tmp = main->envp.head;
	while (tmp)
	{
		if (!tmp->exp)
			printf("export %s=\"%s\"\n", tmp->name, tmp->value);
		else
			printf("export %s\n", tmp->name);
		tmp = tmp->next;
	}
}
int	ms_export(t_main *main, t_nodpars *current)
{
	int	err;
	
	if (current->flags)
	{
		printf("No options allowed for this project.\n");
		return (1);
	}
	if (current->args)
	{
		err = ms_env_format(main, current);
		if (err)
			return (err);
	}
	else
		ms_export_print(main);
	return (0);
}