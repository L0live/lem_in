#include	"../includes/lem_in_bonus.h"

static void	use_obj(t_gl_object *gl_obj, const GLfloat *mvp) {
	glUseProgram(gl_obj->program);
	glBindVertexArray(gl_obj->vertex_array);
	glUniformMatrix4fv(gl_obj->mvp_location, 1, GL_FALSE, mvp);
}

void	draw_rooms(t_data_visu *data_visu, const GLfloat *mvp) {
	t_gl_object *gl_obj = &data_visu->gl_objects[ROOM];

	use_obj(gl_obj, mvp);
	glMultiDrawArrays(GL_TRIANGLE_FAN, gl_obj->first, gl_obj->count, data_visu->objSize[ROOM]);

	t_room *rooms = data_visu->data.rooms;
	while(rooms){
		float x = (float)rooms->x / data_visu->width - BASIC_OFFSET;
		float y = (float)rooms->y / data_visu->height - BASIC_OFFSET;			
		render_text(data_visu, rooms->name, x, y - 0.013f, 0.00092f, 1.0f, 1.0f, 1.0f, (vec4 *)mvp);
		// printf("room name: %s, x: %d, y: %d\n", rooms->name, rooms->x, rooms->y);
		rooms = rooms->next;
	}
}

void	draw_pipes(t_gl_object *gl_obj, int size, const GLfloat *mvp) {
	float	time = (float)glfwGetTime();
	float	shining = (0.1f * sinf(time * 10.0f) + 0.5f);
	
	use_obj(gl_obj, mvp);

	for (size_t i = 0; i < gl_obj->colors_size; i++)
		gl_obj->colors[i] = shining;
	glBindBuffer(GL_ARRAY_BUFFER, gl_obj->color_buffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, gl_obj->colors_size * sizeof(float), gl_obj->colors);			
	glMultiDrawArrays(GL_LINE_LOOP, gl_obj->first, gl_obj->count, size);
}

void	draw_ants(t_data_visu *data_visu, t_loop_data *loop_data, float (*ant_direction)[2]) {
	t_gl_object	*gl_obj = &data_visu->gl_objects[ANT];


	use_obj(gl_obj, (const GLfloat*) loop_data->mvp);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, data_visu->antTexture);


	t_list *line = data_visu->actions_lines;
	for (int actions_index = 0; line && actions_index < loop_data->current_action; actions_index++)
	line = line->next;
	
	bool actions_finished = true;
	
	if(loop_data->is_paused == false){
		if (line){
			t_actions *action = line->content;
			
			while (action){
				t_ant *ant = action->ant;
				t_room *room = action->room;
				
				float tmpX =  room->x;
				float tmpY =  room->y;
				
				room_to_gl(room, data_visu, &tmpX, &tmpY);
				float distX = tmpX - ant->x;
				float distY = tmpY - ant->y;
				
				float distance = sqrtf(distX * distX + distY * distY);
				
				int ant_index = ant - data_visu->ants;

				if (distance > 0.000001f){
					ant_direction[ant_index][0] = distX / distance;
					ant_direction[ant_index][1] = distY / distance;
					actions_finished = false;
					if (distance <= data_visu->speed){
						ant->x = tmpX;
						ant->y = tmpY;
					}
					else{
						ant->x += ant_direction[ant_index][0] * data_visu->speed;
						ant->y += ant_direction[ant_index][1] * data_visu->speed;
					}
				}
				action = action->next;
			}
		}
		
		if (actions_finished)
			loop_data->current_action++;
	}

	for (int ant_index = 0; ant_index < data_visu->data.total_ants; ant_index++){
		glUniform2f(gl_obj->ant_position_location, data_visu->ants[ant_index].x, data_visu->ants[ant_index].y);
		glUniform2f(gl_obj->texture_direction, ant_direction[ant_index][0], ant_direction[ant_index][1]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}
}