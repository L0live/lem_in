#include "../includes/lem_in.h"

// typedef struct room_s{
// 	int				nbr;
// 	int				x, y;
// 	bool			is_full;
// 	struct room_s	**links;
// 	struct room_s	*next;
// } t_room;

void	room_addnew(t_room **rooms, int nbr, int x, int y){
	t_room	*new;

	new = malloc(sizeof(t_room));
	if (!new)
		return ;

	new->id = nbr;
	new->x = x;
	new->y = y;
	new->is_full = false;
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

t_room	*room_getby_nbr(t_room *rooms, int nbr){
    while (rooms != NULL){
        if (rooms->id == nbr)
            return (rooms);
        rooms = rooms->next;
    }
    return (NULL);
}

int room_islink(t_room *room, int link_nbr) {
    for (int i = 0; i < room->links_size; i++) {
        if (room->links[i] == link_nbr)
            return (1);
    }
    return (0);
}

void    room_addlink(t_room *rooms, int room_nbr, int link_nbr) {
    t_room *room = room_getby_nbr(rooms, room_nbr);
    if (room == NULL)
        return;
    if (room->links == NULL) {
        room->links = malloc(sizeof(int));
        if (room->links == NULL)
            return;
        room->links[0] = link_nbr;
        room->links_size = 1;
    } else {
        if (room_islink(room, link_nbr))
            return ;
        int *new_links = malloc(sizeof(int) * (room->links_size + 1));
        if (new_links == NULL)
            return;

        for (int i = 0; i < room->links_size; i++)
            new_links[i] = room->links[i];

        free(room->links);
        room->links = new_links;
        room->links[room->links_size] = link_nbr;
        room->links_size++;
    }
    room_addlink(rooms, link_nbr, room_nbr);
}

void    free_rooms(t_room *rooms) {
    t_room *current = rooms;
    while (current != NULL) {
        t_room *next = current->next;
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
