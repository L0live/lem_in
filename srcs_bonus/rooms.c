#include "../includes/lem_in_bonus.h"

#include "stdio.h"

float	get_radius(t_data_visu *data_visu, int rooms_size){
	float	radius = BASIC_RADIUS;
	float	scale = data_visu->width / sqrtf(rooms_size);
	// float	tmp = data_visu->height / sqrtf(rooms_size);

	// if(scale > tmp)
		// scale = tmp;
	// scale = (scale + tmp) / 2;
	//! idem pour la height ? et on compare ?

	printf("initial radius : %f\nscale%f\n", radius, scale);
	radius *= scale;
	printf("radius : %f\n", radius);
	
	// éviter un radius trop petit ou trop grand
	if (radius > 0.05f)
		radius = 0.05f;
	if (radius < 0.001f)
		radius = 0.001f;
	printf("after radius : %f\n", radius);
	return (radius);
};

static void	add_color(float *colors, int index, float r, float g, float b){
	colors[index * 3 + 0] = r;
	colors[index * 3 + 1] = g;
	colors[index * 3 + 2] = b;
};

static void	set_vertices_and_colors(t_data_visu *data_visu, t_gl_object *obj, int obj_size){
	const int	nVerts = obj->nbSegment + 1;
	const int	total_segments = obj->nbSegment * 2;
	const float	radius = get_radius(data_visu, obj_size);

	printf("radius: %f\n", radius);

	t_room *rooms = data_visu->data.rooms;
	for (int j = 0; rooms; j++) {

		for (int i = 0; i < nVerts; i++) {

			// pour la couleur des rooms
			int color_index = (j * obj->nbSegment) + i;
			// ft_printf("color index %d\n", color_index);
			if (rooms->id == data_visu->data.start_id)
				add_color(obj->colors, color_index, 0.0f, 0.7f, 0.0f);
			else if (rooms->id == data_visu->data.end_id)
				add_color(obj->colors, color_index, 0.7f, 0.0f, 0.0f);
			else
				add_color(obj->colors, color_index, 0.0f, 0.0f, 0.7f);
			
			float angle = 2.0f * M_PI * (float)i / (float)obj->nbSegment;
			int	index = (j * total_segments) + i * 2;

			float point = (float)rooms->x / data_visu->width - BASIC_OFFSET + radius * cosf(angle);
			obj->vertices[index] = point;
			printf("Room %d (x): %f\n", index, point);

			point = (float)rooms->y/ data_visu->height - BASIC_OFFSET + radius * sinf(angle);
			obj->vertices[index + 1] = point;
			printf("Room %d (y): %f\n", index, point);
		}
		rooms = rooms->next;
	};
};

int	set_rooms(t_data_visu *data_visu, t_gl_object *obj, int obj_size){
	//vertices
	obj->vertices_size = obj_size * (obj->nbSegment + 1) * 2;
	obj->vertices = malloc(obj->vertices_size * sizeof(float));
	if(!obj->vertices)
		return (-1);
	//colors
	obj->colors_size = obj_size * (obj->nbSegment + 1) * 3;
	obj->colors = malloc(obj->colors_size * sizeof(float) );
	if (!obj->colors){
		free(obj->vertices);
		return (-1);
	}

	set_vertices_and_colors(data_visu, obj, obj_size);
	return (0);
};