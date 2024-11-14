/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:34:19 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/14 19:13:16 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	parse(t_ms *ms)
{
	if (!lexing(ms))
		return (handle_parse_error(ms));
	if (syntax_error(ms))
		return (handle_parse_error(ms));
	if (!commands(ms))
		return (handle_parse_error(ms));
	expand(ms);
	if (!ft_manage_redir(ms))
		return (handle_parse_error(ms));
	free_tokens(ms->token);
	return (1);
}

int	handle_parse_error(t_ms *ms)
{
	free_tokens(ms->token);
	return (0);
}

int	lexing(t_ms *ms)
{
	t_lex	*lexer;
	t_token	*token;

	lexer = lexer_init(ms);
	if (!lexer || !lexer->input)
		return (0);
	ms->token = get_token(lexer);
	if (!ms->token)
		return (0);
	ms->token->prev = NULL;
	token = ms->token;
	while (token->type != END)
	{
		token->next = get_token(lexer);
		if (!token->next || token->type == ERR)
		{
			ms->exit_code = 258;
			ft_putstr_fd("Error! unclosed quotes\n", 2);
			return (free(lexer), 0);
		}
		token->next->prev = token;
		token = token->next;
	}
	return (free(lexer), 1);
}

int	syntax_error(t_ms *ms)
{
	t_token	*token;

	token = ms->token;
	while (token)
	{
		if ((token->type == PIPE && pipe_error_check(token))
			|| ((token->type == IN || token->type == H_DOC || token->type == OUT
					|| token->type == APPEND) && redir_error_check(token)))
		{
			ms->exit_code = 258;
			return (1);
		}
		token = token->next;
	}
	return (0);
}
