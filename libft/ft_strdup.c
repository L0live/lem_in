/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ft_strdup.c										:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: yanolive <marvin@42.fr>					+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/11/04 20:13:01 by yanolive		  #+#	#+#			 */
/*   Updated: 2023/11/04 20:13:02 by yanolive		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	int		i;
	char	*dst;
	char	*str;

	i = 0;
	if (!s)
		return (NULL);
	str = (char *)s;
	dst = (char *) malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!dst)
		return (NULL);
	while (str[i])
	{
		dst[i] = str[i];
		++i;
	}
	dst[i] = '\0';
	return (dst);
}
