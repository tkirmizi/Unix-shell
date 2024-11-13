/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   other_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:46:35 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/13 12:47:13 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int get_num_len(int n)
{
	int len;

	len = 1;
	if (n < 0)
	{
		len++;
		n = -n;
	}
	while (n >= 10)
	{
		n /= 10;
		len++;
	}
	return (len);
}

void subtitute_free_args(char ***args, int *n, int *s_flag)
{
	char *temp;
	char **split_args;
	char **new_args;

	*s_flag = 0;
	temp = (*args)[*n];
	split_args = ft_split(temp, ' ');
	if (!split_args)
		return;

	new_args = allocate_new_args(*args, temp, split_args, n);
	if (!new_args)
	{
		free(split_args);
		return;
	}

	free(temp);
	free(*args);
	free(split_args);
	*args = new_args;
}

char *str_expander(char *expanded_str, char *str, int *s_flag, t_ms *ms, int len)
{
	int i;

	i = -1;
	while (++i < len)
	{
		if (str[i] == '\'' || str[i] == '\"' || str[i] == '$')
		{
			if (str[i] == '\'')
			{
				process_single_qutoes(str, &i, &expanded_str);
				continue;
			}
			else if (str[i] == '\"')
			{
				process_dbl_quotes(str, &i, &expanded_str, ms);
				continue;
			}
			else if (str[i] == '$')
				process_var_expasion_second(str, &i, &expanded_str, ms, &s_flag);
		}
		else
			expanded_str = ft_strnjoin(expanded_str, &str[i], 1);
	}
	return (expanded_str);
}

int ft_valid_expand(char **s, t_ms *ms, int *s_flag)
{
	char *expanded_str;
	char *str;

	str = *s;
	expanded_str = NULL;
	expanded_str = str_expander(expanded_str, str, s_flag, ms, ft_strlen(str));
	if (expanded_str)
	{
		free(*s);
		*s = expanded_str;
		return (1);
	}
	return (0);
}

int ft_strcmp(const char *s1, const char *s2)
{
	if ((*s1 != *s2) || (*s1 == '\0') || (*s2 == '\0'))
		return ((unsigned char)*s1 - (unsigned char)*s2);
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}