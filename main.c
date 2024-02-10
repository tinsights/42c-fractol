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

int close_window(s_params *p)
{
	mlx_destroy_window(p->mlx_ptr, p->win_ptr);
	mlx_destroy_display(p->mlx_ptr);
	free(p->mlx_ptr);
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

int	main(void)
{
	s_params params;

	params.mlx_ptr = mlx_init();
	params.win_ptr = mlx_new_window(params.mlx_ptr, 990, 300, "test");

	mlx_key_hook(params.win_ptr, press, &params);
	// mlx_mouse_hook(par	ams.win_ptr, mouse, &params);
	mlx_hook(params.win_ptr, 17, 1L<<3, &close_window, &params); // close window
	mlx_hook(params.win_ptr, 25, 0, &test, &params); // close window	mlx_loop(params.mlx_ptr);
	mlx_hook(params.win_ptr, 7, 0, &test, &params); // close window	mlx_loop(params.mlx_ptr);
	mlx_hook(params.win_ptr, 22, 0, &test, &params); // close window	mlx_loop(params.mlx_ptr);
	mlx_loop(params.mlx_ptr);
}