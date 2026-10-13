#include	"../includes/lem_in_bonus.h"

void	keyboard_pause(t_data_visu *data_visu, t_loop_data *loop){
	bool is_pressed = (glfwGetKey(data_visu->window, GLFW_KEY_SPACE) == GLFW_PRESS);
	if (is_pressed && !loop->key_paused)
		loop->is_paused = !loop->is_paused;
	
	loop->key_paused = is_pressed;

	if (loop->is_paused)
		render_text(data_visu, "PAUSE", -0.0f , 0.0f, 0.0042f, 1.0f, 1.0f, 1.0f, loop->mvp);
}

bool	keyboard_restart(t_data_visu *data_visu, t_loop_data *loop){
	bool is_restarted = (glfwGetKey(data_visu->window, GLFW_KEY_R) == GLFW_PRESS);

	if (is_restarted && !loop->key_restart){
		loop->key_restart = true;
		loop->current_action = 0;
		init_position(data_visu);
		return (true);
	}
	loop->key_restart = is_restarted;
	return (false);
}

void	keyboard_plus(t_data_visu *data_visu, t_loop_data *loop){
	(void) loop;
	bool is_plus_pressed  = (glfwGetKey(data_visu->window, GLFW_KEY_KP_ADD) == GLFW_PRESS) || 
                        (glfwGetKey(data_visu->window, GLFW_KEY_EQUAL) == GLFW_PRESS);

	if (is_plus_pressed)
		data_visu->speed += 0.0003f;
}

void	keyboard_minus(t_data_visu *data_visu, t_loop_data *loop){
	(void) loop;
	bool is_minus_pressed = (glfwGetKey(data_visu->window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) || 
							(glfwGetKey(data_visu->window, GLFW_KEY_MINUS) == GLFW_PRESS);

	if (is_minus_pressed && (data_visu->speed - 0.0003f) > 0)
		data_visu->speed -= 0.0003f;
}



