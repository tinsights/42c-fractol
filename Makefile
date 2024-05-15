NAME = fractol

CFLAGS = -Wall -Werror -Wextra -O3
LIBFLAGS = -Lmlx -lmlx -lXext -lX11 -lm -Llibft -lft
INC = -Imlx -Ilibft/includes

LIBDIR = libft/
LIBFT = $(LIBDIR)/libft.a

MLXDIR = mlx/
MLX =  $(MLXDIR)/libmlx.a

SRCS = fractol.c \
		colours.c \
		hooks.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT) $(MLX) Makefile fractol.h
	cc $(CFLAGS) $(OBJS) $(LIBFLAGS) $(INC) -o $(NAME)

debug: $(OBJS) $(LIBFT) $(MLX)
	cc $(CFLAGS) -g -fsanitize=thread $(OBJS) $(LIBFLAGS) $(INC) -o $(NAME)_db

$(OBJS): $(SRCS)
	cc $(CFLAGS) -O3 -c $(SRCS) $(INC)

$(MLX):
	make -C $(MLXDIR)

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