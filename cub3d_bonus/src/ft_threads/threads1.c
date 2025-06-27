/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 22:30:02 by afpachec          #+#    #+#             */
/*   Updated: 2025/05/17 11:45:15 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_threads.h>

void	ftt_thread_wait(t_ftt_thread *thread)
{
	if (!thread)
		return ;
	while (thread->running)
		ft_sleep(1);
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
