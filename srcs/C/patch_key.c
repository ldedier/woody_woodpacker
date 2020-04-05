/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pack_key.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier </var/mail/ldedier>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/05 09:49:51 by ldedier           #+#    #+#             */
/*   Updated: 2020/04/05 09:49:51 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody.h"
#include <sys/mman.h>

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

int		generate_and_patch_key(struct s_woody* woody, uint8_t* new_ptr, char** key)
{
	if (!(*key = generate_key(sizeof(KEY_PLACEHOLDER))))
		return (woody_error("malloc error"));
	if (patch_target(new_ptr + woody->payload_start_off, woody->payload->text_section->sh_size, 0x4444444444444444, woody->file_to_pack->text_section->sh_size))
		return (woody_error("could not find payload jmp argument"));
	if (patch_target_string(new_ptr + woody->payload_start_off, woody->payload->text_section->sh_size, KEY_PLACEHOLDER, *key))
		return (woody_error("could not find key string placeholder"));
	ft_printf("key: ");
	print_string_hexa(*key);
	ft_printf("\n");
	return (0);	
}
