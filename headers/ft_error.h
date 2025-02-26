/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 13:54:37 by dderny            #+#    #+#             */
/*   Updated: 2025/02/26 16:09:31 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_ERROR_H
# define FT_ERROR_H

# define INITERR_CATEGORY "Initialization"
# define INITERROR 1000
# define ERRINITARGS 1001
# define ERRINITVARS 1002
# define ERRINITNOSIG 1003
# define ERRINITPID 1004
# define ERRINITNOFIRSTSIG 1005
# define ERRMESSAGELEN 1006
# define ERRSERVERRETRY 1007

int	print_error(char *category);
int	print_cerror(char *category, int error);

#endif
