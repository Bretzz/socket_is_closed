/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: topiana- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 19:04:52 by topiana-          #+#    #+#             */
/*   Updated: 2024/11/23 23:05:01 by topiana-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*node;

	node = (void *)ft_calloc(1, sizeof(t_list));
	if (!node)
		return (NULL);
	node->content = content;
	node->next = NULL;
	return (node);
}
/*
#include <stdio.h>

void    ft_lstprint(t_list *node)
{
	while(node)
	{
		write(1, node->content, 3);
		node = node->next;
	}
}

int main(void)
{
	char	*cont1 = "abc";
	char	*cont2 = "def";
	char	*cont3 = "ghi";
	t_list *lst = NULL;
	t_list *temp;

	lst = ft_lstnew(cont1);
	temp = lst;
	ft_lstadd_back(&lst, ft_lstnew(cont2));
	//ft_lstadd_back(&lst, ft_lstnew(cont3));
	ft_lstprint(lst);
	return (0);
}*/
