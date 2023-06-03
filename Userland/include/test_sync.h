#ifndef TEST_SYNC_H
#define TEST_SYNC_H

#include <stdint.h>
#include <stdio.h>
#include <semLib.h>
#include <procLib.h>
#include "test_util.h"
#include <syscallsAPI.h>

uint64_t test_sync(); //{n, use_sem, 0}

#endif