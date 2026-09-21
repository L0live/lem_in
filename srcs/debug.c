#include	"../includes/lem_in.h"

void    print_onepath(void *valid){
	t_path	*path = (t_path *)valid;

	ft_printf("\n\nPath:\n");
	ft_printf("	size : %d\n", path->size);
	if(path->parent_path)
	ft_printf("	parent queue : ");
	
	if (path->parent_path) {
		t_list *queue = path->parent_path->queue;
		while (queue != NULL){
			char *room_name = ((t_room *)queue->content)->name;
			ft_printf("%s ", room_name);
			queue = queue->next;
		}
	}
	ft_printf("\n	ants : %d", path->ants);
	ft_printf("\n	queue : ");
			
	t_list	*queue = path->queue;
	while (queue != NULL){
		char *room_name = ((t_room *)queue->content)->name;
		ft_printf("%s ", room_name);
		queue = queue->next;
	}
	ft_printf("\n");
};

//print name et id
void print_paths(t_path *paths)
{
	t_path *path = paths;
	int path_num = 0;

	while (path != NULL){
		ft_printf("\n\nPath %d :\n", path_num++);
		ft_printf("	size : %d\n", path->size);
		ft_printf("	ants : %d\n", path->ants);
		if(path->parent_path)
		ft_printf("	parent queue : ");
		
		if (path->parent_path) {
			t_list *queue = path->parent_path->queue;
			while (queue != NULL){
				char *room_name = ((t_room *)queue->content)->name;
				ft_printf("%s, ", room_name, ((t_room *)queue->content)->id);
				queue = queue->next;
			}
		}
		ft_printf("\n	queue : ");
				
		t_list	*queue = path->queue;
		while (queue != NULL){
			char *room_name = ((t_room *)queue->content)->name;
			ft_printf("%s ", room_name);
			queue = queue->next;
		}
		path = path->next;
	};
	ft_printf("\n\n");
};
