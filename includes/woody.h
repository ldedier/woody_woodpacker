/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   woody.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/01 20:09:52 by ldedier           #+#    #+#             */
/*   Updated: 2020/02/01 20:09:52 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOODY_H
# define WOODY_H

# include <elf.h>
# include <unistd.h>
# include <fcntl.h>
# include "libft.h"

struct s_elf
{
	void *ptr;
	char *filename;
	struct stat st;
	off_t size;
};

int			init(struct s_elf *elf, char *filename);
void			free_all(struct s_elf *elf);
#endif
