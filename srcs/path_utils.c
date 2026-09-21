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
