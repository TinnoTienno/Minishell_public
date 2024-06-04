/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_executor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 17:31:01 by albokanc          #+#    #+#             */
/*   Updated: 2024/05/31 14:41:21 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void 	ms_executor(t_main *main)
{
	main->exitcode = 0;
	ms_envp_tab(main);
	ms_path_init(main);
	if (main->parser.size > 1)
		ms_fork(main);
	else if (main->parser.size == 1)
		ms_no_pipe(main);
}