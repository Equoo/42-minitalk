/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 15:22:25 by dderny            #+#    #+#             */
/*   Updated: 2025/02/26 16:35:46 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
# define MAIN_H
# include <stdint.h>

# ifndef MSG_MAX
#  define MSG_MAX 1000000
# endif

/*
** Timeout time in microseconds / 1000000 = seconds
*/
# ifndef TIMEOUT_TIME
#  define TIMEOUT_TIME 500000
# endif

uint64_t	hash(uint8_t *str);
int			init_client(char **argv);
int			init_server(void);

#endif
