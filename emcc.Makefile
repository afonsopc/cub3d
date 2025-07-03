# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    emcc.Makefile                                      :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: afpachec <afpachec@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/26 17:16:21 by afpachec          #+#    #+#              #
#    Updated: 2025/07/03 17:10:46 by afpachec         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3d
CC = emcc
CFLAGS = -Wall -Wextra -Werror -O3 -D W_WIDTH=400 -D W_HEIGHT=300
INCLUDES = -I headers
LDLIBS = -lm -ldl
SRCS = $(shell find src -name "**.c")
OBJ_DIR = obj
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

CFLAGS += -pthread
LDLIBS += -s USE_PTHREADS=1 -s PTHREAD_POOL_SIZE=5 -s USE_SDL=2 -s ASSERTIONS=1 -s SAFE_HEAP=1 -s STACK_OVERFLOW_CHECK=2
LDLIBS += --preload-file maps --preload-file assets/42lisboa --shell-file minimal.html
LDLIBS += --preload-file assets/fonts --preload-file assets/sounds
LDLIBS += --preload-file assets/textures --preload-file assets/wolf3d
LDLIBS += -s INITIAL_MEMORY=268435456 -s STACK_SIZE=2MB
LDLIBS += -s ENVIRONMENT='web,worker'
LDLIBS += -s EXPORT_ES6=1 -s MODULARIZE=1

all: $(NAME)

$(NAME): headers/miniaudio.h $(OBJS)
	@echo "\033[1;32mCompiling \033[1;0m\"$(OBJS)\"\033[1;32m into \033[1;0m\"$(NAME)\"\033[1;32m.\033[0m"
	@$(CC) -o $(NAME).html $(OBJS) $(CFLAGS) $(INCLUDES) $(LIBS) $(LDLIBS)

headers/miniaudio.h:
	@echo "\033[1;32mCompiling \033[1;0m\"miniaudio\"\033[1;32m.\033[0m"
	@rm -rf headers/miniaudio.h
	@tar -xzf lib/miniaudio.tar.gz -C headers

$(OBJ_DIR)/%.o: %.c
	@echo "\033[1;32mCompiling \033[1;0m\"$<\"\033[1;32m into \033[1;0m\"$@\"\033[1;32m.\033[0m"
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@rm -rf cub3d.html cub3d.data cub3d.wasm cub3d.js

fclean: clean
	@rm -rf $(NAME)
	@rm -rf lib/libmlx.a
	@rm -rf headers/mlx.h
	@rm -rf headers/SDL2/
	@rm -rf lib/libSDL2.a
	@rm -rf headers/miniaudio.h

re: fclean all

errors:
	@bash -c "python3 <(git show error-msgs-script:gen_error_msgs.py)"
	@bash -c "python3 <(git show error-msgs-script:check_error_signals.py) src"

.PHONY: all re clean fclean run val errors
