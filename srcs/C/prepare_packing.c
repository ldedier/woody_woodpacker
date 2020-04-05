/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_packing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier </var/mail/ldedier>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/05 09:45:50 by ldedier           #+#    #+#             */
/*   Updated: 2020/04/05 09:45:50 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody.h"
#include <sys/mman.h>

void *update_shdrs_off(Elf64_Shdr *shdr, size_t shnum, size_t offset, size_t insert_len)
{
	void *first_updated = 0;

	for (uint16_t i = 0; i < shnum ; i++) {
		if (shdr->sh_offset > offset) {
			if (!first_updated)
				first_updated = shdr;
			shdr->sh_offset += insert_len;
		}
		shdr++;
	}
	return first_updated;
}

void	prepare_config(struct s_woody *woody, Elf64_Phdr *phdr, struct s_elf *elf, struct s_elf *payload)
{
	woody->file_to_pack = elf;
	woody->payload = payload;
	woody->insert_offset = phdr->p_offset + phdr->p_filesz;
	woody->bss_len = phdr->p_memsz - phdr->p_filesz;
	woody->new_startpoint_vaddr =  phdr->p_memsz + phdr->p_vaddr;
	woody->payload_start_off = woody->insert_offset + woody->bss_len;
	woody->payload_size_aligned = (payload->text_section->sh_size + 63) & ~63;
	woody->insert_size = woody->payload_size_aligned + woody->bss_len;
	woody->output_len =  elf->st.st_size + woody->insert_size + sizeof(Elf64_Shdr);
}

void	prepare_last_load(struct s_woody *woody, Elf64_Phdr *phdr)
{
	phdr->p_flags |= PF_X;
	phdr->p_filesz += woody->insert_size;
	phdr->p_memsz += woody->payload_size_aligned;
}

/*
** Copy file to new ptr with enough space to add our new section
*/
uint8_t*	create_new_bin(struct s_woody* woody)
{
	uint8_t* new_ptr;

	if (!(new_ptr = ft_memalloc(woody->output_len)))
		return(NULL);

	memmove(new_ptr, woody->file_to_pack->ptr, woody->insert_offset);
	memset(new_ptr + woody->insert_offset + woody->payload_size_aligned, 0x0, woody->bss_len);
	memmove(new_ptr + woody->insert_offset + woody->insert_size,
			woody->file_to_pack->ptr + woody->insert_offset,
			woody->file_to_pack->st.st_size - woody->insert_offset );
	return (new_ptr);
}
