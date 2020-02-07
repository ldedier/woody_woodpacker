/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   woody.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/01 20:09:52 by ldedier           #+#    #+#             */
/*   Updated: 2020/02/01 20:09:52 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOODY_H
# define WOODY_H

# include <elf.h>
# include <unistd.h>
# include <fcntl.h>
# include "libft.h"

struct s_elf
{
	void *ptr;
	char *filename;
	struct stat st;
	off_t size;
	Elf64_Ehdr header;
};

int			init(struct s_elf *elf, char *filename);
void			free_all(struct s_elf *elf);
int			process_woody(struct s_elf *elf);

void			print_elf64_exec_header(Elf64_Ehdr header);
void			print_elf64_program_segment_header(Elf64_Phdr header);
void			print_elf64_section_header(Elf64_Shdr header);

int			not_elf_file(struct s_elf *elf);
int			error_corrupted(struct s_elf *elf);

int			is_corrupted_data(void *address, size_t size, struct s_elf *elf);
int			is_corrupted_string_light(char *str, struct s_elf *elf);

#endif
