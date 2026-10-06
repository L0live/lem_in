#include "../includes/lem_in_bonus.h"


static t_ant *init_ants(t_data_visu *data_visu){
	t_ant   *ants;
	t_room  *start;
	int     total_ants = data_visu->data.total_ants;

	ants = malloc(sizeof(t_ant) * total_ants);
	if (!ants)
		return (NULL);    

	start = room_getby_id(data_visu->data.rooms, data_visu->data.start_id);
	for (int i = 1; i <= total_ants; i++){
		ants[i - 1].id = i;
		ants[i - 1].x = start->x;
		ants[i - 1].y = start->y;
		ants[i - 1].current_room = start;
	}
	
	data_visu->ants = ants;
	return (ants);
}

void	proceed_to_actions(t_list **stdin_content){
	while (*stdin_content && ((char*)(*stdin_content)->content)[0]){
		*stdin_content = (*stdin_content)->next;
	}
	if (*stdin_content)
		*stdin_content = (*stdin_content)->next;
}

void	action_addback(t_actions **lst, t_actions *new)
{
	t_actions	*tmp;

	if (!new)
		return ;
	if (!*lst) {
		*lst = new;
		return ;
	}
	tmp = *lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	return ;
}

static int    ft_isstriter(const char *str, int (*f)(int)){
	while (*str){
		if (!f(*str))
			return (0);
		str++;
	}
	return (-1);
}

static void	free_split(char **split) {
	if (split == NULL)
		return ;
	for (int i = 0; split[i] != NULL; i++)
		free(split[i]);
	free(split);
}

int	fill_action(t_data_visu *data_visu, t_list *action, char *action_line){
	char **elements;

	if (action_line[0] != 'L')
		return (-1);

	elements = ft_split(&action_line[1], '-');
	if (!elements)
		return (-1);

	if (!elements[0] || !ft_isstriter(elements[0], &ft_isdigit))
		return (-1);
	if (!elements[1] || !room_getby_name(data_visu->data.rooms, elements[1]))
		return (-1);
	
	int id = ft_atoi(elements[0]) - 1;
	if (id < 0 || id >= data_visu->data.total_ants)
		return (-1);

	t_actions *new = malloc(sizeof(t_actions));
	if (!new)
		return (-1);
	
	new->ant = &data_visu->ants[id];
	new->room = room_getby_name(data_visu->data.rooms, elements[1]);
	new->next = NULL;

	action_addback(((t_actions**)&action->content), new);
	free_split(elements);
	return (0);
}

int	pars_actions(t_list *stdin_content, t_data_visu *data_visu){
	
	char    *line;
	char	**actions;
	// char    *line;
	t_ant   *ants;
	
	ants = init_ants(data_visu);
	if (!ants)
		return (-1);    

	proceed_to_actions(&stdin_content);

	while (stdin_content){
		line = stdin_content->content;
		// if (line)
			// return (-1);

		actions = ft_split(line, ' ');
		if (!actions)
			return (-1);

		t_list *action_line = ft_lstnew(NULL);
		if (!action_line)
			return (-1);
		for (int i = 0; actions[i]; i++){
			if (fill_action(data_visu, action_line, actions[i]) == -1)
				return (-1);
		}
		free_split(actions);
		ft_lstadd_back(&data_visu->actions_lines, action_line);
		stdin_content = stdin_content->next;
	}

	return (0);    
}
