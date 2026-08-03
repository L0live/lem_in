#include	"../includes/lem_in.h"

// t_ant_queue    *queue_addnew(t_ant_queue *queue){
//     queue->queue = NULL;
//     queue->size = 0;
//     queue->next = NULL;
//     return (queue);
// };

#include	"../includes/lem_in.h"

int path_queue_addnew(t_path *path, t_room *room){
	t_list *new = ft_lstnew((void *)room);
	if (!new)
		return (-1);
	
	ft_lstadd_back(&path->queue, new);
	path->size++;
	
	return (0);
};

int path_addnew(t_path **path, t_room *first_room, t_path *parent_path){
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

	if (first_room && path_queue_addnew(new, first_room) == -1) {
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
			ft_printf("	parent  : %s\n", ((t_room*)(ft_lstlast(path->parent_path->queue)->content))->name);
		ft_printf("	queue : ");
				
		t_list *queue = path->queue;
		while (queue != NULL){
			char *room_name = ((t_room *)queue->content)->name;
			ft_printf("%s ", room_name);
			queue = queue->next;
		}
		path = path->next;
	};
	ft_printf("\n\n");
};


int     breadthfirst_search(t_data *data){
	t_path *paths = NULL;
	t_room *current_room = room_getby_id(data->rooms, data->start_id);

	if (path_addnew(&paths, current_room, NULL) == -1)
		return (-1);

	data->paths = paths;

	t_path  *current_path = paths;
	while (current_path){
		for (int i = 0; i < current_room->links_size; i++){
			t_room *tmp_room = room_getby_id(data->rooms, current_room->links[i]);
			if ((current_path->parent_path && tmp_room->id == ((t_room*)(ft_lstlast(current_path->parent_path->queue)->content))->id)
				|| (tmp_room->links_size == 1 && tmp_room->id != data->end_id))
				continue;
			if(!tmp_room->visited){
				if (current_room->links_size > 2) {
					if (path_addnew(&paths, tmp_room, current_path) == -1)
						return (-1);
				}
				else if (path_queue_addnew(current_path, tmp_room) == -1)
					return (-1);
				if (tmp_room->id != data->end_id)
					tmp_room->visited = true;
				ft_printf("tmp id :%s, verified :%d\n",tmp_room->name, tmp_room->visited );
			}
			else {
				// t_path *visited_path = get_visited_path(paths, tmp_room->id);

				// int queue_count = path_nlen(visited_path, tmp_room->id);
				// if (queue_count <= )
			}
		}
		// current_path = get_next_path(paths, step);
		current_path = current_path->next; // Pas la bonne solution
		if (!current_path)
			break;
		current_room = (t_room *)ft_lstlast(current_path->queue)->content;
	}

	print_paths(data->paths);
	return (0);
};
