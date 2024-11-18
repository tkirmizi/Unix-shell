/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 13:37:20 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/18 11:06:16 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execution(t_ms *ms)
{
	int	count_command;
	int	status;

	status = 0;
	count_command = ft_command_counter(&(ms->cmd));
	if (strcmp(ms->cmd->args[0], "exit") == 0)
		do_exit(&ms);
	if (count_command == 1)
		exec_for_else(ms, status);
	else
		multi_exec(&ms, count_command);
}

void	exec_for_else(t_ms *ms, int status)
{
	pid_t	pid;

	if (ft_is_builtin(&(ms)->cmd) != 10)
		do_builtin(&ms, &(ms->cmd));
	else
	{
		pid = fork();
		if (pid < 0)
			exit(ms->exit_code);
		if (pid == 0)
			one_exec(&ms, &(ms->cmd));
		else
		{
			waitpid(pid, &status, 0);
			ms->exit_code = WEXITSTATUS(status);
		}
	}
}

void	one_exec(t_ms **ms, t_cmd **cmd)
{
	int		i;
	t_cmd	*temp;

	temp = (*cmd);
	dup2((*cmd)->fd_in, STDIN_FILENO);
	dup2((*cmd)->fd_out, STDOUT_FILENO);
	if (strchr((*cmd)->args[0], '/') != NULL)
	{
		if (access((*cmd)->args[0], X_OK) == 0)
		{
			if (execve(temp->args[0], temp->args, (*ms)->env) == -1)
			{
				ft_write_to_fd(2, "error: execution");
				(*ms)->exit_code = 127;
				exit(127);
			}
		}
		else
			one_exec_cont_sec(ms, cmd, &temp, &i);
	}
	else
		one_exec_cont_sec(ms, cmd, &temp, &i);
}

void	one_exec_cont_sec(t_ms **ms, t_cmd **cmd, t_cmd **temp, int *i)
{
	all_path_joiner(ms, cmd);
	arg_join(cmd);
	find_exact_path(ms, cmd, i);
	execve((*temp)->path_for_excat[*i], (*temp)->args, (*ms)->env);
	(*ms)->exit_code = 1;
	exit((*ms)->exit_code);
}

void	exec_for_multi(t_ms **ms, t_cmd **cmd)
{
	int		i;
	t_cmd	*temp;

	temp = (*cmd);
	if (ft_is_builtin(cmd) != 10)
		do_builtin(ms, cmd);
	else if (strchr((*cmd)->args[0], '/') != NULL)
	{
		if (access((*cmd)->args[0], X_OK) == 0)
		{
			if (execve(temp->args[0], temp->args, (*ms)->env) == -1)
			{
				ft_write_to_fd(2, "error: execution");
				exit(127);
			}
		}
		else
			ft_and_exit();
	}
	else
		one_exec_cont_sec(ms, cmd, &temp, &i);
}
