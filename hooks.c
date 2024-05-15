/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 21:35:12 by tjegades          #+#    #+#             */
/*   Updated: 2024/02/17 21:35:13 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

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
	// p->view.zoom_count += (button == 4) + (-1) * (button == 5); // wtf is this?? why do i do this rubbish for norm????
	p->view.max_iter += ((button == 4) + (-1) * (button == 5)) * 5;
	return (1);
}

int	explore_julia(int x, int y, t_params *p)
{
	t_point	point;

	point.a = p->view.origin_pixel.a + (double) x / (p->view.scale);
	point.b = p->view.origin_pixel.b - (double) y / (p->view.scale);
	p->view.c = point;
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
	p->dragging = false;
	return (x * y);
}
