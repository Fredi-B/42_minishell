/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_cmds_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbechtol <fbechtol@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 14:56:41 by fbechtol          #+#    #+#             */
/*   Updated: 2022/03/04 14:56:42 by fbechtol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	child_routine(t_data *data)
{
	prep_fd_in(data);
	prep_fd_out(data);
	execute_cmd(data);
	free_data(data);
	if (data->envp != NULL)
		free_two_d_arr(data->envp);
	exit (0);
}

void	wait_routine(int pid)
{
	int	wstatus;

	waitpid(pid, &wstatus, 0);
	pid = 0;
	while (pid >= 0)
		pid = wait(NULL);
	if (WIFEXITED(wstatus))
		g_exit_status = WEXITSTATUS(wstatus);
}
