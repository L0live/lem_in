#ifndef 	__LEM_IN_BONUS__
# define	__LEM_IN_BONUS__

// For the bonus part, we will use OpenGL
# include	<glad/include/glad/glad.h>
# include	<GLFW/glfw3.h>
# include   <GL/freeglut.h>
# include "lem_in.h"
# include "font8x8.h"
# include "linmath.h"


# ifndef M_PI
#  define M_PI 3.1415926535897932384626433
# endif

typedef struct  gl_object_s{
	GLuint	vertex_array;
	GLuint	vertex_buffer;
	GLuint	program;
	GLint	mvp_location;	//model_view_projection_location
	float	*vertices;
	int		vertices_size;

	int		nbSegment;
	GLint	*first;
	GLint	*count;
} t_gl_object;

typedef struct data_visu_s{
	GLFWwindow  *window;
	t_gl_object	gl_objects[4];
	t_data		data;
	int		objSize[4];
	float	offset;
} t_data_visu;


//rooms.c
void	set_rooms(t_data_visu *data_visu);
#endif
