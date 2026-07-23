#ifndef __LEM_IN__
# define __LEM_IN__

# include	"libft/libft.h"
# include	<stdio.h>
# include	<stdlib.h>
# define START_NBR -1
# define END_NBR -2

typedef struct room_s{
	int				nbr;
	int				x, y;
	bool			is_full;
	int				*links;
	struct room_s	*next;
} t_room;

typedef struct data_s{
	int		total_ants;
	t_room	*rooms;
	t_room	*start;
	t_room	*end;
} t_data;

#endif
