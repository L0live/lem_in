#ifndef 	__LEM_IN_BONUS__
# define	__LEM_IN_BONUS__

// For the bonus part, we will use OpenGL
# include	<glad/include/glad/glad.h>
# include	<GLFW/glfw3.h>
// # include   <GL/freeglut.h>
# include "lem_in.h"
# include "font8x8.h"
# include "linmath.h"

//freetype
#include <ft2build.h>
#include FT_FREETYPE_H

# define OBJS_SIZE 4
# define BASIC_OFFSET 0.5f
# define BASIC_RADIUS 0.02f
# define PIPE_WIDTH 0.005f
# define ANT_STEP 0.005f

# ifndef M_PI
#  define M_PI 3.1415926535897932384626433
# endif

typedef enum e_object_type {PIPE, ANT, ROOM, TEXT} t_object_type;

typedef struct s_glyph{
	GLuint	texture_id;
	int		width;
	int		height;
	int		bearing_x;
	int		bearing_y;
	GLuint	advance;
} t_glyph;

typedef struct  gl_object_s{
	GLuint	vertex_array;
	GLuint	vertex_buffer;
	GLuint	program;

	GLint	mvp_location;	//model_view_projection_location
	GLint	color_location;

	// pour les texte
	GLint	texture_location;
	GLint	text_color_location;
	GLint	texture_direction;
	
	GLint	ant_position_location;

	float	*vertices;
	int		vertices_size;

	int		nbSegment;
	GLint	*first;
	GLint	*count;

	// color
	GLuint	color_buffer;
	GLfloat	*colors;
	size_t	colors_size;
} t_gl_object;


typedef struct	s_ant{
	int		id;
	float	x;
	float	y;
	t_room	*current_room;
} t_ant;

typedef	struct s_actions{
	t_ant				*ant;
	t_room				*room;
	struct s_actions 	*next;
} t_actions;

typedef struct data_visu_s{
	GLFWwindow  *window;
	t_gl_object	gl_objects[OBJS_SIZE];
	t_data		data;
	int			objSize[OBJS_SIZE];
	float		width;
	float		height;
	t_glyph		glyphs[128];

	GLuint		antTexture;
	t_ant		*ants;
	t_list		*actions_lines; // content type : t_actions
	
	float		speed;
} t_data_visu;

typedef struct s_loop_data{
	bool	is_paused;
	bool	key_paused;
	bool	key_restart;
	
	int		current_action;
	
	mat4x4	mvp;
} t_loop_data;

//rooms.c
int		set_rooms(t_data_visu *data_visu, t_gl_object *obj, int obj_size);
int		valid_room_coordonate(float x, float y, t_room	*cible);

//pipe.c
int		set_pipe(t_data_visu *data_visu, t_gl_object *obj, int obj_size);
int		get_pipe_size(t_room *rooms);
void	room_to_gl(t_room *room, t_data_visu *data_visu, float *x, float *y);

//polices.c
void	set_font(t_data_visu *data_visu);
void	render_text(t_data_visu	*data_visu, const char	*text, GLfloat x, GLfloat y, GLfloat scale, GLfloat r, GLfloat g, GLfloat b, mat4x4 mvp);

//ants.c
void	set_ants(t_data_visu *data_visu);

//shaders.c
unsigned int createProgram(t_object_type shader_type);

//gl.c
int		init_glfw(t_data_visu *data);
int		gl_init(t_data_visu *data_visu);
void	set_gl_objects(t_data_visu *data_visu);
void 	cleanup_opengl(GLuint vertex_array, GLuint vertex_buffer, GLuint program);
void	free_gl_objects(t_gl_object *gl_objects);

//gl_init.c
/*
** Crée et configure le VAO/VBO d'un objet OpenGL selon son type :
**   - ANT  : vertices entrelacés (pos vec2 + couleur vec3 + texCoord vec2)
**   - TEXT : buffer dynamique vide (quad vec4 : pos + texCoord) rempli au rendu
**   - autre (ROOM, PIPE...) : positions vec2 + buffer de couleurs vec3 séparé
** Le VAO et le VBO sont unbind à la fin de la fonction.
**
** gl_obj : objet dont on initialise les buffers
** type   : type de l'objet (t_object_type : ROOM, PIPE, ANT, TEXT)
*/
void	set_objects_buffers(t_gl_object *gl_obj, size_t type);

/*
** Récupère les emplacements (locations) des uniforms du shader de l'objet :
**   - TEXT : "text", "textColor"
**   - ANT  : "MVP", "antDirection", "antPosition", "antTexture"
**            (active aussi le blending pour la transparence et
**             lie le sampler de texture à l'unité 0)
**   - autre : "MVP", "objectColor"
** Le programme shader (gl_obj->program) doit déjà être créé.
**
** gl_obj : objet dont on récupère les uniforms
** type   : type de l'objet (t_object_type : ROOM, PIPE, ANT, TEXT)
*/
void	set_objects_uniform_location(t_gl_object *gl_obj, size_t type);


//actions.c
int		pars_actions(t_list *stdin_content, t_data_visu *data_visu);
void	free_actions_lines(t_list **lines);

//ants.c

/* Initialise les positions des ants a start*/
void	init_position(t_data_visu *data_visu);


//debug_bonus.c
/* Print les actions des fourmis par ligne*/
void	print_action_list(t_list *actions_list);


//free_bonus.c
/* */
void	cleanup_visu(t_data_visu *data_visu);

//draw.c
void	draw_rooms(t_data_visu *data_visu, const GLfloat *mvp);
void	draw_pipes(t_gl_object *gl_obj, int size, const GLfloat *mvp);
void	draw_ants(t_data_visu *data_visu, t_loop_data *loop_data, float (*ant_direction)[2]);

//keyboard.c
void	keyboard_pause(t_data_visu *data_visu, t_loop_data *loop);
bool	keyboard_restart(t_data_visu *data_visu, t_loop_data *loop);
void	keyboard_plus(t_data_visu *data_visu);
void	keyboard_minus(t_data_visu *data_visu);

#endif
