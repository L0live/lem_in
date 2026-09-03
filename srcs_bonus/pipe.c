#include "../includes/lem_in_bonus.h"

typedef struct pipe_vars_s{
	int			numSegments;
	int			nVerts;
	int			pipe_size;
	float		radius;
	float		min_x, max_x, min_y, max_y;
	float		width;
	float		height;    
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

void    set_varss(t_data_visu *data_visu, t_pipe_vars *vars){
	t_room *rooms = data_visu->data.rooms; 

	vars->min_x = rooms->x;
	vars->min_y = rooms->y;
	vars->max_x = rooms->x;
	vars->max_y = rooms->y;

	rooms = rooms->next;
	while (rooms){
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

	vars->width = (vars->max_x - vars->min_x);
	vars->height = (vars->max_y - vars->min_y);
	
	if (vars->width == 0.0f)
		vars->width = 5.0f;
	if (vars->height == 0.0f)
		vars->height = 5.0f;
};

void	reset(float* x1, float* y1, float* x2, float* y2){

 	double	dx = *x2 - *x1;
    double	dy = *y2 - *y1;
   	float	length = sqrtf(dx * dx + dy * dy);
	if (length == 0.0f)
		return; 

	float	nx = -dy / length;
    float	ny = dx / length;
    
    float ax = *x1 + tests * nx;
	float ay =  *y1 + tests * ny;
    float bx = *x2 + tests * nx;
	float by = *y2 + tests * ny;
	
	*x1 = ax;
	*y1 = ay;

	*x2 = bx;
	*y2 = by;
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

	
	// if (vecteurDirecteurX != 0)
	// 	x1 += (vecteurDirecteurX > 0 ? (vecteurDirecteurY < 0 ? nx : -nx) : (vecteurDirecteurY < 0 ? -nx : nx)) * tests;
	// if (vecteurDirecteurY != 0)
	// 	y1 += (vecteurDirecteurY > 0 ? (vecteurDirecteurX < 0 ? -ny : ny) : (vecteurDirecteurY < 0 ? ny : -ny)) * tests;
	
	// if (vecteurDirecteurX != 0)
	// 	x2 += (vecteurDirecteurX > 0 ? (vecteurDirecteurY < 0 ? -nx : nx) : (vecteurDirecteurY < 0 ? nx : -nx)) * tests;
	// if (vecteurDirecteurY != 0)
	// 	y2 += (vecteurDirecteurY > 0 ? (vecteurDirecteurX < 0 ? ny : -ny) : (vecteurDirecteurY < 0 ? -ny : ny)) * tests;

	vertices[(*index)++] = x1 + nx * pipe_width;
	vertices[(*index)++] = y1 + ny * pipe_width;

	vertices[(*index)++] = x1 - nx * pipe_width;
	vertices[(*index)++] = y1 - ny * pipe_width;

	vertices[(*index)++] = x2 - nx * pipe_width;
	vertices[(*index)++] = y2 - ny * pipe_width;

	vertices[(*index)++] = x2 + nx * pipe_width;
	vertices[(*index)++] = y2 + ny * pipe_width;
};

static void	room_to_gl(t_room *room, t_pipe_vars *vars, float offset, float *x, float *y){
	*x = ((float)room->x - vars->min_x) / vars->width - offset;
	*y = ((float)room->y - vars->min_y) / vars->height - offset;
};

// static void	add_colors(float *colors, int index, float r, float g, float b){
	// colors[index] = 1.0f;
	// colors[index * 3 + 0] = r;
	// colors[index * 3 + 1] = g;
	// colors[index * 3 + 2] = b;
// };

static float	*get_pipe_vertices(t_data_visu *data_visu, t_pipe_vars *vars){

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
				room_to_gl(room, vars, data_visu->offset,&x1, &y1);
				room_to_gl(linked_room, vars, data_visu->offset,&x2, &y2);
				add_pipe(vertices, &verticeIndex, x1, y1, x2, y2, 0.005f);
				ft_printf("room id %d\n", linked_room->id);
			}
		}
		room = room->next;
	}

	// for (size_t i = 0; i < data_visu->gl_objects[1].colors_size / 3; i++)
		// add_colors(data_visu->gl_objects[1].colors, i, 1.0f, 1.0f, 1.0f);
	for (size_t i = 0; i < data_visu->gl_objects[1].colors_size; i++)
		data_visu->gl_objects[1].colors[i] = 1.0f;	
	// add_colors(data_visu->gl_objects[1].colors, i, 1.0f, 1.0f, 1.0f);
	
	return (vertices);
};

void	set_pipe(t_data_visu *data_visu){

	t_pipe_vars vars = { 1, 4, data_visu->objSize[1], 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

	data_visu->gl_objects[1].colors_size = vars.pipe_size * vars.nVerts * 3;
	data_visu->gl_objects[1].colors = malloc(data_visu->gl_objects[1].colors_size * sizeof(float) );
	if(!data_visu->gl_objects[1].colors)
		return;

	set_varss(data_visu, &vars);
	data_visu->gl_objects[1].vertices_size = data_visu->objSize[1] * vars.nVerts * 2;
	printf("\n\npipe_size (idem que objSize[1])= %d\nobjSize[1] = %d\nnVerts = %d\ncolors_size = %zu floats\ncolor_count = %zu colors\nvertices_size = %d floats\nvertex_count = %d vertices\n\n\n", vars.pipe_size, data_visu->objSize[1], vars.nVerts, data_visu->gl_objects[1].colors_size, data_visu->gl_objects[1].colors_size / 3, data_visu->gl_objects[1].vertices_size, data_visu->gl_objects[1].vertices_size / 2);
	data_visu->gl_objects[1].vertices = get_pipe_vertices(data_visu, &vars);
};
