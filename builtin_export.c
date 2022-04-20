/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbechtol <fbechtol@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 14:56:04 by fbechtol          #+#    #+#             */
/*   Updated: 2022/03/04 14:56:05 by fbechtol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env(t_env *env)
{
	if (env->env_data)
	{
		free(env->env_data);
		env->env_data = NULL;
	}
	if (env->val_data)
	{
		free(env->val_data);
		env->val_data = NULL;
	}
}

void	ft_split_env2(char *src, t_env *env)
{
	char	**split;

	if (!src)
		return ;
	split = ft_split(src, '=');
	env->len_data = ft_strlen(split[0]);
	env->env_data = ft_substr(src, 0, env->len_data);
	if (ft_strlen(src) - env->len_data > 0)
		env->val_data = ft_substr(src, env->len_data + 1, \
						ft_strlen(src) - env->len_data - 1);
	else
		env->val_data = ft_strdup("");
	free_two_d_arr(split);
}

int	ft_find_env(char *src, char *env, size_t len)
{
	t_env	en;

	ft_split_env2(src, &en);
	if (len == en.len_data)
	{
		if (ft_strncmp(env, en.env_data, en.len_data) == OK)
		{
			free_env(&en);
			return (OK);
		}
	}
	free_env(&en);
	return (ERROR);
}

static int	ft_export_2(t_data *data, char **arg)
{
	char	**tmp_str;
	int		i;

	tmp_str = (char **)malloc(sizeof(char *) * (data->cnt + 1));
	if (!tmp_str)
		err_exit(data, "Error: ft_export", 16);
	i = 0;
	while (i < data->cnt - 1)
	{
		tmp_str[i] = ft_strdup(data->envp[i]);
		i++;
	}
	tmp_str[i] = ft_strdup(arg[1]);
	tmp_str[i + 1] = NULL;
	free_two_d_arr(data->envp);
	data->envp = tmp_str;
	data->cnt++;
	return (OK);
}

int	ft_export(t_data *data, char **arg)
{
	int		i;
	t_env	en;

	if (data->pipe_segments->next != NULL || ft_isdigit(arg[1][0]) == 1 \
		|| arg[1][0] == '?')
		return (ERROR);
	ft_split_env2(arg[1], &en);
	if (ft_strlen(en.val_data) == 0 || ft_strlen(en.env_data) == 0)
	{
		free_env(&en);
		return (ERROR);
	}
	i = -1;
	while (++i < data->cnt - 1)
	{
		if (ft_find_env(data->envp[i], en.env_data, en.len_data) == OK)
		{
			free(data->envp[i]);
			data->envp[i] = ft_strdup(arg[1]);
			free_env(&en);
			return (OK);
		}
	}
	free_env(&en);
	return (ft_export_2(data, arg));
}
