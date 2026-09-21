#include	"../includes/lem_in.h"

void	free_one_path_content(void *content){
	t_path *path = (t_path *)content;

	if (!path)
		return;

	//! on free QUE le node de la liste interne, pas le t_room (content)
	ft_lstclear(&path->queue, NULL);
	free(path);
}

void	free_data(t_data *data){
	if (!data)
		return ;
	ft_lstclear(&data->all_paths, free_one_path_content);
	ft_lstclear(&data->valid_paths, free_one_path_content);
	free_rooms(data->rooms);
	data->rooms = NULL;
}