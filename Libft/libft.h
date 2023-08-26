/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhrima <mhrima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/09 10:45:22 by mhrima            #+#    #+#             */
/*   Updated: 2023/08/26 02:18:18 by mhrima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
#define LIBFT_H

#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdarg.h>
#include <math.h>
#include <assert.h>
#include <string.h>

#define ERROR -1

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1
#endif

#ifndef in
#define in STDIN_FILENO
#endif
#ifndef out
#define out STDOUT_FILENO
#endif
#ifndef err
#define err STDERR_FILENO
#endif

#ifndef OPEN_MAX
#define OPEN_MAX FOPEN_MAX
#endif

#include "ctype/ft_ctype.h"
#include "memory/ft_memory.h"
#include "stdlib/ft_stdlib.h"
#include "string/ft_string.h"
#include "stdio/ft_stdio.h"


#endif
