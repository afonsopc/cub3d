# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: afpachec <afpachec@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/26 17:16:21 by afpachec          #+#    #+#              #
#    Updated: 2025/06/27 20:00:21 by afpachec         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3d
CC = cc
CFLAGS = -Wall -Wextra -Werror -O3 -g
# CFLAGS = -Wall -Wextra -Werror -O3 -g -std=c99 -fsanitize=address -fno-omit-frame-pointer
INCLUDES = -I headers
LIBS = -L lib
LDLIBS = -lmlx -lSDL2 -lX11 -lXext -lm -ldl -lpthread
SRCS = $(shell find src -name "**.c")
OBJ_DIR = obj
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))
MAPS = $(wildcard maps/*.cub)
UNAME_S = $(shell uname -s)
RAND=$(shell echo $$RANDOM)
FT_AUDIO_DIR = src/ft_audio

check_flag = $(shell $(CC) $(1) -E -c /dev/null -o /dev/null 2>/dev/null && echo 1 || echo 0)
ifeq ($(UNAME_S),Darwin)
	LIBS += -L /opt/X11/lib
	INCLUDES += -I /opt/X11/include
	LDLIBS += -framework OpenGL
	LDLIBS += -framework AppKit
	LDLIBS += -framework CoreAudio
	LDLIBS += -framework AudioToolbox
	LDLIBS += -framework CoreHaptics
	LDLIBS += -framework CoreVideo
	LDLIBS += -framework GameController
	LDLIBS += -framework IOKit
	LDLIBS += -framework ForceFeedback
	LDLIBS += -framework CoreFoundation
	LDLIBS += -framework Cocoa
	LDLIBS += -framework Carbon 
	LDLIBS += -framework Metal 
else
	ifeq ($(shell $(CC) --version | grep -i clang > /dev/null && echo clang),clang)
    	CFLAGS += -Wno-unknown-warning-option
	else
    	CFLAGS += -Wno-stringop-overflow
	endif
endif

all: $(NAME)

$(NAME): fonts assets/wolf3d lib/libSDL2.a lib/libmlx.a headers/miniaudio.h $(OBJS)
	@echo "\033[1;32mCompiling \033[1;0m\"$(OBJS)\"\033[1;32m into \033[1;0m\"$(NAME)\"\033[1;32m.\033[0m"
	@$(CC) -o $(NAME) $(OBJS) $(CFLAGS) $(INCLUDES) $(LIBS) $(LDLIBS)

lib/libmlx.a:
	@echo "\033[1;32mCompiling \033[1;0m\"minilibx\"\033[1;32m.\033[0m"
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

headers/miniaudio.h:
	@echo "\033[1;32mCompiling \033[1;0m\"miniaudio\"\033[1;32m.\033[0m"
	@rm -rf headers/miniaudio.h
	@tar -xzf lib/miniaudio.tar.gz -C headers

lib/libSDL2.a:
	@echo "\033[1;32mCompiling \033[1;0m\"SDL2\"\033[1;32m.\033[0m"
	@rm -rf lib/SDL2-2.32.8
	@tar -xzf lib/SDL2-2.32.8.tar.gz -C lib
	@mkdir -p lib/SDL2-2.32.8/build
	@cd lib/SDL2-2.32.8/build && ../configure --disable-shared --enable-static --prefix=$(abspath lib) && make
	@cp lib/SDL2-2.32.8/build/build/.libs/libSDL2.a lib/
	@mkdir -p headers/SDL2
	@cp -r lib/SDL2-2.32.8/include/* headers/SDL2/
	@rm -rf lib/SDL2-2.32.8

$(OBJ_DIR)/%.o: %.c
	@echo "\033[1;32mCompiling \033[1;0m\"$<\"\033[1;32m into \033[1;0m\"$@\"\033[1;32m.\033[0m"
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean-everything-but-ft-audio:
	@if [ -d "$(OBJ_DIR)/$(FT_AUDIO_DIR)" ]; then \
		tar -cf ft_audio.tar -C $(OBJ_DIR) $(FT_AUDIO_DIR); \
	fi
	@$(MAKE) clean
	@if [ -f "ft_audio.tar" ]; then \
		mkdir -p $(OBJ_DIR); \
		tar -xf ft_audio.tar -C $(OBJ_DIR); \
		rm -f ft_audio.tar; \
	fi

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -rf $(NAME)
	@rm -rf lib/libmlx.a
	@rm -rf headers/mlx.h
	@rm -rf headers/SDL2/
	@rm -rf lib/libSDL2.a
	@rm -rf headers/miniaudio.h

re: fclean all

cdev: clean-everything-but-ft-audio dev

dev: $(NAME)
	./$(NAME) maps/hub.cub

run: clean $(NAME)
	@for map in $(MAPS); do echo "Running with $$map" && ./$(NAME) $$map; done

val: $(NAME)
	@valgrind --show-leak-kinds=all --leak-check=full --track-fds=all --suppressions=cub.supp ./$(NAME) maps/hub.cub

lldb: $(NAME)
	@lldb -o "run" ./$(NAME) maps/hub.cub

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
