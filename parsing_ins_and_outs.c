/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_ins_and_outs.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbechtol <fbechtol@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 14:59:44 by fbechtol          #+#    #+#             */
/*   Updated: 2022/03/04 14:59:45 by fbechtol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_mode(t_data *data, t_in_out *new_in_or_out);
static void	add_in_out_node(t_data *data);

void	parsing_ins_and_outs(t_data *data)
{
	data->pipe_segments = data->p_s_head;
	while (data->pipe_segments != NULL)
	{
		data->pipe_segments->tokens = data->pipe_segments->t_head;
		while (data->pipe_segments->tokens != NULL)
		{
			if (*data->pipe_segments->tokens->token == '<' \
				|| *data->pipe_segments->tokens->token == '>')
				add_in_out_node(data);
			data->pipe_segments->tokens = data->pipe_segments->tokens->next;
		}
		data->pipe_segments = data->pipe_segments->next;
	}
}

static void	add_in_out_node(t_data *data)
{
	t_in_out	*new_in_or_out;

	new_in_or_out = (t_in_out *)malloc(sizeof(t_in_out));
	if (!new_in_or_out)
		err_exit(data, "Error: malloc new_in_or_out", 28);
	new_in_or_out->here_doc = NULL;
	new_in_or_out->next = NULL;
	set_mode(data, new_in_or_out);
	new_in_or_out->name = ft_strtrim(data->pipe_segments->tokens->token, "> <");
	if (data->pipe_segments->i_o_head == NULL)
	{
		data->pipe_segments->i_o_head = new_in_or_out;
		data->pipe_segments->ins_and_outs = data->pipe_segments->i_o_head;
	}
	else
	{
		data->pipe_segments->ins_and_outs = data->pipe_segments->i_o_head;
		while (data->pipe_segments->ins_and_outs->next != NULL)
			data->pipe_segments->ins_and_outs = \
									data->pipe_segments->ins_and_outs->next;
		data->pipe_segments->ins_and_outs->next = new_in_or_out;
	}
}

static void	set_mode(t_data *data, t_in_out *new_in_or_out)
{
	if (*data->pipe_segments->tokens->token == '<')
	{
		if (*data->pipe_segments->tokens->token == \
								*(data->pipe_segments->tokens->token + 1))
			new_in_or_out->mode = HERE_DOC;
		else
			new_in_or_out->mode = INFILE;
	}
	else if (*data->pipe_segments->tokens->token == '>')
	{
		if (*data->pipe_segments->tokens->token == \
								*(data->pipe_segments->tokens->token + 1))
			new_in_or_out->mode = APPEND;
		else
			new_in_or_out->mode = TRUNCAT;
	}
}
