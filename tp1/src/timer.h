#pragma once

#include <sys/time.h>
#include <sys/resource.h>

typedef struct {
    struct rusage rtime;
} instant;

instant instant_now();

void instant_print_elapsed(instant after, instant before);
