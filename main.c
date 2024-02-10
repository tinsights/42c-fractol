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

// Define the structure for a node in the linked list
typedef struct ColorNode {
    int color; // Store the RGBA color value in a 4-byte integer
    struct ColorNode* next; // Pointer to the next node
    struct ColorNode* prev; // Pointer to the previous node
} ColorNode;

// Function to create a new node with the given color value
ColorNode* createNode(int color) {
    ColorNode* newNode = (ColorNode*)malloc(sizeof(ColorNode));
    if (newNode == NULL) {
        exit(1);
    }
    newNode->color = color;
    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;
}

// Function to add a node to the end of the circular linked list
void appendNode(ColorNode** head, int color) {
    ColorNode* newNode = createNode(color);
    if (*head == NULL) {
        *head = newNode;
        (*head)->next = *head; // Circular linking
        (*head)->prev = *head; // Circular linking
    } else {
        ColorNode* temp = *head;
        while (temp->next != *head) {
            temp = temp->next;
        }
        temp->next = newNode;
        newNode->prev = temp;
        newNode->next = *head; // Circular linking
        (*head)->prev = newNode; // Update the previous of the head
    }
}

// Function to free the memory allocated for the circular linked list
void freeList(ColorNode** head) {
    if (*head == NULL)
        return;
    ColorNode* temp = (*head)->next;
    while (temp != *head) {
        ColorNode* prev = temp;
        temp = temp->next;
        free(prev);
    }
    free(temp); // Free the last node
    *head = NULL;
}

ColorNode *init_colours() {
    // Define some common colors as RGBA values
    int colors[] = {
        0xFFFF0000, // Red
	    0xFF00FF00, // Green
	    0xFF0000FF, // Blue
	    0xFFFFFF00, // Yellow
	    0xFFFFA500, // Orange
	    0xFF8000FF, // Purple
	    0xFFFF00FF, // Cyan
	    0xFFFF00FF, // Magenta
	    0xFF008000, // Olive
	    0xFF000080, // Maroon
	    0xFF008080, // Teal
	    0xFF808080, // Gray
	    0xFFFFD700, // Gold
	    0xFFF0C0FC, // Silver
	    0xFFFFFFFF,
	    0x00000000,
        // Add more colors as needed
    };

    ColorNode* head = NULL;

    // Add the common colors to the circular doubly linked list
    for (unsigned long i = 0; i < sizeof(colors) / sizeof(colors[0]); ++i) {
        appendNode(&head, colors[i]);
    }

    return head;
}


typedef struct t_params
{
	void *mlx_ptr;
	void *win_ptr;

	unsigned int colour;
	ColorNode *head;
} s_params;

int close_window(void *params)
{
	s_params *p = (s_params *) params;

	mlx_destroy_window(p->mlx_ptr, p->win_ptr);
	exit(1);
	return (1);
}

int update_color(s_params *p)
{
	p->colour = p->head->color;
	for (int i = 0; i < 50; i++)
		for (int j = 0; j < 50; j++)
			mlx_pixel_put(p->mlx_ptr, p->win_ptr, i, j, p->colour);
	return (1);
}

int press(int keycode, s_params *p)
{
	printf("Key: %i\n", keycode);
	if (keycode == 114) // red
		p->colour += 5 * 65534;
	else if (keycode == 103) // g
		p->colour += 5 * 254;
	else if (keycode == 98) // b
		p->colour += 5;
	else if (keycode == 65307)
		return (close_window(p));
	update_color(p);
	return (1);
}

int mouse(int button, int x, int y, void *params)
{
	printf("button: %i\n", button);

	s_params *p = (s_params *) params;
	if (button == 1)
		mlx_pixel_put(p->mlx_ptr, p->win_ptr, x, y, p->colour);
	else if (button == 3)
		mlx_clear_window(p->mlx_ptr, p->win_ptr);
	else if (button == 4)
		p->head = p->head->next;
	else if (button == 5)
		p->head = p->head->prev;
	update_color(p);
	return (0);	
}

int drag(int x, int y, void *params)
{
	s_params *p = (s_params *) params;
	for (int i = x - 1; i <= x + 1; i++)
		for (int j = y - 1; j <= y + 1; j++)
			mlx_pixel_put(p->mlx_ptr, p->win_ptr, i, j, p->colour);
	return (1);	
}

int	main(void)
{
	s_params params;

	params.mlx_ptr = mlx_init();
	params.win_ptr = mlx_new_window(params.mlx_ptr, 990, 300, "test");
	params.colour = 0xFFFFFFFF;
	params.head = init_colours();

	printf("&P: %p\n", &params);
	update_color(&params);
	mlx_key_hook(params.win_ptr, press, &params);
	mlx_hook(params.win_ptr, 6, 1L<<8, drag, &params);
	mlx_hook(params.win_ptr, 17, 1L<<3, &close_window, &params);
	mlx_mouse_hook(params.win_ptr, mouse, &params);
	// mlx_expose_hook(params.win_ptr, update_color, &params);
	mlx_loop(params.mlx_ptr);
}