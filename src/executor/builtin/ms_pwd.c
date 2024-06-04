/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albokanc <albokanc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 16:19:44 by albokanc          #+#    #+#             */
/*   Updated: 2024/06/03 14:47:03 by albokanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// La fonction getcwd() copie le chemin d'accès absolu du répertoire 
// de travail courant dans la chaîne pointée par buf, qui est de longueur size. 
// getcwd() allocates the buuffer dynamically using malloc(3) if buf is NULL

int	ms_pwd(t_nodpars *current)
{
	char *dir;

	if (current->flags)
	{
		printf("%s: %s: invalid option \n", current->builtin, current->flags->var);
		return (2);
	}
	else if (ms_fake_flag(current->args->var))
		return (2);
	dir = getcwd(NULL, 0);
	if (dir == NULL)
	{
		printf("getcwd() error\n");
		if (errno == ERANGE)
		{
			printf("Buffer size is too small.\n");
			return (1);
		}
	}
	else
		printf("%s\n", dir);
	free(dir);
	return (0);
}