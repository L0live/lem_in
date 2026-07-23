/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ft_substr.c										:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: yanolive <marvin@42.fr>					+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/10/30 12:22:45 by yanolive		  #+#	#+#			 */
/*   Updated: 2023/11/04 23:43:57 by yanolive		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "libft.h"

static char	*good_malloc(char const *s, unsigned int start, size_t len);

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t			i;
	unsigned char	*rtn;

	if (!s)
		return (NULL);
	i = 0;
	rtn = (unsigned char *)good_malloc(s, start, len);
	if (!rtn)
		return (NULL);
	if (start >= (size_t)ft_strlen(s))
		return ((char *)rtn);
	while (s[start] && len > 0)
	{
		rtn[i] = (unsigned char)s[start];
		++i;
		++start;
		--len;
	}
	rtn[i] = '\0';
	return ((char *)rtn);
}

static char	*good_malloc(char const *s, unsigned int start, size_t len)
{
	size_t			slen;
	unsigned char	*rtn;

	slen = (size_t)ft_strlen(s);
	if (start > slen)
		rtn = ft_calloc(1, sizeof(char));
	else if (len > (slen - start))
		rtn = ft_calloc((slen - start) + 1, sizeof(char));
	else
		rtn = ft_calloc(len + 1, sizeof(char));
	return ((char *)rtn);
}
