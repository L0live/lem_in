#include "../includes/lem_in.h"

// typedef struct room_s{
// 	int				id;
// 	int				x, y;
// 	bool			visited;
// 	struct room_s	**links;
// 	struct room_s	*next;
// } t_room;

void	room_addnew(t_room **rooms, int id, int x, int y){
	t_room	*new;

	new = malloc(sizeof(t_room));
	if (!new)
		return ;

	new->id = id;
	new->x = x;
	new->y = y;
	new->visited = false;
	new->links = NULL;
    new->links_size = 0;
    new->next = NULL;
    
    if (*rooms == NULL){
        *rooms = new;
        return ;
    }
    t_room *current = *rooms;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new;
}

t_room	*room_getby_id(t_room *rooms, int id){
    while (rooms != NULL){
        if (rooms->id == id)
            return (rooms);
        rooms = rooms->next;
    }
    return (NULL);
}

int room_islink(t_room *room, int link_id) {
    for (int i = 0; i < room->links_size; i++) {
        if (room->links[i] == link_id)
            return (1);
    }
    return (0);
}

void    room_addlink(t_room *rooms, int room_id, int link_id) {
    t_room *room = room_getby_id(rooms, room_id);
    if (room == NULL)
        return;
    if (room->links == NULL) {
        room->links = malloc(sizeof(int));
        if (room->links == NULL)
            return;
        room->links[0] = link_id;
        room->links_size = 1;
    } else {
        if (room_islink(room, link_id))
            return ;
        int *new_links = malloc(sizeof(int) * (room->links_size + 1));
        if (new_links == NULL)
            return;

        for (int i = 0; i < room->links_size; i++)
            new_links[i] = room->links[i];

        free(room->links);
        room->links = new_links;
        room->links[room->links_size] = link_id;
        room->links_size++;
    }
    room_addlink(rooms, link_id, room_id);
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

// typedef struct data_s{
// 	int		total_ants;
// 	t_room	**rooms;
// 	t_room	*start;
// 	t_room	*end;
// } t_data;
