/**
 * Copyright 2011 Urs Fässler, www.bitzgi.ch
 * SPDX-License-Identifier:	GPL-3.0+
 */
#include    "processes.h"
#include    <stdlib.h>
#include    <string.h>
#include    "../util/errhandler.h"

TREE_DEFINE(process, RECORD_TREE_LINK);

static int compare(struct process *lhs, struct process *rhs) {
    if( lhs->pid > rhs->pid ){
        return  1;
    } else if( lhs->pid < rhs->pid ){
        return  -1;
    } else {
        return 0;
    }
}

process_tree_t init_processes(){
    process_tree_t res = TREE_INITIALIZER(compare);
    return  res;
}

struct process* create_process( process_tree_t *tree, u32 pid ){
    struct process *proc    = (struct process *)malloc( sizeof(*proc) );
    if( proc != NULL ){
        memset( proc, 0, sizeof(*proc) );
        proc->exit_time = -1;
        proc->pid = pid;
        TREE_INSERT( tree, process, RECORD_TREE_LINK, proc);
    }
    return proc;
};

bool remove_process( process_tree_t *tree, struct process* proc ){
    TREE_REMOVE( tree, process, RECORD_TREE_LINK, proc);
    return true;
};


void iterate_processes( process_tree_t *tree, void (*callback)(struct process *proc, void *data), void *data ){
    TREE_FORWARD_APPLY( tree, process, RECORD_TREE_LINK, callback, data);
};

struct rmmap* find_mmap( struct process* proc, u64 addr ){
    struct rmmap *itr;
    for( itr = proc->mmaps; itr != NULL; itr = itr->next ){
        if( (addr >= itr->start) && (addr <= itr->end) ){
            return itr;
        }
    }
    return NULL;
};

struct process* find_process( process_tree_t *tree, u32 pid ){
    struct process test;
    memset( &test, 0, sizeof(test) );
    test.pid = pid;
    struct process *proc = TREE_FIND( tree, process, RECORD_TREE_LINK, &test);
    return proc;
};
