# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wkorande <willehard@gmail.com>             +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/01/20 19:39:21 by wkorande          #+#    #+#              #
#    Updated: 2020/08/29 14:13:36 by wkorande         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=vk-experiment

SRC= src/main.cpp\
	src/VulkanRenderer.cpp\
	src/Mesh.cpp


VULKAN_SDK_PATH =  /usr/lib/x86_64-linux-gnu #/home/user/VulkanSDK/x.x.x.x/x86_64
LDFLAGS = -L$(VULKAN_SDK_PATH) `pkg-config --static --libs glfw3` -lvulkan
CC=g++

SHADER_DIR=./shaders
SHADERS =	$(wildcard $(SHADER_DIR)/*.vert)\
			$(wildcard $(SHADER_DIR)/*.frag)
SPVS = 	$(patsubst $(SHADER_DIR)/%.vert,$(SHADER_DIR)/%_vert.spv,$(SHADERS))\
		$(patsubst $(SHADER_DIR)/%.frag,$(SHADER_DIR)/%_frag.spv,$(SHADERS))

all: $(NAME) $(SPVS)

$(NAME):
	@printf "compiling: $(NAME)\n"
	@$(CC) $(CFLAGS) -o $(NAME) -I src $(SRC) $(LDFLAGS)

# shaders:
# 	cd ./shaders;\
# 	./compile_shaders.sh

$(SHADER_DIR)/%_vert.spv: $(SHADER_DIR)/%.vert
	@printf "compiling: "
	@glslangValidator -o $@ -V $<

$(SHADER_DIR)/%_frag.spv: $(SHADER_DIR)/%.frag
	@printf "compiling: "
	@glslangValidator -o $@ -V $<


debug:
	$(CC) -g $(CFLAGS) -o $(NAME) $(SRC) $(LDFLAGS)

clean:

fclean: clean
	@rm -f $(NAME)
	@rm -f shaders/*.spv

re: fclean all

.PHONY: all clean fclean re shaders
