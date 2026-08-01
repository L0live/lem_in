#include	"../includes/lem_in.h"

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

	free_paths(data.paths);
	free_rooms(data.rooms);
	return (0);
};