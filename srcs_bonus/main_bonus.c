#include	"../includes/lem_in_bonus.h"
#include	<unistd.h>

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

static void cleanup_opengl(GLuint vertex_array,
                           GLuint vertex_buffer,
                           GLuint program)
{
    if (program != 0)
        glDeleteProgram(program);
    if (vertex_buffer != 0)
        glDeleteBuffers(1, &vertex_buffer);
    if (vertex_array != 0)
        glDeleteVertexArrays(1, &vertex_array);
}


	// static const char* vertex_shader_text =
		// "#version 330 core\n"
		// "uniform mat4 MVP;\n"
		// "layout (location = 0) in vec2 vPos;\n"
		// "layout (location = 1) in vec3 vCol;\n"
		// "out vec3 color;\n"
		// "void main()\n"
		// "{\n"
		// "    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
		// "    color = vCol;\n"
		// "}\n";
		
	// static const char* fragment_shader_text =
		// "#version 330 core\n"
		// "in vec3 color;\n"
		// "out vec4 FragColor;\n"
		// "void main()\n"
		// "{\n"
		// "    FragColor = vec4(color, 1.0);\n"
		// "}\n";

// Vertex Shader source
static const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec4 aPos;\n"
	"uniform mat4 MVP;\n"
    "void main() {\n"
    "   gl_Position = MVP * vec4(aPos.x, aPos.y, 1.0, 1.0);\n"
    "}\0";

// Fragment Shader source
static const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

unsigned int createShaderProgram() {
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




int	main_loop(t_data_visu *data_visu, t_data *data) {
	
    GLuint vertex_buffer, vertex_array, program;
	GLint mvp_location;

	int rooms_size = count_room(data->rooms);
	int numSegments = 5;
    float radius = 0.1f;
	int nVerts = numSegments + 1;
    float* circleVerts = malloc(rooms_size * (nVerts * 2 * sizeof(float)));
	//! a free
	t_room *rooms =  data->rooms;
    
	const int total_segments = numSegments * 2; 
	for (int j = 0; rooms; j++) {
		for (int i = 0; i <= numSegments; i++) {
			float angle = 2.0f * M_PI * (float)i / (float)numSegments;
			printf("rooms.x %f\n", (float)rooms->x);
			printf("rooms.y %f\n", (float)rooms->y);
			circleVerts[(j * total_segments) + i * 2] = (float)rooms->x + radius * cosf(angle);
			circleVerts[(j * total_segments) + i * 2 + 1] = (float)rooms->y + radius * sinf(angle);
		}
		rooms = rooms->next;
	};

	printf("Total rooms %d\n", rooms_size);

	for (int i = 0; i <= rooms_size * numSegments * 2; i += 2){
		if (i % numSegments == 0)
			printf("\n");		
		printf("circleVerts[%d] %f , circleVerts[%d] %f\n",i, circleVerts[i],i + 1, circleVerts[i + 1]);
	}
	


	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);
	glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, nVerts * 2 * sizeof(float), circleVerts, GL_STATIC_DRAW);
	program = createShaderProgram();
	if (program == 0){
		cleanup_opengl(vertex_array, vertex_buffer, program);
		return (-1);
	}
 
 
    mvp_location = glGetUniformLocation(program, "MVP");
 
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                          2 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);




	glfwSetInputMode(data_visu->window, GLFW_STICKY_KEYS, GL_TRUE);

	do{
		float ratio;
        int width, height;
        mat4x4 m, p, mvp;
        glfwGetFramebufferSize(data_visu->window, &width, &height);
        ratio = width / (float) height;
 
        glViewport(0, 0, width, height);
		glClearColor(0.08f, 0.09f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
 
        mat4x4_identity(m);
        // mat4x4_rotate_Z(m, m, (float) glfwGetTime());
        mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        mat4x4_mul(mvp, p, m);
 
		// for (int i = 0; i < rooms_size; i++){
        	// glDrawArrays(GL_LINE_LOOP, i * nVerts, (i + 1) * numSegments);
		// }
		
        glDrawArrays(GL_LINE_LOOP, 0, nVerts * rooms_size);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
        glUseProgram(program);

		glfwSwapBuffers(data_visu->window);
		glfwPollEvents();
	}
	while( glfwGetKey(data_visu->window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
	glfwWindowShouldClose(data_visu->window) == 0 );
	cleanup_opengl(vertex_array, vertex_buffer, program);
	return (0);
};

int main(void){
	t_list	*stdin_content = NULL;
	t_data	data;
	t_data_visu	data_visu;

	if (read_stdin(&stdin_content) == -1)
		return (-1);
	if (!stdin_content)
		return (-1);
	ft_lstprint(stdin_content);
	
	init_data(&data);
	if (parsing(stdin_content, &data) == -1) {
		ft_lstclear(&stdin_content, &free);
		return (-1);
	}
	ft_lstclear(&stdin_content, &free);

	print_room(&data);

	if (init_glfw(&data_visu) == -1) {
		free_rooms(data.rooms);
		return (-1);
	}

	if (main_loop(&data_visu, &data) == -1) {
		glfwDestroyWindow(data_visu.window);
		glfwTerminate();
		free_rooms(data.rooms);
		return (-1);
	}

	glfwDestroyWindow(data_visu.window);
	glfwTerminate();
	free_rooms(data.rooms);
	return (0);
};