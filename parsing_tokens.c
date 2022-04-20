/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbechtol <fbechtol@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 15:02:56 by fbechtol          #+#    #+#             */
/*   Updated: 2022/03/04 15:02:57 by fbechtol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	add_token(t_data *data, int fd_pipe, int i);

int	get_tokens(t_data *data)
{
	int		fd_pipe[2];	
	int		i;
	t_flag	flag;
	char	*tmp_str;

	init_flag_segment(&flag);
	if (pipe(fd_pipe) == -1)
		err_exit(data, "Error: create pipe", 18);
	i = 0;
	tmp_str = data->pipe_segments->tmpargv;
	while (*data->pipe_segments->tmpargv != '\0')
	{
		if (iterate_tmp_argv(data, &flag, &i, fd_pipe) == ERROR)
		{
			data->pipe_segments->tmpargv = tmp_str;
			return (ERROR);
		}
		data->pipe_segments->tmpargv++;
	}
	close(fd_pipe[1]);
	add_token(data, fd_pipe[0], i);
	data->pipe_segments->tmpargv = tmp_str;
	return (OK);
}

int	write_token(t_data *data, int *fd_pipe, int *i, t_flag *flag)
{
	close(*(fd_pipe + 1));
	add_token(data, *fd_pipe, *i);
	if (pipe(fd_pipe) == -1)
		err_exit(data, "Error: create pipe", 20);
	*i = 0;
	if (flag->redir == ON)
	{
		write(*(fd_pipe + 1), data->pipe_segments->tmpargv, 1);
		*i += 1;
		if (*data->pipe_segments->tmpargv == *(data->pipe_segments->tmpargv +1))
		{
			data->pipe_segments->tmpargv++;
			write(*(fd_pipe + 1), data->pipe_segments->tmpargv, 1);
			*i += 1;
		}
		while (*(data->pipe_segments->tmpargv + 1) == ' ')
			data->pipe_segments->tmpargv++;
		if (*(data->pipe_segments->tmpargv + 1) == '\0')
		{
			close(*(fd_pipe + 1));
			close(*(fd_pipe));
			return (ERROR);
		}
	}
	return (OK);
}

static void	add_token(t_data *data, int fd_pipe, int i)
{
	t_token	*new_token;

	if (i == 0)
	{
		close(fd_pipe);
		return ;
	}
	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		err_exit(data, "Error: malloc new_token", 23);
	new_token->token = (char *)malloc(sizeof(char) * i + 1);
	if (!new_token->token)
		err_exit(data, "Error: malloc new_token->token", 30);
	new_token->token[i] = '\0';
	if (read(fd_pipe, new_token->token, i) == -1)
		err_exit(data, "Error: read new_token->token", 28);
	close(fd_pipe);
	write_token_segment(data, new_token);
}
