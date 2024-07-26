NAME = fractol

CFLAGS = -Wall -Werror -Wextra -O3
LIBFLAGS = -Lmlx -lmlx -lXext -lX11 -lm -Llibft -lft
INC = -Imlx -Ilibft/includes

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

$(NAME): $(LIBFT) $(MLX) $(OBJS) Makefile fractol.h
	cc $(CFLAGS) $(OBJS) $(LIBFLAGS) $(INC) -o $(NAME)

$(OBJS): %.o: %.c
	cc $(CFLAGS) -c $< -o $@ $(INC)

$(MLX):
	@if [ ! -d $(MLXDIR) ] || [ ! -f $(MLX) ]; then \
		echo "MLX directory or library not found. Cloning and building..."; \
		rm -rf $(MLXDIR); \
		git clone $(MLX_URL) $(MLXDIR); \
		make -C $(MLXDIR); \
	else \
		echo "MLX library already exists. Skipping clone."; \
	fi

$(LIBFT):
	make -C $(LIBDIR)

clean:
	rm -f $(OBJS)

fclean: clean
	$(MAKE) -C $(LIBDIR) fclean
	$(MAKE) -C $(MLXDIR) clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re