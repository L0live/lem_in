#include "../includes/lem_in.h"

void    print_room(t_data *data){
	t_room *room = data->rooms;

	ft_printf("\n\n##### Lem_in Data #####\n\n");
	ft_printf("Total ants: %d\n", data->total_ants);
	while (room){
		if (room->id == data->start_id)
			ft_printf("\nRoom id: %d (Start)\n", room->id);
		else if (room->id == data->end_id)
			ft_printf("\nRoom id: %d (End)\n", room->id);
		else
			ft_printf("\nRoom id: %d\n", room->id);
		ft_printf("	Name: %s\n", room->name);
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

int	read_stdin(t_list **stdin_content){
	char *line;
	t_list *tmp;

	line = get_next_line(STDIN_FILENO);
	while (line != NULL ){
		if (line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
		if (line[0] != '\0') {
			tmp = ft_lstnew(line);
			if (!tmp) {
				free(line);
				ft_lstclear(stdin_content, &free);
				return(-1);
			}
			ft_lstadd_back(stdin_content, tmp);
		}
		else
			free(line);
		line = get_next_line(STDIN_FILENO);
	}
	return(0);
};