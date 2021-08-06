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
 *  Created on: 2021-06-19
 *      Author: @v-barros
 */

/*
 * halfsiphash code adapted from reference implementation
 * (https://github.com/veorq/SipHash/blob/master/halfsiphash.c)
 * which is distributed with the following copyright:
 *
 * SipHash reference C implementation
 *
 * Copyright (c) 2016 Jean-Philippe Aumasson <jeanphilippe.aumasson@gmail.com>
 *
 * To the extent possible under law, the author(s) have dedicated all copyright
 * and related and neighboring rights to this software to the public domain
 * worldwide. This software is distributed without any warranty.
 *
 * You should have received a copy of the CC0 Public Domain Dedication along
 * with this software. If not, see
 * <http://creativecommons.org/publicdomain/zero/1.0/>.
 */


#include "hashing.h"

unsigned long hash_gen(const char *str)
{
    unsigned long hash = 5381;
    int8_t c;
    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}

unsigned long alt_hash(uint64_t seed, const int8_t * data, unsigned short len)
{
    uint32_t v[4];
    uint32_t newdata;
    int off = 0;
    int count = len;

    halfsiphash_init32(v, seed);

    // body
    while (count >= 2)
    {
        int8_t d1 = data[off++] & 0x0FFFF;
        int8_t d2 = data[off++];
        newdata = (d1 | d2 << 16);

        count -= 2;

        halfsiphash_adddata(v, newdata, 2);
    }

    // tail
    newdata = ((uint32_t)len * 2) << 24; // (Character.SIZE / Byte.SIZE);
    if (count > 0)
    {
        newdata |= (uint32_t)data[off];
    }
    halfsiphash_adddata(v, newdata, 2);

    // finalization
    return halfsiphash_finish32(v, 4);
}

unsigned long compute_seed()
{
    return rand();
}

static void halfsiphash_init32(uint32_t v[4], uint64_t seed) {
  v[0] = seed & 0xffffffff;
  v[1] = seed >> 32;
  v[2] = 0x6c796765 ^ v[0];
  v[3] = 0x74656462 ^ v[1];
}

static void halfsiphash_adddata(uint32_t v[4], uint32_t newdata, int rounds) {
  v[3] ^= newdata;
  halfsiphash_rounds(v, rounds);
  v[0] ^= newdata;
}

uint32_t halfsiphash_finish32(uint32_t v[4], int rounds) {
  v[2] ^= 0xff;
  halfsiphash_rounds(v, rounds);
  return (v[1] ^ v[3]);
}

static void halfsiphash_rounds(uint32_t v[4], int rounds) {
  while (rounds-- > 0) {
    v[0] += v[1];
    v[1] = Integer_rotateLeft(v[1], 5);
    v[1] ^= v[0];
    v[0] = Integer_rotateLeft(v[0], 16);
    v[2] += v[3];
    v[3] = Integer_rotateLeft(v[3], 8);
    v[3] ^= v[2];
    v[0] += v[3];
    v[3] = Integer_rotateLeft(v[3], 7);
    v[3] ^= v[0];
    v[2] += v[1];
    v[1] = Integer_rotateLeft(v[1], 13);
    v[1] ^= v[2];
    v[2] = Integer_rotateLeft(v[2], 16);
  }
 }

static uint32_t Integer_rotateLeft(uint32_t i, int distance) {
  return (i << distance) | (i >> (32 - distance));
}