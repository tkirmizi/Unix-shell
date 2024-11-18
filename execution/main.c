/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 13:36:12 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/18 11:21:58 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	g_signal = 0;

int	process_input(t_ms *ms)
{
	g_signal = 0;
	ms->input = readline("user@user : ");
	if (!ms->input)
		return (write(1, "exit\n", 5), 0);
	add_history(ms->input);
	if (check_input(ms->input))
		return (1);
	if (!parse(ms))
		exit_before_process(&ms);
	update_shell_lvl(&ms);
	execution(ms);
	free_commands(ms->cmd);
	free(ms->input);
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	t_ms	ms;
	t_env	*env_s;

	if (argc != 1)
		return (1);
	env_s = NULL;
	ms.env = envp;
	env_s = ft_save_env(ms.env, env_s);
	if (!env_s)
		return (1);
	ms.env_s = env_s;
	if (init_signals() == -1)
		return (cleanup_env(env_s), 1);
	rl_bind_key('\t', rl_complete);
	while (1)
	{
		if (!process_input(&ms))
			break ;
	}
	rl_clear_history();
	cleanup_env(env_s);
	return ((void)argv, 0);
}
