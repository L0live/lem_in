#include "../includes/lem_in_bonus.h"

int	get_pipe_size(t_room *rooms){
	int count = 0;

	while (rooms){
		for (int i = 0; i < rooms->links_size; i++){
			if (rooms->id < rooms->links[i])
				count++;
		}
		rooms = rooms->next;
	}
	return (count);
};

static void	add_pipe(float *vertices, int *index, float x1, float y1, float x2, float y2){
	const float   vecteurDirecteurX = x2 - x1;
	const float   vecteurDirecteurY = y2 - y1;
	// printf("dx %f, dy %f\n", vecteurDirecteurX, vecteurDirecteurY);

	float   length = sqrtf(vecteurDirecteurX * vecteurDirecteurX + vecteurDirecteurY * vecteurDirecteurY);
	if (length == 0.0f)
		return ;

	float nx = -vecteurDirecteurY / length * PIPE_WIDTH;
	float ny = vecteurDirecteurX / length * PIPE_WIDTH;
	// printf("nx %f et ny %f\n", nx, ny);

	vertices[(*index)++] = x1 + nx;
	vertices[(*index)++] = y1 + ny;

	vertices[(*index)++] = x1 - nx;
	vertices[(*index)++] = y1 - ny;

	vertices[(*index)++] = x2 - nx;
	vertices[(*index)++] = y2 - ny;

	vertices[(*index)++] = x2 + nx;
	vertices[(*index)++] = y2 + ny;
};

static void	room_to_gl(t_room *room, t_data_visu *data_visu, float *x, float *y){
	*x = (float)room->x / data_visu->width - BASIC_OFFSET;
	*y = (float)room->y / data_visu->height - BASIC_OFFSET;
};

static void	set_vertices_and_colors(t_data_visu *data_visu, t_gl_object *obj){
	t_room	*room = data_visu->data.rooms;
	int		verticeIndex = 0;

	while(room){
		for (int i = 0; i < room->links_size; i++){
			if (room->id < room->links[i]){
				t_room	*linked_room = room_getby_id(data_visu->data.rooms, room->links[i]);
				if (!linked_room)
					continue;
				
				float	x1, y1, x2, y2;
				room_to_gl(room, data_visu, &x1, &y1);
				room_to_gl(linked_room, data_visu, &x2, &y2);
				add_pipe(obj->vertices, &verticeIndex, x1, y1, x2, y2);
				ft_printf("room id %d\n", linked_room->id);
			}
		}
		room = room->next;
	}

	for (size_t i = 0; i < obj->colors_size; i++)
		obj->colors[i] = 1.0f; // Blanc partout
};

int	set_pipe(t_data_visu *data_visu, t_gl_object *obj, int obj_size){
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

	set_vertices_and_colors(data_visu, obj);
	return (0);
};
