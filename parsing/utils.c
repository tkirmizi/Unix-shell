/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:47:20 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/14 22:02:13 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	copy_split_words(char **split, char **args, int i)
{
	int	j;
	int	split_count;

	split_count = 0;
	while (split[split_count])
		split_count++;
	j = 1;
	while (j < split_count)
	{
		args[i + j] = ft_strdup(split[j]);
		j++;
	}
	args[i + split_count] = NULL;
	j = 0;
	while (split[j])
	{
		free(split[j]);
		j++;
	}
	free(split);
	return (split_count - 1);
}

void	handle_explanded_sec(t_cmd **cmd, char **expanded, int *i)
{
	free((*cmd)->args[*i]);
	(*cmd)->args[*i] = (*expanded);
}

int	handle_quote_cont(char *str, t_expansion *exp,
	char quote_type, size_t start)
{
	char	*expanded;
	char	*quoted;

	if (quote_type == '\'')
	{
		exp->result = ft_strnjoin(exp->result, &str[start], exp->pos - start);
		exp->in_squote = 0;
	}
	else
	{
		quoted = ft_substr(str, start, exp->pos - start);
		if (!quoted)
			return (0);
		expanded = expand_variables(quoted, exp);
		free(quoted);
		if (!expanded)
			return (0);
		exp->result = ft_strnjoin(exp->result, expanded, ft_strlen(expanded));
		free(expanded);
		exp->in_dquote = 0;
	}
	exp->pos++;
	return (1);
}

int	ft_exp_concan(char *str, int *i, char **ex_str, t_ms *ms)
{
	char	*expansion;
	char	*temp;
	int		start_pos;
	int		end_pos;

	start_pos = *i;
	while (str[*i] && ft_isalnum(str[*i]))
	{
		(*i)++;
		end_pos = *i;
	}
	temp = ft_substr(str, start_pos, end_pos - start_pos);
	if (!temp)
		return (0);
	expansion = ft_fetch_env(temp, ms->env);
	if (!expansion)
	{
		free(temp);
		return (0);
	}
	*ex_str = ft_strnjoin(*ex_str, expansion, ft_strlen(expansion));
	ft_set_env(temp, expansion, 1, ms->env_s);
	free(temp);
	return (1);
}
