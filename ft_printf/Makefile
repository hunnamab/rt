# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: poatmeal <poatmeal@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/09/05 17:12:11 by aagrivan          #+#    #+#              #
#    Updated: 2020/08/30 15:41:10 by poatmeal         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libftprintf.a

CC = gcc
FLAGS = -Wall -Wextra -Werror

SRCS = ft_get_flags.c \
		ft_print_di.c \
		ft_print_o.c \
		ft_print_scp.c \
		ft_print_u.c \
		ft_print_x.c \
		ft_printf.c \
		ft_support.c \
		attributes.c \
		init_f.c \
		mem_alloc.c \
		pow_2.c \
		pow_5.c \
		print_f.c \
		round.c \
		round_change.c \
		nan_inf.c \
		ft_print_other.c
SRC_DIR = srcs/
SRCS_P = $(addprefix $(SRC_DIR),$(SRCS))

LIB_F = ft_itoa_base.c \
		ft_strnew.c \
		ft_putchar.c \
		ft_memset.c \
		ft_strchr.c \
		ft_strcmp.c \
		ft_strlen.c \
		ft_putstr.c \
		ft_strndup.c \
		ft_itoa_base.c \
		ft_itoa_base_address16.c \
		ft_itoa_base_ll.c \
		ft_itoa_base_ll_pos.c \
		ft_itoa_base_ull.c \
		ft_putunbr.c \
		ft_putendl.c \
		ft_memalloc.c \
		ft_memdel.c \
		ft_strdel.c
LIBF_DIR = libft_functions/
LIBF_P = $(addprefix $(LIBF_DIR),$(LIB_F))

HEADER = ft_printf.h
HEAD_DIR = includes/
HEAD_P = $(addprefix $(HEAD_DIR),$(HEADER))

OBJLIBDIR = obj_lib/
OBJ_LIBP = $(addprefix $(OBJLIBDIR),$(LIB_F:%.c=%.o))

OBJDIR = obj_printf/
OBJ_P = $(addprefix $(OBJDIR),$(SRCS:%.c=%.o))

vpath %.c srcs libft_functions
vpath %.h includes

# COLOR
GREEN = \033[0;32m
RED = \033[0;31m
BASE = \033[0m

.PHONY = all clean fclean re

all: $(NAME)

$(NAME): $(OBJ_LIBP) $(OBJ_P)
	@ar rc $(NAME) $(OBJ_LIBP) $(OBJ_P)
	@ranlib $(NAME)

$(OBJDIR)%.o: $(SRC_DIR)%.c $(HEAD_P)
	@mkdir -p $(OBJDIR)
	@$(CC) $(FLAGS) -I $(HEAD_DIR) -c $< -o $@

$(OBJLIBDIR)%.o: $(LIBF_DIR)%.c $(HEAD_P)
	@mkdir -p $(OBJLIBDIR)
	@$(CC) $(FLAGS) -I $(HEAD_DIR) -c $< -o $@

clean:
	@rm -rf $(OBJDIR) $(OBJLIBDIR)
	@echo "$(RED)all object files were deleted.$(BASE)"

fclean: clean
	@rm -rf $(NAME)
	@echo "$(RED)$(NAME) was deleted.$(BASE)"

re: fclean all
