# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/11/07 15:39:13 by hunnamab          #+#    #+#              #
#    Updated: 2021/02/21 19:58:00 by npetrell         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = rt
NAME_LINUX = rt_linux

CC = clang
C_FLAGS = -Wall -Wextra

LIB_FLAGS = -I includes -I libft/

FT_PRINTF = ft_printf

PRINT_FLAG = -I ft_printf/

MAC_FLAGS = -I frameworks/SDL2.framework/Headers -F ./frameworks -framework SDL2 -framework OpenCL -I \
			frameworks/SDL2_image.framework/Headers -framework SDL2_image -I \
			frameworks/SDL2_mixer.framework/Headers -framework SDL2_mixer
LINUX_FLAGS = -lSDL2 -lm -lXext -lcuda -lcudart

PRINTF_HEAD = ft_printf/

SRC_PATH = srcs
OBJ_PATH = objs

LIBRARY = ./libft/libft.a

PRINTF = ./ft_printf/libftprintf.a


SRC_NAME = ../matrix_lib/matr_add_matr.c ../matrix_lib/create_matrix.c \
	../matrix_lib/matr_copy.c ../matrix_lib/matr_div_by_scalar.c \
	../matrix_lib/matr_free.c ../matrix_lib/matr_mul_by_scalar.c \
	../matrix_lib/matr_mul.c  ../matrix_lib/matr_sub_matr.c \
	../matrix_lib/matr_sub_scalar.c ../matrix_lib/matr_to_line.c \
	../matrix_lib/matr_trace.c ../matrix_lib/matr_transpose.c \
	../matrix_lib/matrix_identity.c \
	main.c vector.c utils.c transform.c light.c \
	buffers.c buffers_part_2.c scene.c color.c vector_second.c transform_matrix.c \
	keyboard.c clean.c errors_management.c \
	cl_init.c buffers_material_buf.c \
	scene_parsing/objects_parameters.c scene_parsing/scenes_reader.c \
	scene_parsing/light_parameters.c scene_parsing/parameters_utils.c \
	scene_parsing/camera_parameters.c scene_parsing/define_object.c \
	scene_parsing/scenes_reader_util.c scene_parsing/surfaces.c \
	rt_textures/textures.c \
	rt_textures/texture_loading.c \
	rt_textures/perlin_noise/perlin_noise.c  rt_textures/perlin_noise/perlin_noise_utils.c\
	rt_textures/perlin_noise/noise2d.c \
	rt_objects/sphere.c rt_objects/triangle.c rt_objects/plane.c \
	rt_objects/cylinder.c rt_objects/cone.c \
	rt_objects/ellipsoid.c rt_objects/box.c rt_objects/paraboloid.c \
	rt_objects/torus.c rt_objects/hyperboloid.c \
	filters/filters.c scene_parsing/surfaces.c rt_ui.c rt_ui_part_2.c \
	draw/draw_raycast.c draw/get_refraction_ray.c draw/draw_scene.c \
	draw/draw_normal_buf.c draw/draw_deepth_buf.c draw/get_fresnel_coeff.c \
	draw/get_reflection_ray.c scene_parsing/light_parameters_2.c\
	mouse_click.c mouse_click_part_2.c\
	camera_move.c camera_move2.c cutting.c scene_parsing/scenes_reader_help.c \
	scene_parsing/scenes_reader_util_2.c

OBJ_NAME = $(SRC_NAME:.c=.o)

SRC = $(addprefix $(SRC_PATH)/,$(SRC_NAME))
OBJ = $(addprefix $(OBJ_PATH)/,$(OBJ_NAME))

all: $(LIBRARY) $(PRINTF) $(NAME)

$(LIBRARY):
		@make -C libft
		@make -C $(FT_PRINTF)

$(NAME): $(OBJ) $(LIBRARY)
		@cp -r frameworks/SDL2.framework ~/Library/Frameworks/
		@cp -r frameworks/SDL2_image.framework ~/Library/Frameworks/
		@cp -r frameworks/SDL2_mixer.framework ~/Library/Frameworks/
		@$(CC) $(LIBRARY) $(PRINTF) $(MAC_FLAGS) $^ -o $@ $(LIB_FLAGS) $(PRINT_FLAG)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
				@mkdir $(OBJ_PATH) 2> /dev/null || true
				@mkdir $(OBJ_PATH)/rt_objects 2> /dev/null || true
				@mkdir $(OBJ_PATH)/rt_textures 2> /dev/null || true
				@mkdir $(OBJ_PATH)/rt_textures/perlin_noise 2> /dev/null || true
				@mkdir $(OBJ_PATH)/scene_parsing 2> /dev/null || true
				@mkdir $(OBJ_PATH)/filters 2> /dev/null || true
				@mkdir $(OBJ_PATH)/draw 2> /dev/null || true
				@$(CC) $(C_FLAGS) -c $< $(LIB_FLAGS) $(PRINT_FLAG) -o $@
		
clean:
	@rm -rf $(OBJ)
	@make -C libft clean
	@make -C $(FT_PRINTF) clean
	@rmdir $(OBJ_PATH)/rt_objects 2> /dev/null || true
	@rmdir $(OBJ_PATH)/rt_textures 2> /dev/null || true
	@rmdir $(OBJ_PATH)/rt_textures/perlin_noise 2> /dev/null || true
	@rmdir $(OBJ_PATH)/scene_parsing 2> /dev/null || true
	@rmdir $(OBJ_PATH)/filters 2> /dev/null || true
	@rmdir $(OBJ_PATH)/draw 2> /dev/null || true
	@rmdir $(OBJ_PATH) 2> /dev/null || true
	
fclean: clean
	@rm -f $(NAME)
	@rm -rf ~/Library/Frameworks/SDL2.framework
	@rm -rf ~/Library/Frameworks/SDL2_image.framework
	@rm -rf ~/Library/Frameworks/SDL2_mixer.framework
	@make -C libft fclean
	@make -C $(FT_PRINTF) fclean

re: fclean all

linux: $(NAME_LINUX)

$(NAME_LINUX): $(OBJ_LINUX)
	nvcc -c ./linux_src/*.cu ./linux_src/*.c ./matrix_lib/*.c -lSDL2 -lm -lXext -lcuda -lcudart -I ./linux_src/headers/
	nvcc *.o $(LIBRARY) -o $(NAME_LINUX) $(LINUX_FLAGS)
