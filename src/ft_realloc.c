/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: totommi <totommi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 22:40:03 by topiana-          #+#    #+#             */
/*   Updated: 2025/03/12 12:13:45 by totommi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket_is_closed.h"

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size);

/*
DSCRIPTION
	The realloc() function changes the size of the memory block pointed to  by  ptr
	to  size  bytes.  The contents will be unchanged in the range from the start of
	the region up to the minimum of the old and new sizes.   If  the  new  size  is
	larger  than the old size, the added memory will not be initialized.  If ptr is
	NULL, then the call is equivalent to malloc(size), for all values of  size;  if
	size  is  equal  to  zero,  and ptr is not NULL, then the call is equivalent to
	free(ptr).  Unless ptr is NULL, it must have been returned by an  earlier  call
	to  malloc(),  calloc(),  or  realloc().   If  the area pointed to was moved, a
	free(ptr) is done.

RETURN VALUE
	The  realloc()  function returns a pointer to the newly allocated memory, which
	is suitably aligned for any built-in type, or NULL if the request failed.   The
	returned  pointer may be the same as ptr if the allocation was not moved (e.g.,
	there was room to expand the allocation in-place), or different from ptr if the
	allocation  was moved to a new address.  If size was equal to 0, either NULL or
	a pointer suitable to be passed to free() is returned.  If realloc() fails, the
	original block is left untouched; it is not freed or moved.
*/
void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*ret;

	if (ptr == NULL)
		return(malloc(new_size));
	else if (ptr != NULL && new_size == 0)
		return (free(ptr), NULL);
	ret = malloc(new_size);
	if (ret == NULL)
		return (NULL);
	ft_memmove(ret, ptr, old_size);
	return (ret);
}
