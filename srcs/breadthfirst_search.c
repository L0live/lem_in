#include	"../includes/lem_in.h"

static int	should_skip_room(t_data *data, t_room *room){

	//si cul de sac et si pas le end
	if (room->id != data->end_id && room->links_size == 1)
		return (1);

	if (room->id == data->start_id)
		return (1);

	if (room->visited)
		return (1);
	
	return (0);
};

// retire le premier chemin de queue et on le return pour set currentpath
t_path	*path_queue_take_first(t_path **queue){
	t_path *first;

	if (!queue || !*queue)
		return (NULL);

	first = *queue;
	*queue = first->next;
	first->next = NULL;

	return (first);
}

// ajout path a la fin de queue
void	path_queue_push_back(t_path **queue, t_path *new_path){
	t_path *last;

	new_path->next = NULL;

	if (*queue == NULL){
		*queue = new_path;
		return;
	}

	last = *queue;
	while (last->next)
		last = last->next;

	last->next = new_path;
}


// new path - avec possible chemin parent. ajout du chemin a all path pour plus simple le clean
t_path	*path_new(t_data *data, t_room *room, t_path *parent){

	t_path  *path = malloc(sizeof(t_path));
	if (!path)
		return (NULL);

	t_list  *node = malloc(sizeof(t_list));
	if (!node){
		free(path);
		return (NULL);
	}

	node->content = room;
	node->next = NULL;

	path->queue = node;
	if (!parent)
		path->size = 0;
	else
		path->size = parent->size + 1;
	path->ants = 0;
	path->id_ants = -1;
	path->parent_path = parent;
	path->next = NULL;

	t_list *save_node = malloc(sizeof(t_list));
	if (save_node) {
		save_node->content = path;
		save_node->next = NULL;
		ft_lstadd_back(&data->all_paths, save_node);
	}

	return (path);
}

int	bfs(t_data	*data){

	t_path	*queue;
	t_path	*current_path;
	t_path	*new_path;
	t_room	*current_room;
	t_room	*child_room;
	int		added = false;

	queue = NULL;
	data->paths = NULL;

	current_room = room_getby_id(data->rooms, data->start_id);
	current_path = path_new(data, current_room, NULL);
	
	data->paths = current_path;
	path_queue_push_back(&queue, current_path);
	while (queue){

		
		// recupere du prochain chemin a trater
		current_path = path_queue_take_first(&queue);
		//current room reprense la room sur laquelle on est dans le chemin current_path
		current_room = current_path->queue->content;
		
		//si on est a la fin alors on creer le path valid
		if (current_room->id == data->end_id){

			// on creer le path final avec toute les rooms des parents dans queueu
			t_path *finale_path = path_build_final(current_path);
			if (!finale_path)
				return (-1);

			if (path_already_in_valid_path(data, finale_path)){
				free_one_path_content(finale_path);
				continue;
			}

			//ajout aux chemin valide
			if (valid_path_add(&data->valid_paths, finale_path) == -1){
				free_one_path_content(finale_path);
				return (-1);
			}
			added = true;
			continue;
		}
		
		for (int i = 0; i < current_room->links_size; i++){
			child_room = room_getby_id(data->rooms, current_room->links[i]);

			if (should_skip_room(data, child_room))
				continue;			

			new_path = path_new(data, child_room, current_path);
			if (!new_path)
				return (-1);
						
			//! si start et end directement connecté
			if (child_room->id == data->end_id && current_room->id == data->start_id){
				
				//Si chemin existe deja on skip
				if (path_already_in_valid_path(data, new_path)){
					new_path = NULL;
					continue;
				}
			}

			// ajout new chemin dans queue BFS lutilisr plus tard
			path_queue_push_back(&queue, new_path);

			if (child_room->id != data->end_id && child_room->id != data->start_id)
				child_room->visited = true;
		}
	};


	// clean les chmeins qui reste dans queue si rien trouve	
	while (queue)
		queue = path_queue_take_first(&queue);

	// liberation des chemins saves -> save_node
	ft_lstclear(&data->all_paths, free_one_path_content);
	data->paths = NULL;
	if (added == true)
		return (1);
	else
		return (0);
};
