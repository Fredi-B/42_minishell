/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_split_pipe.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbechtol <fbechtol@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 15:00:23 by fbechtol          #+#    #+#             */
/*   Updated: 2022/03/04 15:00:24 by fbechtol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	ft_count_part_2(char const *s, int symbol, int *word);
static void		init_split(t_split *split);

size_t	ft_count(char const *s, int symbol)
{
	size_t	count;
	int		word;

	word = 0;
	count = ft_count_part_2(s, symbol, &word);
	if (word == 1)
		count++;
	return (count);
}

static size_t	ft_count_part_2(char const *s, int symbol, int *word)
{
	size_t	count;
	int		i;
	int		flag;

	flag = -1;
	count = 0;
	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == '\"' || s[i] == '\'')
		{
			*word = 1;
			flag *= -1;
		}
		else if (s[i] != symbol)
			*word = 1;
		else if (s[i] == symbol && *word == 1 && flag == -1)
		{	
			count++;
			*word = 0;
		}
		i++;
	}
	return (count);
}

char	**ft_split_s(char const *s, char **lst, int symbol)
{
	t_split	split;

	init_split(&split);
	while (s[split.i] != '\0')
	{
		if (s[split.i] == '\"' || s[split.i] == '\'')
			set_word_and_flag(&split);
		else if (s[split.i] != symbol)
			split.word = 1;
		else if (s[split.i] == symbol && split.word == 1 && split.flag == -1)
		{
			lst[split.count] = ft_substr(s, split.last, split.i - split.last);
			split.count++;
			split.word = 0;
			split.last = split.i + 1;
		}
		split.i++;
	}
	if (split.word == 1)
	{
		lst[split.count] = ft_substr(s, split.last, split.i - split.last);
		split.count++;
	}	
	lst[split.count] = NULL;
	return (lst);
}

static void	init_split(t_split *split)
{
	split->flag = -1;
	split->i = 0;
	split->count = 0;
	split->word = 0;
	split->last = 0;
}

char	**split_pipe(char *s, int symbol)
{
	size_t	count;
	char	**lst;

	if (!s)
		return (NULL);
	count = ft_count(s, symbol);
	lst = (char **)malloc(sizeof(char *) * (count + 1));
	if (!lst)
		return (NULL);
	ft_split_s(s, lst, symbol);
	return (lst);
}
