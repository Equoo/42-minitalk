/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 12:01:42 by dderny            #+#    #+#             */
/*   Updated: 2025/02/26 16:29:18 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include <ft_bool.h>
# include <signal.h>
# include <stddef.h>
# include <stdint.h>

# include "users.h"

# ifndef LAG_FACTOR
#  define LAG_FACTOR 1
# endif

int		killandwait(pid_t pid, uint8_t sig);
int		sigwritenbr(int pid, int bits, size_t n);
int		sigreadnbr(t_user *user, uint8_t bit, uint8_t bits, uint64_t *n);
int		toggle_signals(t_bool toggle);

void	client_sig_handler(int sig, siginfo_t *info, void *context);
void	server_sig_handler(int sig, siginfo_t *info, void *context);

#endif
