/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbechtol <fbechtol@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 14:55:03 by fbechtol          #+#    #+#             */
/*   Updated: 2022/03/04 14:55:04 by fbechtol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_folder_long(t_data *data, char	*env)
{
	t_env	env_res;
	int		len;
	int		i;
	char	*tmp_str;

	len = ft_strlen(env);
	if (len == 0)
		return (ft_strdup(""));
	i = 0;
	while (i < data->cnt - 1)
	{
		if (ft_find_env(data->envp[i], env, len) == OK)
		{
			ft_split_env2(data->envp[i], &env_res);
			tmp_str = ft_strdup(env_res.val_data);
			free_env(&env_res);
			return (tmp_str);
		}
		i++;
	}
	return (ft_strdup(""));
}

static void	export_folder_short(t_data *data, char *folder, char *env)
{
	char	**arg;
	char	*tmp;

	arg = (char **) malloc (3 * sizeof(char *));
	if (!arg)
		err_exit(data, "Error: ft_bultin_cd", 19);
	arg[0] = ft_strdup("export");
	tmp = ft_strjoin(env, "=");
	arg[1] = ft_strjoin(tmp, folder);
	free(tmp);
	arg[2] = NULL;
	ft_export(data, arg);
	free_two_d_arr(arg);
}

static void	clean_folder(t_data *data, char *goal, char *current)
{
	int		ret;

	ret = chdir(goal);
	if (ret == OK)
	{
		free(goal);
		goal = getcwd(NULL, 0);
		export_folder_short(data, goal, "PWD");
		export_folder_short(data, current, "OLDPWD");
	}
	else
	{
		write(STDERR_FILENO, "Folder does not exist or no permission\n", 39);
		g_exit_status = ret;
	}
	free(goal);
	free(current);
}

static char	*ft_home(char *str, char *home)
{
	char	*tmp;
	char	*goal;

	tmp = ft_substr(str, 1, ft_strlen(str) - 1);
	goal = ft_strjoin(home, tmp);
	free(tmp);
	return (goal);
}

void	ft_cd(t_data *data, char **arg)
{
	char	*current;
	char	*goal;
	char	*home;

	if (data->pipe_segments->next != NULL)
		return ;
	home = get_folder_long(data, "HOME");
	if (ft_strlen(home) == 0)
	{
		write(STDERR_FILENO, "HOME folder is not set. Be careful!\n", 36);
		home = ft_strdup("/");
	}
	current = getcwd(NULL, 0);
	if (!arg[1])
		goal = ft_strdup(home);
	else if (arg[1][0] == '-')
		goal = get_folder_long(data, "OLDPWD");
	else if (arg[1][0] == '~')
		goal = ft_home(arg[1], home);
	else
		goal = ft_strdup(arg[1]);
	free(home);
	clean_folder(data, goal, current);
}
