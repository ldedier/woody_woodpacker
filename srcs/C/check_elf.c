/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_elf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/14 03:16:51 by ldedier           #+#    #+#             */
/*   Updated: 2020/04/08 13:20:18 by niragne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody.h"

int	identify_elf(struct s_elf *elf)
{
	elf->header = (Elf64_Ehdr*)(Elf64_Ehdr *)elf->ptr;
	if (ft_strncmp(ELFMAG, (char *)elf->header->e_ident, SELFMAG))
		return (not_elf_file(elf));
	return (0);
}

int	elf_is_corrupted(struct s_elf *elf)
{
	Elf64_Shdr *first_section_header;
	Elf64_Shdr *strtable_section_header;

	if (is_corrupted_data(elf->ptr + elf->header->e_phoff,
		elf->header->e_phnum * elf->header->e_phentsize, elf))
		return (1);
	if (is_corrupted_data(elf->ptr + elf->header->e_shoff,
		elf->header->e_shnum * elf->header->e_shentsize, elf))
		return (1);
	if (elf->header->e_shnum == 0
		|| elf->header->e_shstrndx >= elf->header->e_shnum)
		return (1);
	first_section_header = (Elf64_Shdr *)(void *)(elf->ptr + elf->header->e_shoff);
	strtable_section_header = &first_section_header[elf->header->e_shstrndx];
	elf->strtable = (char *)((void *)elf->ptr + strtable_section_header->sh_offset);
	if (get_section(elf, &elf->text_section, ".text"))
		return (1);
	return (0);
}

/*
** Checks wether an elf64 has a valid header and if its section and segments
** headers are not corrupted
*/

int check_elf(struct s_elf *elf)
{
	if (identify_elf(elf))
		return (1);
	if (elf_is_corrupted(elf))
		return (error_corrupted(elf));
	return (0);
}
