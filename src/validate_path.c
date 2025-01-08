/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: quenalla <quenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 00:54:25 by qacjl             #+#    #+#             */
/*   Updated: 2025/01/08 09:57:09 by quenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static void	free_map_copy(char **map_copy, int height)
{
	int	y;

	y = 0;
	while (y < height)
	{
		free(map_copy[y]);
		y++;
	}
	free(map_copy);
}

static char	**copy_map(t_map *map)
{
	char	**map_copy;
	int		y;

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

static int	count_collectables(t_map *map, char **map_copy)
{
	int	collectables;
	int	y;
	int	x;

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

static void	explore_path(char **map, int y, int x, int *info)
{
	if (y < 0 || x < 0 || !map[y]
		|| x >= (int)strlen(map[y]) || map[y][x] == '1' || map[y][x] == 'V')
		return ;
	if (map[y][x] == 'C')
		info[0}--;
	if (map[y][x] == 'E')
		info[1] = 1;
	map[y][x] = 'V';
	mark_visited(map, y + 1, x, collectables, exit_found);
	mark_visited(map, y - 1, x, collectables, exit_found);
	mark_visited(map, y, x + 1, collectables, exit_found);
	mark_visited(map, y, x - 1, collectables, exit_found);
}

int	validate_paths(t_map *map, int player_x, int player_y)
{
	char	**map_copy;
	int	info[2]; //info[0] c'est collectable et info[1] exit_found

	map_copy = copy_map(map);
	if (!map_copy)
		return (write(2, "MEMORY ALLOCATION FAILED\n", 26), 0);
	info[0] = count_collectables(map_copy, map->height);
	info[1] = 0;
	explore_path(map_copy, player_y, player_x, info);
	free_map_copy(map_copy, map->height);
	if (info[0] > 0 || info[1] == 0)
		return (write(2, "INVALID PATHS\n", 15), 0);
	return (1);
}
