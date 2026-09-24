#include	"../includes/lem_in.h"

static int same_path(t_path *path1, t_path *path2){
	t_list *q1 = path1->queue;
	t_list *q2 = path2->queue;

	while (q1 && q2) {
		if (((t_room *)q1->content)->id != ((t_room *)q2->content)->id)
			return (0);
		q1 = q1->next;
		q2 = q2->next;
	}

	if (q1 == NULL && q2 == NULL)
		return (1);	
	return (0);
}

int path_already_in_valid_path(t_data *data, t_path *path){
	t_list *node;
	t_path *valid_path;

	if (!data || !path)
		return (0);

	node = data->valid_paths;
	while (node){
		valid_path = (t_path *)node->content;

		if (same_path(path, valid_path))
			return (1);

		node = node->next;
	}
	return (0);
}

void	reset_paths_visited(t_list *paths){
	t_path	*path;
	t_list	*queue;

	while (paths){
		path = (t_path *)paths->content;
		queue = path->queue;
		while (queue){
			((t_room *)queue->content)->visited = 0;
			queue = queue->next;
		}
		paths = paths->next;
	}
}

int valid_path_add(t_list **valid_paths, t_path *path){
	t_list *new_node;

	new_node = malloc(sizeof(t_list));
	if (!new_node)
		return (-1);

	new_node->content = path;
	new_node->next = NULL;

	ft_lstadd_back(valid_paths, new_node);
	return (0);
}

static int	parent_path_count(t_path *end_path){
	int		length = 0;
	t_path	*current_path = end_path;

	while (current_path){
		length++;
		current_path = current_path->parent_path;
	}
	
	return (length);
}

static t_room	**path_get_rooms(t_path *end_path, int size){
	t_room		**ordered_rooms;
	t_path		*current_path;

	ordered_rooms = malloc(sizeof(t_room *) * size);
	if (!ordered_rooms)
		return (NULL);

	current_path = end_path;
	
	for (int i = size - 1; current_path; i--){
		ordered_rooms[i] = current_path->queue->content;
		current_path = current_path->parent_path;
	}

	return (ordered_rooms);
}

// transforme chaine de parent_pah en un seul chemin - de START a END
t_path *path_build_final(t_path *end_path){

	int	final_len = parent_path_count(end_path);
	t_room **ordered_rooms = path_get_rooms(end_path, final_len);
	if (!ordered_rooms)
		return (NULL);
	
	t_path *final = malloc(sizeof(t_path));
	if (!final) {
		free(ordered_rooms);
		return (NULL);
	}

	final->queue = NULL;
	final->size = final_len;
	final->ants = 0;
	final->id_ants = -1;
	final->parent_path = NULL;
	final->next = NULL;


	t_list	*room_node;
	for (int i = 0; i < final_len; i++) {
		room_node = malloc(sizeof(t_list));
		if (!room_node) {
			ft_lstclear(&final->queue, NULL);
			free(final);
			free(ordered_rooms);
			return (NULL);
		}
		room_node->content = ordered_rooms[i];
		room_node->next = NULL;
		ft_lstadd_back(&final->queue, room_node);
	}

	free(ordered_rooms);
	return (final);
}

t_room	**queue_to_rooms(t_list *queue, int size){

	t_room **rooms;
	int	i = 0;

	rooms = malloc(sizeof(t_room*) * (size + 1));
	if (!rooms)
		return (NULL);

	for (i = 0; queue; i++){
		rooms[i] = queue->content;
		queue = queue->next;
	}
	rooms[i] = NULL;
	return (rooms);
};

void	reset_not_common_rooms(t_data *data, t_room **rooms, int size, int room_id){

	(void)data;

	int ids[size];
	int	y = 0;
	for (int i = size - 2;  i >= 1 && rooms[i]->id != room_id ; i--){
		ids[y] = rooms[i]->id;
		y++; 			
	}

	t_room *todelete = data->rooms;
	while (todelete){

		for (int i = 0; i < y; i++){
			if (ids[i] == todelete->id){
				todelete->visited = false;
				break;			
			}
		}
		todelete = todelete->next;
	}
}

void	delete_path(t_data *data, t_list *path){
	t_list	*current;
	t_list	*previous;

	current = data->valid_paths;
	previous = NULL;

	while (current){
		if (current == path){
			if (previous)
				previous->next = current->next;
			else
				data->valid_paths = current->next;
			free(current);
			return;
		}
		previous = current;
		current = current->next;
	}
}

static int	paths_share_room(t_room **rooms_a, int size_a, t_room **rooms_b, int size_b){

	for (int i = 1; i < size_a - 1; i++){
		for (int j = 1; j < size_b - 1; j++){
			if (rooms_a[i]->id == rooms_b[j]->id)
				return (1);
		}
	}
	return (0);
}

int	clean_samerooms_paths(t_data *data){
	t_list	*current_list;
	t_list	*next_list;
	t_path	*current_path;
	t_path	*next_path;
	t_room	**current_rooms;
	t_room	**next_rooms;

	current_list = data->valid_paths;
	while (current_list && current_list->next){
		next_list = current_list->next;
		current_path = current_list->content;
		next_path = next_list->content;

		current_rooms = queue_to_rooms(current_path->queue, current_path->size);
		next_rooms = queue_to_rooms(next_path->queue, next_path->size);

		if (!current_rooms || !next_rooms){
			free(current_rooms);
			free(next_rooms);
			return (-1);
		}

		if (paths_share_room(current_rooms, current_path->size,
				next_rooms, next_path->size)){
			delete_path(data, next_list);
			free(current_rooms);
			free(next_rooms);

			/* Recommence depuis le premier chemin */
			current_list = data->valid_paths;
			continue ;
		}

		free(current_rooms);
		free(next_rooms);
		current_list = current_list->next;
	}

	return (0);
}
