/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3_surveillance.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataboada <ataboada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 16:10:36 by ataboada          #+#    #+#             */
/*   Updated: 2023/10/09 15:07:19 by ataboada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

void	ft_surveillance(t_data *d);
int		ft_dead_or_full(t_data *d, t_philo *p);

/*
	here is where we check when it's time to end the simulation
	1) ft_surveillance: used to check if the simulation should end or not
		- we can pass a value to the function to set the simulation to end
		- or we can check it by seeing if any philo has died or all have eaten
	2) ft_dead_or_full: used to check if any philo has died or all have eaten
		- if any philo has died, we set the simulation to end
		- if all philos have eaten, we print a message and set the simulation to end
*/

void	ft_surveillance(t_data *d)
{
	int	i;
	int	should_end;

	should_end = FALSE;
	while (should_end == FALSE)
	{
		i = -1;
		while (++i < d->n_philo)
		{
			if (should_end == FALSE && ft_dead_or_full(d, &d->philo[i]) == TRUE)
				should_end = TRUE;
		}
		usleep(10);
	}
}

int	ft_dead_or_full(t_data *d, t_philo *p)
{
	pthread_mutex_lock(&d->mtx_eat);
	if ((ft_get_current_time() - p->started_eating) >= d->time_to_die)
	{
		ft_print_status(p, "died");
		ft_should_simulation_end(p, TRUE);
		pthread_mutex_unlock(&d->mtx_eat);
		return (TRUE);
	}
	if ((d->times_must_eat > 0) && (p->times_eaten >= d->times_must_eat))
	{
		d->n_philo_full++;
		if (d->n_philo_full == d->n_philo)
		{
			ft_should_simulation_end(p, TRUE);
			ft_print_status(p, NULL);
			pthread_mutex_unlock(&d->mtx_eat);
			return (TRUE);
		}
	}
	pthread_mutex_unlock(&d->mtx_eat);
	return (FALSE);
}
