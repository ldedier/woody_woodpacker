/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pack.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/07 17:38:15 by ldedier           #+#    #+#             */
/*   Updated: 2020/04/04 15:32:54 by niragne          ###   ########.fr       */
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

uint8_t*	create_new_bin(struct s_woody* woody)
{
	uint8_t* new_ptr = malloc(woody->output_len);
	if (!new_ptr)
		return(NULL);

	// Copy file to new ptr with enough space to add our new section
	memmove(new_ptr, woody->file_to_pack->ptr, woody->insert_offset);
	memset(new_ptr + woody->insert_offset + woody->payload_size_aligned, 0x0, woody->bss_len);
	memmove(new_ptr + woody->insert_offset + woody->insert_size,
			woody->file_to_pack->ptr + woody->insert_offset,
			woody->file_to_pack->st.st_size - woody->insert_offset );
	return (new_ptr);
}

int	insert_and_patch_payload(struct s_woody* woody, uint8_t* new_ptr, Elf64_Shdr* new_shdr, Elf64_Ehdr* new_hdr)
{
	memmove(new_ptr + woody->payload_start_off, woody->payload->ptr + woody->payload->text_section->sh_offset, woody->payload->text_section->sh_size);
	new_shdr->sh_offset += woody->insert_size;
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
		return (woody_error("MERDE"));
	return (0);
}

void	insert_new_shdr(struct s_woody* woody, uint8_t* new_ptr, Elf64_Shdr* new_shdr, Elf64_Ehdr* new_hdr)
{
	size_t new_shdr_off = (void *)new_shdr - (void *)new_hdr;
	size_t shift_len = woody->output_len - new_shdr_off - sizeof(Elf64_Shdr);
	Elf64_Shdr *prev_shdr = new_shdr - 1;

	memmove(new_ptr + new_shdr_off + sizeof(Elf64_Shdr), new_shdr, shift_len);
	
	new_shdr->sh_name = 0;
	new_shdr->sh_type = SHT_PROGBITS;
	new_shdr->sh_flags = SHF_EXECINSTR | SHF_ALLOC;
	new_shdr->sh_offset = woody->payload_start_off;
	new_shdr->sh_addr = prev_shdr->sh_addr + (woody->payload_start_off - prev_shdr->sh_offset);
	new_shdr->sh_size = woody->payload_size_aligned;
	new_shdr->sh_link = 0;
	new_shdr->sh_info =  0;
	new_shdr->sh_addralign = 16;
	new_shdr->sh_entsize = 0;

	new_hdr->e_shnum += 1;
	new_hdr->e_shstrndx += 1;
}

int		generate_and_patch_key(struct s_woody* woody, uint8_t* new_ptr, char** key)
{
	if (!(*key = generate_key(sizeof(KEY_PLACEHOLDER))))
		return (woody_error("malloc error"));
	if (patch_target(new_ptr + woody->payload_start_off, woody->payload->text_section->sh_size, 0x4444444444444444, woody->file_to_pack->text_section->sh_size))
		return (woody_error("could not find payload jmp argument"));
	ft_printf("text section size: %zu\n", woody->file_to_pack->text_section->sh_size);
	if (patch_target_string(new_ptr + woody->payload_start_off, woody->payload->text_section->sh_size, KEY_PLACEHOLDER, *key))
		return (woody_error("could not find key string placeholder"));

	print_string_hexa(*key);
	printf("\n");
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
//	print_elf64(elf);

	// payload->text_section->sh_size = 81;
	Elf64_Shdr *new_shdr;
	Elf64_Phdr *last_load_segment_header;
	struct s_woody woody;

	if (!(last_load_segment_header = get_last_loaded_segment_header(elf)))
		return (woody_error("not found a single load segment"));
	
	prepare_config(&woody, last_load_segment_header, elf, payload);
	prepare_last_load(&woody, last_load_segment_header);
	
	uint8_t* new_ptr = create_new_bin(&woody);
	
	if (!new_ptr)
		return (woody_error("failed to allocate enough memory for new file"));

	Elf64_Ehdr* new_hdr = (Elf64_Ehdr*)new_ptr;
	printf("ehdr before = %lx\n", elf->header->e_shoff);
	if (new_hdr->e_shoff > woody.insert_offset)
		new_hdr->e_shoff += woody.insert_size;
	printf("ehdr after = %lx\n", new_hdr->e_shoff);

	printf("trying to update offsets with %d %lx %lx\n", new_hdr->e_shnum, woody.insert_offset, woody.insert_size);
	new_shdr = update_shdrs_off((Elf64_Shdr *)(new_ptr + new_hdr->e_shoff), new_hdr->e_shnum, woody.insert_offset, woody.insert_size);
	if (!new_shdr)
	{
		free(new_ptr);
		return(woody_error("a problem occured while parsing sections"));
	}
	printf("new_shdr: %ld\n", new_shdr->sh_addr);

	if (insert_and_patch_payload(&woody, new_ptr, new_shdr, new_hdr))
		return (woody_error("error while patching payload"));

	insert_new_shdr(&woody, new_ptr, new_shdr, new_hdr);
	

	char		*key;
	if (generate_and_patch_key(&woody, new_ptr, &key))
		return (woody_error("error while generating or patching key"));
	hash((char*)(new_ptr + elf->text_section->sh_offset), key, elf->text_section->sh_size);
	free(key);
	
	if (create_packed(&woody, new_ptr))
	{
		return(woody_error("error while creating new file"));
	}
	free(new_ptr);

	return (0);
}

