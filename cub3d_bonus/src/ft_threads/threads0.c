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
	while (thread->active)
	{
		if (thread->running)
		{
			if (thread->routine)
				thread->routine(thread->data);
			thread->running = false;
		}
		ft_sleep(1);
	}
	return (NULL);
}

void	ftt_thread_init(t_ftt_thread *thread)
{
	if (!thread)
		return ;
	thread->active = true;
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
	if (!thread || thread->running)
		return ;
	thread->running = true;
}

void	ftt_thread_stop(t_ftt_thread *thread)
{
	if (!thread)
		return ;
	thread->running = false;
}
