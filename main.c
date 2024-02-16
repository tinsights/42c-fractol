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

#include <X11/keysym.h>

#include "libft.h"
#include <stdlib.h>

# define HEIGHT 400

# define WIDTH 800
enum colours{BLACK= 0x00000000, WHITE = 0x00ffffff};
enum fractol{MANDLEBROT, JULIA, BURNING};

typedef struct t_point
{
	int	x;
	int	y;

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
	int		col_scheme;
	int		invert;

	int		max_iter;
	s_point	c;
	s_point clicked;
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

	enum fractol fractol;
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
	if (keycode == XK_Escape)
		return close_window(p);
	else if(keycode == 65361)
		p->view.origin_pixel.a -= 0.25 / p->view.zoom;
	else if(keycode == 65363)
		p->view.origin_pixel.a += 0.25 / p->view.zoom;
	else if (keycode == 65364)
		p->view.origin_pixel.b -= 0.25 / p->view.zoom;
	else if (keycode == 65362)
		p->view.origin_pixel.b += 0.25 / p->view.zoom;
	else if (keycode == 114)
	{
		// reset view
		p->view.zoom = 1;
		p->view.zoom_count = 0;
		p->view.origin_pixel.a = -2;
		p->view.origin_pixel.b = 1;
		p->view.max_iter = 80;
	}
	else if (keycode == 65451)
		p->view.max_iter += 10;
	else if (keycode == 65453)
		p->view.max_iter -= 10;
	else if (keycode == 32)
		p->view.col_scheme = (p->view.col_scheme + 1) % 2;
	else if (keycode == 65289)
		p->view.invert = (p->view.invert + 1) % 2;
	else
		printf("Key: %i\n", keycode);
	draw(p);
	return (1);
}

unsigned int interpolateColor(unsigned int color1, unsigned int color2, float t) {
    // Extract individual color components
    unsigned char r1 = (color1 >> 16) & 0xFF;
    unsigned char g1 = (color1 >> 8) & 0xFF;
    unsigned char b1 = color1 & 0xFF;
    
    unsigned char r2 = (color2 >> 16) & 0xFF;
    unsigned char g2 = (color2 >> 8) & 0xFF;
    unsigned char b2 = color2 & 0xFF;

    // Interpolate each color component
    unsigned char r = (unsigned char)(r1 + (r2 - r1) * t);
    unsigned char g = (unsigned char)(g1 + (g2 - g1) * t);
    unsigned char b = (unsigned char)(b1 + (b2 - b1) * t);

    // Pack the interpolated color components into a single integer
    return (r << 16) | (g << 8) | b;
}

unsigned int mapRatioToColor(float ratio, s_params *p) {
    // Scale the ratio to the range of the sine function
    float scaledRatio = ratio * 2 * M_PI + log(p->view.zoom);

    // Use a sine function to introduce oscillations
    float r = (sin(scaledRatio) + 1) / 2; // Red component
    float g = (sin(scaledRatio + 2 * M_PI / 3) + 1) / 2; // Green component
    float b = (sin(scaledRatio + 4 * M_PI / 3) + 1) / 2; // Blue component

    // Scale the color components to the range [0, 255]
    unsigned char red = (unsigned char)(r * 255);
    unsigned char green = (unsigned char)(g * 255);
    unsigned char blue = (unsigned char)(b * 255);

    // Pack the color components into a single integer
    return (red << 16) | (green << 8) | blue;
}

unsigned int is_bounded(s_point pt, s_params *p)
{
	s_point zn;
	s_point c;
	double a;

	zn.a = 0.0;
	zn.b = 0.0;

	if (p->fractol == JULIA)
	{
		zn = pt;
		c = p->view.c;
	}
	else c = pt;

	int max_iter = p->view.max_iter + p->view.zoom_count;
	int iter = 0;
	while (iter < max_iter)
	{
		a = zn.a;

		if (p->fractol == BURNING)
		{
			zn.a = (a * a) - (zn.b * zn.b) - c.a;
			zn.b = (2 * fabs(a * zn.b)) - c.b;
		}
		else
		{
			zn.a = (a * a) - (zn.b * zn.b) + c.a;
			zn.b = (2 * a * zn.b) + c.b;
		}

		if ((zn.a * zn.a + zn.b * zn.b) > 4)
			break ;
		iter++;
	}
	double iter_ratio = (float)iter/max_iter;
	unsigned int color = mapRatioToColor(log(p->view.zoom), p);
	if (iter == max_iter)
			return BLACK;
	if (p->view.col_scheme)
	{
		if (iter_ratio > 0.5)
			return interpolateColor(color, WHITE, iter_ratio * 2 - 1);
		else
			return interpolateColor(BLACK, color, iter_ratio / 0.5);
	}
	if (iter_ratio > 0.5)
		return interpolateColor(mapRatioToColor((iter_ratio - 0.5) / 0.5, p), WHITE, (iter_ratio - 0.5) / 0.5);
	else
		return mapRatioToColor(iter_ratio / 0.5, p);

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

			unsigned int colour = is_bounded(point, params);
			if (params->view.invert)
				colour = colour ^ WHITE;
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
		p->view.zoom = p->view.zoom * (1 / 1.1);
		p->view.zoom_count--;

		p->view.origin_pixel.a = point.a - (double) x / (p->view.zoom * p->view.pixel_unit);
		p->view.origin_pixel.b = point.b + (double) y / (p->view.zoom * p->view.pixel_unit);
	}
	else if (button == 1)
	{
		p->view.clicked.x = x;
		p->view.clicked.y = y;
	}
	else if (button == 2)
	{
		printf("x: %i y: %i, a: %lf b: %lf\n", x, y, point.a, point.b);
		printf("zc: %i, zoom: %lf\n", p->view.zoom_count, p->view.zoom);
	}
	draw(p);
	return (1);
}

int drag(int x, int y, s_params *p)
{
	s_point pt = p->view.clicked;

	if (abs(pt.x - x) * abs(pt.y - y) > 10)
	{
		float xratio = (float)(x - p->view.clicked.x) / p->view.pixel_unit;
		float yratio = (float)(y - p->view.clicked.y) / p->view.pixel_unit;
		p->view.origin_pixel.a -= xratio / p->view.zoom;
		p->view.origin_pixel.b += yratio / p->view.zoom;
		p->view.clicked.x = x;
		p->view.clicked.y = y;
		draw(p);
	}
	return (1);	
}

int	main(int ac, char **av)
{
	if (ac == 1
		|| ac == 3
		|| (ac == 2 && ft_strncmp("mandlebrot", av[1], 11) && ft_strncmp(av[1], "burning", 8))
		|| (ac == 4 && ft_strncmp("julia", av[1], 6))
		|| ac > 4)
		return ft_printf("idiot\n");

	s_params params;

	params.fractol = MANDLEBROT;
	params.mlx = mlx_init();
	params.win = mlx_new_window(params.mlx, WIDTH, HEIGHT, "test");
	params.img = mlx_new_image(params.mlx, WIDTH, HEIGHT);
	params.addr = mlx_get_data_addr(params.img, &(params.bpp), &(params.line_size), &(params.endian));
	


	params.view.origin_pixel.x = 0;
	params.view.origin_pixel.y = 0;
	params.view.origin_pixel.a = -2;
	params.view.origin_pixel.b = 1;

	params.view.zoom = 1;
	params.view.zoom_count = 0;
	params.view.pixel_unit = 200;
	params.view.col_scheme = 1;
	params.view.invert = 0;
	params.view.max_iter = 50;

	if (ac == 4)
	{
		params.fractol = JULIA;
		params.view.c.a = (double) ft_atoi(av[2]) / 1000;
		params.view.c.b = (double) ft_atoi(av[3]) / 1000;
	}
	else if (!ft_strncmp(av[1], "burning", 8))
		params.fractol = BURNING;


	draw(&params);
	// close window with ESC key
	// TODO: arrow keys to "move"
	mlx_key_hook(params.win, key_hook, &params);

	mlx_mouse_hook(params.win, mouse_hook, &params);
	mlx_hook(params.win, 6, 1L<<8, drag, &params);

	// close window with x button
	// TODO: understand event masking?
	mlx_hook(params.win, 17, 0, &close_window, &params);
	mlx_loop(params.mlx);
}