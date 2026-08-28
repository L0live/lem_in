#include "../includes/lem_in_bonus.h"

typedef struct rooms_vars_s{
	int			numSegments;
	int			nVerts;
	int			rooms_size;
	float		radius;
	float		min_x, max_x, min_y, max_y;
	float		width;
	float		height;
} t_rooms_vars;

#include "stdio.h"
void set_radius(t_room *rooms, t_rooms_vars *vars){
	float	initial_radius = 0.01f;
	float	scale = vars->width / sqrtf(count_room(rooms)) ;
	//! idem pour la height ? et on compare ?
	printf("initial_radius : %f\nscale%f\n", initial_radius, scale);
	vars->radius = initial_radius * scale;
	
	printf("radius : %f\n", vars->radius);
	// éviter un radius trop petit ou trop grand
	if (vars->radius > 0.05f)
		vars->radius = 0.05f;
	if (vars->radius < 0.01f)
		vars->radius = 0.01f;
	printf("after radius : %f\n", vars->radius);
};

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
	}

	// avant
	// vars->width = (vars->max_x - vars->min_x);
	// if((vars->max_x - vars->min_x) == 0)
	// 	vars->height = 5;
	// vars->height = (vars->max_y - vars->min_y);
	// if((vars->max_y - vars->min_y) == 0)
	// 	vars->height = 5;
	// apres
	vars->width = vars->max_x - vars->min_x;
	if(vars->width == 0)
		vars->width = 5;
	vars->height = vars->max_y - vars->min_y;
	if(vars->height == 0)
		vars->height = 5;
};


void	set_vertices(float *vertices, t_room *rooms, t_rooms_vars *vars, float offset){

	const int total_segments = vars->numSegments * 2;

	for (int j = 0; rooms; j++) {
		for (int i = 0; i < vars->nVerts; i++) {
			float angle = 2.0f * M_PI * (float)i / (float)vars->numSegments;
			int	index = (j * total_segments) + i * 2;

			float point = ((float)rooms->x - vars->min_x) / vars->width - offset + vars->radius * cosf(angle);
			vertices[index] = point;

			point = ((float)rooms->y - vars->min_y)/ vars->height - offset + vars->radius * sinf(angle);
			vertices[index + 1] = point;
		}
		rooms = rooms->next;
	};
};

float    *get_rooms_vertices(t_data_visu *data_visu, t_rooms_vars *vars){
	
	float	*vertices = malloc(data_visu->gl_objects[0].vertices_size * sizeof(float));
	if (!vertices)
		return (NULL);
	
	set_radius(data_visu->data.rooms, vars);
	set_vertices(vertices, data_visu->data.rooms, vars, data_visu->offset);

	// for (int i = 0; i < vars.rooms_size * vars.numSegments * 2; i += 2){
		// if (i % vars.numSegments == 0)
			// printf("\n");		
		// printf("gl_room->vertices[%d] %f , gl_room->vertices[%d] %f\n",i, gl_room->vertices[i],i + 1, gl_room->vertices[i + 1]);
	// }

	return (vertices);
};

void	set_rooms(t_data_visu *data_visu){

	data_visu->offset = 0.5f;
	data_visu->gl_objects[0].nbSegment = 42;
	t_rooms_vars vars = { data_visu->gl_objects[0].nbSegment, data_visu->gl_objects[0].nbSegment + 1, count_room(data_visu->data.rooms), 0.1f, 0.0f, 0.0f, 0.0f, 0.f, 0.0f, 0.f};

	if (vars.rooms_size <= 0)
		return;
        // return (NULL);

	set_vars(data_visu, &vars);

	data_visu->gl_objects[0].vertices_size = vars.rooms_size * vars.nVerts * 2;
	data_visu->gl_objects[0].vertices = get_rooms_vertices(data_visu, &vars);
};