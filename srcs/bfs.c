#include	"../includes/lem_in.h"

t_ant_queue    *queue_addnew(t_ant_queue *queue){
    queue->queue = NULL;
    queue->move_count = 0;
    queue->next = NULL;
    return (queue);
};

int     bfs(t_data *data){
    t_ant_queue *queue = malloc(sizeof(t_ant_queue));
    if (!queue)
        return (1);

    queue = add_new(queue);

    
    while (1){}
    
};
