/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 15:29:49 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/09/04 12:14:03 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **env)
{
	t_ms *ms;
	while (1)
	{
		execution(ms->cmd);
	}
}



void	execution(t_ms *ms) // main execution
{
	char  *path = "PATH";
	t_ms *temp = ms;
	int	count_command;
	count_command = ft_command_counter(ms->cmd);
	
	if (ft_is_builtin(ms))
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
// 		else
// 			multi_exec(ms);
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



void	ft_set_builtin(t_ms *ms)
{
	ms->bin.builtin[0] = "cd";
	ms->bin.builtin[1] = "pwd";
	ms->bin.builtin[2] = "echo";
	ms->bin.builtin[3] = "env";
	ms->bin.builtin[4] = "export";
	ms->bin.builtin[5] = "unset";
	ms->bin.builtin[6] = "exit";
}

int ft_is_builtin(t_ms *ms)
{
	int i;

	i = 0;
	ft_set_builtin(ms);
	
	while (i < 7)
	{
		if (!(ft_strncmp(ms->cmd->args[0], ms->bin.builtin[i], sizeof(ms->cmd->args[0]))))
			return (i); // builtin demek yani
		i++;
	}
	return (0);
}

void	do_builtin(t_ms *ms)
{
	int i = ft_is_builtin(ms);
	// if (i == 0)
	// 	do_cd();
	if (i == 1)
		do_pwd(ms);
	// if (i == 2)
	// 	do_echo();
	if (i == 3)
		do_env(ms);
	if (i == 4)
		do_export(&ms);
	// if (i == 5)
	// 	do_unset();
	// if (i == 6)
	// 	do_exit();
}

void	do_pwd(t_ms *ms)
{
	printf("pwd will bi done\n");
}

void	do_env(t_ms *ms)
{
	int	i;

	i = 0;
	while (ms->env[i])
		printf("%s\n",ms->env[i++]);
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
	// ft_env_checker(&((*ms)->env_s));
	// ft_env_double_checker((*ms)->env);
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

void	do_unset(t_ms **ms)
{
	t_env *temp;
	int	i;

	i = 1;
	temp = (*ms)->env_s;
	while ((*ms)->cmd->args[i] != NULL)
	{
		unset_itself(ms, (*ms)->cmd->args[i]);
		i++;
	}
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
