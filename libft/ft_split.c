/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ft_split.c										 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: yanolive <marvin@42.fr>					+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/10/30 14:36:40 by yanolive		  #+#	#+#			 */
/*   Updated: 2023/10/30 14:36:44 by yanolive		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "libft.h"

static int	sep_count(char const *s, char c);
static char	**if_null(char **rtn);
static void	ft_var(int *i, int *k);

char	**ft_split(char const *s, char c)
{
	int		i;
	int		k;
	int		tmp;
	char	**rtn;

	ft_var(&i, &k);
	if (!s)
		return (NULL);
	rtn = ft_calloc(sizeof(char *), sep_count(s, c) + 1);
	if (!rtn)
		return (NULL);
	while (s[i])
	{
		while (s[i] == c)
			++i;
		if (s[i] == '\0')
			break ;
		tmp = i;
		while (s[i] != c && s[i])
			++i;
		rtn[k] = ft_substr(s, tmp, i - tmp);
		if (!rtn[k++])
			return (if_null(rtn));
	}
	return (rtn);
}

static void	ft_var(int *i, int *k)
{
	*i = 0;
	*k = 0;
}

static char	**if_null(char **rtn)
{
	int	k;

	k = 0;
	while (rtn[k])
	{
		free(rtn[k]);
		++k;
	}
	free(rtn);
	return (NULL);
}

static int	sep_count(char const *s, char c)
{
	int	i;
	int	rtn;

	i = 0;
	rtn = 0;
	while (s[i])
	{
		while (s[i] == c)
			++i;
		if (s[i] == '\0')
			break ;
		if (s[i + 1] == c || s[i + 1] == '\0')
			++rtn;
		++i;
	}
	return (rtn);
}
