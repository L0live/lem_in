#ifndef __LEM_IN__
# define __LEM_IN__

# include	"libft/libft.h"
# include	<stdio.h>
# include	<stdlib.h>
# include	<stdbool.h>



typedef struct ant_queue_s{
	//liste qui va contenir l'ensemble de sommet parcourus qu'on va set au fur et a mesure de l'algorithme de parcours
	t_list		*queue;
	int 		move_count;

	struct ant_queue_s	*next;
} t_ant_queue;

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
	t_ant_queue	*ant_queue;
	char		**matrix;
} t_data;

// structs_utils.c
int		room_addnew(t_room **rooms, int id, char *name, int x, int y);
int		room_addlink(t_room *rooms, int room_id, int link_id);
t_room	*room_getby_id(t_room *rooms, int id);
t_room	*room_getby_name(t_room *rooms, char *name);
void	free_rooms(t_room *rooms);

// parsing.c
int		parsing(t_list *stdin_content, t_data *data);


#endif
