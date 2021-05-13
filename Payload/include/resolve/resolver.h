#pragma once

#include "resolver-505.h"
#include "resolver-672.h"
#include "resolver-702.h"
#include "resolver-755.h"

#ifndef resolve
#define resolve(x) ((void*)((uint8_t *)&gpKernelBase[x]))
#endif

void Resolve(uint64_t kernbase);