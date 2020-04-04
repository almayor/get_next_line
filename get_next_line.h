/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: unite <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/07 14:55:59 by unite             #+#    #+#             */
/*   Updated: 2019/09/19 14:50:58 by unite            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include "libft.h"

int				get_next_line(const int fd, char **line);

# define BUFF_SIZE	32
# define EOL		'\n'

typedef struct
{
	int			fd;
	t_list		*cache;
}				t_fd_cache;

#endif
