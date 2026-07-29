#include	"../includes/lem_in_bonus.h"

int main(void){
	t_list	*stdin_content = NULL;
	t_data	data;

	read_stdin(&stdin_content);
	ft_lstprint(stdin_content);
	
	init_data(&data);
	if (!parsing(stdin_content, &data))
		return (1);
	print_room(&data);

	ft_lstclear(&stdin_content, &free);

	free_rooms(data.rooms);
	return (0);
};