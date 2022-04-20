/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_here_docs.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbechtol <fbechtol@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 14:57:35 by fbechtol          #+#    #+#             */
/*   Updated: 2022/03/04 14:57:36 by fbechtol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	prep_read_here_doc(t_data *data);
static int	read_here_doc(t_data *data);
static int	prep_join_here_doc(t_data *data, char *tmp_next_line, \
								char *limiter, int tmp_fd_in);
static char	*join_here_doc(t_data *data, char *tmp_next_line, char *limiter);

int	parsing_here_docs(t_data *data)
{
	data->pipe_segments = data->p_s_head;
	while (data->pipe_segments != NULL)
	{
		data->pipe_segments->ins_and_outs = data->pipe_segments->i_o_head;
		while (data->pipe_segments->ins_and_outs != NULL)
		{
			if (prep_read_here_doc(data) == ERROR)
				return (ERROR);
			data->pipe_segments->ins_and_outs = \
									data->pipe_segments->ins_and_outs->next;
		}
		data->pipe_segments = data->pipe_segments->next;
	}
	return (OK);
}

static int	prep_read_here_doc(t_data *data)
{
	if (data->pipe_segments->ins_and_outs->mode == HERE_DOC)
	{
		if (*data->pipe_segments->ins_and_outs->name == '\0')
		{
			write(STDERR_FILENO, "Error: syntax here_doc\n", 23);
			g_exit_status = 2;
			return (ERROR);
		}
		data->s_ctrl_c.sa_handler = sig_int_here_doc;
		sigaction(SIGINT, &data->s_ctrl_c, NULL);
		if (read_here_doc(data) == ERROR)
		{
			g_exit_status = 130;
			return (ERROR);
		}
	}
	return (OK);
}

static int	read_here_doc(t_data *data)
{
	char	*tmp_next_line;
	char	*tmp;
	char	*limiter;
	int		i;
	int		tmp_fd_in;

	g_exit_status -= 300;
	limiter = ft_strjoin(data->pipe_segments->ins_and_outs->name, "\n");
	if (!limiter)
		err_exit(data, "Error: ft_strjoin(limiter)", 26);
	tmp_fd_in = dup(STDIN_FILENO);
	tmp = readline("> ");
	if (dup_stdin_back(tmp_fd_in) == ERROR)
		return (ERROR);
	tmp_next_line = ft_strjoin(tmp, "\n");
	free(tmp);
	g_exit_status += 300;
	i = ft_strncmp(tmp_next_line, limiter, ft_strlen(tmp_next_line));
	if (i != 0)
		data->pipe_segments->ins_and_outs->here_doc = ft_strdup(tmp_next_line);
	if (prep_join_here_doc(data, tmp_next_line, limiter, tmp_fd_in) == ERROR)
		return (ERROR);
	close(tmp_fd_in);
	return (OK);
}

static int	prep_join_here_doc(t_data *data, char *tmp_next_line, \
								char *limiter, int tmp_fd_in)
{
	while (ft_strncmp(tmp_next_line, limiter, ft_strlen(tmp_next_line)) != 0)
	{
		g_exit_status -= 300;
		tmp_next_line = join_here_doc(data, tmp_next_line, limiter);
		if (g_exit_status == 130)
		{
			dup2(tmp_fd_in, STDIN_FILENO);
			return (ERROR);
		}
		g_exit_status += 300;
	}
	free(limiter);
	limiter = NULL;
	free(tmp_next_line);
	tmp_next_line = NULL;
	return (OK);
}

static char	*join_here_doc(t_data *data, char *tmp_next_line, char *limiter)
{
	char	*tmp_here_doc;
	char	*tmp;

	free(tmp_next_line);
	tmp_next_line = NULL;
	tmp = readline("> ");
	if (g_exit_status == 130)
		return (ft_strjoin(tmp, "\n"));
	tmp_next_line = ft_strjoin(tmp, "\n");
	free(tmp);
	if (ft_strncmp(tmp_next_line, limiter, ft_strlen(tmp_next_line)) == 0)
		return (tmp_next_line);
	tmp_here_doc = ft_strjoin(data->pipe_segments->ins_and_outs->here_doc, \
								tmp_next_line);
	if (!tmp_here_doc)
		err_exit(data, "Error: ft_strjoin(here_doc)", 27);
	free(data->pipe_segments->ins_and_outs->here_doc);
	data->pipe_segments->ins_and_outs->here_doc = NULL;
	data->pipe_segments->ins_and_outs->here_doc = ft_strdup(tmp_here_doc);
	if (!data->pipe_segments->ins_and_outs->here_doc)
		err_exit(data, "Error: ft_strdup(tmp_here_doc)", 30);
	free(tmp_here_doc);
	tmp_here_doc = NULL;
	return (tmp_next_line);
}
