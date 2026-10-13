#include	"../includes/lem_in_bonus.h"


void	cleanup_visu(t_data_visu *data_visu){
	free_actions_lines(&data_visu->actions_lines);
	free(data_visu->ants);
	data_visu->ants = NULL;
	free_rooms(data_visu->data.rooms);
	data_visu->data.rooms = NULL;
}