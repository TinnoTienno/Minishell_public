/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_child.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 15:46:26 by eschussl          #+#    #+#             */
/*   Updated: 2024/06/04 14:21:08 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ms_pipe_red(int *input, t_nodpars *current, int *output)
{
	int err;
	
	err = 0;
	if (current->input)
	{
		if (input[0] != -1)
		{
			close (input[0]);
			close (input[1]);
		}
		err = ms_input(current, input);
	}
	else if (input[0] == -1)
		input[0] = STDIN_FILENO;
	if (err)
		return (err);
	if (current->output)
	{
		if (output[1] != -1)
		{
			close (output[0]);
			close (output[1]);
		}
		err = ms_output(current, output);
	}
	else if (output[1] == -1)
		output[1] = STDOUT_FILENO;
	return (err);
}

void	ms_frompath(t_main *main, t_nodpars *current)
{
	char	*cmd;

	if (current->cmd)
		cmd = current->cmd;
	else
		cmd = current->builtin;
	if (access(cmd, F_OK) == 0)
	{
		if (access(cmd, X_OK) == 0)
		{
			main->exec.cmdtab = ms_cmdtab(cmd, current, main);
			if (!main->exec.cmdtab)
				ms_exit(main, ms_printerror("Malloc error\n", strerror(errno), "", MS_ERROR_CODE));
			ft_puttab(main->exec.cmdtab);
			main->exec.cmdpath = ft_strdup(cmd);
			if (!main->exec.cmdpath)
				ms_exit(main, ms_printerror("Malloc error\n", strerror(errno), "", MS_ERROR_CODE));
			return ;
		}
		else
			ms_exit(main, ms_printerror("1 Permission denied\n", strerror(errno), "", MS_PERMISSION));
	}
	else
	{
		if (opendir(cmd))
			ms_exit(main, ms_printerror(cmd, ": Is a directory", "", MS_DIRECTORY_ERROR));
		ms_exit(main, ms_printerror("No such file or directory\n", strerror(errno), "", MS_MISSING_DIR));
	}
}

static char	*ms_cmd_on(t_main *main, char *cmd)
{
	char	*test1;

	if (access(cmd, F_OK) != 0)
		ms_exit(main, ms_printerror("Command not found", "", "", MS_CMD_NOT_FOUND));
	if (access(cmd, X_OK) != 0)
		ms_exit(main, ms_printerror("2 Permission denied\n", strerror(errno), "", MS_PERMISSION));
	test1 = ft_strdup(cmd);
	if (!test1)
		ms_exit(main, ms_printerror("Malloc error\n", strerror(errno), "", MS_ERROR_CODE));
	return (test1);
}

char	*ms_cmdcheck(t_main *main, char *cmd)
{
	char	*cmdpath;
	int		i;

	i = -1;
	while (main->exec.path && main->exec.path[++i])
	{
		cmdpath = ms_cmdpath(main->exec.path[i], cmd);
		if (!cmdpath)
			ms_exit(main, ms_printerror("Malloc error\n", strerror(errno), "", MS_ERROR_CODE));
		if (access(cmdpath, F_OK) == 0)
		{
			if (access(cmdpath, X_OK) == 0)
				return (cmdpath);
			free(cmdpath);
			ms_exit(main, ms_printerror("Command not found\n", strerror(errno), "", MS_CMD_NOT_FOUND));
		}
		free(cmdpath);
	}
	return (ms_cmd_on(main, cmd));
}

void	ms_no_path(t_main *main, t_nodpars *current)
{
	main->exec.cmdtab = ms_cmdtab(current->cmd, current, main);
	if (!main->exec.cmdtab)
		ms_exit(main, ms_printerror("Malloc error\n", strerror(errno), "", MS_ERROR_CODE));
	main->exec.cmdpath = ms_cmdcheck(main, current->cmd);
}

void	ms_child(t_main *main, int *input, t_nodpars *current, int *output)
{
	int res;
	
	if (current->cmd && !current->cmd[0])
		ms_exit(main, 0);
	res = ms_pipe_red(input, current, output);
	if (res)
		ms_exit(main, res);
	if (!main->parser.head->cmd && !main->parser.head->builtin)
		ms_exit(main, 0);
	if (ft_ispath(current->cmd) || ft_ispath(current->builtin))
		ms_frompath(main, current);
	else if (current->cmd)
		ms_no_path(main, current);
	if (current->builtin)
	{
		ms_builtin(main, current, input, output);
		ms_exit(main, 0);
	}
	ms_dup(main, input, output);
	if (execve(main->exec.cmdpath, main->exec.cmdtab, main->exec.envptab) == -1)
		ms_exit(main, ms_printerror("3 Execve error\n", strerror(errno), "", MS_EXECVE_ERROR));
}
