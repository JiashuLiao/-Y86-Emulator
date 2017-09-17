#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "y86emul.h"

void disass();


int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage:\ty86dis <inputfile>\n");
        return 0;
    }
    if (strcmp(argv[1], "-h") == 0) {
        printf("Usage:\ty86dis <inputfile>\n");
        return 0;
    }

    load_program(argv[1] );

    if (memory == NULL) {
        fprintf(stderr, "no .size section.\n");
        return 0;
    }
    if (pc == NULL) {
        fprintf(stderr, "no .text section.\n");
        return 0;
    }

    disass();
    free(memory);

    return 0;
}

void disass() {

    while (pc < code_ends) {

        if (*pc == 0x00) {
            printf("nop\n");
            pc += 1;
        } else if (*pc == 0x10) {
            printf("halt\n");
            pc += 1;
        } else if (*pc == 0x20) {
            uint8_t va = 0;
            uint8_t rb = 0;
            uint8_t ra = 0;

            va = *(pc + 1);
            ra = (va & 0xf0) >> 4;
            rb = va & 0x0f;

            printf("rrmovl %d, %d\n", ra, rb);
            pc += 2;
        } else if (*pc == 0x30) {
            uint8_t va = 0;
            uint8_t rb = 0;

            va = *(pc + 1);
            rb = va & 0x0f;

            printf("irmovl 0x%08x, %d\n", *((uint32_t*)(pc + 2)), rb);
            pc += 6;
        } else if (*pc == 0x40) {
            uint8_t va = 0;
            uint8_t rb = 0;
            uint8_t ra = 0;

            va = *(pc + 1);
            ra = (va & 0xf0) >> 4;
            rb = va & 0x0f;

            printf("rmmovl %d, 0x%08x\n", ra, reg[rb]);
            pc += 6;
        } else if (*pc == 0x50) {
            uint8_t va = 0;
            uint8_t rb = 0;
            uint8_t ra = 0;

            va = *(pc + 1);
            ra = (va & 0xf0) >> 4;
            rb = va & 0x0f;

            printf("mrmovl 0x%08x, %d\n", reg[rb], ra);
            pc += 6;
        } else if (*pc == 0x60) {
            uint8_t va = 0;
            uint8_t rb = 0;
            uint8_t ra = 0;

            va = *(pc + 1);
            ra = (va & 0xf0) >> 4;
            rb = va & 0x0f;

            printf("addl %d, %d\n", ra, rb);
            pc += 2;
        } else if (*pc == 0x61) {
            uint8_t va = 0;
            uint8_t rb = 0;
            uint8_t ra = 0;

            va = *(pc + 1);
            ra = (va & 0xf0) >> 4;
            rb = va & 0x0f;

            printf("subl %d, %d\n", ra, rb);
            pc += 2;
        } else if (*pc == 0x62) {
            uint8_t va = 0;
            uint8_t rb = 0;
            uint8_t ra = 0;

            va = *(pc + 1);
            ra = (va & 0xf0) >> 4;
            rb = va & 0x0f;

            printf("andl %d, %d\n", ra, rb);
            pc += 2;
        } else if (*pc == 0x63) {
            uint8_t va = 0;
            uint8_t rb = 0;
            uint8_t ra = 0;

            va = *(pc + 1);
            ra = (va & 0xf0) >> 4;
            rb = va & 0x0f;

            printf("xorl %d, %d\n", ra, rb);
            pc += 2;
        } else if (*pc == 0x64) {
            uint8_t va = 0;
            uint8_t rb = 0;
            uint8_t ra = 0;

            va = *(pc + 1);
            ra = (va & 0xf0) >> 4;
            rb = va & 0x0f;

            printf("mull %d, %d\n", ra, rb);
            pc += 2;
        } else if (*pc == 0x65) {
            uint8_t va = 0;
            uint8_t rb = 0;
            uint8_t ra = 0;

            va = *(pc + 1);
            ra = (va & 0xf0) >> 4;
            rb = va & 0x0f;

            printf("cmpl %d, %d\n", ra, rb);
            pc += 2;
        } else if (*pc == 0x70) {
            uint32_t offset = *((uint32_t*)(pc + 1));

            printf("jmp 0x%08x\n", offset);
            pc += 5;
        } else if (*pc == 0x71) {
            uint32_t offset = *((uint32_t*)(pc + 1));

            printf("jle 0x%08x\n", offset);
            pc += 5;
        } else if (*pc == 0x72) {
            uint32_t offset = *((uint32_t*)(pc + 1));

            printf("jl 0x%08x\n", offset);
            pc += 5;
        } else if (*pc == 0x73) {
            uint32_t offset = *((uint32_t*)(pc + 1));

            printf("je 0x%08x\n", offset);
            pc += 5;
        } else if (*pc == 0x74) {
            uint32_t offset = *((uint32_t*)(pc + 1));

            printf("jne 0x%08x\n", offset);
            pc += 5;
        } else if (*pc == 0x75) {
            uint32_t offset = *((uint32_t*)(pc + 1));

            printf("jge 0x%08x\n", offset);
            pc += 5;
        } else if (*pc == 0x76) {
            uint32_t offset = *((uint32_t*)(pc + 1));

            printf("jg 0x%08x\n", offset);
            pc += 5;
        } else if (*pc == 0x80) {
            uint32_t offset = *((uint32_t*)(pc + 1));

            printf("call 0x%08x\n", offset);
            pc += 5;
        } else if (*pc == 0x90) {
            printf("ret\n");
            pc += 1;
        } else if (*pc == 0xa0) {
            printf("pushl\n");
            pc += 2;
        } else if (*pc == 0xb0) {
            printf("popl\n");
            pc += 2;
        } else if (*pc == 0xc0) {
            printf("readb\n");
            pc += 6;
        } else if (*pc == 0xc1) {
            printf("readl\n");
            pc += 6;
        } else if (*pc == 0xd0) {
            printf("writeb\n");
            pc += 6;
        } else if (*pc == 0xd1) {
            printf("writel\n");
            pc += 6;
        } else if (*pc == 0xe0) {
            printf("movsbl\n");
            pc += 6;
        } else {
            printf("INS\n");
            return;
        }
    }
}
