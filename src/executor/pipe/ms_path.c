/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_path.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 18:11:57 by eschussl          #+#    #+#             */
/*   Updated: 2024/05/22 14:52:44 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_path_init(t_main *main)
{
	t_nodenv	*tmp;

	tmp = main->envp.head;
	while (tmp)
	{
		if (!ft_strncmp(tmp->name, "PATH", 5))
			break;
		tmp = tmp->next;
	}
	if (!tmp)
		return;
	main->exec.path = ft_split(tmp->value, ':');
	if (!main->exec.path)
		ms_exit(main, ms_printerror("Malloc error\n", strerror(errno), "", MS_ERROR_CODE));
}