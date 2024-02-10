NAME = mlx_lab

CFLAGS = -Wall -Werror -Wextra
LIBFLAGS = -Lmlx -lmlx -lXext -lX11
INC = -Imlx

all: $(NAME)

$(NAME):
	cc $(CFLAGS) main.c $(LIBFLAGS) $(INC)