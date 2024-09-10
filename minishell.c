/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 15:29:49 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/09/09 17:23:33 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int	main(int argc, char **argv, char **env)
{
	t_ms *ms;
	
	while (1)
		exec_process(&ms);
	return (clear_history(), (void)argv, (void)argc, 0);
}

int	exec_process(t_ms *ms)
{
	// signal will come
	ms->input = readine(">");
	if (ms->input)
		add_history(ms->input);
		// BOTTOM WILL BE HANDLED BY PARSER
	// else
	// 	return (printf("exit\n"), 0);
	// if (wrong_input(ms->input))
	// 	return (1);
	// if (!parse(ms))
	// 	return (free(ms->input), 1);
	execution(ms);
}