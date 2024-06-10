/*
==============================================================================
Author: Michael Gene Brockus (Dreamer)
Email: michaelbrockus@gmail.com
Organization: Fossil Logic
Description: 
    This file is part of the Fossil Logic project, where innovation meets
    excellence in software development. Michael Gene Brockus, also known as
    "Dreamer," is a dedicated contributor to this project. For any inquiries,
    feel free to contact Michael at michaelbrockus@gmail.com.
==============================================================================
*/
#include "fossil/threads/thread.h"
#include "fossil/common/common.h"
#include <stdlib.h>

// Platform-specific thread start routine function
#ifdef _WIN32
DWORD WINAPI thread_start_routine(LPVOID arg) {
    xtask_t task = *(xtask_t*)arg;
    xtask_func_t task_func = task.task_func;
    xtask_arg_t task_arg = task.arg;
    if (task_func) {
        task_func(task_arg);
    }
    return FOSSIL_SUCCESS;
}
#else
void* thread_start_routine(void *arg) {
    xtask_t task = *(xtask_t*)arg;
    xtask_func_t task_func = task.task_func;
    xtask_arg_t task_arg = task.arg;
    if (task_func) {
        task_func(task_arg);
    }
    return cnullptr;
}
#endif

int32_t fossil_thread_create(xthread_t *thread, xthread_attr_t *attr, xtask_t task) {
    if (!thread || !task.task_func) return FOSSIL_ERROR;

    // Create a new thread attribute if not provided
    xthread_attr_t default_attr;
    if (!attr) {
        if (fossil_thread_attr_create(&default_attr) != 0) {
            return FOSSIL_ERROR;
        }
        attr = &default_attr;
    }

    // Create the thread using the provided attributes and start routine
    #ifdef _WIN32
    *thread = CreateThread(cnullptr, 0, thread_start_routine, (LPVOID)&task, 0, cnullptr);
    #else
    int32_t result = pthread_create(thread, attr, thread_start_routine, (void *)&task);
    #endif

    // Destroy the default thread attribute if created
    if (!attr) {
        fossil_thread_attr_erase(&default_attr);
    }

    #ifndef _WIN32
    return (result == 0) ? FOSSIL_SUCCESS : FOSSIL_ERROR;
    #else
    return (*thread != cnullptr) ? FOSSIL_SUCCESS : FOSSIL_ERROR;
    #endif
}

int32_t fossil_thread_join(xthread_t thread, void **retval) {
    if (!thread) return FOSSIL_ERROR;

#ifdef _WIN32
    DWORD result = WaitForSingleObject(thread, INFINITE);
    if (result == WAIT_OBJECT_0) {
        if (retval) {
            // Get the exit code
            GetExitCodeThread(thread, (LPDWORD)retval);
        }
        CloseHandle(thread);
        return FOSSIL_SUCCESS;
    } else {
        return FOSSIL_ERROR;
    }
#else
    return pthread_join(thread, retval);
#endif
}

int32_t fossil_thread_detach(xthread_t thread) {
    if (!thread) return FOSSIL_ERROR;

#ifdef _WIN32
    return CloseHandle(thread) ? FOSSIL_SUCCESS : FOSSIL_ERROR;
#else
    return pthread_detach(thread);
#endif
}

int32_t fossil_thread_attr_create(xthread_attr_t *attr) {
    if (!attr) return FOSSIL_ERROR;

#ifdef _WIN32
    attr->stack_size = 0;
    attr->detach_state = 0;
    return FOSSIL_SUCCESS;
#else
    return pthread_attr_init(attr);
#endif
}

int32_t fossil_thread_attr_erase(xthread_attr_t *attr) {
    if (!attr) return FOSSIL_ERROR;

#ifdef _WIN32
    // No cleanup required for Windows attributes
    return FOSSIL_SUCCESS;
#else
    return pthread_attr_destroy(attr);
#endif
}