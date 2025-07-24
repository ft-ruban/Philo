#include "../include/parsing.h"

//TODO Maybe deal with the highest double possible?

static double final_conversion_check(double result, double decimal_divisor, int negative)
{
    result = result / decimal_divisor;
	if (negative)
		result *= -1.0;
    return(result);
}

static double	convert_ascii_to_double(int i, const char *nptr, int negative,
		double result)
{
	double	decimal_divisor;
	int		is_fractional;

	decimal_divisor = 1.0;
	is_fractional = 0;
	while ((nptr[i] >= '0' && nptr[i] <= '9') || nptr[i] == '.'
		|| nptr[i] == ',')
	{
		if (nptr[i] == '.' || nptr[i] == ',')
		{
			if (is_fractional)
				break ;
			is_fractional = 1;
			i++;
			continue ;
		}
		result = result * 10 + (nptr[i] - '0');
		if (is_fractional)
			decimal_divisor *= 10.0;
		i++;
	}
    if(nptr[i])
        return(INVALID_INPUT);
    else
	    return (final_conversion_check(result,decimal_divisor,negative));
}

double	ft_atol(const char *nptr)
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
	return (convert_ascii_to_double(i, nptr, negative, 0.0));
}