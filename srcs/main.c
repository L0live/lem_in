#include	"../includes/lem_in.h"

int	main(void){
	t_list	*stdin_content = NULL;
	t_data	data;

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

	if (bfs(&data) == -1){
		free_data(&data);
		return (0);	
	}

	ft_lstiter(data.valid_paths, &print_onepath);

	reset_paths_visited(data.valid_paths);
	remove_start_from_paths(&data);
	ft_lstiter(data.valid_paths, &print_onepath);

	attribute_ants(&data);
	if (ants_actions_loop(&data) == -1){
		free_data(&data);
		return (-1);
	}

	free_data(&data);
	return (0);
};