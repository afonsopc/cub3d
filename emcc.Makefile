# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    emcc.Makefile                                      :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/26 17:16:21 by afpachec          #+#    #+#              #
#    Updated: 2025/07/01 23:44:17 by afpachec         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3d
CC = emcc
CFLAGS = -Wall -Wextra -Werror -O3
INCLUDES = -I headers
LIBS = -L lib
LDLIBS = -lSDL2 -lm -ldl -lpthread
SRCS = $(shell find src -name "**.c")
OBJ_DIR = obj
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))
MAPS = $(wildcard maps/*.cub)
UNAME_S = $(shell uname -s)
RAND=$(shell echo $$RANDOM)
FT_AUDIO_DIR = src/ft_audio

check_flag = $(shell $(CC) $(1) -E -c /dev/null -o /dev/null 2>/dev/null && echo 1 || echo 0)
ifeq ($(CC),emcc)
ifeq ($(UNAME_S),Darwin)
	LIBS += -L /opt/X11/lib
	INCLUDES += -I /opt/X11/include
endif
	LDLIBS += -s USE_SDL=2 -s ASSERTIONS=1 -s SAFE_HEAP=1 -s STACK_OVERFLOW_CHECK=2
	LDLIBS += --preload-file maps --preload-file assets --shell-file minimal.html
	LDLIBS += -s INITIAL_MEMORY=134217728 -s STACK_SIZE=2MB
else ifeq ($(UNAME_S),Darwin)
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
