/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: unite <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/13 19:38:02 by unite             #+#    #+#             */
/*   Updated: 2019/10/01 06:06:40 by unite            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "get_next_line.h"
#include <unistd.h>
#include <stdlib.h>

static int			flush(t_list **cache, int eoltrail, char **line)
{
	t_list	*cache_link_current;
	size_t	len;

	cache_link_current = *cache;
	len = 0;
	while (cache_link_current)
	{
		len += ft_strlen(cache_link_current->content);
		cache_link_current = cache_link_current->next;
	}
	if (len == 0)
		return (0);
	else if (!(*line = ft_strnew(len - eoltrail)))
		return (-1);
	else
	{
		cache_link_current = *cache;
		while (cache_link_current)
		{
			ft_strlcat(*line, cache_link_current->content, len - eoltrail + 1);
			cache_link_current = cache_link_current->next;
		}
		ft_lstdel_str(cache);
		return (1);	
	}
}

static int			get_next_line_recursive(t_fd_cache *fd_cache, char **line)
{
	int		rdbytes;
	t_list	*tmp_link;
	t_list  *last_link;
	char	*new_line;
	int		ret;

	last_link = ft_lstlast(fd_cache->cache);
	if (!fd_cache->cache ||
		!(new_line = ft_strchr(last_link->content, EOL)))
	{
		if (!(tmp_link = ft_lstnew_str_empty(BUFF_SIZE + 1)) ||
			(rdbytes = read(fd_cache->fd, tmp_link->content, BUFF_SIZE)) < 0)
			return (-1);
		ft_lstappend(&(fd_cache->cache), tmp_link);
		if (rdbytes > 0)
			return (get_next_line_recursive(fd_cache, line));
		if (rdbytes == 0)
			return (flush(&(fd_cache->cache), 0, line));
	}
	tmp_link = ft_strlen(new_line + 1) ? ft_lstnew_str(new_line + 1) : NULL;
	ft_strclr(new_line + 1);
	ret = flush(&(fd_cache->cache), 1, line);
	ft_lstappend(&(fd_cache->cache), tmp_link);
	return (ret);
}

static t_list		*del_fd_cache(int fd, t_list *fd_cache_list)
{
	t_list		*fd_cache_link_current;
	t_list		*fd_cache_link_previous;
	t_fd_cache	*fd_cache;

	fd_cache_link_current = fd_cache_list;
	fd_cache_link_previous = NULL;
	while (fd_cache_link_current)
	{
		fd_cache = (t_fd_cache *)fd_cache_link_current->content;
		if (fd_cache->fd == fd)
		{
			free(fd_cache);
			if (fd_cache_link_previous)
				fd_cache_link_previous->next = fd_cache_link_current->next;
			else
				fd_cache_list = fd_cache_link_current->next;
			free(fd_cache_link_current);
			return (fd_cache_list);
		}
		fd_cache_link_previous = fd_cache_link_current;
		fd_cache_link_current = fd_cache_link_current->next;
	}
	return (NULL);
}

static t_fd_cache	*get_fd_cache(int fd, t_list const *fd_cache_list)
{
	t_fd_cache	*fd_cache;

	while (fd_cache_list)
	{
		fd_cache = (t_fd_cache *)fd_cache_list->content;
		if (fd_cache->fd == fd)
			return (fd_cache);
		fd_cache_list = fd_cache_list->next;
	}
	return (NULL);
}

int					get_next_line(const int fd, char **line)
{
	static t_list	*fd_cache_list = NULL;
	t_list			*fd_cache_link;
	t_fd_cache		*fd_cache;
	int				return_code;

	if (!(fd_cache = get_fd_cache(fd, fd_cache_list)))
	{
		if (!(fd_cache = (t_fd_cache *)ft_memalloc(sizeof(t_fd_cache))) ||
			!(fd_cache_link = ft_lstnew_empty()))
			return (-1);
		fd_cache->fd = fd;
		fd_cache->cache = NULL;
		fd_cache_link->content = fd_cache;
		fd_cache_link->content_size = sizeof(fd_cache);
		ft_lstadd(&fd_cache_list, fd_cache_link);
	}
	if (!(return_code = get_next_line_recursive(fd_cache, line)))
		fd_cache_list = del_fd_cache(fd, fd_cache_list);
	return (return_code);
}
