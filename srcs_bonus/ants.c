#include "../includes/lem_in_bonus.h"

//images texture
#define STB_IMAGE_IMPLEMENTATION
#include "../includes/stb_image.h"


GLfloat		vertices[] = {
    // position       // couleur           // coordonnées texture
     0.06f,  0.06f,      1.0f, 0.0f, 0.0f,    1.0f, 1.0f,
     0.06f, -0.06f,      1.0f, 0.0f, 0.0f,    1.0f, 0.0f,
    -0.06f, -0.06f,      1.0f, 0.0f, 0.0f,    0.0f, 0.0f,
    -0.06f,  0.06f,      1.0f, 1.0f, 0.0f,    0.0f, 1.0f
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
	stbi_set_flip_vertically_on_load(1); // OpenGL expects Y-axis to go upwards
	unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (!data){
		ft_printf("Failed to load texture: %s\n", filename);
		return 0;
	}

	// declarer et attacher la texture
	GLuint  antTexture;
	glGenTextures(1, &antTexture);
	glBindTexture(GL_TEXTURE_2D, antTexture);

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

void init_position(t_data_visu *data_visu){
	t_room		*start;
	t_ant		*ants;

	start = room_getby_id(data_visu->data.rooms, data_visu->data.start_id);
	if (!start)
		return ;

	ants = malloc(sizeof(t_ant) * data_visu->data.total_ants);
	if (!ants)
		return ;

	for (int i = 0; i < data_visu->data.total_ants; i++){
		ants[i].x = (float)start->x / data_visu->width - BASIC_OFFSET;
		ants[i].y = (float)start->y / data_visu->height - BASIC_OFFSET;
	}
	data_visu->ants = ants;
}

void    set_ants(t_data_visu *data_visu){
	t_gl_object *ant;

	ant = &data_visu->gl_objects[ANT];
	ant->vertices = vertices;
	// ant->vertices_size = sizeof(vertices);
	ant->vertices_size = sizeof(vertices) / sizeof(vertices[0]);
	
	data_visu->antTexture = loadTexture("ant copy.png");
	if (data_visu->antTexture == 0)
		ft_printf("Erreur de set ants\n");
	
	init_position(data_visu);
};
