/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ft_calloc.c										:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: yanolive <marvin@42.fr>					+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/11/04 19:48:12 by yanolive		  #+#	#+#			 */
/*   Updated: 2023/11/04 19:48:14 by yanolive		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*rtn;

	if (size != 0 && nmemb > (SIZE_MAX - 1) / size)
		return (NULL);
	rtn = malloc(nmemb * size);
	if (!rtn)
		return (NULL);
	ft_bzero(rtn, nmemb * size);
	return (rtn);
}
