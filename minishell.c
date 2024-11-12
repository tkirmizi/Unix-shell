/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 10:49:58 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/12 10:50:02 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void leaks (void)
{
	system("leaks minishell");
}



int g_signal = 0;

void free_commands(t_cmd *cmd) {
  t_cmd *tmp;
  int i;

	tmp = NULL;
	while (cmd)
	{
	tmp = cmd->next;
	i = 0;
	while (cmd->args && cmd->args[i])
	  free(cmd->args[i++]);
	free(cmd->args);
	free(cmd);
	cmd = tmp;
	}
}

int not_empty(const char *input) {
  int i;

	i = 0;
	while (input[i]) {
		if (input[i] != ' ' && input[i] != '\t')
			return (1);
		i++;
	}
	return (0);
}

int wrong_input(char *input) {
	if (!input)
		return (printf("ERROR! No input"), 0);
	if (ft_strlen(input) == 0 || input[0] == '\0' || !not_empty(input))
		return (free(input), 1);
	if (ft_strncmp(input, "clear", 5) == 0)
		return (printf("\033[H\033[J"), free(input), 1);
	return (0);
}

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

int		ft_command_counter(t_cmd **command)
{
	t_cmd *temp;
	int	i;

	i = 0;
	temp = (*command);
	if (temp -> args == NULL)
		return (i);
	else
	{
		if (temp -> next == NULL)
			return (1);
		else
		{
			while (temp -> next != NULL)
			{
				i++;
				temp = temp -> next;
			}
			return (i + 1);
		}
	}
	return (-5); // problem.
}

void  find_exact_path(t_ms **ms, t_cmd **cmd, int	*i)
{
	t_cmd *temp;

	temp = (*cmd);
	*i = 0;
	while (temp->path_for_excat[*i])
	{
		if (access(temp->path_for_excat[*i], X_OK | F_OK) == 0)
			return ;
		(*i)++;
	}
	ft_write_to_fd(2, "command not found\n");
	exit(127);
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

void	do_export(t_ms **ms) // if just export without arguments what will happen ?
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


void	update_path(t_ms **ms) // will be used also for unset
{
	t_env	*temp;
	int		i;
	char	*tmp;

	i = 0;
	temp = (*ms)->env_s;
	// free_dbl_ptr((*ms)->env);
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
		tmp = (*ms)->env[i];
		(*ms)->env[i] = ft_strjoin((*ms)->env[i], "=");
		free(tmp);
		tmp = (*ms)->env[i];
		(*ms)->env[i] = ft_strjoin((*ms)->env[i], temp->env_value);
		free(tmp);
		i++;
		temp = temp->next;
	}
}


void	free_dbl_ptr(char **string)
{
	int i = 0;

	if (string)
	{
		while (string[i])
		{
			free(string[i]);
			i++;
		}
		// free(string);
	}
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

// char	*cur_pwd_return(t_ms **ms)
// {
// 	t_env *temp;
// 	char	*new_pwd;

// 	temp = (*ms)->env_s;
// 	while (temp)
// 	{
// 		if (!(ft_strncmp(temp->env_name, "PWD", 3)))
// 			break;
// 		temp = temp->next;
// 	}
// 	new_pwd = ft_strdup(temp->env_value);
// 	new_pwd = ft_strjoin(new_pwd, "/");
// 	new_pwd = ft_strjoin(new_pwd, (*ms)->cmd->args[1]);
// 	return new_pwd;
// }


char	*cur_pwd_return(t_ms **ms)
{
	t_env	*temp;
	char	*new_pwd;
	char	*tmp;

	temp = (*ms)->env_s;
	while (temp)
	{
		if (!(ft_strncmp(temp->env_name, "PWD", 3)))
			break;
		temp = temp->next;
	}
	new_pwd = ft_strdup(temp->env_value);
	tmp = new_pwd; 
	new_pwd = ft_strjoin(new_pwd, "/");
	free(tmp);
	tmp = new_pwd;
	new_pwd = ft_strjoin(new_pwd, (*ms)->cmd->args[1]);
	free(tmp);
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

// void	do_pwd(t_ms **ms)
// {
// 	t_env *temp;

// 	(*ms)->exit_code = 1;
// 	temp = (*ms)->env_s;
// 	while (temp)
// 	{
// 		if (!(ft_strncmp(temp->env_name, "PWD", 3)))
// 			break;
// 		temp = temp->next;
// 	}
// 	printf("%s\n", temp->env_value);
// 	(*ms)->exit_code = 0;
// }

void	do_pwd(t_ms **ms)
{
	t_env	*temp;
	int		i;
	int		len;

	i = 0;
	(*ms)->exit_code = 1;
	temp = (*ms)->env_s;
	while (temp)
	{
		if (!(ft_strncmp(temp->env_name, "PWD", 3)))
			break;
		temp = temp->next;
	}
	dup2((*ms)->cmd->fd_out, STDOUT_FILENO);
	dup2((*ms)->cmd->fd_in, STDIN_FILENO);
	len = ft_strlen(temp->env_value);
	write(STDOUT_FILENO, temp->env_value, len);
	write(STDOUT_FILENO, "\n", 1);
	// close((*ms)->cmd->fd_out);
	// printf("%s\n", temp->env_value);
	dup2(STDOUT_FILENO, (*ms)->cmd->fd_out);
	dup2(STDIN_FILENO, (*ms)->cmd->fd_in);
	(*ms)->exit_code = 0;
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
	dup2((*ms)->cmd->fd_in, STDIN_FILENO);
	dup2((*ms)->cmd->fd_out, STDOUT_FILENO);
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

void	do_exit(t_ms **ms)
{
	(*ms)->exit_code = 1;
	ft_write_to_fd(STDERR_FILENO, "exit");
	if ((*ms)->cmd->args[1] && (*ms)->cmd->args[2])
		ft_write_to_fd(STDERR_FILENO, "too many arguments");
	else if ((*ms)->cmd->args[1])
	{
		if (!(ft_is_num((*ms)->cmd->args[1])))
		{
			(*ms)->exit_code = 255;
			ft_write_to_fd(STDERR_FILENO, "numeric argument required");
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

void	ft_write_to_fd(int fd, char *string)
{
	int	i;

	i = 0;
	while (string[i])
		write(fd, &string[i++], 1);
	write(fd, "\n", 1);
}

void	token_writter(t_ms **ms)
{
	t_token *temp;
	
	temp = (*ms)->token;
	while (temp)
	{
		printf("type = %u, value = %s\n", temp->type, temp->value);
		temp = temp->next;
	}
}

void	all_path_joiner(t_ms **ms, t_cmd **cmd)
{
	t_ms	*temp;

	temp = (*ms);
	while (temp->env_s)
	{
		if (ft_strncmp(temp->env_s->env_name, "PATH", 4) == 0)
			break;
		temp->env_s = temp->env_s->next;
	}
	(*cmd)->path_for_excat = ft_split(temp->env_s->env_value, ':');
}


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

static void	init_pipes(t_pipe_data *data)
{
	int	i;

	i = 0;
	while (i < data->cmd_count - 1)
	{
		if (pipe(data->fds[i]) == -1)
			exit(EXIT_FAILURE);
		i++;
	}
}

static void	close_pipe_fds(t_pipe_data *data)
{
	int	i;

	i = 0;
	while (i < data->cmd_count - 1)
	{
		close(data->fds[i][0]);
		close(data->fds[i][1]);
		i++;
	}
}

static void	wait_for_children(t_ms **ms, t_pipe_data *data)
{
	int	i;
	int	status;

	i = 0;
	while (i < data->cmd_count)
	{
		waitpid(data->pids[i], &status, 0);
		(*ms)->exit_code = WEXITSTATUS(status);
		i++;
	}
}

void	multi_exec(t_ms **ms, int cmd_count)
{
	t_pipe_data	data;
	t_cmd		*cmd;
	int			fds[cmd_count - 1][2];

	data.fds = fds;
	data.cmd_count = cmd_count;
	data.pids = (pid_t *)malloc(cmd_count * sizeof(pid_t));
	data.cmd_index = 0;
	if (!data.pids)
		return ;
	init_pipes(&data);
	cmd = (*ms)->cmd;
	while (cmd)
	{
		multi_exec_cont(ms, cmd, &data);
		data.cmd_index++;
		cmd = cmd->next;
	}
	close_pipe_fds(&data);
	wait_for_children(ms, &data);
	free(data.pids);
}

static void	handle_first_cmd(t_cmd *cmd, t_pipe_data *data)
{
	cl_fds_first(data->fds, data->cmd_count);
	dup2(data->fds[0][1], STDOUT_FILENO);
	dup2(cmd->fd_out, data->fds[0][1]);
	close(data->fds[0][1]);
}

static void	handle_last_cmd(t_cmd *cmd, t_pipe_data *data)
{
	cl_fds_last(data->fds, data->cmd_count);
	dup2(data->fds[data->cmd_count - 2][0], STDIN_FILENO);
	dup2(cmd->fd_in, data->fds[data->cmd_count - 2][0]);
	dup2(cmd->fd_out, STDOUT_FILENO);
	close(data->fds[data->cmd_count - 2][0]);
}

static void	handle_middle_cmd(t_cmd *cmd, t_pipe_data *data)
{
	cl_fds_middle(data->fds, data->cmd_count, data->cmd_index);
	dup2(data->fds[data->cmd_index - 1][0], STDIN_FILENO);
	dup2(cmd->fd_in, data->fds[data->cmd_index - 1][0]);
	dup2(data->fds[data->cmd_index][1], STDOUT_FILENO);
	dup2(cmd->fd_out, data->fds[data->cmd_index][1]);
	close(data->fds[data->cmd_index - 1][0]);
	close(data->fds[data->cmd_index][1]);
}

void	multi_exec_cont(t_ms **ms, t_cmd *cmd, t_pipe_data *data)
{
	data->pids[data->cmd_index] = fork();
	if (data->pids[data->cmd_index] == -1)
		exit(EXIT_FAILURE);
	if (data->pids[data->cmd_index] == 0)
	{
		if (data->cmd_index == 0)
			handle_first_cmd(cmd, data);
		else if (data->cmd_index == data->cmd_count - 1)
			handle_last_cmd(cmd, data);
		else
			handle_middle_cmd(cmd, data);
		exec_for_multi(ms, &cmd);
		exit(EXIT_SUCCESS);
	}
}

void	cl_fds_first(int (*fds)[2], int cmd_count)
{
	int	i;
	int	j;

	i = 0;
	close(fds[0][0]);
	while (i < cmd_count - 1)
	{
		j = 0;
		while (j < 2)
		{
			if (i != 0)
				close(fds[i][j]);
			j++;
		}
		i++;
	}
}

void	cl_fds_last(int (*fds)[2], int cmd_count)
{
	int	i;
	int	j;

	i = 0;
	while (i < cmd_count - 1)
	{
		j = 0;
		while (j < 2)
		{
			if (i != cmd_count - 2)
				close(fds[i][j]);
			j++;
		}
		i++;
	}
	close(fds[cmd_count - 2][1]);
}

void	cl_fds_middle(int (*fds)[2], int cmd_count, int index)
{
	int	i;

	i = 0;
	while (i < cmd_count - 1)
	{
		if (i != index - 1)
			close(fds[i][0]);
		if (i != index)
			close(fds[i][1]);
		i++;
	}
}



void ft_set_env(char *name, char *value, int overwrite, t_env *env_s)
{
  t_env *tmp;
  char *new_value;

  tmp = env_s;
  while (tmp) {
    if (ft_strncmp(tmp->env_name, name, ft_strlen(name)) == 0) {
      if (overwrite == 1) {
        new_value = ft_strdup(value);
        free(tmp->env_value);
        tmp->env_value = new_value;
      }
      return;
    }
    tmp = tmp->next;
  }
  ft_set_node(&tmp, name, value);
  env_s = tmp;
}

int ft_isspace(int c) {
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' ||
		c == '\r');
}


void	args_writter(t_cmd *cmd)
{
	int	i = 0;
	int	j = 0;
	if (cmd -> next == NULL)
	{
		for (int i = 0; cmd->args[i]; i++)
			printf("args[%d] is %s\n",i, cmd->args[i]);
	}
	else
	{
		while (cmd)
		{
			i = 0;
			while (cmd->args[i])
			{
				printf(" %d. args[%d] = %s\n",j, i, cmd->args[i]);
				i++;
			}
			j++;
			cmd = cmd->next;
		}
	}
}

void	ft_env_checker(t_env **env)
{
	t_env *temp;
	temp = (*env);
	while (temp)
	{
		printf("env_name  %s\n", temp->env_name);
		printf("env_value  %s\n", temp->env_value);
		temp = temp->next;
	}
}

void	ft_env_double_checker(char	**string)
{
	int i = 0;
	while (string[i])
	{
		printf("string[%d] = %s\n",i, string[i]);
		i++;
	}
}



int main(int argc, char **argv, char **envp)
{
	atexit(&leaks);
	t_ms ms;
	t_env *env_s;

	signal_handler();
	env_s = NULL;
	ms.env = envp;
	env_s = ft_save_env(ms.env, env_s);
	ms.env_s = env_s;
	if (argc != 1)
		return (1);
	using_history();
	rl_bind_key('\t', rl_complete);
	while (1) // burdan sonrasi ayni
		if (!process(&ms))
			break;
	return (clear_history(), (void)argv, (void)argc, 0);
}