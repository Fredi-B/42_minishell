/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbechtol <fbechtol@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 15:03:06 by fbechtol          #+#    #+#             */
/*   Updated: 2022/03/04 15:03:07 by fbechtol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	parsing_paths_from_envp(t_data *data);

int	parsing(t_data *data)
{
	parsing_paths_from_envp(data);
	parsing_ins_and_outs(data);
	if (parsing_here_docs(data) == ERROR)
		return (ERROR);
	parsing_cmd_arr(data);
	return (OK);
}

static void	parsing_paths_from_envp(t_data *data)
{
	int	i;

	i = 0;
	while (1)
	{
		if (data->envp[i] == NULL)
		{
			data->paths = NULL;
			return ;
		}
		if (ft_strnstr(data->envp[i], "PATH=", 5) != NULL)
		{
			data->paths = ft_split(&data->envp[i][5], ':');
			break ;
		}
		i++;
	}
}
