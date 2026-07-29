#include	"../includes/lem_in_bonus.h"

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

	print_room(&data);

	free_rooms(data.rooms);
	return (0);
};