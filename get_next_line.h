/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naharumi <naharumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 20:09:18 by naharumi          #+#    #+#             */
/*   Updated: 2024/11/12 19:31:56 by naharumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

char	*get_next_line(int fd);
char	*ft_strchr(const char *s, int c);
char	*ft_strdup(const char *s);
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strlen(const char *s);

#endif