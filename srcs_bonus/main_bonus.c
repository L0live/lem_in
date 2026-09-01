#include	"../includes/lem_in_bonus.h"
#include	<unistd.h>

float tests = 0;

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

static GLuint	compile_shader(GLenum type, const char *source) {
	GLuint shader;
	GLint	status;

	shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
	{
		glDeleteShader(shader);
		return (0);
	}
	return (shader);
}

static void cleanup_opengl(GLuint vertex_array, GLuint vertex_buffer, GLuint program){
    if (program != 0)
        glDeleteProgram(program);
    if (vertex_buffer != 0)
        glDeleteBuffers(1, &vertex_buffer);
    if (vertex_array != 0)
        glDeleteVertexArrays(1, &vertex_array);
}


// // Vertex Shader source
// static const char* vertexShaderSource = "#version 330 core\n"
    // "layout (location = 0) in vec4 aPos;\n"
	// "uniform mat4 MVP;\n"
    // "void main() {\n"
    // "   gl_Position = MVP * vec4(aPos.x, aPos.y, 1.0, 1.0);\n"
    // "}\0";

// // Fragment Shader source
// static const char* fragmentShaderSource = "#version 330 core\n"
    // "out vec4 FragColor;\n"
    // "void main() {\n"
    // "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    // "}\0";

unsigned int createShaderProgram(const char *vertexShaderSource, const char* fragmentShaderSource){

    unsigned int vertexShader = compile_shader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int fragmentShader = compile_shader(GL_FRAGMENT_SHADER, fragmentShaderSource);
	if (vertexShader == 0 || fragmentShader == 0)
		return (0);
    
    unsigned int shaderProgram = glCreateProgram();
	if (shaderProgram == 0){
	    glDeleteShader(vertexShader);
	    glDeleteShader(fragmentShader);
		return (0);
	}

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		glDeleteShader(vertexShader);
	    glDeleteShader(fragmentShader);
		return (0);
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
};


void	get_vertices(t_data_visu *data_visu){

	set_rooms(data_visu);
	set_pipe(data_visu);
	// data_visu->gl_objects[0].vertices = get_rooms_vertices(data_visu);
	// data_visu->gl_objects[0].vertices = get_rooms_vertices(data_visu);
	// data_visu->gl_objects[0].vertices = get_rooms_vertices(data_visu);
};

void	set_obj_vertices(t_data_visu *data_visu){
	data_visu->offset = 0.5f;

	data_visu->objSize[0] = count_room(data_visu->data.rooms);
	data_visu->objSize[1] = tunnel_size(data_visu->data.rooms);
	data_visu->objSize[2] = data_visu->data.total_ants;
	data_visu->objSize[3] = data_visu->objSize[0];

	data_visu->gl_objects[0].nbSegment = 42;
	data_visu->gl_objects[1].nbSegment = 4;

	// set_rooms(data_visu);
	// set_pipe(data_visu);
};

static const char* const *getvertexShader(){

	static const char *vertexShaderSource[4] = {
		"#version 330 core\n"
		"layout (location = 0) in vec2 aPos;\n"
		"layout (location = 1) in vec4 aColor;\n"
		"uniform mat4 MVP;\n"
		"out vec4 vertexColor;\n"
		"void main()\n"
		"{\n"
		"    gl_Position = MVP * vec4(aPos, 1.0, 1.0);\n"
		"    vertexColor = aColor;\n"
		"}\n",

		"#version 330 core\n"
		"layout (location = 0) in vec2 aPos;\n"
		"uniform mat4 MVP;\n"
		"void main()\n"
		"{\n"
		"    gl_Position = MVP * vec4(aPos, 1.0, 1.0);\n"
		"}\n",

		"#version 330 core\n"
		"layout (location = 0) in vec2 aPos;\n"
		"uniform mat4 MVP;\n"
		"void main()\n"
		"{\n"
		"    gl_Position = MVP * vec4(aPos, 1.0, 1.0);\n"
		"}\n",

		"#version 330 core\n"
		"layout (location = 0) in vec2 aPos;\n"
		"uniform mat4 MVP;\n"
		"void main()\n"
		"{\n"
		"    gl_Position = MVP * vec4(aPos, 1.0, 1.0);\n"
		"}\n"
	};

	return (vertexShaderSource);
};

static const char* const *getFragmentShaders(){

	static const char *fragmentShaderSource[4] = {
		"#version 330 core\n"
		"in vec4 vertexColor;\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"    FragColor = vertexColor;\n"
		"}\n",

		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"uniform vec4 objectColor;\n"
		"void main()\n"
		"{\n"
		"    FragColor = objectColor;\n"
		"}\n",

		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"uniform vec4 objectColor;\n"
		"void main()\n"
		"{\n"
		"    FragColor = objectColor;\n"
		"}\n",

		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"uniform vec4 objectColor;\n"
		"void main()\n"
		"{\n"
		"    FragColor = objectColor;\n"
		"}\n"
	};

	return (fragmentShaderSource);
}


int	gl_init(t_data_visu *data_visu){

	const char* const *vertexShaderSource = getvertexShader();
	const char* const *fragmentShaderSource = getFragmentShaders();

	// for (size_t i = 0; i < 4; i++){
	for (size_t i = 0; i < 2; i++){
		t_gl_object *gl_obj = &data_visu->gl_objects[i];

		//positiion
		glGenVertexArrays(1, &gl_obj->vertex_array);
		glGenBuffers(1, &gl_obj->vertex_buffer);
		
		glBindVertexArray(gl_obj->vertex_array);

		glBindBuffer(GL_ARRAY_BUFFER, gl_obj->vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, gl_obj->vertices_size * sizeof(float) , gl_obj->vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
    	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) 0);

		glGenBuffers(1, &gl_obj->color_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, gl_obj->color_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * gl_obj->colors_size, gl_obj->colors, GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);

		gl_obj->program = createShaderProgram(vertexShaderSource[i], fragmentShaderSource[i]);
		if (gl_obj->program == 0){
			cleanup_opengl(gl_obj->vertex_array, gl_obj->vertex_buffer, gl_obj->program);
			return (-1);
		}
		
	    gl_obj->mvp_location = glGetUniformLocation(gl_obj->program, "MVP");
	    gl_obj->color_buffer = glGetUniformLocation(gl_obj->program, "objectColor");
	};

	glfwSetInputMode(data_visu->window, GLFW_STICKY_KEYS, GL_TRUE);

	return (0);
};

void	set_first_and_count(t_data_visu* data_visu){

	for (size_t i = 0; i < 2; i++){
		int size = data_visu->objSize[i];
		t_gl_object *gl_obj = &data_visu->gl_objects[i];

        gl_obj->first = malloc(sizeof(GLint) * size);
        gl_obj->count = malloc(sizeof(GLint) * size);
		
		if (!gl_obj->first || !gl_obj->count)
			return;
		
		for (int j = 0; j < size; j++){
			gl_obj->first[j] = j * gl_obj->nbSegment;
			gl_obj->count[j] = gl_obj->nbSegment;
			printf("first[i] %d\n", j * gl_obj->nbSegment);
			printf("count[i] %d\n", gl_obj->nbSegment);
		}
	}

	//! a free
};

int	main_loop(t_data_visu *data_visu) {
	set_obj_vertices(data_visu);
	get_vertices(data_visu);
	if(gl_init(data_visu))
		return (-1);
	set_first_and_count(data_visu);

	do{
		float ratio;
        int width, height;
        mat4x4 m, p, mvp;
        glfwGetFramebufferSize(data_visu->window, &width, &height);
        ratio = width / (float) height;
 
		// (void)ratio;
        glViewport(0, 0, width, height);
		glClearColor(0.08f, 0.09f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
		
        mat4x4_identity(m);
        mat4x4_ortho(p, -0.7f * ratio, 0.7f * ratio, -0.7f, 0.7f, 1.f, -1.f);
        mat4x4_mul(mvp, p, m);

		// for (size_t i = 0; i < 4; i++){
		for (int i = 1; i>=0; i--){
			t_gl_object *gl_obj = &data_visu->gl_objects[i];
			
			glUseProgram(gl_obj->program);
			glBindVertexArray(gl_obj->vertex_array);

			glUniformMatrix4fv(gl_obj->mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
			if (i == 0)
				glMultiDrawArrays(GL_TRIANGLE_FAN, gl_obj->first, gl_obj->count, data_visu->objSize[i]);
			else
				glMultiDrawArrays(GL_LINE_LOOP, gl_obj->first, gl_obj->count, data_visu->objSize[i]);
		}

		glfwSwapBuffers(data_visu->window);
		glfwPollEvents();
	}
	while( glfwGetKey(data_visu->window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
	glfwWindowShouldClose(data_visu->window) == 0 );
	cleanup_opengl(data_visu->gl_objects[0].vertex_array, data_visu->gl_objects[0].vertex_buffer, data_visu->gl_objects[0].program);
	return (0);
};

int main(void){
	t_list	*stdin_content = NULL;
	t_data_visu	data_visu;

	if (read_stdin(&stdin_content) == -1)
		return (-1);
	if (!stdin_content)
		return (-1);
	ft_lstprint(stdin_content);
	
	init_data(&data_visu.data);
	if (parsing(stdin_content, &data_visu.data) == -1) {
		ft_lstclear(&stdin_content, &free);
		return (-1);
	}
	ft_lstclear(&stdin_content, &free);

	print_room(&data_visu.data);

	if (init_glfw(&data_visu) == -1) {
		free_rooms(data_visu.data.rooms);
		return (-1);
	}

	if (main_loop(&data_visu) == -1) {
		glfwDestroyWindow(data_visu.window);
		glfwTerminate();
		free_rooms(data_visu.data.rooms);
		return (-1);
	}

	glfwDestroyWindow(data_visu.window);
	glfwTerminate();
	free_rooms(data_visu.data.rooms);
	return (0);
};