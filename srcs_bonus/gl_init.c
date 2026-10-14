#include	"../includes/lem_in_bonus.h"

static void	set_ant_buffers(t_gl_object *gl_obj){
	glBufferData(GL_ARRAY_BUFFER, gl_obj->vertices_size * sizeof(float) , gl_obj->vertices, GL_STATIC_DRAW);
	
	//aPos en vec2
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);

	//aColor ne vec3
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float)));


	//aTexCoord en vec2
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5 * sizeof(float)));
}

static void	set_text_buffers(void){
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *)0);
}

static void	set_room_buffers(t_gl_object *gl_obj){
	/* positions */
	glBufferData(GL_ARRAY_BUFFER, gl_obj->vertices_size * sizeof(float) , gl_obj->vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) 0);

	/* couleurs */
	glGenBuffers(1, &gl_obj->color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, gl_obj->color_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * gl_obj->colors_size, gl_obj->colors, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
}

void	set_objects_buffers(t_gl_object *gl_obj, size_t type){
	glGenVertexArrays(1, &gl_obj->vertex_array);
	glBindVertexArray(gl_obj->vertex_array);

	glGenBuffers(1, &gl_obj->vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, gl_obj->vertex_buffer);

	if (type == ANT)
		set_ant_buffers(gl_obj);
	else if (type == TEXT)
		set_text_buffers();
	else
		set_room_buffers(gl_obj);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

static void	set_text_uniform_location(t_gl_object *gl_obj){
	gl_obj->texture_location = glGetUniformLocation(gl_obj->program, "text");
	gl_obj->text_color_location = glGetUniformLocation(gl_obj->program, "textColor");
}

static void	set_ant_uniform_location(t_gl_object *gl_obj){
	// activer la transparence
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	gl_obj->mvp_location = glGetUniformLocation(gl_obj->program, "MVP");
	gl_obj->texture_direction = glGetUniformLocation(gl_obj->program, "antDirection");
	gl_obj->ant_position_location = glGetUniformLocation(gl_obj->program, "antPosition");
	gl_obj->texture_location = glGetUniformLocation(gl_obj->program, "antTexture");
	glUseProgram(gl_obj->program);
	glUniform1i(gl_obj->texture_location, 0);
}

static void	set_room_uniform_location(t_gl_object *gl_obj){
	gl_obj->mvp_location = glGetUniformLocation(gl_obj->program, "MVP");
	gl_obj->color_location = glGetUniformLocation(gl_obj->program, "objectColor");
}

void	set_objects_uniform_location(t_gl_object *gl_obj, size_t type){
	if (type == TEXT)
		set_text_uniform_location(gl_obj);
	else if (type == ANT)
		set_ant_uniform_location(gl_obj);
	else
		set_room_uniform_location(gl_obj);
}
