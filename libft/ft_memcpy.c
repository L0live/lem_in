/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ft_memcpy.c										:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: yanolive <marvin@42.fr>					+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/11/01 13:00:03 by yanolive		  #+#	#+#			 */
/*   Updated: 2023/11/01 13:00:04 by yanolive		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*tmpd;
	unsigned char	*tmps;

	if (!dest && !src && n > 0)
		return (NULL);
	tmpd = (unsigned char *)dest;
	tmps = (unsigned char *)src;
	while (n > 0)
	{
		*tmpd++ = *tmps++;
		--n;
	}
	return (dest);
}
