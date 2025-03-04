/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:14:30 by dderny            #+#    #+#             */
/*   Updated: 2025/03/04 15:27:00 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <ft_printf.h>
#include <unistd.h>

#include "packet.h"
#include "ft_error.h"

int	init_server(void)
{
	ft_printf("Server PID: %d\n", getpid());
	return (EXIT_SUCCESS);
}

