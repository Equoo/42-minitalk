/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 13:54:37 by dderny            #+#    #+#             */
/*   Updated: 2025/02/18 22:29:55 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

# define INITERR_CATEGORY "Initialization"
# define INITERROR 1000
# define ERRINITARGS INITERROR + 1
# define ERRINITVARS INITERROR + 2
# define ERRINITNOSIG INITERROR + 3
# define ERRINITPID INITERROR + 4

int	print_error(char *category);
int	print_cerror(char *category, int error);

#endif