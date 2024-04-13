#ifndef BIT_H
#define BIT_H


#define BIT_FLIP(x, i) x ^ ((x) & (0x1 << (i)))

#define    BIT_MASK(x, i, n) ((x) & ((0xffffffff >> (32 - (n)))      <<  (i)))
#define NIBBLE_MASK(x, i, n) ((x) & ((0xffffffff >> (32 - (n * 4)))  << ((i) * 4)))
#define   BYTE_MASK(x, i, n) ((x) & ((0xffffffff >> (32 - (n * 8)))  << ((i) * 8)))
#define  DBYTE_MASK(x, i, n) ((x) & ((0xffffffff >> (32 - (n * 16))) << ((i) * 16)))


#endif