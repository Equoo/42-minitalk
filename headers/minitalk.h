/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 15:22:25 by dderny            #+#    #+#             */
/*   Updated: 2025/03/04 15:15:05 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
# define MAIN_H
# include <stdint.h>

# ifndef CLIENT
#  define CLIENT 0
# endif

# ifndef SERVER
#  define SERVER 0
# endif

# if CLIENT == 1 && SERVER == 1 && !DEBUG
#  error "CLIENT and SERVER can't be defined at the same time"
# endif

// 32 bit maximum
# ifndef MSG_MAX
#  define MSG_MAX 4294967296
# endif

/*
** Timeout time in microseconds / 1000000 = seconds
*/
# ifndef TIMEOUT_TIME
#  define TIMEOUT_TIME 500000
# endif

int			init_client(char **argv);
int			init_server(void);

#endif
