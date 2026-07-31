#include	"../includes/lem_in.h"

// t_ant_queue    *queue_addnew(t_ant_queue *queue){
//     queue->queue = NULL;
//     queue->size = 0;
//     queue->next = NULL;
//     return (queue);
// };

#include	"../includes/lem_in.h"

t_ant_path    *path_content_add_back(t_ant_path *path, t_room *room){
    t_list *new_node = ft_lstnew((void *)(t_room*)(room));
    if (!new_node)
        return (NULL);
    
    ft_lstadd_back(&path->queue, new_node);
    path->size++;
    
    return (new_node);
};

int path_addnew(t_ant_path **path){
    t_ant_path *new = malloc(sizeof(t_ant_path));
    if (!new)
        return (-1);

    new->queue = NULL;
    new->size = 0;
    new->parent_path = NULL;
    new->next = NULL;

    if (*path == NULL){
        *path = new;
        return (0);
    }
    t_room *current = *path;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new;

    return (0);
};

//print name et id
void print_ant_path(t_ant_path *path)
{
    t_ant_path *temp_path = path;
    int path_num = 0;

    while (temp_path != NULL){
    
        t_list *temp_node = temp_path->queue;
        ft_printf("Path %d (size %d): ", path_num++, temp_path->size);
    
        while (temp_node != NULL){
            int room_id = (int)(intptr_t)(temp_node->content);
            ft_printf("%d ", room_id);
            temp_node = temp_node->next;
        }
        ft_printf("\n");
        temp_path = temp_path->next;
    };
};


int     bfs(t_data *data){
    t_ant_path *paths = NULL;
    
    if (!path_addnew(&paths))
        return (-1);

    data->ant_path = paths;
    t_room *main_room = room_getby_id(data->rooms, data->start_id);
    if (!main_room)
    return (-1);
    
    paths = path_content_add_back(paths, main_room);

    while (1){
        for (int i = 0; i < main_room->links_size; i++){
            t_room *current_room = room_getby_id(data->rooms, main_room->links[i]);
            
            if(!current_room->visited){
                paths = path_addnew(paths);
                current_room->visited = true;

                if (!path)
            }
            if (current_room->visited) {
                t_ant_path *visited_path = get_visited_path(paths, current_room->id);

                int queue_count = path_nlen(visited_path, current_room->id);
                if (queue_count <= )

            }
        }


    }
    
    while(paths->next != NULL){

    }

    print_ant_path(data->ant_path);

    
    while (1){}
    
};
