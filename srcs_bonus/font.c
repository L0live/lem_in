#include "../includes/lem_in_bonus.h"

FT_Library  library;
FT_Face     fontFace;

static GLfloat	get_text_width(t_data_visu *data_visu, const char *text, GLfloat scale){
    GLfloat	width = 0.0f;
    
	for(int i = 0; text[i]; i++){
		if ((unsigned char)text[i] < 128)
			width += data_visu->glyphs[(unsigned char)text[i]].advance * scale;
	}
    return (width);
};

void	render_text( t_data_visu *data_visu,const char *text, GLfloat x, GLfloat y, GLfloat scale, GLfloat r, GLfloat g, GLfloat b, mat4x4 mvp){
	
	t_gl_object	*text_object = &data_visu->gl_objects[2];
	t_glyph 	*glyph;
	GLfloat		xpos;
	GLfloat		ypos;
	GLfloat		width;
	GLfloat		height;
	GLfloat		vertices[6][4];
    GLfloat	text_width = get_text_width(data_visu, text, scale);
    x -= text_width / 2.0f;

	glUseProgram(text_object->program);

	// pour la licalisation comme dans la boucle principale
	if (text_object->mvp_location != -1)
		glUniformMatrix4fv(text_object->mvp_location, 1, GL_FALSE, (const GLfloat *)mvp);
	
	if (text_object->text_color_location != -1)
		glUniform3f(text_object->text_color_location, r, g, b);

	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(text_object->vertex_array);

	for (size_t i = 0; text[i]; i++){
		if (text[i] < 0)
			continue;

		glyph = &data_visu->glyphs[(unsigned char)text[i]];

		xpos = x + glyph->bearing_x * scale;
		ypos = y - (glyph->height - glyph->bearing_y) * scale;

		width = glyph->width * scale;
		height = glyph->height * scale;

		vertices[0][0] = xpos;
		vertices[0][1] = ypos + height;
		vertices[0][2] = 0.0f;
		vertices[0][3] = 0.0f;

		vertices[1][0] = xpos;
		vertices[1][1] = ypos;
		vertices[1][2] = 0.0f;
		vertices[1][3] = 1.0f;

		vertices[2][0] = xpos + width;
		vertices[2][1] = ypos;
		vertices[2][2] = 1.0f;
		vertices[2][3] = 1.0f;

		vertices[3][0] = xpos;
		vertices[3][1] = ypos + height;
		vertices[3][2] = 0.0f;
		vertices[3][3] = 0.0f;

		vertices[4][0] = xpos + width;
		vertices[4][1] = ypos;
		vertices[4][2] = 1.0f;
		vertices[4][3] = 1.0f;

		vertices[5][0] = xpos + width;
		vertices[5][1] = ypos + height;
		vertices[5][2] = 1.0f;
		vertices[5][3] = 0.0f;


		// Rendu du glyphe sur le rectangle
		glBindTexture(GL_TEXTURE_2D,glyph->texture_id);

		// Mise à jour de la mémoire du VBO
		glBindBuffer(GL_ARRAY_BUFFER, text_object->vertex_buffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Rendu du rectangle
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Avancer le curseur au glyphe suivant (noter que l’avance est calculée en 1/64e pixels)
		x += glyph->advance * scale;
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void	set_font(t_data_visu *data_visu){

	//instance de la biblioteque FreeType
	if (FT_Init_FreeType(&library)) {
		ft_printf("ERROR::FREETYPE: Could not init FreeType Library\n");
		return;
	}

	//charger la fonte en tant que face
	if (FT_New_Face(library, "fonts/arial.ttf", 0, &fontFace)){
		ft_printf("ERROR::FREETYPE: Could not init FreeType Library\n");
		//!free library ?
		return;
	}

	// définir la taille que nous voulons extraire de cette face
	FT_Set_Pixel_Sizes(fontFace, 0, 48);
	//!free library et fontFace ?


	// Désactiver la restriction d’alignement d’octets
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (GLubyte c = 0; c < 128; c++){

		// Chargement du glyphe du caractère
		if (FT_Load_Char(fontFace, c, FT_LOAD_RENDER)){
			ft_printf("ERROR::FREETYPE: Failed to load Glyph\n");
			c++;
			continue;
		}

		// Générer la texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, (GLsizei)fontFace->glyph->bitmap.width,
		(GLsizei)fontFace->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, fontFace->glyph->bitmap.buffer);
		// Fixer les options de la texture
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		data_visu->glyphs[c].texture_id = texture;
		data_visu->glyphs[c].width = fontFace->glyph->bitmap.width;
		data_visu->glyphs[c].height = fontFace->glyph->bitmap.rows;
		data_visu->glyphs[c].bearing_x = fontFace->glyph->bitmap_left;
		data_visu->glyphs[c].bearing_y = fontFace->glyph->bitmap_top;
		data_visu->glyphs[c].advance = fontFace->glyph->advance.x >> 6;
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	FT_Done_Face(fontFace);
	FT_Done_FreeType(library);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
};