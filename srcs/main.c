#include	"../includes/lem_in.h"

// void	ants_actions_loop(t_data *data){
// 	t_list *paths = data->valid_paths;
// 	int ants_id = 1;

// 	while (data->total_ants > 6)
// 	{
// 		while (paths){
// 			t_path *tmp_path = (t_path*)paths->content;
			
// 			t_list	*queue = tmp_path->queue;
// 			while(queue && queue->next){
				
// 				t_room *tmp_room = queue->content;
				
// 				ft_printf("tmp_path->ants : %d, is_visited : %d\n", tmp_path->ants, tmp_room->visited);
// 				if (!tmp_room->visited) {
// 					if (tmp_path->ants > 0) {
// 						tmp_room->visited = ants_id++;if (tmp->id != data->end_id)
// 						tmp->visited = tmp_room->visited;
// 					else
// 						data->total_ants--;
// 						tmp_path->ants--;
// 						ft_printf("L%d-%s ", tmp_room->visited, tmp_room->name);
// 					}
// 					break;
// 				}
// 				else if (!((t_room*)queue->next->content)->visited) {
// 					t_room *tmp = queue->next->content;
// 					if (tmp->id != data->end_id)
// 						tmp->visited = tmp_room->visited;
// 					else
// 						data->total_ants--;
// 					ft_printf("L%d-%s ", tmp_room->visited, tmp->name);
// 					tmp_room->visited = 0;
// 					break;
// 				}
// 				queue = queue->next;
// 			}
// 			paths = paths->next;
// 		}
// 		ft_printf("\n");
// 		ft_printf("data->total_ants %d: \n", data->total_ants);
// 		ft_printf("\n");
// 	}
// };

t_room	**path_tabl(t_list *queue, int size){

	t_room **rooms;
	int	i = 0;

	rooms = malloc(sizeof(t_room*) * size);
	if (!rooms)
		return NULL;	

	while (queue && i < size){
		rooms[i] = queue->content;
		queue = queue->next;
		i++;
	};
	
	return (rooms);
};

int count = 0;

void	ants_move(t_data *data, t_path *path, int *id){

	//on deplace toute les fourmis deja presentes dans le path
	//! tableau de room ;)
	t_room **rooms = path_tabl(path->queue, path->size);
	if (!rooms)
		return;	
		//! return -1;	

	int	i = path->size - 1;
	while (i > 0){
		
		if (rooms[i - 1]->visited != 0 && rooms[i]->visited == 0){
			int	tmp_id = rooms[i - 1]->visited;
			rooms[i]->visited = tmp_id;
			rooms[i - 1]->visited = 0;

			ft_printf("L%d-%s ", tmp_id, rooms[i]->name);
			//! a del
			count++;
			if(rooms[i]->id == data->end_id){
				(data->total_ants)--;
				rooms[i]->visited = 0;
			}
		}
		i--;
	};

	//on ajoute une nouvelle fourmis
	if (path->ants > 0 && rooms[i]->visited == 0){
		// if (rooms[i]->id == data->start_id)
		// 	i++;
		rooms[i]->visited = *id;
		ft_printf("L%d-%s ", rooms[i]->visited, rooms[i]->name);
		//! a del
		count++;
		if(rooms[i]->id == data->end_id){
			(data->total_ants)--;
			rooms[i]->visited = 0;
		}
		path->ants--;
		(*id)++;
	}
	free(rooms);
};

void	ants_actions_loop(t_data *data){
	int ant_id = 1;

	ft_printf("\n");
	while (data->total_ants > 0)
	{
		t_list *paths = data->valid_paths;
		while (paths){
		// ft_printf("((t_path *)paths->content)->size : %d\n", ((t_path *)paths->content)->size);
			ants_move(data, paths->content, &ant_id);		
			paths = paths->next;
		}
		ft_printf("\n");
		// ft_printf("\tTotal ant restant : %d\n", data->total_ants);
	}
	ft_printf("Total de deplacement :%d\n", count);
};

t_list *find_best_path(t_list *paths){
	t_list *meilleur = paths;

	while (paths){
		if (((t_path*)paths->content)->size + ((t_path*)paths->content)->ants < ((t_path*)meilleur->content)->size + ((t_path*)meilleur->content)->ants)
			meilleur = paths;		
		paths = paths->next;		
	};
	return (meilleur);
};

void	attribute_ants(t_data *data){

	int total_fourmis = data->total_ants;
	int	ants_id = 0;

	while (total_fourmis > 0){
		t_list *path = find_best_path(data->valid_paths);
		ants_id ++;
		((t_path*)path->content)->ants ++;
		total_fourmis--;
	};
};

void	clean_paths(t_list *paths){
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

void	reset_paths(t_list *paths) {
    t_list	*queue;
    t_path	*path;
    t_room	*room;

    while (paths){
        path = paths->content;
        queue = path->queue;

        while (queue){
            room = queue->content;
            room->visited = 0;
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

	breadthfirst_search(&data);

	if (data.valid_paths){
		attribute_ants(&data);
		clean_paths(data.valid_paths);
		reset_paths(data.valid_paths);
		// ft_lstiter(data.valid_paths, &print_valid);
		ants_actions_loop(&data);
	}

	free_paths(data.paths);
	free_rooms(data.rooms);
	return (0);
};