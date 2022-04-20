/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_cmd_arr.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbechtol <fbechtol@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 14:57:23 by fbechtol          #+#    #+#             */
/*   Updated: 2022/03/04 14:57:24 by fbechtol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	split_cmd_tokens(t_data *data);
static void	add_cmd_to_arr(t_data *data);
static char	**split_space(char *s, int symbol);

void	parsing_cmd_arr(t_data *data)
{
	split_cmd_tokens(data);
	data->pipe_segments = data->p_s_head;
	while (data->pipe_segments != NULL)
	{
		data->pipe_segments->cmd_arr = (char **)malloc(sizeof(char *) \
									* (data->pipe_segments->cnt_cmd_token + 1));
		if (!data->pipe_segments->cmd_arr)
			err_exit(data, "Error: malloc **cmd_arr", 23);
		data->pipe_segments->cmd_arr[data->pipe_segments->cnt_cmd_token] = NULL;
		data->pipe_segments->cnt_cmd_pos = 0;
		data->pipe_segments->tokens = data->pipe_segments->t_head;
		while (data->pipe_segments->tokens != NULL)
		{
			if (*data->pipe_segments->tokens->token != '<' \
				&& *data->pipe_segments->tokens->token != '>')
			{
				add_cmd_to_arr(data);
			}
			data->pipe_segments->tokens = data->pipe_segments->tokens->next;
		}
		data->pipe_segments = data->pipe_segments->next;
	}
}

static void	split_cmd_tokens(t_data *data)
{
	int	i;

	data->pipe_segments = data->p_s_head;
	while (data->pipe_segments != NULL)
	{
		data->pipe_segments->cnt_cmd_token = 0;
		data->pipe_segments->tokens = data->pipe_segments->t_head;
		while (data->pipe_segments->tokens != NULL)
		{
			if (*data->pipe_segments->tokens->token != '<' \
				&& *data->pipe_segments->tokens->token != '>')
			{
				data->pipe_segments->tokens->splitted_token = \
					split_space(data->pipe_segments->tokens->token, ' ');
				i = 0;
				while (data->pipe_segments->tokens->splitted_token[i] != NULL)
				{
					data->pipe_segments->cnt_cmd_token++;
					i++;
				}
			}
			data->pipe_segments->tokens = data->pipe_segments->tokens->next;
		}
		data->pipe_segments = data->pipe_segments->next;
	}
}

static char	**split_space(char *s, int symbol)
{
	size_t	count;
	char	**cmd_arr;

	if (!s)
		return (NULL);
	count = ft_count(s, symbol);
	cmd_arr = (char **)malloc(sizeof(char *) * (count + 1));
	if (!cmd_arr)
		return (NULL);
	ft_split_s(s, cmd_arr, symbol);
	return (cmd_arr);
}

static void	add_cmd_to_arr(t_data *data)
{
	int		i;

	i = 0;
	while (data->pipe_segments->tokens->splitted_token[i] != NULL)
	{
		data->pipe_segments->cmd_arr[data->pipe_segments->cnt_cmd_pos] = \
			ft_strtrim(data->pipe_segments->tokens->splitted_token[i], " ");
		data->pipe_segments->cnt_cmd_pos++;
		i++;
	}
	trim_quotes_in_cmd_arr(data->pipe_segments->cmd_arr, data);
}
