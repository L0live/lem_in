#include "../includes/lem_in_bonus.h"

//images texture
#define STB_IMAGE_IMPLEMENTATION
#include "../includes/stb_image.h"


GLfloat		vertices[] = {
    // position       // couleur           // coordonnées texture
     0.5f,  0.5f,      1.0f, 0.0f, 0.0f,    1.0f, 1.0f,
     0.5f, -0.5f,      0.0f, 1.0f, 0.0f,    1.0f, 0.0f,
    -0.5f, -0.5f,      0.0f, 0.0f, 1.0f,    0.0f, 0.0f,
    -0.5f,  0.5f,      1.0f, 1.0f, 0.0f,    0.0f, 1.0f
};

GLenum	setFormat(int nrChannels){
	GLenum format;

	if (nrChannels == 1)
		format = GL_RED;
	else if (nrChannels == 2)
		format = GL_RG;
	else if (nrChannels == 3)
		format = GL_RGB;
	else if (nrChannels == 4)
		format = GL_RGBA;
	else
		format = GL_RGB;
	return format;
};

GLuint  loadTexture(const char* filename){
	
	int width, height, nrChannels;

	// Load image data
	unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
	stbi_set_flip_vertically_on_load(1); // OpenGL expects Y-axis to go upwards
	
	// declarer et attacher la texture
	GLuint  antTexture;
	glGenTextures(1, &antTexture);
	glBindTexture(GL_TEXTURE_2D, antTexture);

	if (!data){
		ft_printf("Failed to load texture: %s\n", filename);
		return 0;
	}

	GLenum format = setFormat(nrChannels);
	ft_printf("width %d, height %d, nrChannels %d, filename %s, format %d\n",width ,height, nrChannels, filename, format);
	if (format == 0){
		ft_printf("Bad format: %s\n", filename);
		return 0;
	}	

	// Set texture wrapping filters (Clamp to edge prevents ants from bleeding off screens)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	// Set texture filtering (GL_NEAREST is critical for sharp, pixel-perfect ant graphics)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Upload to GPU
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);


	stbi_image_free(data);
	return (antTexture);
};

void    set_ants(t_data_visu *data_visu){
	t_gl_object *ant = &data_visu->gl_objects[ANT];

	ant->vertices = vertices;
	ant->vertices_size = sizeof(vertices);

	data_visu->antTexture = loadTexture("ant copy.png");
	if (data_visu->antTexture == 0)
		ft_printf("Erreur de set ants\n");
};
