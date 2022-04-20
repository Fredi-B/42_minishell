/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_cmds.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbechtol <fbechtol@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 14:56:50 by fbechtol          #+#    #+#             */
/*   Updated: 2022/03/04 14:56:51 by fbechtol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	join_path_and_cmd(t_data *data);
static char	*add_slash_and_join(char *path, char *cmd);

void	launch_cmds(t_data *data)
{
	int	pid;

	pid = 0;
	prep_signals_for_cmds(data);
	data->pipe_segments = data->p_s_head;
	while (data->pipe_segments != NULL)
	{
		create_pipe(data);
		if (builtin_main(data, data->pipe_segments->cmd_arr) != NO)
		{
			data->pipe_segments = data->pipe_segments->next;
			continue ;
		}
		pid = fork();
		if (pid == -1)
			err_exit(data, "Error: create fork", 18);
		if (pid == 0)
			child_routine(data);
		else
			close_read_end(data);
		data->pipe_segments = data->pipe_segments->next;
	}
	close_pipes(data);
	wait_routine(pid);
}

void	execute_cmd(t_data *data)
{
	char	*str;

	str = data->pipe_segments->cmd_arr[0];
	if (ft_strncmp(str, "echo", 4) == 0 && ft_strlen(str) == 4)
		ft_echo(data->pipe_segments->cmd_arr);
	else if (ft_strncmp(str, "env", 3) == 0 && ft_strlen(str) == 3)
		ft_env(data, 0);
	else if (ft_strncmp(str, "pwd", 3) == 0 && ft_strlen(str) == 3)
		ft_pwd();
	else if (ft_strncmp(str, "export", 6) == 0 && ft_strlen(str) == 6 && \
	!data->pipe_segments->cmd_arr[1])
		ft_env(data, 1);
	else
	{
		join_path_and_cmd(data);
		if (execve(data->pipe_segments->cmd_arr[0], \
				data->pipe_segments->cmd_arr, data->envp) == -1)
		{	
			write(2, data->pipe_segments->cmd_arr[0], \
					ft_strlen(data->pipe_segments->cmd_arr[0]));
			write(2, ": command not found\n", 20);
			g_exit_status = 127;
			err_exit(data, "", 0);
		}
	}
}

static void	join_path_and_cmd(t_data *data)
{
	char	*tmp;
	int		i;

	i = 0;
	if (data->paths == NULL || data->pipe_segments->cmd_arr[0][0] == '/' \
	|| data->pipe_segments->cmd_arr[0][0] == '.')
		return ;
	while (data->paths[i] != NULL)
	{
		tmp = add_slash_and_join(data->paths[i], \
								data->pipe_segments->cmd_arr[0]);
		if (access(tmp, X_OK) == 0)
		{
			free(data->pipe_segments->cmd_arr[0]);
			data->pipe_segments->cmd_arr[0] = ft_strdup(tmp);
			free(tmp);
			break ;
		}
		free(tmp);
		i++;
	}
}

static char	*add_slash_and_join(char *path, char *cmd)
{
	char	*tmp_path;
	char	*tmp;

	tmp_path = ft_strjoin(path, "/");
	tmp = ft_strjoin(tmp_path, cmd);
	free(tmp_path);
	return (tmp);
}
