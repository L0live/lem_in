#include	"../includes/lem_in.h"

void print_room(t_data *data){
	t_room *room = data->rooms;

	ft_printf("\n\n##### Lem_in Data #####\n\n");
	ft_printf("Total ants: %d\n", data->total_ants);
	while (room){
		if (room == data->start)
			ft_printf("\nRoom id: %d (Start)\n", room->id);
		else if (room == data->end)
			ft_printf("\nRoom id: %d (End)\n", room->id);
		else
			ft_printf("\nRoom id: %d\n", room->id);
		ft_printf("	At x: %d, y: %d\n", room->x, room->y);
		ft_printf("	Linked to: ");
		for (int i = 0; i < room->links_size - 1; i++)
			ft_printf("%d, ", room->links[i]);
		if (room->links_size > 0)
			ft_printf("%d\n", room->links[room->links_size - 1]);

		room = room->next;
	}
	return ;

}

void	read_stdin(t_list **stdin_content){
	char *line;

	line = get_next_line(STDIN_FILENO);
	while (line != NULL ){
		if (line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
		ft_lstadd_back(stdin_content, ft_lstnew(line));
		line = get_next_line(STDIN_FILENO);
	}
};

int main(void){
	t_list	*stdin_content = NULL;
	t_data	data;

	data.total_ants = 0;
	data.rooms = NULL;
	data.start = NULL;
	data.end = NULL;
	data.matrix = NULL;

	read_stdin(&stdin_content);
	ft_lstprint(stdin_content);
	
	parsing(stdin_content, &data);
	print_room(&data);
	ft_lstclear(&stdin_content, &free);

	data.matrix = create_matrix(data.rooms);
	print_matrix(data.matrix);

	free_rooms(data.rooms);
	free_matrix(data.matrix);
	return (0);
};