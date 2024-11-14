/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:30:14 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/14 19:11:50 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ext_quote_val(t_lex *l, char q)
{
	if (q == '\'' && !l->sq && !l->dq)
		l->sq = 1;
	else if (q == '\'' && l->sq && !l->dq && (l->input[l->pos] == '\''))
		l->sq = 0;
	else if (q == '\"' && !l->sq && !l->dq)
		l->dq = 1;
	else if (q == '\"' && !l->sq && l->dq && (l->input[l->pos] == '\"'))
		l->dq = 0;
}

t_token	*open_quote_val(void)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = ERR;
	token->value = ft_strdup("Unclosed quote");
	token->next = NULL;
	return (token);
}

t_token	*cr_empty_q_t(void)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = WORD;
	token->value = ft_strdup("");
	return (token);
}

int	handle_quote(char *str, t_expansion *exp, char quote_type)
{
	size_t	start;

	exp->pos++;
	start = exp->pos;
	if (quote_type == '\'')
		exp->in_squote = 1;
	else
		exp->in_dquote = 1;
	while (str[exp->pos] && str[exp->pos] != quote_type)
		exp->pos++;
	if (!str[exp->pos])
		return (0);
	return (handle_quote_cont(str, exp, quote_type, start));
}
