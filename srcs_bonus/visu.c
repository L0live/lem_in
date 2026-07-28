#include	"../includes/lem_in.h"

void	normalize_rooms(t_room *rooms) {
	if (rooms == NULL)
		return ;

	int min_x = rooms->x;
	int min_y = rooms->y;

	t_room *current = rooms;
	while (current != NULL) {
		if (current->x < min_x)
			min_x = current->x;
		if (current->y < min_y)
			min_y = current->y;
		current = current->next;
	}

	current = rooms;
	while (current != NULL) {
		current->x -= min_x;
		current->y -= min_y;
		current = current->next;
	}
};

void    get_wd_matrix(t_room *rooms, int *width, int *height) {
	// int min_x, max_x, min_y, max_y;
	int max_x, max_y;
	if (rooms == NULL)
		return ;

	// min_x = rooms->x;
	max_x = rooms->x;
	// min_y = rooms->y;
	max_y = rooms->y;

	while (rooms != NULL) {
		// if (rooms->x < min_x)
			// min_x = rooms->x;
		if (rooms->x > max_x)
			max_x = rooms->x;
		// if (rooms->y < min_y)
			// min_y = rooms->y;
		if (rooms->y > max_y)
			max_y = rooms->y;

		rooms = rooms->next;
	}
	*width = (max_x + 1) * 3 + 1;
	*height = (max_y + 1) * 3 + 1;
}

void    free_matrix(char **matrix) {
	if (matrix == NULL)
		return ;
	for (int i = 0; matrix[i] != NULL; i++)
		free(matrix[i]);
	free(matrix);
}

void    print_matrix(char **matrix) {
	ft_printf("\n\n##### Matrix Visualisation #####\n");
	for (int i = 0; matrix[i] != NULL; i++) {
		ft_printf("%s\n", matrix[i]);
	}
}

char    **create_matrix(t_room *rooms) {
	int width = 0;
	int height = 0;
	get_wd_matrix(rooms, &width, &height);
	char **matrix = malloc((height + 1) * sizeof(char *));
	if (matrix == NULL)
		return NULL;
	
	for (int i = 0; i < height; i++) {
		matrix[i] = malloc((width + 1) * sizeof(char));
		if (matrix[i] == NULL) {
			free_matrix(matrix);
			return NULL;
		}
		for (int j = 0; j < width; j++)
			matrix[i][j] = ' ';
		matrix[i][width] = '\0';
	}
	matrix[height] = NULL;

	while (rooms != NULL) {
		int		row = (rooms->y + 1) * 3;
		int		col = (rooms->x + 1) * 3;
		char	symbol = rooms->id == START_ID ? '$' : (rooms->id == END_ID ? 'X' : rooms->id + '0');

		if (row >= 0 && row < height && col >= 0 && col < width)
			matrix[row][col] = symbol;
		rooms = rooms->next;
	}
	return matrix;
}