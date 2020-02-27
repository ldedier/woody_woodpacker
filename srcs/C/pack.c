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

int	process_woody(struct s_elf *elf, struct s_elf *payload)
{
	struct s_cave	cave;
	size_t		old_entry;
	char		*key;

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
	if (!(key = generate_key(sizeof(KEY_PLACEHOLDER))))
		return (woody_error("malloc error"));
	printf("OFFSET: %ld\n", elf->text_section->sh_offset);
	printf("SIZE: %zu\n", sizeof(KEY_PLACEHOLDER));
	if (patch_target(elf->ptr + cave.offset, payload->text_section->sh_size, 0x4444444444444444, elf->text_section->sh_size))
		return (woody_error("could not find payload jmp argument"));
	if (patch_target_string(elf->ptr + cave.offset, payload->text_section->sh_size, KEY_PLACEHOLDER, key))
		return (woody_error("could not find key string placeholder"));
	ft_printf("size: %zu\n", elf->text_section->sh_size);
	printf("new entry offset : %zu\n", elf->header->e_entry);
	hash(elf->ptr + elf->text_section->sh_offset, key, elf->text_section->sh_size);
	ft_printf("key for \"%s\" : ", elf->filename);
	print_string_hexa(key);
	ft_printf("\n");
	return write_binary_from_elf(elf, PACKED_NAME);
}
