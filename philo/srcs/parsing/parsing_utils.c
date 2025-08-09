/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldevoude <ldevoude@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 10:51:31 by ldevoude          #+#    #+#             */
/*   Updated: 2025/08/09 10:53:01 by ldevoude         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parsing.h"
#include <limits.h>

long	convert_ascii_to_long(int i, const char *nptr, int negative)
{
	long	result;

	result = 0;
	while (nptr[i] <= '9' && nptr[i] >= '0')
	{
		result += nptr[i] - 48;
		if (nptr[i + 1] <= '9' && nptr[i + 1] >= '0')
			result *= 10;
		if (result > (LONG_MAX - (nptr[i] - '0')) / 10)
		{
			if (negative)
				return (LONG_MIN);
			else
				return (LONG_MAX);
		}
		i++;
	}
	if (negative == true)
		result *= -1;
	return (result);
}

long	ft_atol(const char *nptr)
{
	int	i;
	int	negative;

	negative = false;
	i = 0;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == 32)
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			negative = true;
		i++;
	}
	return (convert_ascii_to_long(i, nptr, negative));
}

size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len] != '\0')
		len++;
	return (len);
}
