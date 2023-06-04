#ifndef TEST_SYNC_H
#define TEST_SYNC_H

#include "test_util.h"
#include "procLib.h"
#include "semLib.h"
#include <stdint.h>
#include "stdioAPI.h"
#include "syscallsAPI.h"

uint64_t test_sync(); //{n, use_sem, 0}

#endif