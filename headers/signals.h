/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 12:01:42 by dderny            #+#    #+#             */
/*   Updated: 2025/02/25 23:51:50 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

#include <bits/types/siginfo_t.h>
#include <ft_bool.h>
#include <stddef.h>
#include <stdint.h>

#include "users.h"

#ifndef LAG_FACTOR
# define LAG_FACTOR 1
#endif

int		killandwait(pid_t pid, uint8_t sig);
int		sigwritenbr(int pid, int bits, size_t n);
int		sigreadnbr(t_user *user, uint8_t bit, uint8_t bits, uint64_t *n);
int		toggle_signals(t_bool toggle);

void	server_sig_handler(int sig, siginfo_t *info, void *context);
void	client_sig_handler(int sig);

void	readmessage(t_user *user, uint8_t bit);

#endif