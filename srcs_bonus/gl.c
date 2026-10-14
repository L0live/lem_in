#include	"../includes/lem_in_bonus.h"

int	gl_init(t_data_visu *data_visu){

	for (size_t i = 0; i < OBJS_SIZE; i++){
		t_gl_object *gl_obj = &data_visu->gl_objects[i];

		set_objects_buffers(gl_obj, i);

		gl_obj->program = createProgram((t_object_type)i);
		if (gl_obj->program == 0){
			cleanup_opengl(gl_obj->vertex_array, gl_obj->vertex_buffer, gl_obj->program);
			return (-1);
		}
	
		set_objects_uniform_location(gl_obj, i);
	};

	glfwSetInputMode(data_visu->window, GLFW_STICKY_KEYS, GL_TRUE);

	return (0);
};

int	init_glfw(t_data_visu *data){
	if (!glfwInit()){
		ft_printf("ERROR: glfwInit failed\n");
		return -1;
	}

	data->window = glfwCreateWindow(800, 600, "Lem-in", NULL, NULL);
	if (!data->window){
		ft_printf("ERROR: glfwCreateWindow failed\n");
		glfwTerminate();
		return (-1);
	}

	glfwMakeContextCurrent(data->window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		ft_printf("ERROR: gladLoadGLLoader failed\n");
		glfwDestroyWindow(data->window);
		glfwTerminate();
		return (-1);
	}
	
	glfwSwapInterval(1);
	return (0);
};

static void    set_values(t_data_visu *data_visu){
	t_room *rooms = data_visu->data.rooms; 

	int max_x = rooms->x;
	int min_x = rooms->x;
	int max_y = rooms->y;
	int min_y = rooms->y;

	rooms = rooms->next;
	while (rooms) {
		if (rooms->x > max_x)
			max_x = rooms->x;
		if (rooms->x < min_x)
			min_x = rooms->x;
		if (rooms->y > max_y)
			max_y = rooms->y;
		if (rooms->y < min_y)
			min_y = rooms->y;
		rooms = rooms->next;
	}

	data_visu->width = (max_x - min_x == 0) ? 1.0f : max_x - min_x;
	data_visu->height = (max_y - min_y == 0) ? 1.0f : max_y - min_y;

	rooms = data_visu->data.rooms;
	while (rooms) {
		rooms->x -= min_x;
		rooms->y -= min_y;
		rooms = rooms->next;
	}
};

void	set_gl_objects(t_data_visu *data_visu){
	data_visu->objSize[ROOM] = count_room(data_visu->data.rooms);
	data_visu->objSize[PIPE] = get_pipe_size(data_visu->data.rooms);
	data_visu->objSize[ANT] = data_visu->data.total_ants;
	data_visu->objSize[TEXT] = data_visu->objSize[ROOM];
	data_visu->gl_objects[ROOM].nbSegment = 42;
	data_visu->gl_objects[PIPE].nbSegment = 4;
    set_values(data_visu);

	t_list		*actionsList;
	t_actions	*actions;
	t_ant		*ant;

	actionsList = data_visu->actions_lines;
	while (actionsList){
		actions = actionsList->content;
		while (actions){
			ant = actions->ant;
			ant->x = ant->x / (float)data_visu->width - BASIC_OFFSET;
			ant->y = ant->y / (float)data_visu->height - BASIC_OFFSET;
			actions = actions->next;
		}
		actionsList = actionsList->next;
	}

	if (set_rooms(data_visu, &data_visu->gl_objects[ROOM], data_visu->objSize[ROOM]) == -1){
		ft_printf("ERROR: set_rooms failed\n");
		return;
	}
	if (set_pipe(data_visu, &data_visu->gl_objects[PIPE], data_visu->objSize[PIPE]) == -1){
		ft_printf("ERROR: set_pipe failed\n");
		return;
	}
	set_font(data_visu);
	set_ants(data_visu);
};

void cleanup_opengl(GLuint vertex_array, GLuint vertex_buffer, GLuint program){
    if (program != 0)
        glDeleteProgram(program);
    if (vertex_buffer != 0)
        glDeleteBuffers(1, &vertex_buffer);
    if (vertex_array != 0)
        glDeleteVertexArrays(1, &vertex_array);
}

void	free_gl_objects(t_gl_object *gl_objects){

    for (size_t i = 0; i < OBJS_SIZE; i++){

 		if (gl_objects[i].program)
            glDeleteProgram(gl_objects[i].program);
        if (gl_objects[i].vertex_buffer)
            glDeleteBuffers(1, &gl_objects[i].vertex_buffer);
        if (gl_objects[i].color_buffer)
            glDeleteBuffers(1, &gl_objects[i].color_buffer);
        if (gl_objects[i].vertex_array)
            glDeleteVertexArrays(1, &gl_objects[i].vertex_array);
        if (gl_objects[i].color_buffer)
            glDeleteVertexArrays(1, &gl_objects[i].color_buffer);

		// //! condition temporaire
        // if (i != ANT)
        //     free(gl_objects[i].vertices);			
			
        free(gl_objects[i].colors);
        free(gl_objects[i].first);
        free(gl_objects[i].count);		
    };
};
