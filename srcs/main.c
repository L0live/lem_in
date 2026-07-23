#include	"lem_in.h"

void	ft_lstprint(t_list *lst){
	while (lst){
		ft_printf("%s", lst->content);
		lst = lst->next;
	}
	return ;
};

void	read_stdin(t_list **stdin_content){
	char *line;

	line = get_next_line(STDIN_FILENO);
	while (line != NULL){
		ft_lstadd_back(stdin_content, ft_lstnew(line));
		line = get_next_line(STDIN_FILENO);
	}
};

int main(void){
	
	t_list	*stdin_content = NULL;
	
	read_stdin(&stdin_content);
	ft_lstprint(stdin_content);

	ft_lstclear(&stdin_content, &free);
	return (0);
};