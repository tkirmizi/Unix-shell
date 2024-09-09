/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:55:39 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/09/09 10:56:01 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	do_env(t_ms **ms)
{
	int	i;
	t_ms *temp;

	(*ms)->exit_code = 1;
	i = 0;
	temp = (*ms);
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
