/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 15:22:25 by dderny            #+#    #+#             */
/*   Updated: 2025/02/24 15:16:33 by dderny           ###   ########.fr       */
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

# if CLIENT == 1 && SERVER == 1
#  error "CLIENT and SERVER can't be defined at the same time"
# endif

# ifndef MSG_MAX
#  define MSG_MAX 1000000
# endif

/* 
** Timeout time in seconds
*/
# ifndef TIMEOUT_TIME
#  define TIMEOUT_TIME 2
# endif

uint64_t hash(uint8_t *str);

#endif