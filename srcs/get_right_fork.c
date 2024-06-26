/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_right_fork.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heolivei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 20:04:29 by heolivei          #+#    #+#             */
/*   Updated: 2023/09/09 20:04:34 by heolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	get_right_fork(t_philo *philo)
{
	int	finish;

	finish = 0;
	pthread_mutex_lock(&philo->params->forks[philo->id_fork_right]);
	pthread_mutex_lock(&philo->params->finish_lock);
	finish = philo->params->finish;
	pthread_mutex_unlock(&philo->params->finish_lock);
	pthread_mutex_lock(&philo->params->print);
	if (finish == 0)
		printf("%d %d has taken a fork\n", (int)get_time((__uint64_t)
				philo->params->start_program), philo->id + 1);
	pthread_mutex_unlock(&philo->params->print);
	return (finish);
}
