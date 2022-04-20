/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_lexer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbechtol <fbechtol@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 14:59:51 by fbechtol          #+#    #+#             */
/*   Updated: 2022/03/04 14:59:52 by fbechtol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_pipe_segments_and_tokens(t_data *data, \
											char **tmp_pipe_segments, int i);
static int	add_pipe_segment(t_data *data, char *tmp_pipe_segment);
static void	write_pipe_segment(t_data *data, t_pipe_segment	*new_segment);

/* splitts input into pipe_segments, stores them in linked list.
Calls get_tokens() for each pipe_segment. */
int	lexing(t_data *data, char *read)
{
	char	**tmp_pipe_segments;
	int		i;

	if (read[0] == '|')
	{
		g_exit_status = 258;
		write(STDERR_FILENO, "Error: empty pipe segment\n", 26);
		return (ERROR);
	}
	tmp_pipe_segments = split_pipe(read, '|');
	i = 0;
	while (tmp_pipe_segments[i] != NULL)
	{
		if (get_pipe_segments_and_tokens(data, tmp_pipe_segments, i) == ERROR)
			return (ERROR);
		i++;
	}
	free_two_d_arr(tmp_pipe_segments);
	return (OK);
}

static int	get_pipe_segments_and_tokens(t_data *data, \
											char **tmp_pipe_segments, int i)
{
	if (add_pipe_segment(data, tmp_pipe_segments[i]) == ERROR)
	{
		write(STDERR_FILENO, "Error: empty pipe segment\n", 26);
		free_two_d_arr(tmp_pipe_segments);
		g_exit_status = 258;
		return (ERROR);
	}
	if (get_tokens(data) == ERROR)
	{
		write(STDERR_FILENO, "Error: syntax redirection\n", 26);
		free_two_d_arr(tmp_pipe_segments);
		g_exit_status = 258;
		return (ERROR);
	}
	return (OK);
}

static int	add_pipe_segment(t_data *data, char *tmp_pipe_segment)
{
	t_pipe_segment	*new_segment;
	char			*tmp_str;

	tmp_str = ft_strtrim(tmp_pipe_segment, " ");
	if (ft_strncmp(tmp_str, "", 1) == 0 \
			|| tmp_pipe_segment == NULL)
		return (ERROR);
	new_segment = (t_pipe_segment *)malloc(sizeof(t_pipe_segment));
	if (!new_segment)
		err_exit(data, "Error: malloc new_segment", 25);
	init_pipe_segment(new_segment);
	new_segment->tmpargv = tmp_str;
	write_pipe_segment(data, new_segment);
	return (OK);
}

static void	write_pipe_segment(t_data *data, t_pipe_segment	*new_segment)
{
	if (data->p_s_head == NULL)
	{
		data->p_s_head = new_segment;
		data->pipe_segments = data->p_s_head;
		data->pipe_segments->prev = NULL;
	}
	else
	{
		data->pipe_segments = data->p_s_head;
		while (data->pipe_segments->next != NULL)
			data->pipe_segments = data->pipe_segments->next;
		data->pipe_segments->next = new_segment;
		new_segment->prev = data->pipe_segments;
		data->pipe_segments = new_segment;
	}
}
