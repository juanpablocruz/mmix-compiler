#ifndef MIX_INSTRUCTIONS_H
#define MIX_INSTRUCTIONS_H

#include "mix_computer.h"

#define NO_OP_FOUND -1

// ARITHMETIC OPERATIONS
#define ADD 1
#define SUB 2
#define MUL 3
#define DIV 4

// LOAD OPERATIONS
#define LDA 8
#define LD1 9
#define LD2 10
#define LD3 11
#define LD4 12
#define LD5 13
#define LD6 14
#define LDX 15
#define LDAN 16
#define LD1N 17
#define LD2N 18
#define LD3N 19
#define LD4N 20
#define LD5N 21
#define LD6N 22
#define LDXN 23

// STORE OPERATIONS
#define STA 24
#define ST1 25
#define ST2 26
#define ST3 27
#define ST4 28
#define ST5 29
#define ST6 30
#define STX 31

// ADDRESS TRANSFER OPERATIONS
#define INCA 48 // MOD 0
#define INC1 49 // MOD 0
#define INC2 50 // MOD 0
#define INC3 51 // MOD 0
#define INC4 52 // MOD 0
#define INC5 53 // MOD 0
#define INC6 54 // MOD 0
#define INCX 55 // MOD 0

#define DECA 48 // MOD 1
#define DEC1 49 // MOD 1
#define DEC2 50 // MOD 1
#define DEC3 51 // MOD 1
#define DEC4 52 // MOD 1
#define DEC5 53 // MOD 1
#define DEC6 54 // MOD 1
#define DECX 55 // MOD 1

#define ENTA 48 // MOD 2
#define ENT1 49 // MOD 2
#define ENT2 50 // MOD 2
#define ENT3 51 // MOD 2
#define ENT4 52 // MOD 2
#define ENT5 53 // MOD 2
#define ENT6 54 // MOD 2
#define ENTX 55 // MOD 2

#define ENNA 48 // MOD 3
#define ENN1 49 // MOD 3
#define ENN2 50 // MOD 3
#define ENN3 51 // MOD 3
#define ENN4 52 // MOD 3
#define ENN5 53 // MOD 3
#define ENN6 54 // MOD 3
#define ENNX 55 // MOD 3

// Comparison operators
#define CMPA 56
#define CMP1 57
#define CMP2 58
#define CMP3 59
#define CMP4 60
#define CMP5 61
#define CMP6 62
#define CMPX 63

// Jump operators
#define JMP 39 // MOD 0
#define JSJ 39 // MOD 1
#define JOV 39 // MOD 2
#define JNOV 39 // MOD 3
#define JL 39 // MOD 4
#define JE 39 // MOD 5
#define JG 39 // MOD 6
#define JGE 39 // MOD 7
#define JNE 39 // MOD 8
#define JLE 39 // MOD 9

#define JAN 40 // MOD 0
#define JAZ 40 // MOD 1
#define JAP 40 // MOD 2
#define JANN 40 // MOD 3
#define JANZ 40 // MOD 4
#define JANP 40 // MOD 5
#define JAE 40 // MOD 6
#define JAO 40 // MOD 7

#define JXN 47 // MOD 0
#define JXZ 47 // MOD 1
#define JXP 47 // MOD 2
#define JXNN 47 // MOD 3
#define JXNZ 47 // MOD 4
#define JXNP 47 // MOD 5
#define JXE 47 // MOD 6
#define JXO 47 // MOD 7

#define J1N 41 // MOD 0
#define J1Z 41 // MOD 1
#define J1P 41 // MOD 2
#define J1NN 41 // MOD 3
#define J1NZ 41 // MOD 4
#define J1NP 41 // MOD 5

#define J2N 42 // MOD 0
#define J2Z 42 // MOD 1
#define J2P 42 // MOD 2
#define J2NN 42 // MOD 3
#define J2NZ 42 // MOD 4
#define J2NP 42 // MOD 5

#define J3N 43 // MOD 0
#define J3Z 43 // MOD 1
#define J3P 43 // MOD 2
#define J3NN 43 // MOD 3
#define J3NZ 43 // MOD 4
#define J3NP 43 // MOD 5

#define J4N 44 // MOD 0
#define J4Z 44 // MOD 1
#define J4P 44 // MOD 2
#define J4NN 44 // MOD 3
#define J4NZ 44 // MOD 4
#define J4NP 44 // MOD 5

#define J5N 45 // MOD 0
#define J5Z 45 // MOD 1
#define J5P 45 // MOD 2
#define J5NN 45 // MOD 3
#define J5NZ 45 // MOD 4
#define J5NP 45 // MOD 5

#define J6N 46 // MOD 0
#define J6Z 46 // MOD 1
#define J6P 46 // MOD 2
#define J6NN 46 // MOD 3
#define J6NZ 46 // MOD 4
#define J6NP 46 // MOD 5

// I/O Operators
#define JBUS 34 // MOD I/O unit
#define IOC 35 // MOD I/O unit
#define IN 36 // MOD I/O unit
#define OUT 37 // MOD I/O unit
#define JRED 38 // MOD I/O unit

// Conversion operators
#define NUM 5 // MOD 0
#define CHAR 5 // MOD 1

// Shift operators
#define SLA 6 // MOD 0
#define SRA 6 // MOD 1
#define SLAX 6 // MOD 2
#define SRAX 6 // MOD 3
#define SLC 6 // MOD 4
#define SRC 6 // MOD 5
#define SLB 6 // MOD 6
#define SRB 6 // MOD 7

// Miscellaneous operators
#define MOVE 7 // MOD no. of words
#define NOP 0 // MOD 0
#define HLT 5 // MOD 2
#define ORIG 0 // MOD 1
#define EQU 100

typedef struct{
    char range[6];
} mix_range;

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
} mix_code_instruction;


int get_from_range(int range);
int get_to_range(int range);

void convert_int_to_range(int range, mix_range *str_range);
unsigned int convert_range_to_int(mix_range *str_range);

mix_instruction parse_instruction_code(const char *function);
void parse_instruction(mix_code_instruction *instruction);

// Load
int load_to_register(mix_register *dest, mix_word orig);
int load_to_register(mix_register *dest, mix_word orig, unsigned int mod);

int load_to_register(mix_word *dest, mix_register orig);
int load_to_register(mix_word *dest, mix_register orig, unsigned int mod);

int load_to_register(mix_word *dest, mix_word orig);
int load_to_register(mix_word *dest, mix_word orig, unsigned int mod);

// Arithmetic
int add_to_register(mix_word *dest, mix_word orig);
int mult_registers(mix_computer *computer, mix_word orig);
int div_registers(mix_computer *computer, mix_word orig);

int compare_registers(mix_word a, mix_word b, mix_computer *computer);

void move_n_words(mix_word *dest, mix_word from, unsigned int n);

// Shift
void shift_register_left_circular(mix_word *r);
void shift_register_right_circular(mix_word *r);
void shift_two_registers_right(mix_word *r1, mix_word *r2);
void shift_two_registers_left(mix_word *r1, mix_word *r2);
void shift_register_right(mix_word *r);
void shift_register_left(mix_word *r);

// Cast
void conver_register_to_num(mix_word *r1, mix_word *r2);
void convert_registers_to_char(mix_word *r1, mix_word *r2);

unsigned int increase_word(mix_word *w, int c);
unsigned int increase_register(mix_register *r, int c);


void jum_if_word_negative(mix_computer *computer, mix_word *w, unsigned int registerIndex);
void jum_if_word_positive(mix_computer *computer, mix_word *w, unsigned int registerIndex);
void jum_if_word_zero(mix_computer *computer, mix_word *w, unsigned int registerIndex);
void jum_if_word_no_negative(mix_computer *computer, mix_word *w, unsigned int registerIndex);
void jum_if_word_no_positive(mix_computer *computer, mix_word *w, unsigned int registerIndex);
void jum_if_word_no_zero(mix_computer *computer, mix_word *w, unsigned int registerIndex);
void jum_if_word_even(mix_computer *computer, mix_word *w, unsigned int registerIndex);
void jum_if_word_odd(mix_computer *computer, mix_word *w, unsigned int registerIndex);

void copy_word(mix_word *from, mix_word *to);

#endif // MIX_INSTRUCTIONS_H
