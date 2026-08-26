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




int	main_loop(t_data_visu *data_visu) {
	const int	rooms_size = count_room(data_visu->data.rooms);
	data_visu->offset = 0.5f;
	data_visu->gl_objects[0].vertices = get_rooms_vertices(data_visu);
	glGenVertexArrays(1, &data_visu->gl_objects[0].vertex_array);
	glBindVertexArray(data_visu->gl_objects[0].vertex_array);
	glGenBuffers(1, &data_visu->gl_objects[0].vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, data_visu->gl_objects[0].vertex_buffer);
    // glBufferData(GL_ARRAY_BUFFER, nVerts * 2 * sizeof(float) * rooms_size, circleVerts, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, 43 * 2 * sizeof(float) * rooms_size, data_visu->gl_objects[0].vertices, GL_STATIC_DRAW);
	data_visu->gl_objects[0].program = createShaderProgram();
	if (data_visu->gl_objects[0].program == 0){
		cleanup_opengl(data_visu->gl_objects[0].vertex_array, data_visu->gl_objects[0].vertex_buffer, data_visu->gl_objects[0].program);
		return (-1);
	}
 
 
    data_visu->gl_objects[0].mvp_location = glGetUniformLocation(data_visu->gl_objects[0].program, "MVP");
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                          2 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);




	glfwSetInputMode(data_visu->window, GLFW_STICKY_KEYS, GL_TRUE);
	const int numSegments = 42;
	GLint first[rooms_size];
	GLsizei count[rooms_size];
	for (int i = 0; i < rooms_size; i++){
		first[i] = i * numSegments;
		count[i] = numSegments;
		printf("first[i] %d\n", i * numSegments);
		printf("count[i] %d\n", numSegments);
	}

	

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

		glMultiDrawArrays(GL_LINE_LOOP, first, count, rooms_size);
		
        glUniformMatrix4fv(data_visu->gl_objects[0].mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);

        glUseProgram(data_visu->gl_objects[0].program);

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