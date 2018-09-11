//
// Hodaya Koslowsky 313377673
//

#include <stdio.h>
#include "ex1.h"
#define NUM_OF_BITS 8
#define MASK 0xff

/**
 * in little endian the LSB is in a lower address. We will take the hexadecimal number 1
 * and check if the pointer to the first, and lower address is pointing to 1 or 0;
 */
int is_little_endian() {

    unsigned long x = 0x1; //1 is the byte of the LSB

    char* p = (char*)&x;

    if(*p == 1) {
      return 1;
    }
    return 0;
}

/**
 * We'll use a "mask" - a long unsigned int which has certain bits on 1.
 * The bits will be an unsigned long that is represented as ff in hexadecimal,
 * has a last byte of 8 1's, and the other digits are 0's. The result of a
 * bitwise "and" - & with ff and y will give us the last byte. Then we'll
 * do the bitwise operation of "not" ~ for the mask, and then "and" to the mask
 * and x. This will give us all digits of x besides the last bit.
 * The final result is the new x, plus the new y.
 * @param x an unsigned long.
 * @param y an unsigned long.
 * @return the last byte of y and all other bytes of x in one number.
 */
unsigned long merge_bytes(unsigned long x, unsigned long int y) {
    unsigned long mask = MASK;
    unsigned long lastByteY = mask & y;
    mask = ~mask;
    unsigned long bytesX = mask & x;
    return bytesX + lastByteY;
}

/**
 * Again we'll use a mask, this time the i byte will have 8 0's and the other bytes
 * will 1's. We will move b 8 * i places to the left, and the result will be x & the mask, plus b.
 * @param x an unsigned long
 * @param b an unsigned char that represents the byte that will replace
 * one byte of x.
 * @param i the number of byte to change - the LSB byte is 0,
 * and the MSB byte is number of bits/8 - 1
 * @return the number x, with its i'th byte replaced by b
 */
unsigned long put_byte(unsigned long x, unsigned char b, int i) {

    if (i >= sizeof(x)) {
        printf("i is too big.");
        return 0;
    }
    unsigned long mask = MASK;
    unsigned long newByte = (unsigned long)b;
    mask = ~ (mask << (NUM_OF_BITS * i));
    newByte = newByte << (NUM_OF_BITS * i);
    return (x & mask) + newByte;
}