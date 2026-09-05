#include	"../includes/lem_in_bonus.h"


// void    set_vars(t_data_visu *data_visu, t_rooms_vars *vars){
// 	t_room *rooms = data_visu->data.rooms; 

// 	vars->min_x = rooms->x;
// 	vars->min_y = rooms->y;
// 	vars->max_x = rooms->x;
// 	vars->max_y = rooms->y;

// 	rooms = rooms->next;
// 	while (rooms) {
// 		if (rooms->x > vars->max_x)
// 			vars->max_x = rooms->x;
// 		if (rooms->x < vars->min_x)
// 			vars->min_x = rooms->x;
// 		if (rooms->y > vars->max_y)
// 			vars->max_y = rooms->y;
// 		if (rooms->y < vars->min_y)
// 			vars->min_y = rooms->y;
// 		rooms = rooms->next;
// 	}

// 	data_visu->min_x = vars->min_x;
// 	data_visu->max_x = vars->max_x;
// 	data_visu->min_y = vars->min_y;
// 	data_visu->max_y = vars->max_y;


// 	vars->width = vars->max_x - vars->min_x;
// 	if(vars->width == 0.0f)
// 		vars->width = 1.0f;
// 	vars->height = vars->max_y - vars->min_y;
// 	if(vars->height == 0.0f)
// 		vars->height = 1.0f;
	
// 	data_visu->width = vars->width;
// 	data_visu->height = vars->height;
// }

GLfloat		vertices[] = {
	// positions          // colors           // texture coords
	0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
	0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
};

int	gl_init(t_data_visu *data_visu){

	for (size_t i = 0; i < OBJS_SIZE; i++){
		t_gl_object *gl_obj = &data_visu->gl_objects[i];
		//! on verifie si obj.vertices et obj.colors existe ??

		glGenVertexArrays(1, &gl_obj->vertex_array);
		glGenBuffers(1, &gl_obj->vertex_buffer);
		
		glBindVertexArray(gl_obj->vertex_array);

		glBindBuffer(GL_ARRAY_BUFFER, gl_obj->vertex_buffer);
		if(i == ANT){
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
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
            gl_obj->texture_location = glGetUniformLocation(gl_obj->program, "antTexture");
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

void	set_gl_objects(t_data_visu *data_visu){
	data_visu->offset = 0.5f;

	data_visu->objSize[0] = count_room(data_visu->data.rooms);
	data_visu->objSize[1] = tunnel_size(data_visu->data.rooms);
	data_visu->objSize[2] = data_visu->data.total_ants;
	data_visu->objSize[3] = data_visu->objSize[0];

	data_visu->gl_objects[0].nbSegment = 42;
	data_visu->gl_objects[1].nbSegment = 4;

	set_rooms(data_visu);
	set_pipe(data_visu);
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
