#ifndef __LEM_IN__
# define __LEM_IN__

# include	"libft/libft.h"
# include	<stdio.h>
# include	<stdlib.h>
# include	<stdbool.h>


//dernier element de la queue etant l'id de la room sur laquelle on est
typedef struct path_s{
	t_list			*queue;
	int 			size;
	int 			ants;
	int				id_ants;
	struct path_s	*parent_path;
	struct path_s	*next;
} t_path;

typedef struct room_s{
	int				id;
	char			*name;
	int				x, y;
	int				visited;
	int				*links;
	int				links_size;
	struct room_s	*next;
} t_room;

typedef struct data_s{
	int			total_ants;	
	int			start_id;
	int			end_id;
	t_room		*rooms;
	t_path		*paths;

	//sauvegarde de tous les tpath allouées
	t_list  *all_paths; 

	t_list		*valid_paths;
} t_data;

//parsing.c
int		parsing(t_list *stdin_content, t_data *data);

//breadthfirst_search.c.c
t_path	*path_queue_take_first(t_path **queue);
void	path_queue_push_back(t_path **queue, t_path *new_path);
t_path	*path_new(t_data *data, t_room *room, t_path *parent);
int		bfs(t_data	*data);
void	free_one_path_content(void *content);

// parsing.c
int		parsing(t_list *stdin_content, t_data *data);

//path_utils.c
t_path	*path_build_final(t_path *end_path);
int 	valid_path_add(t_list **valid_paths, t_path *path);
int		path_already_in_valid_path(t_data *data, t_path *path);
void	reset_paths_visited(t_list *paths);



// structs_utils.c
int		room_addnew(t_room **rooms, int id, char *name, int x, int y);
int		room_addlink(t_room *rooms, int room_id, int link_id, int start_id);
t_room	*room_getby_id(t_room *rooms, int id);
t_room	*room_getby_name(t_room *rooms, char *name);
int		count_room(t_room *rooms);
void	free_rooms(t_room *rooms);
void    init_data(t_data *data);

// utils.c
int		minimum_data(t_data *data);
void	print_room(t_data *data);
int		read_stdin(t_list **stdin_content);


//debug.c
void	print_onepath(void *valid);

//atns.c
int		ants_actions(t_data *data, t_path *path, int *id);
int		ants_actions_loop(t_data *data);
void	attribute_ants(t_data *data);
void	remove_start_from_paths(t_data *data);

//free.c
void	free_one_path_content(void *content);
void	free_data(t_data *data);

#endif
