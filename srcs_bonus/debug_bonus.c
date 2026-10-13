#include	"../includes/lem_in_bonus.h"

void	print_action_list(t_list *actions_list){

	t_actions	*actions;

	for (size_t i = 0; actions_list; i++){
		actions = actions_list->content;

		if (actions){
			ft_printf("Action numéro %d\n", i);
			while (actions){
				ft_printf("L%d-%s ", actions->ant->id, actions->room->name);
				actions = actions->next;
			}
			ft_printf("\n");
		}
		actions_list = actions_list->next;
	}
}