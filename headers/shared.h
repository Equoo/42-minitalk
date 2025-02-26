/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shared.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 16:35:23 by dderny            #+#    #+#             */
/*   Updated: 2025/02/26 16:35:30 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHARED_H
# define SHARED_H

# ifndef CLIENT
#  define CLIENT 0
# endif

# ifndef SERVER
#  define SERVER 0
# endif

# if CLIENT == 1 && SERVER == 1 && !DEBUG
#  error "CLIENT and SERVER can't be defined at the same time"
# endif

#endif
