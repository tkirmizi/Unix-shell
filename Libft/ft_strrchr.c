/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 21:16:11 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/03/21 00:43:38 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	unsigned int	i;
	char			*res;
	char			d;

	i = 0;
	d = c;
	res = NULL;
	while (s[i])
	{
		if (s[i] == d)
			res = ((char *) &s[i]);
		i++;
	}
	if (s[i] == d)
		res = ((char *) &s[i]);
	return (res);
}
