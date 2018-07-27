#ifndef MIX_COMPILER_H
#define MIX_COMPILER_H

#include "mix_computer.h"
#include "mix_instructions.h"

int execute_instruction(mix_computer *computer, mix_code_instruction instruction);
int execute_code(mix_computer *computer);
mix_code_instruction process_code_line(char** code);
mix_code_instruction* parse_code(char ** code_buffer, int lines);

void store_instruction_in_memory( mix_word *r, mix_code_instruction instruction);
void get_instruction_from_memory(mix_word r, mix_code_instruction *instruction);

void compile_code(mix_computer *computer,char **code_buffer, unsigned int lines);


#endif // MIX_COMPILER_H
