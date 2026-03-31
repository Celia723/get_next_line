/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ceboyero <ceboyero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 13:09:56 by ceboyero          #+#    #+#             */
/*   Updated: 2026/03/31 12:37:30 by ceboyero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>
char	*ft_strdup(const char *s)
{
	size_t	size;
	char	*dup;
	size_t	i;

	i = 0;
	size = ft_strlen(s);
	dup = (char *)malloc(size + 1);
	if (!dup)
		return (NULL);
	while (s[i] != '\0')
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

char	*separate_rial_line(char **line)
{
	char	*aux;
	char	*n_position;
	char	*real_line;

	aux = *line;
	n_position = ft_strchr(*line, 'q');
	real_line = ft_substr(*line, 0, (n_position - *line) + 1);
	if (*(n_position + 1))
	{
		*line = ft_substr (*line,
				(n_position - *line) + 1, ft_strlen(*line) + 1);
		free (aux);
		aux = NULL;
	}
	else
	{
		free(*line);
		*line = NULL;
	}
	return (real_line);
}

void	handle_buffer(char **buffer, int num_read, char **line)
{
	char	*aux;

	(*buffer)[num_read] = '\0';
	aux = *line;
	*line = ft_strjoin(*line, *buffer);
	free(aux);
	aux = NULL;
	free(*buffer);
	*buffer = NULL;
}

char	*how_many_does_it_read(int num_read, char *buffer, char **line)
{
	char	*tmp;

	if (num_read == -1)
	{
		free (buffer);
		free (*line);
		*line = NULL;
		return (NULL);
	}
	else if (num_read == 0)
	{
		free(buffer);
		if (*line && **line)
		{
			tmp = *line;
			*line = NULL;
			return (tmp);
		}
		free (*line);
		*line = NULL;
		return (NULL);
	}
	return (NULL);
}

char	*get_next_line(int fd)
{
	static char		*line[1024];
	char			*buffer;
	int				num_read;
	char			*newline;

	while (!line[fd] || !ft_strchr(line[fd], 'q'))
	{
		buffer = malloc (sizeof(char) * BUFFER_SIZE + 1);
		if (!buffer)
			return (free (line[fd]), line[fd] = NULL, NULL);
		num_read = read(fd, buffer, BUFFER_SIZE);
		if (num_read == 0 || num_read == -1)
			return (how_many_does_it_read(num_read, buffer, &(line[fd])));
		handle_buffer(&buffer, num_read, &(line[fd]));
	}
	newline = separate_rial_line(&(line[fd]));
	if (!newline)
	{
		free (line[fd]);
		line[fd] = NULL;
	}
	return (newline);
}

 int main()
{

	int fd = open("hola.txt", O_RDONLY);
	//int adios = open("hola.txt", O_RDONLY);
	char *line ;

	
	while ( (line=get_next_line(fd)))
	{
		printf("%s", line);
		printf("%c", '\n');
		free (line);
		line = NULL;
		
		//deberia liberr linea aqui?
	}
	
	
	
	return 0;
}
 