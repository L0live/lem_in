#ifndef 	__LEM_IN_BONUS__
# define	__LEM_IN_BONUS__

# include	"libft/libft.h"
# include	<stdio.h>
# include	<stdlib.h>
# include	<stdbool.h>

# define START_ID -1
# define END_ID -2

// For the bonus part, we will use OpenGL
#include	<GLFW/glfw3.h>
#include	<../glad/include/glad/glad.h>

typedef struct room_s{
	int				id;
	char			*name;
	int				x, y;
	bool			visited;
	int				*links;
	int				links_size;
	struct room_s	*next;
} t_room;

typedef struct data_s{
	int		total_ants;	
	t_room	*rooms;
	int		start;
	int		end;
	char	**matrix;
} t_data;

// structs_utils.c
void	room_addnew(t_room **rooms, int id, int x, int y);
void	room_addlink(t_room *rooms, int room_id, int link_id);
t_room	*room_getby_id(t_room *rooms, int id);
void	free_rooms(t_room *rooms);

// parsing.c
int		parsing(t_list *stdin_content, t_data *data);

// visu.c
void	normalize_rooms(t_room *rooms);
char	**create_matrix(t_room *rooms);
void	print_matrix(char **matrix);
void    free_matrix(char **matrix);


#endif
