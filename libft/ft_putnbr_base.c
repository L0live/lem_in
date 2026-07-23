/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ft_putnbr_base.c								   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: yanolive <marvin@42.fr>					+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/11/12 15:12:34 by yanolive		  #+#	#+#			 */
/*   Updated: 2023/11/12 15:12:35 by yanolive		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putnbr_base(long long int n, char *base, int count)
{
	int	nbase;

	nbase = ft_strlen(base);
	if (n < 0)
	{
		n *= -1;
		count += ft_putchar('-');
	}
	if (n >= nbase)
	{
		count = ft_putnbr_base(n / nbase, base, count);
		count = ft_putnbr_base(n % nbase, base, count);
	}
	else
		count += ft_putchar(base[n]);
	return (count);
}

int	ft_putvnbr_base(unsigned long int n, char *base, int count)
{
	unsigned long int	nbase;

	nbase = (unsigned long int)ft_strlen(base);
	if (n == 0 && count == 0)
		return (ft_putstr("(nil)"));
	if (count == 0)
		count += ft_putstr("0x");
	if (n >= nbase)
	{
		count = ft_putvnbr_base(n / nbase, base, count);
		count = ft_putvnbr_base(n % nbase, base, count);
	}
	else
		count += ft_putchar(base[n]);
	return (count);
}
