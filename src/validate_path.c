/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qacjl <qacjl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 00:54:25 by qacjl             #+#    #+#             */
/*   Updated: 2025/01/08 01:30:57 by qacjl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static void free_map_copy(char **map_copy, int height)
{
	int y;

	y = 0;
	while (y < height)
	{
		free(map_copy[y]);
		y++;
	}
	free(map_copy);
}

static char **copy_map(t_map *map)
{
	char **map_copy;
	int y;

	y = 0;
	map_copy = malloc(sizeof(char *) * (map->height + 1));
	if (!map_copy)
		return (NULL);
	while (y < map->height)
	{
		map_copy[y] = strdup(map->data[y]);
		if (!map_copy[y])
		{
			free_map_copy(map_copy, y);
			return (NULL);
		}
		y++;
	}
	map_copy[y] = NULL;
	return (map_copy);
}

static int count_collectables(t_map *map, char **map_copy)
{
	int collectables;
	int y;
	int x;

	collectables = 0;
	y = 0;
	while (y < map->height)
	{
		x = 0;
		while (x < map->width)
		{
			if (map_copy[y][x] == 'C')
				collectables++;
			x++;
		}
		y++;
	}
	return (collectables);
}

void mark_visited(char **map, int y, int x, int *collectables, int *exit_found)
{
	if (y < 0 || x < 0 || !map[y] || x >= (int)strlen(map[y]) || map[y][x] == '1' || map[y][x] == 'V')
		return;

	if (map[y][x] == 'C')
		(*collectables)--;
	if (map[y][x] == 'E')
		*exit_found = 1;

	map[y][x] = 'V'; // Marque la case comme visitée
	mark_visited(map, y + 1, x, collectables, exit_found);
	mark_visited(map, y - 1, x, collectables, exit_found);
	mark_visited(map, y, x + 1, collectables, exit_found);
	mark_visited(map, y, x - 1, collectables, exit_found);
}

int validate_paths(t_map *map, int player_x, int player_y)
{
	char **map_copy;
	int collectables;
	int exit_found;

	map_copy = copy_map(map);
	exit_found = 0;
	if (!map_copy)
		return (write(2, "MEMORY ALLOCATION FAILED\n", 26), 0);
	collectables = count_collectables(map, map_copy);
	mark_visited(map_copy, player_y, player_x, &collectables, &exit_found);
	free_map_copy(map_copy, map->height);
	if (collectables > 0 || !exit_found)
		return (write(2, "INVALID PATHS\n", 15), 0);
	return (1);
}
