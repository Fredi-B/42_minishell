/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbechtol <fbechtol@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 14:55:16 by fbechtol          #+#    #+#             */
/*   Updated: 2022/03/04 14:55:17 by fbechtol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_flag(t_flag *flag, char c)
{
	if (c == '0')
	{
		flag->redir = OFF;
		flag->dob_quot = OFF;
		flag->sing_quot = OFF;
		flag->dollar = OFF;
		flag->word = OFF;
	}
	else if (c == '\'' && flag->dob_quot == OFF)
	{
		if (flag->sing_quot == OFF)
			flag->sing_quot = ON;
		else
			flag->sing_quot = OFF;
	}
	else if (c == '\"' && flag->sing_quot == OFF)
	{
		if (flag->dob_quot == OFF)
			flag->dob_quot = ON;
		else
			flag->dob_quot = OFF;
	}
	else
		write(1, &c, 1);
}

static int	ft_echo_dash(char **str, int *fakenl, int *nl)
{
	char	*tmp;
	int		k;

	tmp = str[0];
	while (*++str[0] == 'n')
		k = 2;
	if (*str[0] != '\0')
	{
		*fakenl = 1;
		write(1, "-", 1);
		str[0] = tmp;
		++str[0];
		return (ERROR);
	}
	else
		*nl = 2;
	return (OK);
}

static void	ft_echo_arg(char *str, int *nl, char *next, int *written)
{
	t_flag	flag;
	int		fakenl;

	fakenl = 0;
	set_flag(&flag, '0');
	while (*str)
	{
		if (*str == '\"' || *str == '\'')
			set_flag(&flag, *str);
		else if (*str == '-' && *written == 0)
		{
			if (ft_echo_dash(&str, &fakenl, nl) == OK)
			{
				return ;
			}
		}
		write(1, str, 1);
		*written = 1;
		str++;
	}
	if (fakenl == 1 && next != NULL)
		write(1, " ", 1);
	else if (next != NULL)
		write(1, " ", 1);
}

void	ft_echo(char **arg)
{
	int		i;
	int		nl;
	int		written;

	written = 0;
	i = 1;
	nl = 0;
	while (arg[i] != NULL)
	{
		ft_echo_arg(arg[i], &nl, arg[i + 1], &written);
		i++;
	}
	if (nl == 0)
		write(1, "\n", 1);
	exit (0);
}
