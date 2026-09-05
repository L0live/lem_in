#include "../includes/lem_in_bonus.h"

typedef struct rooms_vars_s{
	int			numSegments;
	int			nVerts;
	int			rooms_size;
	float		radius;
} t_rooms_vars;

#include "stdio.h"

void set_radius(t_data_visu *data_visu, t_room *rooms, t_rooms_vars *vars){
	float	initial_radius = 0.01f;
	float	scale = data_visu->width / sqrtf(count_room(rooms)) ;
	// float	tmp = data_visu->height / sqrtf(count_room(rooms)) ;

	// if(scale > tmp)
		// scale = tmp;
	// scale = (scale + tmp) / 2;
	//! idem pour la height ? et on compare ?
	printf("initial_radius : %f\nscale%f\n", initial_radius, scale);
	vars->radius = initial_radius * scale;
	
	printf("radius : %f\n", vars->radius);
	// éviter un radius trop petit ou trop grand
	if (vars->radius > 0.05f)
		vars->radius = 0.05f;
	if (vars->radius < 0.001f)
		vars->radius = 0.001f;
	printf("after radius : %f\n", vars->radius);
};

static void	add_color(float *colors, int index, float r, float g, float b){
	colors[index * 3 + 0] = r;
	colors[index * 3 + 1] = g;
	colors[index * 3 + 2] = b;
};

void	set_vertices(float *vertices, t_data_visu *data_visu, t_rooms_vars *vars, t_gl_object *obj){

	const int	total_segments = vars->numSegments * 2;
	int			color_index;

	t_room *rooms = data_visu->data.rooms;
	for (int j = 0; rooms; j++) {

		for (int i = 0; i < vars->nVerts; i++) {

			// pour la couleur des rooms
			color_index = (j * vars->numSegments) + i;
			// ft_printf("color index %d\n", color_index);
			if (rooms->id == data_visu->data.start_id)
				add_color(obj->colors, color_index, 0.0f, 0.7f, 0.0f);
			else if (rooms->id == data_visu->data.end_id)
				add_color(obj->colors, color_index, 0.7f, 0.0f, 0.0f);
			else
				add_color(obj->colors, color_index, 0.0f, 0.0f, 0.7f);
			
			float angle = 2.0f * M_PI * (float)i / (float)vars->numSegments;
			int	index = (j * total_segments) + i * 2;

			float point = ((float)rooms->x - data_visu->min_x) / data_visu->width - data_visu->offset + vars->radius * cosf(angle);
			vertices[index] = point;

			point = ((float)rooms->y - data_visu->min_y)/ data_visu->height - data_visu->offset + vars->radius * sinf(angle);
			vertices[index + 1] = point;
		}
		rooms = rooms->next;
	};
};

float    *get_rooms_vertices(t_data_visu *data_visu, t_rooms_vars *vars){
	
	float	*vertices = malloc(data_visu->gl_objects[0].vertices_size * sizeof(float));
	if (!vertices){
		free(data_visu->gl_objects[0].colors);
		return (NULL);
	}
	
	set_radius(data_visu, data_visu->data.rooms, vars);
	set_vertices(vertices, data_visu, vars, &data_visu->gl_objects[0]);
	// for (int i = 0; i < vars.rooms_size * vars.numSegments * 2; i += 2){
		// if (i % vars.numSegments == 0)
			// printf("\n");		
		// printf("gl_room->vertices[%d] %f , gl_room->vertices[%d] %f\n",i, gl_room->vertices[i],i + 1, gl_room->vertices[i + 1]);
	// }

	return (vertices);
};

void	set_rooms(t_data_visu *data_visu){

	t_rooms_vars vars = { data_visu->gl_objects[0].nbSegment, data_visu->gl_objects[0].nbSegment + 1, count_room(data_visu->data.rooms), 0.1f};

	//colors
	data_visu->gl_objects[0].colors_size = vars.rooms_size * vars.nVerts * 3;
	data_visu->gl_objects[0].colors = malloc(data_visu->gl_objects[0].colors_size * sizeof(float) );
	if(!data_visu->gl_objects[0].colors)
		return;

	data_visu->gl_objects[0].vertices_size = vars.rooms_size * vars.nVerts * 2;
	data_visu->gl_objects[0].vertices = get_rooms_vertices(data_visu, &vars);
};