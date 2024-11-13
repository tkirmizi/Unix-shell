/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_manage.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkirmizi <tkirmizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:42:07 by tkirmizi          #+#    #+#             */
/*   Updated: 2024/11/13 12:43:04 by tkirmizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char **ft_realloc_array(char **ptr, size_t new_size)
{
	char **new_ptr;
	size_t i;

	i = -1;
	if (!ptr)
		return ((char **)malloc(new_size));
	new_ptr = (char **)malloc(new_size);
	if (!new_ptr)
		return (NULL);
	while (ptr[i] && ++i < (new_size / sizeof(char *) - 1))
	{
		new_ptr[i] = ft_strdup(ptr[i]);
		if (!new_ptr[i])
		{
			while (i > 0)
				free(new_ptr[--i]);
			free(new_ptr);
			return (NULL);
		}
	}
	// new_ptr[i] = NULL;
	i = 0;
	while (ptr[i])
		free(ptr[i++]);
	free(ptr);
	return (new_ptr);
}

void ft_strnjoin_helper(char *dest, const char *src, size_t n)
{
	size_t i;

	i = 0;
	while (i < n && src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
}

char *ft_strnjoin(char *s1, const char *s2, size_t n)
{
	char *str;
	size_t len1;
	size_t i;

	i = -1;
	if (!s2)
		return (s1);
	len1 = s1 ? ft_strlen(s1) : 0;
	if (n > ft_strlen(s2))
		n = ft_strlen(s2);
	str = (char *)malloc((len1 + n + 1) * sizeof(char));
	if (!str)
		return (NULL);
	while (++i < len1)
		str[i] = s1[i];
	i = -1;
	while (++i < n)
		str[len1 + i] = s2[i];
	str[len1 + i] = '\0';
	if (s1)
		free(s1);
	return (str);
}

void free_struct(t_env **stack)
{
	t_env *tmp;
	t_env *current;

	current = NULL;
	if (!stack)
		return;
	while (current)
	{
		tmp = current->next;
		free(current);
		current = tmp;
	}
	*stack = NULL;
}

t_env *find_last(t_env *env_s)
{
	if (NULL == env_s)
		return (NULL);
	while (env_s->next)
		env_s = env_s->next;
	return (env_s);
}