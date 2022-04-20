/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_trim_quotes.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbechtol <fbechtol@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 15:03:00 by fbechtol          #+#    #+#             */
/*   Updated: 2022/03/04 15:03:01 by fbechtol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	set_flag(t_flag *flag, char c)
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
		return (OK);
	return (ERROR);
}

static char	*ft_trim_new(char *str, int cnt, int len, t_data *data)
{
	char	*tmp_str;
	int		i;
	int		j;

	if (cnt >= len)
		return (ft_strdup(""));
	tmp_str = (char *) malloc((len - cnt + 1) * sizeof(char));
	if (!tmp_str)
		err_exit(data, "Error: ft_trim", 14);
	i = 0;
	j = 0;
	while (i < len)
	{
		if (str[i] > 0)
		{
			tmp_str[j] = str[i];
			j++;
		}
		i++;
	}
	tmp_str[len - cnt] = '\0';
	return (tmp_str);
}

static char	*ft_trim_arg(char *str, t_data *data)
{
	t_flag	flag;
	char	*str2;
	int		cnt;
	int		len;

	str2 = str;
	len = ft_strlen(str);
	cnt = 0;
	set_flag(&flag, '0');
	while (*str)
	{
		if (*str == '\"' || *str == '\'')
		{
			if (set_flag(&flag, *str) == ERROR)
			{
				*str = 0;
				cnt++;
			}
		}
		str++;
	}
	str = str2;
	return (ft_trim_new(str, cnt, len, data));
}

void	trim_quotes_in_cmd_arr(char **cmd_arr, t_data *data)
{
	char	*tmp;
	int		i;

	i = 0;
	while (cmd_arr[i])
	{
		tmp = ft_trim_arg(cmd_arr[i], data);
		free(cmd_arr[i]);
		cmd_arr[i] = tmp;
		i++;
	}
}
