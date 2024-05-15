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
	p->view.pixel_unit = SIDE / 4;
	p->view.col_scheme = 1;
	p->view.invert = 0;
	p->view.max_iter = 100;
	p->view.scale = p->view.zoom * p->view.pixel_unit;
}

#include <pthread.h>

typedef struct s_row_thread  
{
	t_params 	*p;
	int			id;
	int			rows;
	int			rowstart;

} t_row;
// routine for each thread

#include <stdio.h>
void *calc_line(void *args)
{
	t_point 		point;
	unsigned int	colour;
	char 			*pixel;
	int 			i;

	t_row *row_args = args;

	t_params *p = row_args->p;

	int j = -1;
	// printf("\tin thread: %i\n\t\trowstart: %i, rows: %i\n",row_args->id,  row_args->rowstart, row_args->rows);

	while (++j < row_args->rows)
	{
		int row = row_args->rowstart + j;
		point.b = p->view.origin_pixel.b - (double) (row) / (p->view.scale);
		i = -1;
		while (++i < SIDE)
		{
			point.a = p->view.origin_pixel.a + (double) i / (p->view.scale);
			colour = pixel_color(point, p);
			if (p->view.invert)
				colour = colour ^ white;
			pixel = p->addr + row * p->line_size + i * (p->bpp / 8);
			*(unsigned int *) pixel = mlx_get_color_value(p->mlx, colour);
		}
	}
	return (NULL);
}



int	draw(t_params *p)
{
	int				j;


	// pthread_mutex_lock(&p->rendering);

	t_row rows[THREADS];

	j = -1;
	while (++j < THREADS)
	{
    	// create a thread for each row of pixels
		rows[j].p = p;
		rows[j].id = j;
		rows[j].rows = SIDE / THREADS;
		rows[j].rowstart = j * rows[j].rows;
		pthread_create(p->threads + j, NULL, &calc_line, rows + j);
	}
	j = -1;
	while (++j < THREADS)
	{
		pthread_join(p->threads[j], NULL);
		// printf("thread %i complete\n", j);

	}
	// pthread_mutex_unlock(&p->rendering);

	mlx_put_image_to_window(p->mlx, p->win, p->img, 0, 0);
	return (1);
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
		ft_printf("\t./fractol julia [RE(Z)] [IM(Z)]\n");
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
	p.win = mlx_new_window(p.mlx, SIDE, SIDE, "fract-ol");
	p.img = mlx_new_image(p.mlx, SIDE, SIDE);
	p.addr = mlx_get_data_addr(p.img, &(p.bpp), &(p.line_size), &(p.endian));
	p.threads = (pthread_t *) ft_calloc(THREADS, sizeof(pthread_t));
	pthread_mutex_init(&p.rendering, NULL);
	mlx_do_key_autorepeaton(p.mlx);

	reset_view(&p);
	mlx_key_hook(p.win, key_hook, &p);
	mlx_mouse_hook(p.win, mouse_hook, &p);
	mlx_hook(p.win, 6, 1L << 13, drag, &p);
	mlx_hook(p.win, 5, 1L << 3, switch_fractol, &p);
	mlx_hook(p.win, 17, 0, &close_window, &p);
	mlx_loop_hook(p.mlx, draw, &p);
	mlx_loop(p.mlx);
}
