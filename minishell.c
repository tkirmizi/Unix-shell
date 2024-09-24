/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 15:29:49 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/09/24 16:05:39 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int	g_signal;

int main(int argc, char **argv, char **envp) {
  t_ms ms;
  t_env *env_s;

	signal_handler();
	env_s = NULL;
	ms.env = envp;
	env_s = save_env(ms.env, env_s);
	ms.env_s = env_s;
	if (argc != 1)		return (printf("ERROR! Use: ./mini_shell\n"), 1);
	using_history();
	rl_bind_key('\t', rl_complete);
	while (1)
		if (!exec_process(&ms))
			break;
	return (clear_history(), (void)argv, (void)argc, 0);
}


int exec_process(t_ms *ms)
{
	g_signal = 0;
	ms->input = readline("PROSI-shell$ :");
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

void	update_shell_lvl(t_ms **ms)
{
	t_env	*temp;
	int	new_shlvl;

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
	update_path(ms);
}

void	update_path(t_ms **ms) // will be used also for unset
{
	t_env	*temp;
	int	i;
	// env dolu ise freelenmeli.
	i = 0;
	temp = (*ms)->env_s;
	while (temp != NULL)
	{
		i++;
		temp = temp->next;
	}
	(*ms)->env = (char**)malloc((i + 1) * sizeof(char *));
	(*ms)->env[i] = NULL;
	temp = (*ms)->env_s;
	i = 0;
	while (temp != NULL)
	{
		(*ms)->env[i] = ft_strdup(temp->env_name);
		(*ms)->env[i] = ft_strjoin((*ms)->env[i], "=");
		(*ms)->env[i] = ft_strjoin((*ms)->env[i], temp->env_value);
		i++;
		temp = temp->next;
	}
}