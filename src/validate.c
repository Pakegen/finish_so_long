/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qacjl <qacjl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 12:07:11 by qacjl             #+#    #+#             */
/*   Updated: 2024/12/13 15:46:32 by qacjl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"so_long.h"

int	validate_filename(const char *filename)
{
	const char	*extension;

	size_t	len;
	size_t	ext_len;

	extension = ".ber";
	len = ft_strlen(filename);
	ext_len = ft_strlen(extension);
	if (len < ext_len || ft_strcmp(filename + len - ext_len, extension) != 0)
		return (0);
	return (1);
}

void	count_elements(t_map *map, int *player, int *exit, int *collectable)
{
	int	y;
	int	x;

	y = 0;
	while (y < map->height)
	{
		x = 0;
		while (x < map->width)
		{
			if (map->data[y][x] == 'P')
				(*player)++;
			else if (map->data[y][x] == 'E')
				(*exit)++;
			else if (map->data[y][x] == 'C')
				(*collectable)++;
			else if (map->data[y][x] != '1' && map->data[y][x] != '0')
			{
				write(2, "INVALID CHARACTER IN MAP\n", 26);
				return;
			}
			x++;
		}
		y++;
	}
}

int	validate_element(t_map *map)
{

	int	player;
	int	exit;
	int	collectable;

	player = 0;
	exit = 0;
	collectable = 0;
	count_elements(map, &player, &exit, &collectable);
	if (player != 1)
		return (write(2, "INVALID NUMBER PLAYER\n", 23), 0);
	if (exit == 0)
		return (write(2, "NO EXIT\n", 9), 0);
	if (collectable == 0)
		return (write(2, "NO COLLECTABLE\n", 16), 0);
	return (1);
}

int	validate_map(t_map *map)
{
	if (!validate_borders(map))
		return (0);
	if (!validate_element(map))
		return (0);
	return (1);
}
