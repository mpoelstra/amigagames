//
// Created by Rich/Defekt on 21.02.2024.
//

#pragma once
#ifndef WINDOWS_COMPAT_H
#define WINDOWS_COMPAT_H

#include <stdio.h>
#include <string.h>

#ifdef MACOS_LINUX

#define fprintf_s fprintf

int fopen_s(FILE** h, const char* fname, const char* mode);

static inline int strncpy_s(char* destination, const char* source,
                            size_t destinationSize)
{
    if (destinationSize == 0)
        return 1;
    strncpy(destination, source, destinationSize - 1);
    destination[destinationSize - 1] = '\0';
    return 0;
}

#endif

#endif
