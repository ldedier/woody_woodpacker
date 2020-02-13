/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pack.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/07 17:38:15 by ldedier           #+#    #+#             */
/*   Updated: 2020/02/07 17:38:15 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody.h"

int	identify_elf(struct s_elf *elf)
{
	elf->header = (Elf64_Ehdr)*(Elf64_Ehdr *)elf->ptr;
	if (ft_strncmp(ELFMAG, (char *)elf->header.e_ident, SELFMAG))
		return (not_elf_file(elf));
	return (0);
}

int	elf_is_corrupted(struct s_elf *elf)
{
	if (is_corrupted_data(elf->ptr + elf->header.e_phoff, elf->header.e_phnum * elf->header.e_phentsize, elf))
		return (1);
	if (is_corrupted_data(elf->ptr + elf->header.e_shoff, elf->header.e_shnum * elf->header.e_shentsize, elf))
		return (1);
	return (0);
}

int	process_woody(struct s_elf *elf)
{
	struct s_cave cave;

	if (identify_elf(elf))
		return (1);
	if (elf_is_corrupted(elf))
		return (error_corrupted(elf));
	if (elf64_get_cave_attributes(elf, &cave.offset, &cave.size))
		return (1);
	if (cave.size < PAYLOAD_SIZE)
		return (woody_error("could not find a suitable part of binary to be injected\n"));
	print_cave(cave);
	return (0);
}


