#ifndef __LEM_IN__
# define __LEM_IN__

# include	"libft/libft.h"
# include	<stdio.h>
# include	<stdlib.h>
# include	<stdbool.h>

typedef struct ant_path_s{
	t_list		*queue;
	int 		size;
	struct ant_path_s	*parent_path;
	struct ant_path_s	*next;
} t_ant_path;

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
	int			total_ants;	
	int			start_id;
	int			end_id;
	t_room		*rooms;
	t_ant_path	*ant_path;
} t_data;

// utils.c
void	print_room(t_data *data);
int		read_stdin(t_list **stdin_content);

// structs_utils.c
int		room_addnew(t_room **rooms, int id, char *name, int x, int y);
int		room_addlink(t_room *rooms, int room_id, int link_id);
t_room	*room_getby_id(t_room *rooms, int id);
t_room	*room_getby_name(t_room *rooms, char *name);
void	free_rooms(t_room *rooms);
void    init_data(t_data *data);

// parsing.c
int		parsing(t_list *stdin_content, t_data *data);

//bfs.c
t_ant_path    *path_addnew(t_ant_path *queue);
void    print_ant_path(t_ant_path *queue);
int     bfs(t_data *data);


#endif
