/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minilibft.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heolivei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 19:02:26 by heolivei          #+#    #+#             */
/*   Updated: 2023/09/09 19:02:30 by heolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	ft_atoi(const char *str)
{
	int	c;
	int	s;
	int	res;

	c = 0;
	s = 1;
	res = 0;
	while (str[c] == ' ' || str[c] == '\n' || str[c] == '\t'
		|| str[c] == '\v' || str[c] == '\f' || str[c] == '\r')
		c++;
	if (str[c] == '-' || str[c] == '+')
	{
		if (str[c] == '-')
			s = -1;
		c++;
	}
	while (str[c] >= '0' && str[c] <= '9')
	{
		res = (res * 10) + (str[c] - 48);
		c++;
	}
	while (str[c])
		if (str[c] < '0' || str[c++] > '9')
			return (-1);
	return (res * s);
}

int	check_any_dead(t_params *params)
{
	int	i;
	int	since_last_meal;

	i = 0;
	while (i < params->n_philo)
	{
		pthread_mutex_lock(&params->philos[i]->lst_meal_lock);
		since_last_meal = get_time(params->start_program)
			- params->philos[i]->time_lst_meal;
		pthread_mutex_unlock(&params->philos[i]->lst_meal_lock);
		if (since_last_meal >= params->n_time_to_die)
		{
			if (params->n_philo == 1)
				return (1);
			printf("%d %d died\n", (int)get_time((__uint64_t)
					params->start_program), params->philos[i]->id + 1);
			pthread_mutex_lock(&params->finish_lock);
			params->finish = 1;
			pthread_mutex_unlock(&params->finish_lock);
			return (1);
		}
		i++;
	}
	return (0);
}

int	kill_philo(t_philo *philo)
{
	int	finish;

	finish = 0;
	if (philo->params->n_philo == 1)
	{
		pthread_mutex_lock(&philo->params->print);
		printf("%d %d has taken a fork\n", philo->time_lst_meal, philo->id + 1);
		pthread_mutex_unlock(&philo->params->print);
		printf("%d %d died\n", (int)get_time
			(philo->params->start_program), philo->id + 1);
		pthread_mutex_lock(&philo->params->finish_lock);
		philo->params->finish = 1;
		pthread_mutex_unlock(&philo->params->finish_lock);
	}
	return (finish);
}

void	commun_eating(t_philo *philo, int finish)
{
	pthread_mutex_lock(&philo->params->print);
	if (finish == 0)
		printf("%d %d is eating\n", (int)get_time((__uint64_t)
				philo->params->start_program), philo->id + 1);
	if (philo->params->n_meals != -1)
	{
		pthread_mutex_lock(&philo->meal_count_lock);
		philo->meals_count++;
		pthread_mutex_unlock(&philo->meal_count_lock);
	}
	pthread_mutex_unlock(&philo->params->print);
	pthread_mutex_lock(&philo->lst_meal_lock);
	philo->time_lst_meal = get_time(philo->params->start_program);
	pthread_mutex_unlock(&philo->lst_meal_lock);
	usleep(philo->params->n_time_to_eat * 1000);
}
