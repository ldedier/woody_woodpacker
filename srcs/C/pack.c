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

int	get_text(struct s_elf *elf, size_t *text_size, unsigned char **data)
{
	Elf64_Shdr *section;
	size_t i;

	i = 0;
	while (i < elf->header->e_shnum)
	{
		section = (Elf64_Shdr *)(elf->ptr + elf->header->e_shoff + i * elf->header->e_shentsize);
		if (is_corrupted_string_light(elf->strtable + section->sh_name, elf))
			return error_corrupted(elf);
		if (!ft_strcmp(elf->strtable + section->sh_name, ".text"))
		{
	//		print_elf64_section_header(*section);
			*text_size = section->sh_size;
			if (is_corrupted_data(elf->ptr + section->sh_offset, section->sh_size, elf))
				return error_corrupted(elf);
			*data = elf->ptr + section->sh_offset;
			return (0);
		}
		i++;
	}
	return (1);
}

size_t		align(size_t x, size_t n)
{
	if (x == 0)
		return (n);
	else
		return ((x + n - 1) & ~(n - 1));
}

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
	size_t		payload_size;
	size_t		old_entry;
	unsigned char	*data;
	int 		fd;

	if (get_text(payload, &payload_size, &data))
		return (1);
	printf("Payload size: %zu\n", payload_size);
	if (elf64_get_cave_attributes(elf, &cave.offset, &cave.size))
		return (1);
	if (cave.size < payload_size)
		return (woody_error("could not find a suitable part of binary to be injected\n"));
	old_entry = elf->header->e_entry;
	print_cave(cave);
	printf("old entry offset : %zu\n", elf->header->e_entry);
	printf("old entry offset : %#lx\n", elf->header->e_entry);
//	print_some(elf->ptr + elf->header->e_entry);
	ft_memcpy(elf->ptr + cave.offset, data, payload_size);
	printf("%ld\n", (void *)data - elf->ptr);
	if (elf->header->e_type == ET_DYN)
	{
		elf->header->e_entry = cave.offset;
		ft_memcpy(elf->ptr + cave.offset, data, payload_size);
		ft_printf("DYN (shared object file)\n");
		if (patch_target(elf->ptr + cave.offset, payload_size, 0x1111111111111111, old_entry - elf->header->e_entry - 5))
			return (woody_error("could not find payload jmp argument"));
	}
	else if (elf->header->e_type == ET_EXEC)
	{
		elf->header->e_entry = (elf->ptr - (void *)data) + cave.offset + elf->header->e_entry; // TO DO
		ft_printf("EXEC (executablefile)\n");
		if (patch_target(elf->ptr + cave.offset, payload_size, 0x1111111111111111, old_entry))
			return (woody_error("could not find payload jmp argument"));
		if (patch_target(elf->ptr + cave.offset, payload_size, 0x2222222222222222, 0))
			return (woody_error("could not find payload jmp argument"));
	}
	else
		return woody_error("this elf is not a valid executable elf");
//	print_some(elf->ptr + elf->header->e_entry);
	printf("new entry offset : %zu\n", elf->header->e_entry);
	if ((fd = open(PACKED_NAME, O_CREAT | O_WRONLY | O_TRUNC, 0755)) < 0)
	{
		perror("open");
		return (1);
	}
	write(fd, elf->ptr, elf->st.st_size);
	close(fd);
	return (0);
}
