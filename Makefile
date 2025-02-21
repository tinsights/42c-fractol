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

$(NAME): $(MLX) $(OBJS) $(LIBFT)  Makefile fractol.h
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
	$(MAKE) -C $(MLXDIR) fclean
	rm -rf $(MLXDIR)
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re