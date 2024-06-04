/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 13:32:45 by eschussl          #+#    #+#             */
/*   Updated: 2024/05/23 19:40:55 by albokanc         ###   ########.fr       */

/*   Updated: 2024/05/22 16:33:55 by eschussl         ###   ########.fr       */

/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/*##################################*/
/*                                  */
/*            LIBRARIES            */
/*                                  */
/*##################################*/

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <errno.h>
# include "libft.h"
# include <signal.h>
# include <aio.h>
# include <dirent.h>


/*##################################*/
/*                                  */
/*             DEFINES              */
/*                                  */
/*##################################*/

// Debut de la gestion d'erreur, plus expliquee dans notes_tests/notes
# define MS_HERE_DOC_FILE ".hd"
# define MS_ERROR_CODE 1
# define MS_SYNTAX_ERROR 2
# define MS_DIRECTORY_ERROR 126
# define MS_CMD_NOT_FOUND 127


# define MS_ERROR_DQUOTES 34
# define MS_ERROR_SQUOTES 35
# define MS_ERROR_STDIN	12
# define MS_ERROR_STDOUT 13
# define MS_PIPE_ERROR 22
# define MS_FORK_ERROR 23
# define MS_MISSING_DIR 1
# define MS_PERMISSION 1

# define MS_DUP_ERROR 1
# define MS_EXECVE_ERROR -1
/*##################################*/
/*                                  */
/*            STRUCTURES            */
/*                                  */
/*##################################*/


typedef struct s_exec
{
	int 	*pid;
	int 	**pipefd;
	char	**cmdtab;
	char	**envptab;
	char	*cmdpath;
	char	**path;
}		t_exec;

typedef enum s_tokens
{
	input = 1, 
	here_doc, 
	output,
	append,
	space,
	word, 
	squote, 
	dquote, 
	other
}	t_tokens;

typedef struct s_nodred t_nodred;

// nodes correspondant a chaque input ou output contenues
// dans un pipe, mod modifie input en here_doc et output en append
struct s_nodred
{
	char		*word;
	t_nodred	*next;
	bool		mod;
};

typedef struct s_nodargs t_nodargs;

struct s_nodargs
{
	char		*var;
	t_nodargs	*next;
} ;
// nodes separees par chaque pipes.

typedef struct s_nodpars t_nodpars;

struct s_nodpars
{
	t_nodred 	*input;
	t_nodred	*output;
	char 		*cmd;
	char 		*builtin;
	t_nodargs	*flags;
	t_nodargs	*args;
	int			index;
	t_nodpars	*next;
};

// structure de reference / de controle

typedef struct s_parser
{
	t_nodpars *head;
	int		size;
}	t_parser;

typedef struct s_nodlex	t_nodlex;
typedef struct s_piplex	t_piplex;

struct s_nodlex
{
	int			index;
	char		*word;
	t_tokens	type;
	t_nodlex	*next;
};

// grande liste separee par les pipes

struct s_piplex
{
	int			index;
	t_nodlex	*head;
	int 		size;
	t_piplex	*next;
};

// structure de reference / de controle

typedef struct s_lexer
{
	t_piplex	*head;
	int			size;
}	t_lexer;

// Liste contenant chaque variable d'environemment et ses valeurs;

typedef struct s_nodenv t_nodenv;

struct s_nodenv
{
	char		*name;
	char		*value;
	bool		exp;
	t_nodenv	*next;
};

// structure de reference des variables d'environnement

typedef struct	s_envp
{
	t_nodenv	*head;
	int			size;
}	t_envp;

typedef struct s_main
{
	t_envp			envp;
	t_lexer			lexer;
	t_parser		parser;
	t_exec			exec;
	int				ilex;
	int				exitcode;
	bool			activepipe;
	bool			nofork;
}				t_main;

/*##################################*/
/*                                  */
/*            FUNCTIONS             */
/*                                  */
/*##################################*/


void	ms_routine(t_main *main);

// lexer

t_nodlex	*ms_new_nodlex(t_main *main, t_nodlex *prev, t_piplex *current);
t_piplex	*ms_new_piplex(t_main *main, int index);
t_nodlex	*ms_space(t_main *main, char *str, t_nodlex *prev, t_piplex *current);
t_nodlex	*ms_string(t_main *main, char *str, t_nodlex *prev, t_piplex *current);
t_nodlex 	*ms_onechar(t_main *main, char *str, t_nodlex *prev, t_piplex *current);
t_nodlex	*ms_twochar(t_main *main, char *str, t_nodlex *prev, t_piplex *current);
void		ms_lexer(t_main *main, char *str);
void		ms_nodefill(t_main *main, t_piplex *node, char *str);
int			ms_str_size(char *str);
int			ms_stop_str(char c);

void		ms_printl(t_lexer head); // a enlever a la fin
void		ms_envp(t_main *main, char **envp);
void		ms_envp_tab(t_main *main);
char 		*ms_envp_name(char *str, int *i);

// parsing 

int		ms_parser(t_main *main);
void	ms_parsing_env(t_main *main, t_piplex *current);
int		ms_parsing_quotes(t_main *main, t_piplex *current);
void	ms_parsing_spaces(t_main *main, t_piplex *current);
int		ms_parsing_words(t_main *main, t_piplex *curlex, t_nodpars *curpars);
int		ms_input_set(t_main *main, t_piplex *curlex, t_nodpars *curpars);
int		ms_output_set(t_main *main, t_piplex *curlex, t_nodpars *curpars);
void	ms_cmd_set(t_main *main, t_piplex *curlex, t_nodpars *curpars);
void	ms_args(t_main *main, t_nodlex*current, t_nodpars *curpars);
void	ms_parsingdquotes(t_main *main, t_piplex *current);
void	ms_printp(t_main *main);

// error handling

int		ms_printerror(char *str, char *str1, char *str2, int nbreturn);
void	ms_error(t_main *main, int errorcode);
void	ms_exit(t_main *main, int exit_code);

// free functions 

void	ms_here_doc_del();
void	ms_free_exec(t_main *main);
void	ms_free_parser(t_main *main);
void	ms_free_lexer(t_main *main);
void	ms_free_envp(t_main *main);

// execution 

void 	ms_executor(t_main *main);
void 	ms_builtin(t_main *main, t_nodpars *current, int *input, int *output);
void	ms_pip_init(t_main *main);
int		ms_check_mod(t_nodred *first);
char	**ms_cmdtab(char *cmd, t_nodpars *current, t_main *main);
char	*ms_cmdpath(char *path, char *cmd);
void	ms_path_init(t_main *main);
void	ms_fork(t_main	*main);
void	ms_child(t_main *main, int *input, t_nodpars *current, int *output);
void	ms_pip_close(t_main *main);
int		ms_input(t_nodpars *current, int *input);
int		ms_output(t_nodpars *current, int *output);
void	ms_dup(t_main *main, int *input, int *output);
void	ms_no_pipe(t_main *main);
int		ms_no_pipe_frompath(t_main *main, t_nodpars *current);
int		ms_no_pipe_nopath(t_main *main, t_nodpars *current);
int		ms_fake_flag(char *var);

// builtin 

int	ms_pwd(t_nodpars *current);
int	ms_env(t_main *main, t_nodpars *current);
int	ms_echo(t_nodpars *current);
int ms_cd(t_main *main, t_nodpars *current);
int	ms_export(t_main *main, t_nodpars *current);
int ms_exit_b(t_main *main, t_nodpars *current);
int ms_unset(t_main *main, t_nodpars *current);

// signals 

void	ms_signal_trap(void);
void	ms_sig_handler(int signal);
void	ms_sig_handler_fork(int signal);
void	ms_sig_handler_hd(int signal);



#endif
