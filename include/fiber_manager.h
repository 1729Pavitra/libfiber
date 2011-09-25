#ifndef _FIBER_MANAGER_H_
#define _FIBER_MANAGER_H_

#include "fiber.h"
#include "work_stealing_deque.h"

typedef struct fiber_manager
{
    fiber_t* volatile current_fiber;
    fiber_t* thread_fiber;
    wsd_work_stealing_deque_t* queue_one;
    wsd_work_stealing_deque_t* queue_two;
    wsd_work_stealing_deque_t* volatile schedule_from;
    wsd_work_stealing_deque_t* volatile store_to;
    wsd_work_stealing_deque_t* done_fibers;
    /* TODO: done_fibers may be better as a global queue to increase re-use, with the cost of added contention */
    /* TODO: done_fibers could also be setup to allow a thread to steal done fibers from other threads */
    int id;
    size_t yield_count;
} fiber_manager_t;

#ifdef __cplusplus
extern "C" {
#endif

extern fiber_manager_t* fiber_manager_create();

extern void fiber_manager_destroy(fiber_manager_t* manager);

extern void fiber_manager_schedule(fiber_manager_t* manager, fiber_t* the_fiber);

extern void fiber_manager_yield(fiber_manager_t* manager);

extern fiber_manager_t* fiber_manager_get();

/* this should be called immediately when the applicaion starts */
extern int fiber_manager_set_total_kernel_threads(size_t num_threads);

#define FIBER_MANAGER_STATE_NONE (0)
#define FIBER_MANAGER_STATE_STARTED (1)
#define FIBER_MANAGER_STATE_ERROR (2)

extern int fiber_manager_get_state();

extern int fiber_manager_get_kernel_thread_count();

#ifdef __cplusplus
}
#endif

#endif
