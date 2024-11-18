/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naharumi <naharumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 20:08:44 by naharumi          #+#    #+#             */
/*   Updated: 2024/11/14 18:59:27 by naharumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

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
			break;
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
	new_line = (char *)malloc(sizeof(char) * (line_len + 1));
	if (!new_line)
	{
		free(*backup);
		*backup = NULL;
		return (NULL);
	}
	ft_memmove(new_line, line, line_len);
	new_line[line_len] = '\0';
	if (*backup)
		free(*backup);
	*backup = ft_strdup(&line[line_len]);
	free(line);
	return (new_line);
}

char	*get_next_line(int fd)
{
	char		*line;
	char		*buffer;
	static char	*backup;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	line = NULL;
	if (backup)
		line = ft_strdup(backup);
	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	line = read_until_nl(fd, line, buffer);
	if (!line || line[0] == '\0')
	{
		free(line);
		if (backup)
		{
			free(backup);
			backup = NULL;
		}
		return (NULL);
	}
	line = update_line_backup(line, &backup);
	if (!line || line[0] == '\0')
	{
		free(line);
		return (NULL);
	}
	return (line);
}
