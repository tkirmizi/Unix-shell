/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 13:36:12 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/13 13:37:02 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int process(t_ms *ms)
{
	g_signal = 0;
	ms->input = readline("user@user : ");
	if (ms->input)
		add_history(ms->input);
	else
		return (printf("exit\n"), 0);
	if (wrong_input(ms->input))
		return (1);
	if (!parse(ms))
		return (free(ms->input), 1);
	update_shell_lvl(&ms);
	execution(ms);
	return (free_commands(ms->cmd), free(ms->input), 1);
}

int main(int argc, char **argv, char **envp)
{
	t_ms ms;
	t_env *env_s;

	signal_handler();
	env_s = NULL;
	ms.env = envp;
	env_s = ft_save_env(ms.env, env_s);
	ms.env_s = env_s;
	if (argc != 1)
		return (1);
	rl_bind_key('\t', rl_complete);
	while (1)
		if (!process(&ms))
			break;
	rl_clear_history();
	cleanup_env(env_s);
	return ((void)argv, (void)argc, 0);
}

int wrong_input(char *input)
{
	if (!input)
		return (printf("ERROR! No input"), 0);
	if (ft_strlen(input) == 0 || input[0] == '\0' || !not_empty(input))
		return (free(input), 1);
	if (ft_strncmp(input, "clear", 5) == 0)
		return (printf("\033[H\033[J"), free(input), 1);
	return (0);
}

int not_empty(const char *input)
{
	int i;

	i = 0;
	while (input[i])
	{
		if (input[i] != ' ' && input[i] != '\t')
			return (1);
		i++;
	}
	return (0);
}

void update_shell_lvl(t_ms **ms)
{
	t_env *temp;
	int new_shlvl;

	temp = (*ms)->env_s;
	while (temp)
	{
		if (ft_strncmp(temp->env_name, "SHLVL", 5) == 0)
			break;
		temp = temp->next;
	}
	new_shlvl = ft_atoi(temp->env_value);
	new_shlvl += 1;
	temp->env_value = ft_itoa(new_shlvl);
}