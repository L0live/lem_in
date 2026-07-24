#include	"../includes/lem_in.h"

void print_room(t_room *room){

	while (room){
		ft_printf("\n\nid de la room %d\n", room->id);
		ft_printf("x %d, ", room->x);
		ft_printf("y %d\n", room->y);
		for (int i = 0; i < room->links_size; i++)
			ft_printf("%d - %d\n", room->id, room->links[i]);
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
	read_stdin(&stdin_content);
	ft_lstprint(stdin_content);
	
	parsing(stdin_content, &data);
	// printf("%d", parsing(stdin_content, &data));
	print_room(data.rooms);

	ft_lstclear(&stdin_content, &free);
	return (0);
};