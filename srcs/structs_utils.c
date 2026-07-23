#include "lem_in.h"

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

	new->nbr = nbr;
	new->x = x;
	new->y = y;
	new->is_full = false;
	new->links = NULL;
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
        if (rooms->nbr == nbr)
            return (rooms);
        rooms = rooms->next;
    }
    return (NULL);
}

void    room_addlink(t_room *rooms, int room_nbr, int link_nbr) {
    t_room *room = room_getby_nbr(rooms, room_nbr);
    if (room == NULL) {
        return;
    
    if (room->links == NULL) {
        room->links = malloc(sizeof(int));
        if (room->links == NULL) {
            return;
        }
        room->links[0] = link_nbr;
    } else {
        int links_count = get_links_count(room);
        int *new_links = malloc(sizeof(int) * (links_count + 1));
        if (new_links == NULL) {
            return;
        }
        for (int i = 0; i < links_count; i++) {
            new_links[i] = room->links[i];
        }
        free(room->links);
        room->links = new_links;
        room->links[links_count] = link_nbr;
    }
}

// typedef struct data_s{
// 	int		total_ants;
// 	t_room	**rooms;
// 	t_room	*start;
// 	t_room	*end;
// } t_data;
