/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 22:18:56 by dderny            #+#    #+#             */
/*   Updated: 2025/02/26 16:35:51 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>

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
	while (1)
		pause();
	return (EXIT_SUCCESS);
}
