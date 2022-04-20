/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_here_docs_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbechtol <fbechtol@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 14:57:27 by fbechtol          #+#    #+#             */
/*   Updated: 2022/03/04 14:57:28 by fbechtol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	dup_stdin_back(int tmp_fd_in)
{
	if (g_exit_status == 130)
	{
		dup2(tmp_fd_in, STDIN_FILENO);
		return (ERROR);
	}
	return (OK);
}
