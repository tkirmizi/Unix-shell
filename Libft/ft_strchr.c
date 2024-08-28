/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 21:16:00 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/03/20 21:21:47 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	unsigned int	i;
	char			d;

	i = 0;
	d = c;
	while (s[i])
	{
		if (s[i] == d)
			return ((char *) &s[i]);
		i++;
	}
	if (s[i] == d)
		return ((char *) &s[i]);
	return (NULL);
}
