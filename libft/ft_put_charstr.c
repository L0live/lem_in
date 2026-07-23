/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ft_put_charstr.c								   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: yanolive <marvin@42.fr>					+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/11/12 15:11:50 by yanolive		  #+#	#+#			 */
/*   Updated: 2023/11/12 15:11:52 by yanolive		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putchar(char c)
{
	write(1, &c, 1);
	return (1);
}

int	ft_putstr(char *s)
{
	int	i;

	if (!s)
		return (ft_putstr("(null)"));
	i = 0;
	while (s[i])
	{
		ft_putchar(s[i]);
		++i;
	}
	return (i);
}
