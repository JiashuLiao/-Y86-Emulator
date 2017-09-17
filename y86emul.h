uint8_t* memory = NULL;
uint8_t* code_ends = NULL;
uint8_t* pc = NULL;

uint32_t mem_size = 0;
uint32_t text_length = 0;

// registers
uint32_t reg[8];

uint8_t OF = 0;
uint8_t SF = 0;
uint8_t ZF = 0;


void do_size(const char* text) {
    sscanf(text, "%x", &mem_size);
    memory = (uint8_t* )malloc(mem_size);
}

void do_long(const char* address, const char* value ) {
    uint32_t addr = 0;
    uint32_t va = 0;
    sscanf(address, "%x", &addr);
    sscanf(value, "%x", &va);
    *((uint32_t*)(memory + addr)) = va;
}

void do_byte(const char* address, const char * value) {
    uint32_t addr = 0;
    uint32_t va = 0;
    sscanf(address, "%x", &addr);
    sscanf(value, "%x", &va);
    *(memory + addr) = va;
}

void do_string(const char* address, const char * value) {
    uint32_t addr = 0;
    sscanf(address, "%x", &addr);
    strcpy((char*)(memory + addr), value );
}

void do_text(const char* address, const char* value) {
    uint32_t addr = 0;
    sscanf(address, "%x", &addr);

    uint8_t* tp = memory + addr;
    pc = tp;

    text_length = strlen(value);
    code_ends = memory + addr + text_length / 2;
    int i;
    for (i =0 ; i < text_length; i += 2) {
        *tp = 0;
        if (value[i] >= '0' && value[i] <= '9') {
            uint8_t va = value[i] - '0';
            (*tp) ^= (va << 4);
        } else {
            uint8_t va = value[i] - 'a' + 0x0a;
            (*tp) ^= (va << 4);
        }
        if (value[i + 1] >= '0' && value[i + 1] <= '9') {
            uint8_t va = value[i + 1] - '0';
            (*tp) ^= va;
        } else {
            uint8_t va = value[i + 1] - 'a' + 0x0a;
            (*tp) ^= va;
        }
        tp++;
    }
}

void load_program(const char* filename) {
    FILE * fp = NULL;

    fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "open file error\n");
        return;
    }

    char buffer[1024] = { '\0' };
    char directive[14] = { '\0' };
    char address[10] = { '\0' };
    char value[1000] = { '\0' };

    while (!feof(fp)) {
        fgets(buffer, 1024, fp);
        sscanf(buffer, "%s%s%s", directive, address, value);

        if (strcmp(directive, ".size") == 0) {
            do_size(address);
        } else if (strcmp(directive, ".string") == 0) {
            int i = 0;

            while (buffer[i] != '\"') {
                i++;
            }
            i++;

            int j = 0;
            while (buffer[i] != '\"' ) {
                value[j] = buffer[i];
                i++, j++;
            }
            value[j] = '\0';

            do_string(address, value);
        } else if (strcmp(directive, ".byte") == 0) {
            do_byte(address, value);
        } else if (strcmp(directive, ".long") == 0) {
            do_long(address, value);
        } else if (strcmp(directive, ".text") == 0) {
            do_text(address, value);
        }
    }

    fclose(fp);
}
