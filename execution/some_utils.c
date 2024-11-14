/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   some_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 13:50:37 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/14 19:22:42 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_is_num(char *string)
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

int	ft_isspace(int c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v'
		|| c == '\f' || c == '\r');
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

int	ft_command_counter(t_cmd **command)
{
	t_cmd	*temp;
	int		i;

	i = 0;
	temp = (*command);
	if (temp->args == NULL)
		return (i);
	else
	{
		if (temp->next == NULL)
			return (1);
		else
		{
			while (temp->next != NULL)
			{
				i++;
				temp = temp->next;
			}
			return (i + 1);
		}
	}
	return (-5);
}
