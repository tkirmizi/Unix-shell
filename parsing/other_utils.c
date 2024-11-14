/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   other_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:46:35 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/14 20:37:05 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	get_num_len(int n)
{
	int	len;

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

void	subtitute_free_args(char ***args, int *n, int *s_flag)
{
	char	*temp;
	char	**split_args;
	char	**new_args;

	*s_flag = 0;
	temp = (*args)[*n];
	split_args = ft_split(temp, ' ');
	if (!split_args)
		return ;
	new_args = allocate_new_args(*args, temp, split_args, n);
	if (!new_args)
	{
		free(split_args);
		return ;
	}
	free(temp);
	free(*args);
	free(split_args);
	*args = new_args;
}

int	ft_valid_expand(char **s, t_ms *ms, int *s_flag)
{
	t_expand	exp;

	exp.str = *s;
	exp.expanded_str = NULL;
	exp.ms = ms;
	exp.s_flag = s_flag;
	exp.len = ft_strlen(exp.str);
	exp.expanded_str = str_expander(&exp);
	if (exp.expanded_str)
	{
		free(*s);
		*s = exp.expanded_str;
		return (1);
	}
	return (0);
}

int	ft_strcmp(const char *s1, const char *s2)
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

char	*str_expander(t_expand *exp)
{
	int	i;

	i = -1;
	while (++i < exp->len)
	{
		if (exp->str[i] == '\'' || exp->str[i] == '\"' || exp->str[i] == '$')
			process_quote_handler(exp, &i);
		else
			exp->expanded_str = ft_strnjoin(exp->expanded_str, &exp->str[i], 1);
	}
	return (exp->expanded_str);
}
