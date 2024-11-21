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

static char	*get_line(char *line)
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
	{
		free(line);
		return (NULL);
	}
	ft_memmove(new_line, line, line_len);
	new_line[line_len] = '\0';
	free(line);
	return (new_line);
}

static char	*update_backup(char *line)
{
	int		i;
	int		len;
	char	*new_backup;

	new_backup = NULL;
	i = 0;
	while (line[i] && line[i] != '\n')
		i++;
	if (line[i] == '\n')
		i++;
	len = ft_strlen(line) - i;
	if (len > 0)
	{
		new_backup = ft_strdup(&line[i]);
		if (!new_backup)
			return (NULL);
	}
	return (new_backup);
}

char	*get_next_line(int fd)
{
	char		*line;
	char		*buffer;
	static char	*backup;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);

	line = NULL;
	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	if (backup)
	{
		line = ft_strdup(backup);
		free(backup);
		backup = NULL;
		if (!line)
		{
			free(buffer);
			return (NULL);
		}
	}
	line = read_until_nl(fd, line, buffer);
	if (!line || line[0] == '\0')
	{
		free(line);
		return (NULL);
	}
	if (ft_strchr(line, '\n'))
	{
		backup = update_backup(line);
		line = get_line(line);
		if (!line || !backup)
		{
			free(backup);
			backup = NULL;
			// if (line)
			// 	return (line);
			free(line);
			return (NULL);
		}
	}
	return (line);
}
