/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   get_next_line_utils.c							  :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: yanolive <marvin@42.fr>					+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/11/16 12:45:01 by yanolive		  #+#	#+#			 */
/*   Updated: 2023/11/16 12:45:03 by yanolive		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "get_next_line.h"

int	gnl_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s && s[i])
		++i;
	return (i);
}

int	ft_checkn(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i] && str[i] != '\n')
		++i;
	if (str[i] == '\0')
		return (0);
	return (1);
}

char	*gnl_calloc(int scontent, int type)
{
	char	*rtn;
	int		i;

	i = 0;
	rtn = malloc(type * scontent);
	if (!rtn)
		return (NULL);
	while (i < scontent)
	{
		rtn[i] = '\0';
		++i;
	}
	return (rtn);
}

char	*gnl_strjoin(char *s1, char *s2)
{
	int		i;
	int		j;
	char	*rtn;

	i = 0;
	j = 0;
	rtn = gnl_calloc(gnl_strlen(s1) + gnl_strlen(s2) + 1, sizeof(char));
	if (!rtn)
		return (NULL);
	while (s1 && s1[i])
	{
		rtn[i] = s1[i];
		++i;
	}
	while (s2 && s2[j])
	{
		rtn[i + j] = s2[j];
		++j;
	}
	rtn[i + j] = '\0';
	if (s1)
		free(s1);
	if (s2)
		free(s2);
	return (rtn);
}

char	*gnl_strdup(char *s)
{
	int		i;
	char	*dst;

	i = 0;
	if (!s)
		return (NULL);
	dst = gnl_calloc(gnl_strlen(s) + 1, sizeof(char));
	if (!dst)
		return (NULL);
	while (s[i])
	{
		dst[i] = s[i];
		++i;
	}
	dst[i] = '\0';
	free(s);
	return (dst);
}
