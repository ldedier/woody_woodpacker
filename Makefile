# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/11/04 14:46:31 by ldedier           #+#    #+#              #
#    Updated: 2019/11/28 11:52:24 by ldedier          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= woody_woodpacker

CC		= gcc -g3
ASM	=	nasm

ECHO = echo
MKDIR = mkdir

DEBUG ?= 0

SRCDIR   = srcs/
OBJDIR   = objs/
BINDIR   = .

INCLUDESDIR =		includes
LIBFTDIR =		libft
LIBFT_INCLUDES_DIR =	includes

ASMFLAGS = -f elf64

SOURCE_PATHS =	C \
		asm

SOURCE_FULL_PATHS = $(addprefix $(SRCDIR), $(SOURCE_PATHS))

VPATH		=	$(INCLUDESDIR) \
			$(SOURCE_FULL_PATHS)

SPEED = -j8

LIBFT = $(LIBFTDIR)/libft.a

SRCS		=	main.c \
			init.c \
			free.c \
			pack.c \
			print.c \
			get_text_segment.c \
			get_cave.c \
			errors.c

INCLUDES	=	woody.h

OBJECTS			=	$(addprefix $(OBJDIR), $(SRCS:.c=.o))
INC 			=	-I $(INCLUDESDIR) -I $(LIBFTDIR)/$(LIBFT_INCLUDES_DIR)

OK_COLOR = \x1b[32;01m
EOC = \033[0m

LFLAGS =	-L $(LIBFTDIR) -lft -Wall -Wextra -Werror
CFLAGS = $(INC) -Wall -Wextra -Werror

ifeq ($(DEBUG), 1)
	CFLAGS += -fsanitize=address
	CC += -g3
	SPEED = -j8
else
	SPEED = -j8
endif

payload.o: 
	$(AS) -o $@
all:
	@$(MAKE) -C $(LIBFTDIR) $(SPEED)
	@$(MAKE) $(BINDIR)/$(NAME) $(SPEED)

debug:
	@$(MAKE) all DEBUG=1

$(LIBFT):
	@$(MAKE) -C $(LIBFTDIR)

$(BINDIR)/$(NAME): $(LIBFT) $(OBJDIR) $(OBJECTS)
	@$(CC) -o $@ $(OBJECTS) $(CFLAGS) $(LFLAGS)
	@$(ECHO) "$(NAME) linked with success !"

$(OBJDIR):
	@$(MKDIR) $@

$(OBJDIR)%.o: %.c $(INCLUDES)
	$(CC) -c $< -o $@ $(CFLAGS)

$(OBJDIR)%.o: %.s $(INCLUDES)
	$(ASM) -c $< -o $@ $(ASMFLAGS)

clean:
	@$(MAKE) clean -C $(LIBFTDIR)
	@$(RM) -rf $(OBJDIR)

fclean: clean
	@$(MAKE) fclean -C $(LIBFTDIR)
	@$(RM) $(BINDIR)/$(NAME)

re: fclean all

.PHONY: all clean fclean re debug
