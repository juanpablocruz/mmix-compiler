#ifndef MIX_COMPUTER_H
#define MIX_COMPUTER_H

#define MIX_POSITIVE 1
#define MIX_NEGATIVE 0
#define MEMORY_SIZE 4000

#define BUFSIZE 1024
#define MAX_CODE_LINES 1000
#define TRUE 1
#define FALSE 0

#define MAX_WORD_SIZE 1<<30
#define MAX_BYTE_SIZE 1<<6

typedef struct {
    unsigned int data:6;
    unsigned int :26;
} mix_byte;

typedef struct {
    unsigned int sign:1;
    struct {
        mix_byte A1;
        mix_byte A2;
        mix_byte I;
        mix_byte F;
        mix_byte C;
    } word;

} mix_word;

typedef struct {
    int operation;
    unsigned int v;

    mix_byte index;
    mix_byte mod;
} mix_instruction;

typedef struct {
    mix_instruction instruction;
    char * label;
    char * argument;
    int hasLabel:1;
    int hasArgument;
    int hasInstruction;
    unsigned int address;
} mix_code_instruction;

/*
 * A register can store either a word or two bytes and sign
*/
typedef union {
    struct {
        unsigned int sign:1;
        mix_byte first;
        mix_byte second;
    } bytes;

    mix_word word;
} mix_register;

typedef struct {
    char *label;
    unsigned int address;
    mix_code_instruction *instruction;
} label_address;

typedef struct {
    char* name;
    unsigned int value;
} mix_variable;

typedef struct {
    unsigned int index;
    mix_code_instruction *instruction;
} mix_address_pending;

typedef struct {
    mix_word rA;
    mix_word rX;
    mix_register rj, r1, r2, r3, r4, r5, r6;
    mix_word memory[MEMORY_SIZE];

    unsigned int overflow:1;

    struct {
        unsigned int E:1;
        unsigned int G:1;
        unsigned int L:1;
    }comparison;

    unsigned int instructionIndex;

    label_address* labels_computed;
    unsigned int labels_count;

    mix_variable* computer_variables;
    unsigned int variables_count;

    mix_address_pending *pending_change;
    unsigned int pending_change_count;
} mix_computer;


void clean_word(mix_word *word);
void init_mix_word(mix_word *w, unsigned int sign, unsigned int data);
void print_word(mix_word *word);
void print_memory(mix_word *memory);
void print_memory(mix_word *memory, int index);
void initMemory(mix_word *memory);
void init_computer(mix_computer *computer);
void print_computer(mix_computer *computer);

mix_register convert_int_to_register(int d);
int convert_register_to_int(mix_register r);
void convert_data_to_bytes(mix_word *r, unsigned int data);
unsigned int convert_bytes_to_data(mix_word *r);
unsigned int get_part_of_byte(unsigned int byte, unsigned int part);
unsigned int get_byte_from_parts(unsigned int p1, unsigned int p2, unsigned int p3, unsigned int p4, unsigned int p5);

#endif // MIX_COMPUTER_H
