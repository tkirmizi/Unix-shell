/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 15:29:49 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/08/30 16:30:03 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **env)
{
	t_ms *mini;
	while (1)
	{
		execution(mini->cmd);
	}
}

void	execution(t_ms *mini)
{
	int	count_command; // count
	count_command = command_counter(mini->cmd);
	if (count_command == 1) // one execution
		one_exec(mini);
	// else
	//	execution with pipes.
}

int		command_counter(t_cmd *command)
{
	int	i;
	
	i = 0;
	if (command -> args == NULL) // no args and what to do
		return (i);
	else if (command -> next == NULL && command -> args) 
			return (1);
	else
	{
		while (command -> next != NULL)
		{
			i++;
			command = command -> next;
		}
		return (i + 1);
	}
}

void	one_exec(t_ms *ms)
{
	char	*path;
	int	i;
	
	i = 0;
	path =  "PATH"; // is it ok to do it ?
	t_ms *temp = ms;
	while (temp->env_s != NULL)
	{
		if (ft_strcmp(temp->env_s->env_name, path) == 0)
		{
			printf("env name = %s\n", temp->env_s->env_name);
			break;
		}
		temp->env_s = temp->env_s->next;
	}
	ms->all_cmd_paths = ft_split(ms->env_s->env_name, ':'); // for all paths
	arg_join(ms);
	find_exact_path(ms, &i);
	execve(ms->all_cmd_paths[i], ms->cmd->args, ms->env);
}

void	find_exact_path(t_ms *ms, int	*i)
{
	while (ms->all_cmd_paths[*i])  // while (pipex->path_ptr[*i])
	{
		if (access(ms->all_cmd_paths[*i], X_OK) == 0)
			return ;
		(*i)++;
	}
	// if it comes here means problem on find exact executable, so i have to handle it.
}

void	arg_join(t_ms *ms)
{
	int		i;
	char	*temp;

	i = 0;
	while (ms->all_cmd_paths[i])
	{
		temp = ms->all_cmd_paths[i];
		ms->all_cmd_paths[i] = ft_strjoin(temp, "/");
		free(temp);
		temp = ms->all_cmd_paths[i];
		ms->all_cmd_paths[i] = ft_strjoin(temp, ms->cmd->args[0]);
		free(temp);
		i++;
	}
}