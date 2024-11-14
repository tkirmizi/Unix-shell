/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:29:13 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/13 17:03:05 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*ft_create_pipe_t(t_lex *lexer)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = PIPE;
	token->value = ft_strdup("|");
	return (lexer->pos++, token);
}

t_token	*ft_input_token(t_lex *lexer)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	if (lexer->input[lexer->pos + 1] == '<')
	{
		token->type = H_DOC;
		token->value = ft_strdup("<<");
		return (lexer->pos += 2, token);
	}
	else
	{
		token->type = IN;
		token->value = ft_strdup("<");
		return (lexer->pos++, token);
	}
}

t_token	*ft_output_token(t_lex *lexer)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	if (lexer->input[lexer->pos + 1] == '>')
	{
		token->type = APPEND;
		token->value = ft_strdup(">>");
		return (lexer->pos += 2, token);
	}
	else
	{
		token->type = OUT;
		token->value = ft_strdup(">");
		return (lexer->pos++, token);
	}
}

t_token	*ft_word_token(t_lex *lex)
{
	int	start;

	start = lex->pos;
	while (lex->input[lex->pos])
	{
		if (lex->input[lex->pos] == '\'' || lex->input[lex->pos] == '\"')
			ext_quote_val(lex, lex->input[lex->pos]);
		if ((ft_isspace(lex->input[lex->pos]) || lex->input[lex->pos] == '|'
				|| lex->input[lex->pos] == '<' || lex->input[lex->pos] == '>')
			&& !(lex->dq || lex->sq))
			break ;
		lex->pos++;
	}
	if ((lex->dq || lex->sq))
		return (open_quote_val());
	return (ft_ext_word_val(start, lex));
}

t_token	*ft_endof_t(void)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = END;
	token->value = NULL;
	token->next = NULL;
	return (token);
}
