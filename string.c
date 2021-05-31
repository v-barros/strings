/*
 * string.c
 *
 *  Created on: 2021-05-29
 *      Author: @v-barros
 */
#include <assert.h>
#include <stdlib.h>
#include "string.h"

void dec_ref_count(struct String * string);

void inc_ref_count(struct String * string);