#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "y86emul.h"

uint32_t stack[1024];
uint32_t sp = 1024;

uint8_t* callstack[1024];
uint32_t top = 1024;

void emulator();


int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage:\ty86emul <inputfile>\n");
        return 0;
    }
    if (strcmp(argv[1], "-h") == 0) {
        printf("Usage:\ty86emul <inputfile>\n");
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

    emulator();
    free(memory);

    return 0;
}

void emulator() {

    while (pc < code_ends) {

        if (*pc == 0x00) {
            pc += 1;
        } else if (*pc == 0x10) {
            pc += 1;
            return ;
        } else if (*pc == 0x20) {
            uint8_t va = 0;
            uint8_t rb = 0;
            uint8_t ra = 0;

            va = *(pc + 1);
            ra = (va & 0xf0) >> 4;
            rb = va & 0x0f;

            reg[rb] = reg[ra];
            pc += 2;
        } else if (*pc == 0x30) {
            uint8_t va = 0;
            uint8_t rb = 0;

            va = *(pc + 1);
            rb = va & 0x0f;

            reg[rb] = *((uint32_t*)(pc + 2));

            pc += 6;
        } else if (*pc == 0x40) {
            uint8_t va = 0;
            uint8_t rb = 0;
            uint8_t ra = 0;

            va = *(pc + 1);
            ra = (va & 0xf0) >> 4;
            rb = va & 0x0f;

            *((uint32_t*)(memory + reg[rb])) = reg[ra];

            pc += 6;
        } else if (*pc == 0x50) {
            uint8_t va = 0;
            uint8_t rb = 0;
            uint8_t ra = 0;

            va = *(pc + 1);
            ra = (va & 0xf0) >> 4;
            rb = va & 0x0f;

            reg[ra] = *((uint32_t*)(memory + reg[rb]));

            pc += 6;
        } else if (*pc == 0x60) {
            uint8_t va = 0;
            uint8_t rb = 0;
            uint8_t ra = 0;

            va = *(pc + 1);
            ra = (va & 0xf0) >> 4;
            rb = va & 0x0f;

            reg[rb] += reg[ra];

            if (reg[rb] == 0) {
                ZF = 1;
            } else {
                ZF = 0;
            }
            if (reg[rb] < 0) {
                SF = 1;
            } else {
                SF = 0;
            }

            pc += 2;
        } else if (*pc == 0x61) {
            uint8_t va = 0;
            uint8_t rb = 0;
            uint8_t ra = 0;

            va = *(pc + 1);
            ra = (va & 0xf0) >> 4;
            rb = va & 0x0f;
            reg[rb] -= reg[ra];

            if (reg[rb] == 0) {
                ZF = 1;
            } else {
                ZF = 0;
            }
            if (reg[rb] < 0) {
                SF = 1;
            } else {
                SF = 0;
            }

            pc += 2;
        } else if (*pc == 0x62) {
            uint8_t va = 0;
            uint8_t rb = 0;
            uint8_t ra = 0;

            va = *(pc + 1);
            ra = (va & 0xf0) >> 4;
            rb = va & 0x0f;

            reg[rb] &= reg[ra];

            OF = 0;
            if(reg[ra] == 0){
                ZF = 1;
            } else {
                ZF = 0;
            }

            pc += 2;
        } else if (*pc == 0x63) {
            uint8_t va = 0;
            uint8_t rb = 0;
            uint8_t ra = 0;

            va = *(pc + 1);
            ra = (va & 0xf0) >> 4;
            rb = va & 0x0f;
            reg[rb] ^= reg[ra];

            OF = 0;

            pc += 2;
        } else if (*pc == 0x64) {
            uint8_t va = 0;
            uint8_t rb = 0;
            uint8_t ra = 0;

            va = *(pc + 1);
            ra = (va & 0xf0) >> 4;
            rb = va & 0x0f;

            reg[rb] *= reg[ra];

            pc += 2;
        } else if (*pc == 0x65) {
            uint8_t va = 0;
            uint8_t rb = 0;
            uint8_t ra = 0;

            va = *(pc + 1);
            ra = (va & 0xf0) >> 4;
            rb = va & 0x0f;

            int32_t test = reg[ra] - reg[rb];

            if (test == 0) {
                ZF = 1;
            } else {
                ZF = 0;
            }
            if (test < 0) {
                SF = 1;
            } else {
                SF = 0;
            }

            pc += 2;
        } else if (*pc == 0x70) {

            uint32_t offset = *((uint32_t*)(pc + 1));
            pc = memory + offset;

        } else if (*pc == 0x71) {
            uint32_t offset = *((uint32_t*)(pc + 1));

            if (SF != OF || ZF == 1) {
                pc = memory + offset;
            } else {
                pc += 5;
            }
        } else if (*pc == 0x72) {
            uint32_t offset = *((uint32_t*)(pc + 1));

            if (SF != OF) {
                pc = memory + offset;
            } else {
                pc += 5;
            }

        } else if (*pc == 0x73) {
            uint32_t offset = *((uint32_t*)(pc + 1));

            if (ZF == 1) {
                pc = memory + offset;
            } else {
                pc += 5;
            }
        } else if (*pc == 0x74) {
            uint32_t offset = *((uint32_t*)(pc + 1));

            if (ZF == 0) {
                pc = memory + offset;
            } else {
                pc += 5;
            }
        } else if (*pc == 0x75) {
            uint32_t offset = *((uint32_t*)(pc + 1));

            if (SF == OF) {
                pc = memory + offset;
            } else {
                pc += 5;
            }
        } else if (*pc == 0x76) {
            uint32_t offset = *((uint32_t*)(pc + 1));

            if (SF == OF && ZF == 0) {
                pc = memory + offset;
            } else {
                pc += 5;
            }
        } else if (*pc == 0x80) {
            uint32_t offset = *((uint32_t*)(pc + 1));

            top--;
            callstack[top] = pc;

            pc = memory + offset;
        } else if (*pc == 0x90) {
            pc = callstack[top];
            top++;
            pc += 5;
        } else if (*pc == 0xa0) {
            uint8_t va = 0;
            uint8_t ra = 0;
            va = *(pc + 1);
            ra = (va & 0xf0) >> 4;

            sp--;
            reg[4] -= 4;

            stack[sp] = reg[ra];
            pc += 2;
        } else if (*pc == 0xb0) {
            uint8_t va = 0;
            uint8_t ra = 0;

            va = *(pc + 1);
            ra = (va & 0xf0) >> 4;

            reg[ra] = stack[sp];
            sp ++;
            reg[4] += 4;

            pc += 2;
        } else if (*pc == 0xc0) {
            uint8_t va = 0;
            uint8_t ra = 0;

            va = *(pc + 1);
            ra = (va & 0xf0) >> 4;

            scanf("%c", memory + reg[ra]);
            getchar();

            ZF = 0;
            SF = 0;
            OF = 0;

            pc += 6;
        } else if (*pc == 0xc1) {
            uint8_t va = 0;
            uint8_t ra = 0;

            va = *(pc + 1);
            ra = (va & 0xf0) >> 4;

            scanf("%d", (int32_t*)(memory + reg[ra]) );
            getchar();

            ZF = 0;
            SF = 0;
            OF = 0;

            pc += 6;
        } else if (*pc == 0xd0) {
            uint8_t va = 0;
            uint8_t ra = 0;

            va = *(pc + 1);
            ra = (va & 0xf0) >> 4;

            printf("%c", *(memory + reg[ra]));

            pc += 6;
        } else if (*pc == 0xd1) {
            uint8_t va = 0;
            uint8_t ra = 0;

            va = *(pc + 1);
            ra = (va & 0xf0) >> 4;

            printf("%d", *((int32_t* )(memory + reg[ra])));

            pc += 6;
        } else if (*pc == 0xe0) {
            uint8_t va = 0;
            uint8_t rb = 0;
            uint8_t ra = 0;

            va = *(pc + 1);
            ra = (va & 0xf0) >> 4;
            rb = va & 0x0f;

            if (mem_size < reg[rb]) {
                printf("ADR\n");
                return;
            }

            reg[ra] = *((int*)(memory + reg[rb]));

            pc += 6;
        } else {
            printf("INS\n");
            return;
        }
    }
}
