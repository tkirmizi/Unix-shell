/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:44:14 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/14 19:15:43 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_lex	*lexer_init(t_ms *ms)
{
	t_lex	*lexer;

	lexer = (t_lex *)malloc(sizeof(t_lex));
	if (!lexer)
		return (NULL);
	lexer->input = ms->input;
	lexer->dq = 0;
	lexer->sq = 0;
	lexer->pos = 0;
	return (lexer);
}

t_token	*ft_ext_word_val(int start, t_lex *lex)
{
	t_token	*token;
	char	*value;

	value = ft_substr(lex->input, start, lex->pos - start);
	token = (t_token *)malloc(sizeof(t_token));
	if (!value || !token)
		return (free(value), NULL);
	token->type = WORD;
	token->value = value;
	return (token);
}
