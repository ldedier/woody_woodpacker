# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/10/03 19:03:26 by ldedier           #+#    #+#              #
#    Updated: 2019/10/11 18:15:46 by ldedier          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libfts.a
GCC = gcc -g3 #-fsanitize=address
AS = nasm -g 

SRCDIR = srcs
TESTDIR = test

VPATH = $(SRCDIR) $(TESTDIR)
OBJDIR = objs
INCLUDESDIR = includes
CHECKER = checker

OK_COLOR = \x1b[32;01m
EOC = \033[0m

TEST_SRCS =		ft_bzero_tester.c\
				ft_strcat_tester.c\
				ft_isalpha_tester.c\
				ft_isdigit_tester.c\
				ft_isalnum_tester.c\
				ft_isascii_tester.c\
				ft_isprint_tester.c\
				ft_toupper_tester.c\
				ft_tolower_tester.c\
				ft_puts_tester.c\
				ft_strlen_tester.c\
				ft_memset_tester.c\
				ft_memcpy_tester.c\
				ft_strdup_tester.c\
				ft_cat_tester.c\
				ft_strcpy_tester.c\
				ft_putstr_fd_tester.c\
				ft_putchar_tester.c\
				ft_strequ_tester.c\
				ft_strcmp_tester.c\
				checker_tools.c\
				ft_atoi_tester.c\
				ft_itoa.c\
				main.c 

SRCS =	ft_isdigit.s ft_isalpha.s ft_isalnum.s ft_isprint.s ft_isascii.s\
		ft_tolower.s ft_toupper.s ft_bzero.s ft_strlen.s ft_strcat.s\
		ft_strcpy.s ft_puts.s ft_memset.s ft_memcpy.s ft_strdup.s\
		ft_hello.s ft_cat.s ft_putstr.s ft_putchar.s ft_putstr_fd.s\
		ft_strequ.s ft_strcmp.s ft_atoi.s

OBJECTS = $(addprefix $(OBJDIR)/, $(SRCS:%.s=%.o))
TEST_OBJECTS  = $(addprefix $(OBJDIR)/, $(TEST_SRCS:%.c=%.o))

INCLUDES_NO_PREFIX = libftasm.h
INCLUDES = $(addprefix $(INCLUDESDIR)/, $(INCLUDES_NO_PREFIX))
INC = -I $(INCLUDESDIR)

ASMFLAGS = -f macho64 
CFLAGS = -Wall -Werror -Wextra
LFLAGS = -L. -lfts

all: $(NAME)

$(CHECKER): $(NAME) $(TEST_OBJECTS)
	$(GCC)  $(TEST_OBJECTS) -o $(CHECKER) $(LFLAGS) -Wl,-no_pie
	@echo "$(OK_COLOR)$(CHECKER) linked with success !$(EOC)"

$(OBJDIR)/%.o: $(TESTDIR)/%.c $(INCLUDES) includes/libftasm_checker.h
	@mkdir -p $(OBJDIR)
	$(GCC) -c $< -o $@ $(CFLAGS) $(INC)

$(OBJDIR)/%.o: $(SRCDIR)/%.s $(INCLUDES)
	@mkdir -p $(OBJDIR)
	$(AS) -o $@ $< $(ASMFLAGS) $(INC) -I $(SRCDIR)

$(NAME): $(OBJECTS)
	ar rc $@ $^
	ranlib $(NAME)
	@echo "$(OK_COLOR)$(NAME) linked with success !$(EOC)"

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME) $(CHECKER)

re: fclean all

.PHONY: all clean fclean re
