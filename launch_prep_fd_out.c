/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_prep_fd_out.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbechtol <fbechtol@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 14:57:05 by fbechtol          #+#    #+#             */
/*   Updated: 2022/03/04 14:57:06 by fbechtol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	dup_outfile(t_data *data, int mode);
static void	dup_write_pipe(t_data *data);

void	prep_fd_out(t_data *data)
{
	int	mode;

	if (data->pipe_segments->next != NULL)
		dup_write_pipe(data);
	if (data->pipe_segments->i_o_head != NULL)
	{
		data->pipe_segments->ins_and_outs = data->pipe_segments->i_o_head;
		while (data->pipe_segments->ins_and_outs != NULL)
		{
			mode = data->pipe_segments->ins_and_outs->mode;
			if (mode == TRUNCAT || mode == APPEND)
				dup_outfile(data, mode);
			data->pipe_segments->ins_and_outs = \
									data->pipe_segments->ins_and_outs->next;
		}
	}
}

static void	dup_outfile(t_data *data, int mode)
{
	int	fd_outfile;

	if (mode == TRUNCAT)
		fd_outfile = open(data->pipe_segments->ins_and_outs->name, \
					O_CREAT | O_WRONLY | O_TRUNC, 0777);
	else
		fd_outfile = open(data->pipe_segments->ins_and_outs->name, \
					O_CREAT | O_WRONLY | O_APPEND, 0777);
	if (fd_outfile == -1)
	{
		g_exit_status = 1;
		err_exit(data, "Error: outfile: permission denied", 33);
	}
	if (dup2(fd_outfile, STDOUT_FILENO) == -1)
		err_exit(data, "Error: redirect STDOUT", 22);
	close(fd_outfile);
}

/* dups read-end and closes both ends 
(fct is in child process only) */
static void	dup_write_pipe(t_data *data)
{
	if (data->pipe_flag_a == WRITE)
	{
		close(data->fd_pipe_a[READ]);
		if (dup2(data->fd_pipe_a[WRITE], STDOUT_FILENO) == -1)
			err_exit(data, "Error: dup pipe to STDOUT", 25);
		close(data->fd_pipe_a[WRITE]);
	}
	else
	{
		close(data->fd_pipe_b[READ]);
		if (dup2(data->fd_pipe_b[WRITE], STDOUT_FILENO) == -1)
			err_exit(data, "Error: dup pipe to STDOUT", 25);
		close(data->fd_pipe_b[WRITE]);
	}
}
