/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 22:18:56 by dderny            #+#    #+#             */
/*   Updated: 2025/03/01 17:46:32 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>

#include "ft_printf.h"
#include "shared.h"
#include "main.h"                    
#include "ft_error.h"

int	main(int argc, char **argv)
{
	if (CLIENT && argc != 3)
		return (print_cerror("Usage", ERRINITARGS));
	if (CLIENT)
		return (init_client(argv));
	else
		init_server();
	sleep(4);
	while (1)
	{
		//unsigned int elapsed = sleep(2);
		//ft_printf("Resume pause %d\n", elapsed);
		usleep(5000);
		ft_printf("Loop\n");
	}
	return (EXIT_SUCCESS);
}
