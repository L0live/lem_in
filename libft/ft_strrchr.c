/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ft_strrchr.c									   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: yanolive <marvin@42.fr>					+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/11/04 14:29:05 by yanolive		  #+#	#+#			 */
/*   Updated: 2023/11/04 14:29:06 by yanolive		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int		i;
	char	*d;

	if (!s)
		return (NULL);
	d = (char *)s;
	i = ft_strlen(d);
	while (i >= 0)
	{
		if (d[i] == (char)c)
			return (&d[i]);
		--i;
	}
	return (NULL);
}
