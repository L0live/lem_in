/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ft_memset.c										:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: yanolive <marvin@42.fr>					+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/10/31 19:50:45 by yanolive		  #+#	#+#			 */
/*   Updated: 2023/10/31 19:50:46 by yanolive		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*tmp;

	if (!s)
		return (NULL);
	tmp = s;
	while (n > 0)
	{
		*tmp++ = (unsigned char)c;
		--n;
	}
	return (s);
}
