/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ft_strchr.c										:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: yanolive <marvin@42.fr>					+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/11/04 14:09:32 by yanolive		  #+#	#+#			 */
/*   Updated: 2023/11/04 14:09:34 by yanolive		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int		i;
	char	*d;

	i = 0;
	if (!s)
		return (NULL);
	d = (char *)s;
	while (d[i])
	{
		if (d[i] == (char)c)
			return (&d[i]);
		++i;
	}
	if (d[i] == (char)c)
		return (&d[i]);
	return (NULL);
}
