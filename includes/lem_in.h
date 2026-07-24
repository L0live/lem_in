#ifndef __LEM_IN__
# define __LEM_IN__

# include	"libft/libft.h"
# include	<stdio.h>
# include	<stdlib.h>
# include	<stdbool.h>

# define START_NBR -1
# define END_NBR -2

typedef struct room_s{
	int				id;
	int				x, y;
	bool			is_full;
	int				*links;
	int				links_size;
	struct room_s	*next;
} t_room;

typedef struct data_s{
	int		total_ants;	
	t_room	*rooms;
	t_room	*start;
	t_room	*end;
	char	**matrix;
} t_data;

// structs_utils.c
void	room_addnew(t_room **rooms, int nbr, int x, int y);
void	room_addlink(t_room *rooms, int room_nbr, int link_nbr);
t_room	*room_getby_nbr(t_room *rooms, int nbr);
void	free_rooms(t_room *rooms);

// parsing.c
int		parsing(t_list *stdin_content, t_data *data);

char	**create_matrix(t_room *rooms);
void	print_matrix(char **matrix);
void    free_matrix(char **matrix);

#endif
