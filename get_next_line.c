/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: unite <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/04 03:21:37 by unite             #+#    #+#             */
/*   Updated: 2020/04/04 04:56:20 by unite            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "get_next_line.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

static int	fail_cleanup(char **cache)
{
	size_t	i;

	i = 0;
	while (i < MAX_FD + 1)
		free(cache[i++]);
	return (-1);
}

static int	get_line(char **cache, int fd, char **line)
{
	char	*nl;
	char	*tmp;

	if ((nl = ft_strchr(cache[fd], '\n')))
	{
		*line = ft_strsub(cache[fd], 0, nl - cache[fd]);
		tmp = ft_strdup(nl + 1);
		free(cache[fd]);
		cache[fd] = tmp;
		if (*line == NULL || cache[fd] == NULL)
			return (fail_cleanup(cache));
		if (*cache[fd] == '\0')
			ft_strdel(&cache[fd]);
	}
	else
	{
		*line = cache[fd];
		cache[fd] = 0;
	}
	return (1);
}

static int	output(char **cache, int fd, ssize_t ret, char **line)
{
	if (ret < 0)
		return (fail_cleanup(cache));
	if (ret == 0 && cache[fd] == NULL)
		return (0);
	else
		return (get_line(cache, fd, line));
}

int			get_next_line(const int fd, char **line)
{
	static char	*cache[MAX_FD + 1];
	char		buff[BUFF_SIZE + 1];
	char		*tmp;
	ssize_t		ret;

	if (fd > MAX_FD || line == NULL)
		return (-1);
	while ((ret = read(fd, buff, BUFF_SIZE)) > 0)
	{
		buff[ret] = '\0';
		if (cache[fd] == NULL)
			cache[fd] = ft_strdup(buff);
		else
		{
			tmp = ft_strjoin(cache[fd], buff);
			free(cache[fd]);
			cache[fd] = tmp;
		}
		if (cache[fd] == NULL)
			return (fail_cleanup(cache));
		if (ft_strchr(cache[fd], '\n'))
			break ;
	}
	return (output(cache, fd, ret, line));
}