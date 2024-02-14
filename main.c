/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 17:08:06 by tjegades          #+#    #+#             */
/*   Updated: 2024/02/07 17:08:07 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <mlx.h>
#include <limits.h>
#include <math.h>

#include <stdio.h>

#include <stdio.h>
#include <stdlib.h>

# define HEIGHT 400

# define WIDTH 800


typedef struct t_params
{
	void	*mlx;
	void	*win;
	void	*img;

	int		bpp;
	int		line_size;
	int		endian;

} s_params;

typedef struct t_point
{
	int	x;
	int	y;

	float a;
	float b;
} s_point;

typedef struct t_view
{
	s_point	origin;
	s_point	translation;
	float	zoom;
} s_view;

int close_window(s_params *p)
{
	mlx_destroy_image(p->mlx, p->img);
	mlx_destroy_window(p->mlx, p->win);
	mlx_destroy_display(p->mlx);
	free(p->mlx);
	exit(1);
	return (1);
}

int press(int keycode, s_params *p)
{
	printf("Key: %i\n", keycode);
	if (keycode == 65307)
		close_window(p);
	return (1);
}

int test(s_params *p)
{
	printf("hello %p\n", p);
	return (1);
}


int is_bounded(s_point point)
{
	int max_iter = 50;

	s_point z;
	z.a = 0;
	z.b = 0;

	while (max_iter > 0)
	{
		s_point old_z;
		old_z = z;
		z.a = (old_z.a * old_z.a) - (old_z.b * old_z.b) + point.a;
		z.b = (2 * old_z.a * old_z.b) + point.b;
		if ((z.a * z.a + z.b * z.b) > 4)
			break ;
		max_iter--;
	}
	return max_iter;
}

int	main(void)
{
	s_params params;

	params.mlx = mlx_init();
	params.win = mlx_new_window(params.mlx, WIDTH, HEIGHT, "test");
	params.img = mlx_new_image(params.mlx, WIDTH, HEIGHT);
	
	char *addr = mlx_get_data_addr(params.img, &params.bpp, &params.line_size, &params.endian);

	// create origin
	s_view view;

	view.origin.x = WIDTH / 2;
	view.origin.y = HEIGHT / 2;
	view.origin.a = 0;
	view.origin.b = 0;

	view.zoom = 200;

	for (int j = 0; j < HEIGHT; j++)
	{
		for (int i = 0; i < WIDTH; i++)
		{
			s_point point;
			point.a = (i - view.origin.x) / view.zoom;
			point.b = (j - view.origin.y) / view.zoom;
			// printf("x: %i y: %i \n", point.x, point.y);
			if (!is_bounded(point))
			{
				printf("the point %f + %fi is bounded\n", point.a, point.b);
				char *pixel = addr + j * params.line_size + i * (params.bpp / 8);
				*(unsigned int *) pixel = 0x00ffffff;
			}
		}
	}

	// int j = 50;
	// for (int i = 0; i < WIDTH; i++)
	// {
	// 	char *pixel = addr + j * params.line_size + i * (params.bpp / 8);
	// 	*(unsigned int *) pixel = 0x00ff0000;
	// }
	mlx_put_image_to_window(params.mlx, params.win, params.img, 0, 0);

	// close window with ESC key
	// TODO: arrow keys to "move"
	mlx_key_hook(params.win, press, &params);

	// close window with x button
	// TODO: understand event masking?
	mlx_hook(params.win, 17, 0, &close_window, &params);
	mlx_loop(params.mlx);
}