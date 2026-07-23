/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   get_next_line.c									:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: yanolive <marvin@42.fr>					+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/11/16 12:45:09 by yanolive		  #+#	#+#			 */
/*   Updated: 2023/11/16 12:45:13 by yanolive		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "get_next_line.h"

static char	*ft_read(char *buffer, int fd);
static char	*ft_next(char *buffer);
static char	*ft_delline(char *buffer);

char	*get_next_line(int fd)
{
	static char	*buffer;
	char		*line;

	if (fd < 0 || BUFFER_SIZE < 1)
		return (NULL);
	buffer = ft_read(buffer, fd);
	line = ft_next(buffer);
	if (!ft_checkn(buffer))
	{
		free(buffer);
		buffer = NULL;
	}
	buffer = ft_delline(buffer);
	return (line);
}

static char	*ft_read(char *buffer, int fd)
{
	char	*tmp;
	char	*rtn;
	ssize_t	rd;

	if (buffer && ft_checkn(buffer))
		return (buffer);
	while (1)
	{
		tmp = gnl_calloc(BUFFER_SIZE + 1, sizeof(char));
		if (!tmp)
			return (NULL);
		rd = read(fd, tmp, BUFFER_SIZE);
		rtn = gnl_strjoin(buffer, tmp);
		if (rd <= 0 || ft_checkn(rtn))
			break ;
		buffer = gnl_strdup(rtn);
	}
	if (rtn && rtn[0] == '\0')
	{
		free(rtn);
		rtn = NULL;
	}
	return (rtn);
}

static char	*ft_next(char *buffer)
{
	char	*line;
	int		i;

	i = 0;
	if (!buffer)
		return (NULL);
	while (buffer[i] && buffer[i] != '\n')
		++i;
	++i;
	line = gnl_calloc(i + 1, sizeof(char));
	if (!line)
		return (NULL);
	i = 0;
	while (buffer[i] && buffer[i] != '\n')
	{
		line[i] = buffer[i];
		++i;
	}
	line[i] = buffer[i];
	return (line);
}

static char	*ft_delline(char *buffer)
{
	int		i;
	int		j;
	char	*newbuff;

	i = 0;
	j = 0;
	if (!buffer)
		return (NULL);
	while (buffer && buffer[i] && buffer[i] != '\n')
		++i;
	++i;
	newbuff = gnl_calloc((gnl_strlen(buffer) - i) + 1, sizeof(char));
	if (!newbuff)
		return (NULL);
	while (buffer && buffer[i + j])
	{
		newbuff[j] = buffer[i + j];
		++j;
	}
	free(buffer);
	if (newbuff[0])
		return (newbuff);
	free(newbuff);
	return (NULL);
}
