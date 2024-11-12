/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naharumi <naharumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 20:08:44 by naharumi          #+#    #+#             */
/*   Updated: 2024/11/12 19:39:28 by naharumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*read_until_nl(int fd, char *buffer, char *line, char *backup)
{
	int	bytes_read;

	// Se houver conteúdo residual no backup, começa a linha com ele
	if (backup)
		line = ft_strdup(backup);
	bytes_read = 1;
	// Lê o arquivo até encontrar '\n' ou até o fim
	while (bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		// Caso a leitura não retorne bytes válidos (fim do arquivo ou erro)
		if (bytes_read <= 0)
		{
			// Se há uma linha já lida, retorna ela
			if (line)
				return (line);
			return (NULL);
		}
		// Garante que o buffer termine com '\0' para garantir uma string válida
		buffer[bytes_read] = '\0';
		// Junta a linha atual com os novos dados do buffer
		line = ft_strjoin(line, buffer);
		// Se encontrar o '\n', quebra o loop
		if (ft_strchr(line, '\n'))
			break ;
	}
	return (line);
}

// Função que retorna a linha antes do '\n' (inclusive)
static char	*get_line(char *str)
{
	char	*line;
	int		line_len;

	// Calcula o comprimento da linha até o primeiro '\n'
	line_len = 0;
	while (str[line_len] && str[line_len] != '\n')
		line_len++;
	if (str[line_len] == '\n')  // Conta também o '\n' se existir
		line_len++;
	// Aloca a memória para a linha
	line = malloc((line_len + 1) * sizeof(char));
	if (line)
	{
		// Copia a linha para a memória alocada
		ft_memmove(line, str, line_len);
		line[line_len] = '\0';  // Garante que a string termina com '\0'
	}
	// Libera o espaço usado pela string original
	free(str);
	return (line);
}

// Função que salva o restante da linha após o '\n'
static char	*save_remainder(char *buffer)
{
	int		i;
	int		len;
	char	*remainder;

	i = 0;
	len = 0;
	remainder = NULL;
	// Encontra o índice do '\n'
	while (buffer[i] && buffer[i] != '\n')
		i++;
	// Se houver um '\n', avança para o próximo caractere
	if (buffer[i] == '\n')
		i++;
	// Calcula o tamanho do restante após o '\n'
	len = ft_strlen(buffer) - i;
	if (buffer[i] != '\0')  // Se houver algum conteúdo restante
	{
		// Aloca memória para o restante da string
		remainder = malloc((len + 1) * sizeof(char));
		if (remainder)
		{
			// Copia o restante da string
			ft_memmove(remainder, &buffer[i], len);
			remainder[len] = '\0';  // Garante que a string termina com '\0'
		}
	}
	return (remainder);
}

// Função principal que retorna a próxima linha de um arquivo
char	*get_next_line(int fd)
{
	char		*buffer;
	char		*line;
	static char	*backup;  // Armazena o conteúdo restante entre chamadas

	// Verifica se a entrada é válida
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);

	line = NULL;
	// Aloca o buffer para leitura dos dados
	buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (buffer == NULL)
		return (NULL);

	// Lê até encontrar um '\n' ou até o fim do arquivo
	line = read_until_nl(fd, buffer, line, backup);

	// Libera o backup se não for mais necessário
	if (backup)
	{
		free(backup);
		backup = NULL;
	}

	// Se encontrou um '\n', separa a linha e armazena o restante no backup
	if (line && ft_strchr(line, '\n'))
	{
		backup = save_remainder(line);
		line = get_line(line);  // Extrai a linha até o '\n'
	}

	// Libera o buffer que foi usado para ler os dados
	free(buffer);

	return (line);
}
