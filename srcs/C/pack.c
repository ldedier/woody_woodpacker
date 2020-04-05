/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pack.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/07 17:38:15 by ldedier           #+#    #+#             */
/*   Updated: 2020/04/04 15:40:27 by niragne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody.h"
#include <sys/mman.h>

int	insert_and_patch_payload(struct s_woody* woody, uint8_t* new_ptr, Elf64_Ehdr* new_hdr)
{
	memmove(new_ptr + woody->payload_start_off, woody->payload->ptr + woody->payload->text_section->sh_offset, woody->payload->text_section->sh_size);
	new_hdr->e_entry = woody->new_startpoint_vaddr;

	if (woody->file_to_pack->header->e_type == ET_DYN)
	{
		if (patch_target(new_ptr + woody->payload_start_off, woody->payload->text_section->sh_size, 0x1111111111111111, woody->file_to_pack->header->e_entry - new_hdr->e_entry - 5))
			return (woody_error("could not find payload jmp argument"));
		if (patch_target(new_ptr + woody->payload_start_off, woody->payload->text_section->sh_size, 0x3333333333333333, woody->file_to_pack->text_section->sh_offset - new_hdr->e_entry - 5))
			return (woody_error("could not find payload jmp argument"));
	}
	else if (woody->file_to_pack->header->e_type == ET_EXEC)
	{
		if (patch_target(new_ptr + woody->payload_start_off, woody->payload->text_section->sh_size, 0x2222222222222222, 0))
			return (woody_error("could not find payload jmp argument"));
		if (patch_target(new_ptr + woody->payload_start_off, woody->payload->text_section->sh_size, 0x1111111111111111, woody->file_to_pack->header->e_entry))
			return (woody_error("could not find payload jmp argument"));
		if (patch_target(new_ptr + woody->payload_start_off, woody->payload->text_section->sh_size, 0x3333333333333333, woody->file_to_pack->text_section->sh_addr))
			return (woody_error("could not find payload jmp argument"));
	}
	else
		return (woody_error("unknown elf type"));
	return (0);
}

int	create_packed(struct s_woody* woody, uint8_t* new_ptr)
{
	int fd;
	if ((fd = open(PACKED_NAME, O_CREAT | O_WRONLY | O_TRUNC, 0755)) < 0)
	{
		perror(PACKED_NAME);
		return (1);
	}
	if (write(fd, new_ptr, woody->output_len) == -1)
	{
		perror(PACKED_NAME);
		return (1);
	}	
	if (close(fd) == -1)
	{
		perror(PACKED_NAME);
		return (1);
	}
	return (0);
}

int	process_woody(struct s_elf *elf, struct s_elf *payload)
{
	char		*key;
	Elf64_Phdr	*last_load_segment_header;
	struct s_woody	 woody;
	uint8_t		*new_ptr;
	Elf64_Ehdr	*new_hdr;

	if (!(last_load_segment_header = get_last_loaded_segment_header(elf)))
		return (woody_error("not found a single load segment"));
	prepare_config(&woody, last_load_segment_header, elf, payload);
	prepare_last_load(&woody, last_load_segment_header);
	if (!(new_ptr = create_new_bin(&woody)))
		return (woody_error("failed to allocate enough memory for new file"));
	new_hdr = (Elf64_Ehdr*)new_ptr;
	if (new_hdr->e_shoff > woody.insert_offset)
		new_hdr->e_shoff += woody.insert_size;
	
	if (insert_and_patch_payload(&woody, new_ptr, new_hdr))
		return (woody_error_free("error while patching payload", new_ptr));
	
	if (generate_and_patch_key(&woody, new_ptr, &key))
		return (woody_error_free("error while generating or patching key", new_ptr));
	hash((char*)(new_ptr + elf->text_section->sh_offset), key, elf->text_section->sh_size);
	free(key);
	if (create_packed(&woody, new_ptr))
		return (woody_error_free("error while creating new file", new_ptr));
	free(new_ptr);
	return (0);
}

