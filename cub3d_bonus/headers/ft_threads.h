/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_threads.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 19:48:00 by afpachec          #+#    #+#             */
/*   Updated: 2025/05/19 22:25:43 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_THREADS_H
# define FT_THREADS_H

// External Libs
# include <pthread.h>

// Internal Libs
# include <ft_utils.h>

typedef struct s_ftt_thread
{
	pthread_t	thread;
	bool		running;
	bool		active;
	void		(*routine)(void *);
	void		*data;
}	t_ftt_thread;

void			ftt_thread_init(t_ftt_thread *thread);
t_ftt_thread	*ftt_thread_new(void);
void			ftt_thread_run(t_ftt_thread *thread);
void			ftt_thread_stop(t_ftt_thread *thread);
void			ftt_thread_wait(t_ftt_thread *thread);
void			ftt_clear_thread(t_ftt_thread *thread);
void			ftt_free_thread(t_ftt_thread *thread);

#endif
