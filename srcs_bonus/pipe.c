#include "../includes/lem_in_bonus.h"

typedef struct pipe_vars_s{
	int			numSegments;
	int			nVerts;
	int			pipe_size;
} t_pipe_vars;

int	tunnel_size(t_room *rooms){

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

static void	add_pipe(float *vertices, int *index, float x1, float y1, float x2, float y2, float pipe_width){

	float   vecteurDirecteurX = x2 - x1;
	float   vecteurDirecteurY = y2 - y1;


	float   length = sqrtf(vecteurDirecteurX * vecteurDirecteurX + vecteurDirecteurY * vecteurDirecteurY);
	if (length == 0.0f)
		return ;

	float nx = -vecteurDirecteurY / length;
	float ny = vecteurDirecteurX / length;

	
	printf("nx %f et ny %f\n", nx, ny);
	printf("dx %f, dy %f\n", vecteurDirecteurX, vecteurDirecteurY);

	nx *= pipe_width;
	ny *= pipe_width;

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
	*x = ((float)room->x - data_visu->min_x) / data_visu->width - data_visu->offset;
	*y = ((float)room->y - data_visu->min_y) / data_visu->height - data_visu->offset;
};

static float	*get_pipe_vertices(t_data_visu *data_visu){

	float	*vertices = malloc(sizeof(float) * (data_visu->gl_objects[1].vertices_size));
	if (!vertices)
		return (NULL);

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
				add_pipe(vertices, &verticeIndex, x1, y1, x2, y2, 0.005f);
				ft_printf("room id %d\n", linked_room->id);
			}
		}
		room = room->next;
	}

	for (size_t i = 0; i < data_visu->gl_objects[1].colors_size; i++)
		data_visu->gl_objects[1].colors[i] = 1.0f;
	
	return (vertices);
};

void	set_pipe(t_data_visu *data_visu){

	t_pipe_vars vars = { 1, 4, data_visu->objSize[1]};

	data_visu->gl_objects[1].colors_size = vars.pipe_size * vars.nVerts * 3;
	data_visu->gl_objects[1].colors = malloc(data_visu->gl_objects[1].colors_size * sizeof(float) );
	if(!data_visu->gl_objects[1].colors)
		return;

	data_visu->gl_objects[1].vertices_size = data_visu->objSize[1] * vars.nVerts * 2;
	printf("\n\npipe_size (idem que objSize[1])= %d\nobjSize[1] = %d\nnVerts = %d\ncolors_size = %zu floats\ncolor_count = %zu colors\nvertices_size = %d floats\nvertex_count = %d vertices\n\n\n", vars.pipe_size, data_visu->objSize[1], vars.nVerts, data_visu->gl_objects[1].colors_size, data_visu->gl_objects[1].colors_size / 3, data_visu->gl_objects[1].vertices_size, data_visu->gl_objects[1].vertices_size / 2);
	data_visu->gl_objects[1].vertices = get_pipe_vertices(data_visu);
};
