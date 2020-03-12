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

int	elf64_get_cave_attributes(struct s_elf *elf, size_t *cave_offset, size_t *cave_size)
{
	size_t i;
	Elf64_Phdr *text_segment_header;
	Elf64_Phdr *segment_header;
	Elf64_Phdr *next_segment_header;

	i = 0;
	*cave_size = -1;

	if ((text_segment_header = get_text_segment_header(elf)) == NULL)
		return (woody_error("could not find .text segment"));

	text_segment_header->p_flags |= PF_W;
	ft_printf("OUAI\n");
	print_elf64_program_segment_header(*text_segment_header);;
	*cave_offset = text_segment_header->p_offset + text_segment_header->p_filesz;
	while (i < elf->header->e_phnum)
	{
		segment_header = (Elf64_Phdr *)(elf->ptr + elf->header->e_phoff + i * elf->header->e_phentsize);

//		segment_header->p_flags |= PF_W | PF_R | PF_X;
		if (segment_header != text_segment_header 
			&& (segment_header->p_offset > *cave_offset)
			&& segment_header->p_offset - *cave_offset < *cave_size)
		{
			next_segment_header = segment_header;
			*cave_size = segment_header->p_offset - *cave_offset;
		}
		i++;
	}
	if (*cave_size == (size_t)-1)
		return (woody_error("could not find a loadable segment to inject our code\n"));

/*
	ft_printf("Text segment header\n");
	print_elf64_program_segment_header(*text_segment_header);

	ft_printf("//////////////////////////////////////////////////\n");
	ft_printf("//////////////////////////////////////////////////\n\n\n");

	ft_printf("Next segment header\n");
	print_elf64_program_segment_header(*next_segment_header);

	ft_printf("//////////////////////////////////////////////////\n");
	ft_printf("//////////////////////////////////////////////////\n\n\n");

	print_elf64(elf);

*/
	next_segment_header->p_flags |= PF_W;
	return (0);
}
