/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   other.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 11:21:19 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/18 11:21:47 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	not_empty(const char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] != ' ' && input[i] != '\t')
			return (1);
		i++;
	}
	return (0);
}

int	handle_clear(char *input)
{
	write(1, "\033[H\033[J", 6);
	free(input);
	return (1);
}

int	check_input(char *input)
{
	if (!input)
	{
		write(2, "ERROR! No input\n", 15);
		return (0);
	}
	if (ft_strlen(input) == 0 || input[0] == '\0' || !not_empty(input))
	{
		free(input);
		return (1);
	}
	if (ft_strncmp(input, "clear", 5) == 0)
		return (handle_clear(input));
	return (0);
}

void	exit_before_process(t_ms **ms)
{
	if ((*ms)->input)
		free((*ms)->input);
	exit_w_out_free("error: parse", 1);
}
