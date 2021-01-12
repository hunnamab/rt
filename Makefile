# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ldeirdre <ldeirdre@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/11/07 15:39:13 by hunnamab          #+#    #+#              #
#    Updated: 2021/01/12 18:41:10 by ldeirdre         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = rt
NAME_LINUX = rt_linux
LIB_FLAGS = -Wall -Wextra
MAC_FLAGS = -I SDL2.framework/Headers -F ./ -framework SDL2 -framework OpenCL -lSDL2_image 
LINUX_FLAGS = -lSDL2 -lm -lXext -lcuda -lcudart
LSDLIMDIR = ./SDL_image/
OTHER2 = ./lib_sdl_im/
LIBRARY = ./libft/libft.a 
HEADER = rt.h
SRC = main.c sphere.c vector.c utils.c \
	light.c triangle.c scenes_reader.c draw.c \
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
	scenes_reader_util.c cl_init.c textures.c texture_mapping.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(OBJ): %.o: %.c $(HEADER)
		gcc -c $(LIB_FLAGS) -I libft/ -I matrix_lib/ -o $@ $<

$(LIBRARY):
		@make -C libft/

$(NAME): $(LIBRARY) $(OBJ)
		@cp -r SDL2.framework ~/Library/Frameworks/
		@gcc $(OBJ) $(LIBRARY)  -L $(OTHER2) -o $(NAME) $(MAC_FLAGS) -I $(HEADER) -I $(LSDLIMDIR)

clean:
	@rm -f $(OBJ)
	@make -C libft clean
fclean: clean
	@rm -f $(NAME)
	@rm -rf ~/Library/Frameworks/SDL2.framework
	@make -C libft fclean

re: fclean all

linux: $(NAME_LINUX)

$(NAME_LINUX): $(OBJ_LINUX)
	nvcc -c ./linux_src/*.cu ./linux_src/*.c ./matrix_lib/*.c -lSDL2 -lm -lXext -lcuda -lcudart -I ./linux_src/headers/
	nvcc *.o $(LIBRARY) -o $(NAME_LINUX) $(LINUX_FLAGS)
