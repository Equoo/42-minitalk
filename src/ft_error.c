/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 23:58:57 by dderny            #+#    #+#             */
/*   Updated: 2025/03/12 11:30:02 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_errno.h>
#include <ft_printf.h>

#include "ft_error.h"

#include <stdlib.h>
#include <sys/time.h>

double get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec + tv.tv_usec / 1000000.0);
}

static char	*str_custom_errors(int err)
{
	if (err == ERRINITUSAGE)
		return ("./client [PID] [message]");
	if (err == ERRINITPID)
		return ("Invalid PID");
	//if (err == ERRINITVARS)
		//return ("No SERVER or CLIENT defined");
	//if (err == ERRINITNOSIG)
		//return ("Message timeout, no signal received from the server");
	if (err == ERRINITLEN)
		return ("Message length is invalid");
	if (err == ERRUSER_MAX)
		return ("Too many connected");
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
		errorstr = ft_strerror(error);
	ft_printf("%s: %s\n", category, errorstr);
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
