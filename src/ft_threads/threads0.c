/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads0.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 19:48:43 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 15:10:22 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_threads.h>

static void	*thread_routine(void *data)
{
	t_ftt_thread	*thread;

	thread = data;
	while (1)
	{
		pthread_mutex_lock(&thread->start_mutex);
		while (!thread->running && thread->active)
			pthread_cond_wait(&thread->start_cond, &thread->start_mutex);
		if (!thread->active)
		{
			pthread_mutex_unlock(&thread->start_mutex);
			break ;
		}
		pthread_mutex_unlock(&thread->start_mutex);
		if (thread->routine)
			thread->routine(thread->data);
		pthread_mutex_lock(&thread->done_mutex);
		thread->running = false;
		pthread_cond_signal(&thread->done_cond);
		pthread_mutex_unlock(&thread->done_mutex);
	}
	return (NULL);
}

void	ftt_thread_init(t_ftt_thread *thread)
{
	if (!thread)
		return ;
	thread->active = true;
	thread->running = false;
	pthread_mutex_init(&thread->start_mutex, NULL);
	pthread_cond_init(&thread->start_cond, NULL);
	pthread_mutex_init(&thread->done_mutex, NULL);
	pthread_cond_init(&thread->done_cond, NULL);
	pthread_create(&thread->thread, NULL, thread_routine, thread);
}

t_ftt_thread	*ftt_thread_new(void)
{
	t_ftt_thread	*thread;

	thread = ft_calloc(1, sizeof(t_ftt_thread));
	if (!thread)
		return (NULL);
	ftt_thread_init(thread);
	return (thread);
}

void	ftt_thread_run(t_ftt_thread *thread)
{
	if (!thread)
		return ;
	pthread_mutex_lock(&thread->start_mutex);
	if (thread->running)
	{
		pthread_mutex_unlock(&thread->start_mutex);
		return ;
	}
	thread->running = true;
	pthread_cond_signal(&thread->start_cond);
	pthread_mutex_unlock(&thread->start_mutex);
}

void	ftt_thread_stop(t_ftt_thread *thread)
{
	if (!thread)
		return ;
	pthread_mutex_lock(&thread->done_mutex);
	thread->running = false;
	pthread_cond_signal(&thread->done_cond);
	pthread_mutex_unlock(&thread->done_mutex);
}
