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
	pthread_mutex_lock(&thread->done_mutex);
	while (thread->running)
		pthread_cond_wait(&thread->done_cond, &thread->done_mutex);
	pthread_mutex_unlock(&thread->done_mutex);
}

void	ftt_clear_thread(t_ftt_thread *thread)
{
	if (!thread)
		return ;
	pthread_mutex_lock(&thread->start_mutex);
	thread->active = false;
	thread->running = false;
	pthread_cond_signal(&thread->start_cond);
	pthread_mutex_unlock(&thread->start_mutex);
	pthread_join(thread->thread, NULL);
	pthread_mutex_destroy(&thread->start_mutex);
	pthread_cond_destroy(&thread->start_cond);
	pthread_mutex_destroy(&thread->done_mutex);
	pthread_cond_destroy(&thread->done_cond);
	ft_bzero(thread, sizeof(t_ftt_thread));
}

void	ftt_free_thread(t_ftt_thread *thread)
{
	if (!thread)
		return ;
	ftt_clear_thread(thread);
	free(thread);
}
