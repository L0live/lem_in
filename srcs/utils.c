#include "../includes/lem_in.h"

int		minimum_data(t_data *data){
	if (data->total_ants == 0){
		ft_putstr_fd("Error : Not enough ants\n", 2);
		return (-1);
	}
	if (!data->rooms || !data->rooms->next){
		ft_putstr_fd("Error : Not enough rooms\n", 2);
		return (-1);
	}
	
	if (data->start_id == -1){
		ft_putstr_fd("Error : Start room not set\n", 2);
		return (-1);
	}
	if (data->end_id == -1){
		ft_putstr_fd("Error : End room not set\n", 2);
		return (-1);
	}

	t_room *start = room_getby_id(data->rooms, data->start_id);
	t_room *end = room_getby_id(data->rooms, data->end_id);

	if (!start || !end)
		return (-1);

	if (start->links_size == 0){
		ft_putstr_fd("Error : Start have no link\n", 2);
		return (-1);
	}		
	if (end->links_size == 0){
		ft_putstr_fd("Error : End have no link\n", 2);
		return (-1);
	}

	return (0);
}

void    print_room(t_data *data){
	t_room *room = data->rooms;

	ft_printf("\n\n##### Lem_in Data #####\n\n");
	ft_printf("Total ants: %d\n", data->total_ants);
	while (room){
		if (room->id == data->start_id)
			ft_printf("\n\nRoom id: %d (Start)\n", room->id);
		else if (room->id == data->end_id)
			ft_printf("\n\nRoom id: %d (End)\n", room->id);
		else
			ft_printf("\n\nRoom id: %d\n", room->id);

		ft_printf("	Name: %s\n", room->name);
		ft_printf("	At x: %d, y: %d\n", room->x, room->y);
		ft_printf("	Link Size: %d\n", room->links_size);
		ft_printf("	Linked to: ");
		for (int i = 0; i < room->links_size; i++)
			ft_printf("%s, ", room_getby_id(data->rooms, room->links[i])->name);

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
		tmp = ft_lstnew(line);
		if (!tmp) {
			free(line);
			ft_lstclear(stdin_content, &free);
			return(-1);
		}
		ft_lstadd_back(stdin_content, tmp);
		line = get_next_line(STDIN_FILENO);
	}
	return(0);
};