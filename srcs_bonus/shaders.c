#include	"../includes/lem_in_bonus.h"
#include <string.h>

static char *read_file(const char *path){
	FILE    *f;
	long    size;
	size_t  read_size;
	char    *buffer;

	f = fopen(path, "rb");
	if (!f){
		fprintf(stderr, "Erreur ouverture fichier : %s\n", path);
		return (NULL);
	}

	if (fseek(f, 0, SEEK_END) != 0){
		fclose(f);
		return (NULL);
	}

	size = ftell(f);
	if (size < 0){
		fclose(f);
		return (NULL);
	}

	rewind(f);

	buffer = malloc((size_t)size + 1);
	if (!buffer){
		fclose(f);
		return (NULL);
	}

	read_size = fread(buffer, 1, (size_t)size, f);
	fclose(f);

	if (read_size != (size_t)size){
		free(buffer);
		return (NULL);
	}

	buffer[size] = '\0';
	return (buffer);
}

static GLuint	compile_shader(GLenum type, const char *source) {
	GLuint shader;
	GLint	status;

	shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE){
		glDeleteShader(shader);
		return (0);
	}
	return (shader);
}

unsigned int createProgram(t_object_type shader_type){
	const char	*vertexPath;
	const char	*fragmentPath;
	GLuint		vertexShader;
    GLuint		fragmentShader;
    GLuint		shaderProgram;

	switch (shader_type) {
		case TEXT:
			vertexPath = "shaders/text.vert";
			fragmentPath = "shaders/text.frag";
			break;
		case ANT:
			vertexPath = "shaders/texture.vert";
			fragmentPath = "shaders/texture.frag";
			break;
		default:
			vertexPath = "shaders/basic.vert";
			fragmentPath = "shaders/basic.frag";
			break;
	}

	char *vertexSource = read_file(vertexPath);
	if (!vertexSource)
		return (0);
	char *fragmentSource = read_file(fragmentPath);
	if (!fragmentSource){
		free(vertexSource);
		return (0);
	}

	// printf("vertexSource %s\nfragmentSource %s\n", vertexSource, fragmentSource );

	vertexShader = compile_shader(GL_VERTEX_SHADER, vertexSource);
	fragmentShader = compile_shader(GL_FRAGMENT_SHADER, fragmentSource);

	free(vertexSource);
	free(fragmentSource);

	if (vertexShader == 0 || fragmentShader == 0){
		if (vertexShader != 0)
			glDeleteShader(vertexShader);
		if (fragmentShader != 0)
			glDeleteShader(fragmentShader);
		return (0);
	}
	
	shaderProgram = glCreateProgram();
	if (shaderProgram == 0){
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		return (0);
	}

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	GLint success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		ft_printf("Info Logs: %s\n", infoLog);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteProgram(shaderProgram);
		return (0);
	}
	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return (shaderProgram);
};