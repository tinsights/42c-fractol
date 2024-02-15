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


typedef struct t_point
{
	long long	x;
	long long	y;

	double a;
	double b;
} s_point;

typedef struct t_view
{
	s_point	origin_pixel;
	s_point	translation;
	int		zoom_count;
	int		pixel_unit;
	double	zoom;
} s_view;

typedef struct t_params
{
	void	*mlx;
	void	*win;
	void	*img;
	void	*addr;

	s_view	view;

	int		bpp;
	int		line_size;
	int		endian;

} s_params;

void draw(s_params *params);


int close_window(s_params *p)
{
	mlx_destroy_image(p->mlx, p->img);
	mlx_destroy_window(p->mlx, p->win);
	mlx_destroy_display(p->mlx);
	free(p->mlx);
	exit(1);
	return (1);
}

int key_hook(int keycode, s_params *p)
{
	if (keycode == 65307)
		close_window(p);
	else if(keycode == 65361)
	{
		p->view.origin_pixel.a -= 0.25 / p->view.zoom;
		draw(p);
	}
	else if(keycode == 65363)
	{
		p->view.origin_pixel.a += 0.25 / p->view.zoom;
		draw(p);
	}
	else if (keycode == 65364)
	{
		p->view.origin_pixel.b -= 0.25 / p->view.zoom;
		draw(p);
	}
	else if (keycode == 65362)
	{
		p->view.origin_pixel.b += 0.25 / p->view.zoom;
		draw(p);
	}
	else if (keycode == 114)
	{
		// reset view
		p->view.zoom = 1;
		p->view.origin_pixel.a = -2;
		p->view.origin_pixel.b = 1;
		draw(p);
	}
	else
		printf("Key: %i\n", keycode);
	return (1);
}

int is_bounded(s_point point)
{
	s_point z;
	double a;

	z.a = 0;
	z.b = 0;
	int max_iter = 50;
	int iter = 0;

	while (iter < max_iter)
	{
		a = z.a;
		
		z.a = (a * a) - (z.b * z.b) + point.a;
		z.b = (2 * a * z.b) + point.b;
		if ((z.a * z.a + z.b * z.b) > 4.0)
			break ;
		iter++;
	}
	// get a colour between red and pink
	// map 0 - max to 0x00ff0000 and 0x00000000
	int red = 0x00ff0000;
	int cmax = 0x00ffd0db;
	int range = cmax - red;

	// printf("%f \n", ((double)iter / max_iter));
	return (cmax - ((double)iter / max_iter) * range);
}

int is_bounded_heart(s_point point, s_params *p)
{
	s_point z;
	double a;

	z.a = 0;
	z.b = 0;
	int max_iter = (p->view.zoom_count < 50) * 50 + !(p->view.zoom_count < 50) * p->view.zoom_count;
	int iter = 0;
	while (iter < max_iter)
	{
		a = z.a;
		// z.a = (fabs(a) * a - (z.b * z.b)) + point.a;
		// z.b = (fabs(a) * z.b * 2) + point.b;

		z.a = (a * a) - (z.b * z.b) + point.a;
		z.b = (2 * a * z.b) + point.b;
		if ((z.a * z.a + z.b * z.b) > 4)
			break ;
		iter++;
	}
	// get a colour between red and pink
	// map 0 - max to 0x00ff0000 and 0x00000000
	int red = 0x00ff0000;
	int cmax = 0x00ffc0cb;
	int range = cmax - red;

	// printf("%f \n", ((double)iter / max_iter));
	return (cmax - ((double)iter / max_iter) * range);
}

void draw(s_params *params)
{
	s_view view = params->view;

	for (int j = 0; j < HEIGHT; j++)
	{
		for (int i = 0; i < WIDTH; i++)
		{
			s_point point;
			point.a = view.origin_pixel.a + (double) i / (view.zoom * view.pixel_unit);
			point.b = view.origin_pixel.b - (double) j / (view.zoom * view.pixel_unit);
			int colour = is_bounded_heart(point, params);
			// if(colour)
			// 	printf("colour: %i %u\n", colour, mlx_get_color_value(params->mlx, colour));
			char *pixel = params->addr + j * params->line_size + i * (params->bpp / 8);
			*(unsigned int *) pixel = mlx_get_color_value(params->mlx, colour);
		}
	}
	mlx_put_image_to_window(params->mlx, params->win, params->img, 0, 0);
}


int mouse_hook(int button, int x, int y, s_params *p)
{
	s_point point;
	point.a = p->view.origin_pixel.a + (double) x / (p->view.zoom * p->view.pixel_unit);
	point.b = p->view.origin_pixel.b - (double) y / (p->view.zoom * p->view.pixel_unit);

	if (button == 4)
	{

		p->view.zoom = p->view.zoom * 1.1;
		p->view.zoom_count++;

		p->view.origin_pixel.a = point.a - (double) x / (p->view.zoom * p->view.pixel_unit);
		p->view.origin_pixel.b = point.b + (double) y / (p->view.zoom * p->view.pixel_unit);

	}
	else if (button == 5)
	{
		p->view.zoom = p->view.zoom * 0.9;
		p->view.zoom_count--;

		p->view.origin_pixel.a = point.a - (double) x / (p->view.zoom * p->view.pixel_unit);
		p->view.origin_pixel.b = point.b + (double) y / (p->view.zoom * p->view.pixel_unit);
	}
	// if (p->view.zoom_count > 200)
	// {
	// 	printf("zc: %i zoom: %lf\n", p->view.zoom_count, p->view.zoom);
	// 	printf("ox: %lli oy: %lli\n", p->view.origin_pixel.x, p->view.origin_pixel.y);

	// 	printf("a: %lf b: %lf\n", point.a, point.b);
	// }
	if (button || x || y)
		draw(p);
	return (1);
}

int	main(void)
{
	s_params params;

	params.mlx = mlx_init();
	params.win = mlx_new_window(params.mlx, WIDTH, HEIGHT, "test");
	params.img = mlx_new_image(params.mlx, WIDTH, HEIGHT);
	params.addr = mlx_get_data_addr(params.img, &(params.bpp), &(params.line_size), &(params.endian));
	
	params.view.origin_pixel.x = 0;
	params.view.origin_pixel.y = 0;
	params.view.origin_pixel.a = -2;
	params.view.origin_pixel.b = 1;
	params.view.zoom_count = 0;

	params.view.zoom = 1;
	params.view.pixel_unit = 200;

	// draw(&params);
	// close window with ESC key
	// TODO: arrow keys to "move"
	mlx_key_hook(params.win, key_hook, &params);

	mlx_mouse_hook(params.win, mouse_hook, &params);

	// close window with x button
	// TODO: understand event masking?
	mlx_hook(params.win, 17, 0, &close_window, &params);
	mlx_loop(params.mlx);
}