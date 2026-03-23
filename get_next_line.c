/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ceboyero <ceboyero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 13:09:56 by ceboyero          #+#    #+#             */
/*   Updated: 2026/03/23 19:47:35 by ceboyero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	char			*buffer;
	int				num_read;
	static char 	*line;
	char			*real_line;
	char			*n_position;
	char			*aux;

	
	if (!buffer)
		return (NULL);
	
	//si no ha encon trado el salto de linea se vuelve a llamar y todo se guarda en line 
	while (!line || !ft_strchr(line, '\n'))
	{
		buffer = malloc (sizeof(char) * BUFFER_SIZE + 1);
		num_read = read(fd, buffer, BUFFER_SIZE);
		buffer[num_read] = '\0';
		if (line)
			aux = line; //vriable aux para liberar el buffer
 		line = ft_strjoin(line,  buffer);
		free(buffer);
		buffer = NULL;
	}//vemos la posicion de la n para extraer la linea real

	n_position = ft_strchr(line, '\n');
	real_line = ft_substr(line, 0, n_position + 1);
	line = ft_substr (line, n_position + 1, ft_strlen(line) + 1);
	//escribimos la linea en la consola
	ft_putstr_fd (real_line, 1); //BORRAR
	return (real_line);
}

char	*read_line()
{
	
}


void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	if (!s)
		return ;
	i = 0;
	while (s[i] != '\0')
	{
		write(fd, &s[i], 1);
		i++;
	}
}

