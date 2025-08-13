/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 22:30:02 by afpachec          #+#    #+#             */
/*   Updated: 2025/08/13 16:50:51 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_threads.h>

void	ftt_thread_wait(t_ftt_thread *thread)
{
	if (!thread)
		return ;
	while (thread->running)
	{
		#ifdef __EMSCRIPTEN__
			emscripten_sleep(0);
		#else
			ft_sleep(1);
		#endif
	}
}

void	ftt_clear_thread(t_ftt_thread *thread)
{
	if (!thread)
		return ;
	thread->active = false;
	pthread_join(thread->thread, NULL);
	ft_bzero(thread, sizeof(t_ftt_thread));
}

void	ftt_free_thread(t_ftt_thread *thread)
{
	if (!thread)
		return ;
	ftt_clear_thread(thread);
	free(thread);
}
