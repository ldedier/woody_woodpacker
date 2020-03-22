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

void test_hash(char *str, char *key)
{
	size_t i = 0;
	char *content = ft_strdup(str);
	size_t len = strlen(content);

	(void)key;
	hash(content, key, len);
	while (i < len)
	{
		ft_printf("%.2hhX", content[i]);
		i++;
	}
	ft_printf("\n%s\n ", content);
	free(content);
}

void test_hash_ptr(char *ptr, char *key, size_t size)
{
	size_t i = 0;

	hash(ptr, key, size);
	while (i < size)
	{
		ft_printf("%.2hhX", ptr[i]);
		i++;
	}
	ft_printf("\n\n");
}

char	random_printable()
{
	return (rand() * (176 - 41) + 41);
}

char	*generate_key(size_t len)
{
	size_t i;
	char *res;

	if (!(res = malloc(len + 1)))
		return (NULL);
	i = 0;
	while (i < len)
		res[i++] = (random() & 0xff) | 0x1;
	res[i] = 0;
	return (res);
}

void	print_string_hexa(char *str)
{
	size_t i;

	i = 0;
	while (str[i])
	{
		ft_printf("%.2hhX", str[i]);
		i++;
	}
}
/*
int	pack_from_offset(struct s_elf *elf, struct s_elf *payload, size_t offset)
{
	size_t		old_entry;
	char		*key;

	old_entry = elf->header->e_entry;
	if (elf->header->e_type == ET_DYN)
	{
		elf->header->e_entry = offset;
		if (patch_target(elf->ptr + offset, payload->text_section->sh_size, 0x1111111111111111, old_entry - elf->header->e_entry - 5))
			return (woody_error("could not find payload jmp argument"));
		if (patch_target(elf->ptr + offset, payload->text_section->sh_size, 0x3333333333333333, elf->text_section->sh_offset - elf->header->e_entry - 5))
			return (woody_error("could not find payload jmp argument"));
	}
	else if (elf->header->e_type == ET_EXEC)
	{
		elf->header->e_entry = elf->text_section->sh_addr - elf->text_section->sh_offset + offset;
		if (patch_target(elf->ptr + offset, payload->text_section->sh_size, 0x2222222222222222, 0))
			return (woody_error("could not find payload jmp argument"));
		if (patch_target(elf->ptr + offset, payload->text_section->sh_size, 0x1111111111111111, old_entry))
			return (woody_error("could not find payload jmp argument"));
		if (patch_target(elf->ptr + offset, payload->text_section->sh_size, 0x3333333333333333, elf->text_section->sh_addr))
			return (woody_error("could not find payload jmp argument"));
	}
	else
		return woody_error("this elf is not a valid executable elf");
	if (!(key = generate_key(sizeof(KEY_PLACEHOLDER))))
		return (woody_error("malloc error"));
	if (patch_target(elf->ptr + offset, payload->text_section->sh_size, 0x4444444444444444, elf->text_section->sh_size))
		return (woody_error("could not find payload jmp argument"));
	if (patch_target_string(elf->ptr + offset, payload->text_section->sh_size, KEY_PLACEHOLDER, key))
		return (woody_error("could not find key string placeholder"));
	hash(elf->ptr + elf->text_section->sh_offset, key, elf->text_section->sh_size);
	return write_binary_from_elf(elf, PACKED_NAME);
}
*/

int	pack(struct s_elf *elf, struct s_elf *payload, size_t offset)
{
	size_t		old_entry;
	char		*key;

	old_entry = elf->header->e_entry;
	if (elf->header->e_type == ET_DYN)
	{
		elf->header->e_entry = offset;
		if (patch_target(payload->ptr + payload->text_section->sh_offset, payload->text_section->sh_size, 0x1111111111111111, old_entry - elf->header->e_entry - 5))
			return (woody_error("could not find payload jmp argument"));
		if (patch_target(payload->ptr + payload->text_section->sh_offset, payload->text_section->sh_size, 0x3333333333333333, elf->text_section->sh_offset - elf->header->e_entry - 5))
			return (woody_error("could not find payload jmp argument"));
	}
	else if (elf->header->e_type == ET_EXEC)
	{
	/*
		elf->header->e_entry = elf->text_section->sh_addr - elf->text_section->sh_offset + offset;
		if (patch_target(elf->ptr + offset, payload->text_section->sh_size, 0x2222222222222222, 0))
			return (woody_error("could not find payload jmp argument"));
		if (patch_target(elf->ptr + offset, payload->text_section->sh_size, 0x1111111111111111, old_entry))
			return (woody_error("could not find payload jmp argument"));
		if (patch_target(elf->ptr + offset, payload->text_section->sh_size, 0x3333333333333333, elf->text_section->sh_addr))
			return (woody_error("could not find payload jmp argument"));
	*/
	}
	else
		return woody_error("this elf is not a valid executable elf");
	if (!(key = generate_key(sizeof(KEY_PLACEHOLDER))))
		return (woody_error("malloc error"));
	if (patch_target(payload->ptr + payload->text_section->sh_offset, payload->text_section->sh_size, 0x4444444444444444, elf->text_section->sh_size))
		return (woody_error("could not find payload jmp argument"));
	if (patch_target_string(payload->ptr + payload->text_section->sh_offset, payload->text_section->sh_size, KEY_PLACEHOLDER, key))
		return (woody_error("could not find key string placeholder"));
	hash(elf->ptr + elf->text_section->sh_offset, key, elf->text_section->sh_size);
	return write_binary_from_elf_and_payload(elf, payload, PACKED_NAME);
}

void	patch_note_segment(Elf64_Phdr	*note_segment,
	size_t offset, struct s_elf *payload)
{
	note_segment->p_type = PT_LOAD;
	note_segment->p_flags |= (PF_X | PF_R);
	note_segment->p_offset = offset;
	note_segment->p_vaddr = offset;
	note_segment->p_paddr = offset;
	note_segment->p_filesz = payload->text_section->sh_size;
	note_segment->p_memsz = payload->text_section->sh_size;
	note_segment->p_align = 0;
}

int	process_woody(struct s_elf *elf, struct s_elf *payload)
{
	Elf64_Phdr	*note_segment;
	size_t		offset;

	offset = elf->st.st_size;
	ft_printf("OFFSET: %d\n", offset);
	printf("Payload size: %zu\n", payload->text_section->sh_size);
	if (!(note_segment = get_note_segment_header(elf)))
		return (woody_error("could not find the note header segment"));
	patch_note_segment(note_segment, offset, payload);
	print_elf64_program_segment_header(*note_segment);
	return (pack(elf, payload, offset));
}
