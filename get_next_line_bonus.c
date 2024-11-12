/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naharumi <naharumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 19:11:01 by naharumi          #+#    #+#             */
/*   Updated: 2024/11/12 19:29:54 by naharumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static t_read *find_or_add_node(t_read **head, int fd)
{
	t_read	*node;
	t_read	*new_node;

	node = *head;
	while (node)
	{
		if (node->fd == fd)
			return (node);
		node = node->next;
	}
	new_node = malloc(sizeof(t_read));
	if (!new_node)
		return (NULL);
	new_node->fd = fd;
	new_node->content = NULL;
	new_node->next = *head;
	*head = new_node;
	return (new_node);
}

static void	remove_node(t_read **head, int fd)
{
	t_read	*temp;
	t_read	*prev;

	temp = *head;
	prev = NULL;
	while (temp && temp->fd != fd)
	{
		prev = temp;
		temp = temp->next;
	}
	if (temp)
	{
		if (prev)
			prev->next = temp->next;
		else
			*head = temp->next;
		free(temp->content);
		free(temp);
	}
}

static char	*read_until_nl(int fd, char **backup)
{
	char	*line;
	char	*buffer;
	int		bytes_read;

	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	if (*backup)
	{
		line = ft_strdup(*backup);
		free(*backup);
		*backup = NULL;
	}
	bytes_read = 1;
	while (bytes_read > 0 && !ft_strchr(line, '\n'))
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		buffer[bytes_read] = '\0';
		line = ft_strjoin(line, buffer);
	}
	free(buffer);
	return (line);
}

static char	*update_backup(char **line, char **backup)
{
	char *newline_pos;

	newline_pos = ft_strchr(*line, '\n');
	if (newline_pos)
	{
		*newline_pos = '\0';
		*backup = ft_strdup(newline_pos + 1);
	}
	else
	{
		free(*backup);
		*backup = NULL;
	}
	return (*line);
}

char	*get_next_line(int fd)
{
	static t_read	*backup;
	t_read			*node;
	char			*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	line = NULL;	
	node = find_or_add_node(&backup, fd);
	line = read_until_nl(fd, &node->content);
	line = update_backup(&line, &node->content);
	if (line && ft_strchr(line, '\n'))
	{
		free(node->content);
		node->content = ft_strdup(ft_strchr(line, '\n') + 1);	
	}
	else
	{
		free(line);
		remove_node(&backup, fd);
		return (NULL);
	}
	return (line);
}
