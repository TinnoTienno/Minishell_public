/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_red.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noda <noda@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 15:20:09 by eschussl          #+#    #+#             */
/*   Updated: 2024/06/01 16:11:40 by noda             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ms_last_infile(t_nodred *current, int *input)
{
	if (access(current->word, F_OK))
		return (ms_printerror("No such file or directory : ", current->word, "", MS_MISSING_DIR));
	else if (access(current->word, R_OK))
		return (ms_printerror("Permission denied ", "", current->word, MS_PERMISSION));
	input[0] = open(current->word, O_RDONLY);
	if (input[0] == -1)
		return (ms_printerror("Permission denied\n", strerror(errno), current->word, MS_PERMISSION));
	return (0);
}
static int	ms_fake_infile(t_nodred *current, int *input)
{
	if (access(current->word, F_OK))
		return (ms_printerror("No such file or directory : ", current->word, "", MS_MISSING_DIR));
	else if (access(current->word, R_OK))
		return (ms_printerror("Permission denied ", "", current->word, MS_PERMISSION));
	input[0] = open(current->word, O_RDONLY);
	if (input[0] == -1)
		return (ms_printerror("Permission denied\n", strerror(errno), current->word, MS_PERMISSION));
	close(input[0]);
	return (0);
}


static int	ms_last_here_doc(int *input)
{
	int		filetmp;
	
	filetmp = open(MS_HERE_DOC_FILE, O_RDONLY);
	if (filetmp == -1)
		return (ms_printerror("Permission denied: ", MS_HERE_DOC_FILE, "", MS_PERMISSION));
	input[0] = filetmp;
	return (0);
}

static int	ms_input(t_nodpars *current, int *input)
{
	t_nodred	*tmp;
	int			err;

	tmp = current->input;
	err = 0;
	while (tmp)
	{
		if (tmp->next && !tmp->mod)
			err = ms_fake_infile(tmp, input);
		else if (tmp->mod == 1 && !tmp->next)
			err = ms_last_here_doc(input);
		else if (!tmp->next)
			err = ms_last_infile(tmp, input);
		if (err)
			return (err);
		tmp = tmp->next;
	}
	return (err);
}

static int	ms_output(t_nodpars *current, int *output)
{
	t_nodred	*tmp;
	
	tmp = current->output;
	while (tmp)
	{
		if (!tmp->mod)
			output[1] = open(tmp->word, O_CREAT| O_WRONLY | O_TRUNC, 0644);
		else
			output[1] = open(tmp->word, O_CREAT | O_RDWR | O_APPEND, 0644);
		if (output[1] == -1)
			return (ms_printerror("Permission denied\n", strerror(errno), tmp->word, MS_PERMISSION));
		if (tmp->next)
			close (output[1]);
		tmp = tmp->next;
	}
	return (0);
}

