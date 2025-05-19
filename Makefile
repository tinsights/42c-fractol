NAME = fractol

CFLAGS = -Wall -Werror -Wextra -g
LIBFLAGS = -Lmlx -lmlx -lXext -lX11 -lm -Llibft -lft 
INC = -Imlx -Ilibft/includes

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	# might be version dependent, unsure
	INC += -I/opt/X11/include
	LIBFLAGS += -L/opt/X11/lib
endif

LIBDIR = libft/
LIBFT = $(LIBDIR)/libft.a

MLX_URL = git@github.com:42Paris/minilibx-linux.git
MLXDIR = mlx/
MLX =  $(MLXDIR)/libmlx.a

SRCS = fractol.c \
		colours.c \
		hooks.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(MLX) $(LIBFT) $(OBJS) Makefile fractol.h
	cc $(CFLAGS) -O3 $(OBJS) $(LIBFLAGS) $(INC) -o $(NAME)

%.o: %.c fractol.h
	cc $(CFLAGS) -c $< $(INC) -o $@

debug: $(MLX) $(OBJS) $(LIBFT) 
	cc $(CFLAGS) -g -fsanitize=thread $(OBJS) $(LIBFLAGS) $(INC) -o $(NAME)_db

$(MLX):
	@if [ ! -d $(MLXDIR) ] || [ ! -f $(MLX) ]; then \
		echo "MLX directory or library not found. Cloning and building..."; \
		rm -rf $(MLXDIR); \
		git clone $(MLX_URL) $(MLXDIR); \
		echo "Building MLX library..."; \
		make -C $(MLXDIR) --silent > /dev/null 2>&1; \
		echo "MLX library built successfully at $(MLX)"; \
	else \
		echo "MLX library already exists. Skipping clone."; \
	fi

$(LIBFT):
	@echo "Building libft..."
	@make -C $(LIBDIR) --silent
	@if [ ! -f $(LIBFT) ]; then \
		echo "Error: libft.a not found."; \
		exit 1; \
	fi
	@echo "Libft library built at $(LIBFT)"


clean:
	@echo "Cleaning up object files..."
	@rm -f $(OBJS)
	@make -C $(LIBDIR) --silent clean
	@make -C $(MLXDIR) --silent clean
	@echo "Cleaned up object files."

fclean: clean
	make -C $(LIBDIR) fclean --silent
	rm -f $(NAME)

re: fclean all

echo:
	echo $(MAKE)
.PHONY: all clean fclean re