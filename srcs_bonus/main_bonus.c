#include	"../includes/lem_in_bonus.h"
#include	<unistd.h>


static void	cleanup_visu(t_data_visu *data_visu){
	free_actions_lines(&data_visu->actions_lines);
	free(data_visu->ants);
	data_visu->ants = NULL;
	free_rooms(data_visu->data.rooms);
	data_visu->data.rooms = NULL;
}

void	set_first_and_count(t_gl_object *gl_obj, int size){

	gl_obj->first = malloc(sizeof(GLint) * size);
	gl_obj->count = malloc(sizeof(GLint) * size);
	
	if (!gl_obj->first || !gl_obj->count){
		free(gl_obj->first);
		free(gl_obj->count);
		gl_obj->first = NULL;
		gl_obj->count = NULL;
		return;
	}
	
	for (int j = 0; j < size; j++){
		gl_obj->first[j] = j * gl_obj->nbSegment;
		gl_obj->count[j] = gl_obj->nbSegment;

	}
};

void	init_direction(t_data_visu *data_visu, float *ant_direction_x, float *ant_direction_y){
	
	for (int i = 0; i < data_visu->data.total_ants; i++){
		ant_direction_x[i] = 1.0f;
		ant_direction_y[i] = 0.0f;
    }	
}

#include <math.h>
int	main_loop(t_data_visu *data_visu) {

	bool	isPaused = false;
	bool	keyPaused = false;
	bool	keyRestart = false;
	
	float	ant_direction_x[data_visu->data.total_ants];
	float	ant_direction_y[data_visu->data.total_ants];
	
	int		current_action;

	set_first_and_count(&data_visu->gl_objects[ROOM], data_visu->objSize[ROOM]);
	set_first_and_count(&data_visu->gl_objects[PIPE], data_visu->objSize[PIPE]);

	current_action = 0;
	init_direction(data_visu, ant_direction_x, ant_direction_y);
	init_position(data_visu);

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
		mat4x4_ortho(p, -1.0f * ratio, 1.0f * ratio, -1.0f, 1.0f, 1.f, -1.f);
		mat4x4_mul(mvp, p, m);
		render_text(data_visu, "LEM-IN", -0.9f * ratio, 0.9f, 0.0013f, 1.0f, 1.0f, 1.0f, mvp);
		
		mat4x4_ortho(p, -0.7f * ratio, 0.7f * ratio, -0.7f, 0.7f, 1.f, -1.f);
		mat4x4_mul(mvp, p, m);

		///pause
		bool isPressed = (glfwGetKey(data_visu->window, GLFW_KEY_SPACE) == GLFW_PRESS);
		if (isPressed && !keyPaused)
			isPaused = !isPaused;
		
		keyPaused = isPressed;
		/////

		///restart
		bool isRestarted = (glfwGetKey(data_visu->window, GLFW_KEY_R) == GLFW_PRESS);
		if (isRestarted && !keyRestart){
			keyRestart = true;
			current_action = 0;
			init_position(data_visu);
		}
		
		keyRestart = isRestarted;
		/////

		if (isPaused)
			render_text(data_visu, "PAUSE", -0.0f , 0.0f, 0.0042f, 1.0f, 1.0f, 1.0f, mvp);
		
		for (int i = 0; i < OBJS_SIZE; i++){
			if (i == TEXT)
				continue;			
			t_gl_object *gl_obj = &data_visu->gl_objects[i];
			
			glUseProgram(gl_obj->program);
			glBindVertexArray(gl_obj->vertex_array);

			glUniformMatrix4fv(gl_obj->mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
			if (i == ROOM)
				glMultiDrawArrays(GL_TRIANGLE_FAN, gl_obj->first, gl_obj->count, data_visu->objSize[i]);
			else if (i == PIPE){
				float	time = (float)glfwGetTime();
				float	shining = (0.1f * sinf(time * 10.0f) + 0.5f);

				for (size_t i = 0; i < gl_obj->colors_size; i++)
					gl_obj->colors[i] = shining;
				glBindBuffer(GL_ARRAY_BUFFER, gl_obj->color_buffer);
				glBufferSubData(GL_ARRAY_BUFFER, 0, gl_obj->colors_size * sizeof(float), gl_obj->colors);			
				glMultiDrawArrays(GL_LINE_LOOP, gl_obj->first, gl_obj->count, data_visu->objSize[i]);
			}
			if (i == ANT){
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, data_visu->antTexture);

				if(isPaused == false){

					t_list *line = data_visu->actions_lines;
					for (int actions_index = 0; line && actions_index < current_action; actions_index++)
					line = line->next;
					
					bool actions_finished = true;
					
					if (line){
						t_actions *action = line->content;
						
						while (action){
							t_ant *ant = action->ant;
							t_room *room = action->room;
							
							// printf("ant (x,y) -> (%f,%f)\n", ant->x, ant->y);
							// printf("room cible (x,y) -> (%f,%f)\n", (float)ant->x, (float)room->y);
							float tmpX =  room->x;
							float tmpY =  room->y;
							
							room_to_gl(room, data_visu, &tmpX, &tmpY);

							float distX = tmpX - ant->x;
							float distY = tmpY - ant->y;
							
							float distance = sqrtf(distX * distX + distY * distY);
							// printf("Distance  -> %f\n", distance);
							
							int ant_index = ant - data_visu->ants;

							if (distance > 0.000001f){
								actions_finished = false;
								ant_direction_x[ant_index] = distX /distance;
								ant_direction_y[ant_index] = distY /distance;
								if (distance <= 0.005){
									ant->x = tmpX;
									ant->y = tmpY;
								}
								else{
									ant->x += ant_direction_x[ant_index] * ANT_STEP;
									ant->y += ant_direction_y[ant_index] * ANT_STEP;
								}
							}
							action = action->next;
						}
					
						if (actions_finished)
							current_action++;
					}
				}

				for (int ant_index = 0; ant_index < data_visu->data.total_ants; ant_index++){
					glUniform2f(gl_obj->ant_position_location, data_visu->ants[ant_index].x, data_visu->ants[ant_index].y);
					glUniform2f(gl_obj->texture_direction, ant_direction_x[ant_index], ant_direction_y[ant_index]);
					glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
				}
			}			
		}

		t_room *rooms = data_visu->data.rooms;
		while(rooms){
			float x = (float)rooms->x / data_visu->width - BASIC_OFFSET;
			float y = (float)rooms->y / data_visu->height - BASIC_OFFSET;			
			render_text(data_visu, rooms->name, x, y - 0.013f, 0.00092f, 1.0f, 1.0f, 1.0f, mvp);
			// printf("room name: %s, x: %d, y: %d\n", rooms->name, rooms->x, rooms->y);
			rooms = rooms->next;
		}

		glfwSwapBuffers(data_visu->window);

		if (isPaused)
			glfwWaitEventsTimeout(0.1);
		else		
			glfwPollEvents();
	}
	while( glfwGetKey(data_visu->window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
	glfwWindowShouldClose(data_visu->window) == 0 );
	free_gl_objects(data_visu->gl_objects);
	return (0);
};

int	wrong_map(t_list *stdin_content){

	char 	cmp[7] = "ERROR";
	char	*last_line = ft_lstlast(stdin_content)->content;
	char	*error = ft_substr(last_line, 0,5);

	//* ft_printf("errorCMP: %s*\n et CharERROR :%s*\n", error, cmp); 
	//* ft_printf("%d\n", ft_strcmp(error, cmp)); 
	if (ft_strcmp(error, cmp) == 0){
		free(error);
		return (-1);
	}
	free(error);
	return (0);
};

void	print_action_list(t_list *actions_list){

	t_actions	*actions;

	for (size_t i = 0; actions_list; i++){
		actions = actions_list->content;

		if (actions){
			ft_printf("Action numéro %d\n", i);
			while (actions){
				ft_printf("L%d-%s ", actions->ant->id, actions->room->name);
				actions = actions->next;
			}
			ft_printf("\n");
		}
		actions_list = actions_list->next;
	}
}

#include <fcntl.h>
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

	if (wrong_map(stdin_content) == -1){
		ft_lstclear(&stdin_content, &free);
		free_rooms(data_visu.data.rooms);
		return (-1);
	}

	int	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdout == -1)
		return (-1);

	fflush(stdout);

	int devnull = open("/dev/null", O_WRONLY);
	if (devnull == -1)
		return (-1);

	if (dup2(devnull, STDOUT_FILENO) == -1)
		return (-1);	
	close(devnull);	

	parsing(stdin_content, &data_visu.data);


	fflush(stdout);
	if (dup2(saved_stdout, STDOUT_FILENO) == -1)
		return (-1);

	if (pars_actions(stdin_content, &data_visu) == -1){
		ft_lstclear(&stdin_content, &free);
		cleanup_visu(&data_visu);
		return (-1);
	}


	ft_lstclear(&stdin_content, &free);

	if (init_glfw(&data_visu) == -1) {
		cleanup_visu(&data_visu);
		return (-1);
	}

	set_gl_objects(&data_visu);
	if(gl_init(&data_visu)){
		cleanup_visu(&data_visu);
		return (-1);	
	}

	if (main_loop(&data_visu) == -1) {
		glfwDestroyWindow(data_visu.window);
		glfwTerminate();
		cleanup_visu(&data_visu);
		return (-1);
	}

	if (data_visu.antTexture)
		glDeleteTextures(1, &data_visu.antTexture);	
	if (data_visu.window)
		glfwDestroyWindow(data_visu.window);
	glfwTerminate();

	cleanup_visu(&data_visu);
	return (0);
};