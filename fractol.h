/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 13:37:32 by tjegades          #+#    #+#             */
/*   Updated: 2024/02/13 13:37:33 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H
# define FRACTOL_H

# include <mlx.h>
# include <X11/keysym.h>

# include <limits.h>
# include <math.h>
# include <stdlib.h>
# include <stdbool.h>
# include "libft.h"

# define HEIGHT 400
# define WIDTH 400

enum e_colours{black= 0x00000000, white = 0x00ffffff};
enum e_mode{mandlebrot, julia, burning};

typedef struct s_point
{
	int		x;
	int		y;

	double	a;
	double	b;
}	t_point;

typedef struct s_view
{
	t_point	origin_pixel;
	t_point	translation;

	double	zoom;
	int		zoom_count;
	int		pixel_unit;
	double	scale;

	int		col_scheme;
	int		invert;

	int		max_iter;

	t_point	c;
	t_point	clicked;
}	t_view;

typedef struct s_params
{
	void		*mlx;
	void		*win;
	void		*img;
	void		*addr;

	t_view		view;

	int			bpp;
	int			line_size;
	int			endian;

	enum e_mode	fractol;
	int			btn_clicked;
	bool		dragging;
}	t_params;

unsigned int	pixel_color(t_point pt, t_params *p);
int				draw(t_params *params);
int				key_hook(int keycode, t_params *p);
int				mouse_hook(int button, int x, int y, t_params *p);
int				explore_julia(int x, int y, t_params *p);
int				drag(int x, int y, t_params *p);
int				switch_fractol(int button, int x, int y, t_params *p);
int				close_window(t_params *p);
void			reset_view(t_params *p);

#endif