
#include "exec.h"

//here odd philo take their left forks then right
//also quick checker in case we just have a single philo
//bcs in that case forks id left and right is the same
//eat, and make there forks available once more.

static int	fork_eat_odd(t_philo *philo)
{
	routine_take_fork(philo, false);
	if (philo->right->id == philo->left->id)
	{
		return(1) ; //define into alone philo
	}
	routine_take_fork(philo, true);
	print_msg_routine(philo, IS_EATING);
	usleep(philo->set->t_eat);
	pthread_mutex_lock(&philo->left->mutex);
	philo->left->available = true;
	pthread_mutex_unlock(&philo->left->mutex);
	pthread_mutex_lock(&philo->right->mutex);	
	philo->right->available = true;
	pthread_mutex_unlock(&philo->right->mutex);
    return(0);
}

static int routine_odd_loop(t_philo *philo)
{
	print_msg_routine(philo, IS_THINKING);
	usleep(philo->set->t_eat / 6);
    pthread_mutex_lock(&philo->set->death_mutex);
	while (!philo->set->death && philo->meals_eaten != philo->set->max_meal)
	{
		pthread_mutex_unlock(&philo->set->death_mutex);
		if(fork_eat_odd(philo))
			return(1); //define into alone philo
		print_msg_routine(philo, IS_SLEEPING);
		usleep(philo->set->t_sleep);
		print_msg_routine(philo, IS_THINKING);
		if (!philo->set->nbr_philo_odd)
			usleep(philo->set->t_eat);
		pthread_mutex_lock(&philo->set->death_mutex);
	}
    pthread_mutex_unlock(&philo->set->death_mutex);
    return(RETURN_SUCCESS);
}

//this is where our odd philo's threads would start.
//at first they wait in wait all thread until all threads
//are created. then it start to think, a usleep of t_eat / 6
//is required for the start of the routine for opti purpose
//then we get into our while loop that represent the general routine
//of our odd_nbr philo's threads

static void fragmented_usleep_odd(t_philo *philo)
{
	// print_msg_routine(philo, IS_THINKING);
	// usleep(philo->set->t_eat / 6);
	pthread_mutex_lock(&philo->set->death_mutex);
	while (!philo->set->death && philo->meals_eaten != philo->set->max_meal)
	{
		pthread_mutex_unlock(&philo->set->death_mutex);
		routine_take_fork(philo, false);
		routine_take_fork(philo, true);
		print_msg_routine(philo, IS_EATING);
		ft_usleep(philo->set->t_eat, philo->set);
		pthread_mutex_lock(&philo->left->mutex);
		philo->left->available = true;
		pthread_mutex_unlock(&philo->left->mutex);
		pthread_mutex_lock(&philo->right->mutex);
		philo->right->available = true;
		pthread_mutex_unlock(&philo->right->mutex);
		print_msg_routine(philo, IS_SLEEPING);
		ft_usleep(philo->set->t_sleep, philo->set);
		print_msg_routine(philo, IS_THINKING);
		if(!philo->set->nbr_philo_odd)
			ft_usleep(philo->set->t_eat, philo->set);
		pthread_mutex_lock(&philo->set->death_mutex);
	}
	pthread_mutex_unlock(&philo->set->death_mutex);
}

void	*routine_odd(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	wait_all_threads(philo->set, philo);
	if (philo->set->edge_case)
	{
		print_msg_routine(philo, IS_THINKING);
		ft_usleep(philo->set->t_eat / 6, philo->set);
		fragmented_usleep_odd(philo);
	}
	else if(routine_odd_loop(philo))
    {
        usleep(philo->set->t_die);
        return(0);
    }
	pthread_mutex_lock(&philo->set->pasta_mutex);
	if (philo->meals_eaten == philo->set->max_meal)
		philo->set->philo_full_pasta = philo->set->philo_full_pasta + 1;
	pthread_mutex_unlock(&philo->set->pasta_mutex);
	return (0);
}
