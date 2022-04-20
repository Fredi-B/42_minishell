/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_v2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbechtol <fbechtol@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 15:03:12 by fbechtol          #+#    #+#             */
/*   Updated: 2022/03/04 15:03:18 by fbechtol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	sig_int_cmds(int signum);
static void	sig_quit_cmds(int signum);

void	sig_int_prompt(int signum __attribute__ ((unused)))
{
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	g_exit_status = 130;
}

void	sig_int_here_doc(int signum __attribute__ ((unused)))
{
	write(1, "\n", 1);
	g_exit_status = 130;
	close(0);
}

void	prep_signals_for_cmds(t_data *data)
{
	data->s_ctrl_c.sa_handler = sig_int_cmds;
	sigaction(SIGINT, &data->s_ctrl_c, NULL);
	data->s_ctrl_back_slash.sa_handler = sig_quit_cmds;
	sigaction(SIGQUIT, &data->s_ctrl_back_slash, NULL);
}

static void	sig_int_cmds(int signum __attribute__ ((unused)))
{
	write(1, "\n", 1);
	g_exit_status = 130;
}

static void	sig_quit_cmds(int signum __attribute__ ((unused)))
{
	write(2, "Quit: 3\n", 8);
	g_exit_status = 131;
}
