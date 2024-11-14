/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:39:30 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/13 17:21:40 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**split_expanded(char *str)
{
	char	**words;
	int		word_count;

	word_count = count_words(str);
	words = (char **)malloc(sizeof(char *) * (word_count + 1));
	if (!words)
		return (NULL);
	return (split_expanded_cont(str, words));
}

char	**split_expanded_cont(char *str, char **words)
{
	int	i;
	int	word_count;

	i = 0;
	word_count = 0;
	while (str[i])
	{
		while (str[i] && ft_isspace(str[i]))
			i++;
		if (!str[i])
			break ;
		if (!fill_word(words, str, &i, word_count))
			return (clean_split_fail(words, word_count));
		word_count++;
	}
	words[word_count] = NULL;
	return (words);
}

int	count_words(char *str)
{
	int	word_count;
	int	i;

	word_count = 0;
	i = 0;
	while (str[i])
	{
		while (str[i] && ft_isspace(str[i]))
			i++;
		if (str[i])
			word_count++;
		while (str[i] && !ft_isspace(str[i]))
			i++;
	}
	return (word_count);
}

int	fill_word(char **words, char *str, int *i, int word_count)
{
	int	start;

	start = *i;
	while (str[*i] && !ft_isspace(str[*i]))
		(*i)++;
	words[word_count] = ft_substr(str, start, *i - start);
	if (!words[word_count])
		return (0);
	return (1);
}

char	**clean_split_fail(char **words, int word_count)
{
	while (--word_count >= 0)
		free(words[word_count]);
	free(words);
	return (NULL);
}
