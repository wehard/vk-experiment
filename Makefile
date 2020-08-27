# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wkorande <willehard@gmail.com>             +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/01/20 19:39:21 by wkorande          #+#    #+#              #
#    Updated: 2020/08/27 16:28:51 by wkorande         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=vk-experiment

SRC= src/main.cpp\
	src/VulkanRenderer.cpp


VULKAN_SDK_PATH =  /usr/lib/x86_64-linux-gnu #/home/user/VulkanSDK/x.x.x.x/x86_64
LDFLAGS = -L$(VULKAN_SDK_PATH) `pkg-config --static --libs glfw3` -lvulkan

all: $(NAME)

$(NAME):
	g++ $(CFLAGS) -o $(NAME) -I src $(SRC) $(LDFLAGS)

debug:
	g++ -g $(CFLAGS) -o $(NAME) $(SRC) $(LDFLAGS)

clean:
	rm -f $(NAME)

re: clean all

.PHONY: all clean re
