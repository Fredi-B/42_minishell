/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_tokens_dollar.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbechtol <fbechtol@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 15:00:27 by fbechtol          #+#    #+#             */
/*   Updated: 2022/03/04 15:00:28 by fbechtol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_questionmark(t_data *data);

void	ft_dollar(t_data *data, int *fd_pipe, int *i, t_flag *flag)
{
	char	*tmp_str_1;
	int		str_i;

	flag->dollar = ON;
	tmp_str_1 = ft_questionmark(data);
	str_i = 0;
	while (tmp_str_1[str_i] != '\0' && tmp_str_1 != NULL)
	{
		write(*(fd_pipe + 1), &tmp_str_1[str_i], 1);
		(*i)++;
		str_i++;
		flag->word = ON;
	}
	flag->dollar = OFF;
	free(tmp_str_1);
}

static int	ft_forwarddata(t_data *data)
{
	int		str_i;

	str_i = 0;
	while (*data->pipe_segments->tmpargv != '\0' \
		&& *data->pipe_segments->tmpargv != ' ' \
		&& *data->pipe_segments->tmpargv != '$' \
		&& *data->pipe_segments->tmpargv != '<' \
		&& *data->pipe_segments->tmpargv != '>' \
		&& *data->pipe_segments->tmpargv != '/' \
		&& *data->pipe_segments->tmpargv != '\'' \
		&& *data->pipe_segments->tmpargv != '"')
	{
		data->pipe_segments->tmpargv++;
		str_i++;
	}
	data->pipe_segments->tmpargv--;
	return (str_i);
}

static char	*ft_questionmark_2(char *tmp_str_2, t_data *data)
{
	char	*tmp_str_1;
	char	*tmp_str_3;
	char	*tmp_str_4;
	int		str_i;

	if (tmp_str_2[0] == '?')
	{
		tmp_str_3 = ft_substr(tmp_str_2, 1, ft_strlen(tmp_str_2));
		tmp_str_4 = ft_itoa(g_exit_status);
		tmp_str_1 = ft_strjoin(tmp_str_4, tmp_str_3);
		free(tmp_str_3);
		free(tmp_str_4);
		return (tmp_str_1);
	}
	tmp_str_1 = get_folder_long(data, tmp_str_2);
	str_i = ft_strlen(tmp_str_1) + ft_strlen(tmp_str_2);
	if (str_i == 0)
		tmp_str_3 = ft_strdup("$\0");
	else
		tmp_str_3 = ft_strdup(tmp_str_1);
	free(tmp_str_1);
	return (tmp_str_3);
}

static char	*ft_questionmark(t_data *data)
{
	char	*tmp_str_1;
	char	*tmp_str_2;
	int		str_i;

	if (ft_strlen(data->pipe_segments->tmpargv) == 1)
		return (ft_strdup("$\0"));
	data->pipe_segments->tmpargv++;
	tmp_str_1 = ft_strdup(data->pipe_segments->tmpargv);
	str_i = ft_forwarddata(data);
	tmp_str_2 = ft_substr(tmp_str_1, 0, str_i);
	free(tmp_str_1);
	tmp_str_1 = ft_questionmark_2(tmp_str_2, data);
	free(tmp_str_2);
	return (tmp_str_1);
}
