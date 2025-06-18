/*
*	Bytewave: A Lightweight Digital Audio Workstation
*	Copyright (C) 2025  Ben Doepker
*
*	This program is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	This program is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "bw-log.h"
#include <stdarg.h>
#include <stdio.h>

void _bw_log(const char* s, ...) {
    va_list ap;
    va_start(ap, s);
    vfprintf(stderr, s, ap);
    va_end(ap);
    fprintf(stderr, "\n");
    fflush(stderr);
}

void _bw_print(const char* s, ...) {
    va_list ap;
    va_start(ap, s);
    vfprintf(stdout, s, ap);
    va_end(ap);
    fprintf(stdout, "\n");
    fflush(stdout);
}
