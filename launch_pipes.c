/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_pipes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbechtol <fbechtol@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 14:56:54 by fbechtol          #+#    #+#             */
/*   Updated: 2022/03/04 14:56:55 by fbechtol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	close_write_end(t_data *data);
static int	new_pipe(int new_pipe[2], int *new_flag, \
						int old_pipe[2], int *old_flag);

/* creates pipe if there is a pipe
closes write-end from previous pipe and sets flag on READ if needed */
void	create_pipe(t_data *data)
{
	if (data->pipe_segments->next != NULL)
	{
		if (data->pipe_flag_a == OFF)
		{
			if (new_pipe(data->fd_pipe_a, &data->pipe_flag_a, \
						data->fd_pipe_b, &data->pipe_flag_b) == ERROR)
				err_exit(data, "Error: create pipe", 18);
		}
		else
		{
			if (new_pipe(data->fd_pipe_b, &data->pipe_flag_b, \
						data->fd_pipe_a, &data->pipe_flag_a) == ERROR)
				err_exit(data, "Error: create pipe", 18);
		}
	}
	else
		close_write_end(data);
}

static int	new_pipe(int new_pipe[2], int *new_flag, \
						int old_pipe[2], int *old_flag)
{
	if (*old_flag != OFF)
	{
		close (old_pipe[WRITE]);
		*old_flag = READ;
	}
	*new_flag = WRITE;
	if (pipe(new_pipe) == -1)
		return (ERROR);
	return (OK);
}

static void	close_write_end(t_data *data)
{
	if (data->pipe_flag_a != OFF)
	{
		close (data->fd_pipe_a[WRITE]);
		data->pipe_flag_a = READ;
	}
	if (data->pipe_flag_b != OFF)
	{
		close (data->fd_pipe_b[WRITE]);
		data->pipe_flag_b = READ;
	}
}

void	close_read_end(t_data *data)
{
	if (data->pipe_flag_a == READ)
	{
		close(data->fd_pipe_a[0]);
		data->pipe_flag_a = OFF;
	}
	if (data->pipe_flag_b == READ)
	{
		close(data->fd_pipe_b[0]);
		data->pipe_flag_b = OFF;
	}
}

void	close_pipes(t_data *data)
{
	if (data->pipe_flag_a != OFF)
	{
		close(data->fd_pipe_a[0]);
		close(data->fd_pipe_a[1]);
		data->pipe_flag_a = OFF;
	}
	if (data->pipe_flag_b != OFF)
	{
		close(data->fd_pipe_b[0]);
		close(data->fd_pipe_b[1]);
		data->pipe_flag_b = OFF;
	}
}
