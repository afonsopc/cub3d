/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image0.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:48:06 by afpachec          #+#    #+#             */
/*   Updated: 2025/07/01 15:35:58 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "image.h"

void	ftm_free_image(void *image)
{
	t_ftm_image	*img;

	if (!image)
		return ;
	img = (t_ftm_image *)image;
	if (img->texture)
		SDL_DestroyTexture(img->texture);
	if (img->surface)
		SDL_FreeSurface(img->surface);
	pthread_mutex_destroy(&img->mutex);
	free(image);
}

static bool	load_image_addresses(t_ftm_image *image)
{
	if (!image->surface)
		return (false);
	image->data = image->surface->pixels;
	image->bits_per_pixel = image->surface->format->BitsPerPixel;
	image->size_line = image->surface->pitch;
	return (true);
}

t_ftm_image	*ftm_image_from_file(t_ftm_window *window, char *path)
{
    t_ftm_image	*image;
    SDL_Surface	*rgb_surface;

	(void)window;
    image = ft_calloc(1, sizeof(t_ftm_image));
    if (!image)
        return (NULL);
    image->path = path;
    image->surface = SDL_LoadBMP(path);
    if (!image->surface)
        return (free(image), NULL);
    if (image->surface->format->BitsPerPixel <= 8)
    {
        rgb_surface = SDL_ConvertSurfaceFormat(image->surface, 
            SDL_PIXELFORMAT_RGBA8888, 0);
        if (rgb_surface)
        {
            SDL_FreeSurface(image->surface);
            image->surface = rgb_surface;
        }
    }
    SDL_SetColorKey(image->surface, SDL_TRUE, SDL_MapRGB(image->surface->format, 255, 0, 255));
    image->size.width = image->surface->w;
    image->size.height = image->surface->h;
    if (!load_image_addresses(image))
        return (ftm_free_image(image), NULL);
    pthread_mutex_init(&image->mutex, NULL);
    image->texture = NULL;
    image->renderer = NULL;
    image->texture_dirty = true;
    return (image);
}

t_ftm_image	*ftm_image_new(t_ftm_window *window, t_size size)
{
	t_ftm_image	*image;

	(void)window;
	image = ft_calloc(1, sizeof(t_ftm_image));
	if (!image)
		return (NULL);
	image->path = NULL;
	image->size = size;
	image->surface = SDL_CreateRGBSurface(0, size.width, size.height,
			32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	if (!image->surface)
		return (ftm_free_image(image), NULL);
	if (!load_image_addresses(image))
		return (ftm_free_image(image), NULL);
	pthread_mutex_init(&image->mutex, NULL);
	image->texture = NULL;
	image->renderer = NULL;
	image->texture_dirty = true;
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
