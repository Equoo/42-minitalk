/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 15:08:56 by dderny            #+#    #+#             */
/*   Updated: 2025/02/17 18:52:16 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <libft.h>
#include <errno.h>
#include <ft_printf.h>
#include "main.h"
#include "error.h"

struct s_env
{
	int		fd;
} typedef t_env;

int	process_close(t_env *env)
{
	(void)env;
	if (errno != 11 && errno && print_error("Initialization"))
		exit(EXIT_FAILURE);
	exit(EXIT_SUCCESS);
	return (EXIT_SUCCESS);
}

int letter = 0;
int bits = 0;
char str[128];

void	handle_signal(int signum)
{
	int bit;

	bit = 0;
	if (signum == SIGUSR2)
		bit = 1;
	letter += bit;
	if (bits < 7)
		letter <<= 1;
	//ft_printf("Signal received %d, %d\n", signum, bits);
	bits++;
}

int	main(int argc, char **argv)
{
	if (!CLIENT && !SERVER)
		return (print_cerror("Initialization", ERRINITVARS));
	if (CLIENT && argc != 3)
		return (print_cerror("Initialization", ERRINITARGS));
	
	unsigned long i = 0;
	if (SERVER)
	{
		ft_printf("PID: %d\n", getpid());
		signal(SIGUSR1, handle_signal);
		signal(SIGUSR2, handle_signal);
		while (1)
		{
			if (pause() == -1)
			{
				if (errno == EINTR)
				{
					if (bits != 8)
						continue ;
					if (!letter)
					{
						str[i] = 0;
						ft_printf("Message: %s\n", str);
						i = 0;
						//str = NULL;
					}
					else
					{
						ft_printf("Add %c, %d\n", letter, letter);
						str[i] = letter;
						i++;
					}
					bits = 0;
					letter = 0;
					continue ;
				}
				return (print_error("Initialization"));
			}
		}
	}
	else
	{
		while (argv[2][i])
		{
			char c = argv[2][i];
			
			int j = 0;
			while (j < 8)
			{
				char cbit = c;
				cbit <<= j;
				cbit >>= 7;
				if (cbit)
					kill(ft_atoi(argv[1]), SIGUSR2);
				else
					kill(ft_atoi(argv[1]), SIGUSR1);
				usleep(5000);
				j++;
			}
			ft_printf("Sent to %d, %c, %i\n", ft_atoi(argv[1]), argv[2][i], argv[2][i]);
			i++;
		}
		i = 0;
		while (i < 8)
		{
			kill(ft_atoi(argv[1]), SIGUSR1);
			usleep(5000);
			i++;
		}
		ft_printf("Signal sent to %d\n", ft_atoi(argv[1]));
	}
	return (0);
}