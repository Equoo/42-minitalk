/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 13:54:37 by dderny            #+#    #+#             */
/*   Updated: 2025/03/04 16:28:50 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_ERROR_H
# define FT_ERROR_H

# define ERRINIT "Initialization"
# define ERRINITUSAGE 1000
# define ERRINITPID 1001

# define ERRUSER "Users"
# define ERRUSER_MAX 1100

# define ERRPACKETS "Packet processing"
# define ERRPACKETS_DIFFER 2000
# define ERRPACKETS_TIMEOUT 2001

int	print_error(char *category);
int	print_cerror(char *category, int error);

#endif
