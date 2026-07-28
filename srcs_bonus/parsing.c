#include "../includes/lem_in.h"

int    ft_isstriter(const char *str, int (*f)(int)){
	while (*str){
		if (!f(*str))
			return (0);
		str++;
	}
	return (1);
}

int    pars_room_id(char *room_id){
	if (ft_isstriter(room_id, &ft_isdigit)){
		return (ft_atoi(room_id));
	}
	else if (!ft_strcmp(room_id, "start")) {
		return (START_ID);
	}
	else if (!ft_strcmp(room_id, "end")) {
		return (END_ID);
	}
	return (-3);
}

void	free_split(char **split) {
	if (split == NULL)
		return ;
	for (int i = 0; split[i] != NULL; i++)
		free(split[i]);
	free(split);
}

int    handle_new_room(t_data *data, char *line, int is_special){
	int id, x, y;
	char    **split_line = ft_split(line, ' ');

	if (!split_line || !split_line[0] || !split_line[1] || !split_line[2] || split_line[3])
		return (1);

	id = pars_room_id(split_line[0]);
	if (id == -3) {
		free_split(split_line);
		return (1);
	}

	if (!ft_isstriter(split_line[1], &ft_isdigit) || !ft_isstriter(split_line[2], &ft_isdigit)) {
		free_split(split_line);
		return (1);
	}
	x = ft_atoi(split_line[1]);
	y = ft_atoi(split_line[2]);
	free_split(split_line);
	room_addnew(&data->rooms, id, x, y);

	if (is_special == START_ID)
	data->start = room_getby_id(data->rooms, id);
	else if (is_special == END_ID)
	data->end = room_getby_id(data->rooms, id);
	return (0);
}

int    parsing(t_list *stdin_content, t_data *data){
	t_list *current = stdin_content;
	char *line = current->content;
	bool    is_links_part = false;

	if (!ft_isstriter(line, &ft_isdigit)){
		return (1);
	}
	data->total_ants = ft_atoi(line);
	current = current->next;
	while (current != NULL) {
		line = current->content;
			if (line[0] == '#') {
			if (line[1] == '#') {
				if (!ft_strcmp(line, "##start")) {
					current = current->next;
					if (handle_new_room(data, current->content, START_ID)) {
						ft_printf("Error : handle_new_room(data, current->content, START_ID)");
						return (1);
					}
				}
				else if (!ft_strcmp(line, "##end")) {
					current = current->next;
					if (handle_new_room(data, current->content, END_ID)) {
						ft_printf("Error : handle_new_room(data, current->content, END_ID)");
						return (1);
					}
				}
			}
		}
		else if (ft_strchr(line, '-')) {
			is_links_part = true;
			char **split_line = ft_split(line, '-');
			if (!split_line || !split_line[0] || !split_line[1] || split_line[2]) {
				free_split(split_line);
				return (1);
			}
			int room_id = pars_room_id(split_line[0]);
			if (room_id == -3) {
				free_split(split_line);
				return (1);
			}
			int link_id = pars_room_id(split_line[1]);
			free_split(split_line);
			if (link_id == -3) {
				return (1);
			}
			room_addlink(data->rooms, room_id, link_id);
		}
		else if (!is_links_part) {
			if (handle_new_room(data, line, 0)) {
				ft_printf("Error : handle_new_room(data, line, 0)");
				return (1);
			}
		}
		current = current->next;
	}
	if (!data->rooms || !data->start || !data->end) {
		return (1);
	}
	return (0);
}