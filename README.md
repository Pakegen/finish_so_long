					PROGRAM SO_LONG	
//////////////////////////////////////////////////////////////////////////////////////////////////////
					map.ber
					
					111111111
					1000001C1
					100000101
					100000101
					1000P0101
					100000101
					1C0000101
					1000001E1
					111111111

///////////////////////////////////////////////////////////////////////////////////////////////////////		
					FICHIER so_long.h :

# ifndef SO_LONG_H
# define SO_LONG_H

#include "../minilibx-linux/mlx.h"
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include "../utils/libft/libft.h"
# define TILE_SIZE 64
# define WINDOW_TITLE "so_long"

typedef enum e_tiletype
{
	EMPTY = '0',
	WALL = '1',
	COLLECTABLE = 'C',
	PLAYER = 'P',
	EXIT = 'E'
}t_tiletype;

typedef struct s_map
{
	char	**data;
	int		width;
	int		height;
}t_map;

typedef struct s_game
{
	void	*mlx;
	void	*window;
	t_map	map;
	void	*player_up;
	void	*player_down;
	void	*player_left;
	void	*player_right;
	void	*current_player_img;
	void	*wall_img;
	void	*floor_img;
	void	*collect_img;
	void	*exit_img;
	int		player_x;
	int		player_y;
	int		collectable;
	int		steps;
}t_game;

int validate_filename(const char *filename);
char **read_map(const char *file, t_map *map);
int validate_map(t_map *map);
void count_elements(t_map *map, int *player, int *exit, int *collectables);
int validate_element(t_map *map);
void load_sprites(t_game *game);
void render_map(t_game *game);
int handle_input(int keycode, t_game *game);
int close_game(t_game *game);
int validate_borders(t_map *map);
void find_player_position(t_game *game);
void	initialize_map(t_map *map);

# endif


///////////////////////////////////////////////////////////////////////////////////////////////////////
					FICHIER main.c ;

#include "so_long.h"

void init_collectables(t_game *game)
{
	int y;
	int x;

	y = 0;
	game->collectable = 0;
	while (y < game->map.height)
	{
		x = 0;
		while (x < game->map.width)
		{
			if (game->map.data[y][x] == 'C')
				game->collectable++;
			x++;
		}
		y++;
	}
	ft_printf("Total collectables: %d\n", game->collectable);
}



void	load_sprites(t_game *game)
{
	int	sprite_width;
	int	sprite_height;

	sprite_width = 0;
	sprite_height = 0;
	game->floor_img = mlx_xpm_file_to_image(game->mlx, "texture/BACKGROUND.xpm", &sprite_width, &sprite_height);
	game->wall_img = mlx_xpm_file_to_image(game->mlx, "texture/WALL.xpm", &sprite_width, &sprite_height);
	game->player_down = mlx_xpm_file_to_image(game->mlx, "texture/slime_down.xpm", &sprite_width, &sprite_height);
	game->player_left = mlx_xpm_file_to_image(game->mlx, "texture/slime_left.xpm", &sprite_width, &sprite_height);
	game->player_up = mlx_xpm_file_to_image(game->mlx, "texture/slime_up.xpm", &sprite_width, &sprite_height);
	game->player_right = mlx_xpm_file_to_image(game->mlx, "texture/slime_right.xpm", &sprite_width, &sprite_height);
	game->current_player_img = mlx_xpm_file_to_image(game->mlx, "texture/slime_right.xpm", &sprite_width, &sprite_height);
	game->collect_img = mlx_xpm_file_to_image(game->mlx, "texture/piece.xpm", &sprite_width, &sprite_height);
	game->exit_img = mlx_xpm_file_to_image(game->mlx, "texture/exit.xpm", &sprite_height, &sprite_height);
}

void	initialize_map(t_map *map)
{
	map->data = NULL;
	map->width = 0;
	map->height = 0;
}

int	close_game(t_game *game)
{
	mlx_destroy_window(game->mlx, game->window);
	exit (0);
	return (0);
}

int	main (int ac, char **av)
{
	t_game game;
	if (ac != 2)
		return (write(2, "ARGUMENT INCORRECT\n", 20), 1);
	initialize_map(&game.map);
	if (!validate_filename(av[1]))
		return (write(2, "INCORRECT FILE\n", 16), 1);
	if (!read_map(av[1], &game.map))
		return (write(2, "CANNOT READ MAP\n", 17), 1);
	if (!validate_map(&game.map))
		return (write(2, "INVALID MAP\n", 13), 1);//ici
	find_player_position(&game.map, &game.player_x, &game.player_y);
	init_collectables(&game);
	game.mlx = mlx_init();
	game.window =  mlx_new_window(game.mlx, game.map.width * TILE_SIZE, game.map.height * TILE_SIZE, WINDOW_TITLE);
	game.steps = 0;
	load_sprites(&game);
	render_map(&game);

	mlx_key_hook(game.window, handle_input, &game);
	mlx_hook(game.window, 17, 0, close_game, &game);
	mlx_loop(game.mlx);

	return (0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
					FICHIER validate.c :
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

int validate_map(t_map *map)
{
	int player_x;
	int player_y;

	player_x = 0;
	player_y = 0;
	if (!validate_borders(map))
		return (0);
	if (!validate_element(map))
		return (0);
	find_player_position(map, &player_x, &player_y);
	if (!validate_paths(map, player_x, player_y))
		return (0);
	return (1);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
					FICHIER validate_2.c :
#include "so_long.h"

static int	validate_horizontal_borders(t_map *map)
{
	int	x;

	x = 0;
	if (!map->data[0] || !map->data[map->height - 1])//ici
		return (write(2, "INVALID HORIZONTAL BORDER\n", 27), 0);
	while (x < map->width)
	{
		if (map->data[0][x] != '1' || map->data[map->height - 1][x] != '1')
			return (write(2, "HORIZONTAL BORDERS NOT CLOSED\n", 31), 0);
		x++;
	}
	return (1);
}

static int	validate_vertical_borders(t_map *map)
{
	int	y;

	y = 0;
	while (y < map->height)
	{
		if (!map->data[y])
			return (write(2, "INVALID VERTICAL BORDERS\n", 26), 0);
		if (map->data[y][0] != '1' || map->data[y][map->width - 1] != '1')
			return (write(2, "VERTICAL BORDERS ARE NOT CLOSED\n", 33), 0);
		y++;
	}
	return (1);
}

int validate_borders(t_map *map)//ici
{
	if (!map || !map->data || map->width <= 0 || map->height <= 0)
		return (write(2, "INVALID MAP DIMENSION\n", 23), 0);
	if (!validate_horizontal_borders(map))
		return (0);
	if (!validate_vertical_borders(map))
		return (0);
	return (1);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
					FICHIER map.c :

#include"so_long.h"

static int	count_lines(const char	*file)
{
	int		fd;
	int		lines;
	char	buffer;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (0);
	lines = 0;
	while (read(fd, &buffer, 1) > 0)
	{
		if (buffer == '\n')
			lines++;
	}
	close(fd);
	if (lines <= 0)
		return (0);
	return (lines);
}

static void	free_map_on_error(char **map_data, int lines)
{
	int	i;

	i = 0;
	while (i < lines)
	{
		free(map_data[i]);
		i++;
	}
	free(map_data);
}

static int allocate_map(t_map *map, int line_count, int fd)
{
	int i = 0;

	map->data = malloc(sizeof(char *) * (line_count + 1));
	if (!map->data)
		return (write(2, "MEMORY ALLOCATION FAILED\n", 26), close(fd), 0);

	while (i < line_count)
	{
		map->data[i] = get_next_line(fd);
		if (!map->data[i])
			return (write(2, "FAILED TO READ A LINE\n", 23), free_map_on_error(map->data, i), close(fd), 0);
		i++;
	}
	map->data[i] = NULL;
	close(fd);
	return (1);
}

char **read_map(const char *file, t_map *map)
{
	int	fd;
	int	line_count;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (write(2, "CANNOT OPEN FILE\n", 18), NULL);

	line_count = count_lines(file);
	if (line_count <= 0)
		return (write(2, "NO LINE IN MAP\n", 16), close(fd), NULL);

	if (!allocate_map(map, line_count, fd))
		return (NULL);
	map->height = line_count;
	map->width = map->data[0] ? ft_strlen(map->data[0]) - 1 : 0;
	return (map->data);
}

void find_player_position(t_game *game)
{
	int y = 0;
	int x;

	while (y < game->map.height)
	{
		x = 0;
		while (x < game->map.width)
		{
			if (game->map.data[y][x] == 'P')
			{
				game->player_x = x;
				game->player_y = y;
                ft_printf("Player found at (%d, %d)\n", game->player_x, game->player_y);
				return;
			}
			x++;
		}
		y++;
	}
	write(2, "NO player\n", 11);
	close_game(game);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
					FICHIER input.c :

#include "so_long.h"

static int	is_move_valid(t_game *game, int new_x, int new_y)
{
	if (new_x < 0 || new_y < 0 || new_x >= game->map.width || new_y >= game->map.height)
		return(write(2, "OUT OF BAND", 12), 0);
	if (game->map.data[new_y][new_x] == '1')
		return (0);
	return (1);
}

static void	update_player_image(t_game *game, int new_x, int new_y)
{
	if (new_y < game->player_y)
		game->current_player_img = game->player_up;
	else if (new_y > game->player_y)
		game->current_player_img = game->player_down;
	else if (new_x < game->player_x)
		game->current_player_img = game->player_left;
	else if (new_x > game->player_x)
		game->current_player_img = game->player_right;
}

static void	process_move(t_game *game, int new_x, int new_y)
{
	if (game->map.data[new_y][new_x] == 'C')
		game->collectable--;
	if (game->map.data[new_y][new_x] == 'E')
	{
		if (game->collectable > 0)
		{
			write(2, "CANNOT EXIT, COLLECTABLE REMAINING\n", 36);
			return;
		}
		close_game(game);
	}
	game->map.data[game->player_y][game->player_x] = '0';
	game->map.data[new_y][new_x] = 'P';
	game->player_y = new_y;
	game->player_x = new_x;
	game->steps++;
	ft_printf("Steps: %d\n", game->steps);
}

void	move_player(t_game *game, int new_y, int new_x)
{
	if (!is_move_valid(game, new_x, new_y))
		return;
	update_player_image(game, new_x, new_y);
	process_move(game, new_x, new_y);
	render_map(game);
}

int	handle_input(int keycode, t_game *game)
{
	int	new_y;
	int	new_x;

	new_y = game->player_y;
	new_x = game->player_x;
	ft_printf("Key pressed: %d\n", keycode);
	if (keycode == 65307)//53
		close_game(game);
	if (keycode == 119 || keycode == 65362) //13 ,126
		new_y--;
	else if (keycode == 115 || keycode == 65364)
		new_y++;
	else if (keycode == 97 || keycode == 65361)
		new_x--;
	else if (keycode == 100 || keycode == 65363)
		new_x++;
	ft_printf("Current position: (%d, %d)\n", game->player_x, game->player_y);
	ft_printf("Move attempt to: (%d, %d)\n", new_x, new_y);
	move_player(game, new_y, new_x);
	return (0);
}


///////////////////////////////////////////////////////////////////////////////////////////////////

					FICHIER render.c
					
#include"so_long.h"

void	render_tile(t_game *game, int y, int x)
{
	void	*img;

	img = game->floor_img;
	if (game->map.data[y][x] == '1')
		img = game->wall_img;
	else if (game->map.data[y][x] == 'C')
		img = game->collect_img;
	else if (game->map.data[y][x] == 'E')
		img = game->exit_img;
	mlx_put_image_to_window(game->mlx, game->window, img, x * TILE_SIZE, y * TILE_SIZE);
}

void	render_map(t_game *game)
{
	int	y;
	int	x;

	y = 0;
	while (y < game->map.height)
	{
		x = 0;
		while (x < game->map.width)
		{
			render_tile(game, y, x);
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(game->mlx, game->window, game->current_player_img, game->player_x * TILE_SIZE,
		 game->player_y * TILE_SIZE);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

					FICHIER validate_path.c

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


///////////////////////////////////////////////////////////////////////////////////////////////////////
					FICHIER MAKEFILE :

NAME = so_long

CC = cc

CFLAGS = -Wall -Werror -Wextra -g


SRCDIR = src

OBJDIR = obj

LIBFTDIR = utils/libft

MLXDIR = minilibx-linux

SRC = $(SRCDIR)/main.c \
      $(SRCDIR)/map.c \
      $(SRCDIR)/validate.c \
      $(SRCDIR)/validate_2.c \
      $(SRCDIR)/input.c \
      $(SRCDIR)/render.c

OBJ = $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)


LIBFT = $(LIBFTDIR)/libft.a

MLX = $(MLXDIR)/libmlx.a

LIBS = -L$(LIBFTDIR) -lft -L$(MLXDIR) -lmlx -lXext -lX11

MLXFLAGS = -I$(MLXDIR)

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT) $(MLX)
	$(CC) $(CFLAGS) $(OBJ) $(LIBS) -o $(NAME)


$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) $(MLXFLAGS) -c $< -o $@

$(LIBFT):
	@$(MAKE) -C $(LIBFTDIR)

$(MLX):
	@$(MAKE) -C $(MLXDIR)

clean:
	@rm -rf  $(OBJDIR)
	@$(MAKE) -C $(LIBFTDIR) clean
	@$(MAKE) -C $(MLXDIR) clean

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFTDIR) fclean

re: fclean all

.PHONY: all clean fclean re val
					
