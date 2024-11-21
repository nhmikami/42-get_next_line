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

static char	*cleanup_and_return(char **line, char **backup)
{
	if (*line)
		free(*line);
	if (*backup)
	{
		free(*backup);
		*backup = NULL;
	}
	return (NULL);
}

static char	*read_until_nl(int fd, char *line, char *buffer)
{
	int		bytes_read;

	bytes_read = 1;
	while (bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read < 0)
		{
			free(buffer);
			return (NULL);
		}
		buffer[bytes_read] = '\0';
		line = ft_strjoin(line, buffer);
		if (!line)
		{
			free(buffer);
			return (NULL);
		}
		if (ft_strchr(line, '\n'))
			break ;
	}
	free(buffer);
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
		return (cleanup_and_return(&line, backup));
	ft_memmove(new_line, line, line_len);
	new_line[line_len] = '\0';
	if (*backup)
		free(*backup);
	*backup = ft_strdup(&line[line_len]);
	if (!*backup)
	{
		free(new_line);
		return (cleanup_and_return(&line, backup));
	}
	free(line);
	return (new_line);
}

char	*get_next_line(int fd)
{
	char		*line;
	char		*buffer;
	static char	*backup;

	if (fd < 0 || BUFFER_SIZE <= 0 || fd > MAX_FD)
		return (NULL);
	line = NULL;
	if (backup[fd])
		line = ft_strdup(backup[fd]);
	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (cleanup_and_return(&line, &backup[fd]));
	line = read_until_nl(fd, line, buffer);
	if (!line || line[0] == '\0')
		return (cleanup_and_return(&line, &backup[fd]));
	line = update_line_backup(line, &backup[fd]);
	if (!line || line[0] == '\0')
		return (cleanup_and_return(&line, &backup[fd]));
	return (line);
}
