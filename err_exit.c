/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   err_exit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbechtol <fbechtol@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 14:56:30 by fbechtol          #+#    #+#             */
/*   Updated: 2022/03/04 14:56:31 by fbechtol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* frees everything needed. prints out error message */
void	err_exit(t_data *data, char *msg, int len)
{
	write(STDERR_FILENO, msg, len);
	if (len != 0)
		write(STDERR_FILENO, "\n", 1);
	free_data(data);
	exit(g_exit_status);
}
