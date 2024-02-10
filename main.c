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

#include <stdio.h>

#include <stdio.h>
#include <stdlib.h>

typedef struct t_params
{
	void *mlx_ptr;
	void *win_ptr;

} s_params;

int close_window(void *params)
{
	s_params *p = (s_params *) params;

	mlx_destroy_window(p->mlx_ptr, p->win_ptr);
	exit(1);
	return (1);
}

int press(int keycode, s_params *p)
{
	printf("Key: %i\n", keycode);
	f (keycode == 65307)
		return (close_window(p));
	return (1);
}

int mouse(int button, int x, int y, void *params)
{
	printf("button: %i\n", button);

	s_params *p = (s_params *) params;
	update_color(p);
	return (0);	
}

int	main(void)
{
	s_params params;

	params.mlx_ptr = mlx_init();
	params.win_ptr = mlx_new_window(params.mlx_ptr, 990, 300, "test");

	mlx_key_hook(params.win_ptr, press, &params);
	mlx_hook(params.win_ptr, 17, 1L<<3, &close_window, &params);
	mlx_mouse_hook(params.win_ptr, mouse, &params);
	mlx_loop(params.mlx_ptr);
}