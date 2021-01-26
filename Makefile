# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/11/07 15:39:13 by hunnamab          #+#    #+#              #
#    Updated: 2021/01/26 21:46:51 by npetrell         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = rt
NAME_LINUX = rt_linux

CC = clang
C_FLAGS = -Wall -Wextra

LIB_FLAGS = -I includes -I libft/
MAC_FLAGS = -I frameworks/SDL2.framework/Headers -F ./frameworks -framework SDL2 -framework OpenCL -I \
			frameworks/SDL2_image.framework/Headers -framework SDL2_image -I \
			frameworks/SDL2_mixer.framework/Headers -framework SDL2_mixer
LINUX_FLAGS = -lSDL2 -lm -lXext -lcuda -lcudart

SRC_PATH = srcs
OBJ_PATH = objs

LIBRARY = ./libft/libft.a

SRC_NAME = ../matrix_lib/matr_add_matr.c ../matrix_lib/create_matrix.c \
	../matrix_lib/matr_copy.c ../matrix_lib/matr_div_by_scalar.c \
	../matrix_lib/matr_free.c ../matrix_lib/matr_mul_by_scalar.c \
	../matrix_lib/matr_mul.c  ../matrix_lib/matr_sub_matr.c \
	../matrix_lib/matr_sub_scalar.c ../matrix_lib/matr_to_line.c \
	../matrix_lib/matr_trace.c ../matrix_lib/matr_transpose.c \
	../matrix_lib/matrix_identity.c \
	main.c vector.c utils.c transform.c light.c draw.c \
	buffers.c scene.c color.c vector_second.c transform_matrix.c \
	keyboard.c clean.c errors_management.c \
	cl_init.c buffers_material_buf.c \
	scene_parsing/objects_parameters.c scene_parsing/scenes_reader.c \
	scene_parsing/light_parameters.c scene_parsing/parameters_utils.c \
	scene_parsing/camera_parameters.c scene_parsing/define_object.c \
	scene_parsing/scenes_reader_util.c \
	rt_textures/textures.c rt_textures/texture_mapping.c \
	rt_textures/texture_loading.c \
	rt_objects/sphere.c rt_objects/triangle.c rt_objects/plane.c \
	rt_objects/cylinder.c rt_objects/cone.c \
	rt_objects/ellipsoid.c rt_objects/box.c rt_objects/paraboloid.c \
	filters/filters.c

OBJ_NAME = $(SRC_NAME:.c=.o)

SRC = $(addprefix $(SRC_PATH)/,$(SRC_NAME))
OBJ = $(addprefix $(OBJ_PATH)/,$(OBJ_NAME))

all: $(LIBRARY) $(NAME)

$(LIBRARY):
		@make -C libft

$(NAME): $(OBJ) $(LIBRARY)
		@cp -r frameworks/SDL2.framework ~/Library/Frameworks/
		@cp -r frameworks/SDL2_image.framework ~/Library/Frameworks/
		@cp -r frameworks/SDL2_mixer.framework ~/Library/Frameworks/
		@$(CC) $(LIBRARY) $(MAC_FLAGS) $^ -o $@ $(LIB_FLAGS)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
				@mkdir $(OBJ_PATH) 2> /dev/null || true
				@mkdir $(OBJ_PATH)/rt_objects 2> /dev/null || true
				@mkdir $(OBJ_PATH)/rt_textures 2> /dev/null || true
				@mkdir $(OBJ_PATH)/scene_parsing 2> /dev/null || true
				@$(CC) $(C_FLAGS) -c $< $(LIB_FLAGS) -o $@
		
clean:
	@rm -rf $(OBJ)
	@make -C libft clean
	@rmdir $(OBJ_PATH)/rt_objects 2> /dev/null || true
	@rmdir $(OBJ_PATH)/rt_textures 2> /dev/null || true
	@rmdir $(OBJ_PATH)/scene_parsing 2> /dev/null || true
	@rmdir $(OBJ_PATH) 2> /dev/null || true
	
fclean: clean
	@rm -f $(NAME)
	@rm -rf ~/Library/Frameworks/SDL2.framework
	@rm -rf ~/Library/Frameworks/SDL2_image.framework
	@rm -rf ~/Library/Frameworks/SDL2_mixer.framework
	@make -C libft fclean

re: fclean all

linux: $(NAME_LINUX)

$(NAME_LINUX): $(OBJ_LINUX)
	nvcc -c ./linux_src/*.cu ./linux_src/*.c ./matrix_lib/*.c -lSDL2 -lm -lXext -lcuda -lcudart -I ./linux_src/headers/
	nvcc *.o $(LIBRARY) -o $(NAME_LINUX) $(LINUX_FLAGS)
