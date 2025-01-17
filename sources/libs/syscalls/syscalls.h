#pragma once

#include <syscalls/types.h>

// clang-format off

#ifdef __kernel__
#    define SYSCALL(NAME) BrResult sys_##NAME
#else
#    define SYSCALL(NAME) BrResult br_##NAME
SYSCALL(syscall) (BrSyscall syscall, BrArg arg1, BrArg arg2, BrArg arg3, BrArg arg4, BrArg arg5);
#endif

typedef struct
{
    char const *message;
    size_t size;
} BrLogArgs;

SYSCALL(log) (BrLogArgs* args);

typedef struct
{
    BrSpace space;
    BrMemObj mem_obj;
    size_t offset;
    size_t size;
    uintptr_t vaddr;
    BrMemFlags flags;
} BrMapArgs;

SYSCALL(map) (BrMapArgs* args);

typedef struct
{
    BrSpace space;
    uintptr_t vaddr;
    size_t size;
} BrUnmapArgs;

SYSCALL(unmap) (BrUnmapArgs* args);

typedef struct
{
    StrFix128 name;
    BrSpace space;
    BrCap caps;
    BrTaskFlags flags;
} BrCreateTaskArgs;

typedef struct
{
    uintptr_t addr;
    size_t size;
    BrMemObjFlags flags;
} BrCreateMemObjArgs;

typedef struct
{
    BrSpaceFlags flags;
} BrCreateSpaceArgs;

typedef struct
{
    BrIrqId irq;
} BrCreateIrqArgs;

typedef struct
{
    BrObjectType type;
    BrId id;
    BrHandle handle;

    union
    {
        BrCreateTaskArgs task;
        BrCreateMemObjArgs mem_obj;
        BrCreateSpaceArgs space;
        BrCreateIrqArgs irq;
    };
} BrCreateArgs;

SYSCALL(create) (BrCreateArgs* args);

typedef struct
{
    BrTask task;
    uintptr_t ip;
    uintptr_t sp;
    BrTaskArgs args;
} BrStartArgs;

SYSCALL(start) (BrStartArgs* args);

typedef struct
{
    BrTask task;
    uintptr_t exit_value;
} BrExitArgs;

SYSCALL(exit) (BrExitArgs* args);

typedef struct
{
    BrId to;
    BrMsg msg;
    BrDeadline deadline;
    BrIpcFlags flags;
} BrIpcArgs;

SYSCALL(ipc) (BrIpcArgs* args);


typedef struct
{
    BrTask task;
    BrCap cap;
} BrDropArgs;

SYSCALL(drop) (BrDropArgs* args);

typedef struct
{
    BrHandle handle;
} BrCloseArgs;

SYSCALL(close) (BrCloseArgs* args);

typedef struct
{
    BrIrq handle;
    BrIrqFlags flags;
} BrBindArgs;

SYSCALL(bind) (BrBindArgs* args);

typedef struct
{
    BrIrq handle;
    BrIrqFlags flags;
} BrUnbindArgs;

SYSCALL(unbind) (BrUnbindArgs* args);


typedef struct
{
    BrIrq handle;
    BrIrqFlags flags;
} BrAckArgs;

SYSCALL(ack) (BrAckArgs* args);

typedef struct 
{
    BrHandle handle;
    BrHandleInfo info;
} BrStatArgs;

SYSCALL(stat)(BrStatArgs* args);
// clang-format on
