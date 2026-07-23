/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ft_strlcat.c									   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: yanolive <marvin@42.fr>					+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/11/04 12:44:06 by yanolive		  #+#	#+#			 */
/*   Updated: 2023/11/04 12:44:08 by yanolive		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	j;
	size_t	tmp;

	if (size == 0)
		return (ft_strlen(src));
	i = ft_strlen(dst);
	j = 0;
	tmp = i;
	if (size < i)
		return (size + ft_strlen(src));
	while (i < (size - 1) && src[j])
	{
		dst[i] = src[j];
		++i;
		++j;
	}
	dst[i] = '\0';
	return (tmp + ft_strlen(src));
}
