#include	"../includes/lem_in.h"

static void reset_visited_other_path(t_data *data){

	t_room	*room;
	t_list	*list;
	t_path	*path;
	t_list	*queue;

	room = data->rooms;
	while (room){
		room->visited = false;
		room = room->next;
	}

	list = data->valid_paths;
	while (list){
		path = list->content;
		queue = path->queue;

		while (queue){
			room = queue->content;
			if (room->id != data->start_id && room->id != data->end_id)
				room->visited = true;
			queue = queue->next;
		}
		list = list->next;
	}
	
};

int	main(void){
	t_list	*stdin_content = NULL;
	t_data	data;
	int		bfs_result;

	if (read_stdin(&stdin_content) == -1)
		return (-1);
	
	init_data(&data);
	parsing(stdin_content, &data);

	if (minimum_data(&data) == -1){
		ft_lstclear(&stdin_content, &free);
		free_rooms(data.rooms);
		return (-1);
	}

	ft_lstclear(&stdin_content, &free);

	while (1){

		// t_room *rooms = data.rooms;
		// for (; rooms; rooms = rooms->next){
		// 	ft_printf("rooms %s visited %d\n", rooms->name, rooms->visited);
		// }		
		bfs_result = bfs(&data);

		if (bfs_result == -1){
			free_data(&data);
			return (0);				
		}
		// ft_lstiter(data.valid_paths, &print_onepath);
		clean_samerooms_paths(&data);
		reset_visited_other_path(&data);		

		if (bfs_result == 0)
			break;		
	}

	ft_lstiter(data.valid_paths, &print_onepath);

	reset_paths_visited(data.valid_paths);
	remove_start_from_paths(&data);

	attribute_ants(&data);
	if (ants_actions_loop(&data) == -1){
		free_data(&data);
		return (-1);
	}

	free_data(&data);
	return (0);
};