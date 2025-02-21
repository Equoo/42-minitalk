/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 23:58:57 by dderny            #+#    #+#             */
/*   Updated: 2025/02/21 02:39:31 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include <ft_printf.h>
#include <errno.h>
#include <string.h>

static char	*str_custom_errors(int err)
{
	if (err == ERRINITARGS)
		return ("./client [PID] [message]");
	if (err == ERRINITPID)
		return ("Invalid PID");
	if (err == ERRINITVARS)
		return ("No SERVER or CLIENT defined");
	if (err == ERRINITNOSIG)
		return ("Message timeout, no signal received from the server");
	return ("Unknown error");
}

static char	*ft_strerror(int err)
{
	if (err == EACCES)
		return ("Permission denied");
	if (err == EEXIST)
		return ("File exists");
	if (err == EFAULT)
		return ("Bad address");
	if (err == EINTR)
		return ("Interrupted system call");
	if (err == EINVAL)
		return ("Invalid argument");
	if (err == EIO)
		return ("I/O error");
	if (err == EISDIR)
		return ("Is a directory");
	if (err == EMFILE)
		return ("Too many open files");
	if (err == ENAMETOOLONG)
		return ("File name too long");
	if (err == ENFILE)
		return ("File table overflow");
	if (err == ENOENT)
		return ("No such file or directory");
	if (err == ENOMEM)
		return ("Out of memory");
	if (err == ENOSPC)
		return ("No space left on device");
	if (err == ENOTDIR)
		return ("Not a directory");
	if (err == EPERM)
		return ("Operation not permitted");
	if (err == EPIPE)
		return ("Broken pipe");
	if (err == EROFS)
		return ("Read-only file system");
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
