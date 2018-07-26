#include "stdio.h"
#include "mix_computer.h"

void clean_word(mix_word *word) {
    word->sign = MIX_POSITIVE;
    word->word.A1.data = 0;
    word->word.A2.data = 0;
    word->word.C.data = 0;
    word->word.F.data = 0;
    word->word.I.data = 0;
}

void init_mix_word(mix_word *w, unsigned int sign, unsigned int data) {
    clean_word(w);
    w->sign = sign;
    convert_data_to_bytes(w, data);
}

void print_word(mix_word *word) {
    printf("%c %02d %02d %02d %02d %02d \n",
           word->sign == MIX_POSITIVE ? '+' : '-',
           word->word.A1.data,
           word->word.A2.data,
           word->word.I.data,
           word->word.F.data,
           word->word.C.data
           );
}

void print_memory(mix_word *memory) {
    int i;
    for (i = 0; i < MEMORY_SIZE; i++) {
        printf("%04d: ", i);
        print_word(&memory[i]);
    }
}

void print_memory(mix_word *memory, int index) {
    if (index >= 0 && index < MEMORY_SIZE) {
        printf("%04d: ", index);
        print_word(&memory[index]);
    }
}

void initMemory(mix_word *memory) {
   int i;
   for (i = 0; i < MEMORY_SIZE; i++) {
       clean_word(&memory[i]);
   }
}

void init_computer(mix_computer *computer) {
    initMemory(computer->memory);

    computer->r1.bytes.sign = MIX_POSITIVE;
    computer->r2.bytes.sign = MIX_POSITIVE;
    computer->r3.bytes.sign = MIX_POSITIVE;
    computer->r4.bytes.sign = MIX_POSITIVE;
    computer->r5.bytes.sign = MIX_POSITIVE;
    computer->r6.bytes.sign = MIX_POSITIVE;
    computer->rj.bytes.sign = MIX_POSITIVE;


    computer->rA.sign = MIX_POSITIVE;
    computer->rX.sign = MIX_POSITIVE;


    computer->overflow = 0;
    computer->comparison.E = 0;
    computer->comparison.L = 0;
    computer->comparison.G = 0;
}

void print_computer(mix_computer *computer) {
    printf("rA: ");
    print_word(&computer->rA);
    printf("rX: ");
    print_word(&computer->rX);

    printf("r1: ");
    print_word(&computer->r1.word);
    printf("r2: ");
    print_word(&computer->r2.word);
    printf("r3: ");
    print_word(&computer->r3.word);
    printf("r4: ");
    print_word(&computer->r4.word);
    printf("r5: ");
    print_word(&computer->r5.word);
    printf("r6: ");
    print_word(&computer->r6.word);
    printf("rj: ");
    print_word(&computer->rj.word);
    printf("\n");
    printf("Instruction Index: %d\n", computer->instructionIndex);

    printf("overflow: %d\n", computer->overflow);
    printf("comparison: E: %d G: %d L: %d\n",
           computer->comparison.E,
           computer->comparison.G,
           computer->comparison.L);

}

mix_register convert_int_to_register(int d) {
    unsigned int p1 = d & 0b111111;
    unsigned int p2= (d & 0b111111000000) >> 6;
    mix_register result;
    result.bytes.first.data = p1;
    result.bytes.second.data = p2;

    return result;
}

int convert_register_to_int(mix_register r) {
    int result;
    result = r.bytes.first.data | (r.bytes.second.data << 6);
    if (r.bytes.sign == MIX_NEGATIVE) {
        result *= -1;
    }
    return result;
}

unsigned int get_part_of_byte(unsigned int byte, unsigned int part) {
    unsigned int result = 0;
    switch (part) {
    case 1: {
        result = (byte & 0x3F);
    }break;
    case 2: {
        result = (byte & 0xFC0) >> 6;
    }break;
    case 3: {
        result = (byte & 0x3F000) >> 12;
    }break;
    case 4: {
        result = (byte & 0xFC0000) >> 18;
    }break;
    case 5: {
        result = (byte & 0x3F000000) >> 24;
    }break;
    }
    return result;
}

unsigned int get_byte_from_parts(unsigned int p1, unsigned int p2, unsigned int p3, unsigned int p4, unsigned int p5) {
    return ((p1 << 24) | (p2 << 18) | (p3 << 12) | (p4 << 6) | p5);
}

void convert_data_to_bytes(mix_word *r, unsigned int data) {
    r->word.A1.data = get_part_of_byte(data, 5);
    r->word.A2.data = get_part_of_byte(data, 4);
    r->word.I.data = get_part_of_byte(data, 3);
    r->word.F.data = get_part_of_byte(data, 2);
    r->word.C.data = get_part_of_byte(data, 1);
}

unsigned int convert_bytes_to_data(mix_word *r) {
    unsigned int result;
    result = get_byte_from_parts(r->word.A1.data,
                                    r->word.A2.data,
                                    r->word.I.data,
                                    r->word.F.data,
                                    r->word.C.data);

    if (r->sign == MIX_NEGATIVE) {
        result *= -1;
    }

    return result;
}
