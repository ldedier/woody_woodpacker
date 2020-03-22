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

# define PAYLOAD_SIZE 400
# define PACKED_NAME "to_change_woody"
# define DEBUG_KEY "pwd12"
# define KEY_PLACEHOLDER "___TO_REMPLACE_KEY___"

struct s_cave
{
	size_t offset;
	size_t size;
};

struct s_elf
{
	void *ptr;
	char *filename;
	struct stat st;
	char *strtable;
	Elf64_Ehdr *header;
	Elf64_Shdr *text_section;
};

int			init(struct s_elf *elf, char *filename);
void			free_all(struct s_elf *elf);
int			get_cave(struct s_elf *elf, size_t *cave_offset, size_t *cave_size);
int			process_woody(struct s_elf *elf, struct s_elf *payload);

void			print_elf64_exec_header(Elf64_Ehdr header);
void			print_elf64_program_segment_header(Elf64_Phdr header);
void			print_elf64_section_header(Elf64_Shdr header);
void			print_elf64(struct s_elf *elf);
void			print_cave(struct s_cave cave);

int			not_elf_file(struct s_elf *elf);
int			error_corrupted(struct s_elf *elf);

int			is_corrupted_data(void *address, size_t size, struct s_elf *elf);
int			is_corrupted_string_light(char *str, struct s_elf *elf);



int			get_text(struct s_elf *elf, size_t *text_size, unsigned char **data, size_t *offset);

int			get_section(struct s_elf *elf, Elf64_Shdr **section_ptr, char *sect_name);
Elf64_Phdr		*get_text_segment_header(struct s_elf *elf);
Elf64_Phdr		*get_note_segment_header(struct s_elf *elf);

int			woody_error(char *msg);

int			elf64_get_cave_attributes(struct s_elf *elf, size_t *cave_offset, size_t *cave_size);


int			write_binary_from_elf(struct s_elf *elf, char *filename);
int     write_binary_from_elf_and_payload(struct s_elf *elf, struct s_elf *payload, char *filename);
int			error_no_section(char *filename, char *sect_name);
int			check_elf(struct s_elf *elf);

void			hash(char *address, char *key, size_t size);

int			patch_target_string(unsigned char *data, size_t size, char *pattern, char *to_remplace);
int			patch_target(unsigned char *data, size_t size, long pattern, long to_remplace);


int			print_some(char *data);
int			scan_target(unsigned char *data, size_t size);
#endif
