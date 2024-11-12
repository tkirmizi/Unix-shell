/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 21:16:09 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/03/20 21:24:23 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char		*dststr;
	const unsigned char	*srcstr;

	if (dst == 0 && src == 0)
		return (dst);
	dststr = dst;
	srcstr = src;
	while (n > 0)
	{
		*dststr++ = *srcstr++;
		n--;
	}
	return (dst);
}
