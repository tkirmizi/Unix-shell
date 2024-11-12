/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 12:19:46 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/03/27 17:52:34 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*new_ptr;
	size_t	i;
	size_t	s1_len;

	s1_len = ft_strlen(s);
	i = 0;
	if (!s || !f)
		return (NULL);
	new_ptr = (char *)malloc(s1_len + 1 * sizeof(char));
	if (!new_ptr)
		return (NULL);
	while (i < s1_len)
	{
		new_ptr[i] = (*f)(i, s[i]);
		i++;
	}
	new_ptr[i] = '\0';
	return (new_ptr);
}
