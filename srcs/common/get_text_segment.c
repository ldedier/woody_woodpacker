/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_text_segment.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier </var/mail/ldedier>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/08 07:05:00 by ldedier           #+#    #+#             */
/*   Updated: 2020/02/08 07:05:00 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody.h"

Elf64_Phdr *get_text_segment_header(struct s_elf *elf)
{
	Elf64_Phdr *segment_header;
	size_t i;

	i = 0;
	while (i < elf->header.e_phnum)
	{
		segment_header = (Elf64_Phdr *)((void *)(elf->ptr
			+ elf->header.e_phoff + i * elf->header.e_phentsize));
		if (segment_header->p_type == PT_LOAD 
			&& segment_header->p_flags & (PF_X | PF_W))
		{
			return segment_header;
		}
		i++;
	}
	return (NULL);
}
