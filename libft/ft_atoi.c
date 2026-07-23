/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ft_atoi.c										  :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: yanolive <marvin@42.fr>					+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/11/04 19:32:00 by yanolive		  #+#	#+#			 */
/*   Updated: 2023/11/04 19:32:01 by yanolive		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "libft.h"

static int	atoi_check(const char *nptr, int *minus);

int	ft_atoi(const char *nptr)
{
	int	i;
	int	minus;
	int	rtn;

	minus = 0;
	i = atoi_check(nptr, &minus);
	rtn = 0;
	if (i == -1)
		return (0);
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		rtn += (nptr[i] - '0');
		if (nptr[i + 1] >= '0' && nptr[i + 1] <= '9')
			rtn *= 10;
		i++;
	}
	if ((minus % 2) == 1)
		rtn *= -1;
	return (rtn);
}

static int	atoi_check(const char *nptr, int *minus)
{
	int	i;

	i = 0;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == ' ')
		++i;
	if (nptr[i] == '-')
	{
		*minus += 1;
		++i;
	}
	else if (nptr[i] == '+')
		++i;
	if (nptr[i] < '0' && nptr[i] > '9')
		return (-1);
	return (i);
}
