/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/07 17:53:07 by ldedier           #+#    #+#             */
/*   Updated: 2020/02/07 17:53:07 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody.h"

void print_elf64_exec_header(Elf64_Ehdr header)
{
	ft_printf("ident: %.*s\n", EI_NIDENT, header.e_ident);
	ft_printf("type: %hu\n", header.e_type);
	ft_printf("machine: %hu\n", header.e_machine);
	ft_printf("version: %u\n", header.e_version);
	ft_printf("entry point: %llu\n", header.e_entry);
	ft_printf("program header table file offset: %llu\n", header.e_phoff);
	ft_printf("section header table file offset: %llu\n", header.e_shoff);
	ft_printf("processor specific flags: %u\n", header.e_flags);
	ft_printf("elf header size: %hu\n", header.e_ehsize);
	ft_printf("program header table entry size: %hu\n", header.e_phentsize);
	ft_printf("program header table entry count: %hu\n", header.e_phnum);
	ft_printf("section header table entry size: %hu\n", header.e_shentsize);
	ft_printf("section header table entry count: %hu\n", header.e_shnum);
	ft_printf("section header string table index: %hu\n", header.e_shstrndx);
}

void print_elf64_program_segment_header(Elf64_Phdr header)
{
	ft_printf("type: %#x\n", header.p_type);
	ft_printf("flags: %.28b\n", header.p_flags);
	ft_printf("offset: %llu\n", header.p_offset);
	ft_printf("virtual address: %llu\n", header.p_vaddr);
	ft_printf("physical address: %llu\n", header.p_paddr);
	ft_printf("size of the segment in the file image: %llu\n", header.p_filesz);
	ft_printf("size of the segment in the memory: %llu\n", header.p_memsz);
	ft_printf("alignment: %llu\n", header.p_align);
}

void print_elf64_section_header(Elf64_Shdr header)
{
	ft_printf("name index: %u\n", header.sh_name);
	ft_printf("type: %#x\n", header.sh_type);
	ft_printf("flags: %llu\n", header.sh_flags);
	ft_printf("virtual address: %llu\n", header.sh_addr);
	ft_printf("file offset: %llu\n", header.sh_offset);
	ft_printf("size in bytes: %llu\n", header.sh_size);
	ft_printf("link to another section: %u\n", header.sh_link);
	ft_printf("additional section information: %u\n", header.sh_info);
	ft_printf("additional section information: %llu\n", header.sh_addralign);
	ft_printf("entry size if section holds table: %llu\n", header.sh_entsize);
}
