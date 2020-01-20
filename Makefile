# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/01/20 19:39:21 by wkorande          #+#    #+#              #
#    Updated: 2020/01/20 20:09:31 by wkorande         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=vk-experiment

VULKAN_SDK_PATH =  /usr/lib/x86_64-linux-gnu #/home/user/VulkanSDK/x.x.x.x/x86_64
LDFLAGS = -L$(VULKAN_SDK_PATH) `pkg-config --static --libs glfw3` -lvulkan

all: $(NAME)

$(NAME):
	g++ $(CFLAGS) -o $(NAME) main.cpp $(LDFLAGS)

clean:
	rm -f $(NAME)

re: clean all

.PHONY: all clean re
