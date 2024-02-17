NAME = fractol

CFLAGS = -Wall -Werror -Wextra
LIBFLAGS = -Lmlx -lmlx -lXext -lX11 -lm -Llibft -lft
INC = -Imlx -Ilibft/includes

LIBDIR = libft/
LIBFT = $(LIBDIR)/libft.a

MLXDIR = mlx/
MLX =  $(MLXDIR)/libmlx.a

SRCS = main.c

all: $(NAME)

$(NAME): $(SRCS) $(LIBFT) $(MLX)
	cc $(CFLAGS) $(SRCS) $(LIBFLAGS) $(INC) -o $(NAME)

$(MLX):
	make -C $(MLXDIR)

$(LIBFT):
	make -C $(LIBDIR)

clean:
	rm -f *.o

fclean: clean
	$(MAKE) -C $(LIBDIR) fclean
	$(MAKE) -C $(MLXDIR) clean
	rm -f $(NAME)

re: fclean all

.PHONY: clean fclean re all