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

int	get_text_size(struct s_elf *elf, size_t *text_size)
{
	(void)elf;
	*text_size = 100;
	return (0);
}

int	process_woody(struct s_elf *elf, struct s_elf *payload)
{
	struct s_cave cave;
	size_t payload_size;

	if (check_elf(payload) || check_elf(elf))
		return (1);
	if (get_text_size(payload, &payload_size))
		return (1);
	printf("Payload size: %zu\n", payload_size);
	if (elf64_get_cave_attributes(elf, &cave.offset, &cave.size))
		return (1);
	if (cave.size < payload_size)
		return (woody_error("could not find a suitable part of binary to be injected\n"));
	print_cave(cave);
	return (0);
}


