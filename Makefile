# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pvcordeiro <pvcordeiro@student.42.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/26 17:16:21 by afpachec          #+#    #+#              #
#    Updated: 2025/06/26 12:54:36 by pvcordeiro       ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3d
CC = cc
CFLAGS = -Wall -Wextra -Werror -g -O3
INCLUDES = -I headers
LIBS = -L lib
LDLIBS = -lmlx -lX11 -lXext -lm
SRCS = $(shell find src -name "**.c")
OBJ_DIR = obj
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))
UNAME_S = $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
	LIBS += -L /opt/homebrew/lib
	INCLUDES += -I /opt/X11/include
	LDLIBS += -framework OpenGL -framework AppKit
endif

all: $(NAME)

$(NAME): lib/libmlx.a $(OBJS)
	@$(CC) -o $(NAME) $(OBJS) $(CFLAGS) $(INCLUDES) $(LIBS) $(LDLIBS)
	@echo "\033[1;32mCompiled \033[1;0m\"$(OBJS)\"\033[1;32m into \033[1;0m\"$(NAME)\"\033[1;32m.\033[0m"

lib/libmlx.a:
	@rm -rf lib/minilibx-linux
	@tar -xzf lib/minilibx-linux.tgz -C lib
ifeq ($(UNAME_S),Darwin)
	@cp lib/libmlx_Darwin.a lib/libmlx.a
else
	@$(MAKE) -C lib/minilibx-linux
	@cp lib/minilibx-linux/libmlx.a lib
endif
	@cp lib/minilibx-linux/mlx.h headers
	@rm -rf lib/minilibx-linux

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	@echo "\033[1;32mCompiled \033[1;0m\"$<\"\033[1;32m into \033[1;0m\"$@\"\033[1;32m.\033[0m"

clean:
	@rm -rf $(OBJ_DIR)
	@rm -rf lib/minilibx-linux

fclean: clean
	@rm -rf $(NAME)
	@rm -rf lib/libmlx.a
	@rm -rf headers/mlx.h

re: fclean all

run: $(NAME)
	./$(NAME) maps/subject.cub
	rm headers/mlx.h

val: $(NAME)
	@valgrind --show-leak-kinds=all --leak-check=full --track-fds=all ./$(NAME) maps/subject.cub

.PHONY: all re clean fclean run val errors
