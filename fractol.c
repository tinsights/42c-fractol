/* ************************************************************************** */
/*                                                                           */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 17:08:06 by tjegades          #+#    #+#             */
/*   Updated: 2024/02/07 17:08:07 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

int	close_window(t_params *p)
{
	mlx_destroy_image(p->mlx, p->img);
	mlx_destroy_window(p->mlx, p->win);
	mlx_destroy_display(p->mlx);
	free(p->mlx);
	exit(1);
	return (1);
}

void	reset_view(t_params *p)
{
	p->view.origin_pixel.x = 0;
	p->view.origin_pixel.y = 0;
	p->view.origin_pixel.a = -2;
	p->view.origin_pixel.b = 2;
	p->view.zoom = 1;
	p->view.zoom_count = 0;
	p->view.pixel_unit = 100;
	p->view.col_scheme = 1;
	p->view.invert = 0;
	p->view.max_iter = 50;
	p->view.scale = p->view.zoom * p->view.pixel_unit;
}

int	key_hook(int keycode, t_params *p)
{
	if (keycode == XK_Escape)
		return (close_window(p));
	else if (keycode == 65361)
		p->view.origin_pixel.a -= 0.25 / p->view.zoom;
	else if (keycode == 65363)
		p->view.origin_pixel.a += 0.25 / p->view.zoom;
	else if (keycode == 65364)
		p->view.origin_pixel.b -= 0.25 / p->view.zoom;
	else if (keycode == 65362)
		p->view.origin_pixel.b += 0.25 / p->view.zoom;
	else if (keycode == 114)
		reset_view(p);
	else if (keycode == 65451)
		p->view.max_iter += 10;
	else if (keycode == 65453)
		p->view.max_iter -= 10;
	else if (keycode == 32)
		p->view.col_scheme = (p->view.col_scheme + 1) % 2;
	else if (keycode == 65289)
		p->view.invert = (p->view.invert + 1) % 2;
	return (draw(p));
}

unsigned int	interpolate_color(int color1, int color2, float t)
{
	unsigned char	c1;
	unsigned char	c2;
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;

	c1 = (color1 >> 16) & 0xFF;
	c2 = (color2 >> 16) & 0xFF;
	r = (c1 + (c2 - c1) * t);
	c1 = (color1 >> 8) & 0xFF;
	c2 = (color2 >> 8) & 0xFF;
	g = (c1 + (c2 - c1) * t);
	c1 = color1 & 0xFF;
	c2 = color2 & 0xFF;
	b = (c1 + (c2 - c1) * t);
	return ((r << 16) | (g << 8) | b);
}

unsigned int	map_ratio_to_color(float ratio, t_params *p)
{
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;

	ratio = ratio * 2 * M_PI + log(p->view.zoom);
	r = ((sin(ratio) + 1) / 2 * 255);
	g = ((sin(ratio + 2 * M_PI / 3) + 1) / 2 * 255);
	b = ((sin(ratio + 4 * M_PI / 3) + 1) / 2 * 255);
	return ((r << 16) | (g << 8) | b);
}

unsigned int	color_map(int iter, int max_iter, t_params *p)
{
	double			iter_ratio;
	unsigned int	color;

	iter_ratio = (float) iter / max_iter;
	color = map_ratio_to_color(log(p->view.zoom), p);
	if (iter == max_iter)
		return (black);
	if (p->view.col_scheme)
	{
		if (iter_ratio > 0.5)
			return (interpolate_color(color, white, iter_ratio * 2 - 1));
		else
			return (interpolate_color(black, color, iter_ratio / 0.5));
	}
	if (iter_ratio > 0.5)
	{
		color = map_ratio_to_color(iter_ratio * 2 - 1, p);
		return (interpolate_color(color, white, iter_ratio * 2 - 1));
	}
	else
		return (map_ratio_to_color(iter_ratio / 0.5, p));
}

int	check_if_bounded(t_point zn, t_point c, int max_iter, enum e_mode mode)
{
	double	a;
	int		iter;

	iter = 0;
	while (iter < max_iter)
	{
		a = zn.a;
		if (mode == burning)
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
	return (iter);
}

unsigned int	pixel_color(t_point pt, t_params *p)
{
	t_point	zn;
	t_point	c;
	int		iter;
	int		max_iter;

	max_iter = p->view.max_iter + p->view.zoom_count;
	zn.a = 0.0;
	zn.b = 0.0;
	c = pt;
	if (p->fractol == julia)
	{
		zn = pt;
		c = p->view.c;
	}
	iter = check_if_bounded(zn, c, max_iter, p->fractol);
	return (color_map(iter, max_iter, p));
}

int	draw(t_params *p)
{
	t_point			point;
	unsigned int	colour;
	int				i;
	int				j;
	char			*pixel;

	j = -1;
	while (++j < HEIGHT)
	{
		i = -1;
		while (++i < WIDTH)
		{
			point.a = p->view.origin_pixel.a + (double) i / (p->view.scale);
			point.b = p->view.origin_pixel.b - (double) j / (p->view.scale);
			colour = pixel_color(point, p);
			if (p->view.invert)
				colour = colour ^ white;
			pixel = p->addr + j * p->line_size + i * (p->bpp / 8);
			*(unsigned int *) pixel = mlx_get_color_value(p->mlx, colour);
		}
	}
	mlx_put_image_to_window(p->mlx, p->win, p->img, 0, 0);
	return (1);
}

int	mouse_hook(int button, int x, int y, t_params *p)
{
	t_point	point;

	point.a = p->view.origin_pixel.a + (double) x / (p->view.scale);
	point.b = p->view.origin_pixel.b - (double) y / (p->view.scale);
	point.x = x;
	point.y = y;
	p->btn_clicked = button;
	if (button == 4)
		p->view.zoom = p->view.zoom * 1.1;
	else if (button == 5)
		p->view.zoom = p->view.zoom * (1 / 1.1);
	else if (button == 1)
		p->view.clicked = point;
	else if (button == 3 && p->fractol == mandlebrot)
	{
		p->view.c = point;
		p->fractol = julia;
		p->dragging = true;
	}
	p->view.scale = p->view.zoom * p->view.pixel_unit;
	p->view.origin_pixel.a = point.a - (double) x / (p->view.scale);
	p->view.origin_pixel.b = point.b + (double) y / (p->view.scale);
	p->view.zoom_count += (button == 4) + (-1) * (button == 5);
	return (draw(p));
}

int	explore_julia(int x, int y, t_params *p)
{
	t_point	point;

	point.a = p->view.origin_pixel.a + (double) x / (p->view.scale);
	point.b = p->view.origin_pixel.b - (double) y / (p->view.scale);
	p->view.c = point;
	draw(p);
	return (1);
}

int	drag(int x, int y, t_params *p)
{
	t_point	pt;
	float	xratio;
	float	yratio;

	pt = p->view.clicked;
	if (p->btn_clicked == 1)
	{
		if (abs(pt.x - x) * abs(pt.y - y) > 10)
		{
			xratio = (float)(x - p->view.clicked.x) / p->view.pixel_unit;
			yratio = (float)(y - p->view.clicked.y) / p->view.pixel_unit;
			p->view.origin_pixel.a -= xratio / p->view.zoom;
			p->view.origin_pixel.b += yratio / p->view.zoom;
			p->view.clicked.x = x;
			p->view.clicked.y = y;
			draw(p);
		}
	}
	else if (p->btn_clicked == 3)
		explore_julia(x, y, p);
	return (1);
}

int	switch_fractol(int button, int x, int y, t_params *p)
{
	if (button == 3 && p->dragging && p->fractol != burning)
		p->fractol = (p->fractol + 1) % 2;
	else if (button == 8)
		p->fractol = (p->fractol - 1) % 3;
	else if (button == 9)
		p->fractol = (p->fractol + 1) % 3;
	draw(p);
	p->dragging = false;
	return (x * y);
}

int	validate_input(int ac, char **av, t_params *p)
{
	if (ac == 1
		|| ac == 3
		|| (ac == 2 && ft_strncmp("mandlebrot", av[1], 11) 
			&& ft_strncmp(av[1], "burning", 8))
		|| (ac == 4 && ft_strncmp("julia", av[1], 6))
		|| ac > 4)
	{
		ft_printf("USAGE: \n\t./fractol [mandlebrot | burning]\n");
		ft_printf("./fractol julia [RE(Z)] [IM(Z)]\n");
		return (0);
	}
	if (ac == 4)
	{
		p->fractol = julia;
		p->view.c.a = (double) ft_atoi(av[2]) / 1000;
		p->view.c.b = (double) ft_atoi(av[3]) / 1000;
	}
	else if (!ft_strncmp(av[1], "burning", 8))
		p->fractol = burning;
	else
		p->fractol = mandlebrot;
	return (1);
}

int	main(int ac, char **av)
{
	t_params	p;

	if (!validate_input(ac, av, &p))
		return (-1);
	p.mlx = mlx_init();
	p.win = mlx_new_window(p.mlx, WIDTH, HEIGHT, "fract-ol");
	p.img = mlx_new_image(p.mlx, WIDTH, HEIGHT);
	p.addr = mlx_get_data_addr(p.img, &(p.bpp), &(p.line_size), &(p.endian));
	reset_view(&p);
	draw(&p);
	mlx_key_hook(p.win, key_hook, &p);
	mlx_mouse_hook(p.win, mouse_hook, &p);
	mlx_hook(p.win, 6, 1L << 13, drag, &p);
	mlx_hook(p.win, 5, 1L << 3, switch_fractol, &p);
	mlx_hook(p.win, 17, 0, &close_window, &p);
	mlx_loop(p.mlx);
}
