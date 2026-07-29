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

int    handle_new_room(t_data *data, char *line, int new_id){
	int id, x, y;
	char    **split_line = ft_split(line, ' ');
	char	*name;
	
	if (!split_line || !split_line[0] || !split_line[1] || !split_line[2] || split_line[3])
		return (-1);
	
	id = new_id;

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
	
	if(room_addnew(&data->rooms, id, name, x, y) == -1) {
		free(name);
		return (-1);
	}
	

	return (0);
}

int	is_boundary(t_data *data, char *line, t_list *current, int id) {
	
	if (!ft_strcmp(line, "start")) {
		data->start_id = id;
		if (handle_new_room(data, current->content, id) == -1) {
			ft_printf("Error : handle_new_room(data, %s, %d)", current->content, id);
			return (-1);
		}
	}
	else if (!ft_strcmp(line, "end")) {
		data->end_id = id;
		if (handle_new_room(data, current->content, id) == -1) {
			ft_printf("Error : handle_new_room(data, %s, %d)", current->content, id);
			return (-1);
		}
	}
	return (0);
};

int	handle_new_link(t_data *data, char *line) {
	t_room 	*tmp;
	int 	room_id;
	int 	link_id;
	char **split_line = ft_split(line, '-');
	if (!split_line || !split_line[0] || !split_line[1] || split_line[2]) {
		free_split(split_line);
		return (-1);
	}

	tmp = room_getby_name(data->rooms, split_line[0]);
	if (!tmp) {
		free_split(split_line);
		return (-1);
	}
	room_id = tmp->id;

	tmp = room_getby_name(data->rooms, split_line[1]);
	if (!tmp) {
		free_split(split_line);
		return (-1);
	}
	link_id = tmp->id;

	free_split(split_line);

	if (room_addlink(data->rooms, room_id, link_id) == -1)
		return (-1);

	return (0);
}

int    parsing(t_list *stdin_content, t_data *data){
	t_list *current = stdin_content;
	char *line = current->content;
	bool    is_links_part = false;
	int		id = 0;
	
	if (!ft_isstriter(line, &ft_isdigit))
		return (-1);

	data->total_ants = ft_atoi(line);
	current = current->next;
	while (current != NULL) {
		
		line = current->content;
		if (line[0] == '#') {
			if (line[1] == '#'){
				if (is_boundary(data, &line[2], current->next, id) == -1)
					return (-1);
				else{
					id++;
					current = current->next;
				}
			}
		}
		else if (ft_strchr(line, '-')) {
			is_links_part = true;
			if(handle_new_link(data, line) == -1) {
				ft_printf("Error : handle_new_link(data, line)");
				return (-1);
			}
		}
		else if (!is_links_part) {
			if (handle_new_room(data, line, id) == -1) {
				ft_printf("Error : handle_new_room(data, line, id)");
				return (-1);
			}
			id++;
		}
		current = current->next;
	}
	if (!data->rooms || data->start_id == -1 || data->end_id == -1)
		return (-1);
	return (0);
}