/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ok.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/14 20:18:48 by ldedier           #+#    #+#             */
/*   Updated: 2020/02/14 20:18:48 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

void v()
{
	printf("hehe\n");
}

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	printf("salut\n");
	printf("hehe\n");
	v();
	return (0);
}
