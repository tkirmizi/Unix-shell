/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:47:07 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/09/20 12:16:24 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	execution(t_ms *ms) // BENIM ANA EXECUTION
{
	int	count_command;

	count_command = ft_command_counter(&(ms->cmd));
	
	if (count_command == 1)
		one_exec(&ms, &(ms->cmd));
	else
		multi_exec(&ms, count_command);
}

void one_exec(t_ms **ms, t_cmd **cmd)
{
	int	i;
	t_cmd *temp;

	int	j = 0;
	temp = (*cmd);
	if (ft_is_builtin(cmd) != 10) // not working after going somewhere with 'cd'
		do_builtin(ms, cmd);
	else if (strchr((*cmd)->args[0], '/') != NULL)
	{
		if (access((*cmd)->args[0], X_OK) == 0)
		{
			if (execve(temp->args[0], temp->args, (*ms)->env) == -1)
			{
				ft_write_to_fd(2, "exeution problem");// exit code will be update to 127
			}
		}
		else
		{
			ft_write_to_fd(2, "execution problem");// exit code will be update to 127
		}
	}
	else
	{
		all_path_joiner (ms, cmd);
		arg_join(cmd);
		find_exact_path(cmd, &i);
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
	int	j;
	t_cmd	*cmd;
	int	fds[c_command-1][2];
	pid_t	*pids;

	pids = (pid_t *)malloc((c_command-1) * sizeof(pid_t)); // sonu null olmali mi ?
	i = 0;
	while (i < c_command-1)
		pipe(fds[i++]); // protection will come
	i = 0;
	cmd = (*ms)->cmd;
	while (cmd)
	{
		multi_exec_cont(ms, cmd, pids, fds, i, c_command);
		i++;
		waitpid(pids[i], NULL, 0);
		cmd = cmd->next;
	}
	j = 0;
	while (j < c_command - 1) 
	{
		close(fds[j][0]);
		close(fds[j][1]);
		j++;
	}
	j = 0;
	while (j < c_command)
    waitpid(pids[j++], NULL, 0);
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
			one_exec(ms, &cmd);
			close(fds[0][1]);
		}
		else if(i == c_command-1)
		{
			cl_fds_last(fds, c_command);
			dup2(fds[c_command-2][0], STDIN_FILENO);
			one_exec(ms, &cmd);
			close(fds[c_command-2][0]);
		}
		else
		{
			waitpid(pids[i-1], NULL, 0);
			cl_fds_middle(fds, c_command, i);
			dup2(fds[i-1][0], STDIN_FILENO);
			dup2(fds[i][1], STDOUT_FILENO);
			one_exec(ms, &cmd);
			close(fds[i-1][0]);
			close(fds[i][1]);
		}
		exit(EXIT_SUCCESS);
	}
	else
		return ;
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
			{
				close(fds[i][j]);
			}
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