#include	"lem_in.h"

void	read_stdin(t_list **stdin_content){
	
	char *line = get_next_line(STDIN_FILENO);
	while (line != NULL){
		ft_lstadd_back(stdin_content, ft_lstnew(line));
		line = get_next_line(STDIN_FILENO);
	}
};

int main(void){
	
	t_list	*stdin_content;
	
	read_stdin(&stdin_content);
	ft_lstprint(stdin_content);

	ft_lstclear(&stdin_content, &free);
	return (0);
};