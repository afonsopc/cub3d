/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image0.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:48:06 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/29 21:13:53 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "image.h"

void	ftm_free_image(void *image)
{
	if (!image)
		return ;
	SDL_FreeSurface(((t_ftm_image *)image)->surface);
	if (((t_ftm_image *)image)->texture)
		SDL_DestroyTexture(((t_ftm_image *)image)->texture);
	free(image);
}

static bool	load_image_addresses(t_ftm_image *image)
{
	if (!image->surface)
		return (false);
	image->data = (char *)image->surface->pixels;
	image->bits_per_pixel = image->surface->format->BitsPerPixel;
	image->size_line = image->surface->pitch;
	return (true);
}

t_ftm_image	*ftm_image_from_file(t_ftm_window *window, char *path)
{
	t_ftm_image	*image;

	image = ft_calloc(1, sizeof(t_ftm_image));
	if (!image)
		return (NULL);
	image->display = window->display;
	image->path = path;
	image->surface = SDL_LoadBMP(path);
	if (!image->surface)
		return (free(image), NULL);
	SDL_SetColorKey(image->surface, SDL_TRUE, SDL_MapRGB(image->surface->format, 255, 0, 255));
	image->texture = SDL_CreateTextureFromSurface(window->display, image->surface);
	if (!image->texture)
		return (SDL_FreeSurface(image->surface), free(image), NULL);
	image->size.width = image->surface->w;
	image->size.height = image->surface->h;
	if (!load_image_addresses(image))
		return (ftm_free_image(image), NULL);
	pthread_mutex_init(&image->mutex, NULL);
	return (image);
}

t_ftm_image	*ftm_image_new(t_ftm_window *window, t_size size)
{
	t_ftm_image	*image;

	image = ft_calloc(1, sizeof(t_ftm_image));
	if (!image)
		return (NULL);
	image->display = window->display;
	image->surface = SDL_CreateRGBSurface(0, size.width, size.height, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	if (!image->surface)
		return (free(image), NULL);
	image->texture = SDL_CreateTextureFromSurface(window->display, image->surface);
	if (!image->texture)
		return (SDL_FreeSurface(image->surface), free(image), NULL);
	image->size = size;
	if (!load_image_addresses(image))
		return (ftm_free_image(image), NULL);
	pthread_mutex_init(&image->mutex, NULL);
	return (image);
}

t_list	*ftm_images_from_files(t_ftm_window *window, char **file_paths)
{
	size_t		i;
	t_list		*list;
	t_ftm_image	*tmp_image;

	list = NULL;
	i = -1;
	while (file_paths[++i])
	{
		tmp_image = ftm_image_from_file(window, file_paths[i]);
		if (!tmp_image)
			return (ft_list_destroy(&list), NULL);
		ft_list_add(&list, tmp_image, (void (*)(void *))ftm_free_image);
	}
	return (list);
}
