/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ft_lstprint.c										:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: natrij and yanolive						+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2026/23/06 11:51:56 by natrij and yanolive  #+#	#+#			 */
/*																			*/
/* ************************************************************************** */

#include "libft.h"

void	ft_lstprint(t_list *lst){
	if (!lst)
		return ;
	
	while (lst){
		ft_printf("%s\n", (char *)lst->content);
		lst = lst->next;
	}
	return ;
};
