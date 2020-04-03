/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/01 18:32:07 by ldedier           #+#    #+#             */
/*   Updated: 2020/04/03 18:40:16 by niragne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <sys/stat.h>
# include <stdio.h>
# include "woody.h"
# include <sys/mman.h>
# include <time.h>

static int print_woody_usage(char *progname)
{
	ft_printf("Usage: %s <elf64>\n", progname);
	return (1);
}

int main(int argc, char **argv)
{
	srand(time(NULL));
	struct s_elf elf;
	struct s_elf payload;
	int ret;

	if (argc != 2)
		return print_woody_usage(argv[0]);
	if (init(&elf, argv[1]) || init(&payload, PAYLOAD_FILE))
		return (1);
	if (check_elf(&payload) || check_elf(&elf))
		return (1);
	ret = process_woody(&elf, &payload);
	free_all(&elf);
	return (ret);
}
