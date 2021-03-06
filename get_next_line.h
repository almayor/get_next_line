/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: unite <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/04 03:20:09 by unite             #+#    #+#             */
/*   Updated: 2020/07/16 03:35:23 by unite            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H

# define GET_NEXT_LINE_H

/*
** Controls the largest file description, as well the overall number of file
** descriptors, that can be simultaneously processed.
*/
# define MAX_FD		512

/*
** @brief Controls the size of the read buffer.
** @details Increasing it can lead to a speed-up due to a reduced number of
** system calls, but will increase the function memory footprint.
*/
# define BUFF_SIZE	32

int		get_next_line(const int fd, char **line);

#endif
