#pragma once

#include <handover/handover.h>
#include <syscalls/types.h>

#define PMM_USED (true)
#define PMM_UNUSED (false)

typedef USizeRange PmmRange;

typedef Result(BrResult, PmmRange) PmmResult;

void pmm_initialize(Handover const *handover);

PmmResult pmm_alloc(size_t size);

PmmResult pmm_used(PmmRange range);

PmmResult pmm_unused(PmmRange range);
