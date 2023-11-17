/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   5_utils.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataboada <ataboada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 11:40:05 by ataboada          #+#    #+#             */
/*   Updated: 2023/08/06 15:31:30 by ataboada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

int			ft_atoi(const char *str);
void		ft_perror(char *str, t_data *data, int flag);
void		ft_free_mtxs(t_data *data);
long long	ft_get_current_time(void);

// this is a function that will convert a string to an integer
int	ft_atoi(const char *str)
{
	int		i;
	int		sig;
	long	res;

	i = 0;
	res = 0;
	sig = 1;
	while ((str[i] >= '\t' && str[i] <= '\r') || str[i] == ' ')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sig = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + str[i] - '0';
		i++;
	}
	return (sig * res);
}

// this is a function the will print the error, free allocated memory and exit
// the program
void	ft_perror(char *str, t_data *data, int flag)
{
	printf("%s", str);
	if (flag == 1)
		free(data->philo);
	if (flag == 2)
	{
		free(data->philo);
		free(data->mtx_fork);
	}
	if (flag == 3)
		ft_free_mtxs(data);
	exit (1);
}

// this is a function that will free the mutexes and the join the threads
void	ft_free_mtxs(t_data *data)
{
	int	i;

	i = -1;
	if (data->philo)
		free(data->philo);
	if (data->mtx_fork)
	{
		while (++i < data->n_philo)
			pthread_mutex_destroy(&data->mtx_fork[i]);
		free(data->mtx_fork);
	}
	pthread_mutex_destroy(&data->mtx_print);
	pthread_mutex_destroy(&data->mtx_eat);
	pthread_mutex_destroy(&data->mtx_end);
}

// this is a function that will return the current time in milliseconds
// we return the number of seconds x1000 to get number of miliseconds
// plus the number of microseconds /1000 to get the remaining miliseconds
long long	ft_get_current_time(void)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
}
