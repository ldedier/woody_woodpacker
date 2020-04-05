/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/07 19:14:33 by ldedier           #+#    #+#             */
/*   Updated: 2020/02/07 19:14:33 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody.h"

int	error_no_section(char *filename, char *sect_name)
{
	ft_dprintf(2, "elf %s does not have a \"%s\" section\n", filename, sect_name);
	return (1);
}

int     is_corrupted_data(void *address, size_t size, struct s_elf *elf)
{
	if (address + size > elf->ptr + elf->st.st_size
			|| address < elf->ptr)
		return (1);
	return (0);
}

int     is_corrupted_offset(uint64_t offset, uint64_t size, struct s_elf *elf)
{
	if (offset + size > (uint64_t)elf->st.st_size)
		return (1);
	return (0);
}

int	not_elf_file(struct s_elf *elf)
{
	ft_dprintf(2, "%s could not be identified as an ELF file\n",
			elf->filename);
	return (1);
}

int error_corrupted(struct s_elf *elf)
{
	ft_dprintf(2, "ELF %s is corrupted\n", elf->filename);
	return (1);
}

int     is_corrupted_string_light(char *str, struct s_elf *elf)
{
	int     i;

	if ((void *)str >= elf->ptr + elf->st.st_size
			|| (void *)str < elf->ptr)
		return (1);
	i = 0;
	while ((void *)str + i < elf->ptr + elf->st.st_size - 1 && str[i])
		i++;
	return (str[i] != 0);
}

int     is_corrupted_string(char *str, struct s_elf *elf, int *len)
{
	int     i;

	*len = -1;
	if ((void *)str >= elf->ptr + elf->st.st_size
			|| (void *)str < elf->ptr)
		return (1);
	i = 0;
	while ((void *)str + i < elf->ptr + elf->st.st_size - 1 && str[i])
		i++;
	if (str[i])
	{
		*len = i + 1;
		return (1);
	}
	return (0);
}

int woody_error(char *msg)
{
	ft_dprintf(2, "%s\n", msg);
	return (1);
}

int woody_error_free(char *msg, void *ptr)
{
	free(ptr);
	return (woody_error(msg));
}
