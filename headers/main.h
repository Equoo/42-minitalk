/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 15:22:25 by dderny            #+#    #+#             */
/*   Updated: 2025/02/19 11:59:51 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
# define MAIN_H

# ifndef CLIENT
#  define CLIENT 0
# endif

# ifndef SERVER
#  define SERVER 0
# endif

# if CLIENT == 1 && SERVER == 1
#  error "CLIENT and SERVER can't be defined at the same time"
# endif

typedef enum e_sigwait
{
	SIGWAITWRITE,
	SIGWAITREAD,
	SIGWAITRES,
	SIGWAITRY
} t_sigwait;

#endif