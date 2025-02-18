/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 23:58:57 by dderny            #+#    #+#             */
/*   Updated: 2025/02/18 13:44:23 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include <ft_printf.h>
#include <errno.h>
#include <string.h>

static char	*str_custom_errors(int err)
{
	if (err == ERRINITARGS)
		return ("Invalid arguments");
	if (err == ERRINITVARS)
		return ("No SERVER or CLIENT defined");
	if (err == ERRINITNOSIG)
		return ("Message timeout, no signal received from the server");
	return ("Unknown error");
}

/*
** Print a custom error message
** @param category: the category of the error
** @param error: the error message
** @return 1
*/
int	print_cerror(char *category, int error)
{
	static int	err = 0;
	char		*errorstr;

	if (err)
		return (error);
	errno = error;
	err = 1;
	if (error > 133)
		errorstr = str_custom_errors(error);
	else
		errorstr = strerror(error);
	ft_printf("Error\n%s: %s\n", category, errorstr);
	return (error);
}

/*
** Print an error message
** @param category: the category of the error
** @return errno
*/
int	print_error(char *category)
{
	print_cerror(category, errno);
	return (errno);
}
