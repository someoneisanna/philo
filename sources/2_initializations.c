/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2_initializations.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataboada <ataboada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 15:24:38 by ataboada          #+#    #+#             */
/*   Updated: 2023/10/09 14:50:21 by ataboada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

void	ft_initialize_data(t_data *data, int ac, char **av);
void	ft_initialize_mtxs(t_data *data);
void	ft_initialize_philo(t_data *data);
void	ft_initialize_thread(t_data *data);
void	*ft_start_simulation(void *philo);

/*
	here the struct variables are being initialized and the threads being created
	1) we initialize the data struct and check if the arguments are correct
	2) we initialize the mutexes that are in the data struct
	3) we initialize the philo struct
	4) we create the threads that make the program work
		- we create a thread for each philosopher with pthread_create
		- ft_start_simulation is the function that each thread will execute
		- for each thread, we check if any philosopher starved or all ate enough
	5) the ft_start_simulation function is the one being performed by the threads
		- first, we put the even philosophers to wait for their turn to eat
		- we then create a condition for when there is only one philosopher
		- we then check if we should end the simulation (one starved or all ate enough)
		- if not, we continue on with the philo tasks: eating, sleeping, thinking
		- lastly, we put the odd philosophers to wait for their turn to eat
*/

void	ft_initialize_data(t_data *d, int ac, char **av)
{
	d->n_philo = ft_atoi(av[1]);
	d->time_to_die = ft_atoi(av[2]);
	d->time_to_eat = ft_atoi(av[3]);
	d->time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		d->times_must_eat = ft_atoi(av[5]);
	else
		d->times_must_eat = 0;
	d->n_philo_full = 0;
	d->end_flag = 0;
	d->start_time = ft_get_current_time();
	d->philo = malloc(sizeof(t_philo) * d->n_philo);
	if (!d->philo)
		ft_perror("Error: Creation of the PHILO MALLOC failed\n", d, 1);
	if (d->n_philo < 1)
		ft_perror("Error: There must be at least one philosopher\n", d, 1);
	if (d->time_to_die < 1 || d->time_to_eat < 1 || d->time_to_sleep < 1)
		ft_perror("Error: Time arguments must be greater than 0\n", d, 1);
	if (ac == 6 && d->times_must_eat < 1)
		ft_perror("Error: If it exists, the last arg must be > 0\n", d, 1);
}

void	ft_initialize_mtxs(t_data *data)
{
	int	i;

	i = -1;
	data->mtx_fork = malloc(sizeof(pthread_mutex_t) * data->n_philo);
	if (!data->mtx_fork)
		ft_perror("Error: Creation of the FORK MALLOC failed\n", data, 2);
	while (++i < data->n_philo)
	{
		if (pthread_mutex_init(&data->mtx_fork[i], NULL) != 0)
			ft_perror("Error: Creation of the FORK MUTEX failed\n", data, 2);
	}
	if (pthread_mutex_init(&data->mtx_print, NULL))
		ft_perror("Error: Creation of the PRINT MUTEX failed\n", data, 2);
	if (pthread_mutex_init(&data->mtx_eat, NULL))
		ft_perror("Error: Creation of the EAT MUTEX failed\n", data, 2);
	if (pthread_mutex_init(&data->mtx_end, NULL))
		ft_perror("Error: Creation of the END MUTEX failed\n", data, 2);
}

void	ft_initialize_philo(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->n_philo)
	{
		data->philo[i].id = i + 1;
		data->philo[i].fork_left = i;
		data->philo[i].fork_right = (i + 1) % data->n_philo;
		data->philo[i].times_eaten = 0;
		data->philo[i].started_eating = data->start_time;
		data->philo[i].finished_eating = data->start_time;
		data->philo[i].data = data;
		i++;
	}
}

void	ft_initialize_thread(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->n_philo)
	{
		if (pthread_create(&data->philo[i].thread, NULL,
				ft_start_simulation, &data->philo[i]) != 0)
			ft_perror("Error: Creation of the thread failed\n", data, 3);
	}
	ft_surveillance(data);
	i = -1;
	while (++i < data->n_philo)
	{
		if (pthread_join(data->philo[i].thread, NULL) != 0)
			ft_perror("Error: Joining of the thread failed\n", data, 3);
	}
}

void	*ft_start_simulation(void *ptr)
{
	t_philo	*philo;

	philo = (t_philo *)ptr;
	if (philo->id % 2 == 0)
		usleep(philo->data->time_to_eat * 1000);
	while (1)
	{
		if (philo->data->n_philo == 1)
		{
			ft_print_status(philo, "has taken a fork");
			usleep(philo->data->time_to_die * 1000);
			ft_print_status(philo, "died");
			ft_should_simulation_end(philo, TRUE);
		}
		if (ft_should_simulation_end(philo, FALSE) == TRUE)
			return (NULL);
		ft_eat(philo);
		ft_print_status(philo, "is sleeping");
		usleep(philo->data->time_to_sleep * 1000);
		ft_print_status(philo, "is thinking");
		if (philo->data->n_philo % 2 != 0)
			usleep(philo->data->time_to_eat * 1000);
	}
	return (NULL);
}
