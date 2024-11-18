#include "get_next_line.h"

char	*ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if (*s == (char)c)
		return ((char *)s);
	return (NULL);
}

char	*ft_strdup(const char *s)
{
	size_t	len;
	char	*dest;

    if (!s)
        return (NULL);
	len = ft_strlen(s);
	dest = (char *)malloc(sizeof(char) * (len + 1));
	if (!dest)
		return (NULL);
    ft_memmove(dest, s, len);
	dest[len] = '\0';
	return (dest);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*str;
	size_t	len1;
    size_t	len2;

    if (!s1)
        return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	len1 = ft_strlen(s1);
    len2 = ft_strlen(s2);
	str = (char *)malloc(sizeof(char) * (len1 + len2 + 1));
	if (!str)
		return (NULL);
    ft_memmove(str, s1, len1);
    ft_memmove(&str[len1], s2, len2);
    str[len1 + len2] = '\0';
	free(s1);
	return (str);
}

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char   	*d;
	const unsigned char	*s;

	d = (unsigned char *)dest;
	s = (const unsigned char *)src;
	if (!dest && !src)
		return (NULL);
	if (s < d)
	{
		while (n--)
			d[n] = s[n];
		return (dest);
	}
	else
		while (n--)
			*(d++) = *(s++);
	return (dest);
}

size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len])
		len++;
	return (len);
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
