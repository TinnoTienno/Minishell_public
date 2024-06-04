/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_freeiarray.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschussl <eschussl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:30:45 by eschussl          #+#    #+#             */
/*   Updated: 2024/05/26 17:44:21 by eschussl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_freeiarray(int **tab, int size)
{
	int	i;
	
	if (!tab)
		return ;
	i = 0;
	while (i < size)
	{
		free (tab[i]);
		tab[i] = NULL;
		i++;
	}
	free (tab);
}