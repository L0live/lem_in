#include "../includes/lem_in.h"

int    ft_isstriter(const char *str, int (*f)(int)){
	while (*str){
		if (!f(*str))
			return (0);
		str++;
	}
	return (-1);
}

void	free_split(char **split) {
	if (split == NULL)
		return ;
	for (int i = 0; split[i] != NULL; i++)
		free(split[i]);
	free(split);
}

// int		is_coord_already_used(t_room *rooms, int x, int y){
// 	if (!rooms)
// 		return (0);
	
// 	while (rooms){
// 		if (rooms->x == x && rooms->y == y)
// 			return (1);		
// 		rooms = rooms->next;
// 	}
// 	return (0);
// }

int    handle_new_room(t_room **rooms, char *line, int new_id){
	int x, y;
	// int id, x, y;
	char    **split_line = ft_split(line, ' ');
	char	*name;

	// ft_printf("\n\nline %s and split_line %s\n",line, split_line);
	if (!split_line || !split_line[0] || !split_line[1] || !split_line[2] || split_line[3]){
		free_split(split_line);
		return (-1);
	}

	if (!ft_isstriter(split_line[1], &ft_isdigit) || !ft_isstriter(split_line[2], &ft_isdigit)) {
		free_split(split_line);
		return (-1);
	}

	name = ft_strdup(split_line[0]);
	if (!name) {
		free_split(split_line);
		return (-1);
	}
	x = ft_atoi(split_line[1]);
	y = ft_atoi(split_line[2]);
	free_split(split_line);
	
	// ft_printf("\n\new_id %d and name %s\n",new_id, name);
	if(room_getby_name(*rooms, name) || room_addnew(rooms, new_id, name, x, y) == -1) {
		free(name);
		return (-1);
	}

	return (0);
}

int	is_boundary(t_data *data, char *line, t_list *current, int id) {

	if (!current || !current->content){
		ft_printf("next line ##start or end its her line settings");
		return (-1);
	}
	if (!ft_strcmp(line, "start")) {
		if (handle_new_room(&data->rooms, current->content, id) == -1)
			return (-1);
		data->start_id = id;
	}
	else if (!ft_strcmp(line, "end")) {

		if (handle_new_room(&data->rooms, current->content, id) == -1)
			return (-1);
		data->end_id = id;
	}
	return (0);
};

int	handle_new_link(t_room *rooms, char *line, int start_id) {
	t_room 	*tmp;
	int 	room_id;
	int 	link_id;
	char **split_line = ft_split(line, '-');
	if (!split_line || !split_line[0] || !split_line[1] || split_line[2]) {
		free_split(split_line);
		return (-1);
	}

	tmp = room_getby_name(rooms, split_line[0]);
	if (!tmp) {
		free_split(split_line);
		return (-1);
	}
	room_id = tmp->id;

	tmp = room_getby_name(rooms, split_line[1]);
	if (!tmp) {
		free_split(split_line);
		return (-1);
	}
	link_id = tmp->id;

	free_split(split_line);

	if (room_addlink(rooms, room_id, link_id, start_id) == -1)
		return (-1);

	return (0);
}

int    parsing(t_list *stdin_content, t_data *data){
	t_list 	*current = stdin_content;
	char	*line = current->content;
	bool    is_links_part = false;
	int		num_line = 1;
	int		id = 0;
	
	if (!ft_isstriter(line, &ft_isdigit) || line[0] == '\0')
		return (-1);

	data->total_ants = ft_atoi(line);
	ft_printf("%s\n", current->content);

	current = current->next;
	while (current != NULL) {
		line = current->content;
		if (line[0] == '\0' || line[0] == 'L')
			return (-1);
		
		if (line[0] == '#') {
			if (line[1] == '#'){
				ft_printf("%s\n", line);
				if (is_boundary(data, &line[2], current->next, id) == -1)
					return (-1);
				else{
					id++;
					num_line++;
					current = current->next;
				}
			}
		}
		else if (ft_strchr(line, '-')) {
			is_links_part = true;
			if(handle_new_link(data->rooms, line, data->start_id) == -1)
				return (-1);
		}
		else if (!is_links_part) {
			if (handle_new_room(&data->rooms, line, id) == -1)
				return (-1);
			id++;
		}
		else
			return (-1);
		ft_printf("%s\n", current->content);
		num_line++;
		current = current->next;
	}

	return (0);
}