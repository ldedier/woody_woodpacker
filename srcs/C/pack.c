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

int	print_some(char *data)
{
	size_t i;
	i = 0;
	while (i < 20)
	{
		printf("%02x\n", data[i]);
		i++;
	}
	return (1);
}

int	patch_target(unsigned char *data, size_t size, long pattern, long to_remplace)
{
	size_t i;
	long *to_compare;
	i = 0;

	while (i < size - sizeof(long))
	{
		to_compare = (long *)(data + i);
		if (*to_compare == pattern)
		{
			printf("jump %lx\n", *to_compare);
			*to_compare = to_remplace;
			printf("jump %ld\n", *to_compare);
			return (0);
		}
		i++;
	}
	return (1);
}

int	process_woody(struct s_elf *elf, struct s_elf *payload)
{
	struct s_cave	cave;
	size_t		old_entry;

	old_entry = elf->header->e_entry;
	printf("Payload size: %zu\n", payload->text_section->sh_size);
	if (elf64_get_cave_attributes(elf, &cave.offset, &cave.size))
		return (1);
	if (cave.size < payload->text_section->sh_size)
		return (woody_error("could not find a suitable part of binary to be injected in\n"));
	print_cave(cave);
	printf("old entry : %zu\n", elf->header->e_entry);
	ft_memcpy(elf->ptr + cave.offset,
		payload->ptr + payload->text_section->sh_offset, payload->text_section->sh_size);
	if (elf->header->e_type == ET_DYN)
	{
		ft_printf("DYN (shared object file)\n");
		elf->header->e_entry = cave.offset;
		if (patch_target(elf->ptr + cave.offset, payload->text_section->sh_size, 0x1111111111111111, old_entry - elf->header->e_entry - 5))
			return (woody_error("could not find payload jmp argument"));
		if (patch_target(elf->ptr + cave.offset, payload->text_section->sh_size, 0x3333333333333333, elf->text_section->sh_offset - elf->header->e_entry - 5))
			return (woody_error("could not find payload jmp argument"));
	}
	else if (elf->header->e_type == ET_EXEC)
	{
		ft_printf("EXEC (executablefile)\n");
		elf->header->e_entry = elf->header->e_entry + (cave.offset - elf->text_section->sh_offset);
		if (patch_target(elf->ptr + cave.offset, payload->text_section->sh_size, 0x2222222222222222, 0))
			return (woody_error("could not find payload jmp argument"));
		if (patch_target(elf->ptr + cave.offset, payload->text_section->sh_size, 0x1111111111111111, old_entry))
			return (woody_error("could not find payload jmp argument"));
		if (patch_target(elf->ptr + cave.offset, payload->text_section->sh_size, 0x3333333333333333, elf->text_section->sh_addr))
			return (woody_error("could not find payload jmp argument"));
	}
	else
		return woody_error("this elf is not a valid executable elf");
	printf("OFFSET: %ld\n", elf->text_section->sh_offset);
	if (patch_target(elf->ptr + cave.offset, payload->text_section->sh_size, 0x4444444444444444, elf->text_section->sh_size))
		return (woody_error("could not find payload jmp argument"));
	printf("new entry offset : %zu\n", elf->header->e_entry);
	hash(elf->ptr + elf->text_section->sh_offset, elf->text_section->sh_size);
	return write_binary_from_elf(elf, PACKED_NAME);
}
