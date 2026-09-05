#include	"../includes/lem_in_bonus.h"
#include	<unistd.h>

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
			// printf("first[i] %d\n", j * gl_obj->nbSegment);
			// printf("count[i] %d\n", gl_obj->nbSegment);
		}
	}
	//! a free
};

int	main_loop(t_data_visu *data_visu) {

	set_gl_objects(data_visu);
	if(gl_init(data_visu)){
		// ft_printf("erreur gl_init");
		return (-1);
	}
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
		mat4x4_ortho(p, -1.0f * ratio, 1.0f * ratio, -1.0f, 1.0f, 1.f, -1.f);
        mat4x4_mul(mvp, p, m);
		render_text(data_visu, "LEM-IN", -0.9f * ratio, 0.9f, 0.0013f, 1.0f, 1.0f, 1.0f, mvp);
		
        mat4x4_ortho(p, -0.7f * ratio, 0.7f * ratio, -0.7f, 0.7f, 1.f, -1.f);
        mat4x4_mul(mvp, p, m);


		for (int i = OBJS_SIZE - 1; i >=0; i--){
			if (i == TEXT)
				continue;			
			t_gl_object *gl_obj = &data_visu->gl_objects[i];
			
			glUseProgram(gl_obj->program);
			glBindVertexArray(gl_obj->vertex_array);

			glUniformMatrix4fv(gl_obj->mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
			if (i == ROOM)
				glMultiDrawArrays(GL_TRIANGLE_FAN, gl_obj->first, gl_obj->count, data_visu->objSize[i]);
			else if (i == PIPE)
				glMultiDrawArrays(GL_LINE_LOOP, gl_obj->first, gl_obj->count, data_visu->objSize[i]);
			if (i == ANT){
				glActiveTexture(GL_TEXTURE0); 
				glBindTexture(GL_TEXTURE_2D, data_visu->textureId);
				glGetUniformLocation(GL_TEXTURE_2D, "antTexture");
				glDrawArrays(GL_LINE_LOOP, 0, 4);
			}				
		}

		t_room *rooms = data_visu->data.rooms;
		while(rooms){
			float x = ((float)rooms->x - data_visu->min_x) / data_visu->width - data_visu->offset;
    		float y = ((float)rooms->y - data_visu->min_y) / data_visu->height - data_visu->offset;			
			render_text(data_visu, rooms->name, x, y - 0.013f, 0.00092f, 1.0f, 1.0f, 1.0f, mvp);
			// printf("room name: %s, x: %d, y: %d\n", rooms->name, rooms->x, rooms->y);
			rooms = rooms->next;
		}

		glfwSwapBuffers(data_visu->window);
		glfwPollEvents();
	}
	while( glfwGetKey(data_visu->window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
	glfwWindowShouldClose(data_visu->window) == 0 );
	cleanup_opengl(data_visu->gl_objects[0].vertex_array, data_visu->gl_objects[0].vertex_buffer, data_visu->gl_objects[0].program);
	free_gl_objects(data_visu->gl_objects);
	return (0);
};

int main(void){
	t_list	*stdin_content = NULL;
	t_data_visu	data_visu;
	
	//! Mieux ?
	ft_bzero(&data_visu, sizeof(data_visu));

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