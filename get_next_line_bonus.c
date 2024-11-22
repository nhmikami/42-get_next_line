/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naharumi <naharumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 19:11:01 by naharumi          #+#    #+#             */
/*   Updated: 2024/11/21 15:23:34 by naharumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static char	*free_and_return(char **line, char **backup)
{
	if (line && *line)
	{
		free(*line);
		*line = NULL;
	}
	if (backup && *backup)
	{
		free(*backup);
		*backup = NULL;
	}
	return (NULL);
}

static char	*read_until_nl(int fd, char *line, char *buffer)
{
	int		bytes_read;
	char	*tmp;

	bytes_read = 1;
	while (bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read < 0)
			return (free_and_return(&line, &buffer));
		else if (bytes_read == 0)
			break ;
		buffer[bytes_read] = '\0';
		tmp = ft_strjoin(line, buffer);
		if (!tmp)
			return (free_and_return(&line, &buffer));
		line = tmp;
		if (ft_strchr(line, '\n'))
			break ;
	}
	free(buffer);
	buffer = NULL;
	return (line);
}

static char	*update_line_backup(char *line, char **backup)
{
	char	*new_line;
	int		line_len;

	line_len = 0;
	while (line[line_len] && line[line_len] != '\n')
		line_len++;
	if (line[line_len] == '\n')
		line_len++;
	new_line = malloc(sizeof(char) * (line_len + 1));
	if (!new_line)
		return (free_and_return(&line, backup));
	ft_memmove(new_line, line, line_len);
	new_line[line_len] = '\0';
	*backup = ft_strdup(&line[line_len]);
	if (!*backup)
	{
		free(new_line);
		return (free_and_return(&line, backup));
	}
	free(line);
	return (new_line);
}

char	*get_next_line(int fd)
{
	char		*line;
	char		*buffer;
	static char	*backup[MAX_FD];

	if (fd < 0 || BUFFER_SIZE <= 0 || fd >= MAX_FD)
		return (NULL);
	line = NULL;
	if (backup[fd])
	{
		line = ft_strdup(backup[fd]);
		free(backup[fd]);
		backup[fd] = NULL;
	}
	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (free_and_return(&line, &backup[fd]));
	line = read_until_nl(fd, line, buffer);
	if (!line || line[0] == '\0')
		return (free_and_return(&line, &backup[fd]));
	line = update_line_backup(line, &backup[fd]);
	if (!line || line[0] == '\0')
		return (free_and_return(&line, &backup[fd]));
	return (line);
}
