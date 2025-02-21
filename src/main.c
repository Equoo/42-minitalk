/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 22:18:56 by dderny            #+#    #+#             */ 	
/*   Updated: 2025/02/21 01:32:43 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <libft.h>
#include <errno.h>
#include <ft_printf.h>
#include <ft_bool.h>
#include "main.h"
#include "error.h"

t_user	users[USERS_MAX];

void	sig_void(int sig, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	(void)sig;
}

void	sig_handler_n(int sig)
{
	(void)sig;
}

void	disconnect_user(int pid)
{
	int	i;

	i = 0;
	while (i < USERS_MAX && users[i].pid != pid)
		i++;
	if (i == USERS_MAX)
		return ;
	users[i].connected = FALSE;
	if (users[i].msg)
		free(users[i].msg);
	users[i].msg = NULL;
	users[i].len = -1;
	users[i].rbytes = 0;
	users[i].nbr = 0;
	users[i].pid = 0;
}

void	disconnect_all(void)
{
	int	i;

	i = 0;
	while (i < USERS_MAX)
	{
		if (users[i].connected)
			disconnect_user(users[i].pid);
		i++;
	}
}

t_user	*getuser(int pid, time_t stime)
{
	int	i;

	if (pid < 1)
		return (NULL);
	i = 0;
	while (i < USERS_MAX && users[i].pid != pid)
	{
		if (users[i].connected && stime - users[i].last_seen > TIMEOUT_TIME)
			disconnect_user(users[i].pid);
		i++;
	}
	if (i == USERS_MAX)
	{
		i = 0;
		while (users[i].connected)
			i++;
		if (i == USERS_MAX)
			return (NULL);
		users[i] = (t_user){};
		users[i].pid = pid;
		users[i].len = -1;
	}
	users[i].last_seen = stime;
	return (&users[i]);
}

void	sig_handler(int sig, siginfo_t *info, void *context)
{
	t_user	*user;
	int		bit;
	
	(void)context;
	user = getuser(info->si_pid, info->si_stime);
	if (!user)
		return ;
	if (sig == SIGUSR2)
		bit = 1;
	
	if (!user->connected && bit)
	{
		user->connected = TRUE;
		kill(user->pid, SIGUSR2);
		return ;
	}

	if (!user->len)
	{
		user->nbr = (user->nbr | bit) << 1;
		if (user->rbytes == 31)
		{
			user->len = user->nbr + 1;
			user->msg = ft_calloc(user->len, sizeof(char));
			if (!user->msg)
			{
				kill(user->pid, SIGUSR1);
				disconnect_all();
				print_error("Memory allocation");
				return ;
			}
			user->rbytes = 0;
			user->nbr = 0;
		}
		else
			user->rbytes++;
		kill(user->pid, SIGUSR2);
	}
	else if (user->nbr < user->len - 1)
	{
		user->msg[user->nbr] |= (bit << (7 - user->rbytes));
		if (user->rbytes == 7)
		{
			user->nbr++;
			user->rbytes = 0;
		}
		else
			user->rbytes++;
		kill(user->pid, SIGUSR2);
	}
	else if (user->nbr == user->len - 1)
	{
		ft_printf("Message from %d: %s\n", user->pid, user->msg);
		free(user->msg);
		disconnect_user(user->pid);
	}
	else
	{
		kill(user->pid, SIGUSR1);
		disconnect_user(user->pid);
	}
}

int	handle_signals(t_bool toggle)
{
	struct sigaction act;
	sigset_t sigset;

	act = (struct sigaction){0};
	sigemptyset(&sigset);
	sigaddset(&sigset, SIGUSR1);
	sigaddset(&sigset, SIGUSR2);
	if (!toggle)
	{
		act.sa_sigaction = sig_void;
	}
	else
		act.sa_sigaction = sig_handler;
	act.sa_flags = SA_SIGINFO;
	act.sa_mask = sigset;
	if (sigaction(SIGUSR1, &act, NULL))
		return (print_error("signal initialization"));
	if (sigaction(SIGUSR2, &act, NULL))
		return (print_error("signal initialization"));
	return (0);
}




/* 
**	Signal handling
*/

int		waitnextsig(t_bool ping)
{
	errno = 0;
	if (!usleep(500000))
	{
		if (!ping)
			return (ERRINITNOFIRSTSIG);
		else // so ping is egal to SIGWAITRY
			return (print_cerror("Signal number send",
						ERRINITNOSIG));
	}
	else if (errno && errno != EINTR)
		return (print_error("Signal number send"));
	return (0);
}

void	sigwritenbr(int pid, int bits, __int64_t n)
{
	int			waitres;
	__int64_t	num;
	int			i;

	i = 0;
	while (i < bits)
	{
		num = (n << i) & (1 << (bits - 1));
		if (num)
			kill(pid, SIGUSR2);
		else
			kill(pid, SIGUSR1);
		waitres = waitnextsig(FALSE);
		if (waitres == ERRINITNOFIRSTSIG)
			waitres = waitnextsig(TRUE);
		if (waitres)
			return ;
		i++;
	}
}





/*
__int64_t	sigreadnbr(int pid, int bits, t_sigwait ping)
{
	int			waitres;
	__int64_t	num;
	int			i;

	i = 0;
	while (i < bits)
	{
		waitres = sigwait(ping);
		if (waitres == ERRINITNOFIRSTSIG)
			waitres = sigwait(SIGWAITRY);
		if (waitres)
			return ;
		sigblock(TRUE);
		num = (num | sigbit) << 1;
		if (pid)
		{
			if (num)
				kill(pid, SIGUSR1);
			else
				kill(pid, SIGUSR2);
		}
		sigblock(FALSE);
		i++;
	}
	return (num);
}*/






int		loop_server(void)
{
	if (usleep(100000) && errno != EINTR)
		return (print_error("Sleep"));
	return (0);
}

int		init_server(void)
{
	ft_printf("Server PID: %d\n", getpid());
	handle_signals(TRUE);
	return (0);
}





int	init_client(char **argv)
{
	int		i;
	int		pid;

	pid = ft_atoi(argv[1]);
	if (pid < 1)
		return (print_cerror("Usage", ERRINITPID));
	if (signal(SIGUSR1, sig_handler_n) == SIG_ERR && print_error("Signal initialization"))
		return (EXIT_FAILURE);
	if (signal(SIGUSR2, sig_handler_n) == SIG_ERR && print_error("Signal initialization"))
		return (EXIT_FAILURE);
	ft_printf("Client PID: %d\n", getpid());	
	i = 0;
	sigwritenbr(pid, 64, ft_strlen(argv[2]));
	while (argv[2][i])
	{
		sigwritenbr(pid, 8, argv[2][i]);
		i++;
	}
	return (EXIT_SUCCESS);
}

int		main(int argc, char **argv)
{
	if (CLIENT && argc != 3)
		return (print_cerror("Usage", ERRINITARGS));
	if (CLIENT)
		return (init_client(argv));
	else
		init_server();
	while (1)
		loop_server();
	return (EXIT_SUCCESS);
}