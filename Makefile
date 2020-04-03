# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: niragne <niragne@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/11/04 14:46:31 by ldedier           #+#    #+#              #
#    Updated: 2020/04/03 17:18:43 by niragne          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= woody_woodpacker

CC		= gcc -g3
ASM		= nasm
PWD		= $(shell pwd)
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

PAYLOAD_SRC = payload.s
PAYLOAD_OBJ = payload.o

LIBFT = $(LIBFTDIR)/libft.a

SRCS_C		=	main.c \
			init.c \
			free.c \
			check_elf.c \
			pack.c \
			print.c \
			get_text_segment.c \
			get_cave.c \
			elf_tools.c \
			patch_target.c \
			debug_target.c \
			errors.c

SRCS_ASM	=	$(PAYLOAD_SRC)

INCLUDES	=	woody.h

OBJECTS			=	$(addprefix $(OBJDIR), $(SRCS_C:.c=.o)) \
				$(addprefix $(OBJDIR), $(SRCS_ASM:.s=.o))

INC 			=	-I $(INCLUDESDIR) -I $(LIBFTDIR)/$(LIBFT_INCLUDES_DIR)

OK_COLOR = \x1b[32;01m
EOC = \033[0m

LFLAGS =	-L $(LIBFTDIR) -lft -Wall -Wextra
CFLAGS = $(INC) -Wall -Wextra -DPAYLOAD_FILE=\"$(PWD)/$(OBJDIR)$(PAYLOAD_OBJ)\"

ifeq ($(DEBUG), 1)
	CFLAGS += -fsanitize=address
	CC += -g3
	SPEED = -j8
else
	SPEED = -j8
endif

all:
	@$(MAKE) -C $(LIBFTDIR) $(SPEED)
	@$(MAKE) $(BINDIR)/$(NAME) $(SPEED)
	@$(MAKE) $(OBJDIR)$(PAYLOAD_OBJ)

debug:
	@$(MAKE) all DEBUG=1

$(LIBFT):
	@$(MAKE) -C $(LIBFTDIR)

$(BINDIR)/$(NAME): $(LIBFT) $(OBJDIR) $(OBJECTS)
	$(CC) -no-pie -o $@ $(OBJECTS) $(CFLAGS) $(LFLAGS)
	@$(ECHO) "$(NAME) linked with success !"

$(OBJDIR):
	@$(MKDIR) $@

$(OBJDIR)$(PAYLOAD_OBJ): $(PAYLOAD_SRC) $(INCLUDES)
	$(ASM) $(ASMFLAGS) -o $@ $< 
	@$(ECHO) "$(PAYLOAD_OBJ) linked with success !"

$(OBJDIR)%.o: %.c $(INCLUDES)
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	@$(MAKE) clean -C $(LIBFTDIR)
	@$(RM) -rf $(OBJDIR)

fclean: clean
	@$(MAKE) fclean -C $(LIBFTDIR)
	@$(RM) $(BINDIR)/$(NAME)

re: fclean all

.PHONY: all clean fclean re debug
