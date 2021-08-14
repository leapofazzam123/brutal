#include <brutal/host/sync.h>
#include <brutal/sync/lock.h>

static bool lock_try_acquire_impl(Lock *lock)
{
    bool expected = false;
    auto result = atomic_compare_exchange_weak(&lock->locked, &expected, true);

    atomic_thread_fence(memory_order_seq_cst);

    return result;
}

bool lock_try_acquire(Lock *lock)
{
    host_enter_critical_section();

    auto result = lock_try_acquire_impl(lock);

    if (!result)
    {
        host_leave_critical_section();
    }

    return result;
}

void lock_acquire(Lock *lock)
{
    host_enter_critical_section();

    while (!lock_try_acquire_impl(lock))
    {
        asm("pause");
    }

    atomic_thread_fence(memory_order_seq_cst);
}

void lock_release(Lock *lock)
{
    atomic_thread_fence(memory_order_seq_cst);
    lock->locked = false;

    host_leave_critical_section();
}