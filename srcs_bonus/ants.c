#include "../includes/lem_in_bonus.h"

//images texture
#define STB_IMAGE_IMPLEMENTATION
#include "../includes/stb_image.h"

GLuint  loadTexture(const char* filename){
	// Load image data

	
	// declarer et attacher la texture
	GLuint  textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	// //initialiser cette texture
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	// glGenerateMipmap(GL_TEXTURE_2D);

	// Set texture wrapping filters (Clamp to edge prevents ants from bleeding off screens)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	// Set texture filtering (GL_NEAREST is critical for sharp, pixel-perfect ant graphics)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
	// Load image data
	stbi_set_flip_vertically_on_load(1); // OpenGL expects Y-axis to go upwards

	if (data) {
		GLenum format = (nrChannels == 4) ? GL_RGBA : (nrChannels == 2) ? GL_RG : GL_RGB;
		ft_printf("width %d, height %d, nrChannels %d,filename %s format %d\n", width, height, nrChannels, filename, format);
		if (!format){
			ft_printf("Failed to load texture: %s\n", filename);
		}
		
		// Upload to GPU
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		ft_printf("Failed to load texture: %s\n", filename);

	stbi_image_free(data);
	return (textureId);
};

void    set_ants(t_data_visu *data_visu){

	GLuint antTexture = loadTexture("ant.png");



	data_visu->textureId = antTexture;
};
