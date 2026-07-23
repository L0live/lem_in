/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ft_printf.c										:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: yanolive <marvin@42.fr>					+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/11/11 12:39:39 by yanolive		  #+#	#+#			 */
/*   Updated: 2023/11/12 20:39:52 by yanolive		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "ft_printf.h"

static int	choose_print(char c, va_list ap);

int	ft_printf(const char *str, ...)
{
	va_list	ap;
	int		count;
	int		i;

	count = 0;
	i = 0;
	if (!str)
		return (0);
	va_start(ap, str);
	while (str[i])
	{
		if (str[i] == '%')
		{
			count += choose_print(str[i + 1], ap);
			++i;
		}
		else
			count += ft_putchar(str[i]);
		if (str[i])
			++i;
	}
	va_end(ap);
	return (count);
}

static int	choose_print(char c, va_list ap)
{
	if (c == '%')
		return (ft_putchar('%'));
	if (c == 'c')
		return (ft_putchar((char)va_arg(ap, int)));
	if (c == 's')
		return (ft_putstr(va_arg(ap, char *)));
	if (c == 'p')
		return (ft_putvnbr_base(va_arg(ap, unsigned long int)
				, "0123456789abcdef", 0));
	if (c == 'd' || c == 'i')
		return (ft_putnbr_base(va_arg(ap, int), "0123456789", 0));
	if (c == 'u')
		return (ft_putnbr_base(va_arg(ap, unsigned int), "0123456789", 0));
	if (c == 'x')
		return (ft_putnbr_base(va_arg(ap, unsigned int)
				, "0123456789abcdef", 0));
	if (c == 'X')
		return (ft_putnbr_base(va_arg(ap, unsigned int)
				, "0123456789ABCDEF", 0));
	if (c != '\0')
		return (ft_putchar(c));
	return (0);
}
