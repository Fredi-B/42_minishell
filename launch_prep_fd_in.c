/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_prep_fd_in.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbechtol <fbechtol@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 14:57:00 by fbechtol          #+#    #+#             */
/*   Updated: 2022/03/04 14:57:01 by fbechtol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	dup_infile(t_data *data);
static void	dup_here_doc(t_data *data);
static void	dup_read_pipe(t_data *data);

void	prep_fd_in(t_data *data)
{
	int	mode;

	if (data->pipe_segments->prev != NULL)
		dup_read_pipe(data);
	if (data->pipe_segments->i_o_head != NULL)
	{
		data->pipe_segments->ins_and_outs = data->pipe_segments->i_o_head;
		while (data->pipe_segments->ins_and_outs != NULL)
		{
			mode = data->pipe_segments->ins_and_outs->mode;
			if (mode == INFILE)
				dup_infile(data);
			else if (mode == HERE_DOC)
				dup_here_doc(data);
			data->pipe_segments->ins_and_outs = \
									data->pipe_segments->ins_and_outs->next;
		}
	}
}

static void	dup_infile(t_data *data)
{
	int	fd_infile;

	fd_infile = open(data->pipe_segments->ins_and_outs->name, \
						O_RDONLY, 0777);
	if (fd_infile == -1)
	{
		g_exit_status = 1;
		err_exit(data, "Error: infile: permission denied", 32);
	}
	if (dup2(fd_infile, STDIN_FILENO) == -1)
		err_exit(data, "Error: dup infile to STDIN", 26);
	close(fd_infile);
}

static void	dup_here_doc(t_data *data)
{
	int	fd_here_doc[2];

	if (pipe(fd_here_doc) == -1)
		err_exit(data, "Error: create fd_here_doc", 25);
	write(fd_here_doc[WRITE], data->pipe_segments->ins_and_outs->here_doc, \
			ft_strlen(data->pipe_segments->ins_and_outs->here_doc));
	if (dup2(fd_here_doc[READ], STDIN_FILENO) == -1)
		err_exit(data, "Error: dup here_doc to STDIN", 28);
	close(fd_here_doc[READ]);
	close(fd_here_doc[WRITE]);
}

/* dups and closes read-end 
(write-end got already closed in create_pipe()) */
static void	dup_read_pipe(t_data *data)
{
	if (data->pipe_flag_a == READ)
	{
		if (dup2(data->fd_pipe_a[READ], STDIN_FILENO) == -1)
			err_exit(data, "Error: dup pipe a to STDIN", 26);
		close(data->fd_pipe_a[READ]);
	}
	else
	{
		if (dup2(data->fd_pipe_b[READ], STDIN_FILENO) == -1)
			err_exit(data, "Error: dup pipe b to STDIN", 26);
		close(data->fd_pipe_b[READ]);
	}
}
