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

int	process_woody(struct s_elf *elf)
{
	Elf64_Phdr *segment_header;
	Elf64_Shdr *section_header;
	size_t i;

	elf->header = (Elf64_Ehdr)*(Elf64_Ehdr *)elf->ptr;
	if (ft_strncmp(ELFMAG, (char *)elf->header.e_ident, SELFMAG))
		return (not_elf_file(elf));
	print_elf64_exec_header(elf->header);
	if (is_corrupted_data(elf->ptr + elf->header.e_phoff, elf->header.e_phnum * elf->header.e_phentsize, elf))
		return (error_corrupted(elf));
	ft_printf("\n\nSegment headers (%d):\n\n", elf->header.e_phnum);

	i = 0;
	while (i < elf->header.e_phnum)
	{
		segment_header = (Elf64_Phdr *)((void *)(elf->ptr + elf->header.e_phoff + i * elf->header.e_phentsize));
		ft_printf("\nsegment #%d:\n\n", i);
		print_elf64_program_segment_header(*segment_header);
		i++;
	}

	if (is_corrupted_data(elf->ptr + elf->header.e_shoff, elf->header.e_shnum * elf->header.e_shentsize, elf))
		return (error_corrupted(elf));
	Elf64_Shdr *first_section_header;
	first_section_header = (Elf64_Shdr *)(void *)(elf->ptr + elf->header.e_shoff);
	Elf64_Shdr *strtable_section = &first_section_header[elf->header.e_shstrndx];

	printf("\n%p\n", elf->ptr);
	printf("%p\n", strtable_section);

	char *strtable= (char *)((void *)elf->ptr + strtable_section->sh_offset);

	i = 0;
	while (i < elf->header.e_shnum)
	{
		section_header = (Elf64_Shdr *)((void *)(elf->ptr + elf->header.e_shoff + i * elf->header.e_shentsize));
		if (is_corrupted_string_light(strtable + section_header->sh_name, elf))
			return (error_corrupted(elf));
		ft_printf("\nsection #%d: %s\n\n", i, strtable + section_header->sh_name);
		print_elf64_section_header(*section_header);
		i++;
	}
	return (0);
}
