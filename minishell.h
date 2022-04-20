/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbechtol <fbechtol@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 14:57:18 by fbechtol          #+#    #+#             */
/*   Updated: 2022/03/04 15:33:40 by fbechtol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* ----------------------------- Includes ---------------------------------- */

# include "./libft/libft.h"
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <limits.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <signal.h>
# include <termios.h>
# include <readline/readline.h>
# include <readline/history.h>

/* ----------------------------- Defines ----------------------------------- */

# define TRUE 1
# define FALSE 0
# define OK 0
# define ERROR -2
# define INFILE 1
# define HERE_DOC 2
# define READ_FROM_PIPE 3
# define TRUNCAT 5
# define APPEND 6
# define WRITE_IN_PIPE 7
# define READ 0
# define WRITE 1
# define OFF 3
# define ON 4

/* ---------------------- Defines for debugging ---------------------------- */

/* # define dsprintf(expr) printf("\n" #expr ":\n|%s|\n", expr)
# define diprintf(expr) printf("\n" #expr ":\n|%d|\n", expr)
# define dwrite(expr) write(2, "\n" #expr "\n", strlen(#expr) + 2)
 */

/* ----------------------------- Structures -------------------------------- */

typedef struct s_in_out
{
	char			*name;
	char			*here_doc;
	int				mode;
	struct s_in_out	*next;

}				t_in_out;

typedef struct s_token
{
	char			*token;
	char			**splitted_token;
	struct s_token	*next;
	struct s_token	*prev;
}				t_token;

typedef struct s_pipe_segment
{
	char					*tmpargv;
	t_token					*tokens;
	t_token					*t_head;
	int						cnt_cmd_token;
	int						cnt_cmd_pos;
	char					**cmd_arr;
	t_in_out				*ins_and_outs;
	t_in_out				*i_o_head;
	struct s_pipe_segment	*next;
	struct s_pipe_segment	*prev;
}				t_pipe_segment;

typedef struct s_data
{
	char				**paths;
	char				**envp;
	int					cnt;
	t_pipe_segment		*pipe_segments;
	t_pipe_segment		*p_s_head;
	int					fd_pipe_a[2];
	int					fd_pipe_b[2];
	int					pipe_flag_a;
	int					pipe_flag_b;
	struct sigaction	s_ctrl_c;
	struct sigaction	s_ctrl_back_slash;

}				t_data;

typedef struct s_env
{
	char	*env_data;
	char	*val_data;
	size_t	len_data;
}				t_env;

typedef struct s_flag
{
	int				redir;
	int				dob_quot;
	int				sing_quot;
	int				dollar;
	int				word;
}				t_flag;

typedef struct s_split
{
	int	count;
	int	i;
	int	word;
	int	flag;
	int	last;
}				t_split;

extern int	g_exit_status;

/*  ------------------------ Function prototypes --------------------------- */
/*  -------------------------- builtin_main.c ------------------------------ */
int		builtin_main(t_data *data, char **cmd_s);
void	ft_env(t_data *data, int flag);
void	ft_pwd(void);

/*  --------------------------- builtin_cd.c ------------------------------- */
void	ft_cd(t_data *data, char **arg);
char	*get_folder_long(t_data *data, char	*env);
/*  -------------------------- builtin_echo.c ------------------------------- */
void	ft_echo(char **arg);
/*  ------------------------- builtin_export.c ------------------------------ */
int		ft_export(t_data *data, char **arg);
void	ft_split_env2(char *src, t_env *env);
int		ft_find_env(char *src, char *env, size_t len);
void	free_env(t_env *env);
/*  -------------------------- builtin_unset.c ------------------------------ */
void	ft_unset(t_data *data, char **arg);
/*  ---------------------------- err_exit.c --------------------------------- */
void	free_data(t_data *data);
void	err_exit(t_data *data, char *msg, int len);
void	free_two_d_arr(char **arr);
/*  ---------------------------- signals.c ---------------------------------- */
void	sig_int_prompt(int signum);
void	sig_int_here_doc(int signum);
void	prep_signals_for_cmds(t_data *data);
/*  --------------------------- parsing_lexer.c ----------------------------- */
int		lexing(t_data *data, char *read);
/*  ------------------------- parsing_lexer_utils.c ------------------------- */
void	init_pipe_segment(t_pipe_segment *new_segment);
void	init_flag_segment(t_flag *flag);
void	switch_flag(int *i_flag);
/*  ------------------------ parsing_split_pipe.c --------------------------- */
char	**split_pipe(char *read, int symbol);
char	**ft_split_s(char const *s, char **lst, int symbol);
size_t	ft_count(char const *s, int symbol);
char	*ft_substr(char const *s, unsigned int start, size_t len);
size_t	ft_strlen(const char *s);
/*  -------------------- parsing_split_pipe_utils.c ------------------------- */
void	set_word_and_flag(t_split *split);
/*  ------------------------------ parsing.c -------------------------------- */
int		parsing(t_data *data);
/*  ----------------------- parsing_ins_and_outs.c -------------------------- */
void	parsing_ins_and_outs(t_data *data);
/*  -------------------------- parsing_tokens.c ----------------------------- */
int		get_tokens(t_data *data);
void	ft_dollar(t_data *data, int *fd_pipe, int *i, t_flag *flag);
int		write_token(t_data *data, int fd_pipe[2], int *i, t_flag *flag);
/*  ----------------------- parsing_tokens_utils.c -------------------------- */
void	write_token_segment(t_data *data, t_token *new_token);
int		iterate_tmp_argv(t_data *data, t_flag *flag, int *i, int *fd_pipe);
/*  ----------------------- parsing_trim_quotes.c -------------------------- */
void	trim_quotes_in_cmd_arr(char **cmd_arr, t_data *data);
/*  ------------------------- parsing_here_docs.c --------------------------- */
int		parsing_here_docs(t_data *data);
/*  ---------------------- parsing_here_docs_utils.c ------------------------ */
int		dup_stdin_back(int tmp_fd_in);
/*  --------------------------- parsing_cmd_arr.c --------------------------- */
void	parsing_cmd_arr(t_data *data);
/*  ---------------------------- launch_cmds.c ------------------------------ */
void	launch_cmds(t_data *data);
void	execute_cmd(t_data *data);
/*  ------------------------- launch_cmds_utils.c --------------------------- */
void	child_routine(t_data *data);
void	wait_routine(int pid);
/*  -------------------------- launch_pipes.c ------------------------------- */
void	create_pipe(t_data *data);
void	close_read_end(t_data *data);
void	close_pipes(t_data *data);
/*  ------------------------- launch_prep_fd_in.c --------------------------- */
void	prep_fd_in(t_data *data);
/*  ------------------------- launch_prep_fd_out.c -------------------------- */
void	prep_fd_out(t_data *data);
/*  ---------------------------- test_fcts.c -------------------------------- */
void	print_cmd_arr(t_data *data);
void	print_token(t_data *data);
void	print_ins_and_outs(t_data *data);

#endif
