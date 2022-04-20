/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbechtol <fbechtol@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 14:56:24 by fbechtol          #+#    #+#             */
/*   Updated: 2022/03/04 14:56:25 by fbechtol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_unset_2(t_data *data, int cnt)
{
	char	**tmp_str;
	int		i;
	int		j;

	tmp_str = (char **)malloc(sizeof(char *) * (data->cnt - 1));
	if (!tmp_str)
		err_exit(data, "Error: ft_export", 16);
	i = 0;
	j = 0;
	while (i < data->cnt - 1)
	{
		if (i != cnt)
		{
			tmp_str[j] = ft_strdup(data->envp[i]);
			j++;
		}
		i++;
	}
	tmp_str[i - 1] = NULL;
	free_two_d_arr(data->envp);
	data->envp = tmp_str;
	data->cnt--;
}

void	ft_unset(t_data *data, char **arg)
{
	int		i;
	int		cnt;
	t_env	env_res;

	if (data->pipe_segments->next != NULL)
		return ;
	if (ft_strlen(arg[1]) == 0)
		return ;
	cnt = -1;
	i = 0;
	while (i < data->cnt - 1)
	{
		ft_split_env2(data->envp[i], &env_res);
		if (ft_strncmp(arg[1], env_res.env_data, env_res.len_data) == OK)
		{
			cnt = i;
		}
		free_env(&env_res);
		i++;
	}
	if (cnt != -1)
		ft_unset_2(data, cnt);
}
