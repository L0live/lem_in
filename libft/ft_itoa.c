/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ft_itoa.c										  :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: yanolive <marvin@42.fr>					+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/10/31 13:34:23 by yanolive		  #+#	#+#			 */
/*   Updated: 2023/10/31 13:34:26 by yanolive		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include <stdlib.h>

static char	*ft_itoa_bis(int *n, int *i);

char	*ft_itoa(int n)
{
	int		i;
	int		tmp;
	char	*rtn;

	i = 0;
	tmp = n;
	while (tmp != 0 || i == 0)
	{
		tmp /= 10;
		++i;
	}
	rtn = ft_itoa_bis(&n, &i);
	if (!rtn)
		return (NULL);
	if (n == 0)
		rtn[0] = '0';
	while (n != 0)
	{
		rtn[i] = (n % 10) + '0';
		n /= 10;
		--i;
	}
	return (rtn);
}

static char	*ft_itoa_bis(int *n, int *i)
{
	char	*rtn;

	if (*n < 0)
	{
		rtn = malloc(sizeof(char) * (*i + 2));
		if (!rtn)
			return (NULL);
		rtn[0] = '-';
		rtn[*i + 1] = '\0';
		if (*n == -2147483648)
		{
			rtn[*i] = '8';
			*i -= 1;
			*n /= 10;
		}
		*n *= -1;
		return (rtn);
	}
	rtn = malloc(sizeof(char) * (*i + 1));
	if (!rtn)
		return (NULL);
	*i -= 1;
	rtn[*i + 1] = '\0';
	return (rtn);
}
