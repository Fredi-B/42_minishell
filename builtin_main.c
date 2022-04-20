/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbechtol <fbechtol@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 14:56:15 by fbechtol          #+#    #+#             */
/*   Updated: 2022/03/04 14:56:16 by fbechtol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	ft_putstr_fd(pwd, 1);
	write(1, "\n", 1);
	free(pwd);
	exit (0);
}

void	ft_env(t_data *data, int flag)
{
	int	i;

	i = 0;
	while (data->envp[i] != NULL)
	{
		if (flag == 1)
			ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(data->envp[i], 1);
		write(1, "\n", 1);
		i++;
	}
	exit (0);
}

static int	ft_exit(char **cmd, t_data *data)
{
	int	i;

	if (cmd[1] != NULL)
	{
		i = 0;
		while (*(*(cmd + 1) + i) != '\0')
		{
			if (ft_isdigit(*(*(cmd + 1) + i)) == 0 && *(*(cmd + 1) + 0) != '-')
				break ;
			i++;
		}
		if (*(*(cmd + 1) + i) == '\0')
			g_exit_status = ft_atoi(cmd[1]);
		else
			g_exit_status = 255;
		if (data->pipe_segments->next != NULL \
		|| data->pipe_segments->prev != NULL)
			return (OK);
	}
	else
		g_exit_status = 0;
	if (data->envp != NULL)
		free_two_d_arr(data->envp);
	err_exit(data, "exit", 4);
	return (OK);
}

int	builtin_main(t_data *data, char **cmd_s)
{
	if (cmd_s == NULL || cmd_s[0] == NULL)
		return (NO);
	if (ft_strncmp(cmd_s[0], "export", 6) == 0 && ft_strlen(cmd_s[0]) == 6 \
	&& cmd_s[1] != NULL)
	{
		if (ft_export(data, cmd_s) == ERROR)
			g_exit_status = 1;
	}
	else if (ft_strncmp(cmd_s[0], "unset", 5) == 0 && ft_strlen(cmd_s[0]) == 5)
		ft_unset(data, cmd_s);
	else if (ft_strncmp(cmd_s[0], "exit", 4) == 0 && ft_strlen(cmd_s[0]) == 4)
		ft_exit(cmd_s, data);
	else if (ft_strncmp(cmd_s[0], "cd", 2) == 0 && ft_strlen(cmd_s[0]) == 2)
		ft_cd(data, cmd_s);
	else
		return (NO);
	return (OK);
}
