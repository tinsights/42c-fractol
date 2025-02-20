NAME = fractol

CFLAGS = -Wall -Werror -Wextra -g
LIBFLAGS = -Lmlx -lmlx -lXext -lX11 -lm -Llibft -lft 
INC = -Imlx -Ilibft/includes -I/opt/X11/include

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	# might be version dependent, unsure
	INC += -I/opt/X11/include
	LIBFLAGS += -L/opt/X11/lib
endif

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
	cc $(CFLAGS) -O3 $(OBJS) $(LIBFLAGS) $(INC) -o $(NAME)

%.o: %.c fractol.h
	cc $(CFLAGS) -c $< $(INC) -o $@

debug: $(OBJS) $(LIBFT) $(MLX)
	cc $(CFLAGS) -g -fsanitize=thread $(OBJS) $(LIBFLAGS) $(INC) -o $(NAME)_db

$(MLX):
	@if [ ! -d $(MLXDIR) ]; then \
		git clone $(MLX_URL) $(MLXDIR);\
	fi
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