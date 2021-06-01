/*
 * memory.c
 *
 *  Created on: 2021-05-31
 *      Author: @v-barros
 */
#include <stdlib.h>
#include <assert.h>
#include "memory.h"

void * alloc(const void * type){ 
    const size_t  size = * (const size_t *) type;
    void * p = calloc(1, size);
    assert(p);
    return p;
}
 
