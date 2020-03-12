/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf_tools.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier </var/mail/ldedier>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/21 04:59:46 by ldedier           #+#    #+#             */
/*   Updated: 2020/02/21 04:59:46 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody.h"

int	get_section(struct s_elf *elf, Elf64_Shdr **section_ptr, char *sect_name)
{
	Elf64_Shdr *section;
	size_t i;

	i = 0;
	while (i < elf->header->e_shnum)
	{
		section = (Elf64_Shdr *)(elf->ptr + elf->header->e_shoff + i * elf->header->e_shentsize);
		if (is_corrupted_string_light(elf->strtable + section->sh_name, elf))
			return error_corrupted(elf);
		if (!ft_strcmp(elf->strtable + section->sh_name, sect_name))
		{
			if (is_corrupted_data(elf->ptr + section->sh_offset, section->sh_size, elf))
				return error_corrupted(elf);
			*section_ptr = section;
			return (0);
		}
		i++;
	}
	return error_no_section(elf->filename, sect_name);
}

int	write_binary_from_elf(struct s_elf *elf, char *filename)
{
	int fd;

	if ((fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0755)) < 0)
	{
		perror(filename);
		return (1);
	}
//	ft_printf("%p\n", elf->ptr);
//	ft_printf("%d\n", fd);
	write(fd, elf->ptr, elf->st.st_size);
	close(fd);
//	ft_printf("WROTE %zu\n", elf->st.st_size);
	return (0);
}
