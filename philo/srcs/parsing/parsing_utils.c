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
			return (negative ? LONG_MIN : LONG_MAX);

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