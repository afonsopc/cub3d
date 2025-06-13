# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/26 17:16:21 by afpachec          #+#    #+#              #
#    Updated: 2025/06/11 12:55:42 by afpachec         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3d
CC = cc
CFLAGS = -Wall -Wextra -Werror -O3 -g
# CFLAGS = -Wall -Wextra -Werror -O3 -g -std=c99 -fsanitize=address -fno-omit-frame-pointer
INCLUDES = -I headers
LIBS = -L lib
LDLIBS = -lmlx -lX11 -lXext -lm -ldl -lpthread
SRCS = $(shell find src -name "**.c")
OBJ_DIR = obj
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))
MAPS = $(wildcard maps/*.cub)
UNAME_S = $(shell uname -s)

check_flag = $(shell $(CC) $(1) -E -c /dev/null -o /dev/null 2>/dev/null && echo 1 || echo 0)
ifeq ($(UNAME_S),Darwin)
	LIBS += -L /opt/homebrew/lib
	INCLUDES += -I /opt/X11/include
	LDLIBS += -framework OpenGL -framework AppKit
else
	ifeq ($(shell $(CC) --version | grep -i clang > /dev/null && echo clang),clang)
    	CFLAGS += -Wno-unknown-warning-option
	else
    	CFLAGS += -Wno-stringop-overflow
	endif
endif

all: $(NAME)

$(NAME): fonts assets/wolf3d lib/libmlx.a $(OBJS)
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
	@cp lib/minilibx-linux/mlx_int.h headers
	@rm -rf lib/minilibx-linux

$(OBJ_DIR)/%.o: %.c
	@echo "\033[1;32mCompiling \033[1;0m\"$<\"\033[1;32m into \033[1;0m\"$@\"\033[1;32m.\033[0m"
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@rm -rf lib/minilibx-linux

fclean: clean
	@rm -rf $(NAME)
	@rm -rf lib/libmlx.a
	@rm -rf headers/mlx.h

re: fclean all

dev: $(NAME)
	./$(NAME) maps/e1m1.cub

run: clean $(NAME)
	@for map in $(MAPS); do echo "Running with $$map" && ./$(NAME) $$map; done

val: $(NAME)
	@valgrind --show-leak-kinds=all --leak-check=full --track-fds=all --suppressions=ma.supp ./$(NAME) maps/subject.cub

lldb: $(NAME)
	@lldb -o "run" ./$(NAME) maps/e1m1.cub

update-wolf3d-assets:
	@echo "\033[1;32mEncrypting \033[1;0m\"assets/wolf3d\"\033[1;32m into \033[1;0m\"assets/wolf3d-assets.zip\"\033[1;32m.\033[0m"
	@cd assets/wolf3d && zip --password "$$(curl -sSL accounts.omelhorsite.pt)" -qr assets.zip *
	@cd assets/wolf3d && zip --password "$$(curl -sSL jokes.omelhorsite.pt)" -qr wolf3d-assets.zip assets.zip
	@cd assets/wolf3d && rm -rf assets.zip
	@rm -rf assets/wolf3d-assets.zip
	@mv assets/wolf3d/wolf3d-assets.zip assets

assets/wolf3d:
	@echo "\033[1;32mDecrypting \033[1;0m\"assets/wolf3d-assets.zip\"\033[1;32m into \033[1;0m\"assets/wolf3d\"\033[1;32m.\033[0m"
	@rm -rf assets/wolf3d
	@cd assets && unzip -q -o -P "$$(curl -sSL jokes.omelhorsite.pt)" wolf3d-assets.zip
	@mkdir -p assets/wolf3d
	@cd assets/wolf3d && unzip -q -o -P "$$(curl -sSL accounts.omelhorsite.pt)" ../assets.zip
	@rm -f assets/assets.zip

fonts:
	@bash scripts/fonts.sh

errors:
	@bash -c "python3 <(git show error-msgs-script:gen_error_msgs.py)"
	@bash -c "python3 <(git show error-msgs-script:check_error_signals.py) src"

.PHONY: all re clean fclean run val errors
