/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 13:54:37 by dderny            #+#    #+#             */
/*   Updated: 2025/03/17 11:09:54 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_ERROR_H
# define FT_ERROR_H

# define ERRINIT "Initialization"
# define ERRUSAGE "Usage"
# define ERRINITUSAGE 1000
# define ERRMESS "Message"
# define ERRINITLEN 1001

# define ERRUSER "User connection"
# define ERRUSER_MAX 1100
# define ERRUSER_TIMEOUT 1101

# define ERRPACKETS "Packet processing"
# define ERRPACKETS_DIFFER 2000
# define ERRPACKETS_ANORMAL 2001

int	print_error(char *category);
int	print_cerror(char *category, int error);

#endif
