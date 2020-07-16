/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: unite <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/04 03:21:37 by unite             #+#    #+#             */
/*   Updated: 2020/07/16 03:34:12 by unite            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "get_next_line.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

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

static int	check_arguments(const int fd, char **line)
{
	if (fd > MAX_FD || line == NULL)
	{
		errno = EINVAL;
		return (1);
	}
	return (0);
}

/*
** @brief Reads next line from a file descriptor.
** @details Reads a line from a file descriptor, where a line is defined as
** a succession of characters that end with ``'\n'`` or with `EOF`. The function
** can read from a file, the standard output, redirection etc, as well as
** multiple file descriptors simultaneously.
** @param[in] fd The file descriptor used to read.
** @param[out] line The address pointing to a string that is dynamically
** allocated by the function and used to save the line read from the file
** descriptor
** @return If successful, `1` is returned. Upon reading end-of-file zero is
** returned. Otherwise, a `-1` is returned and the global variable errno is
** set to indicate the error.
** @note The line is returned <u>without</u> ``'\n'``.
** @warning The function has an undefined behaviour if, between two calls, the
** same file descriptor designs two distinct files althugh the reading from the
** first file was not completed, or if a call to `lseek(2)` was made.
** @remark
** - The maximum number of file descriptors supported is controlled by the macro
** #MAX_FD. If the received file descriptor is larger than #MAX_FD, a `-1`
** is returned, and `errno` is set to `EINVAL`.
** - The size of the reading buffer is defined by the macro #BUFF_SIZE.
*/

int			get_next_line(const int fd, char **line)
{
	static char	*cache[MAX_FD + 1];
	char		buff[BUFF_SIZE + 1];
	char		*tmp;
	ssize_t		ret;

	if (check_arguments(fd, line))
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
