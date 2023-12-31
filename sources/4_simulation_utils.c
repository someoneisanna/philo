/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   4_simulation_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataboada <ataboada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 16:53:36 by ataboada          #+#    #+#             */
/*   Updated: 2023/10/09 16:24:10 by ataboada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

int		ft_should_simulation_end(t_philo *philo, int should_end);
void	ft_print_status(t_philo *philo, char *str);
void	ft_eat(t_philo *philo);

/*
	this is where all the simulation helper functions are
	1) ft_should_simulation_end: used to check if the simulation should end
		- here we assign 1 to the end_flag if we want to end the simulation
		- we also return TRUE if the simulation should end and FALSE otherwise
	2) ft_print_status: used to print the status of the philosopher
		- if we don't pass a string, it means that all philosophers have eaten
		  the number of times they should
	3) ft_eat: this is the function where we simulate the philosopher eating
		- reminder: the philosopher can only eat if he has both forks
*/

int	ft_should_simulation_end(t_philo *philo, int should_end)
{
	pthread_mutex_lock(&philo->data->mtx_end);
	if (should_end == TRUE || philo->data->end_flag == 1)
	{
		if (should_end == TRUE)
			philo->data->end_flag = 1;
		pthread_mutex_unlock(&philo->data->mtx_end);
		return (TRUE);
	}
	pthread_mutex_unlock(&philo->data->mtx_end);
	return (FALSE);
}

void	ft_print_status(t_philo *philo, char *str)
{
	long long	timestamp;

	pthread_mutex_lock(&philo->data->mtx_print);
	if (ft_should_simulation_end(philo, FALSE) == FALSE)
	{
		timestamp = ft_get_current_time() - philo->data->start_time;
		printf("%lld %d %s\n", timestamp, philo->id, str);
	}
	if (str == NULL)
		printf("All philosophers ate %d times\n", philo->data->times_must_eat);
	pthread_mutex_unlock(&philo->data->mtx_print);
}

void	ft_eat(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&philo->data->mtx_fork[philo->fork_left]);
		pthread_mutex_lock(&philo->data->mtx_fork[philo->fork_right]);
	}
	else
	{
		pthread_mutex_lock(&philo->data->mtx_fork[philo->fork_right]);
		pthread_mutex_lock(&philo->data->mtx_fork[philo->fork_left]);
	}
	ft_print_status(philo, "has taken a fork");
	ft_print_status(philo, "has taken a fork");
	ft_print_status(philo, "is eating");
	pthread_mutex_lock(&philo->data->mtx_eat);
	philo->started_eating = ft_get_current_time();
	usleep(philo->data->time_to_eat * 1000);
	philo->finished_eating = ft_get_current_time();
	philo->times_eaten++;
	pthread_mutex_unlock(&philo->data->mtx_eat);
	pthread_mutex_unlock(&philo->data->mtx_fork[philo->fork_right]);
	pthread_mutex_unlock(&philo->data->mtx_fork[philo->fork_left]);
}
