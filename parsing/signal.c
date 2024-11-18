/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 15:56:25 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/18 11:27:49 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	set_term(void)
{
	struct termios	term;
	int				ret;

	ret = tcgetattr(STDIN_FILENO, &term);
	if (ret == -1)
		return (-1);
	term.c_lflag &= ~(ECHOCTL);
	ret = tcsetattr(STDIN_FILENO, TCSANOW, &term);
	if (ret == -1)
		return (-1);
	return (0);
}

void	handle_sigint(int sig)
{
	g_signal = sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_sigquit(int sig)
{
	(void)sig;
	if (!g_signal)
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	handle_eof(void)
{
	if (!g_signal)
	{
		write(1, "exit\n", 5);
		exit(0);
	}
}

int	init_signals(void)
{
	if (set_term() == -1)
		return (-1);
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
	return (0);
}
