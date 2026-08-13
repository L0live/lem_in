#include "../includes/lem_in.h"

int	room_addnew(t_room **rooms, int id, char *name, int x, int y){
	t_room	*new;

	new = malloc(sizeof(t_room));
	if (!new)
		return (-1);

	new->id = id;
	new->name = name;
	new->x = x;
	new->y = y;
	new->visited = false;
	new->links = NULL;
    new->links_size = 0;
    new->next = NULL;
    
    if (*rooms == NULL){
        *rooms = new;
        return (0);
    }
    t_room *current = *rooms;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new;
    return (0);
}

t_room	*room_getby_id(t_room *rooms, int id){
    while (rooms != NULL){
        if (rooms->id == id)
            return (rooms);
        rooms = rooms->next;
    }
    return (NULL);
}

t_room	*room_getby_name(t_room *rooms, char *name){
    while (rooms != NULL){
        if (ft_strcmp(rooms->name, name) == 0)
            return (rooms);
        rooms = rooms->next;
    }
    return (NULL);
}

int room_islink(t_room *room, int link_id) {
    for (int i = 0; i < room->links_size; i++) {
        if (room->links[i] == link_id)
            return (-1);
    }
    return (0);
}

int    room_addlink(t_room *rooms, int room_id, int link_id) {
    t_room *room = room_getby_id(rooms, room_id);
    if (room == NULL)
        return (-1);
    if (room->links == NULL) {
        room->links = malloc(sizeof(int));
        if (room->links == NULL)
            return (-1);
        room->links[0] = link_id;
        room->links_size = 1;
    } else {
        if (room_islink(room, link_id) == -1)
            return (0);
        int *new_links = malloc(sizeof(int) * (room->links_size + 1));
        if (new_links == NULL)
            return (-1);

        for (int i = 0; i < room->links_size; i++)
            new_links[i] = room->links[i];

        free(room->links);
        room->links = new_links;
        room->links[room->links_size] = link_id;
        room->links_size++;
    }
    if (room_addlink(rooms, link_id, room_id) == -1)
        return (-1);
    return (0);
}

void    free_rooms(t_room *rooms) {
    t_room *current = rooms;
    while (current != NULL) {
        t_room *next = current->next;
        free(current->name);
        if (current->links != NULL)
            free(current->links);
        free(current);
        current = next;
    }
}

void    init_data(t_data *data){
    data->total_ants = 0;
    data->rooms = NULL;
    data->start_id = -1;
    data->end_id = -1;
    data->valid_paths = NULL;
    data->paths = NULL;
}
