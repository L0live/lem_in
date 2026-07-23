/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ft_memmove.c									   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: yanolive <marvin@42.fr>					+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/11/01 13:15:28 by yanolive		  #+#	#+#			 */
/*   Updated: 2023/11/01 13:15:29 by yanolive		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "libft.h"

static void	write_mem(unsigned char *tmpd, unsigned char *tmps, size_t n);

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*tmpd;
	unsigned char	*tmps;

	if (!dest || !src)
		return (NULL);
	tmpd = (unsigned char *)dest;
	tmps = (unsigned char *)src;
	if (n == 0)
		return (dest);
	write_mem(tmpd, tmps, n);
	return (dest);
}

static void	write_mem(unsigned char *tmpd, unsigned char *tmps, size_t n)
{
	size_t	i;

	i = 0;
	if (tmpd < tmps)
	{
		while (i < n)
		{
			tmpd[i] = tmps[i];
			++i;
		}
	}
	else if (tmpd > tmps)
	{
		i = (n - 1);
		while ((i + 1) > 0)
		{
			tmpd[i] = tmps[i];
			--i;
		}
	}
	return ;
}
