/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pitc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 17:48:02 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/04 23:17:35 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "image.h"

static void	set_size(t_ftm_image *image, t_pitc_data *pd)
{
	if (!pd->pitc.resize)
		pd->pitc.size = image->size;
	pd->it_limits = pd->pitc.size;
	if (pd->pitc.size.width > pd->canvas->size.width * 2)
		pd->it_limits.width = pd->canvas->size.width * 2 + 1;
	if (pd->pitc.size.height > pd->canvas->size.height * 2)
		pd->it_limits.height = pd->canvas->size.height * 2 + 1;
}

static bool	init_pd(t_pitc_data *pd, t_ftm_image *canvas, t_ftm_image *image,
		t_ftm_pitc_config pitc)
{
	if (!canvas || !image)
		return (false);
	*pd = (t_pitc_data){.canvas = canvas, .image = image, .pitc = pitc};
	pd->cs = image->size;
	if (pitc.crop)
		pd->cs = (t_size){pitc.crop_end.x - pitc.crop_start.x, pitc.crop_end.y
			- pitc.crop_start.y};
	set_size(image, pd);
	if (pd->cs.width <= 0 || pd->cs.height <= 0
		|| pd->pitc.size.width <= 0 || pd->pitc.size.height <= 0)
		return (false);
	pd->scale = (t_coords){
		(double)pd->cs.width / pd->pitc.size.width,
		(double)pd->cs.height / pd->pitc.size.height, 0};
	return (true);
}

static unsigned int	gmp(unsigned int (*pm)(void *, unsigned int),
	void *data, unsigned int prev_pixel, unsigned int pixel)
{
	if (pm)
		pixel = pm(data, pixel);
	if (!pixel)
		return (0);
	if (pixel / 0x01000000 != 0xFF)
		pixel = ftm_blend_pixel(prev_pixel, pixel);
	return (pixel);
}

static bool	set_pixel_maybe(t_pitc_data *pd)
{
	pd->sc = (t_coords){pd->pitc.crop_start.x + round(pd->index.width
			* pd->scale.x), pd->pitc.crop_start.y
		+ round(pd->index.height * pd->scale.y), 0};
	pd->cc = (t_coords){pd->pitc.coords.x + pd->index.width, pd->pitc.coords.y
		+ pd->index.height, 0};
	if (pd->cc.x < 0 || pd->cc.x >= pd->canvas->size.width
		|| pd->cc.y < 0 || pd->cc.y >= pd->canvas->size.height
		|| pd->sc.x < 0 || pd->sc.x >= pd->image->size.width
		|| pd->sc.y < 0 || pd->sc.y >= pd->image->size.height)
		return (false);
	pd->cp = &((unsigned int *)pd->canvas->data)[(int)(pd->cc.y
			* pd->canvas->size.width + pd->cc.x)];
	pd->p = gmp(pd->pitc.pixel_modifier, pd->pitc.pixel_modifier_data,
			*pd->cp, ((unsigned int *)pd->image->data)[(int)(pd->sc.y
				* pd->image->size.width + pd->sc.x)]);
	if (pd->p)
		*pd->cp = pd->p;
	return (true);
}

void	ftm_put_image_to_canvas(t_ftm_image *canvas, t_ftm_image *image,
		t_ftm_pitc_config pitc)
{
	t_pitc_data	pd;

	pitc.coords.x = (int)pitc.coords.x;
	pitc.coords.y = (int)pitc.coords.y;
	if (!init_pd(&pd, canvas, image, pitc))
		return ;
	pd.index.width = -1;
	while (++pd.index.width < pd.it_limits.width)
	{
		pd.index.height = -1;
		while (++pd.index.height < pd.it_limits.height)
			set_pixel_maybe(&pd);
	}
}
