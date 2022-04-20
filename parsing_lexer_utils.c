/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_lexer_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbechtol <fbechtol@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 14:59:48 by fbechtol          #+#    #+#             */
/*   Updated: 2022/03/04 14:59:49 by fbechtol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_pipe_segment(t_pipe_segment *new_segment)
{
	new_segment->tokens = NULL;
	new_segment->t_head = NULL;
	new_segment->cmd_arr = NULL;
	new_segment->ins_and_outs = NULL;
	new_segment->i_o_head = NULL;
	new_segment->next = NULL;
}

void	init_flag_segment(t_flag *flag)
{
	flag->redir = OFF;
	flag->dob_quot = OFF;
	flag->sing_quot = OFF;
	flag->dollar = OFF;
	flag->word = OFF;
}

void	switch_flag(int *i_flag)
{
	if (*i_flag == ON)
		*i_flag = OFF;
	else
		*i_flag = ON;
}
