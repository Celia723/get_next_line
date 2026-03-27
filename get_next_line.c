/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ceboyero <ceboyero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 13:09:56 by ceboyero          #+#    #+#             */
/*   Updated: 2026/03/27 12:29:49 by ceboyero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"


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

char	*get_next_line(int fd)
{
	
	static char 	*line;
	char			*real_line;
	char			*n_position;
	char			*buffer;
	int				num_read;
	char			*aux;
	char		*tmp;
	

	//si no ha encon trado el salto de linea se vuelve a llamar y todo se guarda en line 
	while (!line || !ft_strchr(line, '\n'))
	{
		buffer = malloc (sizeof(char) * BUFFER_SIZE + 1);
		if (!buffer)
			return (0);
		num_read = read(fd, buffer, BUFFER_SIZE);
		if (num_read == -1)
		{
			
			free (buffer);
			return (NULL);
		}
		if (num_read == 0)
		{
			free(buffer);
			if (line)
			{
				tmp = line;
				line = NULL;
				return (tmp);
			}
			else if (!line)
				return(NULL);
			/* if (!tmp)
			{
				tmp = line;
				ft_putstr_fd(tmp, 1);
				return (tmp);
			}else
				return (NULL); */
		}	
		buffer[num_read] = '\0';
		aux = line; //vriable aux para liberar el buffer
 		line = ft_strjoin(line,  buffer);
		free(aux);
		aux = NULL;
		free(buffer);
		buffer = NULL;
	}//vemos la posicion de la n para extraer la linea real
	aux = line;
	n_position = ft_strchr(line, '\n');
	real_line = ft_substr(line, 0, (n_position - line) + 1);
	line = ft_substr (line, (n_position - line) + 1, ft_strlen(line) + 1);
	
	free (aux);
	aux = NULL;
	return (real_line);
	}





/* 
int main()
{

	int fd = open("text.txt", O_RDONLY);
	char *line ;

	while ( (line=get_next_line(fd)))
	{
		free (line);
		line = NULL;
		
		//deberia liberr linea aqui?
	}
	
	
	
	return 0;
}
 */