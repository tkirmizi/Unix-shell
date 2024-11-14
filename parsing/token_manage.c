/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_manage.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:25:47 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/13 17:07:07 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*get_token(t_lex *lexer)
{
	int		input_len;
	char	current_char;

	input_len = ft_strlen(lexer->input);
	while (lexer->pos < input_len)
	{
		current_char = lexer->input[lexer->pos];
		if (ft_isspace(current_char))
			lexer->pos++;
		else if (current_char == '|')
			return (ft_create_pipe_t(lexer));
		else if (current_char == '<')
			return (ft_input_token(lexer));
		else if (current_char == '>')
			return (ft_output_token(lexer));
		else if ((current_char == '\'' || current_char == '\"')
			&& lexer->input[lexer->pos + 1] == current_char)
			return (lexer->pos++, lexer->pos++, cr_empty_q_t());
		else
			return (ft_word_token(lexer));
	}
	return (ft_endof_t());
}

void	free_tokens(t_token *token)
{
	t_token	*temp;

	while (token)
	{
		temp = token->next;
		free(token->value);
		free(token);
		token = temp;
	}
}

void	display_tokens(t_token *token)
{
	char	type_str[12];
	char	*value;

	while (token)
	{
		write(1, "\n--------\n", 10);
		ft_itoa_buf(token->type, type_str);
		write(1, "Token Type: ", 12);
		write(1, type_str, ft_strlen(type_str));
		write(1, "\nToken Value: ", 13);
		if (token->value)
			value = token->value;
		else
			value = "(null)";
		write(1, value, ft_strlen(value));
		write(1, "\n", 1);
		token = token->next;
	}
}

char	*copy_token_value(t_token *token)
{
	char	*new_str;

	new_str = ft_strdup(token->value);
	if (!new_str)
	{
		write(2, "Memory allocation failed\n", 23);
		return (NULL);
	}
	return (new_str);
}

void	clean_str_array(char **arr, int limit)
{
	int	idx;

	idx = 0;
	if (limit == -1)
	{
		while (arr[idx])
		{
			free(arr[idx]);
			idx++;
		}
	}
	else
	{
		while (idx < limit)
		{
			free(arr[idx]);
			idx++;
		}
	}
	free(arr);
}
