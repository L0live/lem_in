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
                           GLuint vertex_shader,
                           GLuint fragment_shader,
                           GLuint program)
{
    if (vertex_shader != 0)
        glDeleteShader(vertex_shader);
    if (fragment_shader != 0)
        glDeleteShader(fragment_shader);
    if (program != 0)
        glDeleteProgram(program);
    if (vertex_buffer != 0)
        glDeleteBuffers(1, &vertex_buffer);
    if (vertex_array != 0)
        glDeleteVertexArrays(1, &vertex_array);
}

int	main_loop(t_data_visu *data_visu) {
	
    GLuint vertex_buffer, vertex_array, vertex_shader, fragment_shader, program;
    GLint mvp_location, vpos_location, vcol_location;

	static const struct
	{
		float x, y;
		float r, g, b;
	} vertices[3] =
	{
		{ -0.6f, -0.4f, 1.f, 0.f, 0.f },
		{  0.6f, -0.4f, 0.f, 1.f, 0.f },
		{   0.f,  0.6f, 0.f, 0.f, 1.f }
	};

	static const char* vertex_shader_text =
		"#version 330 core\n"
		"uniform mat4 MVP;\n"
		"layout (location = 0) in vec2 vPos;\n"
		"layout (location = 1) in vec3 vCol;\n"
		"out vec3 color;\n"
		"void main()\n"
		"{\n"
		"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
		"    color = vCol;\n"
		"}\n";
		
	static const char* fragment_shader_text =
		"#version 330 core\n"
		"in vec3 color;\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"    FragColor = vec4(color, 1.0);\n"
		"}\n";

	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);
	glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	vertex_shader = compile_shader(GL_VERTEX_SHADER, vertex_shader_text);
	fragment_shader = compile_shader(GL_FRAGMENT_SHADER, fragment_shader_text);
	program = glCreateProgram();
	if (vertex_shader == 0 || fragment_shader == 0){
		cleanup_opengl(vertex_array, vertex_buffer, vertex_shader, fragment_shader, program);
		return (-1);
	}
 
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
	{
		GLint link_status;

		glGetProgramiv(program, GL_LINK_STATUS, &link_status);
		if (link_status != GL_TRUE){
			cleanup_opengl(vertex_array, vertex_buffer, vertex_shader, fragment_shader, program);
			return (-1);
		}
	}
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
 
    mvp_location = glGetUniformLocation(program, "MVP");
	vpos_location = 0;
	vcol_location = 1;
 
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                          sizeof(vertices[0]), (void*) 0);
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(vertices[0]), (void*) (sizeof(float) * 2));



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
        mat4x4_rotate_Z(m, m, (float) glfwGetTime());
        mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        mat4x4_mul(mvp, p, m);
 
        glUseProgram(program);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
        glDrawArrays(GL_TRIANGLES, 0, 3);




		glfwSwapBuffers(data_visu->window);
		glfwPollEvents();
	}
	while( glfwGetKey(data_visu->window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
	glfwWindowShouldClose(data_visu->window) == 0 );
	cleanup_opengl(vertex_array, vertex_buffer, vertex_shader, fragment_shader, program);
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

	if (main_loop(&data_visu) == -1) {
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