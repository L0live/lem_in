#include	"../includes/lem_in.h"

void	join_paths(t_data *data, t_path *current_path){

	(void) data	;
	(void) current_path	;
	char * name = ((t_room *)current_path->queue->content)->name;
	ft_printf("name current_room : %s\n", name);
	// while (current_path->parent_path->parent_path){
	// 	ft_lstadd_front(&current_path->queue, current_path->parent_path->queue);
	// 	current_path = current_path->parent_path;
	// };

};