/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/01 18:32:07 by ldedier           #+#    #+#             */
/*   Updated: 2020/02/01 18:32:07 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <sys/stat.h>
# include "woody.h"
# include <sys/mman.h>

int	init(struct s_elf *elf, char *filename)
{
	int fd;

	elf->filename = filename;
	if ((fd = open(filename, O_RDONLY)) < 0)
	{
		ft_dprintf(2, "error opening '%s'\n", filename);
		return (1);
	}
	if ((fstat(fd, &elf->st)) < 0)
	{
		ft_dprintf(2, "error fstat on file '%s'\n", filename);
		close(fd);
		return (1);
	}
	if ((elf->ptr = mmap(0, elf->st.st_size,
					PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
	{
		ft_dprintf(2, "could not map file '%s'\n", filename);
		close(fd);
		return (1);
	}
	return (0);
}
