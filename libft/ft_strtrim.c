/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ft_strtrim.c									   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: yanolive <marvin@42.fr>					+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/10/30 13:39:30 by yanolive		  #+#	#+#			 */
/*   Updated: 2023/10/30 13:39:34 by yanolive		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "libft.h"

static int	size_malloc(char const *s1, char const *set);
static void	ft_strtrim_bis(char **rtn, char const c, int *k, char const *set);
static int	endtrim(unsigned char *str, char const *sep);

char	*ft_strtrim(char const *s1, char const *set)
{
	int		i;
	int		k;
	int		tmp;
	char	*rtn;

	i = 0;
	k = 0;
	if (!s1)
		return (NULL);
	tmp = endtrim((unsigned char *)s1, set) - size_malloc(s1, set);
	if (tmp < 0)
		tmp = 0;
	rtn = ft_calloc(tmp + 1, sizeof(char));
	if (!rtn)
		return (NULL);
	while (s1[i])
	{
		if (i < endtrim((unsigned char *)s1, set))
		{
			rtn[k++] = (unsigned char)s1[i];
			ft_strtrim_bis(&rtn, s1[i], &k, set);
		}
		++i;
	}
	return (rtn);
}

static int	size_malloc(char const *s1, char const *set)
{
	int	i;
	int	j;

	i = 0;
	while (s1[i])
	{
		j = 0;
		while (set[j])
		{
			if (s1[i] == set[j])
				break ;
			++j;
		}
		if (set[j] == '\0')
			break ;
		++i;
	}
	return (i);
}

static void	ft_strtrim_bis(char **rtn, char const c, int *k, char const *set)
{
	int	j;

	j = 0;
	while (set[j] && *k == 1)
	{
		if (c == set[j])
		{
			*k -= 1;
			*rtn[*k] = '\0';
			break ;
		}
		++j;
	}
}

static int	endtrim(unsigned char *str, char const *sep)
{
	int	i;
	int	j;
	int	tmp;

	i = ft_strlen((const char *)str) - 1;
	while ((i + 1) > 0)
	{
		j = 0;
		tmp = i;
		while (sep[j])
		{
			if (str[i] == sep[j])
			{
				--i;
				break ;
			}
			++j;
		}
		if (i == tmp)
			break ;
	}
	return (i + 1);
}
