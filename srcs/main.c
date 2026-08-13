#include	"../includes/lem_in.h"

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

int	ants_actions(t_data *data, t_path *path, int *id){
	int count = 0;

	//! tableau de room ;)
	t_room **rooms = queue_to_rooms(path->queue, path->size);
	if (!rooms)
		return (-1);

	//on deplace toute les fourmis deja presentes dans le path
	for (int i = path->size - 1; i > 0; i--){
		
		if (rooms[i - 1]->visited && !rooms[i]->visited){
			if (rooms[i]->id == data->end_id)
				(data->total_ants)--;
			else
				rooms[i]->visited = rooms[i - 1]->visited;
			
			ft_printf("L%d-%s ", rooms[i - 1]->visited, rooms[i]->name);
			rooms[i - 1]->visited = 0;
			count++;
		}
	};

	//on ajoute une nouvelle fourmis
	if (path->ants > 0 && !rooms[0]->visited){
		if	(rooms[0]->id == data->end_id)
			(data->total_ants)--;
		else
			rooms[0]->visited = *id;
		ft_printf("L%d-%s ", *id, rooms[0]->name);
		count++;
		path->ants--;
		(*id)++;
	}

	free(rooms);
	return (count);
};

int	ants_actions_loop(t_data *data) {
	int actions_count = 0;
	int lignes_count = 0;
	int ant_id = 1;

	ft_printf("\n");
	while (data->total_ants)
	{
		t_list *paths = data->valid_paths;
		while (paths){
		// ft_printf("((t_path *)paths->content)->size : %d\n", ((t_path *)paths->content)->size);
			int path_actions_count = ants_actions(data, paths->content, &ant_id);
			if (path_actions_count == -1)
				return (-1);
			actions_count += path_actions_count;
			paths = paths->next;
		}
		ft_printf("\n");
		lignes_count++;
		// ft_printf("\tTotal ant restant : %d\n", data->total_ants);
	}
	ft_printf("Total d'actions :%d\n", actions_count);
	ft_printf("Total de lignes :%d\n", lignes_count);
	return (0);
};

void	clean_unused_paths(t_list *paths){
	while (paths->next){
		if (((t_path *)paths->next->content)->ants == 0){
			t_list *tmp = paths->next;
			paths->next = tmp->next;
			free(tmp);
		}
		else
			paths = paths->next;
	};
};

void	attribute_ants(t_data *data) {
	t_path *best_path = (t_path*)data->valid_paths->content;
	if (!data->valid_paths->next) {
		best_path->ants = data->total_ants;
		best_path->queue = best_path->queue->next;
		return ;
	}

	for (int i = 0; i < data->total_ants; i++){
		t_list *paths = data->valid_paths;
		while (paths) { // find the best path to add an ant
			t_path *tmp_path = (t_path*)paths->content;
			if (tmp_path->size + tmp_path->ants < best_path->size + best_path->ants)
				best_path = (t_path*)paths->content;		
			paths = paths->next;		
		};
		best_path->ants++;
	};
	clean_unused_paths(data->valid_paths);
};

void	reset_paths(t_list *paths) {
    while (paths){
        t_list *queue = ((t_path *)paths->content)->queue;
        while (queue){
            ((t_room*)queue->content)->visited = 0;
            queue = queue->next;
        }
        paths = paths->next;
    }
}

int main(void){
	t_list	*stdin_content = NULL;
	t_data	data;

	if (read_stdin(&stdin_content) == -1)
		return (-1);
	ft_lstprint(stdin_content);
	
	init_data(&data);
	if (parsing(stdin_content, &data) == -1) {
		ft_lstclear(&stdin_content, &free);
		return (-1);
	}
	ft_lstclear(&stdin_content, &free);

	if (breadthfirst_search(&data) == -1 || !data.valid_paths){
		free_paths(data.paths);
		free_rooms(data.rooms);
		return (-1);
	}

	attribute_ants(&data);
	reset_paths(data.valid_paths);
	ft_lstiter(data.valid_paths, &print_onepath);
	if (ants_actions_loop(&data) == -1){
		free_paths(data.paths);
		free_rooms(data.rooms);
		return (-1);
	}

	free_paths(data.paths);
	free_rooms(data.rooms);
	return (0);
};