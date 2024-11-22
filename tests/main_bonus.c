#include "../get_next_line_bonus.h"
#include <fcntl.h>
#include <stdio.h>

int main(int ac, char **av)
{
	int     fd;
	char    *line;

	if (ac < 2)
	{
		printf("Usage: %s file1 [file2 ...]\n", av[0]);
		return (1);
	}

	for (int i = 1; i < ac; i++)
	{
		fd = open(av[i], O_RDONLY);
		if (fd < 0)
		{
			printf("Error opening file: %s\n", av[i]);
			continue;
		}

		printf("Reading file: %s\n", av[i]);
		while ((line = get_next_line(fd)) != NULL)
		{
			printf("%s", line);
			free(line);
		}
		close(fd);
		printf("\nFinished reading: %s\n", av[i]);
	}

	return (0);
}
