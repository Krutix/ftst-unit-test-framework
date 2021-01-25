/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdiego <fdiego@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 15:20:04 by fdiego            #+#    #+#             */
/*   Updated: 2020/12/31 22:54:33 by fdiego           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	char *const	d = dest;
	size_t		big_word;

	big_word = n / sizeof(long);
	n %= sizeof(long);
	while (big_word-- != 0)
	{
		*(long*)dest = *(long*)src;
		dest += sizeof(long);
		src += sizeof(long);
	}
	while (n-- != 0)
		*(char*)dest++ = *(char*)(src++);
	return (d);
}
