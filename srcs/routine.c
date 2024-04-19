/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heolivei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 19:03:02 by heolivei          #+#    #+#             */
/*   Updated: 2023/09/09 19:03:15 by heolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	ft_usleep(__uint64_t sleep_time)
{
	__uint64_t	start;

	start = get_time(0);
	while ((get_time(0) - start) < sleep_time)
		usleep(500);
}

void	sleeping(t_philo *philo)
{
	int	finish;

	pthread_mutex_lock(&philo->params->finish_lock);
	finish = philo->params->finish;
	pthread_mutex_unlock(&philo->params->finish_lock);
	pthread_mutex_lock(&philo->params->print);
	if (finish == 0)
		printf("%d %d is sleeping\n", (int)get_time((__uint64_t)
				philo->params->start_program), philo->id + 1);
	pthread_mutex_unlock(&philo->params->print);
	usleep(philo->params->n_time_to_sleep * 1000);
}

void	eating(t_philo *philo)
{
	int	finish;

	pthread_mutex_lock(&philo->params->finish_lock);
	finish = philo->params->finish;
	pthread_mutex_unlock(&philo->params->finish_lock);
	if (philo->id % 2 != 0)
	{
		finish = get_right_fork(philo);
		finish = get_left_fork(philo);
		commun_eating(philo, finish);
		pthread_mutex_unlock(&philo->params->forks[philo->id_fork_left]);
		pthread_mutex_unlock(&philo->params->forks[philo->id_fork_right]);
	}
	else
	{
		finish = get_left_fork(philo);
		finish = get_right_fork(philo);
		commun_eating(philo, finish);
		pthread_mutex_unlock(&philo->params->forks[philo->id_fork_right]);
		pthread_mutex_unlock(&philo->params->forks[philo->id_fork_left]);
	}
}

void	thinking(t_philo *philo)
{
	int	finish;

	pthread_mutex_lock(&philo->params->finish_lock);
	finish = philo->params->finish;
	pthread_mutex_unlock(&philo->params->finish_lock);
	pthread_mutex_lock(&philo->params->print);
	if (finish == 0)
		printf("%d %d is thinking\n", (int)get_time((__uint64_t)
				philo->params->start_program), philo->id + 1);
	pthread_mutex_unlock(&philo->params->print);
}

void	*routine(void *arg)
{
	t_philo	*philo;
	int		finish;

	philo = (t_philo *)arg;
	finish = kill_philo(philo);
	while (finish == 0)
	{
		if (philo->id % 2 != 0)
			usleep(100);
		pthread_mutex_lock(&philo->params->finish_lock);
		finish = philo->params->finish;
		pthread_mutex_unlock(&philo->params->finish_lock);
		if (finish == 1)
			return (NULL);
		if (finish == 0)
			eating(philo);
		if (finish == 0)
		{
			sleeping(philo);
			usleep(2000);
		}
		if (finish == 0)
			thinking(philo);
	}
	return (0);
}
