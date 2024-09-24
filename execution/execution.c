/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:47:07 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/09/24 16:08:08 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	execution(t_ms *ms)
{
	int	count_command;
	int	status;

	count_command = ft_command_counter(&(ms->cmd)); 
	if (strcmp(ms->cmd->args[0], "exit") == 0)
		do_exit(&ms);
	if (count_command == 1) 
	{
		if (ft_is_builtin(&(ms)->cmd)!= 10)
			do_builtin(&ms, &(ms->cmd));
		else
		{
			pid_t pid;
			int	fds[1][2];
			if (pipe(fds[0]) == -1)
				exit(ms->exit_code);
			pid = fork();
			if (pid < 0)
				exit(ms->exit_code);
			if (pid == 0)
				one_exec(&ms, &(ms->cmd));
			else
			{
				waitpid(pid, &status, 0);
				close(fds[0][1]);
				close(fds[0][0]);
				ms->exit_code = WEXITSTATUS(status);
			}
		}
	}
	else
		multi_exec(&ms, count_command);
}

void one_exec(t_ms **ms, t_cmd **cmd)
{
	int	i;
	t_cmd *temp;

	int	j = 0;
	temp = (*cmd);

	if (strchr((*cmd)->args[0], '/') != NULL)
	{
		if (access((*cmd)->args[0], X_OK) == 0)
		{
			if (execve(temp->args[0], temp->args, (*ms)->env) == -1)
			{
				ft_write_to_fd(2, "execution problem");
				(*ms)->exit_code = 127;
				exit(127);
			}
		}
		else
		{
			ft_write_to_fd(2, "execution problem");
			(*ms)->exit_code = 127;
			exit(127);
		}
	}
	else
	{
		all_path_joiner (ms, cmd);
		arg_join(cmd);
		find_exact_path(ms, cmd, &i);
		execve(temp->path_for_excat[i], temp->args, (*ms)->env);
		(*ms)->exit_code = 1;
		exit((*ms)->exit_code);
	}
}

void	 multi_exec(t_ms **ms, int c_command) 
{
	int	i;
	int	j;
	t_cmd	*cmd;
	int	fds[c_command-1][2];
	pid_t	*pids;
	int	status;

	pids = (pid_t *)malloc((c_command) * sizeof(pid_t)); // child process kadar
	i = 0;
	while (i < c_command-1)
		pipe(fds[i++]); // protection will come
	i = 0;
	cmd = (*ms)->cmd;
	while (cmd)
	{
		multi_exec_cont(ms, cmd, pids, fds, i, c_command);
		i++;
		cmd = cmd->next;
	}
	j = 0;
	while (j < c_command - 1) 
	{
		close(fds[j][0]);
		close(fds[j][1]);
		j++;
	}

	i = 0;
	while (i < c_command)
	{
		waitpid(pids[i], &status, 0);
		(*ms)->exit_code = WEXITSTATUS(status);
		i++;
	}
}

void	multi_exec_cont(t_ms **ms, t_cmd *cmd, pid_t *pids, int fds[][2], int i, int c_command)
{
	int	j;

	j = 0;
	pids[i] = fork();
	if (pids[i] == 0)
	{
		if (i == 0)
		{
			cl_fds_first(fds, c_command);
			dup2(fds[0][1], STDOUT_FILENO);
			exec_for_multi(ms, &cmd);
			close(fds[0][1]);
		}
		else if(i == c_command-1)
		{
			cl_fds_last(fds, c_command);
			dup2(fds[c_command-2][0], STDIN_FILENO);
			exec_for_multi(ms, &cmd);
			close(fds[c_command-2][0]);
		}
		else
		{
			cl_fds_middle(fds, c_command, i);
			dup2(fds[i-1][0], STDIN_FILENO);
			dup2(fds[i][1], STDOUT_FILENO);
			exec_for_multi(ms, &cmd);
			close(fds[i-1][0]);
			close(fds[i][1]);
		}
		exit(EXIT_SUCCESS);
	}
	else
		return ;
}

void exec_for_multi(t_ms **ms, t_cmd **cmd)
{
	int	i;
	t_cmd *temp;

	int	j = 0;
	temp = (*cmd);
	if (ft_is_builtin(cmd) != 10)
		do_builtin(ms, cmd);
	else if (strchr((*cmd)->args[0], '/') != NULL)
	{
		if (access((*cmd)->args[0], X_OK) == 0)
		{
			if (execve(temp->args[0], temp->args, (*ms)->env) == -1)
			{
				ft_write_to_fd(2, "exeution problem");// exit code will be update to 127
				exit(127);
			}
		}
		else
		{
			ft_write_to_fd(2, "execution problem");
			exit(127);
		}
	}
	else
	{
		all_path_joiner (ms, cmd);
		arg_join(cmd);
		find_exact_path(ms, cmd, &i);
		execve(temp->path_for_excat[i], temp->args, (*ms)->env);
		exit(EXIT_FAILURE);
	}
}

void	arg_join(t_cmd **cmd)
{
	int		i;
	char	*temp;
	t_cmd	*temp2;

	temp2 = (*cmd);
	i = 0;
	while (temp2->path_for_excat[i])
	{
		temp = temp2->path_for_excat[i];
		temp2->path_for_excat[i] = ft_strjoin(temp, "/");
		free(temp);
		temp = temp2->path_for_excat[i];
		temp2->path_for_excat[i] = ft_strjoin(temp, temp2->args[0]);
		free(temp);
		i++;
	}
}


void	cl_fds_first(int (*fds)[2], int c_command)
{
	int i;
	int	j;

	i = 0;
	j = 0;
	
	close(fds[0][0]);
	while (i < c_command - 1)
	{
		j = 0;
		while (j < 2)
		{
			if ((i != 0))
				close(fds[i][j]);
			j++;
		}
		i++;
	}
}
void	cl_fds_last(int (*fds)[2], int c_command)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < c_command - 1)
	{
		j = 0;
		while (j < 2)
		{
			if (i != c_command-2)
				close(fds[i][j]);
			j++;
		}
		i++;
	}
	close(fds[c_command-2][1]);
}

void	cl_fds_middle(int (*fds)[2], int c_command, int i)
{
	int	j;
	int	k;

	j = 0;
	k = 0;
	while (j < c_command-1)
	{
		if (j != i-1)
			close(fds[j][0]);
		j++;
	}
	while (k < c_command-1)
	{
		if (k != i)
			close(fds[k][1]);
		k++;
	}
}
