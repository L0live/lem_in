#include	"../includes/lem_in_bonus.h"
#include	<unistd.h>

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
}

void	init_loop_data(t_data_visu *data_visu, t_loop_data *loop){

	loop->current_action = 0;
	loop->is_paused = false;
	loop->key_paused = false;
	loop->key_restart = false;

	set_first_and_count(&data_visu->gl_objects[ROOM], data_visu->objSize[ROOM]);
	set_first_and_count(&data_visu->gl_objects[PIPE], data_visu->objSize[PIPE]);
}

void	init_mvp(t_data_visu *data_visu, t_loop_data *loop){

	float	ratio;
	int		width;
	int		height;
	mat4x4	m;
	mat4x4	p;
	
	glfwGetFramebufferSize(data_visu->window, &width, &height);

	ratio = width / (float)height;
	
	glViewport(0, 0, width, height);
	glClearColor(0.08f, 0.09f, 0.12f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	mat4x4_identity(m);
	mat4x4_ortho(p, -1.0f * ratio, 1.0f * ratio, -1.0f, 1.0f, 1.f, -1.f);
	mat4x4_mul(loop->mvp, p, m);

	render_text(data_visu, "LEM-IN", -0.9f * ratio, 0.9f, 0.0013f, 1.0f, 1.0f, 1.0f, loop->mvp);
	render_text(data_visu, "Speed (+/-) :", -0.9f * ratio, 0.65f, 0.0010f, 1.0f, 1.0f, 1.0f, loop->mvp);
	
	char *speed = ft_itoa(data_visu->speed * 1000);
	render_text(data_visu, speed, -0.79f * ratio, 0.65f, 0.0010f, 1.0f, 1.0f, 1.0f, loop->mvp);
	
	render_text(data_visu, "Pause (SPACE)", -0.88f * ratio, 0.55f, 0.0010f, 1.0f, 1.0f, 1.0f, loop->mvp);
	render_text(data_visu, "Restart (R)", -0.91f * ratio, 0.45f, 0.0010f, 1.0f, 1.0f, 1.0f, loop->mvp);

	mat4x4_ortho(p, -0.7f * ratio, 0.7f * ratio, -0.7f, 0.7f, 1.f, -1.f);
	mat4x4_mul(loop->mvp, p, m);
}



void	init_direction(float (*ant_direction)[2], int size){
	for (int i = 0; i < size; i++){
		ant_direction[i][0] = 1.0f;
		ant_direction[i][1] = 0.0f;
	}
}

static int	wrong_map(t_list *stdin_content){

	char 	cmp[7] = "ERROR";
	char	*last_line = ft_lstlast(stdin_content)->content;
	char	*error = ft_substr(last_line, 0,5);

	if (ft_strcmp(error, cmp) == 0){
		free(error);
		return (-1);
	}
	free(error);
	return (0);
}

static int	main_loop(t_data_visu *data_visu) {

	t_loop_data 	loop_data;
	float			ant_direction[data_visu->data.total_ants][2];

	init_loop_data(data_visu, &loop_data);
	init_position(data_visu);
	init_direction(ant_direction, data_visu->data.total_ants);

	do{
		init_mvp(data_visu, &loop_data);
		
		if (keyboard_restart(data_visu, &loop_data))
			continue;
		keyboard_plus(data_visu, &loop_data);
		keyboard_minus(data_visu, &loop_data);

		draw_pipes(&data_visu->gl_objects[PIPE], data_visu->objSize[PIPE], (const GLfloat*) loop_data.mvp);
		draw_ants(data_visu, &loop_data, ant_direction);
		draw_rooms(data_visu, (const GLfloat*) loop_data.mvp);

		keyboard_pause(data_visu, &loop_data);
		
		glfwSwapBuffers(data_visu->window);
		
		if (loop_data.is_paused)
			glfwWaitEventsTimeout(0.1);
		else		
			glfwPollEvents();
	}
	while( glfwGetKey(data_visu->window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&

	glfwWindowShouldClose(data_visu->window) == 0 );
	free_gl_objects(data_visu->gl_objects);

	return (0);
}

#include <fcntl.h>
int main(void){
	t_list	*stdin_content = NULL;
	t_data_visu	data_visu;
	
	ft_memset(&data_visu, 0, sizeof(data_visu));
	data_visu.speed = 0.01f;

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
}