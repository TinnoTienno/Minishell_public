/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 15:18:51 by eschussl          #+#    #+#             */
/*   Updated: 2024/05/26 17:36:19 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// exit pour les erreurs de malloc 
void	ms_here_doc_del()
{
	if (!access(".hd", F_OK))
		unlink(".hd");
}
void	ms_exit(t_main *main, int exit_code)
{
	ms_free_lexer(main);
	if (main->activepipe == true)
		ms_pip_close(main);
	ms_free_exec(main);
	ms_free_parser(main);
	ms_free_envp(main);
	ms_here_doc_del();
	exit(exit_code);
}

int	ms_printerror(char *str, char *str1, char *str2, int exit_code)
{
	fd_printf(2, "%s%s%s\n", str, str1, str2);
	return (exit_code);
}

void	ms_error(t_main *main, int errorcode)
{
	//char	**error_tab;
	(void) errorcode;

	ms_free_lexer(main);
	if (main->activepipe == true)
		ms_pip_close(main);
	ms_free_exec(main);
	ms_free_parser(main);
	//error_tab = ms_error_tab_build();
	// if (!error_tab)
	// 	printf("Error Malloc in ms_error_tab_build function\n");
	// else
	// 	ms_printerror(errorcode, error_tab);
}