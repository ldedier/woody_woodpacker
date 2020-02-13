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

NAME	= payload

AS = nasm -g 

ECHO = echo
MKDIR = mkdir

INCLUDESDIR = includes
DEBUG ?= 0

SRCDIR   = srcs/
OBJDIR   = objs/
BINDIR   = .

VPATH		=	$(SRCDIR)

SRCS		=	payload.s

OBJECTS		=	$(addprefix $(OBJDIR), $(SRCS:.s=.o))

INCLUDES_NO_PREFIX = payload.h
INCLUDES = $(addprefix $(INCLUDESDIR)/, $(INCLUDES_NO_PREFIX))
INC = -I $(INCLUDESDIR)

ASMFLAGS = -f macho64

all:
	@$(MAKE) $(BINDIR)/$(NAME)

debug:
	@$(MAKE) all DEBUG=1

$(BINDIR)/$(NAME): $(OBJDIR) $(OBJECTS)
	ld $^ -o $@ $(ASMFLAGS)
	@$(ECHO) "$(NAME) linked with success !"

$(OBJDIR):
	@$(MKDIR) $@

$(OBJDIR)/%.o: $(SRCDIR)/%.s $(INCLUDES)
	@mkdir -p $(OBJDIR)
	$(AS) -o $@ $< $(ASMFLAGS) $(INC)

$(NAME): $(OBJECTS)

clean:
	@$(RM) -rf $(OBJDIR)

fclean: clean
	@$(RM) $(BINDIR)/$(NAME)

re: fclean all

PHONY: all clean fclean re
