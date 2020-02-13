/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/01 18:32:07 by ldedier           #+#    #+#             */
/*   Updated: 2020/02/01 18:32:07 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <sys/stat.h>
# include "woody.h"
# include <sys/mman.h>

void free_all(struct s_elf *elf)
{
	munmap(elf->ptr, elf->st.st_size);
}
