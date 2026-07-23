/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ft_memchr.c										:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: yanolive <marvin@42.fr>					+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/11/04 18:35:12 by yanolive		  #+#	#+#			 */
/*   Updated: 2023/11/04 18:35:14 by yanolive		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*tmp;

	i = 0;
	if (!s)
		return (NULL);
	tmp = (unsigned char *)s;
	while (n > 0)
	{
		if (tmp[i] == (unsigned char)c)
			return (&tmp[i]);
		++i;
		--n;
	}
	return (NULL);
}
