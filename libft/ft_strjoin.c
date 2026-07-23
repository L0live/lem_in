/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ft_strjoin.c									   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: yanolive <marvin@42.fr>					+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/10/30 13:03:43 by yanolive		  #+#	#+#			 */
/*   Updated: 2023/10/30 13:03:47 by yanolive		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		i;
	int		j;
	char	*rtn;

	i = 0;
	j = 0;
	if (!s1 || !s2)
		return (NULL);
	rtn = ft_calloc(ft_strlen(s1) + ft_strlen(s2) + 1, sizeof(char));
	if (!rtn)
		return (NULL);
	while (s1[i])
	{
		rtn[i] = s1[i];
		++i;
	}
	while (s2[j])
	{
		rtn[i + j] = s2[j];
		++j;
	}
	rtn[i + j] = '\0';
	return (rtn);
}
