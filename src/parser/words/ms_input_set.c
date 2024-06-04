/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_input_set.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 11:39:37 by eschussl          #+#    #+#             */
/*   Updated: 2024/06/04 16:20:38 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
extern int g_signal;

void	ms_build_input_node(t_main *main, t_nodpars *current, char *word, bool value)
{
	t_nodred	*tmp;
	t_nodred	*new;

	tmp = NULL;
	new = ft_calloc(sizeof(t_nodred), 1);
	if (!new)
		ms_exit(main, ms_printerror("Malloc error\n", strerror(errno), "", MS_ERROR_CODE));
	if (!current->input)
		current->input = new;
	else
	{
		tmp = current->input;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	new->word = ft_strdup(word);
	if (!new->word)
		ms_exit(main, ms_printerror("Malloc error\n", strerror(errno), "", MS_ERROR_CODE));
	new->mod = value;
	//new->next = NULL;
}
static void	ms_write_to_here_doc(t_main *main, char *lim, int filetmp)
{
	int inputlen;
	int error;
	char	*line;
	
	(void) main;
	error = 0;
	inputlen = ft_strlen(lim);
	while (1)
	{
		// write(1, "Minishell heredoc> ", 20);
		line = readline("Minishell heredoc> ");
		// if (error == GNL_ERROR)
		// 	ms_exit(main, ms_printerror("GNL error\n", strerror(errno), "", MS_ERROR_CODE));
		if (!line)
		{
			ft_putstr_fd("warning: here-document at line 1 delimited by end-of-file\n", 1);
			return ;
		}
		if (g_signal == SIGINT)
		{
			main->exitcode = 130;
			g_signal = 0;
			break ;
		}
		if (!ft_strncmp(line, lim, inputlen) && ft_strlen(line) == inputlen)
			break;
		ft_putstr_fd(line, filetmp);
		free(line);
	}
	if (line)
		free(line);
}

static void ms_here_doc_write(t_main *main, char *lim)
{
	int		filetmp;
	int		pid;
	
	// signal(SIGINT, ms_sig_handler);
	
	// signal(SIGKILL, ms_sig_handler);
	pid = fork();
	if (pid == -1)
		ms_exit(main, ms_printerror("Fork 1 error\n", strerror(errno), "", MS_FORK_ERROR));
	if (pid)
		signal(SIGINT, ms_sig_handler_hd);
	if (pid == 0)
	{	
		signal(SIGINT, SIG_IGN);
		filetmp = open(MS_HERE_DOC_FILE, O_CREAT | O_RDWR | O_TRUNC, 0600);
		if (filetmp == -1)
			ms_exit(main, ms_printerror("Permission denied\n", strerror(errno), "", MS_PERMISSION));
		ms_write_to_here_doc(main, lim, filetmp);
		close (filetmp);
		exit(0);
	}
	waitpid(pid, NULL, 0);
	signal(SIGINT, ms_sig_handler);
}

static int	ms_here_doc(t_main *main, t_nodlex *tmp, t_nodpars *curpars)
{
	if (tmp->type == here_doc && tmp->next && (tmp->next->type == word || tmp->next->type == dquote || tmp->next->type == squote))
	{
		ms_build_input_node(main, curpars, tmp->next->word, true);
		ms_here_doc_write(main, tmp->next->word);
		tmp->next->type = other;
		return (0);
	}
	else if (tmp->type == here_doc && tmp->next && tmp->next->type == space && tmp->next->next && (tmp->next->next->type == word || tmp->next->next->type == dquote || tmp->next->next->type == squote))
	{
		ms_build_input_node(main, curpars, tmp->next->next->word, true);
		ms_here_doc_write(main, tmp->next->next->word);
		tmp->next->next->type = other;
		return (0);
	}
	else if (tmp->type == here_doc)
		return (1);
	return (0);
}
static int	ms_redirect_input(t_main *main, t_nodlex *tmp, t_nodpars *curpars)
{
	if (tmp->type == input && tmp->next && tmp->next->type == space && tmp->next->next && (tmp->next->next->type == word  || tmp->next->next->type == dquote || tmp->next->next->type == squote))
	{
		ms_build_input_node(main, curpars, tmp->next->next->word, false);
		tmp->next->next->type = other;
	}
	else if (tmp->type == input && tmp->next && (tmp->next->type == word  || tmp->next->type == dquote || tmp->next->type == squote))
	{
		ms_build_input_node(main, curpars, tmp->next->word, false);
		tmp->next->type = other;
	}
	else if (tmp->type == input)
		return (1);
	return (0);
}
int	ms_input_set(t_main *main, t_piplex *curlex, t_nodpars *curpars)
{
	t_nodlex	*tmp;
	
	tmp = curlex->head;
	while (tmp)
	{
		if (ms_redirect_input(main, tmp, curpars) || ms_here_doc(main, tmp, curpars))
			return (1);
		tmp = tmp->next;
	}
	return (0);
}