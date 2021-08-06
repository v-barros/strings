/*
 *MIT License
 *
 *Copyright (c) 2021 v-barros
 *
 *Permission is hereby granted, free of charge, to any person obtaining a copy
 *of this software and associated documentation files (the "Software"), to deal
 *in the Software without restriction, including without limitation the rights
 *to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *copies of the Software, and to permit persons to whom the Software is
 *furnished to do so, subject to the following conditions:
 *
 *The above copyright notice and this permission notice shall be included in all
 *copies or substantial portions of the Software.
 *
 *THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *SOFTWARE.
 *
 *hashing.h
 *
 *  Created on: 2021-06-09
 *      Author: @v-barros
 */

#ifndef _HASHING_
#define _HASHING_
#include <inttypes.h>
#include <stdlib.h>

 /* DJB2. See: https://theartincode.stanis.me/008-djb2/ */
unsigned long hash_gen(const char *str);

/* Check: https://github.com/veorq/SipHash/ and https://www.aumasson.jp/siphash/siphash.pdf */
unsigned long alt_hash(uint64_t seed, const int8_t * str, unsigned short len);

/* Seed value used for each alternative hash calculated. */
unsigned long compute_seed();

static void halfsiphash_rounds(uint32_t v[4], int rounds);

static void halfsiphash_init32(uint32_t v[4], uint64_t seed);

static void halfsiphash_adddata(uint32_t v[4], uint32_t newdata, int rounds);

uint32_t halfsiphash_finish32(uint32_t v[4], int rounds);

// utility function copied from java/lang/Integer
static uint32_t Integer_rotateLeft(uint32_t i, int distance);

#endif /*_HASHIGN_*/