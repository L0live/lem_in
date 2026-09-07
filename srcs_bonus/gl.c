#include	"../includes/lem_in_bonus.h"

int	gl_init(t_data_visu *data_visu){

	for (size_t i = 0; i < OBJS_SIZE; i++){
		t_gl_object *gl_obj = &data_visu->gl_objects[i];

		glGenVertexArrays(1, &gl_obj->vertex_array);
		glBindVertexArray(gl_obj->vertex_array);
		
		glGenBuffers(1, &gl_obj->vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, gl_obj->vertex_buffer);

		if(i == ANT){
			// glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			glBufferData(GL_ARRAY_BUFFER, gl_obj->vertices_size * sizeof(float) , gl_obj->vertices, GL_STATIC_DRAW);
			
			//aPos en vec2
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);

			//aColor ne vec3
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float)));


			//aTexCoord en vec2
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5 * sizeof(float)));
		}
		else if (i == TEXT){
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *)0);
		}
		else{
			glBufferData(GL_ARRAY_BUFFER, gl_obj->vertices_size * sizeof(float) , gl_obj->vertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
	    	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) 0);

			glGenBuffers(1, &gl_obj->color_buffer);
			glBindBuffer(GL_ARRAY_BUFFER, gl_obj->color_buffer);

			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * gl_obj->colors_size, gl_obj->colors, GL_STATIC_DRAW);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		gl_obj->program = createProgram((t_object_type)i);
		if (gl_obj->program == 0){
			cleanup_opengl(gl_obj->vertex_array, gl_obj->vertex_buffer, gl_obj->program);
			return (-1);
		}

		if (i == TEXT){
			// gl_obj->mvp_location = glGetUniformLocation(gl_obj->program, "MVP");
            gl_obj->texture_location = glGetUniformLocation(gl_obj->program, "text");
            gl_obj->text_color_location = glGetUniformLocation(gl_obj->program, "textColor");
        }
		else if (i == ANT){
			// activer la transparence
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			gl_obj->mvp_location = glGetUniformLocation(gl_obj->program, "MVP");
            gl_obj->texture_location = glGetUniformLocation(gl_obj->program, "antTexture");
			glUseProgram(gl_obj->program);
    		glUniform1i(gl_obj->texture_location, 0);
		}
		else{
			gl_obj->mvp_location = glGetUniformLocation(gl_obj->program, "MVP");
			gl_obj->color_location = glGetUniformLocation(gl_obj->program, "objectColor");
		}
	};

	glfwSetInputMode(data_visu->window, GLFW_STICKY_KEYS, GL_TRUE);

	return (0);
};

int	init_glfw(t_data_visu *data){
	if (!glfwInit())
	{
		ft_printf("Error: glfwInit failed\n");
		return -1;
	}

	data->window = glfwCreateWindow(800, 600, "Lem-in", NULL, NULL);
	if (!data->window)
	{
		ft_printf("Error: glfwCreateWindow failed\n");
		glfwTerminate();
		return (-1);
	}

	glfwMakeContextCurrent(data->window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		ft_printf("Error: gladLoadGLLoader failed\n");
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

	data_visu->width = max_x - min_x || 1.0f;
	data_visu->height = max_y - min_y || 1.0f;

	rooms = data_visu->data.rooms;
	while (rooms) {
		rooms->x -= min_x;
		rooms->y -= min_y;
		rooms = rooms->next;
	}
};

void	set_gl_objects(t_data_visu *data_visu){
	data_visu->objSize[0] = count_room(data_visu->data.rooms);
	data_visu->objSize[1] = get_pipe_size(data_visu->data.rooms);
	data_visu->objSize[2] = data_visu->data.total_ants;
	data_visu->objSize[3] = data_visu->objSize[0];

	data_visu->gl_objects[0].nbSegment = 42;
	data_visu->gl_objects[1].nbSegment = 4;

    set_values(data_visu);

	if (set_rooms(data_visu, &data_visu->gl_objects[0], data_visu->objSize[0]) == -1){
		ft_printf("Error: set_rooms failed\n");
		return;
	}
	if (set_pipe(data_visu, &data_visu->gl_objects[1], data_visu->objSize[1]) == -1){
		ft_printf("Error: set_pipe failed\n");
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
        if (gl_objects[i].vertices)
            free(gl_objects[i].vertices);
        if (gl_objects[i].colors)
            free(gl_objects[i].colors);
        if (gl_objects[i].first)
            free(gl_objects[i].first);
        if (gl_objects[i].count)
            free(gl_objects[i].count);                        
    };
};
