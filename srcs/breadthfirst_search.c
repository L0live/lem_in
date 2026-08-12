#include	"../includes/lem_in.h"


int	path_addcopy(t_list **paths, t_path *path) {
	t_list *new = ft_lstnew((void *)path);
	if (!new)
		return (-1);
	
	ft_lstadd_back(paths, new);
	return (0);
};

int path_queue_addnew(t_data *data, t_path *path, t_room *room){
	t_list *new = ft_lstnew((void *)room);
	if (!new)
		return (-1);
	
	ft_lstadd_back(&path->queue, new);
	path->size++;

	if (data && room->id == data->end_id){
		while (path->parent_path && path->parent_path->parent_path){
			ft_lstadd_back(&path->parent_path->queue, path->queue);
			path->queue = path->parent_path->queue;
			path->parent_path->size = path->size;
			path = path->parent_path;
		};
		if (path_addcopy(&data->valid_paths, path) == -1)
			return (-1);
	}
	
	return (0);
};

int path_addnew(t_data *data, t_path **path, t_room *first_room, t_path *parent_path){
	t_path *new = malloc(sizeof(t_path));
	if (!new)
		return (-1);

	new->queue = NULL;
	if (parent_path)
		new->size = parent_path->size;
	else
		new->size = 0;
	new->parent_path = parent_path;
	new->next = NULL;

	if (first_room && path_queue_addnew(data, new, first_room) == -1) {
		free(new);
		return (-1);
	}

	if (*path == NULL){
		*path = new;
		return (0);
	}
	t_path *tmp = *path;
	while (tmp->next != NULL) {
		tmp = tmp->next;
	}
	tmp->next = new;

	return (0);
};

void    free_paths(t_path *paths){
	t_path *tmp;
	while (paths != NULL){
		tmp = paths;
		paths = paths->next;
		ft_lstclear(&tmp->queue, NULL);
		free(tmp);
	}
};

//print name et id
void print_paths(t_path *paths)
{
	t_path *path = paths;
	int path_num = 0;

	while (path != NULL){
		ft_printf("\n\nPath %d :\n", path_num++);
		ft_printf("	size : %d\n", path->size);
		if(path->parent_path)
		ft_printf("	parent queue : ");
		
		if (path->parent_path) {
			t_list *queue = path->parent_path->queue;
			while (queue != NULL){
				char *room_name = ((t_room *)queue->content)->name;
				ft_printf("%s ", room_name);
				queue = queue->next;
			}
		}
		ft_printf("\n	queue : ");
				
		t_list	*queue = path->queue;
		while (queue != NULL){
			char *room_name = ((t_room *)queue->content)->name;
			ft_printf("%s ", room_name);
			queue = queue->next;
		}
		path = path->next;
	};
	ft_printf("\n\n");
};

int room_in_path(t_path *path, int room_id){
	t_path *parent = path;
	while (parent){
		t_list *q = parent->queue;
		while (q){
			if (((t_room*)q->content)->id == room_id)
				return (1);
			q = q->next;
		}
		parent = parent->parent_path;
	}
	return (0);
};

int	valid_room(t_path *current_path , t_room *child_room, int end_id){
	if (child_room->visited && child_room->id != 0)
		return (-1);
	if (room_in_path(current_path, child_room->id))
		return (-1);
	if (child_room->links_size == 1 && child_room->id != end_id)
		return (-1);
	return (0);
};

t_path	*path_readdback(t_path **paths, t_path *current_path){
	t_path *tmp = *paths;
	t_path *rtn = NULL;

	if (current_path->next == NULL)
		return (current_path);

	while (tmp->next != NULL) {
		if(tmp->next == current_path){
			rtn = tmp;
			tmp->next = current_path->next;
		}
		tmp = tmp->next;
	}
	tmp->next = current_path;
	current_path->next = NULL;
	return (rtn);
};

void print_valid(void *valid){
	t_path	*path = (t_path *)valid;

	ft_printf("\n\nPath:\n");
		ft_printf("	size : %d\n", path->size);
		if(path->parent_path)
		ft_printf("	parent queue : ");
		
		if (path->parent_path) {
			t_list *queue = path->parent_path->queue;
			while (queue != NULL){
				char *room_name = ((t_room *)queue->content)->name;
				ft_printf("%s ", room_name);
				queue = queue->next;
			}
		}
		ft_printf("\n	queue : ");
				
		t_list	*queue = path->queue;
		while (queue != NULL){
			char *room_name = ((t_room *)queue->content)->name;
			ft_printf("%s ", room_name);
			queue = queue->next;
		}
};

int breadthfirst_search(t_data *data){
	t_path *paths = NULL;
	t_room *current_room = room_getby_id(data->rooms, data->start_id);

	if (path_addnew(NULL, &paths, current_room, NULL) == -1)
		return (-1);

	data->paths = paths;
	t_path *current_path = paths;

	while (current_path){
		current_room = (t_room *)ft_lstlast(current_path->queue)->content;

		if (current_room->id == data->end_id){
			current_path = current_path->next;
			continue;
		}

		int valid_neighbors = 0;
		for (int i = 0; i < current_room->links_size; i++){
			t_room *child_room = room_getby_id(data->rooms, current_room->links[i]);
			if (valid_room(current_path, child_room, data->end_id) == -1)
				continue;
			valid_neighbors++;
		}

		t_path	*tmp = NULL;
		for (int i = 0; i < current_room->links_size; i++){
			t_room *child_room = room_getby_id(data->rooms, current_room->links[i]);
			if (valid_room(current_path, child_room, data->end_id) == -1)
				continue;
			if (valid_neighbors > 1){
				if (path_addnew(data, &paths, child_room, current_path) == -1)
					return (-1);
			}
			else{
				if (path_queue_addnew(data, current_path, child_room) == -1)
					return (-1);
				tmp = current_path;
				current_path = path_readdback(&paths, current_path);
			} 
			if (child_room->id != data->end_id)
				child_room->visited = true;
		}
		if (tmp == current_path)
			continue;
		current_path = current_path->next;
	}

	// print_paths(data->paths);

	ft_lstiter(data->valid_paths, &print_valid);
	return (0);
}
