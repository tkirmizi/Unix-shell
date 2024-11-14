/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:41:41 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/13 17:21:59 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_manage_redir_out(t_ms *mini, t_cmd *cmd, int *i)
{
	if (!cmd->args[*i + 1])
		return (1);
	if (!set_fd(cmd->args[*i], cmd->args[(*i) + 1], cmd, mini))
	{
		mini->exit_code = 1;
		return (0);
	}
	ft_del_array(&cmd->args, *i);
	ft_del_array(&cmd->args, *i);
	(*i)--;
	return (1);
}

int	ft_manage_redir_in(t_ms *mini, t_cmd *cmd, int *i)
{
	if (!cmd->args[*i + 1])
		return (1);
	if ((!ft_strcmp(cmd->args[*i], "<") || (!ft_strcmp(cmd->args[*i], "<<")))
		&& !ft_valid_file(cmd->args[(*i) + 1]))
	{
		mini->exit_code = 1;
		return (0);
	}
	if (!set_fd(cmd->args[*i], cmd->args[(*i) + 1], cmd, mini))
	{
		mini->exit_code = 1;
		return (0);
	}
	ft_del_array(&cmd->args, *i);
	ft_del_array(&cmd->args, *i);
	(*i)--;
	return (1);
}

int	ft_manage_redir_helper(t_ms *mini, t_cmd *cmd, int *i)
{
	if (cmd->args[*i][0] == '<')
	{
		if (!ft_manage_redir_in(mini, cmd, i))
			return (remove_args(cmd), 0);
	}
	else if (cmd->args[*i][0] == '>')
	{
		if (!ft_manage_redir_out(mini, cmd, i))
			return (remove_args(cmd), 0);
	}
	return (1);
}

int	ft_manage_redir(t_ms *mini)
{
	t_cmd	*temp;
	int		i;

	temp = mini->cmd;
	while (mini->cmd)
	{
		i = 0;
		while (mini->cmd->args[i])
		{
			if (!ft_manage_redir_helper(mini, mini->cmd, &i))
			{
				if (!mini->cmd->next)
					return (0);
				break ;
			}
			i++;
		}
		mini->cmd = mini->cmd->next;
	}
	mini->cmd = temp;
	return (1);
}

int	set_fd(char *re, char *path, t_cmd *cmd, t_ms *mini)
{
	set_fd_helper(re, path, cmd);
	if (!ft_strcmp(re, "<<"))
	{
		cmd->fd_in = open(".heredoc_all", O_CREAT | O_RDWR | O_TRUNC, 0644);
		heredoc_all(cmd->fd_in, path, mini);
		close(cmd->fd_in);
		cmd->fd_in = open(".heredoc_all", O_RDONLY);
	}
	if (cmd->fd_in == -1 || cmd->fd_out == -1)
	{
		perror("minishell");
		return (0);
	}
	return (1);
}
