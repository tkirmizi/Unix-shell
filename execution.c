/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:47:07 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/04 11:25:40 by tkirmizi         ###   ########.fr       */
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
		// free_cmd(&(ms->cmd));
	}
	else
		multi_exec(&ms, count_command);
}

void	free_cmd(t_cmd **cmd)
{
	t_cmd *temp;

	temp = (*cmd);
	while (temp)
	{
		// if (temp->args)
		// 	free(temp->args);
		if (temp->path_for_excat)
			free(temp->path_for_excat);
		temp = temp->next;
	}
}

void one_exec(t_ms **ms, t_cmd **cmd)
{
	int	i;
	t_cmd *temp;

	int	j = 0;
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
		{
			ft_write_to_fd(2, "error: execution");
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

	pids = (pid_t *)malloc((c_command) * sizeof(pid_t));
	i = 0;
	while (i < c_command-1)
		pipe(fds[i++]); // protection should come here.
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
	i = 0;
	free(pids);
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
			dup2(cmd->fd_out, fds[0][1]);
			exec_for_multi(ms, &cmd);
			close(fds[0][1]);
		}
		else if(i == c_command-1)
		{
			cl_fds_last(fds, c_command);
			dup2(fds[c_command-2][0], STDIN_FILENO);
			dup2(cmd->fd_in, fds[c_command-2][0]);
			dup2(cmd->fd_out, STDOUT_FILENO);
			exec_for_multi(ms, &cmd);
			close(fds[c_command-2][0]);
		}
		else
		{
			cl_fds_middle(fds, c_command, i);
			dup2(fds[i-1][0], STDIN_FILENO);
			dup2(cmd->fd_in, fds[i-1][0]);
			dup2(fds[i][1], STDOUT_FILENO);
			dup2(cmd->fd_out, fds[i][1]);
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
				ft_write_to_fd(2, "error: execution");
				exit(127);
			}
		}
		else
		{
			ft_write_to_fd(2, "error: execution");
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



void	ft_set_builtin(t_cmd *cmd)
{
	cmd->builtin[0] = "cd";
	cmd->builtin[1] = "pwd";
	cmd->builtin[2] = "echo";
	cmd->builtin[3] = "env";
	cmd->builtin[4] = "export";
	cmd->builtin[5] = "unset";
}

int ft_is_builtin(t_cmd **cmd)
{
	int i;
	t_cmd *temp;

	i = 0;
	temp = (*cmd);
	ft_set_builtin(*cmd);
	
	while (i < 6)
	{
		if (!(ft_strncmp((*cmd)->args[0], (*cmd)->builtin[i], ft_strlen((*cmd)->args[0]))))
			return (i);
		i++;
	}
	return (10);
}

void	do_builtin(t_ms **ms, t_cmd **cmd)
{
	int i;
	i = ft_is_builtin(cmd);
	if (i == 0)
		do_cd(ms);
	if (i == 1)
		do_pwd(ms);
	if (i == 2)
		do_echo(ms);
	if (i == 3)
		do_env(ms);
	if (i == 4)
		do_export(ms);
	if (i == 5)
		do_unset(ms);
}

void	do_cd(t_ms **ms)
{
	t_ms	*temp;

	(*ms)->exit_code = 1;
	temp = (*ms);
	char	*new_pwd;
	if (temp->cmd->args[2])
		printf("Only one path can be followed.\n");
	else if (!(ft_strncmp(temp->cmd->args[1], "-", 2)))
		new_pwd = find_last_part(ms);
	else if (!(ft_strncmp(temp->cmd->args[1], "..", 3)))
		new_pwd = find_prev_path(ms);
	else if (!(ft_strncmp(temp->cmd->args[1], "/", 2)))
		new_pwd = ft_strdup("/");
	else
		new_pwd = cur_pwd_return(ms);
	if (access(new_pwd, F_OK))
		printf("There is no such as path \n");
	else
	{
		change_pwd_oldpwd(ms);
		chdir_getcwd_all(ms, new_pwd);
		update_path(ms);
		(*ms)->exit_code = 0;
	}
}

void	change_pwd_oldpwd(t_ms **ms)
{
	t_env *temp;

	char	*pwd_wcpy;
	temp = (*ms)->env_s;
	while (temp)
	{
		if (!(ft_strncmp(temp->env_name, "PWD", 3)))
			break;
		temp = temp->next;
	}
	pwd_wcpy = ft_strdup(temp->env_value);
	free(temp->env_value);
	temp = (*ms)->env_s;
	while (temp)
	{
		if (!(ft_strncmp(temp->env_name, "OLDPWD", 6)))
			break;
		temp = temp->next;
	}
	free(temp->env_value);
	temp->env_value = ft_strdup(pwd_wcpy);
}

void	chdir_getcwd_all(t_ms **ms, char *new_pwd)
{
	char	*cur_pwd;
	int		pwd_len;
	t_env	*temp;

	temp = (*ms)->env_s;
	pwd_len = ft_strlen(new_pwd) + 1;
	cur_pwd = (char*)malloc((pwd_len) * sizeof(char));
	cur_pwd[pwd_len - 1] = '\0';
	if (chdir(new_pwd))
	{
		perror("Problem on changing directory\n");
		exit(1); // will be changed ?
	}
	cur_pwd = getcwd(cur_pwd, pwd_len);
	while (temp)
	{
		if (!(ft_strncmp(temp->env_name, "PWD", 3)))
			break;
		temp = temp->next;
	}
	temp->env_value = ft_strdup(cur_pwd);
}

char	*find_prev_path(t_ms **ms)
{
	t_env	*temp;
	char	*new_pwd;
	char	*wbtrim;

	temp = (*ms)->env_s;
	while (temp)
	{
		if (!(ft_strncmp(temp->env_name, "PWD", 3)))
			break;
		temp = temp->next;
	}
	new_pwd = ft_strdup(temp->env_value);
	wbtrim = ft_strrchr(new_pwd, '/');
	*wbtrim = '\0';
	return (new_pwd);
}

char	*find_last_part(t_ms **ms)
{
	t_env *temp;

	temp = (*ms)->env_s;
	while (temp)
	{
		if (!(ft_strncmp(temp->env_name, "OLDPWD", 6)))
			break;
		temp = temp->next;
	}
	return (temp->env_value);
}

void	do_echo(t_ms **ms)
{
	t_cmd	*temp;
	int	n_flag;
	int	i;

	(*ms)->exit_code = 1;
	i = 1;
	n_flag = 0;
	temp = (*ms)->cmd;
	if (!temp->args[1])
		printf("\n");
	else
		echo_w_args(ms, temp, n_flag, i);
	(*ms)->exit_code = 0;
}

void	echo_w_args(t_ms **ms, t_cmd *temp, int n_flag, int i)
{
	if (!(ft_strncmp(temp->args[1], "-n", 2)))
		n_flag = 1;
	if (n_flag == 1)
		echo_n_flag(ms, temp);
	else if (n_flag == 0)
	{
		while (temp->args[i])
		{
			printf("%s", temp->args[i]);
			if (temp->args[i+1])
				printf(" ");
			else
				printf("\n");
			i++;
		}
	}
}


void	echo_n_flag(t_ms **ms, t_cmd *temp)
{
	int	i;
	if (!temp->args[2])
		;
	else
	{
		i = 2;
		while (temp->args[i])
		{
			printf("%s", temp->args[i]);
			if (temp->args[i+1])
				printf(" ");
			i++;
		}
		printf("%%\n");
	}
}

void	do_env(t_ms **ms)
{
	int	i;
	t_ms *temp;

	(*ms)->exit_code = 1;
	i = 0;
	temp = (*ms);
	dup2((*ms)->cmd->fd_in, STDIN_FILENO);
	dup2((*ms)->cmd->fd_out, STDOUT_FILENO);
	while (temp->env[i])
		printf("%s\n",temp->env[i++]);
	(*ms)->exit_code = 0;
}

char	*ft_strncpy(char *dst, const char *src, int len)
{
	int	i;

	i = 0;
	if (!src)
		return (NULL);
	while (i < len && src[i])
	{
		dst[i] = src[i];
		i++;
	}
	while (i < len)
	{
		dst[i] = '\0';
		i++;
	}
	return (dst);
}

void	do_exit(t_ms **ms)
{
	(*ms)->exit_code = 1;
	ft_write_to_fd_nl(STDERR_FILENO, "exit");
	if ((*ms)->cmd->args[1] && (*ms)->cmd->args[2])
		ft_write_to_fd_nl(STDERR_FILENO, "too many arguments");
	else if ((*ms)->cmd->args[1])
	{
		if (!(ft_is_num((*ms)->cmd->args[1])))
		{
			(*ms)->exit_code = 255;
			ft_write_to_fd_nl(STDERR_FILENO, "numeric argument required");
		}
		else 
			(*ms)->exit_code =  ft_atoi((*ms)->cmd->args[1]);
	}
	else
		(*ms)->exit_code = 0;
	exit((*ms)->exit_code);
}

int	ft_is_num(char	*string)
{
	int	i;

	i = 0;
	while (string[i])
	{
		if (string[i] < 48 || string[i] > 57)
			return (0);
		i++;
	}
	return (1);
}

void	ft_write_to_fd_nl(int fd, char *string)
{
	int	i;

	i = 0;
	while (string[i])
		write(fd, &string[i++], 1);
	write(fd, "\n", 1);
}

void	ft_write_to_fd(int fd, char *string)
{
	int	i;

	i = 0;
	while (string[i])
		write(fd, &string[i++], 1);
}


void	do_export(t_ms **ms) // without argument will handle.
{
	int	i;
	t_ms *temp;

	(*ms)->exit_code = 1;
	temp = (*ms);
	i = 1;
	while (temp->cmd->args[i])
	{
		check_export_exist(ms, temp->cmd->args[i]);
		i++;
	}
	i = 1;
	temp = (*ms);
	while (temp->cmd->args[i])
	{
		export_itself(ms, temp->cmd->args[i]);
		i++;
	}
	update_path(ms);
	(*ms)->exit_code = 0;
}


void	export_itself(t_ms **ms, char *string)
{
	t_env *temp;
	char *before_eq = NULL;
	char *after_eq;
	int	before_len;

	temp = (*ms)->env_s;
	after_eq = ft_strnstr(string, "=", ft_strlen(string));
	if (after_eq != NULL)
	{
		before_len = after_eq - string;
		before_eq = malloc(before_len + 1);
		if (!before_eq)
		{
			perror("Failed to allocate memory");
			exit(1);
		}
		ft_strncpy(before_eq, string, before_len);
		before_eq[before_len] = '\0';
	}
	while (temp-> next != NULL)
	{
		if (!(ft_strncmp(temp->env_name, before_eq, before_len)))
		{
			free(temp->env_value);
			temp->env_value = ft_strdup(after_eq + 1);
			break;
		}
		temp = temp->next;
	}
	if (temp->next == NULL)
	{
		temp->next = (t_env*)malloc(sizeof(t_env));
		temp->next->env_name = ft_strdup(before_eq);
		temp->next->env_value = ft_strdup(after_eq + 1);
		temp->next->next = NULL;
	}
}

void	check_export_exist(t_ms **ms, char *string)
{
	t_env *temp;
	char *before_eq = NULL;
	char *after_eq;
	int	before_len;

	temp = (*ms)->env_s;
	after_eq = ft_strnstr(string, "=", ft_strlen(string));
	if (after_eq != NULL)
	{
		before_len = after_eq - string;
		before_eq = malloc(before_len + 1);
		if (!before_eq)
		{
			perror("Failed to allocate memory");
			exit(1);
		}
		ft_strncpy(before_eq, string, before_len);
		before_eq[before_len] = '\0';
	}
	while (temp)
	{
		if (!(ft_strncmp(temp->env_name, before_eq, before_len)))
		{
			free(temp->env_value);
			temp->env_value = strdup("");
		}
		temp = temp->next;
	}
	free(before_eq);
}

void	update_path(t_ms **ms)
{
	t_env	*temp;
	int	i;

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
	while (temp != NULL) // buraya niye girmiyor ?
	{
		(*ms)->env[i] = ft_strdup(temp->env_name);
		(*ms)->env[i] = ft_strjoin((*ms)->env[i], "=");
		(*ms)->env[i] = ft_strjoin((*ms)->env[i], temp->env_value);
		i++;
		temp = temp->next;
	}
}

void	do_pwd(t_ms **ms)
{
	t_env *temp;

	(*ms)->exit_code = 1;
	temp = (*ms)->env_s;
	while (temp)
	{
		if (!(ft_strncmp(temp->env_name, "PWD", 3)))
			break;
		temp = temp->next;
	}
	printf("%s\n", temp->env_value);
	(*ms)->exit_code = 0;
}

void	do_unset(t_ms **ms)
{
	t_env *temp;
	int	i;

	(*ms)->exit_code = 1;
	i = 1;
	temp = (*ms)->env_s;
	while ((*ms)->cmd->args[i] != NULL)
	{
		unset_itself(ms, (*ms)->cmd->args[i]);
		i++;
	}
	(*ms)->exit_code = 0;
}
void	unset_itself(t_ms **ms, char *string)
{
	t_env *temp;
	t_env *temp2;
	temp = (*ms)->env_s;
	while (temp)
	{
		if (!(ft_strncmp(string, temp->env_name, ft_strlen(string))))
			break;
		temp2 = temp;
		temp = temp->next;
	}
	if (!temp)
		printf("There is no variable such as '%s'\n", string);
	else if (temp->next == NULL)
	{
		temp2->next = NULL;
		free(temp);
		update_path(ms);
	}
	else
	{
		unset_else(ms, temp, temp2, string);
		update_path(ms);
	}
}
void	unset_else(t_ms **ms, t_env *temp, t_env *temp2, char *string)
{
	if (!(ft_strncmp(temp->env_name, (*ms)->env_s->env_name, ft_strlen(temp->env_name))))
	{
		(*ms)->env_s = (*ms)->env_s->next;
		temp->next = NULL;
		free(temp);
	}
	else
	{
		temp2->next = temp->next;
		temp->next = NULL;
		temp->env_name = NULL;
		temp->env_value = NULL;
		free(temp);
	}
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
	// update_path(ms);
}