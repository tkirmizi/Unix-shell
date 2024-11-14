/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_handle_cont.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:37:39 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/14 19:17:40 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*handle_exit_code(t_expansion *exp, char **result)
{
	char	*exit_str;
	char	*temp;

	exit_str = ft_itoa(exp->last_exit_code);
	if (!exit_str)
		return (NULL);
	temp = ft_strnjoin(*result, exit_str, ft_strlen(exit_str));
	free(exit_str);
	if (!temp)
		return (NULL);
	*result = temp;
	return (*result);
}

char	*handle_pid(t_expansion *exp, char **result)
{
	char	*pid_str;
	char	*temp;

	pid_str = ft_itoa(exp->shell_pid);
	if (!pid_str)
		return (NULL);
	temp = ft_strnjoin(*result, pid_str, ft_strlen(pid_str));
	free(pid_str);
	if (!temp)
		return (NULL);
	*result = temp;
	return (*result);
}

char	*handle_env_var(char *str, size_t *i, char **result)
{
	char	*var_name;
	char	*var_value;
	char	*temp;
	size_t	start;

	start = *i;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	var_name = ft_substr(str, start, *i - start);
	if (!var_name)
		return (NULL);
	var_value = ft_fetch_env(var_name, NULL);
	if (var_value)
	{
		temp = ft_strnjoin(*result, var_value, ft_strlen(var_value));
		if (!temp)
			return (NULL);
		*result = temp;
	}
	free(var_name);
	return (*result);
}

char	*handle_dollar(char *str, size_t *i, t_expansion *exp, char **result)
{
	(*i)++;
	if (str[*i] == '?')
	{
		if (!handle_exit_code(exp, result))
			return (NULL);
		(*i)++;
	}
	else if (str[*i] == '$')
	{
		if (!handle_pid(exp, result))
			return (NULL);
		(*i)++;
	}
	else if (ft_isalpha(str[*i]) || str[*i] == '_')
	{
		if (!handle_env_var(str, i, result))
			return (NULL);
	}
	return (*result);
}

void	destroy_expansion(t_expansion *exp)
{
	if (!exp)
		return ;
	free(exp->result);
	free(exp);
}
