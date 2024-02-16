NAME = mlx_lab

CFLAGS = -Wall -Werror -Wextra
LIBFLAGS = -Lmlx -lmlx -lXext -lX11 -lm -Llibft -lft
INC = -Imlx -Ilibft/includes

all: $(NAME)

$(NAME): libft
	cc $(CFLAGS) main.c $(LIBFLAGS) $(INC)

libft:
	make -C libft

.PHONY: libft