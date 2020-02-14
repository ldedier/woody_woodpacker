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
	while (i < elf->header.e_shnum)
	{
		section = (Elf64_Shdr *)(elf->ptr + elf->header.e_shoff + i * elf->header.e_shentsize);
		if (is_corrupted_string_light(elf->strtable + section->sh_name, elf))
			return error_corrupted(elf);
		if (!ft_strcmp(elf->strtable + section->sh_name, ".text"))
		{
			printf("%s WWWOOOWW\n", elf->strtable + section->sh_name);	
			print_elf64_section_header(*section);
			*text_size = 42;
			*data = NULL;
		}
		i++;
	}
	return (1);
}

int	process_woody(struct s_elf *elf, struct s_elf *payload)
{
	struct s_cave cave;
	size_t	payload_size;
	unsigned char	*data;

	if (check_elf(payload) || check_elf(elf))
		return (1);
	if (get_text(payload, &payload_size, &data))
		return (1);
	printf("Payload size: %zu\n", payload_size);
	if (elf64_get_cave_attributes(elf, &cave.offset, &cave.size))
		return (1);
	if (cave.size < payload_size)
		return (woody_error("could not find a suitable part of binary to be injected\n"));
	print_cave(cave);
	return (0);
}


