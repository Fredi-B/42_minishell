/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_fcts.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbechtol <fbechtol@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 14:56:36 by fbechtol          #+#    #+#             */
/*   Updated: 2022/03/04 14:56:37 by fbechtol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_list(t_data *data);
static void	free_tokens(t_data *data);
static void	free_in_out(t_data *data);

void	free_data(t_data *data)
{
	if (data->paths != NULL)
		free_two_d_arr(data->paths);
	data->pipe_segments = data->p_s_head;
	if (data->pipe_segments != NULL)
		free_list(data);
}

static void	free_list(t_data *data)
{
	t_pipe_segment	*tmp;

	if (data->p_s_head != NULL)
	{
		data->pipe_segments = data->p_s_head;
		while (data->pipe_segments != NULL)
		{
			tmp = data->pipe_segments->next;
			if (data->pipe_segments->cmd_arr != NULL)
				free_two_d_arr(data->pipe_segments->cmd_arr);
			if (data->pipe_segments->tmpargv != NULL)
				free(data->pipe_segments->tmpargv);
			free_tokens(data);
			free_in_out(data);
			free(data->pipe_segments);
			data->pipe_segments = NULL;
			data->pipe_segments = tmp;
		}
	}
}

static void	free_tokens(t_data *data)
{
	t_token	*tmp;

	if (data->pipe_segments->t_head != NULL)
	{
		data->pipe_segments->tokens = data->pipe_segments->t_head;
		while (data->pipe_segments->tokens != NULL)
		{
			tmp = data->pipe_segments->tokens->next;
			free(data->pipe_segments->tokens->token);
			data->pipe_segments->tokens->token = NULL;
			free_two_d_arr(data->pipe_segments->tokens->splitted_token);
			free(data->pipe_segments->tokens);
			data->pipe_segments->tokens = tmp;
		}
	}
}

static void	free_in_out(t_data *data)
{
	t_in_out	*tmp;

	if (data->pipe_segments->i_o_head != NULL)
	{
		data->pipe_segments->ins_and_outs = data->pipe_segments->i_o_head;
		while (data->pipe_segments->ins_and_outs != NULL)
		{
			tmp = data->pipe_segments->ins_and_outs->next;
			free(data->pipe_segments->ins_and_outs->name);
			data->pipe_segments->ins_and_outs->name = NULL;
			if (data->pipe_segments->ins_and_outs->here_doc != NULL)
			{
				free(data->pipe_segments->ins_and_outs->here_doc);
				data->pipe_segments->ins_and_outs->here_doc = NULL;
			}
			free(data->pipe_segments->ins_and_outs);
			data->pipe_segments->ins_and_outs = tmp;
		}
	}
}

void	free_two_d_arr(char **arr)
{
	int	i;

	if (arr == NULL)
		return ;
	i = 0;
	while (arr[i] != NULL)
		i++;
	while (i >= 0)
	{
		free(arr[i]);
		arr[i] = NULL;
		i--;
	}
	free(arr);
	arr = NULL;
}
