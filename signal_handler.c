/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 15:56:25 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/12 10:49:31 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void sigint_handler(int sig)
{
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_signal = sig;
}

void sigquit_handler(int sig)
{
	if (g_signal == 0)
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else
	{
		signal(sig, SIG_DFL);
		raise(sig);
	}
}

void sig_eof_handler(int sig)
{
	if (g_signal == 0)
	{
		printf("exit\n");
		exit(0);
	}
	else
	{
		signal(sig, SIG_DFL);
		raise(sig);
	}
}

int handle_termios(void)
{
	struct termios term_settings;
	int ret;

	ret = tcgetattr(STDIN_FILENO, &term_settings);
	if (ret == -1)
	{
		perror("tcgetattr");
		return (1);
	}
	term_settings.c_lflag &= ~(ECHOCTL);
	ret = tcsetattr(STDIN_FILENO, TCSANOW, &term_settings);
	if (ret == -1)
	{
		perror("tcsetattr");
		return (1);
	}
	return (0);
}

void signal_handler(void)
{
	if (handle_termios() != 0)
	{
		fprintf(stderr, "Error: Failed to set terminal settings\n");
		exit(1);
	}
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}
