/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdiego <fdiego@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/31 22:48:11 by fdiego            #+#    #+#             */
/*   Updated: 2021/01/22 10:05:04 by fdiego           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stddef.h"
#include <stdint.h>

static inline const char	*ft_strlen_check(const char *char_ptr)
{
	if (*char_ptr == 0)
		return (char_ptr);
	if (*(++char_ptr) == 0)
		return (char_ptr);
	if (*(++char_ptr) == 0)
		return (char_ptr);
	if (*(++char_ptr) == 0)
		return (char_ptr);
	if (*(++char_ptr) == 0)
		return (char_ptr);
	if (*(++char_ptr) == 0)
		return (char_ptr);
	if (*(++char_ptr) == 0)
		return (char_ptr);
	if (*(++char_ptr) == 0)
		return (char_ptr);
	return (NULL);
}

size_t				ft_strlen(const char *str)
{
	uint64_t const		*longword_ptr;
	char const *const	start_ptr = str;

	while ((((__uint64_t)str & (sizeof(__uint64_t) - 1)) != 0))
	{
		if (*str == '\0')
			return (str - start_ptr);
		str++;
	}
	longword_ptr = (__uint64_t*)str;
	while (1)
	{
		if (((*longword_ptr - 0x0101010101010101LL) &
				~(*longword_ptr) & 0x8080808080808080LL) != 0)
			return (ft_strlen_check((const char*)longword_ptr) - start_ptr);
		longword_ptr++;
	}
}
