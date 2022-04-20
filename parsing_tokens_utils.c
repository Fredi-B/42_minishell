/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_tokens_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbechtol <fbechtol@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 15:00:35 by fbechtol          #+#    #+#             */
/*   Updated: 2022/03/04 15:00:36 by fbechtol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_redirection(t_data *data, int *fd_pipe, int *i, t_flag *flag);
static void	check_flags_and_write(t_data *data, int fd_pipe[2], \
									int *i, t_flag *flag);

void	write_token_segment(t_data *data, t_token *new_token)
{
	new_token->splitted_token = NULL;
	new_token->next = NULL;
	if (data->pipe_segments->t_head == NULL)
	{
		data->pipe_segments->t_head = new_token;
		data->pipe_segments->tokens = data->pipe_segments->t_head;
		data->pipe_segments->tokens->prev = NULL;
	}
	else
	{
		data->pipe_segments->tokens = data->pipe_segments->t_head;
		while (data->pipe_segments->tokens->next != NULL)
			data->pipe_segments->tokens = data->pipe_segments->tokens->next;
		data->pipe_segments->tokens->next = new_token;
		new_token->prev = data->pipe_segments->tokens;
	}
}

static void	set_flag(t_flag *flag, char c, int *fd_pipe, int *i)
{
	if (c == '\'' && flag->dob_quot == OFF)
	{
		if (flag->sing_quot == OFF)
			flag->sing_quot = ON;
		else
			flag->sing_quot = OFF;
	}
	else if (c == '\"' && flag->sing_quot == OFF)
	{
		if (flag->dob_quot == OFF)
			flag->dob_quot = ON;
		else
			flag->dob_quot = OFF;
	}
	write(*(fd_pipe + 1), &c, 1);
	*i += 1;
}

int	iterate_tmp_argv(t_data *data, t_flag *flag, int *i, int *fd_pipe)
{
	if (flag->dob_quot == OFF && flag->sing_quot == OFF && \
		(*data->pipe_segments->tmpargv == '<' \
			|| *data->pipe_segments->tmpargv == '>'))
	{
		if (ft_redirection(data, fd_pipe, i, flag) == ERROR)
			return (ERROR);
	}
	else if (*data->pipe_segments->tmpargv == ' ')
		check_flags_and_write(data, fd_pipe, i, flag);
	else if (*data->pipe_segments->tmpargv == '\"' )
		set_flag(flag, *data->pipe_segments->tmpargv, fd_pipe, i);
	else if (*data->pipe_segments->tmpargv == '\'' )
		set_flag(flag, *data->pipe_segments->tmpargv, fd_pipe, i);
	else if (*data->pipe_segments->tmpargv == '$' && flag->sing_quot == OFF \
			&& flag->redir == OFF)
		ft_dollar(data, fd_pipe, i, flag);
	else
	{
		write(fd_pipe[1], data->pipe_segments->tmpargv, 1);
		*i += 1;
		flag->word = ON;
	}
	return (OK);
}

static int	ft_redirection(t_data *data, int *fd_pipe, int *i, t_flag *flag)
{
	if (flag->redir == ON)
	{
		if (flag->word == OFF)
		{
			close(fd_pipe[1]);
			return (ERROR);
		}
		else
			check_flags_and_write(data, fd_pipe, i, flag);
	}
	flag->redir = ON;
	flag->word = OFF;
	if (write_token(data, fd_pipe, i, flag) == ERROR)
		return (ERROR);
	return (OK);
}

static void	check_flags_and_write(t_data *data, int *fd_pipe, \
									int *i, t_flag *flag)
{
	if (flag->redir == ON)
	{
		flag->redir = OFF;
		flag->word = OFF;
		write_token(data, fd_pipe, i, flag);
	}
	else
	{
		write(*(fd_pipe + 1), data->pipe_segments->tmpargv, 1);
		(*i)++;
	}
}
