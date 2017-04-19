
/* written by jkan */

#include <linux/uio.h>
#include <linux/kprobes.h>
#include <linux/kallsyms.h>

#include "help.h"

#ifndef _HOOK_H_
#define _HOOK_H_

typedef struct  {
    struct jprobe   _jprobe;
    spinlock_t   	_spinlock;
} hooker_t;

hooker_t* hooker_create(void);

void hooker_release(hooker_t** pMe);

int hooker_enable(hooker_t* pMe);

int hooker_disable(hooker_t* pMe);

#endif
