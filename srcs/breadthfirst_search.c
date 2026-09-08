#include	"../includes/lem_in.h"


int	path_addcopy(t_list **paths, t_path *path) {
	t_list *new = ft_lstnew((void *)path);
	if (!new)
		return (-1);
	
	ft_lstadd_back(paths, new);
	return (0);
};

int	free_and_join_between_path(t_path *paths, t_path *path) {
	while (paths->next) {
		// ft_printf("\nfree_and_join_between_path (is equal to path)\n");
		// print_onepath(path->next);
		if (paths->next == path) {
			// ft_lstclear(&paths->next->queue, NULL);
			t_path	*tmp = paths->next->next;
			free(paths->next);
			paths->next = tmp;
			return (0);
		}
		paths = paths->next;
	}
	free(path);
	return (1);
}

void	reset_child_paths(t_path *paths, t_path *path) {
	ft_printf("\n	# target path:");
	print_onepath(path);
	while (paths && paths->next) {
		ft_printf("\n	# current path:");
		print_onepath(paths->next);
		ft_printf("\n	# current parent_path:");
		print_onepath(paths->next->parent_path);
		if (paths->next->parent_path == path) {
			ft_printf("\n	## ^is cleanable^\n");
			t_list	*queue = paths->next->queue;
			while (queue) {
				((t_room*)queue->content)->visited = false;
				queue = queue->next;
			}
			ft_lstclear(&paths->next->queue, NULL);
			t_path	*tmp = paths->next->next;
			free(paths->next);
			paths->next = tmp;
		}
		paths = paths->next;
	}
}

t_path	*path_queue_addnew(t_data *data, t_path *path, t_room *room, int *safe_return){
	t_list *new = ft_lstnew((void *)room);
	if (!new)
		return (NULL);
	
	ft_lstadd_back(&path->queue, new);
	path->size++;

	if (data && room->id == data->end_id){
		while (path->parent_path && (path->parent_path->parent_path || path->parent_path->size > 1)){
			ft_lstadd_back(&path->parent_path->queue, path->queue);
			// path->queue = NULL;
			path->parent_path->size = path->size;
			t_path	*tmp = path->parent_path;
			print_paths(data->paths);
			if (safe_return)
				*safe_return = free_and_join_between_path(data->paths, path);
			else
				free_and_join_between_path(data->paths, path);
			path = tmp;
			print_paths(data->paths);
			reset_child_paths(data->paths, path);
		};
		if (path_addcopy(&data->valid_paths, path) == -1)
			return (NULL);
		return (path);
	}
	
	return (path);
};

int path_addnew(t_data *data, t_path **path, t_room *first_room, t_path *parent_path){
	int	safe_return = false;
	t_path *new = malloc(sizeof(t_path));
	if (!new)
		return (-1);

	new->queue = NULL;
	if (parent_path)
		new->size = parent_path->size;
	else
		new->size = 0;
	new->parent_path = parent_path;
	new->ants = 0;
	new->next = NULL;

	if (first_room) {
		new = path_queue_addnew(data, new, first_room, &safe_return);
		if (!new) {
			free(new);
			return (-1);
		}
		if (safe_return)
			return (0);
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
		ft_printf("	ants : %d\n", path->ants);
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

	while (tmp && tmp->next != NULL) {
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

void print_onepath(void *valid){
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
	ft_printf("\n	ants : %d", path->ants);
	ft_printf("\n	queue : ");
			
	t_list	*queue = path->queue;
	while (queue != NULL){
		char *room_name = ((t_room *)queue->content)->name;
		ft_printf("%s ", room_name);
		queue = queue->next;
	}
	ft_printf("\n");
};

int breadthfirst_search(t_data *data){
	t_path *paths = NULL;
	t_room *current_room = room_getby_id(data->rooms, data->start_id);

	if (path_addnew(NULL, &paths, current_room, NULL) == -1)
		return (-1);

	data->paths = paths;
	t_path *current_path = paths;

	while (current_path){
		if (current_path->parent_path && !current_path->queue){
			current_path = current_path->next;
			continue;		
		}
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
			ft_printf("child room: %s\n", child_room->name);
			if (valid_neighbors > 1){
				if (path_addnew(data, &paths, child_room, current_path) == -1)
					return (-1);
			}
			else{
				current_path = path_queue_addnew(data, current_path, child_room, NULL);
				if (!current_path)
					return (-1);
				tmp = current_path;
				current_path = path_readdback(&paths, current_path);
			}
			
			// ft_printf("\n	##########################");
			// print_onepath(current_path->next); 
			if (child_room->id != data->end_id)
				child_room->visited = true;
			else
				break;
		}
		if (tmp == current_path)
			continue;
		current_path = current_path->next;
	}

	print_paths(data->paths);
	return (0);
}
