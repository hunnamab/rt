# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/11/07 15:39:13 by hunnamab          #+#    #+#              #
#    Updated: 2020/11/10 18:00:01 by pmetron          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = rtv1
NAME_LINUX = rtv1_linux
LIB_FLAGS = -Wall -Wextra
MAC_FLAGS = -I SDL2.framework/Headers -F ./ -framework SDL2
LINUX_FLAGS = -lSDL2 -lm
LIBRARY = ./libft/libft.a
HEADER = rtv1.h
SRC = main.c sphere.c vector.c utils.c \
	light.c triangle.c scenes_reader.c draw.c ftoi.c \
	objects_parameters.c plane.c cylinder.c cone.c \
	./matrix_lib/matr_add_matr.c ./matrix_lib/create_matrix.c \
	./matrix_lib/matr_copy.c ./matrix_lib/matr_div_by_scalar.c \
	./matrix_lib/matr_free.c ./matrix_lib/matr_mul_by_scalar.c \
	./matrix_lib/matr_mul.c  ./matrix_lib/matr_sub_matr.c \
	./matrix_lib/matr_sub_scalar.c ./matrix_lib/matr_to_line.c \
	./matrix_lib/matr_trace.c ./matrix_lib/matr_transpose.c \
	transform.c ./matrix_lib/matrix_identity.c \
	buffers.c scene.c color.c vector_second.c transform_matrix.c \
	keyboard.c clean.c errors_management.c light_parameters.c \
	parameters_utils.c camera_parameters.c define_object.c \
	scenes_reader_util.c
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(OBJ): %.o: %.c $(HEADER)
		gcc -c $(LIB_FLAGS) -I libft/ -I matrix_lib/ -o $@ $<

$(LIBRARY):
		@make -C libft/

$(NAME): $(LIBRARY) $(OBJ)
		@cp -r SDL2.framework ~/Library/Frameworks/
		@gcc $(OBJ) $(LIBRARY) -o $(NAME) $(MAC_FLAGS) -I $(HEADER)

linux: $(NAME_LINUX) 

$(OBJ): %.o: %.c $(HEADER)
	gcc -c $(LIB_FLAGS) -I libft/ -I matrix_lib/ -o $@ $<

$(LIBRARY):
		@make -C libft/

$(NAME_LINUX): $(LIBRARY) $(OBJ)
		@gcc $(OBJ) $(LIBRARY) -o $(NAME_LINUX) $(LINUX_FLAGS) -I $(HEADER)
		@make clean
clean:
	@rm -f $(OBJ)
	@make -C libft clean

fclean: clean
	@rm -f $(NAME)
	@rm -rf ~/Library/Frameworks/SDL2.framework
	@make -C libft fclean

fclean_linux: clean
	@rm -f $(NAME_LINUX)
	@make -C libft fclean

re: fclean all
