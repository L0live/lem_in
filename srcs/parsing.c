#include "../includes/lem_in.h"

int    ft_isstriter(const char *str, int (*f)(int)){
    while (*str){
        if (!f(*str))
            return (0);
        str++;
    }
    return (1);
}

int    pars_room_nbr(char *room_nbr){
    if (ft_isstriter(room_nbr, &ft_isdigit)){
        return (ft_atoi(room_nbr));
    }
    else if (ft_strcmp(room_nbr, "start")) {
        return (START_NBR);
    }
    else if (ft_strcmp(room_nbr, "end")) {
        return (END_NBR);
    }
    else {
        return (-3);
    }
}

int    handle_new_room(t_data *data, char *line, int is_special){
    int nbr, x, y;
    char    **split_line = ft_split(line, ' ');

    if (!split_line || !split_line[0] || !split_line[1] || !split_line[2] || split_line[3])
        return (1);

    nbr = pars_room_nbr(split_line[0]);
    if (nbr == -3)
        return (1);

    if (!ft_isstriter(split_line[1], &ft_isdigit) || !ft_isstriter(split_line[2], &ft_isdigit))
        return (1);

    x = ft_atoi(split_line[1]);
    y = ft_atoi(split_line[2]);
    room_addnew(&data->rooms, nbr, x, y);

    if (is_special == START_NBR)
    data->start = room_getby_nbr(data->rooms, nbr);
    else if (is_special == END_NBR)
    data->end = room_getby_nbr(data->rooms, nbr);
    return (0);
}

int    parsing(t_list *stdin_content, t_data *data){
    t_list *current = stdin_content;
    char *line = current->content;
    bool    is_links_part = false;

    if (!ft_isstriter(line, &ft_isdigit)){
        return (1);
    }
    data->total_ants = ft_atoi(line);
    current = current->next;
    while (current != NULL) {
        line = current->content;
            if (line[0] == '#') {
            if (line[1] == '#') {
                if (ft_strcmp(line, "##start")) {
                    current = current->next;
                    if (handle_new_room(data, current->content, START_NBR)) {
                        ft_printf("erreur : !handle_new_room(data, current->content, START_NBR");
                        return (1);
                    }
                }
                else if (ft_strcmp(line, "##end")) {
                    current = current->next;
                    if (handle_new_room(data, current->content, END_NBR)) {
                        ft_printf("erreur : !handle_new_room(data, current->content, END_NBR");
                        return (1);
                    }
                }
            }
        }
        else if (ft_strchr(line, '-')) {
            is_links_part = true;
            char **split_line = ft_split(line, '-');
            if (!split_line || !split_line[0] || !split_line[1] || split_line[2]) {
                return (1);
            }
            int room_nbr = pars_room_nbr(split_line[0]);
            if (room_nbr == -3) {
                return (1);
            }
            int link_nbr = pars_room_nbr(split_line[1]);
            if (link_nbr == -3) {
                return (1);
            }
            room_addlink(data->rooms, room_nbr, link_nbr);
        }
        else if (!is_links_part) {
            if (handle_new_room(data, line, 0)) {
                ft_printf("Erreur not links part");
                return (1);
            }
        }
        current = current->next;
    }
    if (!data->rooms || !data->start || !data->end) {
        return (1);
    }
    return (0);
}