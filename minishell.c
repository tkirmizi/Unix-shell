/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 15:29:49 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/09/02 17:57:23 by tkirmizi         ###   ########.fr       */
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
	// if (i == 4)
	// 	do_export();
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

void	do_export(t_ms *ms) // if just export without arguments what will happen ?
{
	int	i;

	i = 1;
	while (ms->cmd->args[i])
	{
		printf("%s kontrol edilior\n", ms->cmd->args[i]);
		check_export_exist(ms, ms->cmd->args[i]);
		i++;
	}
	i = 1;
	// while (ms->cmd->args[i])
	// {
	// 	while (ms->env_s->next != NULL)
	// 		ms->env_s = ms->env_s->next;
	// 	ms->env_s->next->env_name = ms->cmd->args[0];
	// 	ms->env
	// }
}


void	check_export_exist(t_ms *ms, char *string)
{
t_ms *temp;
    char *before_eq;
    char *after_eq;

    after_eq = ft_strnstr(string, "=", ft_strlen(string));
    if (after_eq != NULL)
    {
        size_t before_len = after_eq - string;
        before_eq = malloc(before_len + 1);
        if (!before_eq)
        {
            perror("Failed to allocate memory");
            exit(1);
        }
        strncpy(before_eq, string, before_len); // strncpy will be created
        before_eq[before_len] = '\0';
        
        printf("before_eq = %s\n", before_eq);
        free(before_eq);
    }
    
    printf("after_eq = %s\n", after_eq);
// 	while (ms->env_s->next != NULL)
// 	{
// 		if (!(ft_strncmp(ms->env_s->env_name, ms->cmd->args[0], sizeof(ms->cmd->args[0]))))
// 		{
// 			printf("it is exist\n");
// 			free(ms->env_s->env_name);
// 			free(ms->env_s->env_value);
// 			temp->env_s->next = ms->env_s->next;
// 		}
// 		temp = ms;
// 		ms->env_s = ms->env_s->next;
// 	}
}