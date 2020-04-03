/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pack.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/07 17:38:15 by ldedier           #+#    #+#             */
/*   Updated: 2020/04/03 18:45:54 by niragne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody.h"
#include <sys/mman.h>

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

int	pack_from_offset(struct s_elf *elf, struct s_elf *payload, size_t offset)
{
	size_t		old_entry;
	char		*key;

	old_entry = elf->header->e_entry;
	ft_memcpy(elf->ptr + offset,
		payload->ptr + payload->text_section->sh_offset, payload->text_section->sh_size);
	if (elf->header->e_type == ET_DYN)
	{
		printf("DYN (shared object file)\n");
		printf("ENTRY %zu\n",  elf->header->e_entry );
		printf("SECTION OFFSET %zu\n", elf->text_section->sh_offset);
		printf("text section address: %ld\n", elf->text_section->sh_addr);
		printf("cave offset %zu\n", offset);
		//	elf->header->e_entry = cave.offset;
		elf->header->e_entry = offset;// + (elf->header->e_entry - elf->text_section->sh_offset);
		if (patch_target(elf->ptr + offset, payload->text_section->sh_size, 0x1111111111111111, old_entry - elf->header->e_entry - 5))
			return (woody_error("could not find payload jmp argument"));
		if (patch_target(elf->ptr + offset, payload->text_section->sh_size, 0x3333333333333333, elf->text_section->sh_offset - elf->header->e_entry - 5))
			return (woody_error("could not find payload jmp argument"));
	}
	else if (elf->header->e_type == ET_EXEC)
	{
		printf("EXEC (executablefile)\n");
		printf("ENTRY %zu\n",  elf->header->e_entry );
		printf("SECTION OFFSET %zu\n", elf->text_section->sh_offset);
		printf("cave offset %zu\n", offset);
		elf->header->e_entry = elf->text_section->sh_addr - elf->text_section->sh_offset + offset;
		if (patch_target(elf->ptr + offset, payload->text_section->sh_size, 0x2222222222222222, 0))
			return (woody_error("could not find payload jmp argument"));
		if (patch_target(elf->ptr + offset, payload->text_section->sh_size, 0x1111111111111111, old_entry))
			return (woody_error("could not find payload jmp argument"));
		printf("%ld: old entry\n", old_entry);
		if (patch_target(elf->ptr + offset, payload->text_section->sh_size, 0x3333333333333333, elf->text_section->sh_addr))
			return (woody_error("could not find payload jmp argument"));
		printf("%ld: text section address\n", elf->text_section->sh_addr);
	}
	else
		return woody_error("this elf is not a valid executable elf");
	if (!(key = generate_key(sizeof(KEY_PLACEHOLDER))))
		return (woody_error("malloc error"));
	if (patch_target(elf->ptr + offset, payload->text_section->sh_size, 0x4444444444444444, elf->text_section->sh_size))
		return (woody_error("could not find payload jmp argument"));
	ft_printf("text section size: %zu\n", elf->text_section->sh_size);
	if (patch_target_string(elf->ptr + offset, payload->text_section->sh_size, KEY_PLACEHOLDER, key))
		return (woody_error("could not find key string placeholder"));
	printf("new entry offset : %zu\n", elf->header->e_entry);
	ft_printf("key for \"%s\" : ", elf->filename);
	print_string_hexa(key);
	ft_printf("\n");
	hash(elf->ptr + elf->text_section->sh_offset, key, elf->text_section->sh_size);
	return write_binary_from_elf(elf, PACKED_NAME);
}

int	process_woody_old(struct s_elf *elf, struct s_elf *payload)
{
	struct s_cave	cave;

//	print_elf64(elf);

	Elf64_Phdr *last_load_segment_header;

	if (!(last_load_segment_header = get_last_loaded_segment_header(elf)))
		return (woody_error("not found a single load segment"));

	last_load_segment_header->p_flags |= PF_X;
	last_load_segment_header->p_filesz += 1024;
	last_load_segment_header->p_memsz += 1024;

	printf("Payload size: %zu\n", payload->text_section->sh_size);

	if (elf64_get_cave_attributes(elf, &cave.offset, &cave.size))
		return (1);
	if (cave.size < payload->text_section->sh_size)
		return (woody_error("could not find a suitable part of binary to be injected in\n"));
	print_cave(cave);
	printf("old entry : %zu\n", elf->header->e_entry);
	printf("old text section : %zu\n", elf->text_section->sh_addr);
	return (pack_from_offset(elf, payload, cave.offset));
}

void *update_shdrs_off(Elf64_Shdr *shdr, size_t shnum, size_t offset, size_t insert_len)
{
	void *first_updated = 0;

	for (uint16_t i = 0; i < shnum ; i++) {
		printf("%d\n", shdr->sh_type);
		if (shdr->sh_offset > offset) {
			if (!first_updated)
				first_updated = shdr;
			shdr->sh_offset += insert_len;
		}
		shdr++;
	}
	return first_updated;
}


int	process_woody(struct s_elf *elf, struct s_elf *payload)
{

//	print_elf64(elf);

	// payload->text_section->sh_size = 81;
	Elf64_Shdr *new_shdr;
	Elf64_Phdr *last_load_segment_header;

	if (!(last_load_segment_header = get_last_loaded_segment_header(elf)))
		return (woody_error("not found a single load segment"));
	
	size_t insert_offset = last_load_segment_header->p_offset + last_load_segment_header->p_filesz;
	size_t bss_len = last_load_segment_header->p_memsz - last_load_segment_header->p_filesz;
	size_t new_startpoint_vaddr =  last_load_segment_header->p_memsz + last_load_segment_header->p_vaddr;
	size_t payload_start_off = insert_offset + bss_len;
	size_t payload_size_aligned = (payload->text_section->sh_size + 63) & ~63;
	size_t insert_size = (payload_size_aligned) + bss_len;
	size_t output_len =  elf->st.st_size + insert_size + sizeof(Elf64_Shdr);

	last_load_segment_header->p_flags |= PF_X;
	last_load_segment_header->p_filesz += insert_size;
	last_load_segment_header->p_memsz += payload_size_aligned;

	printf("Payload size: %zu\n", payload->text_section->sh_size);
	printf("Insert offset: %lx\n", insert_offset);
	printf("Insert size: %lx\n", insert_size);
	
	uint8_t* new_ptr;
	new_ptr = malloc(output_len);
	if (!new_ptr)
		return(woody_error("malloc error"));
	Elf64_Ehdr* new_hdr = (Elf64_Ehdr*)new_ptr;

	// Copy file to new ptr with enough space to add our new section
	memmove(new_ptr, elf->ptr, insert_offset);
	memset(new_ptr + insert_offset + payload_size_aligned, 0x0, bss_len);
	memmove(new_ptr + insert_offset + insert_size, elf->ptr + insert_offset, elf->st.st_size - insert_offset );

	printf("ehdr before = %ld\n", elf->header->e_shoff);
	if (new_hdr->e_shoff > insert_offset)
		new_hdr->e_shoff += insert_size;
	printf("ehdr after = %ld\n", new_hdr->e_shoff);

	printf("trying to update offsets with %d %lx %lx\n", new_hdr->e_shnum, insert_offset, insert_size);
	new_shdr = update_shdrs_off((Elf64_Shdr *)(new_ptr + new_hdr->e_shoff), new_hdr->e_shnum, insert_offset, insert_size);
	if (!new_shdr)
	{
		free(new_ptr);
		return(woody_error("a problem occured while parsing sections"));
	}
	printf("new_shdr: %ld\n", new_shdr->sh_addr);
	Elf64_Shdr *prev_shdr = new_shdr - 1;

	memmove(new_ptr + payload_start_off, payload->ptr + payload->text_section->sh_offset, payload->text_section->sh_size);
	new_shdr->sh_offset += insert_size;

	if (elf->header->e_type == ET_DYN)
	{
		new_hdr->e_entry = new_startpoint_vaddr;
		printf("a gougou gaga\n");
		if (patch_target(new_ptr + payload_start_off, payload->text_section->sh_size, 0x1111111111111111, elf->header->e_entry - new_hdr->e_entry - 5))
			return (woody_error("could not find payload jmp argument"));
		if (patch_target(new_ptr + payload_start_off, payload->text_section->sh_size, 0x3333333333333333, elf->text_section->sh_offset - new_hdr->e_entry - 5))
			return (woody_error("could not find payload jmp argument"));
	}
	else if (elf->header->e_type == ET_EXEC)
	{
		new_hdr->e_entry = new_startpoint_vaddr;
		if (patch_target(new_ptr + payload_start_off, payload->text_section->sh_size, 0x2222222222222222, 0))
			return (woody_error("could not find payload jmp argument"));
		if (patch_target(new_ptr + payload_start_off, payload->text_section->sh_size, 0x1111111111111111, elf->header->e_entry))
			return (woody_error("could not find payload jmp argument"));
		if (patch_target(new_ptr + payload_start_off, payload->text_section->sh_size, 0x3333333333333333, elf->text_section->sh_addr))
			return (woody_error("could not find payload jmp argument"));
	}
	else
		return (woody_error("MERDE"));	

	char		*key;

	if (!(key = generate_key(sizeof(KEY_PLACEHOLDER))))
		return (woody_error("malloc error"));
	if (patch_target(new_ptr + payload_start_off, payload->text_section->sh_size, 0x4444444444444444, elf->text_section->sh_size))
		return (woody_error("could not find payload jmp argument"));
	ft_printf("text section size: %zu\n", elf->text_section->sh_size);
	if (patch_target_string(new_ptr + payload_start_off, payload->text_section->sh_size, KEY_PLACEHOLDER, key))
		return (woody_error("could not find key string placeholder"));

	print_string_hexa(key);
	printf("\n");
	// hash(elf->ptr + elf->text_section->sh_offset, key, elf->text_section->sh_size);
	



	size_t new_shdr_off = (void *)new_shdr - (void *)new_hdr;
	size_t shift_len = output_len - new_shdr_off - sizeof(Elf64_Shdr);

	memmove(new_ptr + new_shdr_off + sizeof(Elf64_Shdr), new_shdr, shift_len);
	
	new_shdr->sh_name = 0;
	new_shdr->sh_type = SHT_PROGBITS;
	new_shdr->sh_flags = SHF_EXECINSTR | SHF_ALLOC;
	new_shdr->sh_offset = payload_start_off;
	new_shdr->sh_addr = prev_shdr->sh_addr + (payload_start_off - prev_shdr->sh_offset);
	new_shdr->sh_size = payload_size_aligned;
	new_shdr->sh_link = 0;
	new_shdr->sh_info =  0;
	new_shdr->sh_addralign = 16;
	new_shdr->sh_entsize = 0;

	new_hdr->e_shnum += 1;
	new_hdr->e_shstrndx += 1;
	int fd;


	if ((fd = open(PACKED_NAME, O_CREAT | O_WRONLY | O_TRUNC, 0755)) < 0)
	{
		perror(PACKED_NAME);
		return (1);
	}

	printf("ptr %p\n", elf->text_section->sh_offset);
	printf("%p\n", elf->text_section->sh_size);
	hash((char*)(new_ptr + elf->text_section->sh_offset), key, elf->text_section->sh_size);
	free(key);

	

	// write(fd, elf->ptr, insert_offset);
	// write(fd, buff, insert_size);
	// write(fd, elf->ptr + insert_offset, elf->st.st_size - insert_offset);
	

	write(fd, new_ptr, output_len);
	free(new_ptr);
	
	close(fd);
	
	return (0);
}

