/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colours.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 21:30:43 by tjegades          #+#    #+#             */
/*   Updated: 2024/02/17 21:30:44 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

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
	// int		max_iter;

	// max_iter = p->view.max_iter + p->view.zoom_count;
	zn.a = 0.0;
	zn.b = 0.0;
	c = pt;
	if (p->fractol == julia)
	{
		zn = pt;
		c = p->view.c;
	}
	iter = check_if_bounded(zn, c, p->view.max_iter, p->fractol);
	return (color_map(iter, p->view.max_iter, p));
}
