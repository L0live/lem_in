#include "../includes/lem_in_bonus.h"

typedef struct rooms_vars_s{
	int	numSegments;
	int	nVerts;
	int	rooms_size;
	float		radius;
	float		min_x, max_x, min_y, max_y;
} t_rooms_vars;

// typedef struct rooms_vars_s{

	// const int	numSegments = 42;

	// const int	nVerts = numSegments + 1;

	// const int	rooms_size = count_room(data_visu->data.rooms);

	// float		radius = 0.1f;

	// float		min_x, max_x, min_y, max_y;

// } t_rooms_vars;


void    set_vars(t_data_visu *data_visu, t_rooms_vars *vars){
	t_room *rooms = data_visu->data.rooms; 

	vars->min_x = rooms->x;
	vars->min_y = rooms->y;
	vars->max_x = rooms->x;
	vars->max_y = rooms->y;

	rooms = rooms->next;
	while (rooms) {
		if (rooms->x > vars->max_x)
			vars->max_x = rooms->x;
		if (rooms->x < vars->min_x)
			vars->min_x = rooms->x;
		if (rooms->y > vars->max_y)
			vars->max_y = rooms->y;
		if (rooms->y < vars->min_y)
			vars->min_y = rooms->y;
		rooms = rooms->next;
	};
	vars->max_x -= data_visu->offset + vars->min_x;
	vars->max_y -= data_visu->offset + vars->min_y;

	if (vars->max_x > 22)
		vars->radius /= vars->max_x / 22; 
	else if (vars->max_y > 6)
		vars->radius /= vars->max_y / 6; 
	// printf("max_x %f\nmax_y %f\nmin_c %f\nmin_y %f\n", max_x,max_y, min_x, min_y);
	// printf("Total rooms %d\n", rooms_size);
};


void	set_vertices(float *vertices, t_room *rooms, t_rooms_vars *vars, float offset){
	const int total_segments = vars->numSegments * 2; 
	for (int j = 0; rooms; j++) {
		for (int i = 0; i < vars->numSegments; i++) {
			float angle = 2.0f * M_PI * (float)i / (float)vars->numSegments;
			int	index = (j * total_segments) + i * 2;

			// X
			float point = ((float)rooms->x - vars->min_x)/vars->max_x - offset + vars->radius * cosf(angle);
			vertices[index] = point;

			// Y
			point = ((float)rooms->y - vars->min_y)/ vars->max_y - offset + vars->radius * sinf(angle);
			vertices[index + 1] = point;
		}
		rooms = rooms->next;
	};
};

float    *get_rooms_vertices(t_data_visu *data_visu){
	
	t_rooms_vars vars = { 42, 42 + 1, count_room(data_visu->data.rooms), 0.1f, 0.0f, 0.0f, 0.0f, 0.f};
	// float		min_x, max_x, min_y, max_y;};	
	
	set_vars(data_visu, &vars);
	
	float	*vertices = malloc(vars.rooms_size * (vars.nVerts * 2 * sizeof(float)));
	if (!vertices)
		return (NULL);

	set_vars(data_visu, &vars);
	set_vertices(vertices, data_visu->data.rooms, &vars, data_visu->offset);

	// for (int i = 0; i < vars.rooms_size * vars.numSegments * 2; i += 2){
		// if (i % vars.numSegments == 0)
			// printf("\n");		
		// printf("gl_room->vertices[%d] %f , gl_room->vertices[%d] %f\n",i, gl_room->vertices[i],i + 1, gl_room->vertices[i + 1]);
	// }

	return (vertices);
};