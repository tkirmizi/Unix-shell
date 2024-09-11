/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taha <taha@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:47:07 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/09/11 18:18:31 by taha             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	execution(t_ms *ms) // main execution
{
	char  *path = "PATH";
	t_ms *temp = ms;
	int	count_command;
	
	count_command = ft_command_counter(ms->cmd);
	
	if (ft_is_builtin(ms) != 10)
		do_builtin(ms);
	else
	{

		while (temp->env_s != NULL)
		{
			if (ft_strncmp(temp->env_s->env_name, path, 4) == 0)
				break;
			temp->env_s = temp->env_s->next;
		}
		ms->all_cmd_paths = ft_split(temp->env_s->env_value, ':');
		arg_join(ms);
		// if (count_command == -5)
		// 	problem 
		if (count_command == 1) // one execution
			one_exec(ms);
		else
			multi_exec(ms);
	}
}

int		ft_command_counter(t_cmd *command)
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
	int	i;
	
	i = 0;
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
		multi_exec_cont(cmd, pids, fds, i, c_command);
		i++;
		cmd = cmd->next;
	}
}

void	multi_exec_cont(t_cmd *cmd, pid_t *pids, int fds[][2], int i, int c_command)
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
			// execution
			close(fds[0][1]);
		}
		if (i == c_command - 1)
		{
			cl_fds_last(fds, c_command);
			dup2(fds[c_command - 2][0], STDIN_FILENO);
			// execution
			close(fds[c_command- 2][0]);
		}
		else
		{
			cl_fds_middle(fds, c_command, i);
			dup2(fds[i - 1][0], STDIN_FILENO);
			dup2(fds[i][1], STDOUT_FILENO);
			//execution
			close(fds[i-1][0]);
			close(fds[i][1]);
		}
	}
	else
		waitpid(pids[i], NULL, 0);
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