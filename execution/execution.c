/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:47:07 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/09/12 18:05:09 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	execution(t_ms *ms) // BENIM ANA EXECUTION
{
	int	count_command;

	count_command = ft_command_counter(&(ms->cmd)); // adresi gonderilecek..
	
	if (count_command == 1) // one execution
		one_exec(&ms, &(ms->cmd));
	else
		multi_exec(&ms, count_command);
}

void one_exec(t_ms **ms, t_cmd **cmd)
{
	int	i;
	t_cmd *temp;

	temp = (*cmd);
	if (ft_is_builtin(cmd) != 10) // not working after going somewhere with 'cd'
		do_builtin(ms, cmd);
	else
	{
		all_path_joiner (ms);
		arg_join(ms);
		find_exact_path(cmd, &i);
		ft_write_to_fd(2, (*cmd)->args[0]); // will be deleted
		execve(temp->path_for_excat[i], temp->args, (*ms)->env);
	}
}


void	arg_join(t_ms **ms)
{
	int		i;
	char	*temp;
	t_ms	*temp2;

	temp2 = (*ms);
	i = 0;
	while (temp2->cmd->path_for_excat[i])
	{
		temp = temp2->cmd->path_for_excat[i];
		temp2->cmd->path_for_excat[i] = ft_strjoin(temp, "/");
		free(temp);
		temp = temp2->cmd->path_for_excat[i];
		temp2->cmd->path_for_excat[i] = ft_strjoin(temp, temp2->cmd->args[0]);
		free(temp);
		i++;
	}
}

void	multi_exec(t_ms **ms, int c_command) 
{
	int	i;
	t_cmd	*cmd;
	int	fds[c_command-1][2];
	pid_t	*pids;

	pids = (pid_t *)malloc(c_command * sizeof(pid_t));
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
			cmd = find_true_command(cmd, i);
			ft_write_to_fd(2, "here is working");
			ft_write_to_fd(2, cmd->args[0]);
			one_exec(ms, &cmd); // cmdnin hangisi
			close(fds[0][1]);
		}
		if (i == c_command - 1)
		{
			cl_fds_last(fds, c_command);
			dup2(fds[c_command - 2][0], STDIN_FILENO);
			cmd = find_true_command(cmd, i);
			ft_write_to_fd(2, "here is working");
			ft_write_to_fd(2, cmd->args[0]);
			one_exec(ms, &cmd); // cmdnin hangisi
			close(fds[c_command- 2][0]);
		}
		else
		{
			cl_fds_middle(fds, c_command, i);
			dup2(fds[i - 1][0], STDIN_FILENO);
			dup2(fds[i][1], STDOUT_FILENO);
			cmd = find_true_command(cmd, i);
			ft_write_to_fd(2, "here is working");
			ft_write_to_fd(2, cmd->args[0]);
			one_exec(ms, &cmd); // cmdnin hangisi
			close(fds[i-1][0]);
			close(fds[i][1]);
		}
	}
	else
		waitpid(pids[i], NULL, 0);
}

t_cmd *find_true_command(t_cmd *cmd, int i)
{
	int	j;

	j = 0;
	while (j < i)
		cmd = cmd->next;
	return (cmd);
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